/*
 * Copyright (c) 2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __CONFIG_PARTITION_PLATFORM_H__
#define __CONFIG_PARTITION_PLATFORM_H__

#include "config_tfm.h"

/* Size of input buffer in platform service */
#ifndef PLATFORM_SERVICE_INPUT_BUFFER_SIZE
#pragma message("PLATFORM_SERVICE_INPUT_BUFFER_SIZE is defaulted to 64. Please check and set it explicitly.")
#define PLATFORM_SERVICE_INPUT_BUFFER_SIZE     64
#endif

/* Size of output buffer in platform service */
#ifndef PLATFORM_SERVICE_OUTPUT_BUFFER_SIZE
#pragma message("PLATFORM_SERVICE_OUTPUT_BUFFER_SIZE is defaulted to 64. Please check and set it explicitly.")
#define PLATFORM_SERVICE_OUTPUT_BUFFER_SIZE    64
#endif

/* The stack size of the Platform Secure Partition */
#ifndef PLATFORM_SP_STACK_SIZE
#pragma message("PLATFORM_SP_STACK_SIZE is defaulted to 0x500. Please check and set it explicitly.")
#define PLATFORM_SP_STACK_SIZE                 0x500
#endif

/* Disable Non-volatile counter module */
#ifndef PLATFORM_NV_COUNTER_MODULE_DISABLED
#pragma message("PLATFORM_NV_COUNTER_MODULE_DISABLED is defaulted to 0. Please check and set it explicitly.")
#define PLATFORM_NV_COUNTER_MODULE_DISABLED    0
#endif

#endif /* __CONFIG_PARTITION_PLATFORM_H__ */
