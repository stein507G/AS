#include "config.h"


#define	 MEMORY_16KB		( (16 * 1024) )
#define	 MEMORY_24KB		( (24 * 1024) )

#define	 MEMORY_SIZE		( MEMORY_24KB / 4 )		/* 16 K B = 16 KB /4 ptr increments by 4bytes*/

/* function to copy code to eSRAM*/
void copy_image_to_esram()
{
    unsigned int ii=0;
    unsigned long *exeDestAddr, *exeSrcAddr;

#ifdef DEBUG_MESSAGE
    MSS_UART_polled_tx_string(get_uart(),(uint8_t *)"Copy IAP code to eSRAM_0\n\r");
	MSS_UART_polled_tx_string(get_uart(),(uint8_t *)"\n\r");
#endif

	exeDestAddr = (unsigned long *)ESRAM_IAP_RUN_ADDR;
	exeSrcAddr = (unsigned long *)ENVM_IAP_IMAGE_ADDR;
	for (ii=0; ii < MEMORY_SIZE; ii++ )
	{
		*exeDestAddr++ = *exeSrcAddr++;
	}
}

/* function to remap eSRAM to cortex - M3 code region*/
void remap_user_code_eSRAM_0(void)
{
   int * address = (int *)(ESRAM_IAP_RUN_ADDR + 4); 		//pointer to reset handler of application

#ifdef DEBUG_MESSAGE
   MSS_UART_polled_tx_string(get_uart(),(uint8_t *)"Run IAP..... \r\n");
   MSS_UART_polled_tx_string(get_uart(),(uint8_t *)"\r\n");
#endif

   __set_MSP(*(int*)ESRAM_IAP_RUN_ADDR); 			//set the stack pointer to that of the application
   SYSREG->ESRAM_CR |= 0x1;

   SCB->VTOR = ESRAM_IAP_RUN_ADDR;

   ((void (*)())(*address))(); 				// pointer recast as function pointer and the dereferenced/called
   while(1){ }; 						    //This instruction never executed
}

void run_iap(void)
{
	copy_image_to_esram();
	remap_user_code_eSRAM_0();
}

void run_application(void)
{

#ifdef DEBUG_MESSAGE
	MSS_UART_polled_tx_string(get_uart(),(uint8_t *)"Run Application..... \n\r");
	MSS_UART_polled_tx_string(get_uart(),(uint8_t *)"\r\n");
#endif

	int * address = (int *)(ENVM_APP_IMAGE_ADDR + 4); 		//pointer to reset handler of application
	__set_MSP(*(int*)ENVM_APP_IMAGE_ADDR); 			//set the stack pointer to that of the application

	//SYSREG->ENVM_CR |= 0x1;

	SCB->VTOR = ENVM_APP_IMAGE_ADDR;

   ((void (*)())(*address))(); 				// pointer recast as function pointer and the dereferenced/called
   while(1){ }; 						    //This instruction never executed
}
