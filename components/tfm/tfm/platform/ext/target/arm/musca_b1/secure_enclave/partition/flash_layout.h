/*
 * Copyright (c) 2018-2021 Arm Limited. All rights reserved.
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

/* Flash (and Code SRAM) layout on Musca-B1 if Secure Enclave (SE) is used
 * BL2 is mandatory in this case, and MCUBOOT_IMAGE_NUMBER must be 2
 *
 * SE images are placed in eFlash 0 and eFlash 1 and remapped to SE's memory
 * space by ROM and CODE remap.
 * ROM  remap offset should be set to 0x1A02_0000
 * ROM  remap mask   should be set to 0x0001_FFFF
 * CODE remap offset should be set to 0x1A20_0000
 * CODE remap mask   should be set to 0x003F_FFFF
 *
 * Some memory areas are not accessible for SE over the remap address ranges.
 * ITS, PS, and NV counters are accessible for SE over the flash controllers.
 * The image base addresses for both subsystems can be found in the following
 * table, N/A for unaccessible addresses:
 *
*                                                  SSE-200 address  SE address
 * eFlash 0:
 * SSE-200 BL0 (128 KiB)                           0x1A000000       N/A
 * SE MCUBoot (128 KiB)                            0x1A020000       0x00000000
 * Internal Trusted Storage Area (32 KiB)          0x1A040000       N/A
 * OTP / NV counter area (8  KiB)                  0x1A048000       N/A
 * Unused (208 KiB)                                0x1A04A000       N/A
 *
 * eFlash 1:
 * SE TF-M image          primary slot (384 KiB)   0x1A200000       0x38000000
 * SSE-200 combined image primary slot (512 KiB)   0x1A260000       0x38060000
 * SE TF-M image          secondary slot (384 KiB) 0x1A2E0000       0x380E0000
 * SSE-200 combined image secondary slot (512 KiB) 0x1A360000       0x38160000
 * Scratch area (64 KiB)                           0x1A3C0000       0x381C0000
 * Unused (196 KiB)                                0x1A3D0000       0x381D0000
 *
 * Code SRAM:
 * SSE-200 BL0 ramload area (16 KiB)               0x1A400000       0x38200000
 * SE MCUBoot eFlash driver ramload area (16 KiB)  0x1A404000       0x38204000
 * IPC Shared memory area (476 KiB)                0x1A408000       0x38208000
 *
 * The SE MCUBoot eFlash driver is copied into Code SRAM. If the driver would
 * run from eFlash0 it would not be possible to write the NV counters also
 * placed in eFlash0. It is not advisable to write to and fetch instructions
 * from flash at the same time. For the same reason SSE-200 BL0 is running
 * from Code SRAM.
 */

/* This header file is included from linker scatter file as well, where only a
 * limited C constructs are allowed. Therefore it is not possible to include
 * here the platform_base_address.h to access flash related defines. To resolve this
 * some of the values are redefined here with different names, these are marked
 * with comment.
 */

/* The size of partitions */
#define FLASH_S_PARTITION_SIZE          (0x60000)      /* 384 KiB */
#define FLASH_SSE_200_PARTITION_SIZE    (0x80000)      /* 512 KiB */
#define FLASH_MAX_PARTITION_SIZE        ((FLASH_S_PARTITION_SIZE >   \
                                          FLASH_SSE_200_PARTITION_SIZE) ? \
                                         FLASH_S_PARTITION_SIZE :    \
                                         FLASH_SSE_200_PARTITION_SIZE)

/* Sector size of the embedded flash hardware */
#define FLASH_AREA_IMAGE_SECTOR_SIZE    (0x4000)   /* 16 KB */
#define FLASH_TOTAL_SIZE                (0x200000) /* 2 MB */

/* Sector size of the QSPI flash hardware */
#define QSPI_FLASH_AREA_IMAGE_SECTOR_SIZE (0x1000)   /* 4 KB */
#define QSPI_FLASH_TOTAL_SIZE             (0x800000) /* 8 MB */


/* Flash layout info for BL2 bootloader, images are placed in eFlash1 */
#define FLASH_BASE_ADDRESS              (0x38000000)

#if !defined(MCUBOOT_IMAGE_NUMBER) || (MCUBOOT_IMAGE_NUMBER == 2)
/* SE TF-M image primary slot */
#define FLASH_AREA_0_ID            (1)
#define FLASH_AREA_0_OFFSET        (0)
#define FLASH_AREA_0_SIZE          (FLASH_S_PARTITION_SIZE)

/* SSE-200 image primary slot */
#define FLASH_AREA_1_ID            (FLASH_AREA_0_ID + 1)
#define FLASH_AREA_1_OFFSET        (FLASH_AREA_0_OFFSET + FLASH_AREA_0_SIZE)
#define FLASH_AREA_1_SIZE          (FLASH_SSE_200_PARTITION_SIZE)

/* SE TF-M image secondary slot */
#define FLASH_AREA_2_ID            (FLASH_AREA_1_ID + 1)
#define FLASH_AREA_2_OFFSET        (FLASH_AREA_1_OFFSET + FLASH_AREA_1_SIZE)
#define FLASH_AREA_2_SIZE          (FLASH_S_PARTITION_SIZE)

/* SSE-200 image secondary slot */
#define FLASH_AREA_3_ID            (FLASH_AREA_2_ID + 1)
#define FLASH_AREA_3_OFFSET        (FLASH_AREA_2_OFFSET + FLASH_AREA_2_SIZE)
#define FLASH_AREA_3_SIZE          (FLASH_SSE_200_PARTITION_SIZE)

/* Scratch area */
#define FLASH_AREA_SCRATCH_ID      (FLASH_AREA_3_ID + 1)
#define FLASH_AREA_SCRATCH_OFFSET  (FLASH_AREA_3_OFFSET + FLASH_AREA_3_SIZE)
#define FLASH_AREA_SCRATCH_SIZE    (4 * FLASH_AREA_IMAGE_SECTOR_SIZE)

/* The maximum number of status entries supported by the bootloader. */
#define MCUBOOT_STATUS_MAX_ENTRIES (FLASH_MAX_PARTITION_SIZE / \
                                    FLASH_AREA_SCRATCH_SIZE)

/* Maximum number of image sectors supported by the bootloader. */
#define NEEDED_MCUBOOT_IMG_SECTORS (FLASH_MAX_PARTITION_SIZE / \
                                    FLASH_AREA_IMAGE_SECTOR_SIZE)
#define MCUBOOT_MAX_IMG_SECTORS    (NEEDED_MCUBOOT_IMG_SECTORS > 32 ? \
                                    NEEDED_MCUBOOT_IMG_SECTORS :      \
                                    32)

#else /* MCUBOOT_IMAGE_NUMBER == 2 */
#error "Only MCUBOOT_IMAGE_NUMBER 2 is supported!"
#endif /* MCUBOOT_IMAGE_NUMBER */

/* Flash device name used by BL2
 * Name is defined in flash driver file: Driver_Flash.c
 */
#define FLASH_DEV_NAME Driver_EFLASH1
/* Smallest flash programmable unit in bytes */
#define TFM_HAL_FLASH_PROGRAM_UNIT       (0x4)


/* Assets in eFlash0 */
/* SSE-200 BL0 component */
#define SSE200_BL0_OFFSET               (0x0)
#define SSE200_BL0_SIZE                 (0x20000) /* 128 KB */

/* SE BL2 image */
#define FLASH_AREA_BL2_OFFSET            (SSE200_BL0_OFFSET + \
                                         SSE200_BL0_SIZE)
/* Maximum memory window size with ROM remap */
#define FLASH_AREA_BL2_SIZE              (0x20000) /* 128 KB */

/* Internal Trusted Storage (ITS) Service definitions */
#define FLASH_ITS_AREA_OFFSET           (FLASH_AREA_BL2_OFFSET + \
                                         FLASH_AREA_BL2_SIZE)
#define FLASH_ITS_AREA_SIZE             (2 * FLASH_AREA_IMAGE_SECTOR_SIZE)

/* OTP_definitions */
#define FLASH_OTP_NV_COUNTERS_AREA_OFFSET (FLASH_ITS_AREA_OFFSET + \
                                           FLASH_ITS_AREA_SIZE)
#define FLASH_OTP_NV_COUNTERS_AREA_SIZE   (FLASH_AREA_IMAGE_SECTOR_SIZE * 2)
#define FLASH_OTP_NV_COUNTERS_SECTOR_SIZE FLASH_AREA_IMAGE_SECTOR_SIZE

/* Internal Trusted Storage (ITS) Service definitions
 * Note: Further documentation of these definitions can be found in the
 * TF-M ITS Integration Guide.
 */
#define TFM_HAL_ITS_FLASH_DRIVER Driver_EFLASH0

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

/* Protected Storage (PS) Service definitions. PS is placed in QSPI flash */
#define FLASH_PS_AREA_OFFSET            (0x0)
#define FLASH_PS_AREA_SIZE              (5 * QSPI_FLASH_AREA_IMAGE_SECTOR_SIZE)

/* Protected Storage (PS) Service definitions
 * Note: Further documentation of these definitions can be found in the
 * TF-M PS Integration Guide.
 */
#define TFM_HAL_PS_FLASH_DRIVER Driver_QSPI_FLASH0

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

#endif /* __FLASH_LAYOUT_H__ */
