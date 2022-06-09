/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "cmsis.h"
#include "tfm_hal_platform.h"
#include "uart_stdout.h"
#include "fwu_agent.h"
#include "watchdog.h"

enum tfm_hal_status_t tfm_hal_platform_init(void)
{
    __enable_irq();
    stdio_init();

    if (corstone1000_watchdog_init()) {
        return TFM_HAL_ERROR_GENERIC;
    }

    if (fwu_metadata_init()) {
        return TFM_HAL_ERROR_GENERIC;
    }

    corstone1000_host_watchdog_handler_init();

    return TFM_HAL_SUCCESS;
}

uint32_t tfm_hal_get_ns_entry_point(void)
{
    /* Boot of Host processor not implemented yet. */
    return 0;
}

uint32_t tfm_hal_get_ns_VTOR(void)
{
    /* Boot of Host processor not implemented yet. */
    return 0;
}
