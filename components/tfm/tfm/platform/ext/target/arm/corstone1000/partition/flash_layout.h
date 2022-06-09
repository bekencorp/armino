/*
 * Copyright (c) 2017-2021 Arm Limited. All rights reserved.
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
 * here the platform_base_address.h to access flash related defines. To resolve
 * this some of the values are redefined here with different names, these are
 * marked with comment.
 */

#define MCUBOOT_MAX_IMG_SECTORS         (32)

/********************************/
/* Secure Enclave internal SRAM */
/********************************/
#define SRAM_BASE                       (0x30000000)
#define SRAM_SIZE                       (0x80000)     /* 512 KB */

#define BL2_DATA_GAP_SIZE               (0x09000)     /* 36 KB */

#define BL1_DATA_START                  (SRAM_BASE)
#define BL1_DATA_SIZE                   (0x10000)     /* 64 KiB*/
#define BL1_DATA_LIMIT                  (BL1_DATA_START + BL1_DATA_SIZE - 1)

#ifdef BL1

#define IMAGE_EXECUTABLE_RAM_START      (SRAM_BASE + BL1_DATA_SIZE)
#define IMAGE_EXECUTABLE_RAM_SIZE       (SRAM_SIZE - BL1_DATA_SIZE)

#endif /* BL1 */

/*****************/
/***** Flash *****/
/*****************/

#define FLASH_BASE_ADDRESS              (0x68000000)

#if PLATFORM_IS_FVP
/* INTEL STRATA J3 NOR FLASH NVM */
#define STRATA_NVM_FLASH_TOTAL_SIZE     (0x04000000)  /* 32 MB Nor Flash (PMOD SF3) */
#define STRATA_NVM_FLASH_SECTOR_SIZE    (0x00001000)  /* 64 KB Sub sector size*/
#define STRATA_NVM_FLASH_PAGE_SIZE      (256U)        /* 64 KB */
#define STRATA_NVM_FLASH_PROGRAM_UNIT   (1U)          /* 4 B */

#define FLASH_DEV_NAME                  Driver_FLASH0
#define FLASH_TOTAL_SIZE                (STRATA_NVM_FLASH_TOTAL_SIZE)  /* 32 MB */
#define FLASH_AREA_IMAGE_SECTOR_SIZE    (STRATA_NVM_FLASH_SECTOR_SIZE)      /* 4 KiB */
#define FLASH_SECTOR_SIZE               (STRATA_NVM_FLASH_SECTOR_SIZE) /* 1 kB */
#define TFM_HAL_FLASH_PROGRAM_UNIT      (STRATA_NVM_FLASH_PROGRAM_UNIT)

#define STRATA_SE_FLASH_TOTAL_SIZE      (0x00800000)  /* 32 MB Nor Flash (PMOD SF3) */
#define STRATA_SE_FLASH_SECTOR_SIZE     (0x00001000)  /* 64 KB Sub sector size*/
#define STRATA_SE_FLASH_PAGE_SIZE       (256U)        /* 64 KB */
#define STRATA_SE_FLASH_PROGRAM_UNIT    (1U)          /* 4 B */

#define FLASH_DEV_NAME_SE_SECURE_FLASH  Driver_FLASH1
#define SECURE_FLASH_SECTOR_SIZE        STRATA_NVM_FLASH_SECTOR_SIZE

#else

/* PMOD SF3 NOR FLASH */
#define PMOD_SF3_FLASH_TOTAL_SIZE       (0x02000000)  /* 32 MB Nor Flash (PMOD SF3) */
#define PMOD_SF3_FLASH_SECTOR_SIZE      (0x00001000)  /* 4 KB Sub sector size*/
#define PMOD_SF3_FLASH_PAGE_SIZE        (256U)        /* 256 B */
#define PMOD_SF3_FLASH_PROGRAM_UNIT     (1U)          /* 1 B */

/* SST26VF064B NOR FLASH */
#define SST26VF064B_FLASH_TOTAL_SIZE    (0x00800000)  /* 8 MB Nor Flash (SST26VF064B) */
#define SST26VF064B_FLASH_SECTOR_SIZE   (0x00010000)  /* 64 KB Sub sector size*/
#define SST26VF064B_FLASH_PAGE_SIZE     (256U)        /* 256 B */
#define SST26VF064B_FLASH_PROGRAM_UNIT  (1U)          /* 1 B */

#define FLASH_DEV_NAME                  Driver_FLASH0

#define FLASH_TOTAL_SIZE                (PMOD_SF3_FLASH_TOTAL_SIZE)  /* 32 MB */
#define FLASH_AREA_IMAGE_SECTOR_SIZE    (PMOD_SF3_FLASH_SECTOR_SIZE) /* 4 KiB */
#define FLASH_SECTOR_SIZE               (PMOD_SF3_FLASH_SECTOR_SIZE) /* 1 kB */
#define TFM_HAL_FLASH_PROGRAM_UNIT      (PMOD_SF3_FLASH_PROGRAM_UNIT)

#define FLASH_DEV_NAME_SE_SECURE_FLASH  Driver_FLASH1
#define SECURE_FLASH_SECTOR_SIZE        SST26VF064B_FLASH_SECTOR_SIZE

#endif

/* Flash layout (32MB) :-
 *
 * 1 MB     : FWU_METADATA_PARTITION_SIZE
 * 15.5 MB  : BANK 1 PARTITION SIZE
 * 15.5 MB  : BANK 2 PARTITION SIZE
 *
 */
#define FWU_METADATA_PARTITION_SIZE     (0x100000)   /* 1MB */
#define BANK_PARTITION_SIZE             (0xF80000)   /* 15.5 MB */

#define FLASH_BASE_OFFSET               (0x0)

/* BANK layout (15MB: BANK_PARTITION_SIZE) :-
 *
 * 200 KB    : SE_BL2_PARTITION_SIZE + SE_BL2_PARTITION_SIZE
 * 752 KB    : TFM_PARTITION_SIZE + TFM_PARTITION_SIZE
 * 2 MB      : FIP_PARTITION_SIZE
 * 12+ MB    : KERNEL_PARTITION_SIZE
 *
 */
#define SE_BL2_PARTITION_SIZE           (0x19000)    /* 100 KB */
#ifdef TEST_S
#define TFM_PARTITION_SIZE              (0x61C00)    /* 391 KB */
#else
#define TFM_PARTITION_SIZE              (0x5E000)    /* 376 KB */
#endif
#define FIP_PARTITION_SIZE              (0x200000)   /* 2 MB */
#define KERNEL_PARTITION_SIZE           (0xC00000)   /* 12 MB */




/* 1MB: space in flash to store metadata and uefi variables */
#define FWU_METADATA_FLASH_DEV          (FLASH_DEV_NAME)
#define FWU_METADATA_FLASH_SECTOR_SIZE  (FLASH_SECTOR_SIZE)

#define FWU_METADATA_PARTITION_OFFSET   (FLASH_BASE_OFFSET)
#define FWU_METADATA_AREA_SIZE          (FWU_METADATA_FLASH_SECTOR_SIZE)
#define FWU_METADATA_REPLICA_1_OFFSET   (FLASH_BASE_OFFSET)
#define FWU_METADATA_REPLICA_2_OFFSET   (FWU_METADATA_REPLICA_1_OFFSET + \
                                         FWU_METADATA_AREA_SIZE)
#define FWU_PRIVATE_AREA_SIZE           (FLASH_SECTOR_SIZE)
#define FWU_PRIVATE_AREA_OFFSET         (FWU_METADATA_REPLICA_2_OFFSET + \
                                         FWU_METADATA_AREA_SIZE)

#define NR_OF_FW_BANKS                  (2)
#define NR_OF_IMAGES_IN_FW_BANK         (4) /* Secure Enclave: BL2 and TF-M \
                                             * Host: FIP and Kernel image
                                             */

#define BANK_0_PARTITION_OFFSET         (FWU_METADATA_PARTITION_OFFSET + \
                                         FWU_METADATA_PARTITION_SIZE)
#define BANK_1_PARTITION_OFFSET         (BANK_0_PARTITION_OFFSET + \
                                         BANK_PARTITION_SIZE)

/************************************************************/
/* Bank : Images flash offsets are with respect to the bank */
/************************************************************/

/* Image 0: BL2 primary and secondary images */
#define FLASH_AREA_8_ID                 (1)
#define FLASH_AREA_8_OFFSET             (0) /* starting from 0th offset of the bank */
#define FLASH_AREA_8_SIZE               (SE_BL2_PARTITION_SIZE)

#define FLASH_AREA_9_ID                 (FLASH_AREA_8_ID + 1)
#define FLASH_AREA_9_OFFSET             (FLASH_AREA_8_OFFSET + FLASH_AREA_8_SIZE)
#define FLASH_AREA_9_SIZE               (SE_BL2_PARTITION_SIZE)

#define FWU_METADATA_IMAGE_0_OFFSET     (FLASH_AREA_8_OFFSET)
#define FWU_METADATA_IMAGE_0_SIZE_LIMIT (FLASH_AREA_8_SIZE + FLASH_AREA_9_SIZE)

/* Macros needed to imgtool.py, used when creating BL2 signed image */
#define BL2_IMAGE_LOAD_ADDRESS          (SRAM_BASE + TFM_PARTITION_SIZE + BL2_DATA_GAP_SIZE)
#define BL2_IMAGE_OFFSET                (0x0)
#define BL2_IMAGE_MAX_SIZE              (SE_BL2_PARTITION_SIZE)

#define BL1_FLASH_AREA_IMAGE_PRIMARY(x)     (((x) == 0) ? FLASH_AREA_8_ID : \
                                                          255 )
#define BL1_FLASH_AREA_IMAGE_SECONDARY(x)   (((x) == 0) ? FLASH_AREA_9_ID : \
                                                          255 )

#define BL1_FLASH_AREA_IMAGE_SCRATCH        255

/* Image 1: TF-M primary and secondary images */
#define FLASH_AREA_0_ID                 (1)
#define FLASH_AREA_0_OFFSET             (FLASH_AREA_9_OFFSET + \
                                         FLASH_AREA_9_SIZE)
#define FLASH_AREA_0_SIZE               (TFM_PARTITION_SIZE)

#define FLASH_AREA_1_ID                 (FLASH_AREA_0_ID + 1)
#define FLASH_AREA_1_OFFSET             (FLASH_AREA_0_OFFSET + FLASH_AREA_0_SIZE)
#define FLASH_AREA_1_SIZE               (TFM_PARTITION_SIZE)

#define FWU_METADATA_IMAGE_1_OFFSET     (FLASH_AREA_0_OFFSET)
#define FWU_METADATA_IMAGE_1_SIZE_LIMIT (FLASH_AREA_0_SIZE + FLASH_AREA_1_SIZE)

/* Image 2: Host FIP */
#define FIP_SIGNATURE_AREA_SIZE         (0x1000)      /* 4 KB */

#define FLASH_FIP_OFFSET                (FLASH_AREA_1_OFFSET + \
                                         FLASH_AREA_1_SIZE + FIP_SIGNATURE_AREA_SIZE)
#define FLASH_FIP_ADDRESS               (FLASH_BASE_ADDRESS + FLASH_FIP_OFFSET)
#define FLASH_FIP_SIZE                  (FIP_PARTITION_SIZE)

/* Host BL2 (TF-A) primary and secondary image. */
#define FLASH_AREA_2_ID                 (FLASH_AREA_1_ID + 1)
#define FLASH_AREA_3_ID                 (FLASH_AREA_2_ID + 1)
#define FLASH_INVALID_OFFSET            (0xFFFFFFFF)
#define FLASH_INVALID_SIZE              (0xFFFFFFFF)

#define FWU_METADATA_IMAGE_2_OFFSET     (FLASH_FIP_OFFSET)
#define FWU_METADATA_IMAGE_2_SIZE_LIMIT (FLASH_FIP_SIZE)

/* Macros needed to imgtool.py, used when creating TF-M signed image */
#define IMAGE_LOAD_ADDRESS              (SRAM_BASE)
#define SECURE_IMAGE_OFFSET             (0x0)
#define SECURE_IMAGE_MAX_SIZE           (TFM_PARTITION_SIZE)
#define NON_SECURE_IMAGE_OFFSET         (TFM_PARTITION_SIZE)
#define NON_SECURE_IMAGE_MAX_SIZE       (0x0)

#define FLASH_AREA_IMAGE_PRIMARY(x)     (((x) == 0) ? FLASH_AREA_0_ID : \
                                         ((x) == 1) ? FLASH_AREA_2_ID : \
                                                      255 )
#define FLASH_AREA_IMAGE_SECONDARY(x)   (((x) == 0) ? FLASH_AREA_1_ID : \
                                         ((x) == 1) ? FLASH_AREA_3_ID : \
                                                      255 )

#define FLASH_AREA_IMAGE_SCRATCH        255

/* Image 3: Kernel image */
#define KERNEL_PARTITION_OFFSET         (FLASH_FIP_OFFSET + FLASH_FIP_SIZE)

#define FWU_METADATA_IMAGE_3_OFFSET     (KERNEL_PARTITION_OFFSET)
#define FWU_METADATA_IMAGE_3_SIZE_LIMIT (KERNEL_PARTITION_SIZE)


/*******************************/
/*** ITS, PS and NV Counters ***/
/*******************************/

#define FLASH_ITS_AREA_OFFSET           (0x10000)  /* 64 KB */
#define FLASH_ITS_AREA_SIZE             (4 * SECURE_FLASH_SECTOR_SIZE)

#define FLASH_PS_AREA_OFFSET            (FLASH_ITS_AREA_OFFSET + \
                                         FLASH_ITS_AREA_SIZE)
#define FLASH_PS_AREA_SIZE              (16 * SECURE_FLASH_SECTOR_SIZE)

/* OTP_definitions */
#define FLASH_OTP_NV_COUNTERS_AREA_OFFSET (FLASH_PS_AREA_OFFSET + \
                                           FLASH_PS_AREA_SIZE)
#define FLASH_OTP_NV_COUNTERS_AREA_SIZE   (SECURE_FLASH_SECTOR_SIZE * 2)
#define FLASH_OTP_NV_COUNTERS_SECTOR_SIZE SECURE_FLASH_SECTOR_SIZE

/* Internal Trusted Storage (ITS) Service definitions
 * Note: Further documentation of these definitions can be found in the
 * TF-M ITS Integration Guide.
 */
#define TFM_HAL_ITS_FLASH_DRIVER FLASH_DEV_NAME_SE_SECURE_FLASH

/* Protected Storage (PS) Service definitions
 * Note: Further documentation of these definitions can be found in the
 * TF-M PS Integration Guide.
 */
#define TFM_HAL_PS_FLASH_DRIVER FLASH_DEV_NAME_SE_SECURE_FLASH

/* In this target the CMSIS driver requires only the offset from the base
 * address instead of the full memory address.
 */
/* Base address of dedicated flash area for ITS */
#define TFM_HAL_ITS_FLASH_AREA_ADDR     FLASH_ITS_AREA_OFFSET
/* Size of dedicated flash area for ITS */
#define TFM_HAL_ITS_FLASH_AREA_SIZE     FLASH_ITS_AREA_SIZE
/* Number of physical erase sectors per logical FS block */
#define TFM_HAL_ITS_SECTORS_PER_BLOCK   (1)
/* Smallest flash programmable unit in bytes */
#define TFM_HAL_ITS_PROGRAM_UNIT        (1)

/* In this target the CMSIS driver requires only the offset from the base
 * address instead of the full memory address.
 */
/* Base address of dedicated flash area for PS */
#define TFM_HAL_PS_FLASH_AREA_ADDR      FLASH_PS_AREA_OFFSET
/* Size of dedicated flash area for PS */
#define TFM_HAL_PS_FLASH_AREA_SIZE      FLASH_PS_AREA_SIZE
/* Number of physical erase sectors per logical FS block */
#define TFM_HAL_PS_SECTORS_PER_BLOCK    (4)
/* Smallest flash programmable unit in bytes */
#define TFM_HAL_PS_PROGRAM_UNIT         (1)

#define OTP_NV_COUNTERS_FLASH_DEV FLASH_DEV_NAME_SE_SECURE_FLASH

/* OTP / NV counter definitions */
#define TFM_OTP_NV_COUNTERS_AREA_SIZE   (FLASH_OTP_NV_COUNTERS_AREA_SIZE / 2)
#define TFM_OTP_NV_COUNTERS_AREA_ADDR   FLASH_OTP_NV_COUNTERS_AREA_OFFSET
#define TFM_OTP_NV_COUNTERS_SECTOR_SIZE FLASH_OTP_NV_COUNTERS_SECTOR_SIZE
#define TFM_OTP_NV_COUNTERS_BACKUP_AREA_ADDR (TFM_OTP_NV_COUNTERS_AREA_ADDR + \
                                              TFM_OTP_NV_COUNTERS_AREA_SIZE)
#endif /* __FLASH_LAYOUT_H__ */
