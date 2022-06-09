/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "tfm_plat_nv_counters.h"

enum tfm_plat_err_t tfm_plat_init_nv_counter(void)
{
    return TFM_PLAT_ERR_SUCCESS;
}

enum tfm_plat_err_t tfm_plat_read_nv_counter(enum tfm_nv_counter_t counter_id,
                                             uint32_t size, uint8_t *val)
{
    return TFM_PLAT_ERR_UNSUPPORTED;
}

enum tfm_plat_err_t tfm_plat_increment_nv_counter(
                                              enum tfm_nv_counter_t counter_id)
{
    return TFM_PLAT_ERR_UNSUPPORTED;
}

enum tfm_plat_err_t tfm_plat_set_nv_counter(enum tfm_nv_counter_t counter_id,
                                            uint32_t value)
{
    return TFM_PLAT_ERR_UNSUPPORTED;
}
