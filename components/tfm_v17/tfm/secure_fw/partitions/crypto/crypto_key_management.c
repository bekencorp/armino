/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
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

#ifndef MBEDTLS_PSA_CRYPTO_KEY_ID_ENCODES_OWNER
#error "MBEDTLS_PSA_CRYPTO_KEY_ID_ENCODES_OWNER must be selected in Mbed TLS config file"
#endif

/*!
 * \addtogroup tfm_crypto_api_shim_layer
 *
 */

/*!@{*/
#if CRYPTO_KEY_MODULE_ENABLED
psa_status_t tfm_crypto_key_management_interface(psa_invec in_vec[],
                                            psa_outvec out_vec[],
                                            mbedtls_svc_key_id_t *encoded_key)
{
    const struct tfm_crypto_pack_iovec *iov = in_vec[0].base;
    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;
    int32_t partition_id = MBEDTLS_SVC_KEY_ID_GET_OWNER_ID(*encoded_key);

    switch (iov->function_id) {
    case TFM_CRYPTO_IMPORT_KEY_SID:
    {
        const struct psa_client_key_attributes_s *client_key_attr =
                                                                 in_vec[1].base;
        const uint8_t *data = in_vec[2].base;
        size_t data_length = in_vec[2].len;
        psa_key_id_t *key_id = out_vec[0].base;
        psa_key_attributes_t key_attributes = PSA_KEY_ATTRIBUTES_INIT;

        status = tfm_crypto_key_attributes_from_client(client_key_attr,
                                                       partition_id,
                                                       &key_attributes);
        if (status != PSA_SUCCESS) {
            return status;
        }

        status = psa_import_key(&key_attributes,
                                data, data_length, encoded_key);
        /* Update the imported key id */
        *key_id = MBEDTLS_SVC_KEY_ID_GET_KEY_ID(*encoded_key);
    }
    break;
    case TFM_CRYPTO_OPEN_KEY_SID:
    {
        psa_key_id_t *key_id = out_vec[0].base;

        status = psa_open_key(*encoded_key, encoded_key);
        *key_id = MBEDTLS_SVC_KEY_ID_GET_KEY_ID(*encoded_key);
    }
    break;
    case TFM_CRYPTO_CLOSE_KEY_SID:
    {
        status = psa_close_key(*encoded_key);
    }
    break;
    case TFM_CRYPTO_DESTROY_KEY_SID:
    {
        status = psa_destroy_key(*encoded_key);
    }
    break;
    case TFM_CRYPTO_GET_KEY_ATTRIBUTES_SID:
    {
        psa_key_attributes_t key_attributes = PSA_KEY_ATTRIBUTES_INIT;
        struct psa_client_key_attributes_s *client_key_attr = out_vec[0].base;

        status = psa_get_key_attributes(*encoded_key, &key_attributes);
        if (status == PSA_SUCCESS) {
            status = tfm_crypto_key_attributes_to_client(&key_attributes,
                                                         client_key_attr);
        }
    }
    break;
    case TFM_CRYPTO_RESET_KEY_ATTRIBUTES_SID:
    {
        psa_key_attributes_t key_attributes = PSA_KEY_ATTRIBUTES_INIT;
        struct psa_client_key_attributes_s *client_key_attr = out_vec[0].base;

        status = tfm_crypto_key_attributes_from_client(client_key_attr,
                                                       partition_id,
                                                       &key_attributes);
        if (status != PSA_SUCCESS) {
            return status;
        }

        psa_reset_key_attributes(&key_attributes);

        status = tfm_crypto_key_attributes_to_client(&key_attributes,
                                                     client_key_attr);
    }
    break;
    case TFM_CRYPTO_EXPORT_KEY_SID:
    {
        uint8_t *data = out_vec[0].base;
        size_t data_size = out_vec[0].len;

        status = psa_export_key(*encoded_key, data, data_size,
                                &(out_vec[0].len));
        if (status != PSA_SUCCESS) {
            out_vec[0].len = 0;
        }
    }
    break;
    case TFM_CRYPTO_EXPORT_PUBLIC_KEY_SID:
    {
        uint8_t *data = out_vec[0].base;
        size_t data_size = out_vec[0].len;

        status = psa_export_public_key(*encoded_key, data, data_size,
                                       &(out_vec[0].len));
        if (status != PSA_SUCCESS) {
            out_vec[0].len = 0;
        }
    }
    break;
    case TFM_CRYPTO_PURGE_KEY_SID:
    {
        status = psa_purge_key(*encoded_key);
    }
    break;
    case TFM_CRYPTO_COPY_KEY_SID:
    {
        psa_key_attributes_t key_attributes = PSA_KEY_ATTRIBUTES_INIT;
        const struct psa_client_key_attributes_s *client_key_attr =
                                                                 in_vec[1].base;
        psa_key_id_t *target_key_id = out_vec[0].base;
        mbedtls_svc_key_id_t target_key = MBEDTLS_SVC_KEY_ID_INIT;

        status = tfm_crypto_key_attributes_from_client(client_key_attr,
                                                       partition_id,
                                                       &key_attributes);
        if (status != PSA_SUCCESS) {
            return status;
        }

        status = psa_copy_key(*encoded_key, &key_attributes, &target_key);
        if (status != PSA_SUCCESS) {
            return status;
        }

        *target_key_id = MBEDTLS_SVC_KEY_ID_GET_KEY_ID(target_key);
    }
    break;
    case TFM_CRYPTO_GENERATE_KEY_SID:
    {
        psa_key_attributes_t key_attributes = PSA_KEY_ATTRIBUTES_INIT;
        const struct psa_client_key_attributes_s *client_key_attr =
                                                                 in_vec[1].base;
        psa_key_id_t *key_handle = out_vec[0].base;

        status = tfm_crypto_key_attributes_from_client(client_key_attr,
                                                       partition_id,
                                                       &key_attributes);
        if (status != PSA_SUCCESS) {
            return status;
        }

        status = psa_generate_key(&key_attributes, encoded_key);
        if (status != PSA_SUCCESS) {
            return status;
        }

        *key_handle = MBEDTLS_SVC_KEY_ID_GET_KEY_ID(*encoded_key);
    }
    break;
    default:
        return PSA_ERROR_NOT_SUPPORTED;
    }

    return status;
}
#else /* CRYPTO_KEY_MODULE_ENABLED  */
psa_status_t tfm_crypto_key_management_interface(psa_invec in_vec[],
                                            psa_outvec out_vec[],
                                            mbedtls_svc_key_id_t *encoded_key)
{
    (void)in_vec;
    (void)out_vec;
    (void)encoded_key;

    return PSA_ERROR_NOT_SUPPORTED;
}
#endif /* CRYPTO_KEY_MODULE_ENABLED  */
/*!@}*/
