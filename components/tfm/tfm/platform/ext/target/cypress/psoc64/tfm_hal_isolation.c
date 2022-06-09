/*
 * Copyright (c) 2020-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "array.h"
#include "cy_device.h"
#include "mmio_defs.h"
#include "target_cfg.h"
#include "tfm_api.h"
#include "tfm_hal_defs.h"
#include "tfm_multi_core.h"
#include "tfm_plat_defs.h"
#include "tfm_peripherals_def.h"
#include "load/asset_defs.h"
#include "load/spm_load_api.h"

enum tfm_hal_status_t tfm_hal_set_up_static_boundaries(void)
{
    Cy_PDL_Init(CY_DEVICE_CFG);

    if (smpu_init_cfg() != TFM_PLAT_ERR_SUCCESS) {
        return TFM_HAL_ERROR_GENERIC;
    }

    if (ppu_init_cfg() != TFM_PLAT_ERR_SUCCESS) {
        return TFM_HAL_ERROR_GENERIC;
    }

    if (bus_masters_cfg() != TFM_PLAT_ERR_SUCCESS) {
        return TFM_HAL_ERROR_GENERIC;
    }

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
 * Implementation of tfm_hal_bind_boundaries() on PSOC64:
 *
 * The API encodes some attributes into a handle and returns it to SPM.
 * The attributes include isolation boundaries, privilege, and MMIO information.
 * When scheduler switches running partitions, SPM compares the handle between
 * partitions to know if boundary update is necessary. If update is required,
 * SPM passes the handle to platform to do platform settings and update
 * isolation boundaries.
 */
enum tfm_hal_status_t tfm_hal_bind_boundaries(
                                    const struct partition_load_info_t *p_ldinf,
                                    void **pp_boundaries)
{
    uint32_t i, j;
    bool privileged;
    const struct asset_desc_t *p_asset;

    if (!p_ldinf || !pp_boundaries) {
        return TFM_HAL_ERROR_GENERIC;
    }

#if TFM_LVL == 1
    privileged = true;
#else
    privileged = IS_PARTITION_PSA_ROT(p_ldinf);
#endif

    p_asset = (const struct asset_desc_t *)LOAD_INFO_ASSET(p_ldinf);

    /*
     * Validate if the named MMIO of partition is allowed by the platform.
     * Otherwise, skip validation.
     *
     * NOTE: Need to add validation of numbered MMIO if platform requires.
     */
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
            return TFM_HAL_ERROR_GENERIC;
        }
    }
    *pp_boundaries = (void *)(((uint32_t)privileged) & HANDLE_ATTR_PRIV_MASK);

    return TFM_HAL_SUCCESS;
}

enum tfm_hal_status_t tfm_hal_update_boundaries(
                             const struct partition_load_info_t *p_ldinf,
                             void *p_boundaries)
{
    CONTROL_Type ctrl;
    bool privileged = !!((uint32_t)p_boundaries & HANDLE_ATTR_PRIV_MASK);

    /* Privileged level is required to be set always */
    ctrl.w = __get_CONTROL();
    ctrl.b.nPRIV = privileged ? 0 : 1;
    __set_CONTROL(ctrl.w);

    return TFM_HAL_SUCCESS;
}
