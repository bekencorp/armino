#include "bk_fake_clock.h"
#include "bk_wdt.h"
#include "rthw.h"
#include "rtdef.h"

void bk_tick_handle(uint8_t arg)
{
	rt_tick_increase();

#if defined(RT_USING_ALARM)
	rt_alarm_update(NULL, 0);
#endif

#if (CONFIG_TASK_WDT)
	bk_task_wdt_timeout_check();
#endif
}

int bk_update_tick(uint32_t tick)
{
	if (tick) {
		rt_enter_critical();

		/* adjust OS tick */
		rt_tick_set(rt_tick_get() + tick);
		/* check system timer */
		rt_timer_check();
		rt_exit_critical();
	}

	return 0;
}

uint64_t bk_get_tick(void)
{
	return (uint64_t)rt_tick_get();
}

uint32_t bk_get_second(void)
{
	return (rt_tick_get() / RT_TICK_PER_SECOND);
}

uint32_t bk_get_ms_per_tick(void)
{
	return (1000/RT_TICK_PER_SECOND);
}

uint32_t bk_get_ticks_per_second(void)
{
	return RT_TICK_PER_SECOND;
}



