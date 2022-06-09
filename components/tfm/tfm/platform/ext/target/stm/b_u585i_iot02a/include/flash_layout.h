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

/* This header file is included from linker scatter file as well, where only a
 * limited C constructs are allowed. Therefore it is not possible to include
 * here the platform_retarget.h to access flash related defines. To resolve this
 * some of the values are redefined here with different names, these are marked
 * with comment.
 */
 /* Flash layout for b_u585i_iot02a  with BL2 (multiple image boot):
 *
 * 0x0000_0000 SCRATCH (64KB)
 * 0x0001_0000 BL2 - counters(16 KB)
 * 0x0001_4000 BL2 - MCUBoot (84 KB)
 * 0x0002_7000 OTP Write Protect (4KB)
 * 0x0002_8000 NV counters area (16 KB)
 * 0x0002_c000 Secure Storage Area (16 KB)
 * 0x0003_0000 Internal Trusted Storage Area (16 KB)
 * 0x0003_4000 Secure image     primary slot (256 KB)
 * 0x0007_4000 Non-secure image primary slot (512 KB)
 * 0x000f_4000 Secure image     secondary slot (256 KB)
 * 0x0013_4000 Non-secure image secondary slot (512 KB)
 *
 * Bl2 binary is written at 0x1_2000:
 * it contains bl2_counter init value, OTP write protect, NV counters area init.
 */

/* Flash layout info for BL2 bootloader */
#define FLASH_AREA_IMAGE_SECTOR_SIZE    (0x2000)     /* 8 KB */
#define FLASH_B_SIZE                    (0x100000)   /* 1 MBytes*/
#define FLASH_TOTAL_SIZE                (FLASH_B_SIZE+FLASH_B_SIZE) /* 2 MBytes */
#define FLASH_BASE_ADDRESS              (0x0c000000) /* same as FLASH0_BASE_S */

/* Flash device ID */


/* Offset and size definitions of the flash partitions that are handled by the
 * bootloader. The image swapping is done between IMAGE_0 and IMAGE_1, SCRATCH
 * is used as a temporary storage during image swapping.
 */

/* scratch area */
#define FLASH_AREA_SCRATCH_OFFSET       (0x0)
#define FLASH_AREA_SCRATCH_SIZE         (0x10000) /* 64 KB */

/* control scratch area */
#if (FLASH_AREA_SCRATCH_OFFSET % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0
#error "FLASH_AREA_SCRATCH_OFFSET not aligned on FLASH_AREA_IMAGE_SECTOR_SIZE"
#endif /* (FLASH_AREA_SCRATCH_OFFSET % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0*/

/* area for bl2 anti roll back counter */
#define FLASH_BL2_NVCNT_AREA_OFFSET     (FLASH_AREA_SCRATCH_SIZE)
#define FLASH_BL2_NVCNT_AREA_SIZE       (FLASH_AREA_IMAGE_SECTOR_SIZE+FLASH_AREA_IMAGE_SECTOR_SIZE)
/* Area for downloading bl2 image */
#define FLASH_AREA_BL2_BIN_OFFSET       (FLASH_BL2_NVCNT_AREA_OFFSET +FLASH_AREA_IMAGE_SECTOR_SIZE)
/* personal Area Not used */
#define FLASH_AREA_PERSO_OFFSET         (FLASH_BL2_NVCNT_AREA_OFFSET +FLASH_BL2_NVCNT_AREA_SIZE)
#define FLASH_AREA_PERSO_SIZE           (0x0)
/* control personal area */
#if (FLASH_AREA_PERSO_OFFSET % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0
#error "FLASH_AREA_PERSO_OFFSET not aligned on FLASH_AREA_IMAGE_SECTOR_SIZE"
#endif /* FLASH_AREA_PERSO_OFFSET % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0 */

/* area for BL2 code protected by hdp */
#define FLASH_AREA_BL2_OFFSET           (FLASH_AREA_PERSO_OFFSET+FLASH_AREA_PERSO_SIZE )
#define FLASH_AREA_BL2_SIZE             (0x12000)
/* HDP area end at this address */
#define FLASH_BL2_HDP_END               (FLASH_AREA_BL2_OFFSET+FLASH_AREA_BL2_SIZE-1)
/* area for BL2 code not protected by hdp */
#define FLASH_AREA_BL2_NOHDP_OFFSET     (FLASH_AREA_BL2_OFFSET+FLASH_AREA_BL2_SIZE)
#define FLASH_AREA_BL2_NOHDP_CODE_SIZE  (0x1000)
#define FLASH_AREA_OTP_OFFSET           (FLASH_AREA_BL2_NOHDP_OFFSET+FLASH_AREA_BL2_NOHDP_CODE_SIZE)
#define FLASH_AREA_OTP_SIZE             (0x1000)
#define FLASH_AREA_BL2_NOHDP_SIZE       (FLASH_AREA_OTP_SIZE+FLASH_AREA_BL2_NOHDP_CODE_SIZE)
/* control area for BL2 code protected by hdp */
#if (FLASH_AREA_BL2_NOHDP_OFFSET % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0
#error "HDP area must be aligned on FLASH_AREA_IMAGE_SECTOR_SIZE"
#endif /* (FLASH_AREA_BL2_NOHDP_OFFSET % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0 */

/* Non Volatile Counters definitions */
#define FLASH_NV_COUNTERS_AREA_SIZE      (FLASH_AREA_IMAGE_SECTOR_SIZE+FLASH_AREA_IMAGE_SECTOR_SIZE)
#define FLASH_NV_COUNTERS_AREA_OFFSET   (FLASH_AREA_BL2_NOHDP_OFFSET+FLASH_AREA_BL2_NOHDP_SIZE)
/* Control Non Volatile Counters definitions */
#if (FLASH_NV_COUNTER_AREA_SIZE % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0
#error "FLASH_NV_COUNTER_AREA_SIZE not aligned on FLASH_AREA_IMAGE_SECTOR_SIZE"
#endif /*  (FLASH_NV_COUNTER_AREA_SIZE % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0 */

/* Secure Storage (PS) Service definitions */
#define FLASH_PS_AREA_SIZE             (FLASH_AREA_IMAGE_SECTOR_SIZE+FLASH_AREA_IMAGE_SECTOR_SIZE)
#define FLASH_PS_AREA_OFFSET           (FLASH_NV_COUNTERS_AREA_OFFSET+FLASH_NV_COUNTERS_AREA_SIZE)

/* Control Secure Storage (PS) Service definitions*/
#if (FLASH_PS_AREA_OFFSET % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0
#error "FLASH_PS_AREA_OFFSET not aligned on FLASH_AREA_IMAGE_SECTOR_SIZE"
#endif /*  (FLASH_PS_AREA_OFFSET % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0 */

/* Internal Trusted Storage (ITS) Service definitions */
#define FLASH_ITS_AREA_OFFSET           (FLASH_PS_AREA_OFFSET+FLASH_PS_AREA_SIZE)
#define FLASH_ITS_AREA_SIZE             (FLASH_AREA_IMAGE_SECTOR_SIZE+FLASH_AREA_IMAGE_SECTOR_SIZE)

/*Control  Internal Trusted Storage (ITS) Service definitions */
#if (FLASH_ITS_AREA_OFFSET % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0
#error "FLASH_ITS_AREA_OFFSET not aligned on FLASH_AREA_IMAGE_SECTOR_SIZE"
#endif /*  (FLASH_ITS_AREA_OFFSET % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0 */

#define FLASH_S_PARTITION_SIZE          (0x40000) /* 256 KB for S partition */
#define FLASH_NS_PARTITION_SIZE         (0x80000) /* 512 KB for NS partition */

#define FLASH_PARTITION_SIZE            (FLASH_S_PARTITION_SIZE+FLASH_NS_PARTITION_SIZE)

#define FLASH_MAX_PARTITION_SIZE        ((FLASH_S_PARTITION_SIZE >   \
                                          FLASH_NS_PARTITION_SIZE) ? \
                                         FLASH_S_PARTITION_SIZE : \
                                         FLASH_NS_PARTITION_SIZE)
/* Secure image primary slot */
#define FLASH_AREA_0_ID                 (1)
#define FLASH_AREA_0_DEVICE_ID          (FLASH_DEVICE_ID-FLASH_DEVICE_ID)
#define FLASH_AREA_0_OFFSET             (FLASH_ITS_AREA_OFFSET+FLASH_ITS_AREA_SIZE)
/* Control  Secure image primary slot */
#if (FLASH_AREA_0_OFFSET  % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0
#error "FLASH_AREA_0_OFFSET  not aligned on FLASH_AREA_IMAGE_SECTOR_SIZE"
#endif /*  (FLASH_AREA_0_OFFSET  % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0 */

#define FLASH_AREA_0_SIZE               (FLASH_S_PARTITION_SIZE)
/* Non-secure image primary slot */
#define FLASH_AREA_1_ID                 (FLASH_AREA_0_ID + 1)
#define FLASH_AREA_1_DEVICE_ID          (FLASH_AREA_0_DEVICE_ID)
#define FLASH_AREA_1_OFFSET             (FLASH_AREA_0_OFFSET + FLASH_AREA_0_SIZE)
/* Control Non-secure image primary slot */
#if (FLASH_AREA_1_OFFSET  % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0
#error "FLASH_AREA_1_OFFSET  not aligned on FLASH_AREA_IMAGE_SECTOR_SIZE"
#endif /* (FLASH_AREA_1_OFFSET  % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0  */

#define FLASH_AREA_1_SIZE               (FLASH_NS_PARTITION_SIZE)
/* Secure image secondary slot */
#define FLASH_AREA_2_ID                 (FLASH_AREA_1_ID + 1)
#define FLASH_AREA_2_DEVICE_ID          (FLASH_AREA_1_DEVICE_ID)
#define FLASH_AREA_2_OFFSET             (FLASH_AREA_1_OFFSET + FLASH_AREA_1_SIZE)
/* Control  Secure image secondary slot */
#if (FLASH_AREA_2_OFFSET  % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0
#error "FLASH_AREA_2_OFFSET  not aligned on FLASH_AREA_IMAGE_SECTOR_SIZE"
#endif /*   (FLASH_AREA_2_OFFSET  % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0 */

#define FLASH_AREA_2_SIZE               (FLASH_S_PARTITION_SIZE)
/* Non-secure image secondary slot */
#define FLASH_AREA_3_ID                 (FLASH_AREA_2_ID + 1)
#define FLASH_AREA_3_DEVICE_ID          (FLASH_AREA_2_DEVICE_ID)
#define FLASH_AREA_3_OFFSET             (FLASH_AREA_2_OFFSET + FLASH_AREA_2_SIZE)

#if (FLASH_AREA_3_OFFSET  % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0
#error "FLASH_AREA_3_OFFSET  not aligned on FLASH_AREA_IMAGE_SECTOR_SIZE"
#endif /*  (FLASH_AREA_3_OFFSET  % FLASH_AREA_IMAGE_SECTOR_SIZE) != 0 */
/*Control Non-secure image secondary slot */
#define FLASH_AREA_3_SIZE               (FLASH_NS_PARTITION_SIZE)
#define FLASH_AREA_END_OFFSET           (FLASH_AREA_3_OFFSET + FLASH_AREA_3_SIZE)
#define FLASH_AREA_SCRATCH_ID           (FLASH_AREA_3_ID + 1)
#define FLASH_AREA_SCRATCH_DEVICE_ID    (FLASH_AREA_3_DEVICE_ID)

/*
 * The maximum number of status entries supported by the bootloader.
 */
#define MCUBOOT_STATUS_MAX_ENTRIES         ((FLASH_MAX_PARTITION_SIZE) / \
                                            FLASH_AREA_SCRATCH_SIZE)
/* Maximum number of image sectors supported by the bootloader. */
#define MCUBOOT_MAX_IMG_SECTORS           ((FLASH_MAX_PARTITION_SIZE) / \
                                           FLASH_AREA_IMAGE_SECTOR_SIZE)

#define SECURE_IMAGE_OFFSET             (0x0)
#define SECURE_IMAGE_MAX_SIZE           FLASH_S_PARTITION_SIZE

#define NON_SECURE_IMAGE_OFFSET         (SECURE_IMAGE_OFFSET + SECURE_IMAGE_MAX_SIZE)
#define NON_SECURE_IMAGE_MAX_SIZE       FLASH_NS_PARTITION_SIZE

/* Flash device name used by BL2 and NV Counter
 * Name is defined in flash driver file: low_level_flash.c
 */
#define TFM_NV_COUNTERS_FLASH_DEV       TFM_Driver_FLASH0
#define FLASH_DEV_NAME                  TFM_Driver_FLASH0
#define TFM_HAL_FLASH_PROGRAM_UNIT       (0x10)
/* Protected Storage (PS) Service definitions
 * Note: Further documentation of these definitions can be found in the
 * TF-M PS Integration Guide.
 */
#define TFM_HAL_PS_FLASH_DRIVER TFM_Driver_FLASH0

/* In this target the CMSIS driver requires only the offset from the base
 * address instead of the full memory address.
 */
#define PS_SECTOR_SIZE                 FLASH_AREA_IMAGE_SECTOR_SIZE
/* The sectors must be in consecutive memory location */
#define PS_NBR_OF_SECTORS              (FLASH_PS_AREA_SIZE / PS_SECTOR_SIZE)
/* The maximum asset size to be stored in the ITS area */
#define ITS_SECTOR_SIZE                 FLASH_AREA_IMAGE_SECTOR_SIZE
/* The sectors must be in consecutive memory location */
#define ITS_NBR_OF_SECTORS              (FLASH_ITS_AREA_SIZE / ITS_SECTOR_SIZE)

/* Base address of dedicated flash area for PS */
#define TFM_HAL_PS_FLASH_AREA_ADDR    FLASH_PS_AREA_OFFSET
/* Size of dedicated flash area for PS */
#define TFM_HAL_PS_FLASH_AREA_SIZE    FLASH_PS_AREA_SIZE
#define PS_RAM_FS_SIZE                TFM_HAL_PS_FLASH_AREA_SIZE
/* Number of physical erase sectors per logical FS block */
#define TFM_HAL_PS_SECTORS_PER_BLOCK  (1)
/* Smallest flash programmable unit in bytes */
#define TFM_HAL_PS_PROGRAM_UNIT       (0x10)

/* Internal Trusted Storage (ITS) Service definitions
 * Note: Further documentation of these definitions can be found in the
 * TF-M ITS Integration Guide.
 */
#define TFM_HAL_ITS_FLASH_DRIVER TFM_Driver_FLASH0

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
#define TFM_HAL_ITS_PROGRAM_UNIT       (0x10)
/* OTP area definition */
#define TFM_OTP_NV_COUNTERS_AREA_ADDR    FLASH_AREA_OTP_OFFSET
#define TFM_OTP_NV_COUNTERS_AREA_SIZE    FLASH_AREA_OTP_SIZE
/* NV Counters definitions */
#define TFM_NV_COUNTERS_AREA_ADDR        FLASH_NV_COUNTERS_AREA_OFFSET
#define TFM_NV_COUNTERS_AREA_SIZE        (0x20)/* 32 Bytes */
#define TFM_NV_COUNTERS_SECTOR_ADDR      FLASH_NV_COUNTERS_AREA_OFFSET
#define TFM_NV_COUNTERS_SECTOR_SIZE      FLASH_AREA_IMAGE_SECTOR_SIZE

/* BL2 NV Counters definitions  */
#define BL2_NV_COUNTERS_AREA_ADDR        FLASH_BL2_NVCNT_AREA_OFFSET
#define BL2_NV_COUNTERS_AREA_SIZE        FLASH_BL2_NVCNT_AREA_SIZE

/* FIXME: not valid today */
#define BL2_S_RAM_ALIAS_BASE             (0x30000000)
#define BL2_NS_RAM_ALIAS_BASE            (0x20000000)

/*  This area in SRAM 2 is updated BL2 and can be lock to avoid any changes */
#define BOOT_TFM_SHARED_DATA_SIZE        (0x400)
#define BOOT_TFM_SHARED_DATA_BASE        (0x3003fc00)
#endif /* __FLASH_LAYOUT_H__ */
