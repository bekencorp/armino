/*
 * Copyright (c) 2018-2021 Arm Limited. All rights reserved.
 * Copyright 2019-2021 NXP. All rights reserved.
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

/* Flash layout on LPC55S69 with BL2 (multiple image boot):
 *
 * 0x0000_0000 BL2 - MCUBoot (32 KB)
 * 0x0000_8000 Secure image     primary slot (160 KB)
 * 0x0003_0000 Non-secure image primary slot (96 KB)
 * 0x0004_8000 Secure image     secondary slot (160 KB)
 * 0x0007_0000 Non-secure image secondary slot (96 KB)
 * 0x0008_8000 Protected Storage Area (10 KB)
 * 0x0008_A800 Internal Trusted Storage Area (8 KB)
 * 0x0008_C800 OTP / NV counters area (2 KB)
 * 0x0008_D000 Unused (44 KB)
 *
 * Flash layout on LPC55S69 with BL2 (single image boot):
 *
 * 0x0000_0000 BL2 - MCUBoot (32 KB)
 * 0x0000_8000 Primary image area   (Secure + Non-secure images) (256 KB)
 * 0x0004_8000 Secondary image area (Secure + Non-secure images) (256 KB)
 * 0x0008_8000 Protected Storage Area (10 KB)
 * 0x0008_A800 Internal Trusted Storage Area (8 KB)
 * 0x0008_C800 OTP / NV counters area (2 KB)
 * 0x0008_D000 Unused (44 KB)
 *
 * Flash layout on LPC55S69 without BL2:
 *
 * 0x0000_0000 Primary image area(576 kB):
 *    0x0000_0000 Secure     image primary (320 kB)
 *    0x0005_0000 Non-secure image primary (256 kB)
 * Reserved area:
 * 0xXXXX_XXXX Secure Binary tail Area (4 KB), if SB is used.
 * 0xXXXX_XXXX Protected Storage Area (3 KB)
 * 0xXXXX_XXXX Internal Trusted Storage Area (3 KB)
 * 0xXXXX_XXXX OTP area (2 KB)
 * 0xXXXX_XXXX Unused
 */

/* This header file is included from linker scatter file as well, where only a
 * limited C constructs are allowed. Therefore it is not possible to include
 * here the platform_base_address.h to access flash related defines. To resolve
 * this some of the values are redefined here with different names, these are
 * marked with comment.
 */

/* Size of a Secure and of a Non-secure image */
#ifdef BL2

#define FLASH_S_PARTITION_SIZE              (0x28000) /* S partition: 160 KB */
#define FLASH_NS_PARTITION_SIZE             (0x18000) /* NS partition: 96 KB */
#define FLASH_MAX_PARTITION_SIZE            ((FLASH_S_PARTITION_SIZE >   \
                                             FLASH_NS_PARTITION_SIZE) ? \
                                             FLASH_S_PARTITION_SIZE :    \
                                             FLASH_NS_PARTITION_SIZE)

#else /* NO BL2 */

#define FLASH_S_PARTITION_SIZE              (0x50000)       /* S partition: 320 kB*/
#define FLASH_NS_PARTITION_SIZE             (0x40000)       /* NS partition: 256 kB*/

#endif /* BL2 */

/* Sector size of the embedded flash hardware (erase/program) */
#define FLASH_AREA_IMAGE_SECTOR_SIZE        (512)           /* 512 B. Flash memory program/erase operations have a page granularity. */

/* FLASH size */
#define FLASH_TOTAL_SIZE                    (0x000A0000-(17*FLASH_AREA_IMAGE_SECTOR_SIZE))    /* 631.5 kB. The last 17 pages (8.5KB) are reserved for PFR on the 640KB flash. */
                                                            
/* Flash layout info for BL2 bootloader */
#define FLASH_BASE_ADDRESS                  (0x00000000)

#ifdef BL2
/* Offset and size definitions of the flash partitions that are handled by the
 * bootloader. The image swapping is done between IMAGE_PRIMARY and
 * IMAGE_SECONDARY, SCRATCH is used as a temporary storage during image
 * swapping.
 */
#define FLASH_AREA_BL2_OFFSET      (0x0)
#define FLASH_AREA_BL2_SIZE        (0x8000) /* 32 KB */

#if !defined(MCUBOOT_IMAGE_NUMBER) || (MCUBOOT_IMAGE_NUMBER == 1)
/* Secure + Non-secure image primary slot */
#define FLASH_AREA_0_ID            (1)
#define FLASH_AREA_0_OFFSET        (FLASH_AREA_BL2_OFFSET + FLASH_AREA_BL2_SIZE)
#define FLASH_AREA_0_SIZE          (FLASH_S_PARTITION_SIZE + \
                                    FLASH_NS_PARTITION_SIZE)
/* Secure + Non-secure secondary slot */
#define FLASH_AREA_2_ID            (FLASH_AREA_0_ID + 1)
#define FLASH_AREA_2_OFFSET        (FLASH_AREA_0_OFFSET + FLASH_AREA_0_SIZE)
#define FLASH_AREA_2_SIZE          (FLASH_S_PARTITION_SIZE + \
                                    FLASH_NS_PARTITION_SIZE)
/* Not used (scratch area), the 'Swap' firmware upgrade operation is not
 * supported on LPC55S69.
 */
#define FLASH_AREA_SCRATCH_ID      (FLASH_AREA_2_ID + 1)
#define FLASH_AREA_SCRATCH_OFFSET  (FLASH_AREA_2_OFFSET + FLASH_AREA_2_SIZE)
#define FLASH_AREA_SCRATCH_SIZE    (0)
/* Maximum number of image sectors supported by the bootloader. */
#define MCUBOOT_MAX_IMG_SECTORS    ((FLASH_S_PARTITION_SIZE + \
                                     FLASH_NS_PARTITION_SIZE) / \
                                    FLASH_AREA_IMAGE_SECTOR_SIZE)
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
#define FLASH_AREA_2_OFFSET        (FLASH_AREA_1_OFFSET + FLASH_AREA_1_SIZE)
#define FLASH_AREA_2_SIZE          (FLASH_S_PARTITION_SIZE)
/* Non-secure image secondary slot */
#define FLASH_AREA_3_ID            (FLASH_AREA_2_ID + 1)
#define FLASH_AREA_3_OFFSET        (FLASH_AREA_2_OFFSET + FLASH_AREA_2_SIZE)
#define FLASH_AREA_3_SIZE          (FLASH_NS_PARTITION_SIZE)
/* Not used (scratch area), the 'Swap' firmware upgrade operation is not
 * supported on LPC55S69.
 */
#define FLASH_AREA_SCRATCH_ID      (FLASH_AREA_3_ID + 1)
#define FLASH_AREA_SCRATCH_OFFSET  (FLASH_AREA_3_OFFSET + FLASH_AREA_3_SIZE)
#define FLASH_AREA_SCRATCH_SIZE    (0)
/* Maximum number of image sectors supported by the bootloader. */
#define MCUBOOT_MAX_IMG_SECTORS    (FLASH_MAX_PARTITION_SIZE / \
                                    FLASH_AREA_IMAGE_SECTOR_SIZE)
#else /* MCUBOOT_IMAGE_NUMBER > 2 */
#error "Only MCUBOOT_IMAGE_NUMBER 1 and 2 are supported!"
#endif /* MCUBOOT_IMAGE_NUMBER */

/* Not used, the 'Swap' firmware upgrade operation is not supported on LPC55S69.
 * The maximum number of status entries supported by the bootloader.
 */
#define MCUBOOT_STATUS_MAX_ENTRIES (0)

#else /* NO BL2 */


#ifdef SB_FILE /* Use signed Secure Binary (SB) image */
#define FLASH_SB_TAIL   0x1000 /* 4 KB */
#else
#define FLASH_SB_TAIL   0x0 /* 0 KB */
#endif

/* Secure + Non-secure image primary slot */
#define FLASH_AREA_0_ID            (1)
#define FLASH_AREA_0_OFFSET        (0x0)
#define FLASH_AREA_0_SIZE          (FLASH_S_PARTITION_SIZE + \
                                    FLASH_NS_PARTITION_SIZE + \
                                    FLASH_SB_TAIL)
                                    
/* Not used*/
#define FLASH_AREA_SCRATCH_ID      (FLASH_AREA_0_ID + 1)
#define FLASH_AREA_SCRATCH_OFFSET  (FLASH_AREA_0_OFFSET + FLASH_AREA_0_SIZE)
#define FLASH_AREA_SCRATCH_SIZE    (0)
#endif /* BL2 */

/* Note: FLASH_PS_AREA_OFFSET, FLASH_ITS_AREA_OFFSET and
 * FLASH_OTP_NV_COUNTERS_AREA_OFFSET point to offsets in flash, but reads and
 * writes to these addresses are redirected to Code SRAM by Driver_Flash.c.
 */

/* Protected Storage (PS) Service definitions */
#define FLASH_PS_AREA_OFFSET            (FLASH_AREA_SCRATCH_OFFSET + \
                                         FLASH_AREA_SCRATCH_SIZE)
#define FLASH_PS_AREA_SIZE              (0xC00) /* 3 KB */

/* Internal Trusted Storage (ITS) Service definitions */
#define FLASH_ITS_AREA_OFFSET           (FLASH_PS_AREA_OFFSET + \
                                         FLASH_PS_AREA_SIZE)
#define FLASH_ITS_AREA_SIZE             (0xC00) /* 3 KB */

/* OTP_definitions */
#define FLASH_OTP_NV_COUNTERS_AREA_OFFSET (FLASH_ITS_AREA_OFFSET + \
                                           FLASH_ITS_AREA_SIZE)
#define FLASH_OTP_NV_COUNTERS_AREA_SIZE   (FLASH_AREA_IMAGE_SECTOR_SIZE * 4)
#define FLASH_OTP_NV_COUNTERS_SECTOR_SIZE FLASH_AREA_IMAGE_SECTOR_SIZE

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
/* Smallest flash programmable unit in bytes */
#define TFM_HAL_FLASH_PROGRAM_UNIT       FLASH_AREA_IMAGE_SECTOR_SIZE

/* Protected Storage (PS) Service definitions
 * Note: Further documentation of these definitions can be found in the
 * TF-M PS Integration Guide.
 */
#define TFM_HAL_PS_FLASH_DRIVER Driver_FLASH0

/* In this target the CMSIS driver requires only the offset from the base
 * address instead of the full memory address.
 */
/* Base address of dedicated flash area for PS */
#define TFM_HAL_PS_FLASH_AREA_ADDR      FLASH_PS_AREA_OFFSET
/* Size of dedicated flash area for PS */
#define TFM_HAL_PS_FLASH_AREA_SIZE      FLASH_PS_AREA_SIZE
#define PS_RAM_FS_SIZE                  TFM_HAL_PS_FLASH_AREA_SIZE
/* Number of physical erase sectors per logical FS block */
#define TFM_HAL_PS_SECTORS_PER_BLOCK    3

/* Smallest flash programmable unit in bytes */
#define TFM_HAL_PS_PROGRAM_UNIT         FLASH_AREA_IMAGE_SECTOR_SIZE
#define PS_FLASH_NAND_BUF_SIZE          (FLASH_AREA_IMAGE_SECTOR_SIZE * \
                                        TFM_HAL_ITS_SECTORS_PER_BLOCK)

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
#define TFM_HAL_ITS_SECTORS_PER_BLOCK   3

/* Smallest flash programmable unit in bytes */
#define TFM_HAL_ITS_PROGRAM_UNIT        FLASH_AREA_IMAGE_SECTOR_SIZE
#define ITS_FLASH_NAND_BUF_SIZE        (FLASH_AREA_IMAGE_SECTOR_SIZE * \
                                        TFM_HAL_ITS_SECTORS_PER_BLOCK)

/* OTP / NV counter definitions */
#define TFM_OTP_NV_COUNTERS_AREA_SIZE   (FLASH_OTP_NV_COUNTERS_AREA_SIZE / 2)
#define TFM_OTP_NV_COUNTERS_AREA_ADDR   FLASH_OTP_NV_COUNTERS_AREA_OFFSET
#define TFM_OTP_NV_COUNTERS_SECTOR_SIZE FLASH_OTP_NV_COUNTERS_SECTOR_SIZE
#define TFM_OTP_NV_COUNTERS_BACKUP_AREA_ADDR (TFM_OTP_NV_COUNTERS_AREA_ADDR + \
                                              TFM_OTP_NV_COUNTERS_AREA_SIZE)

/* Use Flash memory to store Code data */
#define S_ROM_ALIAS_BASE    (0x10000000)
#define NS_ROM_ALIAS_BASE   (0x00000000)

/* Use SRAM[0-4] memory to store RW data */
#define S_RAM_ALIAS_BASE    (0x30000000)
#define NS_RAM_ALIAS_BASE   (0x20000000)

#define TOTAL_ROM_SIZE      FLASH_TOTAL_SIZE
#define TOTAL_RAM_SIZE      (0x00044000)     /* RAM 0-4. 272 KB RAM for data (without SRAM X for code)*/

#endif /* __FLASH_LAYOUT_H__ */