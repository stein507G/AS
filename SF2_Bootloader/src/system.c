#include "config.h"

mss_uart_instance_t * get_uart()
{
	return &g_mss_uart0;
}


void MSS_UART0_enable_intr()
{
	NVIC_EnableIRQ(UART0_IRQn);
}

void MSS_UART0_disable_intr()
{
	NVIC_DisableIRQ(UART0_IRQn);
}

#ifdef DEBUG_MESSAGE
void intro()
{
	MSS_UART_polled_tx_string(get_uart(),(uint8_t *)"\n\r");
    MSS_UART_polled_tx_string(get_uart(),(uint8_t *)"************************************\n\r");
    MSS_UART_polled_tx_string(get_uart(), (uint8_t *)"    SmartFusion2 Bootloader v");
    MSS_UART_polled_tx_string(get_uart(), (uint8_t *)BL_FIRMWARE_VERSION_STRING);
    MSS_UART_polled_tx_string(get_uart(), (uint8_t *)"\n\r    ");
    MSS_UART_polled_tx_string(get_uart(), (uint8_t *)BL_FIRMWARE_DATE_STRING);
    MSS_UART_polled_tx_string(get_uart(), (uint8_t *)"\n\r");
    MSS_UART_polled_tx_string(get_uart(),(uint8_t *)"************************************\n\r");
    MSS_UART_polled_tx_string(get_uart(),(uint8_t *)"\n\r");
}
#endif

void init_system()
{
	/* Turn off the watchdog */
	SYSREG->WDOG_CR = 0;

	SCB->VTOR = ENVM_BOOTLOADER_IMAGE_ADDR;

	InitStorageInfo();

	// Check BL Version
    if( GetBLVer() != BL_FIRMWARE_VERSION ) {
    	// Save BL Version
    	SetBLVer(BL_FIRMWARE_VERSION);
//    	SaveStorageInfo();
    	SaveStorageInfo(DS_TYPE_SYSTEM);
    }

	MSS_UART_init(get_uart(), MSS_UART_921600_BAUD, MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);
	
	init_timer_1();

#if 1
// POWER BTN 설정
	CPU_PWR_CON_HIGH;
	FPGA_WRITE_WORD(W2_POWER_BTN_TIME, (ST_ON_BTN_DET_TIME_DEFAULT | (ST_OFF_SEQUENCE_TIME_DEFAULT << 16)));	// 버튼 누름 시간 , 버튼 인지 후 Off 시간

// LED 설정
//	InitialLED();

	BUZZER_CON_OFF;
//	BUZZER_CON_ON;
	EXT_LED_CON_OFF;
#endif


#ifdef DEBUG_MESSAGE
    intro();
#else
    //post_prog_run();		//211021 제거.
#endif

}

void reset_system()
{
	//SCB->VTOR = ENVM_BOOTLOADER_IMAGE_ADDR;

	NVIC_SystemReset();
}

