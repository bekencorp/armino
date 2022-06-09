/*
 * Copyright (c) 2017-2021 Arm Limited. All rights reserved.
 * Copyright (c) 2019-2020, Cypress Semiconductor Corporation. All rights reserved.
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

#ifndef __FLASH_LAYOUT_H__
#define __FLASH_LAYOUT_H__

/* Flash layout with BL2:
 *
 * Not supported
 *
 * Flash layout if BL2 not defined:
 *
 * 0x1000_0000 Secure     image primary (320 KB)
 * 0x1005_0000 Non-secure image primary (1152 KB)
 * 0x1017_0000 Secure     image secondary (320 KB)
 * 0x101c_0000 - 0x101f_ffff Reserved
 *  0x101c_0000 Internal Trusted Storage Area (16 KB)
 *  0x101c_4000 Protected Storage Area (24 KB)
 *  0x101c_a000 Unused area (22 KB)
 *  0x101c_f800 OTP / NV counters area (2 KB)
 *  0x101d_0000 Reserved (192 KB)
 * 0x101f_ffff End of Flash
 *
 */

/* This header file is included from linker scatter file as well, where only a
 * limited C constructs are allowed. Therefore it is not possible to include
 * here the platform_base_address.h to access flash related defines. To resolve
 * this some of the values are redefined here with different names, these are
 * marked with comment.
 */

#ifdef BL2
#error "BL2 configuration is not supported"
#endif /* BL2 */

/* The size of S partition */
#define FLASH_S_PARTITION_SIZE          0x50000      /* 320 KB */
/* The size of NS partition */
#define FLASH_NS_PARTITION_SIZE         0x120000     /* 1152 KB */

/* Sector size of the flash hardware; same as FLASH0_SECTOR_SIZE */
#define FLASH_AREA_IMAGE_SECTOR_SIZE    (0x200)      /* 512 B */
/* Same as FLASH0_SIZE */
#define FLASH_TOTAL_SIZE                (0x00200000) /* 2 MB */

/* Flash layout info for BL2 bootloader */
#define FLASH_BASE_ADDRESS              (0x10000000U) /* same as FLASH0_BASE */

/* Reserved areas */
#define FLASH_DATA_AREA_OFFSET          (SECURE_IMAGE_OFFSET + \
                                         2*SECURE_IMAGE_MAX_SIZE + \
                                         NON_SECURE_IMAGE_MAX_SIZE)

/* FixMe: implement proper mcuboot partitioning for CYBL */

/* Internal Trusted Storage Area */
#define FLASH_ITS_AREA_OFFSET           (FLASH_DATA_AREA_OFFSET)
#define FLASH_ITS_AREA_SIZE             (0x4000)    /* 16 KB */

/* Protected Storage Area */
#define FLASH_PS_AREA_OFFSET            (FLASH_ITS_AREA_OFFSET + \
                                         FLASH_ITS_AREA_SIZE)
#define FLASH_PS_AREA_SIZE              (0x6000)   /* 24 KB */

/* Unused Area */
#define FLASH_UNUSED_AREA_OFFSET        (FLASH_PS_AREA_OFFSET + \
                                         FLASH_PS_AREA_SIZE)
#define FLASH_UNUSED_AREA_SIZE          (0x5800)   /* 22 KB */

/* OTP_definitions */
#define FLASH_OTP_NV_COUNTERS_AREA_OFFSET (FLASH_UNUSED_AREA_OFFSET + \
                                           FLASH_UNUSED_AREA_SIZE)
#define FLASH_OTP_NV_COUNTERS_AREA_SIZE   (FLASH_AREA_IMAGE_SECTOR_SIZE * 4)
#define FLASH_OTP_NV_COUNTERS_SECTOR_SIZE FLASH_AREA_IMAGE_SECTOR_SIZE

#define FLASH_AREA_SYSTEM_RESERVED_SIZE (0x30000) /* 192 KB */


/* Secure and non-secure images definition in flash area */

#define SECURE_IMAGE_OFFSET             0x0

#define SECURE_IMAGE_MAX_SIZE           FLASH_S_PARTITION_SIZE

#define NON_SECURE_IMAGE_OFFSET         (SECURE_IMAGE_OFFSET + \
                                         SECURE_IMAGE_MAX_SIZE)

#define NON_SECURE_IMAGE_MAX_SIZE       FLASH_NS_PARTITION_SIZE

/* Check if it fits into available Flash*/

#define FLASH_DATA_AREA_SIZE            (FLASH_ITS_AREA_SIZE + \
                                         FLASH_PS_AREA_SIZE + \
                                         FLASH_UNUSED_AREA_SIZE + \
                                         FLASH_OTP_NV_COUNTERS_AREA_SIZE + \
                                         FLASH_AREA_SYSTEM_RESERVED_SIZE)

#if (FLASH_DATA_AREA_OFFSET + FLASH_DATA_AREA_SIZE) > (FLASH_TOTAL_SIZE)
#error "Out of Flash memory"
#endif

#define TFM_HAL_FLASH_PROGRAM_UNIT      0x1

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
#define TFM_HAL_PS_SECTORS_PER_BLOCK  8
/* Smallest flash programmable unit in bytes */
#define TFM_HAL_PS_PROGRAM_UNIT       0x1

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
#define TFM_HAL_ITS_SECTORS_PER_BLOCK  8
/* Smallest flash programmable unit in bytes */
#define TFM_HAL_ITS_PROGRAM_UNIT       0x1
/* Decrease flash wear slightly, at the cost of increased ITS service memory */
#define ITS_MAX_BLOCK_DATA_COPY 512

/* OTP / NV counter definitions */
#define TFM_OTP_NV_COUNTERS_AREA_SIZE   (FLASH_OTP_NV_COUNTERS_AREA_SIZE / 2)
#define TFM_OTP_NV_COUNTERS_AREA_ADDR   FLASH_OTP_NV_COUNTERS_AREA_OFFSET
#define TFM_OTP_NV_COUNTERS_SECTOR_SIZE FLASH_OTP_NV_COUNTERS_SECTOR_SIZE
#define TFM_OTP_NV_COUNTERS_BACKUP_AREA_ADDR (TFM_OTP_NV_COUNTERS_AREA_ADDR + \
                                              TFM_OTP_NV_COUNTERS_AREA_SIZE)


/* Use Flash to store Code data */
#define S_ROM_ALIAS_BASE  (0x10000000)
#define NS_ROM_ALIAS_BASE (0x10000000)

/* Use SRAM to store RW data */
#define S_RAM_ALIAS_BASE  (0x08000000)
#define NS_RAM_ALIAS_BASE (0x08000000)

#endif /* __FLASH_LAYOUT_H__ */
