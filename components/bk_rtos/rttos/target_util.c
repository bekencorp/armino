#include <common/bk_include.h>
#include "bk_arm_arch.h"

#include "bk_misc.h"
#include <rtthread.h>
/*******************************************************************************
* Function Implemantation
*******************************************************************************/
/*
	MCLK:26MHz, delay(1): about 25us
				delay(10):about 125us
				delay(100):about 850us
 */
void delay(INT32 num)
{
	volatile INT32 i, j;

	for (i = 0; i < num; i ++) {
		for (j = 0; j < 100; j ++)
			;
	}
}

/*
	when parameter is 1, the return result is approximately 1 ms;
 */
void delay_ms(UINT32 ms_count)
{
	rt_thread_delay(rt_tick_from_millisecond(ms_count));
}

/*
	[delay offset]worst case: delay about 1 second;
 */
void delay_sec(UINT32 ms_count)
{
	rt_thread_delay(rt_tick_from_millisecond(ms_count * 1000));
}

/*
	[delay offset]worst case: delay about 1 tick;
 */
void delay_tick(UINT32 tick_count)
{
	rt_thread_delay(tick_count);
}

// EOF
