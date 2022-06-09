/*
 * Copyright (c) 2019-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef WATCHDOG_H
#define WATCHDOG_H

#include <stddef.h>
#include <stdint.h>

#include "Driver_Common.h"
#include "arm_watchdog_drv.h"
#include "platform_base_address.h"

/**
 *  \brief Initializes Secure Enclave & SoC Watchdog.
 *
 *  \returns ARM Driver return code.
 */
int corstone1000_watchdog_init();

/**
 *  \brief Reset the Secure Enclave & SoC Watchdog's.
 * 
 *  \returns ARM Driver return code.
 */
int corstone1000_watchdog_reset_timer();

/**
 *  \brief Initializes Interrupt Handler for
 *   Secure Host Watchdog (WS1).
 *
 *  \returns  VOID
 */
void corstone1000_host_watchdog_handler_init();


#endif /* watchdog_h */
