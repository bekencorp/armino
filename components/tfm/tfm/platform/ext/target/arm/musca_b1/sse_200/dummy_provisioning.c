/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "tfm_plat_provisioning.h"

int tfm_plat_provisioning_is_required(void)
{
    return 0;
}

enum tfm_plat_err_t tfm_plat_provisioning_perform(void)
{
    return TFM_PLAT_ERR_SUCCESS;
}

void tfm_plat_provisioning_check_for_dummy_keys(void)
{
}
