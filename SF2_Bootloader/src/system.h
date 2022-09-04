#ifndef _SYSTEM_H_
#define _SYSTEM_H_
// ===================================================================
// Oven
// ===================================================================
#define FPGA_BASE_ADDR   		(0x50000000U)				// #define COREMEMCTRL_0                   0x50000000U

#define COMMON_CONTROL						0x0000U			// sel[14:12] = "000"
#define LED_SWITCH								0x1000U			// sel[14:12] = "001"
#define TRXM_CONTROL							0x2000U			// sel[14:12] = "010"
#define ADC_IF_MUX								0x3000U			// sel[14:12] = "011"
#define STEPCONTROL13_0						0x4000U			// sel[14:12] = "100"
#define STEPCONTROL13_1						0x5000U			// sel[14:12] = "101"
#define STEPCONTROL18_0						0x6000U			// sel[14:12] = "110"
#define PWM_CON8_0								0x7000U			// sel[14:12] = "111"

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
	#define  R0_FPGA_VER 					(COMMON_CONTROL + 0x00000000)	//						(COMMON_CTRL_BASE_ADDR + (0x00000000 << 2))
	#define  R6_SIGNAL_IN 				(COMMON_CONTROL + 0x00000006)	//			(COMMON_CTRL_BASE_ADDR + (0x00000006 << 2))

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
		#define 	POWER_OFF_SEQUENCE_TIME					(ST_OFF_SEQUENCE_TIME_DEFAULT /	20)					// 500msec 단위
		#define 	BUZZER_OFF_SEQUENCE_TIME				1					// 500msec				// CheckDeviceState()

	#define  T5_FLED_CONFIG_LOAD 		(LED_SWITCH + 0x00000005)	// Touch
	#define  W6_FLED_COLOR_TIME1		(LED_SWITCH + 0x00000006)
		#define 	FLED_COLOR1					0xFF0000			// 0xff0000						// Color 설정 ( [23:16]Green / [15:8] Red / [7:0] Blue
//		#define 	FLED_COLOR1					0x800040									// Color 설정 ( [23:16]Green / [15:8] Red / [7:0] Blue
		#define 	FLED_COLOR_TIME1				4										// 유지 시간 설정 : (100mSec 단위) : 100msec x 10 = 1sec
	#define  W7_FLED_COLOR_TIME2		(LED_SWITCH + 0x00000007)
		#define 	FLED_COLOR2					0xFF0000				//0x00ff00
		#define 	FLED_COLOR_TIME2				5										// 100msec x 10 = 1sec
	#define  W8_FLED_COLOR_TIME3		(LED_SWITCH + 0x00000008)
		#define 	FLED_COLOR3					0xFF0000			//	0x0000ff
		#define 	FLED_COLOR_TIME3				1										// 100msec x 10 = 1sec


//================================================================================
#define PWM_CON_BASE_ADDR 				PWM_CON8_0

	#define  W0_PWM1_OFFSET 		(PWM_CON8_0 + 0x00000000)
	#define  W1_PWM2_OFFSET 		(PWM_CON8_0 + 0x00000001)
	#define  W2_PWM3_OFFSET 		(PWM_CON8_0 + 0x00000002)
	#define  W3_PWM4_OFFSET 		(PWM_CON8_0 + 0x00000003)
//	#define  W4_CTRL_OFFSET 		0x00000004
	#define  W4_FAN_ON_OFF 			(PWM_CON8_0 + 0x00000004)
	#define  W5_HEAT_COOL				(PWM_CON8_0 + 0x00000005)
	#define  W6_ON_OFF					(PWM_CON8_0 + 0x00000006)
	#define  W7_PUMP_ON_OFF			(PWM_CON8_0 + 0x00000007)


//================================================================================
//===========================================================================================

#define  TWOPORTRAM_BASEADDR_MOTOR			STEPCONTROL13_0

//#define FPGA_BASE_ADDR   			(0x50000000U)
#define FPGA_WRITE_WORD(X,Y)		*(volatile uint32_t *) (FPGA_BASE_ADDR + ((X)<<2)) = (Y)
#define FPGA_READ_WORD(X)			*(volatile uint32_t *) (FPGA_BASE_ADDR + ((X)<<2))

#define LED_W_Addr			0x400D
#define LED_ON				0x0
#define LED_OFF				0x1
#define LED_OnOff(X)		FPGA_WRITE_WORD(LED_W_Addr, X)


// ===================================================================
// Oven
// ===================================================================
#define CPU_PWR_CON_LOW			FPGA_WRITE_WORD(W0_POWER_BTN, ((0 << POW_CON_5V_IN) | (1 << POW_BTN_EN)))
#define CPU_PWR_CON_HIGH		FPGA_WRITE_WORD(W0_POWER_BTN, ((1 << POW_CON_5V_IN) | (1 << POW_BTN_EN)))

#define BUZZER_CON_OFF			FPGA_WRITE_WORD(W3_BUZZER_CON, 0x0)
#define BUZZER_CON_ON				FPGA_WRITE_WORD(W3_BUZZER_CON, 0x1)

#define EXT_LED_CON_OFF			FPGA_WRITE_WORD(W2_LED_EXT_CON, 0x0)
#define EXT_LED_CON_ON			FPGA_WRITE_WORD(W2_LED_EXT_CON, 0x1)


#define FPGA_WRITE_WORD(X,Y)		*(volatile uint32_t *) (FPGA_BASE_ADDR + ((X)<<2)) = (Y)
#define FPGA_READ_WORD(X)			*(volatile uint32_t *) (FPGA_BASE_ADDR + ((X)<<2))

#define LED_W_Addr			0x400D

#define LED_ON				0x0
#define LED_OFF				0x1

#define LED_OnOff(X)		FPGA_WRITE_WORD(LED_W_Addr, X)

mss_uart_instance_t * get_uart();


void MSS_UART0_enable_intr();
void MSS_UART0_disable_intr();

void intro();

void init_system();
void reset_system();

#endif /* __SYSTEM_H__ */
