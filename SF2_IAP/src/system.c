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

void intro()
{
	MSS_UART_polled_tx_string(get_uart(),(uint8_t *)"\n\r");
    MSS_UART_polled_tx_string(get_uart(),(uint8_t *)"************************************\n\r");
    MSS_UART_polled_tx_string(get_uart(), (uint8_t *)"    SmartFusion2 IAP v");
    MSS_UART_polled_tx_string(get_uart(), (uint8_t *)IAP_FIRMWARE_VERSION_STRING);
    MSS_UART_polled_tx_string(get_uart(), (uint8_t *)"\n\r    ");
    MSS_UART_polled_tx_string(get_uart(), (uint8_t *)IAP_FIRMWARE_DATE_STRING);
    MSS_UART_polled_tx_string(get_uart(), (uint8_t *)"\n\r");
    MSS_UART_polled_tx_string(get_uart(),(uint8_t *)"************************************\n\r");

    MSS_UART_polled_tx_string(get_uart(), (uint8_t *)"\n\r");
	MSS_UART_polled_tx_string(get_uart(), (uint8_t *)"Ready to update.....  \n\r");
	MSS_UART_polled_tx_string(get_uart(), (uint8_t *)"\n\r");

}

void init_system()
{
	/* Turn off the watchdog */
	SYSREG->WDOG_CR = 0;

	SCB->VTOR = ESRAM_IAP_RUN_ADDR;

	init_queue();

    InitStorageInfo();

	// Check IAP Version
    if( GetIAPVer() != IAP_FIRMWARE_VERSION ) {
    	// Save IAP Version
    	SetIAPVer(IAP_FIRMWARE_VERSION);
    	SaveStorageInfo(DS_TYPE_SYSTEM);
    }

    MSS_UART_init(get_uart(), MSS_UART_921600_BAUD, MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);
    MSS_UART_set_rx_handler(get_uart(), uart0_rx_handler, MSS_UART_FIFO_SINGLE_BYTE);

    init_timer_1();

#ifdef DEBUG_MESSAGE
    intro();
#else
    //post_prog_run();		//211021 Á¦°Å.
#endif
}

void reset_system()
{
	//SCB->VTOR = ENVM_BOOTLOADER_IMAGE_ADDR;

	NVIC_SystemReset();
}
