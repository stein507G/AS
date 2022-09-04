#ifndef DEFINE_AS_H_
#define DEFINE_AS_H_

#if QT_PROG_ADJUST
#else
#include "Chrozen_AS_hw_platform.h"		//#include "LCAS_SOC_2016_hw_platform.h"
#endif

#include "option_AS.h"

#if 1 // 20210429
// �������� ���� - ��Ʈ�� ����ȵ�.
#define SYSTEM_MODEL				"YI9650"
#define CONNECT_CHECK_MODEL				"YL9152 LC AutoSampler"
#else
#define SYSTEM_MODEL				"YL9152 LC AutoSampler"
#endif

// 20181012 - Adjust data ����� ���ؼ� ���Ӱ�
#define HOME_ADJUST_VERSION_CHECK_DATA	0x7234AA55


#define MECH_VER				7 // 8 //	6 // 5  // 3 - ���� ��  // 2 - ���ڸ�
#define BOARD_REV				3 // 2018
//#define BOARD_REV				4 // 20200422

#if MECH_VER == 6	// Board  Rev 3 // �ⱸ�� ���� 6 - ��ó�� ��õ� ��ǰ
//	#define NDEBUG_DP
// 20191228
	#define FIRMWARE_VERSION		" 1.1.3"	// �Ǿ��� space
	#define MACHINE_VER		" 1.3.6"				// 3: Board   6: Machine
#elif MECH_VER == 7// Board  Rev 3 // �ⱸ�� ���� 7
//	#define NDEBUG_DP
// �ⱸ�� ���� 7 - 20191223
// 20191228
//	#define FIRMWARE_VERSION		" 2.3.7"	// �Ǿ��� space
// 20200109 - Board  Rev 3 / �ⱸ�� ���� 8 - Cooling / Y / ... / Vial Height

//	#if NEW_BOARD_ENCODER_2020
// Eencoder �߰�  // 20200814
//		#define FIRMWARE_VERSION		" 3.4.9"	// �Ǿ��� space
// 20201204				// z�� �ӵ� ���� 
//		#define FIRMWARE_VERSION		" 3.4.12"	// �Ǿ��� space
// 20210125				// ChroZen AS : RGB LED ����
//		#define FIRMWARE_VERSION		" 4.0.1"	// �Ǿ��� space
// 20210429				// ChroZen AS : RGB LED ���� - ����
//		#define FIRMWARE_VERSION		" 5.04"	// �Ǿ��� space
//		#define MACHINE_VER		" 5.04"
// 20210601
// init command delay ���� : �����̰� �� ��Ŷó���� ���ϴ� ������ �� 
//		#define FIRMWARE_VERSION		" 5.05"	// �Ǿ��� space
//		#define MACHINE_VER		" 5.05"
// 20210609
//#if POZ_UPPER_DETECT_ERROR_DELAY 
//		#define FIRMWARE_VERSION		" 5.06"	// �Ǿ��� space
//		#define MACHINE_VER		" 5.06"
// 20210709
// KCJ -TCPIP
//		#define FIRMWARE_VERSION		" 5.10"	// �Ǿ��� space
//		#define MACHINE_VER		" 5.10"
// 20210723
// KCJ - TCPIP 
//		#define FIRMWARE_VERSION		" 5.11"	// �Ǿ��� space
//		#define MACHINE_VER		" 5.11"
// 20210730
// goPosETC_Z_check() ���� - error_16
//		#define FIRMWARE_VERSION		" 5.12"	// �Ǿ��� space
//		#define MACHINE_VER		" 5.12"
// 20210826
// Full loop : Sampling loop 200u ���� : �ʹ����� ���� sampling�Ѵ�.
//		#define FIRMWARE_VERSION		" 5.13"	// �Ǿ��� space
//		#define MACHINE_VER		" 5.13"

// 20210910
	// RTC disp ����
//		#define FIRMWARE_VERSION		" 5.14"	// �Ǿ��� space
//		#define MACHINE_VER		" 5.14"

// 20211222	-- Leak Error ���� : Stop �̳� Init ������� ���۾���.
//#define AS_COOLING_OPTION			1
//		#define FIRMWARE_VERSION		" 5.15"	// �Ǿ��� space
//		#define MACHINE_VER		" 5.15"

// 20220308
// #define CHROZEN_Z_HEIGHT_SENSOR_MODIFY		1
//		#define FIRMWARE_VERSION		" 5.16" // �Ǿ��� space
//		#define MACHINE_VER 	" 5.16"

//====================================================================================
// RS422
//====================================================================================
// 20211113
// #define RS422_ENABLE			1
// MIX_  table - 10���� - RS422_ENABLE
// �߿��� ������ FIRMWARE_VERSION���� �����Ѵ�.
	//#define	RS422_MIX_STRUCT_DOWNSIZE
	//#define MIX_TABLE_COUNT 	10
// 2021.11.1x - RS422�� ���� ��ġ
//		#define FIRMWARE_VERSION		" 5.20" // �Ǿ��� space
//		#define MACHINE_VER 	" 5.20"
//		#define AS_FIRMWARE_VERSION 	520 	// 20211115 �߰�		

// 20220308
// #define CHROZEN_Z_HEIGHT_SENSOR_MODIFY		1
//		#define FIRMWARE_VERSION		" 5.21" // �Ǿ��� space
//		#define MACHINE_VER 	" 5.21"
//		#define AS_FIRMWARE_VERSION 	521 	// 20211115 �߰�				


// 20220523
// #define HOME_CHECK_STEP_CNT_REDUCE			1
// 20220721
// �������� ����ȭ �۾�
// 20220729
//#define RUN_WAIT_HOME_FIND_MODIFY			1
// 20220803
//	#define VIAL_XY_CALIBRATION_FIX				1
		#define FIRMWARE_VERSION		" 5.22" // �Ǿ��� space
		#define MACHINE_VER 	" 5.22"
		#define AS_FIRMWARE_VERSION 	522 	// 20211115 �߰�				

//	#else
	// 20200513 - Board  Rev 3 & 4
//		#define FIRMWARE_VERSION		" 3.3.8"	// �Ǿ��� space
//		#define MACHINE_VER		" 3.3.8"				// 2: firmware 3: Board   8: Machine
//	#endif	
#endif

// �̵� 
//#define SYSTEM_DISP_MODEL		"YL9150 Plus LC AutoSampler"
#define SYSTEM_DISP_MODEL		"YI9650 HPLC AutoSampler"
#define SYSTEM_VERSION			MACHINE_VER








//########################################################################################################
#define FPGA_BASE_ADDR   		(0x50000000U)		// #define COREMEMCTRL_0     0x50000000U
//########################################################################################################
#define COMMON_CONTROL						0x0000U			// sel[14:12] = "000"
#define LED_SWITCH								0x1000U			// sel[14:12] = "001"
#define TRXM_CONTROL							0x2000U			// sel[14:12] = "010"
#define ADC_IF_MUX								0x3000U			// sel[14:12] = "011"
//#define STEPCONTROL18_0						0x4000U			// sel[14:12] = "100"
//#define STEPCONTROL18_1						0x5000U			// sel[14:12] = "101"
//#define STEPCONTROL13_0						0x6000U			// sel[14:12] = "110"
#define STEPCONTROL18_0						(FPGA_BASE_ADDR + (0x4000U << 2))			// sel[14:12] = "100"
#define STEPCONTROL18_1						(FPGA_BASE_ADDR + (0x5000U	<< 2))		// sel[14:12] = "101"
#define STEPCONTROL13_0						(FPGA_BASE_ADDR + (0x6000U	<< 2))		// sel[14:12] = "110"
#define PWM_CON8_0								0x7000U			// sel[14:12] = "111"
//########################################################################################################


//================================================================================
#define COMMON_CTRL_BASE_ADDR 				COMMON_CONTROL
// write
	#define  W1_START_CONFIG 				(COMMON_CONTROL + 0x00000001)	//					(COMMON_CTRL_BASE_ADDR + (0x00000001 << 2))
	//	0 : Rising Edge Detection
	//	1 : Falling Edge Detection

	#define  W2_LED_EXT_CON 				(COMMON_CONTROL + 0x00000002)	//					(COMMON_CTRL_BASE_ADDR + (0x00000002 << 2))
	#define  W3_BUZZER_CON	 				(COMMON_CONTROL + 0x00000003)	//					(COMMON_CTRL_BASE_ADDR + (0x00000003 << 2))
	#define  W4_MOTOR_POW_CON	 			(COMMON_CONTROL + 0x00000004)	//					(COMMON_CTRL_BASE_ADDR + (0x00000004 << 2))
	#define  W5_REMOTE_OUT	 				(COMMON_CONTROL + 0x00000005)	//					(COMMON_CTRL_BASE_ADDR + (0x00000005 << 2))
	#define  W6_START_OUT	 					(COMMON_CONTROL + 0x00000006)	//					(COMMON_CTRL_BASE_ADDR + (0x00000006 << 2))

//	#define  W10_ENABLE_STARTOUT			0x00000010	//		 		(COMMON_CTRL_BASE_ADDR + (0x00000010 << 2))
//	#define  W11_STARTOUT_PULSETIME_DATA	0x00000011	// 		(COMMON_CTRL_BASE_ADDR + (0x00000011 << 2))
// read
	#define  R0_FPGA_VER 					(COMMON_CONTROL + 0x00000000)	
	#define  R6_SIGNAL_IN 				(COMMON_CONTROL + 0x00000006)	//	[bit2: Power button] / [bit1: remote start] / [bit0:remote ready]
		

//================================================================================
#define LED_SWITCH_CTRL_BASE_ADDR 				LED_SWITCH
// write
	#define  W0_POWER_BTN 					(LED_SWITCH + 0x00000000)	//				(LED_SWITCH_CTRL_BASE_ADDR + (0x00000000 << 2))
		#define 	POW_BTN_EN					(0)			// bit
		#define		POW_CON_5V_IN 			(3)			// bit
	#define  T1_POW_BTN_DET_CLR 		(LED_SWITCH + 0x00000001)	// Touch
	#define  W2_POWER_BTN_TIME 			(LED_SWITCH + 0x00000002)
		#define		ST_ON_BTN_DET_TIME_DEFAULT			0x012C						// 300 x 10ms
//		#define 	ST_OFF_SEQUENCE_TIME_DEFAULT		0x02BC						// 500 x 10ms
		#define 	ST_OFF_SEQUENCE_TIME_DEFAULT		0x012C						// 300 x 10ms
		#define 	POWER_OFF_SEQUENCE_TIME					(ST_OFF_SEQUENCE_TIME_DEFAULT /	20)					// 500msec ����
		#define 	BUZZER_OFF_SEQUENCE_TIME				1					// 500msec				// CheckDeviceState()
		
	#define  T5_FLED_CONFIG_LOAD 		(LED_SWITCH + 0x00000005)	// Touch
	#define  W6_FLED_COLOR_TIME1		(LED_SWITCH + 0x00000006)
		#define 	FLED_COLOR1					0xFF0000			// 0xff0000						// Color ���� ( [23:16]Green / [15:8] Red / [7:0] Blue
//		#define 	FLED_COLOR1					0x800040									// Color ���� ( [23:16]Green / [15:8] Red / [7:0] Blue
		#define 	FLED_COLOR_TIME1				4										// ���� �ð� ���� : (100mSec ����) : 100msec x 10 = 1sec
	#define  W7_FLED_COLOR_TIME2		(LED_SWITCH + 0x00000007)
		#define 	FLED_COLOR2					0xFF0000				//0x00ff00
		#define 	FLED_COLOR_TIME2				5										// 100msec x 10 = 1sec
	#define  W8_FLED_COLOR_TIME3		(LED_SWITCH + 0x00000008)
		#define 	FLED_COLOR3					0xFF0000			//	0x0000ff
		#define 	FLED_COLOR_TIME3				1										// 100msec x 10 = 1sec


//================================================================================
#define PWM_CON_BASE_ADDR 				PWM_CON8_0
/*
	#define  S1_PWM1_OFFSET 		(PWM_CON8_0 + 0x00000000)		// PADDR[4:2]		parameter S1_PWM1 = 3'b000; --> b0 0000
	#define  S2_PWM2_OFFSET 		(PWM_CON8_0 + 0x00000004)		// PADDR[4:2]		parameter S2_PWM2 = 3'b001; --> b0 0100
	#define  S3_PWM3_OFFSET 		(PWM_CON8_0 + 0x00000008)		// PADDR[4:2]		parameter S3_PWM3 = 3'b010; --> b0 1000
	#define  S4_PWM4_OFFSET 		(PWM_CON8_0 + 0x0000000C)		// PADDR[4:2]		parameter S4_PWM4 = 3'b011; --> b0 1100
	#define  S5_CTRL_OFFSET 		(PWM_CON8_0 + 0x00000010)		// PADDR[4:2]		parameter S4_PWM4 = 3'b011; --> b1 0000

	#define S5_FAN_ON_OFF
	#define S6_HEAT_COOL
	#define S7_ON_OFF
	#define S8_PUMP_ON_OFF
*/

	#define  W0_PWM1_OFFSET 		(PWM_CON8_0 + 0x00000000)
	#define  W1_PWM2_OFFSET 		(PWM_CON8_0 + 0x00000001)
	#define  W2_PWM3_OFFSET 		(PWM_CON8_0 + 0x00000002)
	#define  W3_PWM4_OFFSET 		(PWM_CON8_0 + 0x00000003)
//	#define  W4_CTRL_OFFSET 		0x00000004
	#define  W4_FAN_ON_OFF 			(PWM_CON8_0 + 0x00000004)
	#define  W5_HEAT_COOL				(PWM_CON8_0 + 0x00000005)
	#define  W6_ON_OFF					(PWM_CON8_0 + 0x00000006)
	#define  W7_PUMP_ON_OFF			(PWM_CON8_0 + 0x00000007)




// ����������Ʈ ������ �ٲ۴�.
//#define  TWOPORTRAM_BASEADDR_MOTOR_X			STEPCONTROL18_0
//#define  TWOPORTRAM_BASEADDR_MOTOR_Y			STEPCONTROL18_1
//#define  TWOPORTRAM_BASEADDR_MOTOR_Z			STEPCONTROL18_2
//#define  TWOPORTRAM_BASEADDR_MOTOR_SYR		STEPCONTROL18_3
//#define  TWOPORTRAM_BASEADDR_MOTOR_VAL		STEPCONTROL13_0
//#define  TWOPORTRAM_BASEADDR_MOTOR_INJ		STEPCONTROL13_1

#define  TWOPORTRAM_BASEADDR_MOTOR_Z			STEPCONTROL18_0
#define  TWOPORTRAM_BASEADDR_MOTOR_Y			STEPCONTROL18_1
#define  TWOPORTRAM_BASEADDR_MOTOR_X			STEPCONTROL18_0
#define  TWOPORTRAM_BASEADDR_MOTOR_SYR		STEPCONTROL18_1
#define  TWOPORTRAM_BASEADDR_MOTOR_VAL		STEPCONTROL13_0
#define  TWOPORTRAM_BASEADDR_MOTOR_INJ		STEPCONTROL13_0

	#define  BASEADDR_MOTOR_X_Z					STEPCONTROL18_0
	#define  BASEADDR_MOTOR_Y_SYR				STEPCONTROL18_1
	#define  BASEADDR_MOTOR_VAL_INJ			STEPCONTROL13_0


	#define  BASEADDR_MOTOR_X					STEPCONTROL18_0
	#define  BASEADDR_MOTOR_Y					STEPCONTROL18_1
	#define  BASEADDR_MOTOR_Z					STEPCONTROL18_0
	#define  BASEADDR_MOTOR_SYR				STEPCONTROL18_1
	#define  BASEADDR_MOTOR_VAL				STEPCONTROL13_0
	#define  BASEADDR_MOTOR_INJ				STEPCONTROL13_0


//=============================================================================================
// BASEADDR_MOTOR
// 0 ~ 149 : Ram Data - Step data
// SF2
	#define  ACCEL_OFFSET 		(0x000003C0	<< 2)	// PADDR[9:3]  7'b1111000;  => b11 1100 0000 => 3C0
	#define  DEACC_OFFSET 		(0x000003C8	<< 2)		// PADDR[9:3]  7'b1111001;  => b11 1100 1000 => 3C8
	#define  TOTAL_OFFSET 		(0x000003D0	<< 2)		// PADDR[9:3]  7'b1111010;  => b11 1101 0000 => 3D0
	#define  START_SPEED  		(0x000003D8	<< 2)	// PADDR[9:3]  7'b1111011;  => b11 1101 1000 => 3D8
	#define  CTRL_PORT 		  	(0x000003E0	<< 2)		// PADDR[9:3]  7'b1111100;  => b11 1110 0000 => 3E0

	#define  S5_SEL_MOTOR  		(0x000003E8	<< 2)		// PADDR[9:3]  7'b1111101;  => b11 1110 1000 => 3E8
	#define  S6_COUNT_CTRL 		(0x000003F0	<< 2)		// PADDR[9:3]  7'b1111110;  => b11 1111 0000 => 3F0
	#define  S7_SEL_STATE			(0x000003F8	<< 2)		// 						 7'b1111111;	=> b11 1111 1000 => 3F8
//=============================================================================================

//	#define STEPCONTROL18_0						0x4000U			// sel[14:12] = "100"
//	#define  BASEADDR_MOTOR_X_Z					STEPCONTROL18_0

//	#define  CTRL_PORT_ADDR_MOTOR_X_Z				( FPGA_BASE_ADDR + ((BASEADDR_MOTOR_X_Z + CTRL_PORT) << 2) )

	
//=============================================================================================
	#define  CTRL_PORT_ADDR_MOTOR_X_Z				(BASEADDR_MOTOR_X_Z + CTRL_PORT)
	#define  CTRL_PORT_ADDR_MOTOR_Y_SYR			(BASEADDR_MOTOR_Y_SYR + CTRL_PORT)
//	#define  CTRL_PORT_ADDR_MOTOR_Z				(BASEADDR_MOTOR_Z + CTRL_PORT)
//	#define  CTRL_PORT_ADDR_MOTOR_SYR			(BASEADDR_MOTOR_SYR + CTRL_PORT)
	#define  CTRL_PORT_ADDR_MOTOR_VAL_INJ			(BASEADDR_MOTOR_VAL_INJ + CTRL_PORT)
//	#define  CTRL_PORT_ADDR_MOTOR_INJ			(BASEADDR_MOTOR_INJ + CTRL_PORT)

//=============================================================================================
	#define  SEL_MOTOR_ADDR_MOTOR_X_Z				(BASEADDR_MOTOR_X_Z + S5_SEL_MOTOR)
	#define  SEL_MOTOR_ADDR_MOTOR_Y_SYR			(BASEADDR_MOTOR_Y_SYR + S5_SEL_MOTOR)
	#define  SEL_MOTOR_ADDR_MOTOR_VAL_INJ		(BASEADDR_MOTOR_VAL_INJ + S5_SEL_MOTOR)

	#define  ENCODER_CTRL_ADDR_MOTOR_X_Z				(BASEADDR_MOTOR_X_Z + S6_COUNT_CTRL)
	#define  ENCODER_CTRL_ADDR_MOTOR_Y_SYR			(BASEADDR_MOTOR_Y_SYR + S6_COUNT_CTRL)
	#define  ENCODER_CTRL_ADDR_MOTOR_VAL_INJ		(BASEADDR_MOTOR_VAL_INJ + S6_COUNT_CTRL)

	#define  SEL_STATE_ADDR_MOTOR_X_Z				(BASEADDR_MOTOR_X_Z + S7_SEL_STATE)
	#define  SEL_STATE_ADDR_MOTOR_Y_SYR			(BASEADDR_MOTOR_Y_SYR + S7_SEL_STATE)
	#define  SEL_STATE_ADDR_MOTOR_VAL_INJ		(BASEADDR_MOTOR_VAL_INJ + S7_SEL_STATE)

//=============================================================================================
//bit3 ~ bit1 -> 0 ~ 5 : x,y,z,syr,val,inj
//bit4 -> 1 : hold0
//bit5 -> 1 : hold1

	#define CMD_SEL_MOTOR(WHO)	*((uint32_t volatile *)(devSet.selMotorAddr[WHO])) = WHO;

//	#define CMD_CTRL_PORT(WHO, CMD)	*((uint32_t volatile *)(devSet.ctrlPortAddr[WHO])) = (CMD);


//=============================================================================================
//bit0 -> 1 : Clear (1->0)
//bit1 -> 1 : hold
//bit2 -> 1 : enable
//���� bit3 -> 0 : up / 1 : Down
	#define CMD_ENCODER_CLR_CNT(WHO)		{	*((uint32_t volatile *)(devSet.encoderCtrlAddr[WHO])) = 0x01; *((uint32_t volatile *)(devSet.encoderCtrlAddr[WHO])) = 0x0;}
/*
	#define CMD_ENCODER_CLR_CNT_X_Z				*((uint32_t volatile *)(ENCODER_CTRL_ADDR_MOTOR_X_Z)) = 0x1; *((uint32_t volatile *)(ENCODER_CTRL_ADDR_MOTOR_X_Z)) = 0x0
	#define CMD_ENCODER_CLR_CNT_Y_SYR			*((uint32_t volatile *)(ENCODER_CTRL_ADDR_MOTOR_Y_SYR)) = 0x1; *((uint32_t volatile *)(ENCODER_CTRL_ADDR_MOTOR_Y_SYR)) = 0x0
	#define CMD_ENCODER_CLR_CNT_VAL_INJ		*((uint32_t volatile *)(ENCODER_CTRL_ADDR_MOTOR_VAL_INJ)) = 0x1; *((uint32_t volatile *)(ENCODER_CTRL_ADDR_MOTOR_VAL_INJ)) = 0x0
*/

// clear �� enable
	#define CMD_ENCODER_CLR_CNT_ENABLE(WHO)		{	*((uint32_t volatile *)(devSet.encoderCtrlAddr[WHO])) = 0x01; *((uint32_t volatile *)(devSet.encoderCtrlAddr[WHO])) = 0x4;}


	#define CMD_ENCODER_DISABLE(WHO)		{	*((uint32_t volatile *)(devSet.encoderCtrlAddr[WHO])) = 0x00;}
	#define CMD_ENCODER_ENABLE(WHO)		{	*((uint32_t volatile *)(devSet.encoderCtrlAddr[WHO])) = 0x04;}
	#define CMD_ENCODER_HOLD_CNT(WHO)		{	*((uint32_t volatile *)(devSet.encoderCtrlAddr[WHO])) = 0x06;}
	#define CMD_ENCODER_ACT_CNT(WHO)		{	*((uint32_t volatile *)(devSet.encoderCtrlAddr[WHO])) = 0x04;}

// 	110b (enable / hold / clear)
	#define CMD_ENCODER_HOLD_CNT_X_Z			*((uint32_t volatile *)(ENCODER_CTRL_ADDR_MOTOR_X_Z)) = 0x6
	#define CMD_ENCODER_ACT_CNT_X_Z				*((uint32_t volatile *)(ENCODER_CTRL_ADDR_MOTOR_X_Z)) = 0x4
	#define CMD_ENCODER_HOLD_CNT_Y_SYR		*((uint32_t volatile *)(ENCODER_CTRL_ADDR_MOTOR_Y_SYR)) = 0x6
	#define CMD_ENCODER_ACT_CNT_Y_SYR			*((uint32_t volatile *)(ENCODER_CTRL_ADDR_MOTOR_Y_SYR)) = 0x4
	#define CMD_ENCODER_HOLD_CNT_VAL_INJ	*((uint32_t volatile *)(ENCODER_CTRL_ADDR_MOTOR_VAL_INJ)) = 0x6
	#define CMD_ENCODER_ACT_CNT_VAL_INJ		*((uint32_t volatile *)(ENCODER_CTRL_ADDR_MOTOR_VAL_INJ)) = 0x4

	#define CMD_ENCODER_ENABLE_X_Z				*((uint32_t volatile *)(ENCODER_CTRL_ADDR_MOTOR_X_Z)) = 0x4
	#define CMD_ENCODER_DISABLE_X_Z				*((uint32_t volatile *)(ENCODER_CTRL_ADDR_MOTOR_X_Z)) = 0x0
	#define CMD_ENCODER_ENABLE_Y					*((uint32_t volatile *)(ENCODER_CTRL_ADDR_MOTOR_Y_SYR)) = 0x4
	#define CMD_ENCODER_DISABLE_SYR				*((uint32_t volatile *)(ENCODER_CTRL_ADDR_MOTOR_Y_SYR)) = 0x0
	#define CMD_ENCODER_ENABLE_VAL_INJ		*((uint32_t volatile *)(ENCODER_CTRL_ADDR_MOTOR_VAL_INJ)) = 0x4
	#define CMD_ENCODER_DISABLE_VAL_INJ		*((uint32_t volatile *)(ENCODER_CTRL_ADDR_MOTOR_VAL_INJ)) = 0x0

	#define CMD_SEL_STATE(WHO)	*((uint32_t volatile *)(devSet.selStateAddr[WHO])) = WHO;

// ============================================================================================================
// 20220729 test  - Step Count 
// ============================================================================================================
	#define CMD_CLR_STEP_COUNT(WHO) *((uint32_t volatile *)(devSet.baseAddr[WHO] + (0x301 << 2))) = 0x0
	#define CMD_EN_STEP_COUNT(WHO)	*((uint32_t volatile *)(devSet.baseAddr[WHO] + (0x302 << 2))) = 0x1
	#define CMD_DIS_STEP_COUNT(WHO)	*((uint32_t volatile *)(devSet.baseAddr[WHO] + (0x302 << 2))) = 0x0
	
	#define READ_STEP_COUNT(WHO) *((uint32_t volatile *)(devSet.baseAddr[WHO] + (0x1 << 2)))
// 20220729 End
// ============================================================================================================


// ============================================================================================================
// 20220808 - Check encoder direction
// ============================================================================================================
#if CHECK_ENCODER_DIRECTION
#if 0
//====
	#define CMD_CHK_DIR_RESET_Z 	*((uint32_t volatile *)(devSet.baseAddr[MOTOR_Z] + (0x308 << 2))) = 0x0
	#define CMD_CHK_DIR_READY_Z 	*((uint32_t volatile *)(devSet.baseAddr[MOTOR_Z] + (0x309 << 2))) = 0x0
	#define CMD_CHK_DIR_START_Z 	*((uint32_t volatile *)(devSet.baseAddr[MOTOR_Z] + (0x30A << 2))) = 0x0
	#define CMD_CHK_DIR_ACT_END_Z 	*((uint32_t volatile *)(devSet.baseAddr[MOTOR_Z] + (0x30B << 2))) = 0x0

// moveStepSpeed ��ɿ��� �Էµ� ���� ����ȴ�. -- �Ʒ� CMD_CHK_DIR_STATE_DIR_Z�� ������� ����.
// Syringe Motor�� ���ڴ��� �ݴ�� �����Ǿ� �ִ�. ��µǴ� ���� �ݴ���. - FPGA���� �ɼǻ������� �߰� ��. 
//	#define CMD_CHK_DIR_STATE_DIR_Z *((uint32_t volatile *)(devSet.baseAddr[MOTOR_Z] + (0x304 << 2)))
	#define CMD_CHK_DIR_OPTION_DIR_Z *((uint32_t volatile *)(devSet.baseAddr[MOTOR_Z] + (0x304 << 2)))

//====
	#define CMD_CHK_DIR_RESET_X 	*((uint32_t volatile *)(devSet.baseAddr[MOTOR_X] + (0x30C << 2))) = 0x0
	#define CMD_CHK_DIR_READY_X 	*((uint32_t volatile *)(devSet.baseAddr[MOTOR_X] + (0x30D << 2))) = 0x0
	#define CMD_CHK_DIR_START_X 	*((uint32_t volatile *)(devSet.baseAddr[MOTOR_X] + (0x30E << 2))) = 0x0
	#define CMD_CHK_DIR_ACT_END_X 	*((uint32_t volatile *)(devSet.baseAddr[MOTOR_X] + (0x30F << 2))) = 0x0

	#define CMD_CHK_DIR_OPTION_DIR_X *((uint32_t volatile *)(devSet.baseAddr[MOTOR_X] + (0x305 << 2)))

//====
	#define CMD_CHK_DIR_RESET_Y 	*((uint32_t volatile *)(devSet.baseAddr[MOTOR_Y] + (0x308 << 2))) = 0x0
	#define CMD_CHK_DIR_READY_Y 	*((uint32_t volatile *)(devSet.baseAddr[MOTOR_Y] + (0x309 << 2))) = 0x0
	#define CMD_CHK_DIR_START_Y 	*((uint32_t volatile *)(devSet.baseAddr[MOTOR_Y] + (0x30A << 2))) = 0x0
	#define CMD_CHK_DIR_ACT_END_Y 	*((uint32_t volatile *)(devSet.baseAddr[MOTOR_Y] + (0x30B << 2))) = 0x0

	#define CMD_CHK_DIR_OPTION_DIR_Y	*((uint32_t volatile *)(devSet.baseAddr[MOTOR_Y] + (0x304 << 2)))

//====
	#define CMD_CHK_DIR_RESET_SYR 	*((uint32_t volatile *)(devSet.baseAddr[MOTOR_SYR] + (0x30C << 2))) = 0x0
	#define CMD_CHK_DIR_READY_SYR 	*((uint32_t volatile *)(devSet.baseAddr[MOTOR_SYR] + (0x30D << 2))) = 0x0
	#define CMD_CHK_DIR_START_SYR 	*((uint32_t volatile *)(devSet.baseAddr[MOTOR_SYR] + (0x30E << 2))) = 0x0
	#define CMD_CHK_DIR_ACT_END_SYR 	*((uint32_t volatile *)(devSet.baseAddr[MOTOR_SYR] + (0x30F << 2))) = 0x0

	#define CMD_CHK_DIR_OPTION_DIR_SYR *((uint32_t volatile *)(devSet.baseAddr[MOTOR_SYR] + (0x305 << 2)))
#else
	#define CMD_CHK_DIR_RESET(WHO)		*((uint32_t volatile *)(devSet.cmdChkDir_RESET_Addr[WHO])) = 0x0
	#define CMD_CHK_DIR_READY(WHO)		*((uint32_t volatile *)(devSet.cmdChkDir_READY_Addr[WHO])) = 0x0
	#define CMD_CHK_DIR_START(WHO)		*((uint32_t volatile *)(devSet.cmdChkDir_START_Addr[WHO])) = 0x0
	#define CMD_CHK_DIR_ACT_END(WHO)	*((uint32_t volatile *)(devSet.cmdChkDir_ACT_END_Addr[WHO])) = 0x0

// moveStepSpeed ��ɿ��� �Էµ� ���� ����ȴ�. -- �Ʒ� CMD_CHK_DIR_STATE_DIR_Z�� ������� ����.
// Syringe Motor�� ���ڴ��� �ݴ�� �����Ǿ� �ִ�. ��µǴ� ���� �ݴ���. - FPGA���� �ɼǻ������� �߰� ��. 
	#define CMD_CHK_DIR_OPTION_DIR_Z 	*((uint32_t volatile *)(devSet.baseAddr[MOTOR_Z] + (0x304 << 2)))		// fpga RD_CHK_DIR_STATE_0
	#define CMD_CHK_DIR_OPTION_DIR_X 	*((uint32_t volatile *)(devSet.baseAddr[MOTOR_X] + (0x305 << 2)))		// fpga RD_CHK_DIR_STATE_1
	#define CMD_CHK_DIR_OPTION_DIR_Y	*((uint32_t volatile *)(devSet.baseAddr[MOTOR_Y] + (0x304 << 2)))		// fpga RD_CHK_DIR_STATE_0
	#define CMD_CHK_DIR_OPTION_DIR_SYR 	*((uint32_t volatile *)(devSet.baseAddr[MOTOR_SYR] + (0x305 << 2)))		// fpga RD_CHK_DIR_STATE_1
#endif

#if 0	
// READ_CHK_DIR_Z - bit[1:0]-DIr / bit[5:2]-Pos / bit[7]Break / bit[6] Error
	#define READ_CHK_DIR_Z		*((uint32_t volatile *)(devSet.baseAddr[MOTOR_Z] + (0x4 << 2))) 	// ���� [15:0]
// READ_CHK_DIR_X - bit[17:16]-DIr / bit[21:18]-Pos / bit[23]Break / bit[22] Error
	#define READ_CHK_DIR_X		*((uint32_t volatile *)(devSet.baseAddr[MOTOR_X] + (0x5 << 2)))		// ���� [31:16]
	#define READ_CHK_DIR_Y		*((uint32_t volatile *)(devSet.baseAddr[MOTOR_Y] + (0x4 << 2))) 	// ���� [15:0]
	#define READ_CHK_DIR_SYR	*((uint32_t volatile *)(devSet.baseAddr[MOTOR_SYR] + (0x5 << 2)))		// ���� [31:16]
#else
	#define CHK_DIR_CW				0x0
	#define	CHK_DIR_CCW				0x1
	#define	CHK_DIR_ERR				0x2
	#define	CHK_DIR_NONE			0x3

	#define READ_CHK_DIR(WHO)	*((uint32_t volatile *)(devSet.read_ChkDir_Addr[WHO]))
#endif
	#define READ_CHK_ZX		*((uint32_t volatile *)(devSet.baseAddr[MOTOR_Z] + (0x2 << 2)))		// ���� [15:0]
	#define READ_CHK_YS 	*((uint32_t volatile *)(devSet.baseAddr[MOTOR_Y] + (0x2 << 2))) 	// ���� [15:0]
	
#else
	#define CMD_CHK_DIR_RESET_Z
	#define CMD_CHK_DIR_READY_Z
	#define CMD_CHK_DIR_START_Z
	#define CMD_CHK_DIR_ACT_END_Z

	#define CMD_CHK_DIR_OPTION_DIR_Z

#define READ_CHK_ZX
	#define READ_CHK_DIR_Z

	#define CMD_CHK_DIR_RESET_X
	#define CMD_CHK_DIR_READY_X
	#define CMD_CHK_DIR_START_X
	#define CMD_CHK_DIR_ACT_END_X

	#define CMD_CHK_DIR_OPTION_DIR_X

	#define READ_CHK_DIR_X

	#define CMD_CHK_DIR_RESET_Y
	#define CMD_CHK_DIR_READY_Y
	#define CMD_CHK_DIR_START_Y
	#define CMD_CHK_DIR_ACT_END_Y

	#define CMD_CHK_DIR_OPTION_DIR_Y

#define READ_CHK_YS
	#define READ_CHK_DIR_Y
//====
	#define CMD_CHK_DIR_RESET_SYR
	#define CMD_CHK_DIR_READY_SYR
	#define CMD_CHK_DIR_START_SYR
	#define CMD_CHK_DIR_ACT_END_SYR

	#define CMD_CHK_DIR_OPTION_DIR_SYR

	#define READ_CHK_DIR_SYR
#endif
// 20220808 End
// ============================================================================================================



//#define READ_CHK_DIR(WHO)		*((uint32_t volatile *)(devSet.baseAddr[WHO] + (0x4 << 2) + (((WHO) & 0x1) << 2))		// ���� [31:16]

//#define READ_CHK_DIR(WHO)		*((uint32_t volatile *)(devSet.baseAddr[WHO] + (0x4 + WHO) << 2)			// addr : x(4) , Y(5), z(6), Syr(7)




// ex)
	//(*((uint32_t volatile *)(FABRIC_STEPSET_APB20_BASE_ADDR + CTRL_PORT)) = ctrl_port);

//core_gpio.h -- #include "cpu_types.h" ���� �ڲٿ����� ����.
// ��� ���� ���� ������ �ٲ۴�. - property�� �������� ��� ���ϼ���
// ${workspace_loc:/Webserver_MSS_CM3_0_hw_platform/hal}
// ${workspace_loc:/Webserver_MSS_CM3_0_hw_platform/hal/CortexM3}
// ${workspace_loc:/Webserver_MSS_CM3_0_hw_platform/hal/CortexM3/GNU} �ڷ� �̵�

// �ߺ�����
// addr_t







//    static
extern unsigned char bInitTicks;

// Simple periodic polling function
#define PERIODIC_FUNCTION(var,time,function,err) \
		get_System_ticks(); \
    if((sysTicks - var) > time) \
    {                               \
        var += time;                \
        err = function;             \
    }

#if SOC_TYPE_SF2
#define NEXT_CASE_DELAY_INTERVAL		300
#define NEXT_CASE_DELAY_RUN					500
#define STEP_DELAY_INTERVAL		10			// 10ms
#define WAIT_AFTER_INJECTION				5000			// 5��

#define WAIT_HOME_FIND_INTERVAL_100MS		100
#define WAIT_HOME_FIND_INTERVAL_200MS		200	//2000
#define WAIT_HOME_FIND_INTERVAL		WAIT_HOME_FIND_INTERVAL_200MS

#define WAIT_MIX_PROGRAM				500			// 0.5��
#define WAIT_MICRO_PUMP_START				500			// 0.5��

#define MICRO_PUMP_TIME_CONVERSION	1000		// �ʴ����κ�ȯ 

#define WAKEUP_DELAY_TIME			500		// 0.5��

#define CONVERT_WAIT_TIME_TO_COUNT			1000		//  time(sec) to time tick

#define MOVE_XY_DELAY_TIME		200		// 0.2��

#define DELAY_TIME_TICK_COUNT_0_1_SEC		100		// 0.1��
#define DELAY_TIME_TICK_COUNT_2_SEC		2000		// 2��

//#define DELAY_Z_ERROR_RECHECK_TIME		200		// 200ms
#define DELAY_Z_ERROR_RECHECK_TIME		5000 //500		// 500ms  - 20210730

#define HOME_FINE_CHECK_MAX_TIME_X		7000	// 7�� Ȩ ã�� 
#define HOME_FINE_CHECK_MAX_TIME_Y		7000	// 7�� Ȩ ã�� 
#define HOME_FINE_CHECK_MAX_TIME_Z		7000	// 7�� Ȩ ã�� 
#define HOME_FINE_CHECK_MAX_TIME_SYR		7000	// 7�� Ȩ ã�� 
#define HOME_FINE_CHECK_MAX_TIME_VAL		7000	// 7�� Ȩ ã�� 
#define HOME_FINE_CHECK_MAX_TIME_INJ		7000	// 7�� Ȩ ã�� 

#else
#define NEXT_CASE_DELAY_INTERVAL		3000
#define NEXT_CASE_DELAY_RUN					5000
#define STEP_DELAY_INTERVAL		100			// 10ms
#define WAIT_AFTER_INJECTION				50000			// 5��

#define WAIT_HOME_FIND_INTERVAL_100MS		1000
#define WAIT_HOME_FIND_INTERVAL_200MS		2000	//2000
#define WAIT_HOME_FIND_INTERVAL		WAIT_HOME_FIND_INTERVAL_200MS

#define WAIT_MIX_PROGRAM				5000			// 0.5��
#define WAIT_MICRO_PUMP_START				5000			// 0.5��

#define MICRO_PUMP_TIME_CONVERSION	10000

#define WAKEUP_DELAY_TIME			5000		// 0.5��

#define CONVERT_WAIT_TIME_TO_COUNT			10000		//  time(sec) to time tick

#define MOVE_XY_DELAY_TIME		2000		// 0.2��

#define DELAY_TIME_TICK_COUNT_0_1_SEC		1000		// 0.1��
#define DELAY_TIME_TICK_COUNT_2_SEC		20000		// 2��

#define DELAY_Z_ERROR_RECHECK_TIME		2000		// 200ms

#define HOME_FINE_CHECK_MAX_TIME_X		70000	// 7�� Ȩ ã�� 
#define HOME_FINE_CHECK_MAX_TIME_Y		70000	// 7�� Ȩ ã�� 
#define HOME_FINE_CHECK_MAX_TIME_Z		70000	// 7�� Ȩ ã�� 
#define HOME_FINE_CHECK_MAX_TIME_SYR	70000	// 7�� Ȩ ã�� 
#define HOME_FINE_CHECK_MAX_TIME_VAL	70000	// 7�� Ȩ ã�� 
#define HOME_FINE_CHECK_MAX_TIME_INJ	70000	// 7�� Ȩ ã�� 

#endif


#define GO_NEXT_CASE_TIME(var,time,value) \
		get_System_ticks(); \
    if((sysTicks - (var)) > (time)) \
    { \
        var += time; \
        ++value; \
    }

#define  WAIT_TIME_VALUE_CASE_STATE(VALUE,TIME)  \
	if(bInitTicks == NO) {	\
			init_ticks(&nextCaseDelay);	\
			bInitTicks = YES; \
	}	\
	else {	\
			get_System_ticks();	\
	    if((sysTicks - nextCaseDelay) > (TIME))	{ \
	        ++VALUE; \
	        bInitTicks = NO; \
	    } \
	}

#define	RYU_Delay(delay) \
	{ \
	  uint32_t tickstart = 0;	\
	  tickstart = get_Systicks(); \
	  while((get_Systicks() - tickstart) < (delay))	 {} \
	}
// ex)
	//(*((uint32_t volatile *)(COREGPIO_BASE_ADDR + GPIO_OUT_REG_OFFSET)) = (gpio_pattern & 0xfffffffe));



// CoreGpio Reading
#define POWER_SW_OFF_TIME			50		// 5sec

#define SWITCH_1_ID						0
#define SWITCH_2_ID						1
#define DOOR_ERROR						2
#define START_IN_ID						3
#define READY_IN_ID						4
#define LEAK_SEN_ID						5
#define CPU_ONOFF_DET_ID			6
#define POWER_BUTTON_ID				CPU_ONOFF_DET_ID


#define FIRST_YEAR			2001u
#define LAST_YEAR			2099u
#define SECOND_YEAR			86400u


#define TEMP_SUM_TIME		16  // 8

#if TEMP_SUM_TIME == 16
  #define TEMP_ADC_SUM		16000
  #define TEMP_AVE_DIV		4

	#define SUM_DIGIT_TO_EXT_TEMP_FACTOR		1.86264514923095703125e-6		 // 500(10mV) / 2^24 / 16 = 500 / 2^28
#elif TEMP_SUM_TIME == 8
  #define TEMP_ADC_SUM		8000
  #define TEMP_AVE_DIV		3

	#define SUM_DIGIT_TO_EXT_TEMP_FACTOR		3.7252902984619140625e-6		 // 500(10mV) / 2^24 / 8 = 500 / 2^27
#else
  #define TEMP_ADC_SUM		4000
  #define TEMP_AVE_DIV		2

	#define SUM_DIGIT_TO_EXT_TEMP_FACTOR		7.450580596923828125e-6  		 // 500(10mV) / 2^24 / 4 = 500 / 2^26	 ���� �� ��.
#endif

#if 0
//======================================================
// X40
/*	
  #define MAIN_TEMP_CONST_A		6.1357931E-14
  #define MAIN_TEMP_CONST_B		1.0832671e5 
  #define MAIN_TEMP_CONST_C		-11.981412
*/
// ���� 
//y = 6.9082889E-01x2 + 3.6348411E+01x - 1.1981412E+01
  #define MAIN_TEMP_CONST_A		0.6993407 
  #define MAIN_TEMP_CONST_B		36.3269082
  #define MAIN_TEMP_CONST_C		-11.9732419
/*	
// moving 8
//y = 1.0794201E-02x2 + 4.5435513E+00x - 1.1981412E+01		
  #define MAIN_TEMP_CONST_A		1.0794201E-02 
  #define MAIN_TEMP_CONST_B		4.5435513
  #define MAIN_TEMP_CONST_C		-11.9732419
*/
FD//======================================================
	#else
//======================================================
// X60
/*	
	#define MAIN_TEMP_CONST_A		2.7270192E-14
  #define MAIN_TEMP_CONST_B		7.2217804E-06
  #define MAIN_TEMP_CONST_C		-1.1981412E+01
*/
// ���� 
//y = 3.0703506E-01x2 + 2.4232274E+01x - 1.1981412E+01
	#define MAIN_TEMP_CONST_A		0.30703506
  #define MAIN_TEMP_CONST_B		24.232274
  #define MAIN_TEMP_CONST_C		-11.981412
/*  
// moving 8
//y = 4.7974228E-03x2 + 3.0290342E+00x - 1.1981412E+01R�� = 9.9999981E-01
	#define MAIN_TEMP_CONST_A		4.7974228E-03
  #define MAIN_TEMP_CONST_B		3.0290342
  #define MAIN_TEMP_CONST_C		-11.981412
*/
//======================================================
#endif

	#define DIGIT_TO_VOLT_FACTOR				2.98023223876953125e-7	// 5volt / 2^24
	#define DIGIT_TO_EXT_TEMP_FACTOR		2.98023223876953125e-5	// 500(10mV) / 2^24

	#define POW_V33_DIGIT_TO_VOLT_FACTOR		(DIGIT_TO_VOLT_FACTOR * 2.0f)
	#define POW_V5_DIGIT_TO_VOLT_FACTOR		(DIGIT_TO_VOLT_FACTOR * 3.0f)
	#define POW_V5N_DIGIT_TO_VOLT_FACTOR		(DIGIT_TO_VOLT_FACTOR * 3.0f)			// �������.
	#define POW_V12_DIGIT_TO_VOLT_FACTOR		(DIGIT_TO_VOLT_FACTOR * 11.0f)





#endif	// #ifndef DEFINE_AS_H_

