// Copyright 2022-2023 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "partitions.h"

/* This header file is included from linker scatter file as well, where only a
 * limited C constructs are allowed. Therefore it is not possible to include
 * here the platform_retarget.h to access flash related defines. To resolve this
 * some of the values are redefined here with different names, these are marked
 * with comment.
 */

/* Size of a Secure and of a Non-secure image */
#define FLASH_S_PARTITION_SIZE          CONFIG_PRIMARY_S_PHY_PARTITION_SIZE
#define FLASH_NS_PARTITION_SIZE         CONFIG_PRIMARY_NS_PHY_PARTITION_SIZE
#define FLASH_MAX_PARTITION_SIZE        ((FLASH_S_PARTITION_SIZE >   \
                                          FLASH_NS_PARTITION_SIZE) ? \
                                         FLASH_S_PARTITION_SIZE :    \
                                         FLASH_NS_PARTITION_SIZE)

/* Sector size of the flash hardware; same as FLASH0_SECTOR_SIZE */
#define FLASH_AREA_IMAGE_SECTOR_SIZE    (0x1000)     /* 4 KB */

/* Same as FLASH0_SIZE */
#define FLASH_TOTAL_SIZE                (0x00400000) /* 4 MB TODO peter*/

/* Flash layout info for BL2 bootloader */
#define FLASH_BASE_ADDRESS              (0x02000000)

/* Secure image primary slot */
#define FLASH_AREA_0_ID            (1)
#define FLASH_AREA_0_OFFSET        CONFIG_PRIMARY_S_PHY_PARTITION_OFFSET
#define FLASH_AREA_0_SIZE          CONFIG_PRIMARY_S_VIRTUAL_PARTITION_SIZE

/* Non-secure image primary slot */
#define FLASH_AREA_1_ID            (FLASH_AREA_0_ID + 1)
#define FLASH_AREA_1_OFFSET        CONFIG_PRIMARY_NS_PHY_PARTITION_OFFSET
#define FLASH_AREA_1_SIZE          CONFIG_PRIMARY_NS_VIRTUAL_PARTITION_SIZE

/* Secure image secondary slot */
#define FLASH_AREA_2_ID            (FLASH_AREA_1_ID + 1)
#define FLASH_AREA_2_OFFSET        CONFIG_SECONDARY_S_PHY_PARTITION_OFFSET
#define FLASH_AREA_2_SIZE          CONFIG_SECONDARY_S_VIRTUAL_PARTITION_SIZE

/* Non-secure image secondary slot */
#define FLASH_AREA_3_ID            (FLASH_AREA_2_ID + 1)
#define FLASH_AREA_3_OFFSET        CONFIG_SECONDARY_NS_PHY_PARTITION_OFFSET
#define FLASH_AREA_3_SIZE          CONFIG_SECONDARY_NS_VIRTUAL_CODE_SIZE

/* Scratch area */
#define FLASH_AREA_SCRATCH_ID      (FLASH_AREA_3_ID + 1)
#define FLASH_AREA_SCRATCH_OFFSET  (FLASH_AREA_3_OFFSET + FLASH_AREA_3_SIZE)
#define FLASH_AREA_SCRATCH_SIZE    (FLASH_MAX_PARTITION_SIZE)

/* The maximum number of status entries supported by the bootloader. */
#define MCUBOOT_STATUS_MAX_ENTRIES (FLASH_MAX_PARTITION_SIZE / FLASH_AREA_SCRATCH_SIZE)

/* Maximum number of image sectors supported by the bootloader. */
#define MCUBOOT_MAX_IMG_SECTORS    (FLASH_MAX_PARTITION_SIZE / FLASH_AREA_IMAGE_SECTOR_SIZE)

/* Protected Storage (PS) Service definitions */
#define FLASH_PS_AREA_OFFSET            CONFIG_PS_PHY_PARTITION_OFFSET
#define FLASH_PS_AREA_SIZE              CONFIG_PS_PHY_PARTITION_SIZE

/* Internal Trusted Storage (ITS) Service definitions */
#define FLASH_ITS_AREA_OFFSET           CONFIG_ITS_PHY_PARTITION_OFFSET
#define FLASH_ITS_AREA_SIZE             CONFIG_ITS_PHY_PARTITION_SIZE

/* OTP_definitions */
#define FLASH_OTP_NV_COUNTERS_AREA_OFFSET (FLASH_ITS_AREA_OFFSET + FLASH_ITS_AREA_SIZE)
#define FLASH_OTP_NV_COUNTERS_AREA_SIZE   (FLASH_AREA_IMAGE_SECTOR_SIZE * 2)
#define FLASH_OTP_NV_COUNTERS_SECTOR_SIZE FLASH_AREA_IMAGE_SECTOR_SIZE

/* Offset and size definition in flash area used by assemble.py */
#define SECURE_IMAGE_OFFSET             0 //TODO peter FLASH_AREA_0_OFFSET
#define SECURE_IMAGE_MAX_SIZE           FLASH_S_PARTITION_SIZE

#define NON_SECURE_IMAGE_OFFSET         (SECURE_IMAGE_OFFSET + SECURE_IMAGE_MAX_SIZE)
#define NON_SECURE_IMAGE_MAX_SIZE       FLASH_NS_PARTITION_SIZE

/* Flash device name used by BL2
 * Name is defined in flash driver file: Driver_Flash.c
 */
#define FLASH_DEV_NAME Driver_FLASH0
/* Smallest flash programmable unit in bytes */
#define TFM_HAL_FLASH_PROGRAM_UNIT       (0x1)

/* Protected Storage (PS) Service definitions
 * Note: Further documentation of these definitions can be found in the
 * TF-M PS Integration Guide.
 */
#define TFM_HAL_PS_FLASH_DRIVER Driver_FLASH0

/* In this target the CMSIS driver requires only the offset from the base
 * address instead of the full memory address.
 */
/* Base address of dedicated flash area for PS */
#define TFM_HAL_PS_FLASH_AREA_ADDR    FLASH_PS_AREA_OFFSET
/* Size of dedicated flash area for PS */
#define TFM_HAL_PS_FLASH_AREA_SIZE    FLASH_PS_AREA_SIZE
#define PS_RAM_FS_SIZE                TFM_HAL_PS_FLASH_AREA_SIZE
/* Number of physical erase sectors per logical FS block */
#define TFM_HAL_PS_SECTORS_PER_BLOCK  (1)
/* Smallest flash programmable unit in bytes */
#define TFM_HAL_PS_PROGRAM_UNIT       (0x1)

/* Internal Trusted Storage (ITS) Service definitions
 * Note: Further documentation of these definitions can be found in the
 * TF-M ITS Integration Guide. The ITS should be in the internal flash, but is
 * allocated in the external flash just for development platforms that don't
 * have internal flash available.
 */
#define TFM_HAL_ITS_FLASH_DRIVER Driver_FLASH0

/* In this target the CMSIS driver requires only the offset from the base
 * address instead of the full memory address.
 */
/* Base address of dedicated flash area for ITS */
#define TFM_HAL_ITS_FLASH_AREA_ADDR    FLASH_ITS_AREA_OFFSET
/* Size of dedicated flash area for ITS */
#define TFM_HAL_ITS_FLASH_AREA_SIZE    FLASH_ITS_AREA_SIZE
#define ITS_RAM_FS_SIZE                TFM_HAL_ITS_FLASH_AREA_SIZE
/* Number of physical erase sectors per logical FS block */
#define TFM_HAL_ITS_SECTORS_PER_BLOCK  (1)
/* Smallest flash programmable unit in bytes */
#define TFM_HAL_ITS_PROGRAM_UNIT       (0x1)

/* OTP / NV counter definitions */
#define TFM_OTP_NV_COUNTERS_AREA_SIZE   (FLASH_OTP_NV_COUNTERS_AREA_SIZE / 2)
#define TFM_OTP_NV_COUNTERS_AREA_ADDR   FLASH_OTP_NV_COUNTERS_AREA_OFFSET
#define TFM_OTP_NV_COUNTERS_SECTOR_SIZE FLASH_OTP_NV_COUNTERS_SECTOR_SIZE
#define TFM_OTP_NV_COUNTERS_BACKUP_AREA_ADDR (TFM_OTP_NV_COUNTERS_AREA_ADDR + \
                                              TFM_OTP_NV_COUNTERS_AREA_SIZE)

//TODO peter prefer reg_base.h
/* Use SRAM1 memory to store Code data */
#define S_ROM_ALIAS_BASE  (0x02000000)
#define NS_ROM_ALIAS_BASE (0x12000000)

#define S_RAM_ALIAS_BASE  (0x28000000)
#define NS_RAM_ALIAS_BASE (0x38000000)

#define TOTAL_ROM_SIZE FLASH_TOTAL_SIZE
#define TOTAL_RAM_SIZE    (KB(512))

#define CONFIG_BL2_RAM_LOAD_ADDR               0x28040000

#define FLASH_AREA_IMAGE_PRIMARY(x)     (((x) == 0) ? FLASH_AREA_0_ID : \
                                         ((x) == 1) ? FLASH_AREA_1_ID : \
                                                      255 )
#define FLASH_AREA_IMAGE_SECONDARY(x)   (((x) == 0) ? FLASH_AREA_2_ID : \
                                         ((x) == 1) ? FLASH_AREA_3_ID : \
                                                      255 )
#define FLASH_AREA_IMAGE_SCRATCH        FLASH_AREA_SCRATCH_ID


