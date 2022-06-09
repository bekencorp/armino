/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "tfm_hal_defs.h"
#include "tfm_hal_platform_common.h"

enum tfm_hal_status_t tfm_hal_platform_init(void)
{
    return tfm_hal_platform_common_init();
}
