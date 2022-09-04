#ifndef OPTION_AS_H_
#define OPTION_AS_H_


#define BOARD_TEST_PRGM				0			// 보드테스트를 목적으로 - 자동 반복 운동
#define MACHINE_TEST_PRGM			0			// 기구적인 테스트 - Key B/d 입력이용

// debug_inform.h - ####
//#define NDEBUG_DP

// 20210503
#define SOC_TYPE_SF2					1
	// SYS_TICKS_SF2

#define NET_CONNECT_TEST			0

// 20210603 - RTC Write 
#define RTC_WRITE_SF2			1


//===========================================================================================
// 20200514 Encoder 적용 - new Board에 적용함.
//===========================================================================================
#define NEW_BOARD_ENCODER_2020			1
#if NEW_BOARD_ENCODER_2020
	#define FPGA_VERSION_TRIM			1
		#define FPGA_VERSION_TRIM_BREAK			1				// motorResetCmd   or   motorReadyCmd
	// 20200515
		#define FPGA_VERSION_TRIM_ADJUST_SLEEP		1			// AS_ADJUST_MODE_SLEEP

		#define GO_POS_ETC_USE					1

		#define BUZZER_MODIFY						1					// 구형보드와 반대 
//===========================================================================================
#else
//===========================================================================================
// 20200507 TRIM - old Board에 적용됨. - old board사용의 경우 아래 설정 선택 
//===========================================================================================
	#define FPGA_VERSION_TRIM			1
		#define FPGA_VERSION_TRIM_BREAK			1				// motorResetCmd   or   motorReadyCmd
	// 20200515
		#define FPGA_VERSION_TRIM_ADJUST_SLEEP		1			// AS_ADJUST_MODE_SLEEP
#endif

#define FPGA_VERSION_TRIM_PUMP_TIME			1

//==================================================================================
// readyChkEncoder_Syr()
// #if SYRINGE_ENCODER_REMOVE_WASHING
// 20200526
// Washing 삭제
// SYRINGE_ENCODER_REMOVE_END
// endsequence 버림 - 원점 찾기 동작 
// 인젝션 끝난다음 삭제 - 원점 찾기 동작 
// 
//==================================================================================
#define SYRINGE_ENCODER_REMOVE_WASHING				1
#define SYRINGE_ENCODER_REMOVE_END					1
#define SYRINGE_ENCODER_REMOVE_INIT					1
//===========================================================================================

// 20200527 다음 바이알 전에 세척 안함. 수정 
#define WASH_BETWEEN_VIALS_BEFORE_FIX				1


// 20200529
// Diagnosis에서 사용 추가 
// Photo interrupter 테스트 용도로 사용한다.
//#define PHOTO_SENSOR_TEST						1			


// 20200609
// 바이알 트레이 좌우 
#define VIAL_TRAY_CONFIG_FIX				1


// #define STEPCONTROL_1_0                 0x40057000U
// CoreAPB3_0을 32bit로 설정하였다. offset은 4byte단위로
	#define  FABRIC_STEPSET_APB20_BASE_ADDR		STEPCON_0//STEP_SET16_0

/*
#define COREGPIO_0                      0x40050000U
#define STEPCONTROL18_0                 0x40051000U
#define STEPCONTROL18_1                 0x40052000U
#define STEPCONTROL18_2                 0x40053000U
#define STEPCONTROL18_3                 0x40054000U
#define STEPCONTROL13_0                 0x40055000U
#define STEPCONTROL13_1                 0x40056000U
#define PWM_CON8_0                      0x40057000U
*/

//#define  STEPSET_BASEADDR_MOTOR_X					STEPCONTROL18_0//STEPASCON18_0

// 20200813
// - whahing port 깊이 수정 5mm 덜 내려 가도록 , Vial height 까지 합하면 5mm정도 
#define  COORD_Z_WASH_MODIFY				1	

// injection height 에 따라 Vial bottom 이 변경되는 오류 수정
// Wash, Waste
#define  VIAL_BOTTOM_CAL_FIX				1

// Injection 깊이(Inj port z)  adjust 초기값 수정 (3mm)
#define  INJECTION_DEPTH_MODIFY			1

// vial heigth adjust 초기값 수정
#define  VIAL_TOP_MODIFY				1


// 20210127
// RGB LED Control
//#define  FLED_CONTROL_ADD				1
// POWER BUTTON Control
#define  POWER_BTN_CONTROL_ADD				1


















		// Autotunning 실행
#define AUTO_TUNNING_CONTROL		0

#define HEATING_SYSTEM					0
#define COOLING_SYSTEM					1
//#define AUTO_TUNNING_TARGET			HEATING_SYSTEM
#define AUTO_TUNNING_TARGET			COOLING_SYSTEM

#if	AUTO_TUNNING_TARGET == HEATING_SYSTEM
#define AUTO_TUNNING_SET_TEMP	40.0		// Heating
#else
//#define AUTO_TUNNING_SET_TEMP	15.0		// Cooling
#define AUTO_TUNNING_SET_TEMP	6.0		// Cooling - 20191128
#endif


#define MOTOR_POWER_DOWN_MODE			1						// 20171213

#define MICRO_PUMP_VOLTAGE_ADJUSTMENT			1			// 20171216

#define VIAL_HEIGHT_DATUM			1			// vial 높이 기준
#define VIAL_POS_MODIFY				1			// 트레이 좌우틀어짐조정

/*
#define EEPROM_SAVE_MODIFY		1			// eeprom저장을 한번에 쓸수 있도록 - 에러 방지
#define HOME_CHECK_MODIFY_1ST		1		// 홈체크후 조금더 홈방향으로 이동
#define HOME_CHECK_MODIFY_2ND		2		// 홈체크후 바로 스톱 후 reset -> ready
#define HOME_CHECK_MODIFY		HOME_CHECK_MODIFY_1ST	//HOME_CHECK_MODIFY_2ND
*/
//#define MODIFY_HOME_CHECK		1
//#define MODIFY_HOME_Z_CHECK	1

// home check and go home
// 주입끝나고 홈으로 이동시 센서 체크
#define MODIFY_GO_HOME_AND_CHECK	0

// 인젝션동작후  sleep 모드로
#define INJECTION_END_MOTOR_SLEEP		1
// 시컨스 동작후 sleep 모드로
#define READY_MODE_MOTOR_SLEEP		1
// run 중 대기 상태에서 Sleep
//#define RUN_WAIT_MOTOR_SLEEP		1

// Power Down mode로 동작시킨다.

// 에러 발생후 COMMAND_INIT 명령을 받았을 경우 subMenu초기화
#define INIT_SUBMENU	         1

// Step 계산시 플로팅포인트 계산 에러 (goSyr())
//#define FLOAT_CAL_ERROR_FIX			1			// 삭제했음 20180920
//#define FLOAT_INPUT_GOSYR_FIX			1		// 삭제했음 20180920

// 20170927 인젝션볼륨입력 소수점 1자리
//#define FLOAT_INPUT_INJECTION_VOLUME			1

// 20170927 혼합프로그램볼륨입력 소수점 1자리
#define FLOAT_INPUT_MIX_VOLUME			1

#define VIAL_PLATE_CHECK				0	// vialPlateCheck

// 삭제했음 20180920
//#define POZ_UPPER_DETECT_MODIFY		1  // z축 이동시 upper check 추가 -upper에서 체크 안되고 detect위치에서 체크 되어야 함.

// z축아래로 이동시(Wash , Injector)  이동시 센서를 잘못 디텍션해서 에러 발생
// - 원인은 정확치 않으나 한 번더 센서 체크로 미봉책
//#define POZ_UPPER_DETECT_ERROR_FIX		1  // 20180920
#define POZ_UPPER_DETECT_ERROR_DELAY		1  // 20210604

#define POZ_UPPER_DETECT_ERROR_HOME_DELAY		1  // 20210614	// init_home

#define HOME_CHECK_RETRY_COUNT		3  // 20210614	// init_home 에러 발생시 재시도 



//#define MOVE_XY_DELAY_MODIFY	1		// 삭제했음 20180920
//#define WAKEUP_DELAY_MODIFY		1		// 삭제했음 20180920

// 여러번읽지 않고 한번에 읽어서 처리하도록 한다.
//#define EEPROM_READING_SPEED_UP		1		// 삭제했음 20191223

// 20170927 microPickInjection에서 사용
#define TRANSPORT_LIQUID_USE			1

//#define VALVE_HOME_CHECK_MODIFY		1
//#define INJECTOR_HOME_CHECK_MODIFY		1

// 20171213
// Fault 상태에서만 COMMAND_INIT동작
#define RECEIVE_COMMAND_INIT_FAULT		0
// 모든 상태에서 COMMAND_STOP 동작
#define RECEIVE_COMMAND_STOP_ANY		1

// 20200602
// initialize 중에도 명령 받음.
#define RECEIVE_COMMAND_INIT_ANY		1

// 20181010
// 초기에 저장데이터를 잃어 버린경우를 대비하여 보완
// 저장장소 백업
// 데이터에 대한 CheckSum
#define EEPROM_ADJUST_DATA_SAVE_FIX			1

// 20191205 - 소음줄이도록 - 기구마다 틀려서 적용안하는 것으로
#define MODIFY_MAX_SPEED_Z			1

// 20201130	-
// 밸브와 인젝터의 파워를 줄이고 z축을 동작(하강)시키면 소음이 발생한다.
// 이유는 잘 몰라 - 그라운드 연결 미흡
#define Z_MOTOR_NOISE_FIX				1
// 20191223 - 맥어드레스 생성방법변경 - 시리얼 입력시 생성되도록
#define MAKE_MAC_ADDR_SERIAL		1
	#define MAKE_MAC			0xAA
	#define LOAD_MAC			0x00
	#define APPLY_MAC			0x55

#define LOAD_IPADDRESS    0xAA
#define DEFAULT_IPADDRESS    0x55

#define MODIFY_MAX_SPEED_X		1
// 20191223 -- modify speed Y axis - 기구물 동작 불량 - 과부하로 오동작
#define MODIFY_MAX_SPEED_Y		1

//#define MODIFY_DEFAULT_SYRINGE_DOWN_SPEED			1		// SPEED_LEVEL_3

#define MODIFY_ADJUST_PRGM_TEST_FN	1

// 20200109 -- tcp_close 실행을 안하도록
#define FIX_TCP_CLOSE_EXCUTE_1			1
#define FIX_TCP_CLOSE_EXCUTE_2			1			// 괜찮은지 검초해야 된다. - 괜찮은 듯하다?

// 20200217 -- Driver IC 과열 방지
// Z축은 Sleep mode에서 Power down mode로 딥스위치 변경한다.
//#define POWER_DOWN_SYR_MODIFY		1	// Power Down - Syringe
//#define POWER_DOWN_Z_MODIFY		1		// Power Down - Z축 제외

// 20200514
//#define SYRINGE_HOME_FIND_FIX		1 // 홈찾기 FIX








// 20210611
#define HOME_FIND_TIME_CHECK_X		1 // 홈찾기 - 일정시간이후 못 찾으면 에러 
#define HOME_FIND_TIME_CHECK_Y		1 // 홈찾기 - 일정시간이후 못 찾으면 에러 
#define HOME_FIND_TIME_CHECK_Z		1 // 홈찾기 - 일정시간이후 못 찾으면 에러 

// 20220725
#define HOME_FIND_TIME_CHECK_SYR	1 // 홈찾기 - 일정시간이후 못 찾으면 에러 
#define HOME_FIND_TIME_CHECK_VAL	1 // 홈찾기 - 일정시간이후 못 찾으면 에러 
#define HOME_FIND_TIME_CHECK_INJ	1 // 홈찾기 - 일정시간이후 못 찾으면 에러 


// 20210615
#define Y_ENCODER_CHECK_ERR_MODIFY			1

// 20210618
#define CHECK_POSITION_MODIFY			1		// 에러발생시 checkPosition() 재실행
#if CHECK_POSITION_MODIFY
	#define Z_POS_CHECK_MASK				0x20
	#define Z_POS_CHECK1						0x30
	#define Z_POS_CHECK2						0x20
#else
	#define Z_POS_CHECK_MASK				0x30
	#define Z_POS_CHECK1						0x30
	#define Z_POS_CHECK2						0x20
#endif



//########################################################################################
// TCP_IP
//########################################################################################
#define		LWIP_VERSION_2				0

// 20210617
#define TCP_SERVER_RECV_MODIFY			1
// 20210618
#define TCP_SERVER_CONNECTION_ONLY_ONE			0


// 김창준 수석 TCP수정 
// lwipopts.h
#define KCJ_TCP_LWIPOPTS_H						1	
// lwIP Send/Recv 시 큐 증가 오류로 Ethernet Interrupt 동기화 처리.
#define KCJ_TCP_INTR			0



#define KCJ_TCP_CONNECTION_CLOSE				1


#define KCJ_TCP_SERVER		1					// 20210722


//#define KCJ_USED_RECV_QUEUE							1


#define USED_RECV_QUEUE		1	//(+)210715 CJKIM, Received data process changing

#define SERVER_CLOSE_RUN_STOP_CMD			0

// 20220720
// 220408 CJKIM , LWIP 2.2.0에서 적용한 것으로 1.3.2에서 적용 시 UVD 장비 리셋됨.
// LWIP에서는 사용하지 말것.
#define USE_tcp_send_empty_ack			0


//########################################################################################
// RS422
// 20210727
//########################################################################################
#define RS422_ENABLE			1

#define RS422_MY_ID					RS422_ID_AS

#define RS422_MASTER			0
#define RS422_SLAVE				1

#define RS422_DISABLE_INTR			1			// erhernet 연결되면 RS422인터럽트 비활성
#define RS422_SIZE_4BYTE_ALIGN 			1		// size -- 4 byte align

// 버퍼(큐)를 4BYte 단위로 저장
#define RS422_BUFFER_4BYTE		0
#define RS422_USE_MEMCPY 			0

#define RS422_MIX_STRUCT_DOWNSIZE			1

// 20211124
// RS422 연결끊어지면 (request 없으면 State or Diag등등 큐에 쌓지 않도록 수정)
//#define RS422_SEND_STATE_FIX					1
//#define RS422_DONOT_SEND_ACK

// 20211201
#define RS422_REQUEST_CMD_INDEX_ADD			1			// request(0xB0) 요청시 이전에 보냈던 PACKET과 INDEX를 추가해서 전송한다.




// =========================================================================
// 20200826
// Pull loop : Sampling loop 200u 오류 : 너무많은 양을 sampling한다.
#define SAMPLE_VOLUME_FIX_PULL_LOOP			1		// 20210826


//########################################################################################
// store information to envm
// SERVICE PROGRAM
// 20210910
//########################################################################################
#define STORE_INFORMATION_ENVM			1

#define INFORM_COMMON_STORAGE_USE			1			// serial / ip /mac 등 LC 공통 영역 사용

// 20211130
#define COMMAND_END_CMD_FOR_ANALYSYS_END			1			// clarity의 정상적인      분석종료일 때만 전송된다. - led 전환에 사용

//########################################################################################
// 20211222 - Cooling AS SF2 - PWM분해능 1024로 동작, 펌웨어는 256분해능으로 프로그램 되어있다.
#define AS_COOLING_OPTION			1

// 20220307 - Needle 검출 - 센서핀의 구조가 Chrozen에서 바뀌었다. - 1.5mm정도 차이 
// 바이알 등 감지할 때 1.5Cm정도 더 내려가야 감지(센서를 벗어나게) 된다.
// Chrozen만 반영한다.
#define CHROZEN_Z_HEIGHT_SENSOR_MODIFY		1

// 20220523
// 케이블이 뽑혀있을 경우 문제 z축 계속내려간다.
// 5mm정도 움직이도록 - 너무 작은가?.. 
// A/S 나 처음 조립시는 여러번 껏다가 켜기를 반복해야 한다. 
#define HOME_CHECK_STEP_CNT_REDUCE			1

// 20220729
#define RUN_WAIT_HOME_FIND_MODIFY			1


#endif	// #ifndef OPTION_AS_H_

