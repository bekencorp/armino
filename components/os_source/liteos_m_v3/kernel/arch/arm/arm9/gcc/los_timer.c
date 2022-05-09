/*
 * Copyright (c) 2013-2019 Huawei Technologies Co., Ltd. All rights reserved.
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

#include "los_config.h"
#include "los_sched.h"
#include "los_arch_context.h"
#include "los_arch_interrupt.h"
#include "los_reg.h"

#define OS_TIMER_CLKDIV_POS     3
#define OS_TIMER_CLKDIV_MASK    7
#define OS_TIMER_INT_POS        7
#define OS_TIMER_INT_MASK       7
#define OS_TIMER_IRQ_NUM        8
#define OS_TIMER_ENABLE         (1U << 0)
#define OS_TIMER_32K_CLK_BIT    (1U << 21)
#define OS_TIMER_CNT_READ_BIT   (1U << 0)

#define OS_TIMER_REG_BASE           0x00802A40UL
#define OS_TIMER_CLK_PWD_ADDR       0x00802008UL
#define OS_TIMER_PERIOD_REG_ADDR    (OS_TIMER_REG_BASE)
#define OS_TIMER_CTL_REG_ADDR       (OS_TIMER_REG_BASE + 12)
#define OS_TIMER_READ_CTL_ADDR      (OS_TIMER_REG_BASE + 16)
#define OS_TIMER_READ_VAL_ADDR      (OS_TIMER_REG_BASE + 20)

/* ****************************************************************************
Function    : HalTickStart
Description : Configure Tick Interrupt Start
Input       : none
output      : none
return      : LOS_OK - Success , or LOS_ERRNO_TICK_CFG_INVALID - failed
**************************************************************************** */
WEAK UINT32 HalTickStart(OS_TICK_HANDLER handler)
{
    UINT32 intSave = LOS_IntLock();
    UINT32 value;

    READ_UINT32(value, OS_TIMER_CLK_PWD_ADDR);
    value &= ~(OS_TIMER_32K_CLK_BIT);
    WRITE_UINT32(value, OS_TIMER_CLK_PWD_ADDR);

    value = LOSCFG_BASE_CORE_TICK_RESPONSE_MAX;
    WRITE_UINT32(value, OS_TIMER_PERIOD_REG_ADDR);

    READ_UINT32(value, OS_TIMER_CTL_REG_ADDR);
    value &= ~(OS_TIMER_CLKDIV_MASK << OS_TIMER_CLKDIV_POS); // The default is 1, and the clock does not divide.
    value &= ~(OS_TIMER_INT_MASK << OS_TIMER_INT_POS); // Clearing interruption.
    value |= 0x1 << OS_TIMER_INT_POS;
    value |= OS_TIMER_ENABLE; // Enable timer.
    WRITE_UINT32(value, OS_TIMER_CTL_REG_ADDR);

    (VOID)HalHwiCreate(OS_TIMER_IRQ_NUM, 0, 0, (HWI_PROC_FUNC)handler, 0);
    LOS_IntRestore(intSave);

    return LOS_OK;
}

STATIC VOID HalClockIrqClear(VOID)
{
    UINT32 mask = OS_TIMER_INT_MASK << OS_TIMER_INT_POS;
    UINT32 status;

    do {
        WRITE_UINT32(mask, OS_TIMER_CTL_REG_ADDR);
        READ_UINT32(status, OS_TIMER_CTL_REG_ADDR);
    } while (status & mask);
}

WEAK VOID HalSysTickReload(UINT64 nextResponseTime)
{
    HalTickLock();
    WRITE_UINT32(nextResponseTime, OS_TIMER_PERIOD_REG_ADDR);
    HalClockIrqClear();
    HalTickUnlock();
}

WEAK UINT64 HalGetTickCycle(UINT32 *period)
{
    UINT32 val;

    READ_UINT32(*period, OS_TIMER_PERIOD_REG_ADDR);

    WRITE_UINT32(OS_TIMER_CNT_READ_BIT, OS_TIMER_READ_CTL_ADDR);
    do {
        READ_UINT32(val, OS_TIMER_READ_CTL_ADDR);
    } while (val & OS_TIMER_CNT_READ_BIT); // Wait for the setting to take effect.

    READ_UINT32(val, OS_TIMER_READ_VAL_ADDR);

    return (UINT64)val;
}

WEAK VOID HalTickLock(VOID)
{
    UINT32 value;

    READ_UINT32(value, OS_TIMER_CTL_REG_ADDR);
    value &= ~OS_TIMER_ENABLE;
    value &= ~(OS_TIMER_INT_MASK << OS_TIMER_INT_POS);
    value |= 0x1 << OS_TIMER_INT_POS;
    WRITE_UINT32(value, OS_TIMER_CTL_REG_ADDR);
}

WEAK VOID HalTickUnlock(VOID)
{
    UINT32 value;

    READ_UINT32(value, OS_TIMER_CTL_REG_ADDR);
    value |= OS_TIMER_ENABLE;
    value &= ~(OS_TIMER_INT_MASK << OS_TIMER_INT_POS);
    value |= 0x1 << OS_TIMER_INT_POS;
    WRITE_UINT32(value, OS_TIMER_CTL_REG_ADDR);
}

UINT32 HalEnterSleep(VOID)
{
    dsb();
    wfi();
    isb();

    return LOS_OK;
}
