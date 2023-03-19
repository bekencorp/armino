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

#include "los_timer.h"
#include "los_config.h"
#include "los_tick.h"
#include "los_reg.h"
#include "los_arch_interrupt.h"
#include "los_sched.h"
#include "los_arch_timer.h"
#include "riscv_hal.h"


WEAK UINT32 HalTickStart(OS_TICK_HANDLER handler)
{
    g_sysClock = OS_SYS_CLOCK;
    g_cyclesPerTick = g_sysClock / LOSCFG_BASE_CORE_TICK_PER_SECOND;
    g_intCount = 0;

    HalClockInit(handler, (UINT32)LOSCFG_BASE_CORE_TICK_RESPONSE_MAX);

    return LOS_OK; /* never return */
}

WEAK VOID HalSysTickReload(UINT64 nextResponseTime)
{
    UINT64 timeMax = (UINT64)LOSCFG_BASE_CORE_TICK_RESPONSE_MAX - 1;
    UINT64 timer;
    UINT32 timerL, timerH;
    READ_UINT32(timerL, MTIMER);
    READ_UINT32(timerH, MTIMER + MTIMER_HI_OFFSET);
    timer = OS_COMBINED_64(timerH, timerL);
    if ((timeMax - nextResponseTime) > timer) {
        timer += nextResponseTime;
    } else {
        timer = timeMax;
    }

    HalIrqDisable(RISCV_MACH_TIMER_IRQ);
    WRITE_UINT32(0xffffffff, MTIMERCMP + MTIMER_HI_OFFSET);
    WRITE_UINT32((UINT32)timer, MTIMERCMP);
    WRITE_UINT32((UINT32)(timer >> SHIFT_32_BIT), MTIMERCMP + MTIMER_HI_OFFSET);
    HalIrqEnable(RISCV_MACH_TIMER_IRQ);
}

WEAK UINT64 HalGetTickCycle(UINT32 *period)
{
    (VOID)period;
    UINT32 timerL, timerH;

    READ_UINT32(timerL, MTIMER);
    READ_UINT32(timerH, MTIMER + MTIMER_HI_OFFSET);
    return OS_COMBINED_64(timerH, timerL);
}

UINT32 HalEnterSleep(VOID)
{
    wfi();

    return LOS_OK;
}

