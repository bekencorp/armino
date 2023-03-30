#include <common/bk_include.h>
#include "portmacro.h"
#include "bk_timer.h"
#include "los_reg.h"
#include "los_debug.h"
#include "los_timer.h"
#include "riscv_hal.h"
#include "soc_common.h"
#include "los_config.h"
#include "los_context.h"
#include <os/os.h>
#include "bk_arch.h"
#include "los_interrupt.h"
#include "los_tick.h"
#include <driver/timer.h>
#include <components/system.h>
#include "los_arch_interrupt.h"
#include "los_arch_timer.h"
#include "los_sched.h"
#include "mon_call.h"


uint32_t  platform_is_in_interrupt_context( void )
{
	return ((uint32_t)HalIsIntActive());
}

