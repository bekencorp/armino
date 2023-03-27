/*
 * Copyright (c) 2019-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "platform_retarget.h"
#include "timer_cmsdk.h"
#include "tfm_plat_test.h"

#include "smm_mps2.h"

#define TIMER_RELOAD_VALUE (16*1024*1024)

//TODO implement timer interrupt via armino timer driver

void tfm_plat_test_secure_timer_start(void)
{
}

void tfm_plat_test_secure_timer_clear_intr(void) {
}

void tfm_plat_test_secure_timer_stop(void)
{
}

void tfm_plat_test_non_secure_timer_start(void)
{
}

void tfm_plat_test_non_secure_timer_stop(void)
{
}
