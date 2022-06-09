/*
 * Copyright (c) 2020-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "tfm_api.h"
#include "tfm_hal_defs.h"
#include "tfm_multi_core.h"
#include "tfm_hal_isolation.h"
#include "mpu_config.h"

#ifdef CONFIG_TFM_ENABLE_MEMORY_PROTECT


REGION_DECLARE(Image$$, TFM_UNPRIV_CODE, $$RO$$Base);
REGION_DECLARE(Image$$, TFM_UNPRIV_CODE, $$RO$$Limit);
REGION_DECLARE(Image$$, TFM_APP_CODE_START, $$Base);
REGION_DECLARE(Image$$, TFM_APP_CODE_END, $$Base);
REGION_DECLARE(Image$$, TFM_APP_RW_STACK_START, $$Base);
REGION_DECLARE(Image$$, TFM_APP_RW_STACK_END, $$Base);
#ifdef CONFIG_TFM_PARTITION_META
REGION_DECLARE(Image$$, TFM_SP_META_PTR, $$ZI$$Base);
REGION_DECLARE(Image$$, TFM_SP_META_PTR, $$ZI$$Limit);
#endif /* CONFIG_TFM_PARTITION_META */


static void configure_mpu(uint32_t rnr, uint32_t base, uint32_t limit,
                                uint32_t is_xn_exec, uint32_t ap_permissions)
{
    uint32_t size; /* region size */
    uint32_t rasr; /* region attribute and size register */
    uint32_t rbar; /* region base address register */

    size = get_rbar_size_field(limit - base);

    rasr = ARM_MPU_RASR(is_xn_exec, ap_permissions, TEX, NOT_SHAREABLE,
            NOT_CACHEABLE, NOT_BUFFERABLE, SUB_REGION_DISABLE, size);
    rbar = base & MPU_RBAR_ADDR_Msk;

    ARM_MPU_SetRegionEx(rnr, rbar, rasr);
}

#endif /* CONFIG_TFM_ENABLE_MEMORY_PROTECT */

enum tfm_hal_status_t tfm_hal_set_up_static_boundaries(void)
{
#ifdef CONFIG_TFM_ENABLE_MEMORY_PROTECT
    uint32_t rnr = TFM_ISOLATION_REGION_START_NUMBER; /* current region number */
    uint32_t base; /* start address */
    uint32_t limit; /* end address */

    ARM_MPU_Disable();

    /* TFM Core unprivileged code region */
    base = (uint32_t)&REGION_NAME(Image$$, TFM_UNPRIV_CODE, $$RO$$Base);
    limit = (uint32_t)&REGION_NAME(Image$$, TFM_UNPRIV_CODE, $$RO$$Limit);

    configure_mpu(rnr++, base, limit, XN_EXEC_OK, AP_RO_PRIV_UNPRIV);

    /* RO region */
    base = (uint32_t)&REGION_NAME(Image$$, TFM_APP_CODE_START, $$Base);
    limit = (uint32_t)&REGION_NAME(Image$$, TFM_APP_CODE_END, $$Base);

    configure_mpu(rnr++, base, limit, XN_EXEC_OK, AP_RO_PRIV_UNPRIV);

    /* RW, ZI and stack as one region */
    base = (uint32_t)&REGION_NAME(Image$$, TFM_APP_RW_STACK_START, $$Base);
    limit = (uint32_t)&REGION_NAME(Image$$, TFM_APP_RW_STACK_END, $$Base);

    configure_mpu(rnr++, base, limit, XN_EXEC_NOT_OK, AP_RW_PRIV_UNPRIV);

#ifdef CONFIG_TFM_PARTITION_META
    /* TFM partition metadata pointer region */
    base = (uint32_t)&REGION_NAME(Image$$, TFM_SP_META_PTR, $$ZI$$Base);
    limit = (uint32_t)&REGION_NAME(Image$$, TFM_SP_META_PTR, $$ZI$$Limit);

    configure_mpu(rnr++, base, limit, XN_EXEC_NOT_OK, AP_RW_PRIV_UNPRIV);
#endif

    arm_mpu_enable();

#endif /* CONFIG_TFM_ENABLE_MEMORY_PROTECT */
    return TFM_HAL_SUCCESS;
}

enum tfm_hal_status_t tfm_hal_memory_has_access(uintptr_t base,
                                                size_t size,
                                                uint32_t attr)
{
    enum tfm_status_e status;

    status = tfm_has_access_to_region((const void *)base, size, attr);
    if (status != TFM_SUCCESS) {
         return TFM_HAL_ERROR_MEM_FAULT;
    }

    return TFM_HAL_SUCCESS;
}

/*
 * Implementation of tfm_hal_bind_boundaries() on Corstone1000:
 *
 * The API encodes some attributes into a handle and returns it to SPM.
 * The attributes include isolation boundaries, privilege, and MMIO information.
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
 *      BIT | 31        24 | 23         20 | ... | 7           4 | 3        0 |
 *          | Unique Index | Region Attr 5 | ... | Region Attr 1 | Privileged |
 *
 *      In which the "Region Attr i" is:
 *      BIT |       3      | 2        0 |
 *          | 1: RW, 0: RO | MMIO Index |
 *
 * - For isolation level 1/2
 *      BIT | 31                           0 |
 *          | 1: privileged, 0: unprivileged |
 *
 * This is a reference implementation on AN521, and may have some limitations.
 * 1. The maximum number of allowed MMIO regions is 5.
 * 2. Highest 8 bits are for index. It supports 256 unique handles at most.
 *
 */

#define HANDLE_PER_ATTR_BITS            (0x4)
#define HANDLE_ATTR_PRIV_MASK           ((1 << HANDLE_PER_ATTR_BITS) - 1)

enum tfm_hal_status_t tfm_hal_bind_boundaries(
                                    const struct partition_load_info_t *p_ldinf,
                                    void **pp_boundaries)
{
    bool privileged;

    if (!p_ldinf || !pp_boundaries) {
        return TFM_HAL_ERROR_GENERIC;
    }

#if TFM_LVL == 1
    privileged = true;
#else
    privileged = IS_PARTITION_PSA_ROT(p_ldinf);
#endif

    *pp_boundaries = (void *)(((uint32_t)privileged) & HANDLE_ATTR_PRIV_MASK);

    /* NOTE: Need to add validation of numbered MMIO if platform requires. */
    /* Platform does not have a need for MMIO yet. */

    return TFM_HAL_SUCCESS;
}

enum tfm_hal_status_t tfm_hal_update_boundaries(
                             const struct partition_load_info_t *p_ldinf,
                             void *p_boundaries)
{
    CONTROL_Type ctrl;
    uint32_t local_handle = (uint32_t)p_boundaries;
    bool privileged = !!(local_handle & HANDLE_ATTR_PRIV_MASK);

    /* Privileged level is required to be set always */
    ctrl.w = __get_CONTROL();
    ctrl.b.nPRIV = privileged ? 0 : 1;
    __set_CONTROL(ctrl.w);

    return TFM_HAL_SUCCESS;
}
