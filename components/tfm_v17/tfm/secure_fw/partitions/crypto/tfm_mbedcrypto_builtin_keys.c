/*
 * Copyright (c) 2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "tfm_mbedcrypto_include.h"
#include "tfm_crypto_defs.h"
#include "tfm_plat_crypto_keys.h"

/* This function is required by MbedTLS to enable builtin key support, it maps
 * builtin key IDs to cryptographic drivers and slots. The actual data is
 * deferred to a platform function, as different platforms may have different
 * key storage capabilities.
 */
psa_status_t mbedtls_psa_platform_get_builtin_key(
    mbedtls_svc_key_id_t key_id,
    psa_key_lifetime_t *lifetime,
    psa_drv_slot_number_t *slot_number)
{
    enum tfm_plat_err_t plat_err;

    plat_err = tfm_plat_builtin_key_get_lifetime_and_slot(key_id, lifetime,
                                                          slot_number);
    if (plat_err != TFM_PLAT_ERR_SUCCESS) {
        return PSA_ERROR_DOES_NOT_EXIST;
    }

    return PSA_SUCCESS;
}
