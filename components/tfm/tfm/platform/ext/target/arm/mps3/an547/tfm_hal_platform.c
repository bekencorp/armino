/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "cmsis.h"
#include "target_cfg.h"
#include "tfm_hal_platform.h"
#include "tfm_peripherals_def.h"
#include "uart_stdout.h"
#include "device_definition.h"

extern const struct memory_region_limits memory_regions;

enum tfm_hal_status_t tfm_hal_platform_init(void)
{
    enum tfm_plat_err_t plat_err = TFM_PLAT_ERR_SYSTEM_ERR;
    enum syscounter_armv8_m_cntrl_error_t counter_err = SYSCOUNTER_ARMV8_M_ERR_NONE;

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

    /* Syscounter enabled by default. This way App-RoT partitions can use
     * systimers without the need to add the syscounter as an mmio devide.
     */
    counter_err = syscounter_armv8_m_cntrl_init(&SYSCOUNTER_CNTRL_ARMV8_M_DEV_S);
    if (counter_err != SYSCOUNTER_ARMV8_M_ERR_NONE) {
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

    return TFM_HAL_SUCCESS;
}

void tfm_hal_system_reset(void)
{
    __disable_irq();
    mpc_revert_non_secure_to_secure_cfg();

    NVIC_SystemReset();
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
