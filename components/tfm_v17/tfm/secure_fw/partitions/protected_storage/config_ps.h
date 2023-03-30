/*
 * Copyright (c) 2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __CONFIG_PARTITION_PS_H__
#define __CONFIG_PARTITION_PS_H__

#include "config_platform.h"
#include "config_tfm.h"

/* Create flash FS if it doesn't exist for Protected Storage partition */
#ifndef PS_CREATE_FLASH_LAYOUT
#pragma message("PS_CREATE_FLASH_LAYOUT is defaulted to 1. Please check and set it explicitly.")
#define PS_CREATE_FLASH_LAYOUT           1
#endif

/* Enable emulated RAM FS for platforms that don't have flash for Protected Storage partition */
#ifndef PS_RAM_FS
#pragma message("PS_RAM_FS is defaulted to 0. Please check and set it explicitly.")
#define PS_RAM_FS                        0
#endif

/* Enable rollback protection for Protected Storage partition */
#ifndef PS_ROLLBACK_PROTECTION
#pragma message("PS_ROLLBACK_PROTECTION is defaulted to 1. Please check and set it explicitly.")
#define PS_ROLLBACK_PROTECTION           1
#endif

/* Validate filesystem metadata every time it is read from flash */
#ifndef PS_VALIDATE_METADATA_FROM_FLASH
#pragma message("PS_VALIDATE_METADATA_FROM_FLASH is defaulted to 1. Please check and set it explicitly.")
#define PS_VALIDATE_METADATA_FROM_FLASH  1
#endif

/* The maximum asset size to be stored in the Protected Storage */
#ifndef PS_MAX_ASSET_SIZE
#pragma message("PS_MAX_ASSET_SIZE is defaulted to 2048. Please check and set it explicitly.")
#define PS_MAX_ASSET_SIZE                2048
#endif

/* The maximum number of assets to be stored in the Protected Storage */
#ifndef PS_NUM_ASSETS
#pragma message("PS_NUM_ASSETS is defaulted to 10. Please check and set it explicitly.")
#define PS_NUM_ASSETS                    10
#endif

/* The stack size of the Protected Storage Secure Partition */
#ifndef PS_STACK_SIZE
#pragma message("PS_STACK_SIZE is defaulted to 0x700. Please check and set it explicitly.")
#define PS_STACK_SIZE                    0x700
#endif

/* Check invalid configs. */
#if PS_ROLLBACK_PROTECTION && PLATFORM_NV_COUNTER_MODULE_DISABLED
#error "Invalid config: PS_ROLLBACK_PROTECTION and PLATFORM_NV_COUNTER_MODULE_DISABLED!"
#endif

#if PS_ROLLBACK_PROTECTION && (!defined(PS_ENCRYPTION))
#error "Invalid config: PS_ROLLBACK_PROTECTION and NOT PS_ENCRYPTION!"
#endif

#if (!PS_ROLLBACK_PROTECTION) && defined(PS_ENCRYPTION) && \
    (defined(PS_CRYPTO_AEAD_ALG_GCM) || defined(PS_CRYPTO_AEAD_ALG_CCM))
#error "Invalid config: NOT PS_ROLLBACK_PROTECTION and PS_ENCRYPTION and PSA_ALG_GCM or PSA_ALG_CCM!"
#endif

#endif /* __CONFIG_PARTITION_PS_H__ */
