/*
 * Copyright (c) 2018-2022, Arm Limited. All rights reserved.
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
#if CRYPTO_CIPHER_MODULE_ENABLED
psa_status_t tfm_crypto_cipher_interface(psa_invec in_vec[],
                                         psa_outvec out_vec[],
                                         mbedtls_svc_key_id_t *encoded_key)
{
    const struct tfm_crypto_pack_iovec *iov = in_vec[0].base;
    psa_status_t status = PSA_ERROR_NOT_SUPPORTED;
    psa_cipher_operation_t *operation = NULL;
    uint32_t *p_handle = NULL;
    uint16_t sid = iov->function_id;

    if (sid == TFM_CRYPTO_CIPHER_ENCRYPT_SID) {
#if CRYPTO_SINGLE_PART_FUNCS_DISABLED
        return PSA_ERROR_NOT_SUPPORTED;
#else
        const uint8_t *input = in_vec[1].base;
        size_t input_length = in_vec[1].len;
        uint8_t *output = out_vec[0].base;
        size_t output_size = out_vec[0].len;

        status = psa_cipher_encrypt(*encoded_key, iov->alg, input, input_length,
                                    output, output_size, &out_vec[0].len);
        if (status != PSA_SUCCESS) {
            out_vec[0].len = 0;
        }
        return status;
#endif
    }

    if (sid == TFM_CRYPTO_CIPHER_DECRYPT_SID) {
#if CRYPTO_SINGLE_PART_FUNCS_DISABLED
        return PSA_ERROR_NOT_SUPPORTED;
#else
        const uint8_t *input = in_vec[1].base;
        size_t input_length = in_vec[1].len;
        uint8_t *output = out_vec[0].base;
        size_t output_size = out_vec[0].len;

        status = psa_cipher_decrypt(*encoded_key, iov->alg, input, input_length,
                                    output, output_size, &out_vec[0].len);
        if (status != PSA_SUCCESS) {
            out_vec[0].len = 0;
        }
        return status;
#endif
    }

    if ((sid == TFM_CRYPTO_CIPHER_ENCRYPT_SETUP_SID) ||
        (sid == TFM_CRYPTO_CIPHER_DECRYPT_SETUP_SID)) {
        p_handle = out_vec[0].base;
        *p_handle = iov->op_handle;
        status = tfm_crypto_operation_alloc(TFM_CRYPTO_CIPHER_OPERATION,
                                            out_vec[0].base,
                                            (void **)&operation);
    } else {
        status = tfm_crypto_operation_lookup(TFM_CRYPTO_CIPHER_OPERATION,
                                             iov->op_handle,
                                             (void **)&operation);
        if ((sid == TFM_CRYPTO_CIPHER_FINISH_SID) ||
            (sid == TFM_CRYPTO_CIPHER_ABORT_SID)) {
            /*
             * finish()/abort() interface put handle in out_vec[0].
             * Therefore, out_vec[0] shall be specially set to original handle
             * value. Otherwise, the garbage data in message out_vec[0] may
             * override the original handle value in client, after lookup fails.
             */
            p_handle = out_vec[0].base;
            *p_handle = iov->op_handle;
        }
    }
    if (status != PSA_SUCCESS) {
        if (sid == TFM_CRYPTO_CIPHER_ABORT_SID) {
            /*
             * Mbed TLS psa_cipher_abort() will return a misleading error code
             * if it is called with invalid operation content, since it
             * doesn't validate the operation handle.
             * It is neither necessary to call tfm_crypto_operation_release()
             * with an invalid handle.
             * Therefore return PSA_SUCCESS directly as psa_cipher_abort() can
             * be called multiple times.
             */
            return PSA_SUCCESS;
        }
        return status;
    }

    switch (sid) {
    case TFM_CRYPTO_CIPHER_GENERATE_IV_SID:
    {
        unsigned char *iv = out_vec[0].base;
        size_t iv_size = out_vec[0].len;

        status = psa_cipher_generate_iv(operation, iv, iv_size, &out_vec[0].len);
        if (status != PSA_SUCCESS) {
            out_vec[0].len = 0;
        }
        return status;
    }
    case TFM_CRYPTO_CIPHER_SET_IV_SID:
    {
        const unsigned char *iv = in_vec[1].base;
        size_t iv_length = in_vec[1].len;

        return psa_cipher_set_iv(operation, iv, iv_length);
    }
    case TFM_CRYPTO_CIPHER_ENCRYPT_SETUP_SID:
    {
        status = psa_cipher_encrypt_setup(operation, *encoded_key, iov->alg);
        if (status != PSA_SUCCESS) {
            goto release_operation_and_return;
        }
    }
    break;
    case TFM_CRYPTO_CIPHER_DECRYPT_SETUP_SID:
    {
        status = psa_cipher_decrypt_setup(operation, *encoded_key, iov->alg);
        if (status != PSA_SUCCESS) {
            goto release_operation_and_return;
        }
    }
    break;
    case TFM_CRYPTO_CIPHER_UPDATE_SID:
    {
        const uint8_t *input = in_vec[1].base;
        size_t input_length = in_vec[1].len;
        unsigned char *output = out_vec[0].base;
        size_t output_size = out_vec[0].len;

        status = psa_cipher_update(operation, input, input_length,
                                   output, output_size, &out_vec[0].len);
        if (status != PSA_SUCCESS) {
            out_vec[0].len = 0;
        }
        return status;
    }
    case TFM_CRYPTO_CIPHER_FINISH_SID:
    {
        uint8_t *output = out_vec[1].base;
        size_t output_size = out_vec[1].len;

        status = psa_cipher_finish(operation,
                                   output, output_size, &out_vec[1].len);
        if (status == PSA_SUCCESS) {
            /* In case of success automatically release the operation */
            goto release_operation_and_return;
        } else {
            out_vec[1].len = 0;
        }
    }
    break;
    case TFM_CRYPTO_CIPHER_ABORT_SID:
    {
        status = psa_cipher_abort(operation);
        goto release_operation_and_return;
    }
    default:
        return PSA_ERROR_NOT_SUPPORTED;
    }

    return status;

release_operation_and_return:
    /* Release the operation context, ignore if the operation fails. */
    (void)tfm_crypto_operation_release(p_handle);
    return status;
}
#else /* CRYPTO_CIPHER_MODULE_ENABLED */
psa_status_t tfm_crypto_cipher_interface(psa_invec in_vec[],
                                         psa_outvec out_vec[],
                                         mbedtls_svc_key_id_t *encoded_key)
{
    (void)in_vec;
    (void)out_vec;
    (void)encoded_key;

    return PSA_ERROR_NOT_SUPPORTED;
}
#endif /* CRYPTO_CIPHER_MODULE_ENABLED */
/*!@}*/
