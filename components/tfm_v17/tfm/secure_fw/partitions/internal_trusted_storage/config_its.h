/*
 * Copyright (c) 2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __CONFIG_PARTITION_ITS_H__
#define __CONFIG_PARTITION_ITS_H__

#include "config_tfm.h"
#include "config_ps.h"

/* Create flash FS if it doesn't exist for Internal Trusted Storage partition */
#ifndef ITS_CREATE_FLASH_LAYOUT
#pragma message("ITS_CREATE_FLASH_LAYOUT is defaulted to 1. Please check and set it explicitly.")
#define ITS_CREATE_FLASH_LAYOUT          1
#endif

/* Enable emulated RAM FS for platforms that don't have flash for Internal Trusted Storage partition */
#ifndef ITS_RAM_FS
#pragma message("ITS_RAM_FS is defaulted to 0. Please check and set it explicitly.")
#define ITS_RAM_FS                       0
#endif

/* Validate filesystem metadata every time it is read from flash */
#ifndef ITS_VALIDATE_METADATA_FROM_FLASH
#pragma message("ITS_VALIDATE_METADATA_FROM_FLASH is defaulted to 1. Please check and set it explicitly.")
#define ITS_VALIDATE_METADATA_FROM_FLASH 1
#endif

/* Enable ITS_VALIDATE_METADATA_FROM_FLASH when PS_VALIDATE_METADATA_FROM_FLASH is enabled */
#if (!ITS_VALIDATE_METADATA_FROM_FLASH) && PS_VALIDATE_METADATA_FROM_FLASH
#pragma message("ITS_VALIDATE_METADATA_FROM_FLASH is redefined to 1.")
#undef ITS_VALIDATE_METADATA_FROM_FLASH
#define ITS_VALIDATE_METADATA_FROM_FLASH 1
#endif

/* The maximum asset size to be stored in the Internal Trusted Storage */
#ifndef ITS_MAX_ASSET_SIZE
#pragma message("ITS_MAX_ASSET_SIZE is defaulted to 512. Please check and set it explicitly.")
#define ITS_MAX_ASSET_SIZE               512
#endif

/*
 * Size of the ITS internal data transfer buffer
 * (Default to the max asset size so that all requests can be handled in one iteration.)
 */
#ifndef ITS_BUF_SIZE
#pragma message("ITS_BUF_SIZE is defaulted to ITS_MAX_ASSET_SIZE. Please check and set it explicitly.")
#define ITS_BUF_SIZE                     ITS_MAX_ASSET_SIZE
#endif

/* The maximum number of assets to be stored in the Internal Trusted Storage */
#ifndef ITS_NUM_ASSETS
#pragma message("ITS_NUM_ASSETS is defaulted to 10. Please check and set it explicitly.")
#define ITS_NUM_ASSETS                   10
#endif

/* The stack size of the Internal Trusted Storage Secure Partition */
#ifndef ITS_STACK_SIZE
#pragma message("ITS_STACK_SIZE is defaulted to 0x720. Please check and set it explicitly.")
#define ITS_STACK_SIZE                   0x720
#endif

#endif /* __CONFIG_PARTITION_ITS_H__ */
