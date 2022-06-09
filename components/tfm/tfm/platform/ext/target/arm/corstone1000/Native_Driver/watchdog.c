/*
 * Copyright (c) 2019-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "watchdog.h"

#include "arm_watchdog_drv.h"
#include "platform_base_address.h"
#include "Driver_Common.h"
#include "cmsis.h"
#include <stdio.h>

/* SoC watchdog config */
struct arm_watchdog_dev_cfg_t SOC_WD_DEV_CFG = {CORSTONE1000_SOC_WATCHDOG_BASE};
struct arm_watchdog_dev_data_t SOC_WD_DEV_DATA;
struct arm_watchdog_dev_t SOC_WD_DEV = {&SOC_WD_DEV_CFG, &SOC_WD_DEV_DATA};
#define SOC_WD_TIMEOUT 0x00030000

/* SE watchdog config */
struct arm_watchdog_dev_cfg_t SE_WD_DEV_CFG = {CORSTONE1000_WATCHDOG_TIMER_BASE};
struct arm_watchdog_dev_data_t SE_WD_DEV_DATA;
struct arm_watchdog_dev_t SE_WD_DEV = {&SE_WD_DEV_CFG, &SE_WD_DEV_DATA};
#define SE_WD_TIMEOUT 0x0FFFFFFF
#define SE_WD_IRQn SE_WATCHDOG_TIMER_IRQn


/* SoC watchdog */
void NMI_Handler(void)
{
    /* Unlock, clear and lock the watchdog timer */
    arm_watchdog_unlock(&SOC_WD_DEV);
    arm_watchdog_clear_interrupt_and_refresh_counter(&SOC_WD_DEV);
    arm_watchdog_lock(&SOC_WD_DEV);
}

/* SE watchdog */
void SE_WATCHDOG_TIMER_IRQHandler(void)
{
    /* Unlock, clear and lock the watchdog timer */
    arm_watchdog_unlock(&SE_WD_DEV);
    arm_watchdog_clear_interrupt_and_refresh_counter(&SE_WD_DEV);
    arm_watchdog_lock(&SE_WD_DEV);
}

int corstone1000_watchdog_init()
{
    __disable_irq();

    /* SoC Watchdog setup */
    arm_watchdog_init(&SOC_WD_DEV, SOC_WD_TIMEOUT);
    arm_watchdog_unlock(&SOC_WD_DEV);
    arm_watchdog_enable(&SOC_WD_DEV);
    arm_watchdog_lock(&SOC_WD_DEV);

    if (!arm_watchdog_is_initialized(&SOC_WD_DEV))
        return ARM_DRIVER_ERROR_UNSUPPORTED;

    if (!arm_watchdog_is_enabled(&SOC_WD_DEV))
        return ARM_DRIVER_ERROR_UNSUPPORTED;

    /* SE Watchdog setup */
    arm_watchdog_init(&SE_WD_DEV, SE_WD_TIMEOUT);
    arm_watchdog_unlock(&SE_WD_DEV);
    arm_watchdog_enable(&SE_WD_DEV);
    arm_watchdog_lock(&SE_WD_DEV);

    NVIC_EnableIRQ(SE_WD_IRQn);

    if (!arm_watchdog_is_initialized(&SE_WD_DEV))
        return ARM_DRIVER_ERROR_UNSUPPORTED;

    if (!arm_watchdog_is_enabled(&SE_WD_DEV))
        return ARM_DRIVER_ERROR_UNSUPPORTED;

    __enable_irq();
    
    return ARM_DRIVER_OK;
}

/*
 * Secure Host Watchdog WS1 Handler
 * efi_reset_system from the host triggers "Secure
 * watchdog WS1 interrupt" to reset the system. Host
 * cannot access this interrupt by design, so SE
 * resets the system using this handler
 *
 */
void SECURE_WATCHDOG_WS1_IRQHandler(void){
    NVIC_SystemReset();
}

/*Enable Secure Watchdog WS1 Interrupt
 * in NVIC controller (asserted by host)*/
void corstone1000_host_watchdog_handler_init(){
    NVIC_EnableIRQ(SECURE_WATCHDOG_WS1_IRQn);
}

