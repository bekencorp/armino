/*
 * Copyright (c) 2019-2022, Arm Limited. All rights reserved.
 * Copyright (c) 2021, Nordic Semiconductor ASA.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stddef.h>
#include <stdint.h>

#include "config_crypto.h"
#include "tfm_mbedcrypto_include.h"

#include "tfm_crypto_api.h"
#include "tfm_crypto_defs.h"

/*!
 * \addtogroup tfm_crypto_api_shim_layer
 *
 */

/*!@{*/
psa_status_t tfm_crypto_random_interface(psa_invec in_vec[],
                                         psa_outvec out_vec[])
{
#if !CRYPTO_RNG_MODULE_ENABLED
    (void)in_vec;
    (void)out_vec;

    return PSA_ERROR_NOT_SUPPORTED;
#else
    uint8_t *output = out_vec[0].base;
    size_t output_size = out_vec[0].len;

    return psa_generate_random(output, output_size);
#endif
}
/*!@}*/
