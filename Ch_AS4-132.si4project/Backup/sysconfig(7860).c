
#include "sys_config_mss_clocks.h"
#include "config.h"

#include "EEPROM.h"
//#include "protocolType_oven.h"
//#include "protocolType_HPLC.h"

//#include "Ch_oven_type.h"
//#include "fpga_ChAS.h"
#include "conf_eth.h"

//#include "Motor_Ctrl.h"

#include "Led_task.h"


#define WDOG_SYSREG_CR_ENABLE_MASK      0x00000001u

#define FLAG_SERIAL_NO		0
#define FLAG_IP_ADDR			1
#define FLAG_MAC_ADDR			2

extern void uart_tx_handler(mss_uart_instance_t * this_uart);
extern void mss_uart1_rx_handler(mss_uart_instance_t * this_uart);

#define PWM_PRESCALE    32		// 35Mhz / 254 / 32 --> 4.3K
#define PWM_PERIOD      254 // Full���� 255�� ���ϱ� ���ؼ�  // 256	

extern ETHERNET_CONFIG_t ethernetConfig;
//extern Oven_State_t	ovenStt;	// ����������ݿ� ���� ��

extern SYSTEM_INFORM_t sysInform;

INFORMATION_t as_INFO;
STORAGE_SYS_INFO_T storageSysInfo;

//extern char ethAddr[6];
extern LCAS_DEV_SET_t devSet;


#define ETH_ADDR_LEN 6
#define IP_ADDR_LEN 4

// ryu
unsigned char my_ip[IP_ADDR_LEN] =	{
	ETHERNET_CONF_IPADDR0, 
	ETHERNET_CONF_IPADDR1,
	ETHERNET_CONF_IPADDR2,
	ETHERNET_CONF_IPADDR3
};
unsigned char my_mac[ETH_ADDR_LEN] = {
	ETHERNET_CONF_ETHADDR0,
	ETHERNET_CONF_ETHADDR1,
	ETHERNET_CONF_ETHADDR2,
	ETHERNET_CONF_ETHADDR3,
	ETHERNET_CONF_ETHADDR4,
//	ETHERNET_CONF_ETHADDR4
	ETHERNET_CONF_ETHADDR5
};
unsigned char my_netMask[IP_ADDR_LEN]	= { 
	ETHERNET_CONF_NET_MASK0,
	ETHERNET_CONF_NET_MASK1,
	ETHERNET_CONF_NET_MASK2,
	ETHERNET_CONF_NET_MASK3
};
unsigned char my_gateway[IP_ADDR_LEN] = { 
	ETHERNET_CONF_GATEWAY_ADDR0,
	ETHERNET_CONF_GATEWAY_ADDR1,
	ETHERNET_CONF_GATEWAY_ADDR2,
	ETHERNET_CONF_GATEWAY_ADDR3
};




void cpu_init(void)
{
	uint32_t timer1_load_value;

	  /* Turn off the watchdog */
	SYSREG->WDOG_CR = 0;
	
//=============================================================================================
// Configure UART0 for Debug.
//---------------------------------------------
// Newlib_stubs.c     
//#define MICROSEMI_STDIO_BAUD_RATE  MSS_UART_460800_BAUD     
//---------------------------------------------
// Register the function that will be called on UART0 receive interrupts.
// ���ͷ�Ʈ�� ����ϰ��� �� ���� ���� �ɺ��� �������Ѵ�.
// Property-->Setting-->Symbols--> MICROSEMI_STDIO_THRU_MMUART0 //ACTEL_STDIO_THRU_UART  
// MICROSEMI_STDIO_THRU_MMUART0
//---------------------------------------------
#if 1
  MSS_UART_init(&g_mss_uart0,
//  							MSS_UART_460800_BAUD,
  							MSS_UART_115200_BAUD,
                MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

//	MSS_UART_set_rx_handler(&g_mss_uart0, mss_uart0_rx_handler,MSS_UART_FIFO_SINGLE_BYTE /* MSS_UART_FIFO_FOUR_BYTES */);
//=========================================================================
#endif

//=============================================================================================
// Configure UART1
//---------------------------------------------
#if 0
	MSS_UART_init(&g_mss_uart1,
								MSS_UART_460800_BAUD, // MSS_UART_230400_BAUD , // //MSS_UART_115200_BAUD,
                MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);
	MSS_UART_set_rx_handler(&g_mss_uart1, mss_uart1_rx_handler,MSS_UART_FIFO_SINGLE_BYTE /* MSS_UART_FIFO_FOUR_BYTES */);

//	MSS_UART_enable_irq(&g_mss_uart1,(MSS_UART_RBF_IRQ));
//	MSS_UART_set_tx_handler(&g_mss_uart1, uart_tx_handler);
//	MSS_UART_set_tx_handler(gp_my_uart, uart_tx_handler);	
//=========================================================================
#endif

//=============================================================================================
// Configure Timer1.
//---------------------------------------------
	timer1_load_value = MSS_SYS_APB_0_CLK_FREQ / 1000;	// 1ms Timer Value
	MSS_TIM1_init( MSS_TIMER_PERIODIC_MODE );
	MSS_TIM1_load_immediate( timer1_load_value );
	MSS_TIM1_start();
	MSS_TIM1_enable_irq();
//=========================================================================

  SYSREG->WDOG_CR = WDOG_SYSREG_CR_ENABLE_MASK;
  /* Watchdog initialization. */
  MSS_WD_init();
  MSS_WD_enable_timeout_irq();

//	NVIC_SetPriority(EthernetMAC_IRQn, 5u);
//	NVIC_SetPriority(EthernetMAC_IRQn, 0u);	// RYU_TCP			
//	NVIC_SetPriority(EthernetMAC_IRQn, 1u);	// RYU_TCP			

}

#if 0
void cpu_init_Main(void)
{

	NVIC_SetPriority(EthernetMAC_IRQn, 5u);
//	NVIC_SetPriority(EthernetMAC_IRQn, 0u);	// RYU_TCP			

//=============================================================================================
// Configure I2C for RTC(DS1308)
//---------------------------------------------
	//MSS_I2C_init( I2C_MASTER, MASTER_SER_ADDR, MSS_I2C_PCLK_DIV_160 );
//=========================================================================

//=============================================================================================
// Configure SPI for FRAM,  etc
//---------------------------------------------
//	spi_sd_init_Main();
//=========================================================================
}
#endif

void InterruptEnable_Main(void)
{

// �켱���� 5 : ��Ŷ�� ��ġ�� ��찡 �����.  - printf ���� �����ϴ� ���� �����. - orignal
// �켱���� 1 : ��Ŷ �� ��ģ��.
// �켱����01 : RYU_TCP EEPROM ����� ������ �߻�(����)

// ������ �켱������ ã�ƾ� 
// The default priority is 0 for every interrupt. 
// This is the highest possible priority.
	NVIC_SetPriority(EthernetMAC_IRQn, 5u);		// ��Ŷ�� ��ġ�� ��찡 �����.  - printf ���� �����ϴ� ���� �����.
//	XXX NVIC_SetPriority(EthernetMAC_IRQn, 0u);	// RYU_TCP EEPROM ����� ������ �߻�(����)
//	NVIC_SetPriority(EthernetMAC_IRQn, 1u);	// RYU_TCP

// ADC Auto Sampling interrupt enable ----------
    NVIC_EnableIRQ( FabricIrq1_IRQn );
    NVIC_ClearPendingIRQ( FabricIrq1_IRQn );
// START_IN : remote
    NVIC_EnableIRQ( FabricIrq2_IRQn );
    NVIC_ClearPendingIRQ( FabricIrq2_IRQn );

// RS422 Rx intr
//	NVIC_EnableIRQ( FabricIrq0_IRQn );
//	NVIC_ClearPendingIRQ( FabricIrq0_IRQn );

// (Master) rs422 Auto Completion interrupt enable ----------
//	NVIC_EnableIRQ( FabricIrq3_IRQn );
//	NVIC_ClearPendingIRQ( FabricIrq3_IRQn );

#if 0
    NVIC_EnableIRQ( FabricIrq4_IRQn );
    NVIC_ClearPendingIRQ( FabricIrq4_IRQn );

    NVIC_EnableIRQ( FabricIrq5_IRQn );
    NVIC_ClearPendingIRQ( FabricIrq5_IRQn );

// Key In
    NVIC_EnableIRQ( FabricIrq6_IRQn );
    NVIC_ClearPendingIRQ( FabricIrq6_IRQn );

// RTC
    NVIC_EnableIRQ( FabricIrq7_IRQn );
    NVIC_ClearPendingIRQ( FabricIrq7_IRQn );

    NVIC_EnableIRQ( FabricIrq8_IRQn );
    NVIC_ClearPendingIRQ( FabricIrq8_IRQn );
#endif		
}

#define POWERDOWN_SET_VALUE		1
#define SLEEP_SET_VALUE				0

#define MOTOR_POWERDOWN_MODE_SET_SAM1	(1 << 0)
#define MOTOR_POWERDOWN_MODE_SET_SAM2	(1 << 1)
#define MOTOR_POWERDOWN_MODE_SET_M3	(1 << 2)
#define MOTOR_POWERDOWN_MODE_SET_M4	(1 << 3)
#define MOTOR_POWERDOWN_MODE_SET_M5	(1 << 4)
#define MOTOR_POWERDOWN_MODE_SET_M6	(1 << 5)

#define MOTOR_SLEEP_MODE_SET_SAM1	(0 << 0)
#define MOTOR_SLEEP_MODE_SET_SAM2	(0 << 1)
#define MOTOR_SLEEP_MODE_SET_M3	(0 << 2)
#define MOTOR_SLEEP_MODE_SET_M4	(0 << 3)
#define MOTOR_SLEEP_MODE_SET_M5	(0 << 4)
#define MOTOR_SLEEP_MODE_SET_M6	(0 << 5)

void init_Control_port() // init_CoreGpio();
{
	unsigned int motorPowerModeData = 0x0;

#if 0		// CoreGpio��� 
// ����� FPGA���� in/out�� �������� ����ϰ� �ִ�.
// GPIO_init()�� ����ϸ� �ʱ�ȭ ������ �ȴ�.
//GPIO_init( &g_gpio,	COREGPIO_BASE_ADDR, GPIO_APB_32_BITS_BUS );  <== ��Ʈ �ʱ�ȭ �� 
 		g_gpio.base_addr = COREGPIO_APBBIF_0_BASE_ADDR;
    g_gpio.apb_bus_width = GPIO_APB_32_BITS_BUS;
#endif

// ���� bit ���� 
// [27:22] ==> [Inj:Val:Syr:X:Y:Z] 
#if MOTOR_POWER_DOWN_MODE
//		Core_gpio_pattern = 0x0000bf00; 	
// 20200217
//		Core_gpio_pattern = 0x0e40bf00; 	// [Inj:Val:Syr:-:-:Z] 
		motorPowerModeData = (motorPowerModeData
												| MOTOR_POWERDOWN_MODE_SET_SAM1 
												| MOTOR_POWERDOWN_MODE_SET_SAM2 
												| MOTOR_POWERDOWN_MODE_SET_M3
												| MOTOR_POWERDOWN_MODE_SET_M4
												| MOTOR_POWERDOWN_MODE_SET_M5
												| MOTOR_POWERDOWN_MODE_SET_M6);							
#else
//		Core_gpio_pattern = 0x0000bf00;
		motorPowerModeData = (motorPowerModeData
												| MOTOR_SLEEP_MODE_SET_SAM1 
												| MOTOR_SLEEP_MODE_SET_SAM2 
												| MOTOR_SLEEP_MODE_SET_M3
												| MOTOR_SLEEP_MODE_SET_M4
												| MOTOR_SLEEP_MODE_SET_M5
												| MOTOR_SLEEP_MODE_SET_M6);												
#endif

		FPGA_WRITE_WORD(W4_MOTOR_POW_CON, motorPowerModeData);
//		*((uint32_t volatile *)(COREGPIO_APBBIF_0_OUT_REG)) = (Core_gpio_pattern);

//readyOnOff
//remoteAction(
		REMOTE_READY_OUT_CONTACT_OPEN;

		REMOTE_START_OUT_CONTACT_OPEN;

// POWER BTN ���� 
		CPU_PWR_CON_HIGH;
		FPGA_WRITE_WORD(W2_POWER_BTN_TIME, (ST_ON_BTN_DET_TIME_DEFAULT | (ST_OFF_SEQUENCE_TIME_DEFAULT << 16)));	// ��ư ���� �ð� , ��ư ���� �� Off �ð�
//		FPGA_WRITE_WORD(W2_POWER_BTN_TIME, ST_ON_BTN_DET_TIME_DEFAULT | (300 << 16));	// ��ư ���� �ð� , ��ư ���� �� Off �ð� 300 x 10msec
// LED ����
		InitialLED();
		
		BUZZER_CON_OFF;

//BUZZER_CON_ON;

EXT_LED_CON_OFF;


/*		
 		g_gpio1.base_addr = COREGPIO_APBBIF_1_BASE_ADDR;
    g_gpio1.apb_bus_width = GPIO_APB_32_BITS_BUS;
*/

// ===================================================================================================
// Example
// ===================================================================================================
#if 0
	gpio_in = HAL_get_32bit_reg( this_gpio->base_addr, GPIO_IN );
	(*((uint32_t volatile *)(COREGPIO_APBBIF_0_BASE_ADDR + GPIO_OUT_REG_OFFSET)) = (Core_gpio_pattern & 0xfffffffe));	    
	Core_gpioInput = (*((uint32_t volatile *)(COREGPIO_APBBIF_0_IN_REG)));
#endif

}

void init_system()
{
	static uint8_t msen_switch[5];
	static uint8_t rotory_switch[5];

  /*gcsvc.MasterSlave = */	
	Intro();
	init_Control_port(); // init_CoreGpio();

	PCF8574_ROTARY_init();
// �� ����Ʈ�� ������ ������ ó���� �����߻��� ��찡..  
	I2C_PCF8574_read(PCF8574_ROTARY_SLAVE_ADDRESS, rotory_switch, 3);

// ���͸� ����ġ ��ȣ�� �߸� �Ǿ 
	devSet.IP_switch = 7 - ( (rotory_switch[2] & 0x02) << 1 | (rotory_switch[2] & 0x04) >> 1 | (rotory_switch[2] & 0x08)>> 3);
	dp("devSet.IP_switch = %x\n\r",devSet.IP_switch);

//	PCF8574_TRAY_SEN_init();
//	I2C_PCF8574_read(PCF8574_TRAY_SEN_SLAVE_ADDRESS, msen_switch, sizeof(msen_switch));
	I2C_PCF8574_read(PCF8574_TRAY_SEN_SLAVE_ADDRESS, msen_switch, 3);

	devSet.msen_switch = (msen_switch[2] & 0x3f);
	dp("devSet.msen_switch = %x\n\r",devSet.msen_switch);

//===========================================================================================================
// eNVM
//===========================================================================================================
	devState.envmDataAvailable = isEnvmDataAvailable();





//	initEnvmData();

	initSystemInform();

//	rtcTime_Macaddr_Init();
	rtcTime_Init();

//	EEPROM_init();			
}

int rtcTime_Init()
{
	unsigned char saveFlag;
	uint8_t rtc_time[10];
	
	eepromRead_Data((RTC_INIT_SAVE_ADDR), 1, &(saveFlag));
	if(saveFlag != DATA_SAVE_FLAG) {
		rtc_time[0] = RTC_SECONDS_REG_ADDR;		// add
sysInform.cSysTime.second =	rtc_time[1] = 0x11;	// sec
sysInform.cSysTime.minute = rtc_time[2] = 0x11;	// min
sysInform.cSysTime.hour = rtc_time[3] = 0x11;	// hour
		rtc_time[4] = 0x04;	// day
sysInform.cSysDate.date = rtc_time[5] = 0x11;	// date
sysInform.cSysDate.month = rtc_time[6] = 0x11;	// month
sysInform.cSysDate.year = rtc_time[7] = 0x20;	// year
		
		rtcWrite_time(rtc_time);

//		eepromRead_Data((RTC_INIT_SAVE_ADDR), 1, &(saveFlag));
		saveFlag = DATA_SAVE_FLAG;
		eepromWrite_Data((RTC_INIT_SAVE_ADDR), 1, &(saveFlag));
dfp("rtcWrite_time\n");	
		return RE_ERR;
	}
	else {
		rtcRead_time(RTC_SECONDS_REG_ADDR,rtc_time);

		sysInform.cSysTime.second =	// rtc_time[0];
			rtc_time[0] / 16 * 10 + rtc_time[0] % 16;
		sysInform.cSysTime.minute = // rtc_time[1];
			rtc_time[1] / 16 * 10 + rtc_time[1] % 16;		
		sysInform.cSysTime.hour = //rtc_time[2];
			rtc_time[2] / 16 * 10 + rtc_time[2] % 16;		
			rtc_time[3];
		sysInform.cSysDate.date = //rtc_time[4];
			rtc_time[4] / 16 * 10 + rtc_time[4] % 16;		
		sysInform.cSysDate.month = //rtc_time[5];
			rtc_time[5] / 16 * 10 + rtc_time[5] % 16;		
		sysInform.cSysDate.year = //rtc_time[6];
			rtc_time[6] / 16 * 10 + rtc_time[6] % 16;		
		int i;
		printf("rtc_time = "); for(i=0 ; i<7; i++) {	printf("%x : ",rtc_time[i]);	} printf("\n");	

	}

}

// ó������ ����Ⱑ ����Ǹ� mac address�� �����Ѵ�.
void make_MACADDR(int NewMAC)
{
	unsigned char macAddr[7];
	unsigned int time;
//	unsigned char time;
	char tmpMac[5];
	int i;

	eepromRead_Data(MAC_ADDRESS_ADDR_SAVE,MAC_ADDR_READ_LEN,&macAddr);

	if(NewMAC == MAKE_MAC) { 
		
// ���� ������ �ʿ��� time
/*
		time = (unsigned int)(sysInform.cSysDate.year << 26) + (unsigned int)(sysInform.cSysDate.month << 22) + (unsigned int)(sysInform.cSysDate.date << 17) 
				+ (unsigned int)(sysInform.cSysTime.hour << 12) + (unsigned int)(sysInform.cSysTime.minute << 6) + (unsigned int)(sysInform.cSysTime.second);
dp("time=%d\n",time);

		time = (unsigned int)(sysInform.cSysTime.minute << 6) + (unsigned int)(sysInform.cSysTime.second);
dp("time=%d\n",time);		
		time = (unsigned int)(sysInform.cSysTime.minute << 6);
dp("time=%d\n",time);
		time = (unsigned int)(sysInform.cSysTime.minute << 6) + (unsigned int)(sysInform.cSysTime.second) + 35;
dp("time=%d\n",time);
*/
		time = (unsigned int)(sysInform.cSysDate.year << 10) + (unsigned int)(sysInform.cSysDate.month << 8) + (unsigned int)(sysInform.cSysDate.date << 6) 
				+ (unsigned int)(sysInform.cSysTime.hour << 4) + (unsigned int)(sysInform.cSysTime.minute << 2) + (unsigned int)(sysInform.cSysTime.second);

dp("time=%d\n",time);
/*
//		if(devState.connected == YES) {	// ó�� ������ �� �ʱ�ȭ �Ѵ�. - ��⸶�� �ٸ� �ƾ�巹���� �����ϱ� ���Ͽ� �����Ѵ�.
dp("y%x\n",sysInform.cSysDate.year);
dp("m%x\n",sysInform.cSysDate.month);
dp("d%x\n",sysInform.cSysDate.date);
dp("h%x\n",sysInform.cSysTime.hour);
dp("m%x\n",sysInform.cSysTime.minute);
dp("s%d\n",sysInform.cSysTime.second);
*/
			srand(time);
int r[6];			
			for(i=0; i<ETH_ADDR_LEN; i++) {
				r[i] = rand();
				my_mac[i] = r[i] % 256;
dfp("rand = %x %x time=%d\n",r[i], my_mac[i], time);
			}

			my_mac[0] = ETHERNET_CONF_ETHADDR0;
			my_mac[1] = ETHERNET_CONF_ETHADDR1;

dfp("sysInform.cSerialNo = %s \n",sysInform.cSerialNo);
/*
			strncpy(tmpMac, &sysInform.cSerialNo[4],2);	
			tmpMac[2] = '\0';
			my_mac[4] = atoi(tmpMac);
*/
			strncpy(tmpMac, &sysInform.cSerialNo[10],2);	
			tmpMac[2] = '\0';
			my_mac[5] = atoi(tmpMac);
			
iprintf("[New MAC] my_mac[0]=[0x%x] [0x%x] [0x%x] [0x%x] [0x%x] [0x%x] \n\r",
	my_mac[0],my_mac[1],my_mac[2],my_mac[3],my_mac[4],my_mac[5]);

			macAddr[0] = DATA_SAVE_FLAG;	
			macAddr[1] = my_mac[0];
			macAddr[2] = my_mac[1];
			macAddr[3] = my_mac[2];
			macAddr[4] = my_mac[3];
			macAddr[5] = my_mac[4];		
			macAddr[6] = my_mac[5];			
			eepromWrite_Data(MAC_ADDRESS_ADDR_SAVE,MAC_ADDR_READ_LEN,&macAddr);	
	}
	else {	// LOAD_MAC
		if(macAddr[0]	== DATA_SAVE_FLAG) {
	//		eepromRead_Data(MAC_ADDRESS_ADDR_0,ETH_ADDR_LEN,&my_mac);
			my_mac[0] = macAddr[1];
			my_mac[1] = macAddr[2];
			my_mac[2] = macAddr[3];
			my_mac[3] = macAddr[4];
			my_mac[4] = macAddr[5];
			my_mac[5] = macAddr[6];		
iprintf("[loadMac] my_mac[0]=[0x%x] [0x%x] [0x%x] [0x%x] [0x%x] [0x%x] \n\r",
	my_mac[0],my_mac[1],my_mac[2],my_mac[3],my_mac[4],my_mac[5]);

		}
		else if(NewMAC == APPLY_MAC) { 	// �޵簪���� �ʱ�ȭ 
		
		}
		else {	// �ʱ�ȭ 
// ����Ʈ ���� ����ȴ�.
			my_mac[0] = ETHERNET_CONF_ETHADDR0;
			my_mac[1] = ETHERNET_CONF_ETHADDR1;
			my_mac[2] = ETHERNET_CONF_ETHADDR2;
			my_mac[3] = ETHERNET_CONF_ETHADDR3;
			my_mac[4] = ETHERNET_CONF_ETHADDR4;
			my_mac[5] = ETHERNET_CONF_ETHADDR5 + devSet.IP_switch;
			
dfp("[default MAC] my_mac[0]=[0x%x] [0x%x] [0x%x] [0x%x] [0x%x] [0x%x] \n\r",
	my_mac[0],my_mac[1],my_mac[2],my_mac[3],my_mac[4],my_mac[5]);
		}
	}
}

void saveEthernetConfig(int who)
{
	unsigned char saveFlag = DATA_SAVE_FLAG;
	unsigned char data[21];

	data[0] = DATA_SAVE_FLAG;

	memcpy(&data[1], &my_ip, 4);
	memcpy(&data[5], &my_gateway, 4);
	memcpy(&data[9], &my_netMask, 4);

//	memcpy(&data[1], &(ethernetConfig.cIPAddress), ETHERNET_CONFIG_DATA_READSIZE);
	eepromWrite_Data((ETHERNET_CONFIG_DATA_SAVE), ETHERNET_CONFIG_DATA_READSIZE + 1, &(data[0]));
}

void init_IpAddr(unsigned char sw)
{
	my_ip[0] =	ETHERNET_CONF_IPADDR0;
	my_ip[1] =	ETHERNET_CONF_IPADDR1;
	my_ip[2] =	ETHERNET_CONF_IPADDR2;
	my_ip[3] =	ETHERNET_CONF_IPADDR3 + sw;

// macaddress�� ���� 
#if 0
	my_mac[0] = ETHERNET_CONF_ETHADDR0;
	my_mac[1] = ETHERNET_CONF_ETHADDR1;
	my_mac[2] = ETHERNET_CONF_ETHADDR2;		
	my_mac[3] = ETHERNET_CONF_ETHADDR3;
	my_mac[4] = ETHERNET_CONF_ETHADDR4;
	my_mac[5] = ETHERNET_CONF_ETHADDR5;		
#endif

	my_netMask[0] = ETHERNET_CONF_NET_MASK0;
	my_netMask[1] = ETHERNET_CONF_NET_MASK1;
	my_netMask[2] = ETHERNET_CONF_NET_MASK2;
	my_netMask[3] = ETHERNET_CONF_NET_MASK3;

	my_gateway[0] = ETHERNET_CONF_GATEWAY_ADDR0;
	my_gateway[1] = ETHERNET_CONF_GATEWAY_ADDR1;
	my_gateway[2] = ETHERNET_CONF_GATEWAY_ADDR2;
	my_gateway[3] = ETHERNET_CONF_GATEWAY_ADDR3;

#if STORE_INFORMATION_ENVM
		memcpy(as_INFO.cIPAddress, my_ip, 4);
		memcpy(as_INFO.cGateway, my_gateway, 4);
	//	memcpy(as_INFO.cMacAddress, my_mac, 6); 
		memcpy(as_INFO.cNetmask, my_netMask, 4);
#endif	
}

void loadEthernetConfig(int who)
{
	unsigned char saveFlag;// = DATA_SAVE_FLAG;
	unsigned char data[21];

	make_MACADDR(LOAD_MAC);

	//eepromRead_Data(ETHERNET_CONFIG_DATA_SAVE,ETHERNET_CONFIG_DATA_READSIZE,&data);
	eepromRead_Data(ETHERNET_CONFIG_DATA_SAVE,ETHERNET_CONFIG_DATA_READSIZE,data);

	saveFlag = data[0];

	if(devSet.IP_switch == 7) { // default ip���  // DEFAULT_IPADDRESS

		init_IpAddr(0);

		saveEthernetConfig(0);
	}
	else {
//	if(who == LOAD_IPADDRESS) { // load ip
//	}
		if(saveFlag	== DATA_SAVE_FLAG) {
//			eepromRead_Data(AS_IP_ADDRESS_ADDR0,IP_ADDR_LEN,&my_ip);
			my_ip[0] = data[1];
			my_ip[1] = data[2];
			my_ip[2] = data[3];
//			my_ip[3] = data[4];
			my_ip[3] = data[4] + devSet.IP_switch;

			my_gateway[0] = data[5];
			my_gateway[1] = data[6];
			my_gateway[2] = data[7];
			my_gateway[3] = data[8];

			my_netMask[0] = data[9];
			my_netMask[1] = data[10];
			my_netMask[2] = data[11];
			my_netMask[3] = data[12];		
	dfp("load === my_ip[0]=[0x%x] [0x%x] [0x%x] [0x%x] \n\r",
					my_ip[0],my_ip[1],my_ip[2],my_ip[3]);
			
		}
		else {	// �ʱ�ȭ default - rotory sw
			init_IpAddr(devSet.IP_switch);
		}
		
	}

	sprintf(ethernetConfig.cIPAddress,"%d.%d.%d.%d",my_ip[0], my_ip[1], my_ip[2], my_ip[3]);
	dprintf("%s %3d.%3d.%3d.%3d\n",ethernetConfig.cIPAddress, my_ip[0], my_ip[1], my_ip[2], my_ip[3]);

	sprintf(ethernetConfig.cGateway,"%d.%d.%d.%d",my_gateway[0], my_gateway[1], my_gateway[2], my_gateway[3]);
	dprintf("%s %3d.%3d.%3d.%3d\n",ethernetConfig.cGateway, my_gateway[0], my_gateway[1], my_gateway[2], my_gateway[3]);

	sprintf(ethernetConfig.cNetmask,"%d.%d.%d.%d",my_netMask[0], my_netMask[1], my_netMask[2], my_netMask[3]);
	dprintf("%s %3d.%3d.%3d.%3d\n",ethernetConfig.cNetmask, my_netMask[0], my_netMask[1], my_netMask[2], my_netMask[3]);







	strcpy(ethernetConfig.cFirmwareVersion, FIRMWARE_VERSION);
	strcpy(ethernetConfig.cMachineVersion, MACHINE_VER);
	
}

void eeprom2envm_data()
{
	uint8_t data[32];
	uint32_t volatile addr;
	uint32_t len;
	unsigned char saveIP = NO;
	unsigned char macAddr[7];
	unsigned char saveFlag;// = DATA_SAVE_FLAG;
	OVEN_TEMPCAL_t tempCal; 	
	uint8_t rtc_time[10];

// =================== Serial No + install Date
	eepromRead_Data(SYSTEM_INFORM_DATA_SAVE, (SYSTEM_INFORM_DATA_READSIZE + 1), data);
	saveFlag = data[0];
	if(saveFlag == DATA_SAVE_FLAG) {
		memcpy(as_INFO.cSerialNO, data[1], SYSTEM_INFORM_DATA_READ_SERIAL);
		as_INFO.cInstallDate[0] = 21;
		as_INFO.cInstallDate[1] = data[18];
		as_INFO.cInstallDate[2] = data[19];
		as_INFO.cInstallDate[3] = data[20];	
//		memcpy(as_INFO.cInstallDate, data[18], 3);

		uint8_t saveFlag = DATA_SAVE_FLAG;
		NVM_write((INFO_ENVM_SAVE_FLAG_ADDR + FLAG_SERIAL_NO), &saveFlag, 1, NVM_DO_NOT_LOCK_PAGE);
	
		memcpy(&idata[0] ,&as_INFO.cSerialNO[0], sizeof(as_INFO.cSerialNO));
		ret = NVM_write(INFO_SERIAL_NO_ENVM_ADDR, idata, INFO_SERIAL_NO_ENVM_LEN, NVM_DO_NOT_LOCK_PAGE);
	
		save_InstallDate_Envm();
	}
	else {
 		memcpy(as_INFO.cSerialNO, "AS2021171112", (INFO_SERIAL_NO_ENVM_LEN)); 			
//				memcpy(oven_INFO.cInstallDate, "20211111", INFO_INSTALL_DATE_ENVM_LEN);
		as_INFO.cInstallDate[0] = 20;
		as_INFO.cInstallDate[1] = 21;
		as_INFO.cInstallDate[2] = 11;
		as_INFO.cInstallDate[3] = 11;
	}
// =================== IP ADDR 
	eepromRead_Data(ETHERNET_CONFIG_DATA_SAVE,ETHERNET_CONFIG_DATA_READSIZE,data);
	saveFlag = data[0];

	if(devSet.IP_switch == 7) { // default ip���	// DEFAULT_IPADDRESS - ����
		init_IpAddr(0);
		saveIP = YES;
	
		save_IPADDR();
	}
	else {
		if(saveFlag == DATA_SAVE_FLAG) {
			my_ip[0] = data[1]; my_ip[1] = data[2]; my_ip[2] = data[3];
			my_ip[3] = data[4]; //	my_ip[3] = data[4] + ovenStt.IP_switch;
			my_gateway[0] = data[5]; my_gateway[1] = data[6]; my_gateway[2] = data[7];	my_gateway[3] = data[8];
			my_netMask[0] = data[9]; my_netMask[1] = data[10]; my_netMask[2] = data[11]; my_netMask[3] = data[12];

			memcpy(as_INFO.cIPAddress, my_ip, 4);
			memcpy(as_INFO.cGateway, my_gateway, 4);
		//	memcpy(as_INFO.cMacAddress, my_mac, 6); 
			memcpy(as_INFO.cNetmask, my_netMask, 4);
saveIP = YES;
		save_IPADDR();

		iprintf("[load IP] -- eNVM save");				
		}
		else {	// �ʱ�ȭ default - rotory sw - �������.
			init_IpAddr(devSet.IP_switch);
saveIP = NO;
		iprintf("[default IP] ");
		}
	}

	
// =================== mac_addr
	eepromRead_Data(MAC_ADDRESS_ADDR_SAVE,MAC_ADDR_READ_LEN,macAddr);
	saveFlag = macAddr[0];		
	if(saveFlag == DATA_SAVE_FLAG) {//		if(macAddr[0] == DATA_SAVE_FLAG) {
		NVM_write((INFO_ENVM_SAVE_FLAG_ADDR + FLAG_MAC_ADDR), &saveFlag, 1, NVM_DO_NOT_LOCK_PAGE);
		
		my_mac[0] = macAddr[1];
		my_mac[1] = macAddr[2];
		my_mac[2] = macAddr[3];
		my_mac[3] = macAddr[4];
		my_mac[4] = macAddr[5];
		my_mac[5] = macAddr[6]; 	

		my_mac[5] = ETHERNET_CONF_ETHADDR5 + devSet.IP_switch; 		

		addr = INFO_MACADDR_ENVM_ADDR;
		len = INFO_MACADDR_ENVM_LEN;
		NVM_write(addr, my_mac, len, NVM_DO_NOT_LOCK_PAGE);

	memcpy(as_INFO.cMacAddress, my_mac, INFO_MACADDR_ENVM_LEN);

		iprintf("[load MAC] ");
	}
	else {	// �ʱ�ȭ // ����Ʈ ���� ����ȴ�. - IP switch ����
		my_mac[0] = ETHERNET_CONF_ETHADDR0;
		my_mac[1] = ETHERNET_CONF_ETHADDR1;
		my_mac[2] = ETHERNET_CONF_ETHADDR2;
		my_mac[3] = ETHERNET_CONF_ETHADDR3;
		my_mac[4] = ETHERNET_CONF_ETHADDR4;
		my_mac[5] = ETHERNET_CONF_ETHADDR5 + devSet.IP_switch;
		iprintf("[default MAC] ");
	}
iprintf(" my_mac[0]=[0x%x] [0x%x] [0x%x] [0x%x] [0x%x] [0x%x] \n\r",
		my_mac[0],my_mac[1],my_mac[2],my_mac[3],my_mac[4],my_mac[5]);

// =================== sysConfig



	
// =================== Special
	eepromRead_Data(SPECIAL_DATA_SAVE,(sizeof(special) + 1), data);
//	eepromRead_Data(SPECIAL_DATA_SAVE,(size + 1),&data);	
	saveFlag = data[0];
	if(saveFlag == DATA_SAVE_FLAG) {
		memcpy(&special, &data[1], sizeof(special));
		checkSpecialValue();

		save_Special_Envm();
	}
	else {
		initSpecialValue();
	}

// =================== Temp Calibration
	GfTempConstA = MAIN_TEMP_CONST_A;
	GfTempConstB = MAIN_TEMP_CONST_B;
	GfTempConstC = MAIN_TEMP_CONST_C;
	ovenStt.btTempCalibDataNo = 0;

	if(TempConst(DATA_LOAD) == 1) {
		TempConstCalc(TEMP_CONST_CALCULATION);		// Calc
		TempConst_Envm(DATA_SAVE);
//			save_TempCal_Envm();
	}
	else TempConstCalc(TEMP_CONST_DEFAULT); 								// Default


}

#if INFORM_COMMON_STORAGE_USE
// =================== Serial No + install Date		// sysInform // eepromRead_Data((SYSTEM_INFORM_DATA_ADDR), SYSTEM_INFORM_DATA_READSIZE, &(sysInform.cSerialNo));
// =================== mac_addr		// eepromRead_Data(MAC_ADDRESS_ADDR_SAVE,MAC_ADDR_READ_LEN,&macAddr);
// =================== IP ADDR		// eepromRead_Data(ETHERNET_CONFIG_DATA_SAVE,ETHERNET_CONFIG_DATA_READSIZE,data);

// sysConfig - tmp.trayLeft > TRAY_NONE		// 	eepromRead_Data(SYSTEM_CONFIG_DATA_SAVE,(size + 1),&readSysCfgBuf);

// =================== Special		special.LED_onoff // eepromRead_Data(SPECIAL_DATA_SAVE,(size + 1),&readBuf);
// =================== Temp Calibration		// eepromRead_Data(TEMP_CAL_DATA_ADDR,20,&tempCalData.meaTemp1); 
// tempCalData.setTemp1 = TEMP_CAL_SET_1ST;
// =================== RTC				// eepromRead_Data((RTC_INIT_SAVE_ADDR), 1, &(saveFlag));

// =================== adjust		// eepromRead_Data(HOME_ADJUST_VERSION_CHECK_ADDR, HOME_ADJUST_VERSION_CHECK_SIZE,(uint8_t *)(&devSet.adjust_ver));
// usedTime.time 	// eepromRead_Data((USED_TIME_DATA_ADDR), sizeof(usedTime), (uint8_t *)( &(usedTime)));

void initEnvmData()
{
		uint8_t data[32];
		uint32_t volatile addr;
		uint32_t len;
		unsigned char saveIP = NO;
		unsigned char macAddr[7];
		unsigned char saveFlag;// = DATA_SAVE_FLAG;
//		OVEN_TEMPCAL_t tempCal;

		uint8_t eeprom2envm = NO;;
		uint8_t firmwareUpdate = NO;

		uint8_t saveEnvmFlag[3];	// serial / ip_addr / mac_addr

		devState.envmDataAvailable = isEnvmDataAvailable();

		if(devState.envmDataAvailable == RE_ERR) { // ��ȯ�ؾ�
			eeprom2envm_data();
			eeprom2envm = YES;
		}
		else { // ������̴�.  //		if(ovenStt.envmDataAvailable) {
		// =================== Save Flag
			addr = INFO_ENVM_SAVE_FLAG_ADDR;
			len = INFO_ENVM_SAVE_FLAG_LEN;
			MSS_NVM_read( (uint8_t *)addr, saveEnvmFlag, len);
	//		saveEnvmFlag[FLAG_SERIAL_NO] = data[0];
	//		saveEnvmFlag[FLAG_IP_ADDR] = data[1];
	//		saveEnvmFlag[FLAG_MAC_ADDR] = data[2];

			// =================== Serial No + install Date
			if(saveEnvmFlag[FLAG_SERIAL_NO] = DATA_SAVE_FLAG) {
				addr = INFO_SERIAL_NO_ENVM_ADDR;
				len = INFO_SERIAL_NO_ENVM_LEN;
				MSS_NVM_read( (uint8_t *)addr, data, len);
				memcpy(oven_INFO.cSerialNO, &data[0], (INFO_SERIAL_NO_ENVM_LEN));
				dp("####### read Serial No : [%s] [%c]\n", &data[0], data[0]);

				MSS_NVM_read( (uint8_t *)INFO_INSTALL_DATE_ENVM_ADDR, data, INFO_INSTALL_DATE_ENVM_LEN);
				memcpy(oven_INFO.cInstallDate, &data[0], INFO_INSTALL_DATE_ENVM_LEN);
#if INFORM_SERIAL_NO_ADD
				memcpy(gOVEN_INFORM.cSerialNO, oven_INFO.cSerialNO, (INFO_SERIAL_NO_ENVM_LEN));
#endif
			}
			else {
				dp("#not saved - Serial No : [%s] \n", data);
				memcpy(oven_INFO.cSerialNO, "CO202111-543FADE", (INFO_SERIAL_NO_ENVM_LEN));
				memcpy(oven_INFO.cInstallDate, "20211111", INFO_INSTALL_DATE_ENVM_LEN);
			}
		// =================== mac_addr
			make_MACADDR(LOAD_MAC);

		// =================== IP ADDR
			if(ovenStt.IP_switch == 7) { // default ip���	// DEFAULT_IPADDRESS - ����
				init_IpAddr(0);
				saveIP = YES;

				save_IPADDR();
			}
//			if(ovenStt.IP_switch != 7) {
			else {
				if(saveEnvmFlag[FLAG_IP_ADDR] = DATA_SAVE_FLAG) {
					addr = INFO_IP_ADDR_ENVM_ADDR;
					len = INFO_IP_ADDR_ENVM_LEN;
					MSS_NVM_read( (uint8_t *)addr, data, len);
	iprintf(" 	#read IP Config!!! \n");
					memcpy(my_ip, &data[4], 4);
					my_ip[3] = data[4 + 3] + ovenStt.IP_switch;
					memcpy(my_gateway, &data[8], 4);
					memcpy(my_netMask, &data[12], 4);

					memcpy(oven_INFO.cIPAddress, my_ip, 4);
					memcpy(oven_INFO.cGateway, my_gateway, 4);
					memcpy(oven_INFO.cNetmask, my_netMask, 4);
				}
				else {
	iprintf(" 	#init IP Config!!! \n");
					init_IpAddr(ovenStt.IP_switch);
				}
			}
		// =================== Special
			addr = INFO_SPECIAL_ENVM_ADDR;
			len = INFO_SPECIAL_ENVM_LEN;
			MSS_NVM_read( (uint8_t *)addr, data, len);
			if(data[0] == 0xAA && data[1] == 0xAA && data[2] == 0xAA && data[3] == 0xAA) {
				iprintf("####### read Special [%d] [%d] [%d] [%d] [%d]\n", data[4], data[5], data[6], data[7], data[8]);

				memcpy(&gOVEN_CONFIG, &data[4], 12);
//				memcpy(&oven_INFO.fMaxTemperature, &data[4], 12);
				checkOvenConfigValue();
			}
			else {
				iprintf("#not saved - Special\n");

				initOvenConfigValue();
			}
		// =================== Temp Calibration
			GfTempConstA = MAIN_TEMP_CONST_A;
			GfTempConstB = MAIN_TEMP_CONST_B;
			GfTempConstC = MAIN_TEMP_CONST_C;
			ovenStt.btTempCalibDataNo = 0;

			if(TempConst_Envm(DATA_LOAD) == 1) {
				TempConstCalc(TEMP_CONST_CALCULATION);		// Calc
			}
			else TempConstCalc(TEMP_CONST_DEFAULT); 		// Default

		// =================== RTC
			rtcTime_Init();

		}
// =======================================================================

// =======================================================================
// =======================================================================

		oven_INFO.FirmwareVersion = OVEN_FIRMWARE_VERSION;//((OVEN_FIRMWARE_VERSION / 100) << 16) | (OVEN_FIRMWARE_VERSION % 100);
		oven_INFO.FPGAVer = FPGA_READ_WORD(R0_FPGA_VER);

		oven_INFO.cSerialNO;
	//		memcpy(oven_INFO.cIPAddress, my_ip, 4);
	//		memcpy(oven_INFO.cGateway, my_gateway, 4);
		memcpy(oven_INFO.cMacAddress, my_mac, 6);
		strcpy(oven_INFO.cModel, SYSTEM_MODEL);
	//		memcpy(oven_INFO.cNetmask, my_netMask, 4);

		firmwareUpdate = isFirmwareUpdate();

	dp("#firmwareUpdate = %d eeprom2envm = %d \n", firmwareUpdate, eeprom2envm);
		if(firmwareUpdate == YES || eeprom2envm == YES) {
			save_Storage_INFO_Envm();
		}
}
#endif

//===== Envm Data ���� =====
// Common ���� 0x3F0FC �� ���� 0xAA5555AA�� �ƴϸ� �����Ѵ�.
// <<Common����  >> 
//	  Ver ���� : app Ver, FPGA Ver
// 		Serial No
// 		macAddr
//		model
// <<EEPROM Data>>
// 		
int isEnvmDataAvailable()
{
	uint8_t rdata[4];
	uint32_t volatile addr;
	uint32_t len,UsedENVM;
	uint32_t envm = COPY_DONE_FRAM_TO_ENVM;
	
	addr = INFO_COMMON_VER_SER_ENVM_ADDR + USED_ENVM_STORAGE_OFFSET;
	len = 4;

	MSS_NVM_read( (uint8_t *)addr, (uint8_t *)&UsedENVM, len);

//	if(!strncmp(&envm, &UsedENVM, 4)) {
	if(envm == UsedENVM) {
dp("==Envm_Data available!!!\n");
		return RE_OK;
	}
	else return RE_ERR;

//	if(rdata[0] != 0xAA || rdata[1] != 0x55 || rdata[2] != 0x55 || rdata[3] != 0xAA) return RE_OK;
//	else return RE_ERR;

}

int isFirmwareUpdate()	
{
	uint8_t rdata[4];
	uint32_t volatile addr;
	uint32_t len,ver;
	uint32_t envm = COPY_DONE_FRAM_TO_ENVM;
	
	addr = INFO_COMMON_VER_SER_ENVM_ADDR + APP_VER_STORAGE_OFFSET;
	len = 4;

// ����� ������ ���Ѵ�.
	MSS_NVM_read( (uint8_t *)addr, (uint8_t *)&ver, len);

	if(oven_INFO.FirmwareVersion == ver) {
dp("App_Ver Modified!!!\n");
		return RE_OK;
	}
	else return RE_ERR;
}


int save_IPADDR()
{
	uint32_t volatile addr;
	uint32_t len;
	uint8_t idata[32];
	int i;

	// Save Flag
	uint8_t saveFlag = DATA_SAVE_FLAG;
 	NVM_write((INFO_ENVM_SAVE_FLAG_ADDR + FLAG_IP_ADDR), &saveFlag, 1, NVM_DO_NOT_LOCK_PAGE);

	addr = INFO_IP_ADDR_ENVM_ADDR;
	len = INFO_IP_ADDR_ENVM_LEN;
	memcpy(&idata[0], as_INFO.cIPAddress, 4);
	memcpy(&idata[4], as_INFO.cGateway, 4);
//	memcpy(&idata[8], oven_INFO.cNetmask, 4);		 - netmask, port no ����

	NVM_write(addr, idata, len, NVM_DO_NOT_LOCK_PAGE);
	iprintf(" 	#save IP Config!!! \n"); 				

	return 0;
}

int checkSpecialValue()
{
	if(special.LED_onoff >= ON) special.LED_onoff = ON;
	else special.LED_onoff = OFF;
	if(special.Buzzer_onoff >= ON) special.Buzzer_onoff = ON;
	else special.Buzzer_onoff = OFF;
	if(special.btLeakCheck >= ON) special.btLeakCheck = ON;
	else special.Door_check = OFF; 		
	if(special.Door_check >= ON) special.Door_check = ON;
	else special.Door_check = OFF;
	
	as_INFO.LED_onoff = special.LED_onoff;
	as_INFO.Door_check = special.Door_check;
	as_INFO.Leak_check = special.btLeakCheck;
	as_INFO.Buzzer_onoff = special.Buzzer_onoff;

	return RE_OK;
}

int save_Special_Envm()
{
	uint32_t volatile addr;
	uint32_t len;
	uint8_t idata[32];

	addr = INFO_SPECIAL_ENVM_ADDR;
	len = INFO_SPECIAL_ENVM_LEN;

			// Save Flag
	idata[0] = 0xAA;		idata[1] = 0xAA;		idata[2] = 0xAA;		idata[3] = 0xAA;
	memcpy(&idata[4], &as_INFO.LED_onoff,4);
/*
	idata[4] = as_INFO.LED_onoff;
	idata[5] = as_INFO.Door_check;
	idata[6] = as_INFO.Leak_check;
	idata[7] = as_INFO.Buzzer_onoff;
*/	
//	idata[8] = oven_INFO.TempConModule;

	NVM_write(addr, idata, len, NVM_DO_NOT_LOCK_PAGE);
	iprintf(" 	#save save_Special_Config!!! \n");				

	return 0;
}

int initSpecialValue()
{
//	gOVEN_CONFIG.fMaxTemperature = OVEN_TEMP_SET_MAX; // �ִ뼳�� �µ�
//	gOVEN_CONFIG.fMinTemperature = OVEN_TEMP_SET_MIN; // �ּҼ��� �µ�

	special.LED_onoff = ON;
	special.Buzzer_onoff = ON;
	special.Door_check = ON;
	special.btLeakCheck = ON;

//	oven_INFO.fMaxTemperature = special.fMaxTemperature;
//	oven_INFO.fMinTemperature = special.fMinTemperature;

	as_INFO.LED_onoff = special.LED_onoff;
	as_INFO.Door_check = special.Door_check;
	as_INFO.Leak_check = special.btLeakCheck;
	as_INFO.Buzzer_onoff = special.Buzzer_onoff;

	return RE_OK;
}

