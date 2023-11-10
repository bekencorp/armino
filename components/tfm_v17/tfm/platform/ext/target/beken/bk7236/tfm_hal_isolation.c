/*
 * Copyright (c) 2020-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <arm_cmse.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "array.h"
#include "cmsis.h"
#include "Driver_Common.h"
#include "mmio_defs.h"
#include "mpu_armv8m_drv.h"
#include "region.h"
#include "target_cfg.h"
#include "tfm_hal_defs.h"
#include "tfm_hal_isolation.h"
#include "tfm_peripherals_def.h"
#include "load/partition_defs.h"
#include "load/asset_defs.h"
#include "load/spm_load_api.h"

/* It can be retrieved from the MPU_TYPE register. */
#define MPU_REGION_NUM                  8
#define PROT_BOUNDARY_VAL \
    ((1U << HANDLE_ATTR_PRIV_POS) & HANDLE_ATTR_PRIV_MASK)

#ifdef CONFIG_TFM_ENABLE_MEMORY_PROTECT

#if (((2 == TFM_LVL) && CONFIG_TFM_MMIO_REGION_ENABLE) || (CONFIG_BEKEN_SUPPORT_MPU && defined(CONFIG_TFM_ENABLE_MEMORY_PROTECT)))
static uint32_t n_configured_regions = 0;
#endif

struct mpu_armv8m_dev_t dev_mpu_s = {MPU_BASE};

#ifdef CONFIG_TFM_PARTITION_META
REGION_DECLARE(Image$$, TFM_SP_META_PTR, $$ZI$$Base);
REGION_DECLARE(Image$$, TFM_SP_META_PTR, $$ZI$$Limit);
#endif /* CONFIG_TFM_PARTITION_META */

#if TFM_LVL == 3
/* Isolation level 3 needs to reserve at lease one MPU region for private data asset. */
#define MIN_NR_PRIVATE_DATA_REGION    1

static uint32_t idx_boundary_handle = 0;
REGION_DECLARE(Image$$, PT_RO_START, $$Base);
REGION_DECLARE(Image$$, PT_RO_END, $$Base);
REGION_DECLARE(Image$$, PT_PRIV_RWZI_START, $$Base);
REGION_DECLARE(Image$$, PT_PRIV_RWZI_END, $$Base);

const static struct mpu_armv8m_region_cfg_t isolation_regions[] = {
    {
        0, /* will be updated before using */
        (uint32_t)&REGION_NAME(Image$$, PT_RO_START, $$Base),
        (uint32_t)&REGION_NAME(Image$$, PT_RO_END, $$Base),
        MPU_ARMV8M_MAIR_ATTR_CODE_IDX,
        MPU_ARMV8M_XN_EXEC_OK,
        MPU_ARMV8M_AP_RO_PRIV_UNPRIV,
        MPU_ARMV8M_SH_NONE,
    },
    /* For isolation Level 3, set up static isolation for privileged data.
     * Unprivileged data is dynamically set during Partition scheduling.
     */
    {
        0, /* will be updated before using */
        (uint32_t)&REGION_NAME(Image$$, PT_PRIV_RWZI_START, $$Base),
        (uint32_t)&REGION_NAME(Image$$, PT_PRIV_RWZI_END, $$Base),
        MPU_ARMV8M_MAIR_ATTR_DATA_IDX,
        MPU_ARMV8M_XN_EXEC_NEVER,
        MPU_ARMV8M_AP_RW_PRIV_ONLY,
        MPU_ARMV8M_SH_NONE,
    },
#ifdef CONFIG_TFM_PARTITION_META
    /* TFM partition metadata pointer region */
    {
        0, /* will be updated before using */
        (uint32_t)&REGION_NAME(Image$$, TFM_SP_META_PTR, $$ZI$$Base),
        (uint32_t)&REGION_NAME(Image$$, TFM_SP_META_PTR, $$ZI$$Limit),
        MPU_ARMV8M_MAIR_ATTR_DATA_IDX,
        MPU_ARMV8M_XN_EXEC_NEVER,
        MPU_ARMV8M_AP_RW_PRIV_UNPRIV,
        MPU_ARMV8M_SH_NONE
    }
#endif
};
#else /* TFM_LVL == 3 */
/* Isolation level 1&2 do not need to reserve MPU region for private data asset. */
#define MIN_NR_PRIVATE_DATA_REGION    0

REGION_DECLARE(Image$$, ER_VENEER, $$Base);
REGION_DECLARE(Image$$, VENEER_ALIGN, $$Limit);
REGION_DECLARE(Image$$, TFM_UNPRIV_CODE, $$RO$$Base);
REGION_DECLARE(Image$$, TFM_UNPRIV_CODE, $$RO$$Limit);
REGION_DECLARE(Image$$, TFM_APP_CODE_START, $$Base);
REGION_DECLARE(Image$$, TFM_APP_CODE_END, $$Base);
REGION_DECLARE(Image$$, TFM_APP_RW_STACK_START, $$Base);
REGION_DECLARE(Image$$, TFM_APP_RW_STACK_END, $$Base);

const struct mpu_armv8m_region_cfg_t region_cfg[] = {
    /* Veneer region */
    {
        0, /* will be updated before using */
        (uint32_t)&REGION_NAME(Image$$, ER_VENEER, $$Base),
        (uint32_t)&REGION_NAME(Image$$, VENEER_ALIGN, $$Limit),
        MPU_ARMV8M_MAIR_ATTR_CODE_IDX,
        MPU_ARMV8M_XN_EXEC_OK,
        MPU_ARMV8M_AP_RO_PRIV_UNPRIV,
        MPU_ARMV8M_SH_NONE
    },
    /* TFM Core unprivileged code region */
    {
        0, /* will be updated before using */
        (uint32_t)&REGION_NAME(Image$$, TFM_UNPRIV_CODE, $$RO$$Base),
        (uint32_t)&REGION_NAME(Image$$, TFM_UNPRIV_CODE, $$RO$$Limit),
        MPU_ARMV8M_MAIR_ATTR_CODE_IDX,
        MPU_ARMV8M_XN_EXEC_OK,
        MPU_ARMV8M_AP_RO_PRIV_UNPRIV,
        MPU_ARMV8M_SH_NONE
    },
    /* RO region */
    {
        0, /* will be updated before using */
        (uint32_t)&REGION_NAME(Image$$, TFM_APP_CODE_START, $$Base),
        (uint32_t)&REGION_NAME(Image$$, TFM_APP_CODE_END, $$Base),
        MPU_ARMV8M_MAIR_ATTR_CODE_IDX,
        MPU_ARMV8M_XN_EXEC_OK,
        MPU_ARMV8M_AP_RO_PRIV_UNPRIV,
        MPU_ARMV8M_SH_NONE
    },
    /* RW, ZI and stack as one region */
    {
        0, /* will be updated before using */
        (uint32_t)&REGION_NAME(Image$$, TFM_APP_RW_STACK_START, $$Base),
        (uint32_t)&REGION_NAME(Image$$, TFM_APP_RW_STACK_END, $$Base),
        MPU_ARMV8M_MAIR_ATTR_DATA_IDX,
        MPU_ARMV8M_XN_EXEC_NEVER,
        MPU_ARMV8M_AP_RW_PRIV_UNPRIV,
        MPU_ARMV8M_SH_NONE
    },
#ifdef CONFIG_TFM_PARTITION_META
    /* TFM partition metadata pointer region */
    {
        0, /* will be updated before using */
        (uint32_t)&REGION_NAME(Image$$, TFM_SP_META_PTR, $$ZI$$Base),
        (uint32_t)&REGION_NAME(Image$$, TFM_SP_META_PTR, $$ZI$$Limit),
        MPU_ARMV8M_MAIR_ATTR_DATA_IDX,
        MPU_ARMV8M_XN_EXEC_NEVER,
        MPU_ARMV8M_AP_RW_PRIV_UNPRIV,
        MPU_ARMV8M_SH_NONE
    }
#endif
};
#endif /* TFM_LVL == 3 */
#endif /* CONFIG_TFM_ENABLE_MEMORY_PROTECT */

#ifdef TFM_FIH_PROFILE_ON
#ifdef CONFIG_TFM_ENABLE_MEMORY_PROTECT
static fih_int fih_verify_mpu_armv8m_region_enabled(
                                const struct mpu_armv8m_dev_t *dev,
                                const struct mpu_armv8m_region_cfg_t *region_cfg,
                                uint32_t region_nr)
{
    MPU_Type *mpu = (MPU_Type *)dev->base;

    uint32_t base_cfg;
    uint32_t limit_cfg;

    if ((region_cfg->region_base & ~MPU_RBAR_BASE_Msk) != 0) {
        FIH_RET(fih_int_encode(MPU_ARMV8M_ERROR));
    }

    mpu->RNR  = region_nr & MPU_RNR_REGION_Msk;

    base_cfg = region_cfg->region_base & MPU_RBAR_BASE_Msk;
    base_cfg |= (region_cfg->attr_sh << MPU_RBAR_SH_Pos) & MPU_RBAR_SH_Msk;
    base_cfg |= (region_cfg->attr_access << MPU_RBAR_AP_Pos) & MPU_RBAR_AP_Msk;
    base_cfg |= (region_cfg->attr_exec << MPU_RBAR_XN_Pos) & MPU_RBAR_XN_Msk;
    if (mpu->RBAR != base_cfg) {
        FIH_RET(fih_int_encode(MPU_ARMV8M_ERROR));
    }

    limit_cfg = (region_cfg->region_limit-1) & MPU_RLAR_LIMIT_Msk;
    limit_cfg |= (region_cfg->region_attridx << MPU_RLAR_AttrIndx_Pos) &
                 MPU_RLAR_AttrIndx_Msk;
    limit_cfg |= MPU_RLAR_EN_Msk;
    if (mpu->RLAR != limit_cfg) {
        FIH_RET(fih_int_encode(MPU_ARMV8M_ERROR));
    }

    FIH_RET(fih_int_encode(MPU_ARMV8M_OK));
}

static fih_int fih_verify_mpu_armv8m_enabled(struct mpu_armv8m_dev_t *dev,
                                             uint32_t privdef_en,
                                             uint32_t hfnmi_en)
{
    MPU_Type *mpu = (MPU_Type *)dev->base;

    if (mpu->MAIR0 != ((MPU_ARMV8M_MAIR_ATTR_DEVICE_VAL << MPU_MAIR0_Attr0_Pos) |
                       (MPU_ARMV8M_MAIR_ATTR_CODE_VAL << MPU_MAIR0_Attr1_Pos) |
                       (MPU_ARMV8M_MAIR_ATTR_DATA_VAL << MPU_MAIR0_Attr2_Pos))) {
        FIH_RET(fih_int_encode(MPU_ARMV8M_ERROR));
    }

    if (mpu->CTRL != (MPU_CTRL_ENABLE_Msk |
                      (privdef_en ? MPU_CTRL_PRIVDEFENA_Msk : 0) |
                      (hfnmi_en   ? MPU_CTRL_HFNMIENA_Msk   : 0))) {
        FIH_RET(fih_int_encode(MPU_ARMV8M_ERROR));
    }

    FIH_RET(fih_int_encode(MPU_ARMV8M_OK));
}
#endif /* CONFIG_TFM_ENABLE_MEMORY_PROTECT */

fih_int tfm_hal_verify_static_boundaries(void)
{
    fih_int fih_rc = FIH_FAILURE;

    /* Verify isolation boundaries between SPE and NSPE */
    FIH_CALL(fih_verify_sau_and_idau_cfg, fih_rc);
    if (fih_not_eq(fih_rc, fih_int_encode(ARM_DRIVER_OK))) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }
    FIH_CALL(fih_verify_mpc_cfg, fih_rc);
    if (fih_not_eq(fih_rc, fih_int_encode(ARM_DRIVER_OK))) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }
    FIH_CALL(fih_verify_ppc_cfg, fih_rc);
    if (fih_not_eq(fih_rc, fih_int_encode(ARM_DRIVER_OK))) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }

    /* Repeat the verification to mitigate instruction skip */
    FIH_CALL(fih_verify_sau_and_idau_cfg, fih_rc);
    if (fih_not_eq(fih_rc, fih_int_encode(ARM_DRIVER_OK))) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }
    FIH_CALL(fih_verify_mpc_cfg, fih_rc);
    if (fih_not_eq(fih_rc, fih_int_encode(ARM_DRIVER_OK))) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }
    FIH_CALL(fih_verify_ppc_cfg, fih_rc);
    if (fih_not_eq(fih_rc, fih_int_encode(ARM_DRIVER_OK))) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }

    /* Verify static isolation boundaries inside SPE */
#ifdef CONFIG_TFM_ENABLE_MEMORY_PROTECT
    uint32_t region_nr;

    if ((ARRAY_SIZE(region_cfg) + MIN_NR_PRIVATE_DATA_REGION) > MPU_REGION_NUM) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }

    for (region_nr = 0; region_nr < ARRAY_SIZE(region_cfg); region_nr++) {
        /* Verify regions are correctly enabled */
        FIH_CALL(fih_verify_mpu_armv8m_region_enabled, fih_rc, &dev_mpu_s, &region_cfg[region_nr], region_nr);
        if (fih_not_eq(fih_rc, fih_int_encode(MPU_ARMV8M_OK))) {
            FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
        }
        /* Repeat the verification to mitigate instruction skip */
        FIH_CALL(fih_verify_mpu_armv8m_region_enabled, fih_rc, &dev_mpu_s, &region_cfg[region_nr], region_nr);
        if (fih_not_eq(fih_rc, fih_int_encode(MPU_ARMV8M_OK))) {
            FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
        }
    }

    /* Verify MPU is correctly enabled */
    FIH_CALL(fih_verify_mpu_armv8m_enabled, fih_rc, &dev_mpu_s,
                PRIVILEGED_DEFAULT_ENABLE, HARDFAULT_NMI_ENABLE);
    if (fih_not_eq(fih_rc, fih_int_encode(MPU_ARMV8M_OK))) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }
    /* Repeat the verification to mitigate instruction skip */
    FIH_CALL(fih_verify_mpu_armv8m_enabled, fih_rc, &dev_mpu_s,
                PRIVILEGED_DEFAULT_ENABLE, HARDFAULT_NMI_ENABLE);
    if (fih_not_eq(fih_rc, fih_int_encode(MPU_ARMV8M_OK))) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }
#endif /* CONFIG_TFM_ENABLE_MEMORY_PROTECT */

    FIH_RET(fih_int_encode(TFM_HAL_SUCCESS));
}
#endif /* TFM_FIH_PROFILE_ON */

FIH_RET_TYPE(enum tfm_hal_status_t) tfm_hal_set_up_static_boundaries(
                                                uintptr_t *p_spm_boundary)
{
    fih_int fih_rc = FIH_FAILURE;
    /* Set up isolation boundaries between SPE and NSPE */
    FIH_CALL(sau_and_idau_cfg, fih_rc);
    if (fih_not_eq(fih_rc, fih_int_encode(ARM_DRIVER_OK))) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }
    FIH_CALL(mpc_init_cfg, fih_rc);
    if (fih_not_eq(fih_rc, fih_int_encode(ARM_DRIVER_OK))) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }
    FIH_CALL(ppc_init_cfg, fih_rc);
    if (fih_not_eq(fih_rc, fih_int_encode(ARM_DRIVER_OK))) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }

    /* Set up static isolation boundaries inside SPE */
#if CONFIG_BEKEN_SUPPORT_MPU	
#ifdef CONFIG_TFM_ENABLE_MEMORY_PROTECT
    struct mpu_armv8m_region_cfg_t localcfg;
    int32_t i;

    mpu_armv8m_clean(&dev_mpu_s);

    if ((ARRAY_SIZE(region_cfg) + MIN_NR_PRIVATE_DATA_REGION) > MPU_REGION_NUM) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }

    /* Update MPU region numbers. The numbers start from 0 and are continuous. */
    for (i = 0; i < ARRAY_SIZE(region_cfg); i++) {
        spm_memcpy(&localcfg, &region_cfg[i], sizeof(localcfg));
        /* Update region number */
        localcfg.region_nr = i;
        /* Enable regions */
        FIH_CALL(mpu_armv8m_region_enable, fih_rc, &dev_mpu_s, &localcfg);
        if (fih_not_eq(fih_rc, fih_int_encode(MPU_ARMV8M_OK))) {
            FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
        }
    }
    n_configured_regions = i;

    /* Enable MPU */
    FIH_CALL(mpu_armv8m_enable, fih_rc, &dev_mpu_s,
             PRIVILEGED_DEFAULT_ENABLE, HARDFAULT_NMI_ENABLE);
    if (fih_not_eq(fih_rc, fih_int_encode(MPU_ARMV8M_OK))) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }
#endif /* CONFIG_TFM_ENABLE_MEMORY_PROTECT */
#endif /* CONFIG_BEKEN_SUPPORT_MPU */

    *p_spm_boundary = (uintptr_t)PROT_BOUNDARY_VAL;

    FIH_RET(fih_int_encode(TFM_HAL_SUCCESS));
}

/*
 * Implementation of tfm_hal_bind_boundaries() on BK7236:
 *
 * The API encodes some attributes into a handle and returns it to SPM.
 * The attributes include isolation boundaries, privilege, and mmio information.
 * When scheduler switches running partitions, SPM compares the handle between
 * partitions to know if boundary update is necessary. If update is required,
 * SPM passes the handle to platform to do platform settings and update
 * isolation boundaries.
 *
 * The handle should be unique under isolation level 3. The implementation
 * encodes an index at the highest 8 bits to assure handle uniqueness. While
 * under isolation level 1/2, handles may not be unique.
 *
 * The encoding format assignment:
 * - For isolation level 3
 *      BIT | 31        24 | 23         20 | ... | 7           4 | 3       0 |
 *          | Unique Index | Region Attr 5 | ... | Region Attr 1 | Base Attr |
 *
 *      In which the "Region Attr i" is:
 *      BIT |       3      | 2        0 |
 *          | 1: RW, 0: RO | MMIO Index |
 *
 *      In which the "Base Attr" is:
 *      BIT |               1                |                           0                     |
 *          | 1: privileged, 0: unprivileged | 1: Trustzone-specific NSPE, 0: Secure partition |
 *
 * - For isolation level 1/2
 *      BIT | 31     2 |              1                |                           0                     |
 *          | Reserved |1: privileged, 0: unprivileged | 1: Trustzone-specific NSPE, 0: Secure partition |
 *
 * This is a reference implementation on BK7236, and may have some limitations.
 * 1. The maximum number of allowed MMIO regions is 5.
 * 2. Highest 8 bits are for index. It supports 256 unique handles at most.
 */
FIH_RET_TYPE(enum tfm_hal_status_t) tfm_hal_bind_boundary(
                                    const struct partition_load_info_t *p_ldinf,
                                    uintptr_t *p_boundary)
{
    bool privileged;
    bool ns_agent;
    uint32_t partition_attrs = 0;

#if CONFIG_TFM_MMIO_REGION_ENABLE == 1
#if TFM_LVL == 2
    struct mpu_armv8m_region_cfg_t localcfg;
#endif
    uint32_t i, j;
    const struct asset_desc_t *p_asset;
    struct platform_data_t *plat_data_ptr;
    fih_int fih_rc = FIH_FAILURE;
#endif /* CONFIG_TFM_MMIO_REGION_ENABLE == 1 */

    if (!p_ldinf || !p_boundary) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }

#if TFM_LVL == 1
    privileged = true;
#else
    privileged = IS_PARTITION_PSA_ROT(p_ldinf);
#endif

    ns_agent = IS_PARTITION_NS_AGENT(p_ldinf);

    /*
     * Validate if the named MMIO of partition is allowed by the platform.
     * Otherwise, skip validation.
     *
     * NOTE: Need to add validation of numbered MMIO if platform requires.
     */
//TODO wzl-issue7, double check the following macro
#if CONFIG_TFM_MMIO_REGION_ENABLE == 1
    p_asset = LOAD_INFO_ASSET(p_ldinf);

    for (i = 0; i < p_ldinf->nassets; i++) {
        if (!(p_asset[i].attr & ASSET_ATTR_NAMED_MMIO)) {
            continue;
        }
        for (j = 0; j < ARRAY_SIZE(partition_named_mmio_list); j++) {
            if (p_asset[i].dev.dev_ref == partition_named_mmio_list[j]) {
                break;
            }
        }

        if (j == ARRAY_SIZE(partition_named_mmio_list)) {
            /* The MMIO asset is not in the allowed list of platform. */
            FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
        }
        /* Assume PPC & MPC settings are required even under level 1 */
        plat_data_ptr = REFERENCE_TO_PTR(p_asset[i].dev.dev_ref,
                                         struct platform_data_t *);
        //TODO peter can add bk7236 configure here
#if TFM_LVL == 2
        /*
         * Static boundaries are set. Set up MPU region for MMIO.
         * Setup regions for unprivileged assets only.
         */
        if (!privileged) {
            localcfg.region_base = plat_data_ptr->periph_start;
            localcfg.region_limit = plat_data_ptr->periph_limit;
            localcfg.region_attridx = MPU_ARMV8M_MAIR_ATTR_DEVICE_IDX;
            localcfg.attr_access = MPU_ARMV8M_AP_RW_PRIV_UNPRIV;
            localcfg.attr_sh = MPU_ARMV8M_SH_NONE;
            localcfg.attr_exec = MPU_ARMV8M_XN_EXEC_NEVER;
            localcfg.region_nr = n_configured_regions++;

            FIH_CALL(mpu_armv8m_region_enable, fih_rc, &dev_mpu_s, &localcfg);
            if (fih_not_eq(fih_rc, fih_int_encode(MPU_ARMV8M_OK))) {
                FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
            }
        }
#elif TFM_LVL == 3
        /* Encode MMIO attributes into the "partition_attrs". */
        partition_attrs <<= HANDLE_PER_ATTR_BITS;
        partition_attrs |= ((j + 1) & HANDLE_ATTR_INDEX_MASK);
        if (p_asset[i].attr & ASSET_ATTR_READ_WRITE) {
            partition_attrs |= HANDLE_ATTR_RW_POS;
        }
#endif
    }
#endif /* CONFIG_TFM_MMIO_REGION_ENABLE == 1 */

#if TFM_LVL == 3
    partition_attrs <<= HANDLE_PER_ATTR_BITS;
    /*
     * Highest 8 bits are reserved for index, if they are non-zero, MMIO numbers
     * must have exceeded the limit of 5.
     */
    if (partition_attrs & HANDLE_INDEX_MASK) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }
    HANDLE_ENCODE_INDEX(partition_attrs, idx_boundary_handle);
#endif

    partition_attrs |= ((uint32_t)privileged << HANDLE_ATTR_PRIV_POS) &
                        HANDLE_ATTR_PRIV_MASK;
    partition_attrs |= ((uint32_t)ns_agent << HANDLE_ATTR_NS_POS) &
                        HANDLE_ATTR_NS_MASK;
    *p_boundary = (uintptr_t)partition_attrs;

    FIH_RET(fih_int_encode(TFM_HAL_SUCCESS));
}

FIH_RET_TYPE(enum tfm_hal_status_t) tfm_hal_activate_boundary(
                             const struct partition_load_info_t *p_ldinf,
                             uintptr_t boundary)
{
    CONTROL_Type ctrl;
    uint32_t local_handle = (uint32_t)boundary;
    bool privileged = !!(local_handle & HANDLE_ATTR_PRIV_MASK);

#if TFM_LVL == 3
    struct mpu_armv8m_region_cfg_t localcfg;
    uint32_t i, mmio_index;
    struct platform_data_t *plat_data_ptr;
    const struct asset_desc_t *rt_mem;
    fih_int fih_rc = FIH_FAILURE;
#endif /* TFM_LVL == 3 */

    /* Privileged level is required to be set always */
    ctrl.w = __get_CONTROL();
    ctrl.b.nPRIV = privileged ? 0 : 1;
    __set_CONTROL(ctrl.w);

#if TFM_LVL == 3
    if (!p_ldinf) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }

    /* Update regions, for unprivileged partitions only */
    if (privileged) {
        FIH_RET(fih_int_encode(TFM_HAL_SUCCESS));
    }

    /* Setup runtime memory first */
    localcfg.attr_exec = MPU_ARMV8M_XN_EXEC_NEVER;
    localcfg.attr_sh = MPU_ARMV8M_SH_NONE;
    localcfg.region_attridx = MPU_ARMV8M_MAIR_ATTR_DATA_IDX;
    localcfg.attr_access = MPU_ARMV8M_AP_RW_PRIV_UNPRIV;
    rt_mem = LOAD_INFO_ASSET(p_ldinf);
    /*
     * BK7236 shortcut: The first item is the only runtime memory asset.
     * Platforms with many memory assets please check this part.
     */
    for (i = 0;
         i < p_ldinf->nassets && !(rt_mem[i].attr & ASSET_ATTR_MMIO);
         i++) {
        localcfg.region_nr = n_configured_regions + i;
        localcfg.region_base = rt_mem[i].mem.start;
        localcfg.region_limit = rt_mem[i].mem.limit;

        FIH_CALL(mpu_armv8m_region_enable, fih_rc, &dev_mpu_s, &localcfg);
        if (fih_not_eq(fih_rc, fih_int_encode(MPU_ARMV8M_OK))) {
            FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
        }
    }

    i = n_configured_regions + i;

#if CONFIG_TFM_MMIO_REGION_ENABLE == 1
    /* Named MMIO part */
    local_handle = local_handle & (~HANDLE_INDEX_MASK);
    local_handle >>= HANDLE_PER_ATTR_BITS;
    mmio_index = local_handle & HANDLE_ATTR_INDEX_MASK;

    localcfg.region_attridx = MPU_ARMV8M_MAIR_ATTR_DEVICE_IDX;

    while (mmio_index && i < MPU_REGION_NUM) {
        plat_data_ptr =
          (struct platform_data_t *)partition_named_mmio_list[mmio_index - 1];
        localcfg.region_nr = i++;
        localcfg.attr_access = (local_handle & HANDLE_ATTR_RW_POS)?
                            MPU_ARMV8M_AP_RW_PRIV_UNPRIV :
                            MPU_ARMV8M_AP_RO_PRIV_UNPRIV;
        localcfg.region_base = plat_data_ptr->periph_start;
        localcfg.region_limit = plat_data_ptr->periph_limit;

        FIH_CALL(mpu_armv8m_region_enable, fih_rc, &dev_mpu_s, &localcfg);
        if (fih_not_eq(fih_rc, fih_int_encode(MPU_ARMV8M_OK))) {
            FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
        }

        local_handle >>= HANDLE_PER_ATTR_BITS;
        mmio_index = local_handle & HANDLE_ATTR_INDEX_MASK;
    }
#endif

    /* Disable unused regions */
    while (i < MPU_REGION_NUM) {
        FIH_CALL(mpu_armv8m_region_disable, fih_rc, &dev_mpu_s, i++);
        if (fih_not_eq(fih_rc, fih_int_encode(MPU_ARMV8M_OK))) {
            FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
        }
    }
#endif /* TFM_LVL == 3 */
    FIH_RET(fih_int_encode(TFM_HAL_SUCCESS));
}

FIH_RET_TYPE(enum tfm_hal_status_t) tfm_hal_memory_check(
                                           uintptr_t boundary, uintptr_t base,
                                           size_t size, uint32_t access_type)
{
    int flags = 0;

    /* If size is zero, this indicates an empty buffer and base is ignored */
    if (size == 0) {
        FIH_RET(fih_int_encode(TFM_HAL_SUCCESS));
    }

    if (!base) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_INVALID_INPUT));
    }

    if ((access_type & TFM_HAL_ACCESS_READWRITE) == TFM_HAL_ACCESS_READWRITE) {
        flags |= CMSE_MPU_READWRITE;
    } else if (access_type & TFM_HAL_ACCESS_READABLE) {
        flags |= CMSE_MPU_READ;
    } else {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_INVALID_INPUT));
    }

    if (!((uint32_t)boundary & HANDLE_ATTR_PRIV_MASK)) {
        flags |= CMSE_MPU_UNPRIV;
    }

    if ((uint32_t)boundary & HANDLE_ATTR_NS_MASK) {
        CONTROL_Type ctrl;
        ctrl.w = __TZ_get_CONTROL_NS();
        if (ctrl.b.nPRIV == 1) {
            flags |= CMSE_MPU_UNPRIV;
        } else {
            flags &= ~CMSE_MPU_UNPRIV;
        }
        flags |= CMSE_NONSECURE;
    }

    if (cmse_check_address_range((void *)base, size, flags) != NULL) {
        FIH_RET(fih_int_encode(TFM_HAL_SUCCESS));
    } else {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_MEM_FAULT));
    }
}

bool tfm_hal_boundary_need_switch(uintptr_t boundary_from,
                                  uintptr_t boundary_to)
{
    if (boundary_from == boundary_to) {
        return false;
    }

    if (((uint32_t)boundary_from & HANDLE_ATTR_PRIV_MASK) &&
        ((uint32_t)boundary_to & HANDLE_ATTR_PRIV_MASK)) {
        return false;
    }
    return true;
}
