/*
 * Copyright (c) 2019-2022, Arm Limited. All rights reserved.
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
#if CRYPTO_ASYM_SIGN_MODULE_ENABLED
psa_status_t tfm_crypto_asymmetric_sign_interface(psa_invec in_vec[],
                                                  psa_outvec out_vec[],
                                             mbedtls_svc_key_id_t *encoded_key)
{
    const struct tfm_crypto_pack_iovec *iov = in_vec[0].base;
    psa_status_t status = PSA_ERROR_NOT_SUPPORTED;

    switch (iov->function_id) {
    case TFM_CRYPTO_ASYMMETRIC_SIGN_MESSAGE_SID:
    {
        const uint8_t *input = in_vec[1].base;
        size_t input_length = in_vec[1].len;
        uint8_t *signature = out_vec[0].base;
        size_t signature_size = out_vec[0].len;

        status = psa_sign_message(*encoded_key, iov->alg, input, input_length,
                                  signature, signature_size, &(out_vec[0].len));
        if (status != PSA_SUCCESS) {
            out_vec[0].len = 0;
        }
        return status;
    }
    case TFM_CRYPTO_ASYMMETRIC_VERIFY_MESSAGE_SID:
    {
        const uint8_t *input = in_vec[1].base;
        size_t input_length = in_vec[1].len;
        const uint8_t *signature = in_vec[2].base;
        size_t signature_length = in_vec[2].len;

        return psa_verify_message(*encoded_key, iov->alg, input, input_length,
                                  signature, signature_length);
    }
    case TFM_CRYPTO_ASYMMETRIC_SIGN_HASH_SID:
    {
        const uint8_t *hash = in_vec[1].base;
        size_t hash_length = in_vec[1].len;
        uint8_t *signature = out_vec[0].base;
        size_t signature_size = out_vec[0].len;

        status = psa_sign_hash(*encoded_key, iov->alg, hash, hash_length,
                               signature, signature_size, &(out_vec[0].len));
        if (status != PSA_SUCCESS) {
            out_vec[0].len = 0;
        }
        return status;
    }
    case TFM_CRYPTO_ASYMMETRIC_VERIFY_HASH_SID:
    {
        const uint8_t *hash = in_vec[1].base;
        size_t hash_length = in_vec[1].len;
        const uint8_t *signature = in_vec[2].base;
        size_t signature_length = in_vec[2].len;

        return psa_verify_hash(*encoded_key, iov->alg, hash, hash_length,
                               signature, signature_length);
    }
    default:
        return PSA_ERROR_NOT_SUPPORTED;
    }

    return PSA_ERROR_NOT_SUPPORTED;
}
#else /* CRYPTO_ASYM_SIGN_MODULE_ENABLED */
psa_status_t tfm_crypto_asymmetric_sign_interface(psa_invec in_vec[],
                                                  psa_outvec out_vec[],
                                             mbedtls_svc_key_id_t *encoded_key)
{
    (void)in_vec;
    (void)out_vec;
    (void)encoded_key;

    return PSA_ERROR_NOT_SUPPORTED;
}
#endif /* CRYPTO_ASYM_SIGN_MODULE_ENABLED */

#if CRYPTO_ASYM_ENCRYPT_MODULE_ENABLED
psa_status_t tfm_crypto_asymmetric_encrypt_interface(psa_invec in_vec[],
                                                     psa_outvec out_vec[],
                                             mbedtls_svc_key_id_t *encoded_key)
{
    const struct tfm_crypto_pack_iovec *iov = in_vec[0].base;
    psa_status_t status = PSA_ERROR_NOT_SUPPORTED;

    switch (iov->function_id) {
    case TFM_CRYPTO_ASYMMETRIC_ENCRYPT_SID:
    {
        const uint8_t *input = in_vec[1].base;
        size_t input_length = in_vec[1].len;
        const uint8_t *salt = in_vec[2].base;
        size_t salt_length = in_vec[2].len;
        uint8_t *output = out_vec[0].base;
        size_t output_size = out_vec[0].len;

        status = psa_asymmetric_encrypt(*encoded_key, iov->alg,
                                        input, input_length,
                                        salt, salt_length,
                                        output, output_size,
                                        &(out_vec[0].len));
        if (status != PSA_SUCCESS) {
            out_vec[0].len = 0;
        }
        return status;
    }
    case TFM_CRYPTO_ASYMMETRIC_DECRYPT_SID:
    {
        const uint8_t *input = in_vec[1].base;
        size_t input_length = in_vec[1].len;
        const uint8_t *salt = in_vec[2].base;
        size_t salt_length = in_vec[2].len;
        uint8_t *output = out_vec[0].base;
        size_t output_size = out_vec[0].len;

        status = psa_asymmetric_decrypt(*encoded_key, iov->alg,
                                        input, input_length,
                                        salt, salt_length,
                                        output, output_size,
                                        &(out_vec[0].len));
        if (status != PSA_SUCCESS) {
            out_vec[0].len = 0;
        }
        return status;
    }
    default:
        return PSA_ERROR_NOT_SUPPORTED;
    }

    return PSA_ERROR_NOT_SUPPORTED;
}
#else /* CRYPTO_ASYM_ENCRYPT_MODULE_ENABLED */
psa_status_t tfm_crypto_asymmetric_encrypt_interface(psa_invec in_vec[],
                                                     psa_outvec out_vec[],
                                             mbedtls_svc_key_id_t *encoded_key)
{
    (void)in_vec;
    (void)out_vec;
    (void)encoded_key;

    return PSA_ERROR_NOT_SUPPORTED;
}
#endif /* CRYPTO_ASYM_ENCRYPT_MODULE_ENABLED */
/*!@}*/
