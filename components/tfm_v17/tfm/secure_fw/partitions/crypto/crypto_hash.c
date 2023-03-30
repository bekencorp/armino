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
#if CRYPTO_HASH_MODULE_ENABLED
psa_status_t tfm_crypto_hash_interface(psa_invec in_vec[],
                                       psa_outvec out_vec[])
{
    const struct tfm_crypto_pack_iovec *iov = in_vec[0].base;
    psa_status_t status = PSA_ERROR_NOT_SUPPORTED;
    psa_hash_operation_t *operation = NULL;
    uint32_t *p_handle = NULL;
    uint16_t sid = iov->function_id;

    if (sid == TFM_CRYPTO_HASH_COMPUTE_SID) {
#if CRYPTO_SINGLE_PART_FUNCS_DISABLED
        return PSA_ERROR_NOT_SUPPORTED;
#else
        const uint8_t *input = in_vec[1].base;
        size_t input_length = in_vec[1].len;
        uint8_t *hash = out_vec[0].base;
        size_t hash_size = out_vec[0].len;

        status = psa_hash_compute(iov->alg, input, input_length,
                                  hash, hash_size, &out_vec[0].len);
        if (status != PSA_SUCCESS) {
            out_vec[0].len = 0;
        }
        return status;
#endif
    }

    if (sid == TFM_CRYPTO_HASH_COMPARE_SID) {
#if CRYPTO_SINGLE_PART_FUNCS_DISABLED
        return PSA_ERROR_NOT_SUPPORTED;
#else
        const uint8_t *input = in_vec[1].base;
        size_t input_length = in_vec[1].len;
        const uint8_t *hash = in_vec[2].base;
        size_t hash_length = in_vec[2].len;

        return psa_hash_compare(iov->alg, input, input_length,
                                hash, hash_length);
#endif
    }

    if (sid == TFM_CRYPTO_HASH_SETUP_SID) {
        p_handle = out_vec[0].base;
        *p_handle = iov->op_handle;
        status = tfm_crypto_operation_alloc(TFM_CRYPTO_HASH_OPERATION,
                                            out_vec[0].base,
                                            (void **)&operation);
    } else {
        status = tfm_crypto_operation_lookup(TFM_CRYPTO_HASH_OPERATION,
                                             iov->op_handle,
                                             (void **)&operation);
        if ((sid == TFM_CRYPTO_HASH_FINISH_SID) ||
            (sid == TFM_CRYPTO_HASH_VERIFY_SID) ||
            (sid == TFM_CRYPTO_HASH_ABORT_SID)) {
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
        if (sid == TFM_CRYPTO_HASH_ABORT_SID) {
            /*
             * Mbed TLS psa_hash_abort() will return a misleading error code
             * if it is called with invalid operation content, since it
             * doesn't validate the operation handle.
             * It is neither necessary to call tfm_crypto_operation_release()
             * with an invalid handle.
             * Therefore return PSA_SUCCESS directly as psa_hash_abort() can
             * be called multiple times.
             */
            return PSA_SUCCESS;
        }
        return status;
    }

    switch (sid) {
    case TFM_CRYPTO_HASH_SETUP_SID:
    {
        status = psa_hash_setup(operation, iov->alg);
        if (status != PSA_SUCCESS) {
            goto release_operation_and_return;
        }
    }
    break;
    case TFM_CRYPTO_HASH_UPDATE_SID:
    {
        const uint8_t *input = in_vec[1].base;
        size_t input_length = in_vec[1].len;

        return psa_hash_update(operation, input, input_length);
    }
    case TFM_CRYPTO_HASH_FINISH_SID:
    {
        uint8_t *hash = out_vec[1].base;
        size_t hash_size = out_vec[1].len;

        status = psa_hash_finish(operation, hash, hash_size, &out_vec[1].len);
        if (status == PSA_SUCCESS) {
            goto release_operation_and_return;
        } else {
            out_vec[1].len = 0;
        }
    }
    break;
    case TFM_CRYPTO_HASH_VERIFY_SID:
    {
        const uint8_t *hash = in_vec[1].base;
        size_t hash_length = in_vec[1].len;

        status = psa_hash_verify(operation, hash, hash_length);
        if (status == PSA_SUCCESS) {
            goto release_operation_and_return;
        }
    }
    break;
    case TFM_CRYPTO_HASH_ABORT_SID:
    {
        status = psa_hash_abort(operation);
        goto release_operation_and_return;
    }
    case TFM_CRYPTO_HASH_CLONE_SID:
    {
        psa_hash_operation_t *target_operation = NULL;
        p_handle = out_vec[0].base;
        *p_handle = *((uint32_t *)in_vec[1].base);

        /* Allocate the target operation context in the secure world */
        status = tfm_crypto_operation_alloc(TFM_CRYPTO_HASH_OPERATION,
                                            p_handle,
                                            (void **)&target_operation);
        if (status != PSA_SUCCESS) {
            return status;
        }
        status = psa_hash_clone(operation, target_operation);
        if (status != PSA_SUCCESS) {
            (void)tfm_crypto_operation_release(p_handle);
        }
    }
    break;
    default:
        return PSA_ERROR_NOT_SUPPORTED;
    }

    return status;

release_operation_and_return:
    /* Release the operation context, ignore if the release fails. */
    (void)tfm_crypto_operation_release(p_handle);
    return status;
}
#else /* CRYPTO_HASH_MODULE_ENABLED */
psa_status_t tfm_crypto_hash_interface(psa_invec in_vec[],
                                       psa_outvec out_vec[])
{
    (void)in_vec;
    (void)out_vec;

    return PSA_ERROR_NOT_SUPPORTED;
}
#endif /* CRYPTO_HASH_MODULE_ENABLED */
/*!@}*/
