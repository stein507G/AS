/*
 * Interrupt.c
 *
 *  Created on: 2015. 1. 7.
 *      Author: chanii
 */

#include "config.h"

//.word  Reset_Handler
//.word  NMI_Handler

void prvGetRegistersFromStack(uint32_t *pulFaultStackAddress)
{
	/* These are volatile to try and prevent the compiler/linker optimising them
	 away as the variables never actually get used.  If the debugger won't show the
	 values of the variables, make them global my moving their declaration outside
	 of this function. */

	volatile unsigned int r0;
	volatile unsigned int r1;
	volatile unsigned int r2;
	volatile unsigned int r3;
	volatile unsigned int r12;
	volatile unsigned int lr; /* Link register. */
	volatile unsigned int pc; /* Program counter. */
	volatile unsigned int psr;/* Program status register. */

	r0 = pulFaultStackAddress[0];
	r1 = pulFaultStackAddress[1];
	r2 = pulFaultStackAddress[2];
	r3 = pulFaultStackAddress[3];

	r12 = pulFaultStackAddress[4];
	lr = pulFaultStackAddress[5];
	pc = pulFaultStackAddress[6];
	psr = pulFaultStackAddress[7];


}

__attribute__((__interrupt__)) void NMI_Handler(void)
{
	//reset_system();
	while (1);
}

__attribute__((__interrupt__)) void HardFault_Handler(void)
{

#if 0
	 __asm volatile
	 (
	 " tst lr, #4                                                \n"
	 " ite eq                                                    \n"
	 " mrseq r0, msp                                             \n"
	 " mrsne r0, psp                                             \n"
	 " ldr r1, [r0, #24]                                         \n"
	 " ldr r2, handler2_address_const                            \n"
	 " bx r2                                                     \n"
	 " handler2_address_const: .word prvGetRegistersFromStack    \n"
	 );
#else
	__asm volatile
	(
			" tst lr, #4                                                \n"
			" ite eq                                                    \n"
			" mrseq r0, msp                                             \n"
			" mrsne r0, psp                                             \n"
			" b prvGetRegistersFromStack                               \n"
	);
#endif
	//reset_system();
	while (1);
}

__attribute__((__interrupt__)) void MemManage_Handler(void)
{
	//reset_system();
	while (1);
}
__attribute__((__interrupt__)) void BusFault_Handler(void)
{
	//reset_system();
	while (1);
}
__attribute__((__interrupt__)) void UsageFault_Handler(void)
{
	//reset_system();
	while (1);
}

__attribute__((__interrupt__)) void WdogWakeup_IRQHandler(void)
{
	//reset_system();
	while (1);
}


__attribute__((__interrupt__)) void CommSwitchError_IRQHandler(void)
{
	//reset_system();
	while (1);
}

__attribute__((__interrupt__)) void CacheError_IRQHandler(void)
{
	//reset_system();
	while (1);
}


__attribute__((__interrupt__)) void ECC_Error_IRQHandler(void)
{
	//reset_system();
	while (1);
}
