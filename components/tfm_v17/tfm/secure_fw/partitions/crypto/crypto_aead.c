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
 * \defgroup tfm_crypto_api_shim_layer Set of functions implementing a thin shim
 *                                  layer between the TF-M Crypto service
 *                                  frontend and the underlying library which
 *                                  implements the PSA Crypto APIs
 *                                  (i.e. mbed TLS)
 */

/*!@{*/
#if CRYPTO_AEAD_MODULE_ENABLED
psa_status_t tfm_crypto_aead_interface(psa_invec in_vec[],
                                       psa_outvec out_vec[],
                                       mbedtls_svc_key_id_t *encoded_key)
{
    const struct tfm_crypto_pack_iovec *iov = in_vec[0].base;
    psa_status_t status = PSA_ERROR_NOT_SUPPORTED;
    psa_aead_operation_t *operation = NULL;
    uint32_t *p_handle = NULL;
    uint16_t sid = iov->function_id;

    if (sid == TFM_CRYPTO_AEAD_ENCRYPT_SID) {
#if CRYPTO_SINGLE_PART_FUNCS_DISABLED
        return PSA_ERROR_NOT_SUPPORTED;
#else
        const struct tfm_crypto_aead_pack_input *aead_pack_input =
                                                                 &iov->aead_in;
        const uint8_t *nonce = aead_pack_input->nonce;
        size_t nonce_length = aead_pack_input->nonce_length;
        const uint8_t *plaintext = in_vec[1].base;
        size_t plaintext_length = in_vec[1].len;
        uint8_t *ciphertext = out_vec[0].base;
        size_t ciphertext_size = out_vec[0].len;
        const uint8_t *additional_data = in_vec[2].base;
        size_t additional_data_length = in_vec[2].len;

        status = psa_aead_encrypt(*encoded_key, iov->alg, nonce, nonce_length,
                                  additional_data, additional_data_length,
                                  plaintext, plaintext_length,
                                  ciphertext, ciphertext_size, &out_vec[0].len);
        if (status != PSA_SUCCESS) {
            out_vec[0].len = 0;
        }
        return status;
#endif
    }

    if (sid == TFM_CRYPTO_AEAD_DECRYPT_SID) {
#if CRYPTO_SINGLE_PART_FUNCS_DISABLED
        return PSA_ERROR_NOT_SUPPORTED;
#else
        const struct tfm_crypto_aead_pack_input *aead_pack_input =
                                                                 &iov->aead_in;
        const uint8_t *nonce = aead_pack_input->nonce;
        size_t nonce_length = aead_pack_input->nonce_length;
        const uint8_t *ciphertext = in_vec[1].base;
        size_t ciphertext_length = in_vec[1].len;
        uint8_t *plaintext = out_vec[0].base;
        size_t plaintext_size = out_vec[0].len;
        const uint8_t *additional_data = in_vec[2].base;
        size_t additional_data_length = in_vec[2].len;

        status = psa_aead_decrypt(*encoded_key, iov->alg, nonce, nonce_length,
                                  additional_data, additional_data_length,
                                  ciphertext, ciphertext_length,
                                  plaintext, plaintext_size, &out_vec[0].len);
        if (status != PSA_SUCCESS) {
            out_vec[0].len = 0;
        }
        return status;
#endif
    }

    if ((sid == TFM_CRYPTO_AEAD_ENCRYPT_SETUP_SID) ||
        (sid == TFM_CRYPTO_AEAD_DECRYPT_SETUP_SID)) {
        p_handle = out_vec[0].base;
        *p_handle = iov->op_handle;
        status = tfm_crypto_operation_alloc(TFM_CRYPTO_AEAD_OPERATION,
                                            out_vec[0].base,
                                            (void **)&operation);
    } else {
        status = tfm_crypto_operation_lookup(TFM_CRYPTO_AEAD_OPERATION,
                                             iov->op_handle,
                                             (void **)&operation);
        if ((sid == TFM_CRYPTO_AEAD_FINISH_SID) ||
            (sid == TFM_CRYPTO_AEAD_VERIFY_SID) ||
            (sid == TFM_CRYPTO_AEAD_ABORT_SID)) {
            /*
             * finish()/abort() interface put handle pointer in out_vec[0].
             * Therefore, out_vec[0] shall be specially set to original handle
             * value. Otherwise, later psa_write() may override the original
             * handle value in client with garbage data in message out_vec[0],
             * if lookup fails.
             */
            p_handle = out_vec[0].base;
            *p_handle = iov->op_handle;
        }
    }
    if (status != PSA_SUCCESS) {
        if (sid == TFM_CRYPTO_AEAD_ABORT_SID) {
            /*
             * Mbed TLS psa_aead_abort() will return a misleading error code
             * if it is called with invalid operation content, since it
             * doesn't validate the operation handle.
             * It is neither necessary to call tfm_crypto_operation_release()
             * with an invalid handle.
             * Therefore return PSA_SUCCESS directly as psa_aead_abort() can be
             * called multiple times.
             */
            return PSA_SUCCESS;
        }
        return status;
    }

    switch (sid) {
    case TFM_CRYPTO_AEAD_ENCRYPT_SETUP_SID:
    {
        status = psa_aead_encrypt_setup(operation, *encoded_key, iov->alg);
        if (status != PSA_SUCCESS) {
            goto release_operation_and_return;
        }
    }
    break;
    case TFM_CRYPTO_AEAD_DECRYPT_SETUP_SID:
    {
        status = psa_aead_decrypt_setup(operation, *encoded_key, iov->alg);
        if (status != PSA_SUCCESS) {
            goto release_operation_and_return;
        }
    }
    break;
    case TFM_CRYPTO_AEAD_FINISH_SID:
    {
        uint8_t *ciphertext = out_vec[2].base;
        size_t ciphertext_size = out_vec[2].len;
        uint8_t *tag = out_vec[1].base;
        size_t tag_size = out_vec[1].len;

        status = psa_aead_finish(operation,
                                 ciphertext, ciphertext_size, &out_vec[2].len,
                                 tag, tag_size, &out_vec[1].len);
        if (status == PSA_SUCCESS) {
            goto release_operation_and_return;
        } else {
            out_vec[1].len = 0;
            out_vec[2].len = 0;
        }
    }
    break;
    case TFM_CRYPTO_AEAD_ABORT_SID:
    {
        status = psa_aead_abort(operation);
        goto release_operation_and_return;
    }
    case TFM_CRYPTO_AEAD_GENERATE_NONCE_SID:
    {
        uint8_t *nonce = out_vec[0].base;
        size_t nonce_size = out_vec[0].len;

        status = psa_aead_generate_nonce(operation,
                                         nonce,
                                         nonce_size,
                                         &out_vec[0].len);
        if (status != PSA_SUCCESS) {
            out_vec[0].len = 0;
        }
        return status;
    }
    case TFM_CRYPTO_AEAD_SET_NONCE_SID:
    {
        const uint8_t *nonce = in_vec[1].base;
        size_t nonce_size = in_vec[1].len;

        return psa_aead_set_nonce(operation, nonce, nonce_size);
    }
    case TFM_CRYPTO_AEAD_SET_LENGTHS_SID:
    {
        return psa_aead_set_lengths(operation, iov->ad_length,
                                    iov->plaintext_length);
    }
    case TFM_CRYPTO_AEAD_UPDATE_SID:
    {
        const uint8_t *input = in_vec[1].base;
        size_t input_length = in_vec[1].len;
        uint8_t *output = out_vec[0].base;
        size_t output_size = out_vec[0].len;

        status = psa_aead_update(operation, input, input_length,
                                 output, output_size, &out_vec[0].len);
        if (status != PSA_SUCCESS) {
            out_vec[0].len = 0;
        }
        return status;
    }
    case TFM_CRYPTO_AEAD_UPDATE_AD_SID:
    {
        const uint8_t *input = in_vec[1].base;
        size_t input_length = in_vec[1].len;

        return psa_aead_update_ad(operation, input, input_length);
    }
    case TFM_CRYPTO_AEAD_VERIFY_SID:
    {
        const uint8_t *tag = in_vec[1].base;
        size_t tag_length = in_vec[1].len;
        uint8_t *plaintext = out_vec[1].base;
        size_t plaintext_size = out_vec[1].len;

        status = psa_aead_verify(operation,
                                 plaintext, plaintext_size, &out_vec[1].len,
                                 tag, tag_length);
        if (status == PSA_SUCCESS) {
            goto release_operation_and_return;
        } else {
            out_vec[1].len = 0;
        }
    }
    break;
    default:
        return PSA_ERROR_NOT_SUPPORTED;
    }

    return status;

release_operation_and_return:
    /* Release the operation context, ignore if the operation fails. */
    (void)tfm_crypto_operation_release(p_handle);
    return status;
}
#else /* CRYPTO_AEAD_MODULE_ENABLED */
psa_status_t tfm_crypto_aead_interface(psa_invec in_vec[],
                                       psa_outvec out_vec[],
                                       mbedtls_svc_key_id_t *encoded_key)
{
    (void)in_vec;
    (void)out_vec;
    (void)encoded_key;

    return PSA_ERROR_NOT_SUPPORTED;
}
#endif /* CRYPTO_AEAD_MODULE_ENABLED */
/*!@}*/
