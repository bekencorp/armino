/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __BUILD_CONFIG_CHECK__
#define __BUILD_CONFIG_CHECK__

/*
 * This file checks whether the build configurations is set correctly or not.
 * Also sets necessary toolchain-specific configurations.
 */

/*
 * The default semihosting underlying triggers exceptions (Usually by a 'BKPT'
 * instruction) to request semihosting handling, while secure firmware does
 * not provide corresponded handler for this instruction.
 *
 * Besides semihosting, a few extra symbols need to be re-targeted if 'main'
 * has 'argv'. The following symbol can disable both semihosting underlying
 * and 'argv' usage for 'main'. (The entry symbol name must be 'main', as
 * Armclang takes 'main' as the default application entry.)
 */
#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
__asm("  .global __ARM_use_no_argv\n");
#endif

/* Check isolation levels. */
#ifndef TFM_LVL
#error TFM_LVL is not defined.
#elif (TFM_LVL != 1) && (TFM_LVL != 2) && (TFM_LVL != 3)
#error Invalid TFM_LVL value. The possible values are 1, 2 and 3.
#endif

/* Check if there are unsupported thread priority. */
#include "load/spm_load_api.h"
#include "thread.h"

#if TO_THREAD_PRIORITY(PARTITION_PRI_HIGHEST) != THRD_PRIOR_HIGHEST ||  \
    TO_THREAD_PRIORITY(PARTITION_PRI_HIGH) != THRD_PRIOR_HIGH ||        \
    TO_THREAD_PRIORITY(PARTITION_PRI_NORMAL) != THRD_PRIOR_MEDIUM ||    \
    TO_THREAD_PRIORITY(PARTITION_PRI_LOW) != THRD_PRIOR_LOW ||          \
    TO_THREAD_PRIORITY(PARTITION_PRI_LOWEST) != THRD_PRIOR_LOWEST
#error "Partition priority converting to thread priority error!"
#endif

#endif /* __BUILD_CONFIG_CHECK__ */
