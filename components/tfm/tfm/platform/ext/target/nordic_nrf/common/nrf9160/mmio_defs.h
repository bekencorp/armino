/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __MMIO_DEFS_H__
#define __MMIO_DEFS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "tfm_peripherals_def.h"
#include "tfm_peripherals_config.h"

/* Boundary handle binding macros. */
#define HANDLE_PER_ATTR_BITS (0x4)
#define HANDLE_ATTR_PRIV_MASK ((1 << HANDLE_PER_ATTR_BITS) - 1)

/* Allowed named MMIO of this platform */
const uintptr_t partition_named_mmio_list[] = {
#if TFM_PERIPHERAL_REGULATORS_SECURE
    (uintptr_t)TFM_PERIPHERAL_REGULATORS,
#endif
#if TFM_PERIPHERAL_CLOCK_SECURE
    (uintptr_t)TFM_PERIPHERAL_CLOCK,
#endif
#if TFM_PERIPHERAL_POWER_SECURE
    (uintptr_t)TFM_PERIPHERAL_POWER,
#endif
#if TFM_PERIPHERAL_SPIM0_SECURE
    (uintptr_t)TFM_PERIPHERAL_SPIM0,
#endif
#if TFM_PERIPHERAL_SPIS0_SECURE
    (uintptr_t)TFM_PERIPHERAL_SPIS0,
#endif
#if TFM_PERIPHERAL_TWIM0_SECURE
    (uintptr_t)TFM_PERIPHERAL_TWIM0,
#endif
#if TFM_PERIPHERAL_TWIS0_SECURE
    (uintptr_t)TFM_PERIPHERAL_TWIS0,
#endif
#if TFM_PERIPHERAL_UARTE0_SECURE
    (uintptr_t)TFM_PERIPHERAL_UARTE0,
#endif
#if TFM_PERIPHERAL_SPIM1_SECURE
    (uintptr_t)TFM_PERIPHERAL_SPIM1,
#endif
#if TFM_PERIPHERAL_SPIS1_SECURE
    (uintptr_t)TFM_PERIPHERAL_SPIS1,
#endif
#if TFM_PERIPHERAL_TWIM1_SECURE
    (uintptr_t)TFM_PERIPHERAL_TWIM1,
#endif
#if TFM_PERIPHERAL_TWIS1_SECURE
    (uintptr_t)TFM_PERIPHERAL_TWIS1,
#endif
#if TFM_PERIPHERAL_UARTE1_SECURE
    (uintptr_t)TFM_PERIPHERAL_UARTE1,
#endif
#if TFM_PERIPHERAL_SPIM2_SECURE
    (uintptr_t)TFM_PERIPHERAL_SPIM2,
#endif
#if TFM_PERIPHERAL_SPIS2_SECURE
    (uintptr_t)TFM_PERIPHERAL_SPIS2,
#endif
#if TFM_PERIPHERAL_TWIM2_SECURE
    (uintptr_t)TFM_PERIPHERAL_TWIM2,
#endif
#if TFM_PERIPHERAL_TWIS2_SECURE
    (uintptr_t)TFM_PERIPHERAL_TWIS2,
#endif
#if TFM_PERIPHERAL_UARTE2_SECURE
    (uintptr_t)TFM_PERIPHERAL_UARTE2,
#endif
#if TFM_PERIPHERAL_SPIM3_SECURE
    (uintptr_t)TFM_PERIPHERAL_SPIM3,
#endif
#if TFM_PERIPHERAL_SPIS3_SECURE
    (uintptr_t)TFM_PERIPHERAL_SPIS3,
#endif
#if TFM_PERIPHERAL_TWIM3_SECURE
    (uintptr_t)TFM_PERIPHERAL_TWIM3,
#endif
#if TFM_PERIPHERAL_TWIS3_SECURE
    (uintptr_t)TFM_PERIPHERAL_TWIS3,
#endif
#if TFM_PERIPHERAL_UARTE3_SECURE
    (uintptr_t)TFM_PERIPHERAL_UARTE3,
#endif
#if TFM_PERIPHERAL_SAADC_SECURE
    (uintptr_t)TFM_PERIPHERAL_SAADC,
#endif
#if TFM_PERIPHERAL_TIMER0_SECURE
    (uintptr_t)TFM_PERIPHERAL_TIMER0,
#endif
#if TFM_PERIPHERAL_TIMER1_SECURE
    (uintptr_t)TFM_PERIPHERAL_TIMER1,
#endif
#if TFM_PERIPHERAL_TIMER2_SECURE
    (uintptr_t)TFM_PERIPHERAL_TIMER2,
#endif
#if TFM_PERIPHERAL_RTC0_SECURE
    (uintptr_t)TFM_PERIPHERAL_RTC0,
#endif
#if TFM_PERIPHERAL_RTC1_SECURE
    (uintptr_t)TFM_PERIPHERAL_RTC1,
#endif
#if TFM_PERIPHERAL_DPPI_SECURE
    (uintptr_t)TFM_PERIPHERAL_DPPI,
#endif
#if TFM_PERIPHERAL_WDT_SECURE
    (uintptr_t)TFM_PERIPHERAL_WDT,
#endif
#if TFM_PERIPHERAL_EGU0_SECURE
    (uintptr_t)TFM_PERIPHERAL_EGU0,
#endif
#if TFM_PERIPHERAL_EGU1_SECURE
    (uintptr_t)TFM_PERIPHERAL_EGU1,
#endif
#if TFM_PERIPHERAL_EGU2_SECURE
    (uintptr_t)TFM_PERIPHERAL_EGU2,
#endif
#if TFM_PERIPHERAL_EGU3_SECURE
    (uintptr_t)TFM_PERIPHERAL_EGU3,
#endif
#if TFM_PERIPHERAL_EGU4_SECURE
    (uintptr_t)TFM_PERIPHERAL_EGU4,
#endif
#if TFM_PERIPHERAL_EGU5_SECURE
    (uintptr_t)TFM_PERIPHERAL_EGU5,
#endif
#if TFM_PERIPHERAL_PWM0_SECURE
    (uintptr_t)TFM_PERIPHERAL_PWM0,
#endif
#if TFM_PERIPHERAL_PWM1_SECURE
    (uintptr_t)TFM_PERIPHERAL_PWM1,
#endif
#if TFM_PERIPHERAL_PWM2_SECURE
    (uintptr_t)TFM_PERIPHERAL_PWM2,
#endif
#if TFM_PERIPHERAL_PWM3_SECURE
    (uintptr_t)TFM_PERIPHERAL_PWM3,
#endif
#if TFM_PERIPHERAL_PDM_SECURE
    (uintptr_t)TFM_PERIPHERAL_PDM,
#endif
#if TFM_PERIPHERAL_I2S_SECURE
    (uintptr_t)TFM_PERIPHERAL_I2S,
#endif
#if TFM_PERIPHERAL_IPC_SECURE
    (uintptr_t)TFM_PERIPHERAL_IPC,
#endif
#if TFM_PERIPHERAL_FPU_SECURE
    (uintptr_t)TFM_PERIPHERAL_FPU,
#endif
#if TFM_PERIPHERAL_NVMC_SECURE
    (uintptr_t)TFM_PERIPHERAL_NVMC,
#endif
#if TFM_PERIPHERAL_VMC_SECURE
    (uintptr_t)TFM_PERIPHERAL_VMC,
#endif
#if TFM_PERIPHERAL_GPIO0_SECURE
    (uintptr_t)TFM_PERIPHERAL_GPIO0,
#endif
};

#ifdef __cplusplus
}
#endif

#endif /* __MMIO_DEFS_H__ */
