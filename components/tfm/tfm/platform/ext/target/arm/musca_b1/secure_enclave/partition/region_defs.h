/*
 * Copyright (c) 2017-2022 Arm Limited. All rights reserved.
 * Copyright (c) 2021, Cypress Semiconductor Corporation. All rights reserved.
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

#ifndef __REGION_DEFS_H__
#define __REGION_DEFS_H__

#include "flash_layout.h"

#define BL2_HEAP_SIZE           (0x0001000)
#define BL2_MSP_STACK_SIZE      (0x0001800)

#define S_HEAP_SIZE             (0x0000200)
#define S_MSP_STACK_SIZE_INIT   (0x0000400)
#define S_MSP_STACK_SIZE        (0x0000800)

/* This size of buffer is big enough to store an attestation
 * token produced by initial attestation service
 */
#define PSA_INITIAL_ATTEST_TOKEN_MAX_SIZE   (0x250)

#ifndef LINK_TO_SECONDARY_PARTITION
#define S_IMAGE_PRIMARY_PARTITION_OFFSET   (FLASH_AREA_0_OFFSET)
#define S_IMAGE_SECONDARY_PARTITION_OFFSET (FLASH_AREA_2_OFFSET)
#else
#define S_IMAGE_PRIMARY_PARTITION_OFFSET   (FLASH_AREA_2_OFFSET)
#define S_IMAGE_SECONDARY_PARTITION_OFFSET (FLASH_AREA_0_OFFSET)
#endif /* !LINK_TO_SECONDARY_PARTITION */

#define IMAGE_S_CODE_SIZE \
            (FLASH_S_PARTITION_SIZE - BL2_HEADER_SIZE - BL2_TRAILER_SIZE)

/* Code SRAM area */
#define CODE_SRAM_BASE              (0x38200000)
#define CODE_SRAM_SIZE              (0x00080000) /* 512 KiB */

/* Reserved area for SSE-200 BL0 */
#define SSE200_BL0_RAM_BASE         CODE_SRAM_BASE
#define SSE200_BL0_RAM_SIZE         (0x00004000) /* 16 KiB */

/* eFlash drivers are copied to Code SRAM in BL2 */
#define EFLASH_DRIVER_REGION_BASE   (SSE200_BL0_RAM_BASE + SSE200_BL0_RAM_SIZE)
#define EFLASH_DRIVER_REGION_SIZE   (0x00004000) /* 16 KiB */

/* Memory area used as shared memory between SSE-200 and SE */
#define IPC_SHARED_MEMORY_BASE      (EFLASH_DRIVER_REGION_BASE + \
                                     EFLASH_DRIVER_REGION_SIZE)
#define IPC_SHARED_MEMORY_SIZE      (CODE_SRAM_SIZE - \
                                     SSE200_BL0_SIZE - \
                                     EFLASH_DRIVER_REGION_SIZE) /* 476 KiB */

/* Secure Enclave internal SRAM */
#define SRAM_BASE               (0x30000000)
#define SRAM_SIZE               (0x00010000)     /* 64 KiB */

/* Secure regions */
#define S_CODE_START            (FLASH_BASE_ADDRESS + \
                                 S_IMAGE_PRIMARY_PARTITION_OFFSET + \
                                 BL2_HEADER_SIZE)
#define S_CODE_SIZE             (IMAGE_S_CODE_SIZE)
#define S_CODE_LIMIT            (S_CODE_START + S_CODE_SIZE - 1)

#define S_DATA_START            (SRAM_BASE)
#define S_DATA_SIZE             (SRAM_SIZE)
#define S_UNPRIV_DATA_SIZE      (0x2800)
#define S_DATA_LIMIT            (S_DATA_START + S_DATA_SIZE - 1)
#define S_DATA_PRIV_START       (S_DATA_START + S_UNPRIV_DATA_SIZE)

/* Size of vector table: 47 interrupt handlers + 4 bytes MPS initial value */
#define S_CODE_VECTOR_TABLE_SIZE    (0xC0)

/* Shared data area between bootloader and runtime firmware.
 * Shared data area is allocated at the beginning of the privileged data area,
 * it is overlapping with TF-M Secure code's MSP stack
 */
#define BOOT_TFM_SHARED_DATA_BASE (S_DATA_PRIV_START)
#define BOOT_TFM_SHARED_DATA_SIZE (0x400)
#define BOOT_TFM_SHARED_DATA_LIMIT (BOOT_TFM_SHARED_DATA_BASE + \
                                    BOOT_TFM_SHARED_DATA_SIZE - 1)

/* Whole SSE-200 image treated as Non-secure */
#ifndef LINK_TO_SECONDARY_PARTITION
#define NS_IMAGE_PRIMARY_PARTITION_OFFSET (FLASH_AREA_1_OFFSET)
#else
#define NS_IMAGE_PRIMARY_PARTITION_OFFSET (FLASH_AREA_3_OFFSET)
#endif /* !LINK_TO_SECONDARY_PARTITION */

#define NS_PARTITION_START (FLASH_BASE_ADDRESS + \
                            NS_IMAGE_PRIMARY_PARTITION_OFFSET)
#define NS_PARTITION_SIZE (FLASH_SSE_200_PARTITION_SIZE)

/* The shared memory is treated as NS memory */
#define NS_DATA_START   IPC_SHARED_MEMORY_BASE
#define NS_DATA_SIZE    IPC_SHARED_MEMORY_SIZE
#define NS_DATA_LIMIT   (NS_DATA_START + NS_DATA_SIZE - 1)

#define NS_CODE_START   (NS_PARTITION_START)
#define NS_CODE_SIZE    (NS_PARTITION_SIZE)
#define NS_CODE_LIMIT   (NS_CODE_START + NS_CODE_SIZE - 1)

/* Secondary partition for new images in case of firmware upgrade.
 * This area is reserved for both secondary images. */
#define SECONDARY_PARTITION_START (S_IMAGE_SECONDARY_PARTITION_OFFSET)
#define SECONDARY_PARTITION_SIZE  (FLASH_S_PARTITION_SIZE + \
                                   FLASH_SSE_200_PARTITION_SIZE)

/* Bootloader regions */
/* ROM remap is mapped to 0x0 in SE */
#define BL2_CODE_START    (0x0)
#define BL2_CODE_SIZE     (FLASH_AREA_BL2_SIZE)
#define BL2_CODE_LIMIT    (BL2_CODE_START + BL2_CODE_SIZE - 1)

#define BL2_DATA_START    (S_DATA_START)
#define BL2_DATA_SIZE     (S_DATA_SIZE)
#define BL2_DATA_LIMIT    (BL2_DATA_START + BL2_DATA_SIZE - 1)

/* NSPE-to-SPE interrupt */
#define MAILBOX_IRQ MHU0_MSG_0_IRQn

#endif /* __REGION_DEFS_H__ */

