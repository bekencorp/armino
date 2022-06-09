/*
 * Copyright (c) 2019-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "timer_cmsdk_drv.h"
#include "tfm_plat_defs.h"
#include "tfm_plat_test.h"
#include "device_definition.h"

#define TIMER_RELOAD_VALUE          (16*1024*1024)

void tfm_plat_test_secure_timer_start(void)
{
    if (!timer_cmsdk_is_initialized(&CMSDK_TIMER0_DEV_S)) {
        timer_cmsdk_init(&CMSDK_TIMER0_DEV_S);
    }
    timer_cmsdk_set_reload_value(&CMSDK_TIMER0_DEV_S, TIMER_RELOAD_VALUE);
    timer_cmsdk_enable(&CMSDK_TIMER0_DEV_S);
    timer_cmsdk_enable_interrupt(&CMSDK_TIMER0_DEV_S);
}

void tfm_plat_test_secure_timer_stop(void)
{
    timer_cmsdk_disable(&CMSDK_TIMER0_DEV_S);
    timer_cmsdk_disable_interrupt(&CMSDK_TIMER0_DEV_S);
    timer_cmsdk_clear_interrupt(&CMSDK_TIMER0_DEV_S);
}

void tfm_plat_test_non_secure_timer_start(void)
{
    if (!timer_cmsdk_is_initialized(&CMSDK_TIMER1_DEV_NS)) {
        timer_cmsdk_init(&CMSDK_TIMER1_DEV_NS);
    }
    timer_cmsdk_set_reload_value(&CMSDK_TIMER1_DEV_NS, TIMER_RELOAD_VALUE);
    timer_cmsdk_enable(&CMSDK_TIMER1_DEV_NS);
    timer_cmsdk_enable_interrupt(&CMSDK_TIMER1_DEV_NS);
}

void tfm_plat_test_non_secure_timer_stop(void)
{
    timer_cmsdk_disable(&CMSDK_TIMER1_DEV_NS);
    timer_cmsdk_disable_interrupt(&CMSDK_TIMER1_DEV_NS);
    timer_cmsdk_clear_interrupt(&CMSDK_TIMER1_DEV_NS);
}
