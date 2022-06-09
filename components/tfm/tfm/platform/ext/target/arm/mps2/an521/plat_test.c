/*
 * Copyright (c) 2019-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "platform_retarget.h"
#include "platform_retarget_dev.h"
#include "timer_cmsdk.h"
#include "tfm_plat_test.h"

#include "smm_mps2.h"

#define TIMER_RELOAD_VALUE (16*1024*1024)

void tfm_plat_test_secure_timer_start(void)
{
    if (!cmsdk_timer_is_initialized(&CMSDK_TIMER0_DEV_S)) {
        cmsdk_timer_init(&CMSDK_TIMER0_DEV_S);
    }
    cmsdk_timer_set_reload_value(&CMSDK_TIMER0_DEV_S, TIMER_RELOAD_VALUE);
    cmsdk_timer_enable(&CMSDK_TIMER0_DEV_S);
    cmsdk_timer_enable_interrupt(&CMSDK_TIMER0_DEV_S);
}

void tfm_plat_test_secure_timer_clear_intr(void) {
    cmsdk_timer_clear_interrupt(&CMSDK_TIMER0_DEV_S);
}

void tfm_plat_test_secure_timer_stop(void)
{
    cmsdk_timer_disable(&CMSDK_TIMER0_DEV_S);
    cmsdk_timer_disable_interrupt(&CMSDK_TIMER0_DEV_S);
    cmsdk_timer_clear_interrupt(&CMSDK_TIMER0_DEV_S);
}

void tfm_plat_test_non_secure_timer_start(void)
{
    if (!cmsdk_timer_is_initialized(&CMSDK_TIMER1_DEV_NS)) {
        cmsdk_timer_init(&CMSDK_TIMER1_DEV_NS);
    }
    cmsdk_timer_set_reload_value(&CMSDK_TIMER1_DEV_NS, TIMER_RELOAD_VALUE);
    cmsdk_timer_enable(&CMSDK_TIMER1_DEV_NS);
    cmsdk_timer_enable_interrupt(&CMSDK_TIMER1_DEV_NS);
}

void tfm_plat_test_non_secure_timer_stop(void)
{
    cmsdk_timer_disable(&CMSDK_TIMER1_DEV_NS);
    cmsdk_timer_disable_interrupt(&CMSDK_TIMER1_DEV_NS);
    cmsdk_timer_clear_interrupt(&CMSDK_TIMER1_DEV_NS);
}
