/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "compiler_ext_defs.h"
#include "ns_agent_tz.h"
#include "tfm_arch.h"
#include "tfm_hal_platform.h"

/*
 * Initialization before launching NSPE (in C source).
 */
__used uint32_t ns_agent_tz_init_c(void)
{
    /* SCB_NS.VTOR points to the Non-secure vector table base address */
    SCB_NS->VTOR = tfm_hal_get_ns_VTOR();

    /* Setups Main stack pointer of the non-secure code */
    __TZ_set_MSP_NS(tfm_hal_get_ns_MSP());

    return tfm_hal_get_ns_entry_point();
}
