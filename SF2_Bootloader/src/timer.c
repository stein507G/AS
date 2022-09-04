#include "config.h"

volatile uint32_t Time1mS = 0;

__attribute__((__interrupt__)) void Timer1_IRQHandler(void)
{
	Time1mS++;

	MSS_TIM1_clear_irq(); /* Clear TIM1 interrupt */
}

void init_timer_1()
{
	uint32_t timer1_load_value;

    timer1_load_value = MSS_SYS_APB_0_CLK_FREQ / 1000;	// 1ms Timer Value
	MSS_TIM1_init( MSS_TIMER_PERIODIC_MODE );
	MSS_TIM1_load_immediate( timer1_load_value );
	MSS_TIM1_start();
	MSS_TIM1_enable_irq();
}

uint32_t get_mili_time()
{
	return Time1mS;
}

uint32_t diff_mili_time( uint32_t time)
{
	uint32_t elapse;

	if(get_mili_time() >= time) {
		elapse = get_mili_time() - time;
	}
	else {
		elapse = (UINT_MAX - time) -  get_mili_time();
	}

	return elapse;
}

