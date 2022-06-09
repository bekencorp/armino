/*
 * Copyright (c) 2018-2021 Arm Limited. All rights reserved.
 * Copyright (c) 2020 Nordic Semiconductor ASA. All rights reserved.
 * Copyright (c) 2021 Laird Connectivity. All rights reserved.
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

/* Flash layout on BL5340 Application MCU with BL2:
 *
 * Internal flash:
 * 0x0000_0000 BL2 - MCUBoot (64 KB)
 * 0x0001_0000 Primary image area (864 KB):
 *    0x0001_0000 Secure     image primary (576 KB)
 *    0x000A_0000 Non-secure image primary (288 KB)
 * 0x000e_8000 Scratch (32 KB)
 * 0x000f_0000 Protected Storage area (16 KB)
 * 0x000f_4000 Internal Trusted Storage area (8 KB)
 * 0x000f_6000 OTP / NV counters area (8 KB)
 * 0x000f_8000 Unused (optional NVS used in Zephyr) (32 KB)
 * QSPI:
 * 0x0000_0000 Secondary image area (864 KB):
 *    0x0000_0000 Secure     image secondary (576 KB)
 *    0x0009_0000 Non-secure image secondary (288 KB)
 * 0x000d_8000 Unused (160 KB)
 * 0x0010_0000 Unused (optional LFS used in Zephyr) (7 MB)
 *
 *
 * Flash layout on BL5340 Application MCU without BL2:
 *
 * Internal flash:
 * 0x0000_0000 Primary image area (928 KB):
 *    0x0000_0000 Secure     image primary (576 KB)
 *    0x0009_0000 Non-secure image primary (352 KB)
 * 0x000e_8000 Scratch (32 KB)
 * 0x000f_0000 Protected Storage main area (16 KB)
 * 0x000f_4000 Internal Trusted Storage main area (8 KB)
 * 0x000f_6000 OTP / NV counters area (8 KB)
 * 0x000f_8000 Unused (optional NVS used in Zephyr) (32 KB)
 * QSPI:
 * 0x0000_0000 Unused (1 MB)
 * 0x0010_0000 Unused (optional LFS used in Zephyr) (7 MB)
 */

/* This header file is included from linker scatter file as well, where only a
 * limited C constructs are allowed. Therefore it is not possible to include
 * here the platform_base_address.h to access flash related defines. To resolve
 * this some of the values are redefined here with different names, these are
 * marked with comment.
 */

/* Size of a Secure and of a Non-secure image */
#define FLASH_S_PARTITION_SIZE                (0x90000)       /* S partition: 576 KB */
#ifdef BL2
/* Using BL2 */
#define FLASH_NS_PARTITION_SIZE               (0x48000)       /* NS partition: 288 KB */
#else
/* Without BL2 */
#define FLASH_NS_PARTITION_SIZE               (0x58000)       /* NS partition: 352 KB */
#endif
#define FLASH_MAX_PARTITION_SIZE        ((FLASH_S_PARTITION_SIZE >   \
                                          FLASH_NS_PARTITION_SIZE) ? \
                                         FLASH_S_PARTITION_SIZE :    \
                                         FLASH_NS_PARTITION_SIZE)

/* Sector size of the embedded flash hardware (erase/program) */
#define FLASH_AREA_IMAGE_SECTOR_SIZE        (0x1000)          /* 4 KB. Flash memory program/erase operations have a page granularity. */

/* FLASH size */
#define FLASH_TOTAL_SIZE                    (0x100000)        /* 1024 KB */

/* Sector size of QSPI flash chip */
#define QSPI_FLASH_AREA_IMAGE_SECTOR_SIZE   (0x1000)          /* 4 KB */

/* Total size of QSPI flash chip */
#define QSPI_FLASH_TOTAL_SIZE               (0x100000)        /* 8192 KB = 8 MB */

/* Flash layout info for BL2 bootloader */
#define FLASH_BASE_ADDRESS                  (0x00000000)


/* Offset and size definitions of the flash partitions that are handled by the
 * bootloader. The image swapping is done between IMAGE_PRIMARY and
 * IMAGE_SECONDARY, SCRATCH is used as a temporary storage during image
 * swapping.
 */
#define FLASH_AREA_BL2_OFFSET      (0x0)
#define FLASH_AREA_BL2_SIZE        (0x10000) /* 64 KB */

#define FLASH_AREA_SCRATCH_SIZE    (0x8000) /* 32 KB */

#if !defined(MCUBOOT_IMAGE_NUMBER) || (MCUBOOT_IMAGE_NUMBER == 1)
/* Secure + Non-secure image primary slot (internal flash) */
#define FLASH_AREA_0_ID            (1)
#define FLASH_AREA_0_OFFSET        (FLASH_AREA_BL2_OFFSET + FLASH_AREA_BL2_SIZE)
#define FLASH_AREA_0_SIZE          (FLASH_S_PARTITION_SIZE + \
                                    FLASH_NS_PARTITION_SIZE)

/* Secure + Non-secure secondary slot (QSPI) */
#define FLASH_AREA_2_ID            (FLASH_AREA_0_ID + 1)
#define FLASH_AREA_2_OFFSET        (0)
#define FLASH_AREA_2_SIZE          (FLASH_S_PARTITION_SIZE + \
                                    FLASH_NS_PARTITION_SIZE)

/* Scratch area */
#define FLASH_AREA_SCRATCH_ID      (FLASH_AREA_2_ID + 1)
#define FLASH_AREA_SCRATCH_OFFSET  (FLASH_AREA_0_OFFSET + FLASH_AREA_0_SIZE)

/* Maximum number of image sectors supported by the bootloader. */
#define MCUBOOT_MAX_IMG_SECTORS    ((FLASH_S_PARTITION_SIZE + \
                                     FLASH_NS_PARTITION_SIZE) / \
                                    FLASH_AREA_IMAGE_SECTOR_SIZE)

#define MCUBOOT_STATUS_MAX_ENTRIES ((FLASH_S_PARTITION_SIZE + \
                                     FLASH_NS_PARTITION_SIZE) / \
                                    FLASH_AREA_SCRATCH_SIZE)
#elif (MCUBOOT_IMAGE_NUMBER == 2)
/* Secure image primary slot */
#define FLASH_AREA_0_ID            (1)
#define FLASH_AREA_0_OFFSET        (FLASH_AREA_BL2_OFFSET + FLASH_AREA_BL2_SIZE)
#define FLASH_AREA_0_SIZE          (FLASH_S_PARTITION_SIZE)

/* Non-secure image primary slot */
#define FLASH_AREA_1_ID            (FLASH_AREA_0_ID + 1)
#define FLASH_AREA_1_OFFSET        (FLASH_AREA_0_OFFSET + FLASH_AREA_0_SIZE)
#define FLASH_AREA_1_SIZE          (FLASH_NS_PARTITION_SIZE)

/* Secure image secondary slot */
#define FLASH_AREA_2_ID            (FLASH_AREA_1_ID + 1)
#define FLASH_AREA_2_OFFSET        (0)
#define FLASH_AREA_2_SIZE          (FLASH_S_PARTITION_SIZE)

/* Non-secure image secondary slot */
#define FLASH_AREA_3_ID            (FLASH_AREA_2_ID + 1)
#define FLASH_AREA_3_OFFSET        (FLASH_AREA_2_OFFSET + FLASH_AREA_2_SIZE)
#define FLASH_AREA_3_SIZE          (FLASH_NS_PARTITION_SIZE)

/* Scratch area */
#define FLASH_AREA_SCRATCH_ID      (FLASH_AREA_3_ID + 1)
#define FLASH_AREA_SCRATCH_OFFSET  (FLASH_AREA_1_OFFSET + FLASH_AREA_1_SIZE)

/* Maximum number of image sectors supported by the bootloader. */
#define MCUBOOT_STATUS_MAX_ENTRIES (FLASH_MAX_PARTITION_SIZE / \
                                    FLASH_AREA_SCRATCH_SIZE)

#define MCUBOOT_MAX_IMG_SECTORS    (FLASH_MAX_PARTITION_SIZE / \
                                    FLASH_AREA_IMAGE_SECTOR_SIZE)
#else /* MCUBOOT_IMAGE_NUMBER > 2 */
#error "Only MCUBOOT_IMAGE_NUMBER 1 and 2 are supported!"
#endif /* MCUBOOT_IMAGE_NUMBER */

#define FLASH_PS_AREA_OFFSET            (FLASH_AREA_SCRATCH_OFFSET + FLASH_AREA_SCRATCH_SIZE)
#define FLASH_PS_AREA_SIZE              (0x4000)   /* 16 KB */

/* Internal Trusted Storage (ITS) Service definitions */
#define FLASH_ITS_AREA_OFFSET           (FLASH_PS_AREA_OFFSET + \
                                         FLASH_PS_AREA_SIZE)
#define FLASH_ITS_AREA_SIZE             (0x2000)   /* 8 KB */

/* OTP_definitions */
#define FLASH_OTP_NV_COUNTERS_AREA_OFFSET (FLASH_ITS_AREA_OFFSET + \
                                           FLASH_ITS_AREA_SIZE)
#define FLASH_OTP_NV_COUNTERS_AREA_SIZE   (FLASH_AREA_IMAGE_SECTOR_SIZE * 2)
#define FLASH_OTP_NV_COUNTERS_SECTOR_SIZE FLASH_AREA_IMAGE_SECTOR_SIZE

/* Non-secure storage region */
#define NRF_FLASH_NS_STORAGE_AREA_OFFSET    (FLASH_TOTAL_SIZE - \
                                             NRF_FLASH_NS_STORAGE_AREA_SIZE)
#define NRF_FLASH_NS_STORAGE_AREA_SIZE      (0x8000)   /* 32 KB */

/* Offset and size definition in flash area used by assemble.py */
#define SECURE_IMAGE_OFFSET             (0x0)
#define SECURE_IMAGE_MAX_SIZE           FLASH_S_PARTITION_SIZE

#define NON_SECURE_IMAGE_OFFSET         (SECURE_IMAGE_OFFSET + \
                                         SECURE_IMAGE_MAX_SIZE)
#define NON_SECURE_IMAGE_MAX_SIZE       FLASH_NS_PARTITION_SIZE

/* Flash device name used by BL2
 * Name is defined in flash driver file: Driver_Flash.c
 */
#define FLASH_DEV_NAME Driver_FLASH0
#define TFM_HAL_FLASH_PROGRAM_UNIT       (0x4)

/* Flash device name used by secondary slot and scratch area
 * Name is defined in flash driver file: Driver_QSPI.c
 */
#define FLASH_DEV_NAME_2  Driver_FLASH1
#define FLASH_DEVICE_ID_2 (FLASH_DEVICE_ID+1)
#define FLASH_DEV_NAME_3  Driver_FLASH1
#define FLASH_DEVICE_ID_3 (FLASH_DEVICE_ID+1)
#define FLASH_DEV_NAME_SCRATCH Driver_FLASH0
#define FLASH_DEVICE_ID_SCRATCH (FLASH_DEVICE_ID)

#define TFM_HAL_ITS_FLASH_DRIVER Driver_FLASH0
#define TFM_HAL_PS_FLASH_DRIVER Driver_FLASH0

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
#define TFM_HAL_PS_PROGRAM_UNIT       (0x4)

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
#define TFM_HAL_ITS_PROGRAM_UNIT       (0x4)

/* OTP / NV counter definitions */
#define TFM_OTP_NV_COUNTERS_AREA_SIZE   (FLASH_OTP_NV_COUNTERS_AREA_SIZE / 2)
#define TFM_OTP_NV_COUNTERS_AREA_ADDR   FLASH_OTP_NV_COUNTERS_AREA_OFFSET
#define TFM_OTP_NV_COUNTERS_SECTOR_SIZE FLASH_OTP_NV_COUNTERS_SECTOR_SIZE
#define TFM_OTP_NV_COUNTERS_BACKUP_AREA_ADDR (TFM_OTP_NV_COUNTERS_AREA_ADDR + \
                                              TFM_OTP_NV_COUNTERS_AREA_SIZE)

/* Use Flash memory to store Code data */
#define FLASH_BASE_ADDRESS (0x00000000)
#define S_ROM_ALIAS_BASE   FLASH_BASE_ADDRESS
#define NS_ROM_ALIAS_BASE  FLASH_BASE_ADDRESS

/* Use SRAM memory to store RW data */
#define SRAM_BASE_ADDRESS (0x20000000)
#define S_RAM_ALIAS_BASE  SRAM_BASE_ADDRESS
#define NS_RAM_ALIAS_BASE SRAM_BASE_ADDRESS

#define TOTAL_ROM_SIZE FLASH_TOTAL_SIZE
#define TOTAL_RAM_SIZE (0x00080000)     /* 512 KB */

#endif /* __FLASH_LAYOUT_H__ */
