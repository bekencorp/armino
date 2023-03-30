/*
 * Copyright (c) 2013-2020, Huawei Technologies Co., Ltd. All rights reserved.
 * Copyright (c) 2020-2021 Huawei Device Co., Ltd. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 *    conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 *    of conditions and the following disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 *    to endorse or promote products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "riscv_hal.h"
#include "soc_common.h"
#include "los_config.h"
#include "los_debug.h"
#include "los_reg.h"
#include "los_timer.h"
#include "los_tick.h"
#include "los_arch_interrupt.h"
#include "los_sched.h"
#include "los_arch_timer.h"
#include "mon_call.h"

extern void riscv_set_mtimercmp(UINT64 new_time);
extern UINT64 riscv_get_mtimer(void);

UINT32 HalTickStart(OS_TICK_HANDLER handler)
{
	(VOID)handler;

    g_sysClock = OS_SYS_CLOCK;
    g_cyclesPerTick = g_sysClock / LOSCFG_BASE_CORE_TICK_PER_SECOND;
    g_intCount = 0;

	uint64_t ullNextTime;

	ullNextTime = riscv_get_mtimer();
	ullNextTime += ( uint64_t ) ( OS_SYS_CLOCK / LOSCFG_BASE_CORE_TICK_PER_SECOND );
	riscv_set_mtimercmp(ullNextTime);

	mon_timer_int_clear();

    return LOS_OK;
}

VOID HalSysTickReload(UINT64 nextResponseTime)
{
	unsigned int intSave;

	intSave = LOS_IntLock();

	UINT64 timeMax = (UINT64)LOSCFG_BASE_CORE_TICK_RESPONSE_MAX - 1;
	UINT64 timer;

	timer = riscv_get_mtimer();
	if ((timeMax - nextResponseTime) > timer) {
		timer += nextResponseTime;
	} else {
		timer = timeMax;
	}

	riscv_set_mtimercmp(timer);

	mon_timer_int_clear();

	LOS_IntRestore(intSave);
}

UINT64 HalGetTickCycle(UINT32 *period)
{
	(VOID)period;
	unsigned int intSave;
	unsigned long long currentTick;

	intSave = LOS_IntLock();
	
	currentTick = riscv_get_mtimer();
	LOS_IntRestore(intSave);

	return currentTick;
}

UINT32 HalEnterSleep(VOID)
{
    wfi();

    return LOS_OK;
}

