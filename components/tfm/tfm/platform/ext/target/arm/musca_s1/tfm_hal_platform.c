/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "cache_drv.h"
#include "cmsis.h"
#include "cmsis_driver_config.h"
#include "musca_s1_scc_drv.h"
#include "target_cfg.h"
#include "tfm_hal_platform.h"
#include "tfm_plat_defs.h"
#include "uart_stdout.h"
#if (CONFIG_TFM_FP == 2) && (TEST_NS_FPU == 1)
#include "tfm_plat_test.h"
#endif

extern const struct memory_region_limits memory_regions;

enum tfm_hal_status_t tfm_hal_platform_init(void)
{
    enum tfm_plat_err_t plat_err = TFM_PLAT_ERR_SYSTEM_ERR;

    musca_s1_scc_mram_fast_read_enable(&MUSCA_S1_SCC_DEV);

    arm_cache_enable_blocking(&SSE_200_CACHE_DEV);

    plat_err = enable_fault_handlers();
    if (plat_err != TFM_PLAT_ERR_SUCCESS) {
        return TFM_HAL_ERROR_GENERIC;
    }

    plat_err = system_reset_cfg();
    if (plat_err != TFM_PLAT_ERR_SUCCESS) {
        return TFM_HAL_ERROR_GENERIC;
    }

    plat_err = init_debug();
    if (plat_err != TFM_PLAT_ERR_SUCCESS) {
        return TFM_HAL_ERROR_GENERIC;
    }

    __enable_irq();
    stdio_init();

    plat_err = nvic_interrupt_target_state_cfg();
    if (plat_err != TFM_PLAT_ERR_SUCCESS) {
        return TFM_HAL_ERROR_GENERIC;
    }

    plat_err = nvic_interrupt_enable();
    if (plat_err != TFM_PLAT_ERR_SUCCESS) {
        return TFM_HAL_ERROR_GENERIC;
    }

#if (CONFIG_TFM_FP == 2) && (TEST_NS_FPU == 1)
    /* Configure secure timer */
    tfm_plat_test_secure_timer_nvic_configure();
    if (!timer_cmsdk_is_initialized(&CMSDK_TIMER0_DEV_S)) {
        timer_cmsdk_init(&CMSDK_TIMER0_DEV_S);
    }

    /* Configure non-secure timer */
    tfm_plat_test_non_secure_timer_nvic_configure();
    if (!timer_cmsdk_is_initialized(&CMSDK_TIMER1_DEV_NS)) {
        timer_cmsdk_init(&CMSDK_TIMER1_DEV_NS);
    }
#endif

    return TFM_HAL_SUCCESS;
}

uint32_t tfm_hal_get_ns_VTOR(void)
{
    return memory_regions.non_secure_code_start;
}

uint32_t tfm_hal_get_ns_MSP(void)
{
    return *((uint32_t *)memory_regions.non_secure_code_start);
}

uint32_t tfm_hal_get_ns_entry_point(void)
{
    return *((uint32_t *)(memory_regions.non_secure_code_start + 4));
}
