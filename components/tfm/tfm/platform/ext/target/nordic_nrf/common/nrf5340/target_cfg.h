/*
 * Copyright (c) 2017-2019 Arm Limited
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __TARGET_CFG_H__
#define __TARGET_CFG_H__

/**
 * \file target_cfg.h
 * \brief nRF5340 target configuration header
 *
 * This file contains the platform specific functions to configure
 * the Cortex-M33 core, memory permissions and security attribution
 * on the nRF5340 platform.
 *
 * Memory permissions and security attribution are configured via
 * the System Protection Unit (SPU) which is the nRF specific Implementation
 * Defined Attribution Unit (IDAU).
 */


#include "tfm_plat_defs.h"

#define TFM_DRIVER_STDIO    Driver_USART1
#define NS_DRIVER_STDIO     Driver_USART0

/**
 * \brief Store the addresses of memory regions
 */
struct memory_region_limits {
    uint32_t non_secure_code_start;
    uint32_t non_secure_partition_base;
    uint32_t non_secure_partition_limit;
    uint32_t veneer_base;
    uint32_t veneer_limit;
#ifdef BL2
    uint32_t secondary_partition_base;
    uint32_t secondary_partition_limit;
#endif /* BL2 */
#ifdef NRF_NS_STORAGE
    uint32_t non_secure_storage_partition_base;
    uint32_t non_secure_storage_partition_limit;
#endif /* NRF_NS_STORAGE */
};

/**
 * \brief Holds the data necessary to do isolation for a specific peripheral.
 */
struct platform_data_t
{
    uint32_t periph_start;
    uint32_t periph_limit;
};

/**
 * \brief Configures memory permissions via the System Protection Unit.
 *
 * \return Returns values as specified by the \ref tfm_plat_err_t
 */
enum tfm_plat_err_t spu_init_cfg(void);

/**
 * \brief Configures peripheral permissions via the System Protection Unit.
 *
 * The function does the following:
 * - grants Non-Secure access to nRF peripherals that are not Secure-only
 * - grants Non-Secure access to DDPI channels
 * - grants Non-Secure access to GPIO pins
 *
 * \return Returns values as specified by the \ref tfm_plat_err_t
 */
enum tfm_plat_err_t spu_periph_init_cfg(void);

/**
 * \brief Restrict access to peripheral to secure
 */
void spu_periph_configure_to_secure(uint32_t periph_num);

/**
 * \brief Allow non-secure access to peripheral
 */
void spu_periph_configure_to_non_secure(uint32_t periph_num);

/**
 * \brief Clears SPU interrupt.
 */
void spu_clear_irq(void);

/**
 * \brief Configures SAU and IDAU.
 */
void sau_and_idau_cfg(void);

/**
 * \brief Enables the fault handlers and sets priorities.
 *
 * \return Returns values as specified by the \ref tfm_plat_err_t
 */
enum tfm_plat_err_t enable_fault_handlers(void);

/**
 * \brief Configures the system reset request properties
 *
 * \return Returns values as specified by the \ref tfm_plat_err_t
 */
enum tfm_plat_err_t system_reset_cfg(void);

/**
 * \brief Configures the system debug properties.
 *
 * \return Returns values as specified by the \ref tfm_plat_err_t
 */
enum tfm_plat_err_t init_debug(void);

/**
 * \brief Configures all external interrupts to target the
 *        NS state, apart for the ones associated to secure
 *        peripherals (plus SPU)
 *
 * \return Returns values as specified by the \ref tfm_plat_err_t
 */
enum tfm_plat_err_t nvic_interrupt_target_state_cfg(void);

/**
 * \brief This function enable the interrupts associated
 *        to the secure peripherals (plus the isolation boundary violation
 *        interrupts)
 *
 * \return Returns values as specified by the \ref tfm_plat_err_t
 */
enum tfm_plat_err_t nvic_interrupt_enable(void);

#endif /* __TARGET_CFG_H__ */
