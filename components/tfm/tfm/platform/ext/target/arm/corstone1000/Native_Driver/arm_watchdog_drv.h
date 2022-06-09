/*
 * Copyright (c) 2016-2020 ARM Limited
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

/**
 * \file arm_watchdog_drv.h
 * \brief Generic driver for ARM watchdogs.
 */

#ifndef __ARM_WATCHDOG_DRV_H__
#define __ARM_WATCHDOG_DRV_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

enum arm_watchdog_error_t {
    ARM_WATCHDOG_ERR_NONE = 0,  /*!< No error */
    ARM_WATCHDOG_ERR_NOT_INIT,  /*!< Watchdog is not initialized */
    ARM_WATCHDOG_ERR_NOT_ENAB,  /*!< Watchdog is not enabled */
    ARM_WATCHDOG_ERR_LOCKED     /*!< Watchdog is locked */
};

/* ARM watchdog device configuration structure */
struct arm_watchdog_dev_cfg_t {
    const uint32_t base;  /*!< Watchdog base address */
};

/* ARM watchdog device data structure */
struct arm_watchdog_dev_data_t {
    uint32_t state;    /*!< Indicates if the watchdog
                            is initialized and enabled */
    uint32_t timeout;  /*!< Timeout to reset in cycles */
};

/* ARM watchdog device structure */
struct arm_watchdog_dev_t {
    const struct arm_watchdog_dev_cfg_t* const cfg;  /*!< Watchdog
                                                          configuration */
    struct arm_watchdog_dev_data_t* const data;      /*!< Watchdog data */
};

/**
 * \brief Initializes a watchdog hardware.
 *
 * \param[in] dev      Watchdog to be initialized \ref arm_watchdog_dev_t
 * \param[in] timeout  Timeout in cycles - 0 assings timeout to max value.
 *
 * \note This function doesn't check if dev is NULL.
 *       This function leaves the watchdog locked. Before any further
 *       operations, it needs to be unlocked and locked again.
 */
void arm_watchdog_init(struct arm_watchdog_dev_t* dev, uint32_t timeout);

/**
 * \brief Feeds the watchdog to not cause a reset.
 *
 * \param[in] dev  Watchdog to be fed \ref arm_watchdog_dev_t
 *
 * \return Returns error code as specified in \ref arm_watchdog_error_t
 *
 * \note This function doesn't check if dev is NULL.
 */
enum arm_watchdog_error_t arm_watchdog_feed(struct arm_watchdog_dev_t* dev);

/**
 * \brief Clear the interrupt and load timeout value to the load register.
 *
 * \param[in] dev   Watchdog to be fed \ref arm_watchdog_dev_t
 *
 * \return Returns error code as specified in \ref arm_watchdog_error_t
 *
 * \note This function doesn't check if dev is NULL.
 */
enum arm_watchdog_error_t
arm_watchdog_clear_interrupt_and_refresh_counter(
    struct arm_watchdog_dev_t* dev);

/**
 * \brief Enables the watchdog.
 *
 * \param[in] dev  Watchdog to be enabled \ref arm_watchdog_dev_t
 *
 * \return Returns error code as specified in \ref arm_watchdog_error_t
 *
 * \note This function doesn't check if dev is NULL.
 */
enum arm_watchdog_error_t arm_watchdog_enable(struct arm_watchdog_dev_t* dev);

/**
 * \brief Checks if the watchdog is enabled
 *
 * \param[in] dev Watchdog to be checked \ref arm_watchdog_dev_t
 *
 * \return 1 if watchdog is enabled, 0 otherwise.
 *
 * \note This function doesn't check if dev is NULL.
 */
uint32_t arm_watchdog_is_enabled(struct arm_watchdog_dev_t* dev);

/**
 * \brief Disables the watchdog.
 *
 * \param[in] dev  Watchdog to be disabled \ref arm_watchdog_dev_t
 *
 * \return 1 if watchdog is enabled, 0 otherwise.
 *
 * \note This function doesn't check if dev is NULL.
 */
enum arm_watchdog_error_t arm_watchdog_disable(struct arm_watchdog_dev_t* dev);

/**
 * \brief Locks the registers to not be written again.
 *
 * \param[in] dev  Watchdog to be locked \ref arm_watchdog_dev_t
 *
 * \note This function doesn't check if dev is NULL.
 */
void arm_watchdog_lock(struct arm_watchdog_dev_t* dev);

/**
 * \brief Checks if the watchdog registers are locked
 *
 * \param[in] dev Watchdog to be checked \ref arm_watchdog_dev_t
 *
 * \return 1 if the registers are locked, 0 otherwise
 *
 * \note This function doesn't check if dev is NULL.
 */
uint32_t arm_watchdog_is_locked(struct arm_watchdog_dev_t* dev);

/**
 * \brief Unlocks the registers to configure the watchdog again.
 *
 * \param[in] dev  Watchdog to be unlocked \ref arm_watchdog_dev_t
 *
 * \note This function doesn't check if dev is NULL.
 */
void arm_watchdog_unlock(struct arm_watchdog_dev_t* dev);

/**
 * \brief Gets if watchdog driver is initialized
 *
 * \param[in] dev  Watchdog to be initialized \ref arm_watchdog_dev_t
 *
 * \returns 1 if watchdog driver is initialized. Otherwise 0.
 */
uint32_t arm_watchdog_is_initialized(struct arm_watchdog_dev_t* dev);

/**
 * \brief Provides watchdog remaining time before reset.
 *
 * \param[in] dev  Watchdog to get the remaining time \ref arm_watchdog_dev_t
 *
 * \return 0 if watchdog is disabled.
 *
 * \note This function doesn't check if dev is NULL.
 */
uint32_t arm_watchdog_get_remaining_time(struct arm_watchdog_dev_t* dev);

#ifdef __cplusplus
}
#endif
#endif /* __ARM_WATCHDOG_DRV_H__ */

