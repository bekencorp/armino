/*
 * Copyright (c) 2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */


#include <stdbool.h>
#include "device_definition.h"

#define NR_FP_REG                               (32U)
#define REL_VALUE_FP_REGS_INVALIDATED           (0xDEADBEEF)
#define REL_VALUE_FP_REGS_NOT_INVALIDATED       (0xBEEFDEAD)

void non_secure_timer_set_reload_value(uint32_t value)
{
    timer_cmsdk_set_reload_value(&CMSDK_TIMER1_DEV_NS, value);
}

void non_secure_timer_stop(void)
{
    timer_cmsdk_disable(&CMSDK_TIMER1_DEV_NS);
    timer_cmsdk_disable_interrupt(&CMSDK_TIMER1_DEV_NS);
    timer_cmsdk_clear_interrupt(&CMSDK_TIMER1_DEV_NS);
}

/*
 * Check whether FP registers are invalidated.
 * Return:
 *   True - FP registers are invalidated
 *   False - FP registers are not invalidated
 */
#if defined (__GNUC__)
__attribute__((noinline))
#endif
bool is_fp_regs_invalidated(void)
{
    static uint32_t fp_buffer[NR_FP_REG] = {0};
    uint32_t i;

    /* Dump FP data from FP registers to buffer */
    __asm volatile(
        "vstm      %0, {S0-S31}            \n"
        :
        :"r"(fp_buffer)
        :"memory"
    );

    for(i = 0; i < NR_FP_REG; i++) {
        if (fp_buffer[i] != 0){
            return false;
        }
    }
    return true;
}

void TIMER1_Handler()
{
    /* Check whether FP regs is invalidated */
    if (is_fp_regs_invalidated()) {
        non_secure_timer_set_reload_value(REL_VALUE_FP_REGS_INVALIDATED);
    } else {
        non_secure_timer_set_reload_value(REL_VALUE_FP_REGS_NOT_INVALIDATED);
    }

    non_secure_timer_stop();
}
