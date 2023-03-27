#include <common/bk_include.h>
#include "portmacro.h"
#include "riscv_hal.h"
#include "soc_common.h"
#include "los_config.h"
#include "los_debug.h"
#include "los_reg.h"
#include "los_timer.h"
#include "los_context.h"
#include "los_tick.h"
#include "los_arch_interrupt.h"

#include "bk_los_timer.h"


static OS_TICK_HANDLER g_tickHandler = NULL;


void HalTickHandler(u8 param)
{
    if (g_tickHandler != NULL) {
        g_tickHandler();
    }
}


void HalClockInit(OS_TICK_HANDLER handler, UINT64 responseTimeMAX)
{
	UINT32 ret = -1;
	UINT32 int_save;

	(VOID)responseTimeMAX;
	int_save = LOS_IntLock();
	g_tickHandler = handler;

    ret = HalHwiCreate(RISCV_MACH_TIMER_IRQ, OS_HWI_PRIO_LOWEST, 0, (HWI_PROC_FUNC)HalTickHandler, 0);
    LOS_IntRestore(int_save);
    if (LOS_OK != ret) {
        PRINT_ERR("Clock init failed!\n");
    }
}


unsigned int HalTickStart(OS_TICK_HANDLER handler)
{

	g_sysClock = OS_SYS_CLOCK;
	g_cyclesPerTick = g_sysClock / LOSCFG_BASE_CORE_TICK_PER_SECOND;
    g_intCount = 0;

    HalClockInit(handler, LOSCFG_BASE_CORE_TICK_RESPONSE_MAX);

    return LOS_OK;
}


void HalSysTickReload(unsigned long long nextResponseTime)
{
	unsigned int intSave;

	intSave = LOS_IntLock();

	UINT64 timeMax = (UINT64)LOSCFG_BASE_CORE_TICK_RESPONSE_MAX - 1;
	UINT64 timer;

	timer = prvReadMtime();
	if ((timeMax - nextResponseTime) > timer) {
		timer += nextResponseTime;
	} else {
		timer = timeMax;
	}

	prvWriteMtimeCmp(timer);

	LOS_IntRestore(intSave);
}

unsigned long long HalGetTickCycle(unsigned int *period)
{
	(VOID)period;
	unsigned int intSave;
	unsigned long long currentTick;

	intSave = LOS_IntLock();
	
	currentTick = prvReadMtime();
	LOS_IntRestore(intSave);

	return currentTick;
}

UINT32 HalEnterSleep(VOID)
{
    wfi();

    return LOS_OK;
}
