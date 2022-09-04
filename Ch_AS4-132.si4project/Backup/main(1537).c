/*-------------------------------------------------------------------------*/
#include "config.h"

#include "lwip/init.h"
#include "lwip/tcp.h"
#include "netif/etharp.h"
#if LWIP_VERSION_2
#include "lwip/timeouts.h"
#include "lwip/priv/tcp_priv.h"
#endif
#include "tcp_server.h"
#include "netconf.h"
#include "debug.h"

#include "EEPROM.h"

#include "RS422_if.h"
#include "adc_if_AS.h"

#include "net_AS.h"
#include "LcasAction_task.h"
#include "MotorCtrl_task.h"

#include "LcasAction_task.h"
#include "MotorCtrl_task.h"
#include "led_task.h"
#include "PID_Control_AS.h"

#if BOARD_TEST_PRGM || MACHINE_TEST_PRGM
#include "testPrgm.h"
#endif

/*==============================================================================
 * Global variables.
 */

extern uint32_t LocalTime;

uint8_t FrameReceived = 0;

extern volatile int TimeEvent;

volatile int TimeEvent_1ms = NO;		// pAction_Fn() 딜레이를 주기위한... 


#define TIME_100HZ_CNT		10				// 10msec
#define TIME_50HZ_CNT			20				// 20msec
#define TIME_20HZ_CNT			50				// 50msec
#define TIME_10HZ_CNT			100
#define TIME_WDT_RESET_CNT		20	// 10	

#define CNT_LWIP_PERIODIC_HANDLE			1		// 첫번째
	#define CNT_LWIP_PERIODIC_CHK			5		// 20Hz
#define CNT_READY_POW_BTN_BASE				2
	#define CNT_CHECK_POW_BTN			20
#define CNT_ADC_PROCESS								3						
//#define CNT_4													4
#define CNT_SVCDATA_SEND_BASE					4
	#define CNT_SVCDATA_SEND		100
#define CNT_USED_TIME_SAVE_BASE			4
	#define CNT_USED_TIME_SAVE		100
#define CNT_EVENT_TIME_BASE						5
	#define CNT_EVENT_TIME			5				// 100HZ / 5 = 20Hz
#define CNT_STATE_CHECK_BASE					6
	#define CNT_STATE_CHECK			50 //100				// 100HZ / 100 = 1Hz
#define CNT_STATE_SEND_BASE						7
	#define CNT_STATE_SEND			100				// 100HZ / 100 = 1Hz
#define CNT_WATCHDOG_RTC_BASE					8
	#define CNT_WATCHDOG_RTC			100				// 100HZ / 100 = 1Hz
#define CNT_RS422_TSTAT_CHECK_BASE		9
	#define CNT_RS422_TSTAT_CHECK			5				// 100HZ / 5 = 20Hz
#define CNT_RESET_10									10 


extern GCSVC_ST gcsvc;
extern RS422_ST rs422if;
//extern struct tcp_server_struct *g_es;
extern ADC_ST adcif;

//extern LCAS_STATE_t LcasState;
extern LCAS_DEV_STATE_t devState;
//extern LCAS_RUN_SETUP_t runSetup;
//extern SYSTEM_CONFIG_t sysConfig;

//extern SYSTEM_INFORM_t sysInform;
//extern LCAS_USED_TIME_t usedTime;
extern LCAS_DEV_SET_t devSet;
//extern LCAS_SPECIAL_t special;
//extern ETHERNET_CONFIG_t ethernetConfig;


uint32_t pwnControl = 0;

uint32_t gbFabricRemoteInt = NO;
//uint32_t FabricGpioIntCnt = 0;

uint32_t remoteStartInCnt = 0;		// 100ms 단위
uint32_t remoteStartInTime = 0;		// 100ms 단위

uint32_t remoteStartOutSig = NO;	// 리모트 단자로 출력해야 된다.
uint32_t remoteStartOutCnt = 0;		
uint32_t remoteStartOutTime = 1;		// 100ms 단위

uint32_t gbRTC_Match = NO;


//void lwip_init(void);		// lwip : init.h
extern void netif_config(void);

#define RYU_TEST			0

int main()
{

	uint32_t cmd;

	uint32_t wdg_reset;
	uint32_t wdg_reset_cnt=0;		// 1sec
	uint32_t timeCnt10Hz = 0;	  
	uint32_t timeCnt20Hz = 0;	
	uint32_t timeCnt50Hz = 0;		
	uint32_t timeCnt100Hz = 0;			
	uint32_t eventTimeCnt = 0;

	uint32_t cnt_LwIP_Periodic_Handle = 0;

	uint32_t timeStateActCnt = 0;
	uint32_t timeSataeSendCnt = 0;
	uint32_t stateCheckCnt = 0;	
	uint32_t rs422TstatCheckCnt = 0;	
//	uint32_t sendSvcdataCnt = 0;
	uint32_t usedTimeSaveCnt = 0;

	uint32_t checkPowerBtnCnt = 0;

	uint32_t remoteIn_powerBtn = 0;

	uint32_t lwip_periodic_handle_Cnt = 0;

	uint8_t time[10];

uint32_t subMenu_ihpS1 = 1;		
uint32_t subMenu_ihpS2 = 1;		

uint32_t subMenu_goSam1 = 1;		
uint32_t subMenu_goSam2 = 1;		

//=====================================================
// initialize hardware
//=====================================================
  cpu_init();

	wdg_reset = MSS_WD_timeout_occured();
	if(wdg_reset)
	{
		MSS_WD_clear_timeout_event();
		iprintf("!!!!!! Watchdog Reset !!!!!\r\n");
	}

	init_system();
	InterruptEnable_Main();	
//=====================================================

//=====================================================
// initialize Network
//=====================================================
	loadEthernetConfig(0);

#if ryuSF
  init_mac();	xxx// Mac Address설정
#endif

//printf("--loadEthernetConfig()\r\n");	
// Sys_cfg.c - mac address
	lwip_init();//LwIP_Init();
iprintf("--lwip_init()\r\n");
	netif_config();
iprintf("--netif_config()\r\n");
	tcp_server_init(); //--> move to DHCP process
iprintf("--tcp_server_init()\r\n");

//	InterruptEnable_Main();
//=====================================================
//iprintf("--InterruptEnable_Main()\r\n");

#if RS422_ENABLE
	rs422_SetMyBoardId(RS422_MY_ID);
	rs422_init();
	InterruptEnable_rs422();
	rs422_PacketParse_init();

	rs422_make_SendData_Header();
#endif

  adc_init();
	pfAdcProcess = adc_process;

#if	RS422_ENABLE
	rs422if.connected = NO;
#endif
	devState.connected = NO;
	devState.disconnected = NO;

	netConnectAction();

#if 0

	if(1) defaultSetting();
	else loadSaveValue();

	initOvenConfig(DATA_LOAD);

	initStepCtrlValue();

#if INTERNAl_FAN_ALWAYS_ON
IntFanConOn(INTERNAL_FAN_SPEED_NORMAL);
#else
IntFanConOff();
#endif

#endif

	Init_rtcTime();

#if 1
	initValue();	

//	APC_Rs422_PacketParse_init();
//dp0("APC_Rs422_PacketParse_init\r\n");

	initSystemInform();

	initSystemConfig();

	initTempCalData();
	//		initRemoteConfig();
	initRunSetup();
	initSequence();

	initAuxEvntValue();

	initMixTable();
	initMixSetup();

	initDiagData();

	initSpecial(DATA_LOAD);

	usedTimeCheck(DATA_LOAD);

	initStepCtrlValue();

	readMotorState();

// 20210602
	eepromRead_Data(HOME_ADJUST_VERSION_CHECK_ADDR, HOME_ADJUST_VERSION_CHECK_SIZE,(uint8_t *)(&devSet.adjust_ver));
	eepromRead_Data(HOME_ADJUST_DATA_STARTADDR,sizeof(devSet.adjust),devSet.adjust.saveFlag);	
	checkAdjustData();
	
	initCoordinate();

#endif


	MSS_WD_reload();

  tcp_tmr();
  etharp_tmr();

//	PID_Control_Init();
//	pLed_Control_Fn = led_Control;
//	pLed_Control_Fn = led_Control_Blank;				

//	init_Send_func();

//dp("-----s1[%08X] s2[%08X] m3[%08X]\n", samState.btMotorState[MOTOR_SAM1], samState.btMotorState[MOTOR_SAM2], samState.btMotorState[MOTOR_M3]);
//	pLoop_Injection_Fn = full_LoopInjection;
//	pTime_Event_Fn = time_Event_Blank;

// =========================================================================================
// 초기화 작업 : ADC 초기화 작업 
// -----------------------------------------------------------------------------------------
	while(adcif.State != ADCST_RUN) {
		if(TimeEvent)		// 1msec
		{
			//TimeEvent = NO;
			pfAdcProcess();
		}
	}

	timeStateActCnt = 0;

//=================================================================================
// 함수 포인터 초기화
//=================================================================================
#if AUTO_TUNNING_CONTROL
	pPID_Control_Fn = pidAutoTunningInit;
#else
	pPID_Control_Fn = PID_Control_Init;
#endif

//	pLed_Control_Fn = led_Control;
	pSend_State_Fn = send_State_Blank;
	pSend_DiagData_Fn = send_DiagData_Blank;
	pLoop_Injection_Fn = full_LoopInjection;
	pTime_Event_Fn = time_Event_Blank;

	pLed_Control_Fn = led_Control;

#if BOARD_TEST_PRGM
	boardTest_Function_Set();
#elif MACHINE_TEST_PRGM
	machineTest_Function_Set();
#else
	pAction_Fn[AS_ACTION_NONE] = actionNone;				// AS_ACTION_NONE
	pAction_Fn[AS_ACTION_INITILIZE] = actionInitilize;	// AS_ACTION_INITILIZE
	pAction_Fn[AS_ACTION_READY] = actionReady;				// AS_ACTION_READY
	pAction_Fn[AS_ACTION_MIX] = actionMix;					// AS_ACTION_MIX
	pAction_Fn[AS_ACTION_RUN] = actionRun;					// AS_ACTION_RUN
	pAction_Fn[AS_ACTION_FAULT] = actionFault;				// AS_ACTION_FAULT
	pAction_Fn[AS_ACTION_STANDBY] = actionStandby;			// AS_ACTION_STANDBY
	pAction_Fn[AS_ACTION_DIAGNOSTICS] = actionDiagnostics;	// AS_ACTION_DIAGNOSTICS
	pAction_Fn[AS_ACTION_ADJUST] = actionAdjust;				// AS_ACTION_ADJUST
	pAction_Fn[AS_ACTION_SERVICE_WASH] = actionServiceWash;	// AS_ACTION_SERVICE_WASH
	pAction_Fn[AS_ACTION_STOP_FUNCTION] = actionStopFunction;		// AS_ACTION_STOP_FUNCTION
/*
	pDiag_Fn[0] = actionDiagNone;
	pDiag_Fn[1] = actionDiagHeaterCheck;
	pDiag_Fn[2] = actionDiagCoolerCheck;
	pDiag_Fn[3] = actionDiagTempsenCheck;
*/
#endif

	LED_Act_Initiaize()	;

  for(;;)
  {
		if(TimeEvent)		// 1msec
		{
			TimeEvent = NO;
TimeEvent_1ms = YES;			

#if RS422_ENABLE
/** 수신보다 앞쪽에 위치하는 것이 ...*/
if(rs422if.TSTAT == 0)
{	// 처음엔 완료로 셋팅
	// 송신버퍼가 비워지지 않아서 보내지 못한 데이터가 있다.
	// Slave의 경우임.
	if(rs422if.selfMsgBufHead != rs422if.selfMsgBufTail) {
		rs422_selfMsgBuf[rs422if.selfMsgBufTail];
	}
	else if(rs422if.sendDataHead != rs422if.sendDataTail) {
		rs422if.sendDataInfo[rs422if.sendDataTail];

	}
	
}

			if(rs422if.rx_intr == 1) {	// RS422수신 인터럽트 발생
				rs422if.rx_intr = 0;
				rs422_PacketParse_Slave();
			}
#endif

			switch(++timeCnt100Hz) {
				case CNT_LWIP_PERIODIC_HANDLE:
#if LWIP_VERSION_2
					/* Handle timeouts */
					sys_check_timeouts();
#else
					if(++lwip_periodic_handle_Cnt == CNT_LWIP_PERIODIC_CHK) {
						lwip_periodic_handle_Cnt = 0;
						LwIP_Periodic_Handle(LocalTime);
					}
#endif
					break;
				case CNT_READY_POW_BTN_BASE:
				// RYU_SF2
					remoteIn_powerBtn = FPGA_READ_WORD(R6_SIGNAL_IN);	// R6_SIGNAL_IN  ---	[bit2: Power button] / [bit1: remote start] / [bit0:remote ready]

					remoteControl(remoteIn_powerBtn);

					if(++checkPowerBtnCnt == CNT_CHECK_POW_BTN) {
						checkPowerBtnCnt = 0;
						CheckPowerBtn(remoteIn_powerBtn);	// 5hz
					}
					break;
				case CNT_ADC_PROCESS:		// ADC init & Reading  ---> TempControlFunc() , OvenTempReadyCheck()
					pfAdcProcess();
					break;
				case CNT_USED_TIME_SAVE_BASE:	// case CNT_SVCDATA_SEND_BASE:
					if(++usedTimeSaveCnt == CNT_USED_TIME_SAVE) {
						usedTimeSaveCnt = 0;
						usedTimeSaveFunc();
					}
					break;
				case CNT_EVENT_TIME_BASE:
				// RYU_SF2  -- timer_100ms()
					devState.analysisTime100msCnt++;
					pTime_Event_Fn();
					pLed_Control_Fn();
					break;
				case CNT_STATE_CHECK_BASE:
					if(++stateCheckCnt >= CNT_STATE_CHECK) {	// 2Hz
						stateCheckCnt = 0;

						devState.diagTimeCount++;				// LcasAction_task.c

						if(devState.adReadValid) {
							CheckDeviceState();		// Power Button 관련 , Door , Magnetic switch , leak
						}
					}
					break;
				case CNT_STATE_SEND_BASE:
					if(++timeSataeSendCnt >= CNT_STATE_SEND) {	// 1Hz
						timeSataeSendCnt = 0;
						// RYU_SF2  -- timer_100ms()
						pSend_State_Fn();  // --> pSend_DiagData_Fn();
					}
					break;
// RS422 ======================================================
				case CNT_RS422_TSTAT_CHECK_BASE:
					if(++rs422TstatCheckCnt >= CNT_RS422_TSTAT_CHECK) {		// 20Hz
						if(rs422if.TSTAT == 0) 
						{	// 처음엔 완료로 셋팅
							// 송신버퍼가 비워지지 않아서 보내지 못한 데이터가 있다.
							// Slave의 경우임.
						}
					}
					break;
				case CNT_WATCHDOG_RTC_BASE:
					if(++wdg_reset_cnt >= CNT_WATCHDOG_RTC) {	// 1Hz
						wdg_reset_cnt =0;
						MSS_WD_reload();	// 1Sec 마다 실행할 것
//RemoteInCheck();

//						uint8_t time[10];
						rtcRead_time(RTC_SECONDS_REG_ADDR,time);
//						get_RtcTime();
//iprintf("devState.actionMenu = %d\n",devState.actionMenu);

#if 1
if(devState.save_EEPROM_sysConfig == YES) {
	if(save_EEPROM_sysConfig_data()) {
		devState.save_EEPROM_sysConfig = NO;
	}
}

if(devState.save_EEPROM_special == YES) {
	if(save_EEPROM_special_data()) {
		devState.save_EEPROM_special = NO;
	}
}
#endif

if(adcif.State == ADCST_RUN) {	// adc 초기화 완료된상태 
	// 온도데이터를 가지고 
	// MAC Address 생성등 
}

#if 0
static int k=0, n=0;   
if(++k == 60) {
	n++;
	iprintf("s%d\n",n);
	k=0;
}
#endif

#define RTC_TEST	0
#if RTC_TEST
						int i;
						static int j=0;
						static int fanPow = 0,fancnt = 0;
						static int odd = 0;
						if(++j == 5) {
							j = 0;
iprintf("rtc_time = "); for(i=0 ; i<7; i++) {	iprintf("%x : ",time[i]);	} iprintf("\n");
//	eepromRead_Data(HOME_ADJUST_VERSION_CHECK_ADDR, HOME_ADJUST_VERSION_CHECK_SIZE,&devSet.adjust_ver);
	
/*
if(odd == 0) {
	odd = 1;
	INTERNAL_FAN_OFF;
}
else {
	odd = 0;
	INTERNAL_FAN_ON;
}		
*/	
/*
if(++fancnt == 4) {
	fancnt = 0;
	FanConOn(fanPow);	// 25:동작안함.   30:동작(소음) 40:동작(소음) 60:동작(소음)
	fanPow = fanPow + 10;
	dp("fanPow = %d\n", fanPow);
}
*/		
						}
#endif
//dfp("s1[%08X] s2[%08X] m3[%08X]\n",samState.btMotorState[MOTOR_SAM1], samState.btMotorState[MOTOR_SAM2], samState.btMotorState[MOTOR_M3]);						
					}
					break;

				case CNT_RESET_10:
					timeCnt100Hz = 0;
					break;

				default:
					timeCnt100Hz = 0;
					break;
			}

/*
			if(++timeCnt20Hz >= TIME_20HZ_CNT) {	// 20Hz
				timeCnt20Hz = 0;
				eventCtrl.bTimeEvent = ON;

				if(++tempCtrlTickCnt == 4) {	// 5Hz 
					tempCtrlTickCnt = 0;

					if(++timeSataeSendCnt == 5) {	// 1Hz
						timeSataeSendCnt = 0;
						stateCheck();	// sensor error check
						pSend_State_Fn();
					}
				}

// RS422 ======================================================
#if 1		
				if(rs422if.TSTAT == 0) 
				{	// 처음엔 완료로 셋팅
					// 송신버퍼가 비워지지 않아서 다시 보내야 하는 경우다.
					// APC의 경우임.
				}
#endif

// WatchDog & RTC Time read ===================================
#if 1
				if(++wdg_reset_cnt == TIME_WDT_RESET_CNT) {	// 1Hz
					wdg_reset_cnt =0;
					MSS_WD_reload();	// 1Sec 마다 실행할 것

uint8_t time[10];
	rtcRead_time(RTC_SECONDS_REG_ADDR,time);
int i,j;
if(++j == 5) {
	j = 0;
//	dp("rtc_time = "); for(i=0 ; i<7; i++) {	dp("%x : ",time[i]);	} dp("\n");	
}
				}
#endif
			}
*/

		pAction_Fn[devState.actionMenu](0);		// Action();


//==========================================================================		
#if 0	// 테스트 - 터미널 입력
		if (((g_mss_uart0.hw_reg->LSR)&0x01) == 1 )
		{
			cmd = g_mss_uart0.hw_reg->RBR;
			Function_test(cmd);
			cmd = 'X';
		}
#endif
//==========================================================================					
		}// if(TimeEvent)

// RYU_SF2
#if 1
//		pAction_Fn[devState.actionMenu](0);		// Action();
		readMotorState();
		if(devSet.posCheckEnable == YES) checkPosition();
#endif

	}	//for(;;)

}


