#include <common/bk_include.h>
#include "bk_los_timer.h"
#include "bk_timer.h"
#include "los_context.h"
#include <os/os.h>
#include "arm_arch.h"
#include "los_interrupt.h"
#include "los_tick.h"
#include <driver/timer.h>
#include <components/system.h>

static OS_TICK_HANDLER g_tickHandler = NULL;

void HalTickHandler(u8 param)
{
    if (g_tickHandler != NULL) {
        g_tickHandler();
    }
}

unsigned int HalTickStart(OS_TICK_HANDLER handler)
{
	unsigned int intSave;

	intSave = LOS_IntLock();
	g_tickHandler = handler;

	bk_tick_init();
	LOS_IntRestore(intSave);

    return 0;
}

void HalTickLock(void)
{
	bk_timer_disable(LOS_TICK_TIMER_ID);
}

void HalTickUnlock(void)
{
	bk_timer_enable(LOS_TICK_TIMER_ID);
}

void HalClockIrqClear(void)
{
}

void HalSysTickReload(unsigned long long nextResponseTime)
{
	unsigned int intSave;
	unsigned long long nextResponseMs;

	intSave = LOS_IntLock();
	nextResponseMs = nextResponseTime * LOS_TICK_MS / OS_CYCLE_PER_TICK;
	bk_tick_reload(nextResponseMs);
	LOS_IntRestore(intSave);
}

unsigned long long HalGetTickCycle(unsigned int *period)
{
	unsigned long long currentTick;
	unsigned int intSave;
	
	intSave = LOS_IntLock();
	*period = bk_timer_get_period(LOS_TICK_TIMER_ID);
	currentTick = (unsigned long long)bk_timer_get_cnt(LOS_TICK_TIMER_ID);
	LOS_IntRestore(intSave);
	
	return currentTick;
}

UINT64 OsGetCurrSecond(VOID)
{
	return rtos_get_tick_count() * LOS_TICK_MS / 1000;
}
// eof

