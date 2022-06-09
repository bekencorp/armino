/*
 * Copyright (c) 2019-2020, Cypress Semiconductor Corporation. All rights reserved.
 * Copyright (c) 2021, Arm Limited. All rights reserved.
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
#include "driver_smpu.h"
#include "nv_counters.h"
#include "pc_config.h"
#include "region_defs.h"
#include "RTE_Device.h"
#include "smpu_config.h"
#include <string.h>
#include "tfm_spm_log.h"
#include "tfm_hal_its.h"
#include "tfm_hal_ps.h"
#include "tfm_memory_utils.h"
#include "flash_layout.h"

#include "cy_prot.h"

/* Affect all 8 subregions */
#define ALL_ENABLED 0
#define SMPU_NAME_MAX_SIZE 18

struct smpu_resources {
    PROT_SMPU_SMPU_STRUCT_Type *smpu;
    cy_stc_smpu_cfg_t slave_config;
    cy_stc_smpu_cfg_t master_config;
};

static const char * smpu_name(const SMPU_Resources *smpu_dev)
{
    switch ((int)smpu_dev->smpu) {
    case (int)PROT_SMPU_SMPU_STRUCT0:
        return "SMPU 0";
    case (int)PROT_SMPU_SMPU_STRUCT1:
        return "SMPU 1";
    case (int)PROT_SMPU_SMPU_STRUCT2:
        return "SMPU 2";
    case (int)PROT_SMPU_SMPU_STRUCT3:
        return "SMPU 3";
    case (int)PROT_SMPU_SMPU_STRUCT4:
        return "SMPU 4";
    case (int)PROT_SMPU_SMPU_STRUCT5:
        return "SMPU 5";
    case (int)PROT_SMPU_SMPU_STRUCT6:
        return "SMPU 6";
    case (int)PROT_SMPU_SMPU_STRUCT7:
        return "SMPU 7";
    case (int)PROT_SMPU_SMPU_STRUCT8:
        return "SMPU 8";
    case (int)PROT_SMPU_SMPU_STRUCT9:
        return "SMPU 9";
    case (int)PROT_SMPU_SMPU_STRUCT10:
        return "SMPU 10";
    case (int)PROT_SMPU_SMPU_STRUCT11:
        return "SMPU 11";
    case (int)PROT_SMPU_SMPU_STRUCT12:
        return "SMPU 12";
    case (int)PROT_SMPU_SMPU_STRUCT13:
        return "SMPU 13";
    case (int)PROT_SMPU_SMPU_STRUCT14:
        return "SMPU 14";
    case (int)PROT_SMPU_SMPU_STRUCT15:
        return "SMPU 15";
    default:
        return "Unrecognised SMPU";
    }
}

static bool is_runtime(const SMPU_Resources *smpu_dev)
{
    if ((smpu_dev->slave_config.address == SMPU_DYNAMIC_BASE) &&
        (smpu_dev->slave_config.regionSize == SMPU_DYNAMIC_REGIONSIZE) &&
        (smpu_dev->slave_config.subregions == SMPU_DYNAMIC_SUBREGIONS))
        return true;
    return false;
}

static bool is_whole_power_of_two(size_t size)
{
    return ((size - 1) & size) == 0;
}

static bool is_aligned(uint32_t base, size_t size)
{
    return (base % size) == 0;
}

/* size must be a whole power of two, >= 4 */
static cy_en_prot_size_t bytes_to_regionsize(size_t size)
{
    int ret = 1;

    while (REGIONSIZE_TO_BYTES(ret) < size)
        ret += 1;
    return (cy_en_prot_size_t)ret;
}

static uint32_t round_up_to_power_of_two(uint32_t x)
{
    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x++;
    return x;
}

static uint32_t round_down_to_power_of_two(uint32_t x)
{
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x = x - (x >> 1);
    return x;
}

static uint32_t round_down_to_multiple(uint32_t base, uint32_t regionSize)
{
    return (regionSize * (base / regionSize));
}

/* Maps 0..7 to CY_PROT_SUBREGION_DIS0..CY_PROT_SUBREGION_DIS7 */
static uint32_t subregion_num_to_mask(int num) {
    return CY_PROT_SUBREGION_DIS0 << num;
}

#define MIN_REGIONSIZE 256
/* The largest regionSize is actually 4GB, but that doesn't fit in a uint32_t */
#define MAX_REGIONSIZE (1 * 1024 * 1024 * 1024)
#define NUM_SUBREGIONS 8

static cy_en_prot_status_t calc_smpu_params(uint32_t base,
                                            size_t size,
                                            cy_stc_smpu_cfg_t *slave_config)
{
    /* Simplest case - base is a multiple of the size,
     * and size is a whole power of two, and >= 256
     */
    if (is_whole_power_of_two(size) &&
        is_aligned(base, size) &&
        (size >= MIN_REGIONSIZE)) {
        slave_config->address = (void *)base;
        slave_config->regionSize = bytes_to_regionsize(size);
        slave_config->subregions = ALL_ENABLED;

        return CY_PROT_SUCCESS;
    } else {
        /* Try to find a regionSize that could work */
        uint32_t low_rs = round_up_to_power_of_two(size);
        uint32_t high_rs = round_down_to_power_of_two(size * NUM_SUBREGIONS);
        uint32_t regionSize;

        if (low_rs < MIN_REGIONSIZE)
            low_rs = MIN_REGIONSIZE;

        if (size > MAX_REGIONSIZE / NUM_SUBREGIONS)
            high_rs = MAX_REGIONSIZE;

        for (regionSize = low_rs; regionSize <= high_rs; regionSize <<= 1)
        {
            const uint32_t sub_size = regionSize / NUM_SUBREGIONS;
            uint32_t address;
            int n;
            uint32_t mask = 0;

            /* Would this work with the base address ? */
            if (base % sub_size) {
                /* TODO If we get here, are we guaranteed to fail ? */
                continue;
            }

            /* is the size 1..8 * subregion size ? */
            for (n = 1; n <= NUM_SUBREGIONS; n++) {
                if (size == n * sub_size) {
                    break;
                }
            }
            if (n > NUM_SUBREGIONS) {
                continue;
            }

            /* What would the base address be? */
            address = round_down_to_multiple(base, regionSize);

            /* Would the upper limit of the SMPU region cover what we need? */
            if (address + regionSize < base + size) {
                continue;
            }

            /* Calculate the SMPU config */
            slave_config->address = (void *)address;
            slave_config->regionSize = bytes_to_regionsize(regionSize);

            /* Figure out which subregions to disable */
            for (int num = 0; num < NUM_SUBREGIONS; num += 1) {
                if (address + num * sub_size < base) {
                    /* Disable this subregion */
                    mask |= subregion_num_to_mask(num);
                } else if (address + num * sub_size >= base + size) {
                    /* Disable this subregion */
                    mask |= subregion_num_to_mask(num);
                }
            }
            slave_config->subregions = mask;

            return CY_PROT_SUCCESS;
        }

        /* If we get here, we failed */
        return CY_PROT_FAILURE;
    }
}

static cy_en_prot_status_t get_region(const PROT_SMPU_SMPU_STRUCT_Type *smpu,
                                      uint32_t *base, size_t *size)
{
    cy_en_prot_status_t ret = CY_PROT_SUCCESS;

    /* Figure out the base, size, and subregion mask to use */
    if (smpu == ITS_SMPU_STRUCT) {
        struct tfm_hal_its_fs_info_t its_fs_info;
        /* Retrieve the ITS region definition */
        tfm_hal_its_fs_info(&its_fs_info);
        *base = its_fs_info.flash_area_addr;
        *size = its_fs_info.flash_area_size;
    } else if (smpu == NVC_SMPU_STRUCT) {
        /* Retrieve the OTP / NV area info */
        *base = FLASH_OTP_NV_COUNTERS_AREA_OFFSET;
        *size = FLASH_OTP_NV_COUNTERS_AREA_SIZE;
    } else if (smpu == PS_SMPU_STRUCT) {
        struct tfm_hal_ps_fs_info_t ps_fs_info;
        /* Retrieve the PS region definition */
        tfm_hal_ps_fs_info(&ps_fs_info);
        *base = ps_fs_info.flash_area_addr;
        *size = ps_fs_info.flash_area_size;
    } else {
        /* We don't know where to get the region definition */
        ret = CY_PROT_FAILURE;
    }
    /* flash driver uses offsets rather than absolute addresses,
     * so we need to add the flash base address here.
     */
    *base += FLASH_BASE_ADDRESS;

    return ret;
}

static cy_en_prot_status_t populate_region(const PROT_SMPU_SMPU_STRUCT_Type *smpu,
                                           cy_stc_smpu_cfg_t *slave_config)
{
    cy_en_prot_status_t ret;
    uint32_t base;
    size_t size;

    ret = get_region(smpu, &base, &size);

    if (ret == CY_PROT_SUCCESS) {
        /* And figure out how to configure the SMPU region */
        ret = calc_smpu_params(base, size, slave_config);
    }

    return ret;
}

static void print_smpu_config(const cy_stc_smpu_cfg_t *slave_config)
{
    SPMLOG_INFMSGVAL(" Address = ", (uintptr_t)slave_config->address);
    SPMLOG_INFMSGVAL(" Size (bytes) = ",
                     REGIONSIZE_TO_BYTES(slave_config->regionSize));
    if (slave_config->subregions == ALL_ENABLED) {
        SPMLOG_INFMSG(" All subregions enabled\r\n");
    } else {
        SPMLOG_INFMSGVAL("\tsubregion size (bytes) = ",
                         REGIONSIZE_TO_BYTES(slave_config->regionSize)/8);
        for (int i=0; i<8; i++) {
            if (slave_config->subregions & (1<<i)) {
                SPMLOG_INFMSGVAL("\tDisabled subregion ", i);
            } else {
                SPMLOG_INFMSGVAL("\tEnabled subregion ", i);
            }
        }
    }
}

static void dump_smpu(const PROT_SMPU_SMPU_STRUCT_Type *smpu)
{
    uint32_t base;
    size_t size;
    uint32_t reg = smpu->ATT0;

    if (CY_PROT_SUCCESS == get_region(smpu, &base, &size)) {
        SPMLOG_INFMSGVAL(" Wanted address = ", base);
        SPMLOG_INFMSGVAL(" Wanted size (bytes) = ", size);
    } else {
        SPMLOG_ERRMSG(" Unsupported dynamic SMPU region\r\n");
    }

    if (_FLD2BOOL(PROT_SMPU_SMPU_STRUCT_ATT0_ENABLED, reg)) {
        uint32_t size = _FLD2VAL(PROT_SMPU_SMPU_STRUCT_ATT0_REGION_SIZE, reg);
        uint32_t subregions;

        reg = smpu->ADDR0;
        subregions = _FLD2VAL(PROT_SMPU_SMPU_STRUCT_ADDR0_SUBREGION_DISABLE, reg);
        SPMLOG_INFMSGVAL(" Configured address = ",
                _FLD2VAL(PROT_SMPU_SMPU_STRUCT_ADDR0_ADDR24, reg) << 8);
        SPMLOG_INFMSGVAL(" Configured size (bytes) = ", REGIONSIZE_TO_BYTES(size));
        if (subregions == ALL_ENABLED) {
            SPMLOG_INFMSG(" All subregions enabled\r\n");
        } else {
            SPMLOG_INFMSGVAL("\tsubregion size (bytes) = ",
                REGIONSIZE_TO_BYTES(size)/8);
            for (int i=0; i<8; i++) {
                if (subregions & (1<<i)) {
                    SPMLOG_INFMSGVAL("\tDisabled subregion ", i);
                } else {
                    SPMLOG_INFMSGVAL("\tEnabled subregion ", i);
                }
            }
        }
    } else {
        SPMLOG_ERRMSG("SMPU slave is disabled\r\n");
    }
}

/* API functions */

void SMPU_Print_Config(const SMPU_Resources *smpu_dev)
{
    char smpu_str[SMPU_NAME_MAX_SIZE] = {0};

    strcpy(smpu_str, smpu_name(smpu_dev));
    SPMLOG_INFMSG(smpu_str);
    if (is_runtime(smpu_dev)) {
        SPMLOG_INFMSG(" - configured algorithmically.\r\n");

        dump_smpu(smpu_dev->smpu);
    } else {
        SPMLOG_INFMSG(" - configured at compile time.\r\n");

        print_smpu_config(&smpu_dev->slave_config);
    }
}

cy_en_prot_status_t SMPU_Configure(const SMPU_Resources *smpu_dev)
{
    cy_en_prot_status_t ret;

    if (is_runtime(smpu_dev)) {
        cy_stc_smpu_cfg_t slave_config;

        /* Start with a verbatim copy of the slave config */
        tfm_memcpy(&slave_config,
                   &smpu_dev->slave_config,
                   sizeof(slave_config));

        ret = populate_region(smpu_dev->smpu, &slave_config);
        if (ret != CY_PROT_SUCCESS) {
            return ret;
        }

        ret = Cy_Prot_ConfigSmpuSlaveStruct(smpu_dev->smpu,
                                            &slave_config);
    } else {
        /* Use the slave config verbatim */
        ret = Cy_Prot_ConfigSmpuSlaveStruct(smpu_dev->smpu,
                                            &smpu_dev->slave_config);
    }

    if (ret != CY_PROT_SUCCESS) {
        return ret;
    }
    ret = Cy_Prot_ConfigSmpuMasterStruct(smpu_dev->smpu,
                                         &smpu_dev->master_config);
    if (ret != CY_PROT_SUCCESS) {
        return ret;
    }
    ret = Cy_Prot_EnableSmpuSlaveStruct(smpu_dev->smpu);
    if (ret != CY_PROT_SUCCESS) {
        return ret;
    }
    ret = Cy_Prot_EnableSmpuMasterStruct(smpu_dev->smpu);
    return ret;
}

/* Only allow privileged secure PC=1 bus masters to change unconfigured SMPUs */
cy_en_prot_status_t protect_unconfigured_smpus(void)
{
    const cy_stc_smpu_cfg_t smpu_config = COMMON_SMPU_MASTER_CONFIG;
    cy_en_prot_status_t ret = CY_PROT_SUCCESS;
    int i;
    uint32_t att0, att1;

    for (i = 0; i < CPUSS_PROT_SMPU_STRUCT_NR; i++) {
        att0 = PROT->SMPU.SMPU_STRUCT[i].ATT0;
        att1 = PROT->SMPU.SMPU_STRUCT[i].ATT1;

        if ((_FLD2VAL(PROT_SMPU_SMPU_STRUCT_ATT0_ENABLED, att0) == 0)
            && (_FLD2VAL(PROT_SMPU_SMPU_STRUCT_ATT1_ENABLED, att1) == 0)) {

            ret = Cy_Prot_ConfigSmpuMasterStruct(&PROT->SMPU.SMPU_STRUCT[i],
                                                 &smpu_config);
            if (ret != CY_PROT_SUCCESS) {
                break;
            }
            ret = Cy_Prot_EnableSmpuMasterStruct(&PROT->SMPU.SMPU_STRUCT[i]);
            if (ret != CY_PROT_SUCCESS) {
                break;
            }
        }
    }

    return ret;
}

/* Exported per-SMPU macros */
#define DEFINE_SMPU(N) const SMPU_Resources SMPU##N##_Resources = { \
    .smpu = PROT_SMPU_SMPU_STRUCT##N, \
    .slave_config = SMPU##N##_SLAVE_CONFIG, \
    .master_config = SMPU##N##_MASTER_CONFIG, \
}; \

#if (RTE_SMPU0)
DEFINE_SMPU(0)
#endif /* RTE_SMPU0 */

#if (RTE_SMPU1)
DEFINE_SMPU(1)
#endif /* RTE_SMPU1 */

#if (RTE_SMPU2)
DEFINE_SMPU(2)
#endif /* RTE_SMPU2 */

#if (RTE_SMPU3)
DEFINE_SMPU(3)
#endif /* RTE_SMPU3 */

#if (RTE_SMPU4)
DEFINE_SMPU(4)
#endif /* RTE_SMPU4 */

#if (RTE_SMPU5)
DEFINE_SMPU(5)
#endif /* RTE_SMPU5 */

#if (RTE_SMPU6)
DEFINE_SMPU(6)
#endif /* RTE_SMPU6 */

#if (RTE_SMPU7)
DEFINE_SMPU(7)
#endif /* RTE_SMPU7 */

#if (RTE_SMPU8)
DEFINE_SMPU(8)
#endif /* RTE_SMPU8 */

#if (RTE_SMPU9)
DEFINE_SMPU(9)
#endif /* RTE_SMPU9 */

#if (RTE_SMPU10)
DEFINE_SMPU(10)
#endif /* RTE_SMPU10 */

#if (RTE_SMPU11)
DEFINE_SMPU(11)
#endif /* RTE_SMPU11 */

#if (RTE_SMPU12)
DEFINE_SMPU(12)
#endif /* RTE_SMPU12 */

#if (RTE_SMPU13)
DEFINE_SMPU(13)
#endif /* RTE_SMPU13 */

/* Note that SMPUs 14 and 15 are fixed by romboot */
