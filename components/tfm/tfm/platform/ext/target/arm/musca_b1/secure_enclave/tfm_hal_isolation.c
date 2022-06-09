/*
 * Copyright (c) 2020-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "mmio_defs.h"
#include "tfm_api.h"
#include "tfm_hal_defs.h"
#include "tfm_multi_core.h"
#include "load/spm_load_api.h"

enum tfm_hal_status_t tfm_hal_set_up_static_boundaries(void)
{
    /* Nothing to do, there is no isolation HW in this platform to be
     * configured by Secure Enclave */
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
 * Implementation of tfm_hal_bind_boundaries() on Secure enclave:
 */
enum tfm_hal_status_t tfm_hal_bind_boundaries(
                                    const struct partition_load_info_t *p_ldinf,
                                    void **pp_boundaries)
{
    bool privileged = true;

    if (!p_ldinf || !pp_boundaries) {
        return TFM_HAL_ERROR_GENERIC;
    }

    *pp_boundaries = (void *)(((uint32_t)privileged) & HANDLE_ATTR_PRIV_MASK);

    return TFM_HAL_SUCCESS;
}

enum tfm_hal_status_t tfm_hal_update_boundaries(
                             const struct partition_load_info_t *p_ldinf,
                             void *p_boundaries)
{
    (void)p_ldinf;
    (void)p_boundaries;
    return TFM_HAL_SUCCESS;
}
