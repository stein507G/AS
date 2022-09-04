#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#define FPGA_BASE_ADDR   			(0x50000000U)
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
