/*
 * Copyright (c) 2019-2020, Cypress Semiconductor Corporation. All rights reserved.
 * Copyright (c) 2019-2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
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

#ifndef __SMPU_CONFIG_H__
#define __SMPU_CONFIG_H__

#include "flash_layout.h"
#include "region_defs.h"

#include "cy_prot.h"

/* This macro depends on the actual CY_PROT_REGIONSIZE_XXX values */
#define REGIONSIZE_TO_BYTES(X)      (1UL << (1 + (X)))

/* This triplet of flag values indicates that the actual values
 * must be retrieved from provisioning data at runtime.
 */
#define SMPU_DYNAMIC_BASE       ((void *)-1)
#define SMPU_DYNAMIC_REGIONSIZE ((cy_en_prot_size_t)0)
#define SMPU_DYNAMIC_SUBREGIONS (0)

/* The actual SMPU configs */

/* SMPU configs can only be changed by privileged secure PC=1 bus masters */
#define COMMON_SMPU_MASTER_CONFIG {\
    .userPermission = CY_PROT_PERM_R, \
    .privPermission = CY_PROT_PERM_RW, \
    .secure = false, \
    .pcMatch = false, \
    .pcMask = ONLY_BL2_SPM_MASK, \
}

/* SMPU0 - secure primary image in Flash */
#define SMPU0_BASE          S_ROM_ALIAS(SECURE_IMAGE_OFFSET)
#define SMPU0_REGIONSIZE    PROT_SIZE_512KB_BIT_SHIFT
#define SMPU0_SUBREGION_DIS (CY_PROT_SUBREGION_DIS5 | \
                             CY_PROT_SUBREGION_DIS6 | \
                             CY_PROT_SUBREGION_DIS7)
#define SMPU0_SLAVE_CONFIG {\
    .address = (void *)SMPU0_BASE, \
    .regionSize = (cy_en_prot_size_t) SMPU0_REGIONSIZE, \
    .subregions = SMPU0_SUBREGION_DIS, \
    .userPermission = CY_PROT_PERM_RX, \
    .privPermission = CY_PROT_PERM_RX, \
    .secure = false, \
    .pcMatch = false, \
    .pcMask = SECURE_PCS_MASK, \
}
#define SMPU0_MASTER_CONFIG COMMON_SMPU_MASTER_CONFIG

/* SMPU requires base address aligned to size */
#if SMPU0_BASE % REGIONSIZE_TO_BYTES(SMPU0_REGIONSIZE)
#error "Flash layout has changed - SMPU0 needs updating"
#endif

/*
 * SMPU0 protected area should be exactly the size of the secure primary image
 */
#if FLASH_S_PARTITION_SIZE != (5 * REGIONSIZE_TO_BYTES(SMPU0_REGIONSIZE)/8)
#error "Flash layout has changed - FLASH_S_PARTITION_SIZE isn't 5/8 of SMPU0_REGIONSIZE"
#endif

/* SMPU1 - Internal Trusted Storage in Flash */
/* Dynamically configured from provisioning data */
#define ITS_SMPU_STRUCT     PROT_SMPU_SMPU_STRUCT1
#define SMPU1_SLAVE_CONFIG {\
    .address = SMPU_DYNAMIC_BASE, \
    .regionSize = SMPU_DYNAMIC_REGIONSIZE, \
    .subregions = SMPU_DYNAMIC_SUBREGIONS, \
    .userPermission = CY_PROT_PERM_DISABLED, \
    .privPermission = CY_PROT_PERM_RW, \
    .secure = false, \
    .pcMatch = false, \
    .pcMask = SECURE_PCS_MASK, \
}
#define SMPU1_MASTER_CONFIG COMMON_SMPU_MASTER_CONFIG

/* SMPU2 - NV counters in Flash */
/* Dynamically configured from provisioning data */
#define NVC_SMPU_STRUCT     PROT_SMPU_SMPU_STRUCT2
#define SMPU2_SLAVE_CONFIG {\
    .address = SMPU_DYNAMIC_BASE, \
    .regionSize = SMPU_DYNAMIC_REGIONSIZE, \
    .subregions = SMPU_DYNAMIC_SUBREGIONS, \
    .userPermission = CY_PROT_PERM_DISABLED, \
    .privPermission = CY_PROT_PERM_RW, \
    .secure = false, \
    .pcMatch = false, \
    .pcMask = SECURE_PCS_MASK, \
}
#define SMPU2_MASTER_CONFIG COMMON_SMPU_MASTER_CONFIG

/* SMPU3 - PS in Flash */
/* Dynamically configured from provisioning data */
#define PS_SMPU_STRUCT      PROT_SMPU_SMPU_STRUCT3
#define SMPU3_SLAVE_CONFIG {\
    .address = SMPU_DYNAMIC_BASE, \
    .regionSize = SMPU_DYNAMIC_REGIONSIZE, \
    .subregions = SMPU_DYNAMIC_SUBREGIONS, \
    .userPermission = CY_PROT_PERM_RW, \
    .privPermission = CY_PROT_PERM_RW, \
    .secure = false, \
    .pcMatch = false, \
    .pcMask = SECURE_PCS_MASK, \
}
#define SMPU3_MASTER_CONFIG COMMON_SMPU_MASTER_CONFIG

/* SMPU6 - 32KB of unprivileged secure data in SRAM */
#define SMPU6_BASE         S_DATA_START
#define SMPU6_REGIONSIZE   PROT_SIZE_32KB_BIT_SHIFT
#define SMPU6_SLAVE_CONFIG {\
    .address = (void *)SMPU6_BASE, \
    .regionSize = (cy_en_prot_size_t) SMPU6_REGIONSIZE, \
    .subregions = ALL_ENABLED, \
    .userPermission = CY_PROT_PERM_RW, \
    .privPermission = CY_PROT_PERM_RW, \
    .secure = false, \
    .pcMatch = false, \
    .pcMask = SECURE_PCS_MASK, \
}
#define SMPU6_MASTER_CONFIG COMMON_SMPU_MASTER_CONFIG

/* SMPU requires base address aligned to size */
#if SMPU6_BASE % REGIONSIZE_TO_BYTES(SMPU6_REGIONSIZE)
#error "Flash layout has changed - SMPU6 needs updating"
#endif

/* SMPU6 should exactly cover the unprivileged secure SRAM */
#if REGIONSIZE_TO_BYTES(SMPU6_REGIONSIZE) != S_UNPRIV_DATA_SIZE
#error "SMPU6_REGIONSIZE should match S_UNPRIV_DATA_SIZE"
#endif

/* SMPUs 7 and 8 - 160KB of privileged secure data at S_DATA_PRIV_START in SRAM */
#define SMPU7_BASE          S_RAM_ALIAS(0)
#define SMPU7_REGIONSIZE    PROT_SIZE_128KB_BIT_SHIFT
#define SMPU7_SUBREGION_DIS (CY_PROT_SUBREGION_DIS0 | \
                             CY_PROT_SUBREGION_DIS1)
#define SMPU7_SLAVE_CONFIG {\
    .address = (void *)SMPU7_BASE, \
    .regionSize = (cy_en_prot_size_t) SMPU7_REGIONSIZE, \
    .subregions = SMPU7_SUBREGION_DIS, \
    .userPermission = CY_PROT_PERM_DISABLED, \
    .privPermission = CY_PROT_PERM_RW, \
    .secure = false, \
    .pcMatch = false, \
    .pcMask = SECURE_PCS_MASK, \
}
#define SMPU7_MASTER_CONFIG COMMON_SMPU_MASTER_CONFIG

/* SMPU requires base address aligned to size */
#if SMPU7_BASE % REGIONSIZE_TO_BYTES(SMPU7_REGIONSIZE)
#error "Flash layout has changed - SMPU7 needs updating"
#endif

/*
 * S_DATA_PRIV_START must equal the base address of subregion 2 of
 * SMPU7
 */
#if S_DATA_PRIV_START != (SMPU7_BASE + \
                          (2 * REGIONSIZE_TO_BYTES(SMPU7_REGIONSIZE) / 8))
#error "Flash layout has changed - S_DATA_PRIV_START isn't subregion 2 of SMPU7"
#endif

#define SMPU8_BASE          S_RAM_ALIAS(0x20000)
#define SMPU8_REGIONSIZE    PROT_SIZE_64KB_BIT_SHIFT
#define SMPU8_SLAVE_CONFIG {\
    .address = (void *)SMPU8_BASE, \
    .regionSize = (cy_en_prot_size_t) SMPU8_REGIONSIZE, \
    .subregions = ALL_ENABLED, \
    .userPermission = CY_PROT_PERM_DISABLED, \
    .privPermission = CY_PROT_PERM_RW, \
    .secure = false, \
    .pcMatch = false, \
    .pcMask = SECURE_PCS_MASK, \
}
#define SMPU8_MASTER_CONFIG COMMON_SMPU_MASTER_CONFIG

/* SMPU requires base address aligned to size */
#if SMPU8_BASE % REGIONSIZE_TO_BYTES(SMPU8_REGIONSIZE)
#error "Flash layout has changed - SMPU8 needs updating"
#endif

/*
 * SMPU8 must immediately follow SMPU7
 */
#if SMPU8_BASE != (SMPU7_BASE + REGIONSIZE_TO_BYTES(SMPU7_REGIONSIZE))
#error "Flash layout has changed - SMPU8 doesn't immediately follow SMPU7"
#endif

/* SMPU7 and SMPU8 should exactly cover the privileged secure SRAM which
 * consists of S_PRIV_DATA and S_RAM_CODE
 */
#if ((6*REGIONSIZE_TO_BYTES(SMPU7_REGIONSIZE)/8) + \
     REGIONSIZE_TO_BYTES(SMPU8_REGIONSIZE)) != \
     (S_PRIV_DATA_SIZE + S_RAM_CODE_SIZE)
#error "SMPU7+SMPU8 REGIONSIZE should match privileged secure SRAM size"
#endif

/* SMPU9 - 2KB of privileged executable data in SRAM
 * Note: Region resides in subregion 7 of SMPU 8*/
#define SMPU9_BASE         S_RAM_CODE_START
#define SMPU9_REGIONSIZE   PROT_SIZE_2KB_BIT_SHIFT
#define SMPU9_SLAVE_CONFIG {\
    .address = (void *)SMPU9_BASE, \
    .regionSize = (cy_en_prot_size_t) SMPU9_REGIONSIZE, \
    .subregions = ALL_ENABLED, \
    .userPermission = CY_PROT_PERM_DISABLED, \
    .privPermission = CY_PROT_PERM_RX, \
    .secure = false, \
    .pcMatch = false, \
    .pcMask = SECURE_PCS_MASK, \
}
#define SMPU9_MASTER_CONFIG COMMON_SMPU_MASTER_CONFIG

/* SMPU requires base address aligned to size */
#if SMPU9_BASE % REGIONSIZE_TO_BYTES(SMPU9_REGIONSIZE)
#error "Flash layout has changed - SMPU9 needs updating"
#endif

#if S_RAM_CODE_SIZE != REGIONSIZE_TO_BYTES(SMPU9_REGIONSIZE)
#error "SMPU9_REGIONSIZE is not equal S_RAM_CODE_SIZE"
#endif

/* SMPU9 should be contained within SMPU8 */
#if SMPU9_BASE < SMPU8_BASE
#error "SMPU9 is below SMPU8"
#endif

#if (SMPU9_BASE + REGIONSIZE_TO_BYTES(SMPU9_REGIONSIZE)) > \
    (SMPU8_BASE + REGIONSIZE_TO_BYTES(SMPU8_REGIONSIZE))
#error "SMPU9 is not within SMPU8"
#endif

#endif /* __SMPU_CONFIG_H__ */
