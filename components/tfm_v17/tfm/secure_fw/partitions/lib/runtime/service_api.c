/*
 * Copyright (c) 2020-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "cmsis_compiler.h"
#include "service_api.h"
#include "psa/service.h"
#include "svc_num.h"
#include "utilities.h"

__attribute__((naked))
int32_t tfm_core_get_boot_data(uint8_t major_type,
                               struct tfm_boot_data *boot_status,
                               uint32_t len)
{
    __ASM volatile(
        "SVC    "M2S(TFM_SVC_GET_BOOT_DATA)"               \n"
        "BX     lr                                         \n"
        );
}

#if TFM_LVL != 1
/* Entry point when Partition FLIH functions return */
__attribute__((naked))
void tfm_flih_func_return(psa_flih_result_t result)
{
    __ASM volatile("SVC "M2S(TFM_SVC_FLIH_FUNC_RETURN)"           \n"
                   );
}
#endif /* TFM_LVL != 1 */
