/*
 * Copyright (c) 2018-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_CRYPTO_API_H__
#define __TFM_CRYPTO_API_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "tfm_crypto_defs.h"
#include "psa/crypto_client_struct.h"

/**
 * \brief List of possible operation types supported by the TFM based
 *        implementation. This type is needed by the operation allocation,
 *        lookup and release functions.
 *
 */
enum tfm_crypto_operation_type {
    TFM_CRYPTO_OPERATION_NONE = 0,
    TFM_CRYPTO_CIPHER_OPERATION = 1,
    TFM_CRYPTO_MAC_OPERATION = 2,
    TFM_CRYPTO_HASH_OPERATION = 3,
    TFM_CRYPTO_KEY_DERIVATION_OPERATION = 4,
    TFM_CRYPTO_AEAD_OPERATION = 5,

    /* Used to force the enum size */
    TFM_CRYPTO_OPERATION_TYPE_MAX = INT_MAX
};

/*
 * Macro to determine the group_id corresponding to a function_id by
 * accessing the tfm_crypto_func_sid table
 */
#define TFM_CRYPTO_GET_GROUP_ID(_function_id)    \
                        ((enum tfm_crypto_group_id)((_function_id) & 0xFF))

/**
 * \brief Initialise the service
 *
 * \return Return values as described in \ref psa_status_t
 */
psa_status_t tfm_crypto_init(void);

/**
 * \brief Initialise the Alloc module
 *
 * \return Return values as described in \ref psa_status_t
 */
psa_status_t tfm_crypto_init_alloc(void);

/**
 * \brief Returns the ID of the caller
 *
 * \param[out] id Pointer to hold the ID of the caller
 *
 * \return Return values as described in \ref psa_status_t
 */
psa_status_t tfm_crypto_get_caller_id(int32_t *id);

/**
 * \brief Gets key attributes from client key attributes.
 *
 * \param[in]  client_key_attr  Client key attributes
 * \param[in]  client_id        Partition ID of the calling client
 * \param[out] key_attributes   Key attributes
 *
 * \return Return values as described in \ref psa_status_t
 */
psa_status_t tfm_crypto_key_attributes_from_client(
                    const struct psa_client_key_attributes_s *client_key_attr,
                    int32_t client_id,
                    psa_key_attributes_t *key_attributes);

/**
 * \brief Converts key attributes to client key attributes.
 *
 * \param[in]  key_attributes   Key attributes
 * \param[out] client_key_attr  Client key attributes
 *
 * \return Return values as described in \ref psa_status_t
 */
psa_status_t tfm_crypto_key_attributes_to_client(
                        const psa_key_attributes_t *key_attributes,
                        struct psa_client_key_attributes_s *client_key_attr);

/**
 * \brief Allocate an operation context in the backend
 *
 * \param[in]  type   Type of the operation context to allocate
 * \param[out] handle Pointer to hold the allocated handle
 * \param[out  ctx    Double pointer to the corresponding context
 *
 * \return Return values as described in \ref psa_status_t
 */
psa_status_t tfm_crypto_operation_alloc(enum tfm_crypto_operation_type type,
                                        uint32_t *handle,
                                        void **ctx);
/**
 * \brief Release an operation context in the backend
 *
 * \param[in/out] handle Pointer to the handle of the context to release
 *
 * \return Return values as described in \ref psa_status_t
 */
psa_status_t tfm_crypto_operation_release(uint32_t *handle);
/**
 * \brief Look up an operation context in the backend for the corresponding
 *        frontend operation
 *
 * \param[in]  type   Type of the operation context to look up
 * \param[in]  handle Handle of the context to lookup
 * \param[out] ctx    Double pointer to the corresponding context
 *
 * \return Return values as described in \ref psa_status_t
 */
psa_status_t tfm_crypto_operation_lookup(enum tfm_crypto_operation_type type,
                                         uint32_t handle,
                                         void **ctx);

/**
 * \brief This function acts as interface from the framework dispatching
 *        calls to the set of functions that implement the PSA Crypto APIs.
 *        It is based on the Uniform Signatures prototype.
 *
 * \param[in]  in_vec   Array of invec parameters
 * \param[in]  in_len   Length of the valid entries in in_vec
 * \param[out] out_vec  Array of outvec parameters
 * \param[in]  out_len  Length of the valid entries in out_vec
 *
 * \return Return values as described in \ref psa_status_t
 */
psa_status_t tfm_crypto_api_dispatcher(psa_invec in_vec[],
                                       size_t in_len,
                                       psa_outvec out_vec[],
                                       size_t out_len);
/**
 * \brief This function acts as interface for the Key management module
 *
 * \param[in]  in_vec   Array of invec parameters
 * \param[out] out_vec  Array of outvec parameters
 * \param[in]  encoded_key Key encoded with partition_id and key_id
 *
 * \return Return values as described in \ref psa_status_t
 */
psa_status_t tfm_crypto_key_management_interface(psa_invec in_vec[],
                                            psa_outvec out_vec[],
                                            mbedtls_svc_key_id_t *encoded_key);
/**
 * \brief This function acts as interface for the MAC module
 *
 * \param[in]  in_vec   Array of invec parameters
 * \param[out] out_vec  Array of outvec parameters
 * \param[in]  encoded_key Key encoded with partition_id and key_id
 *
 * \return Return values as described in \ref psa_status_t
 */
psa_status_t tfm_crypto_mac_interface(psa_invec in_vec[],
                                      psa_outvec out_vec[],
                                      mbedtls_svc_key_id_t *encoded_key);
/**
 * \brief This function acts as interface for the Cipher module
 *
 * \param[in]  in_vec   Array of invec parameters
 * \param[out] out_vec  Array of outvec parameters
 * \param[in]  encoded_key Key encoded with partition_id and key_id
 *
 * \return Return values as described in \ref psa_status_t
 */
psa_status_t tfm_crypto_cipher_interface(psa_invec in_vec[],
                                         psa_outvec out_vec[],
                                         mbedtls_svc_key_id_t *encoded_key);
/**
 * \brief This function acts as interface for the AEAD module
 *
 * \param[in]  in_vec   Array of invec parameters
 * \param[out] out_vec  Array of outvec parameters
 * \param[in]  encoded_key Key encoded with partition_id and key_id
 *
 * \return Return values as described in \ref psa_status_t
 */
psa_status_t tfm_crypto_aead_interface(psa_invec in_vec[],
                                       psa_outvec out_vec[],
                                       mbedtls_svc_key_id_t *encoded_key);

/**
 * \brief This function acts as interface for the Asymmetric signing module
 *
 * \param[in]  in_vec   Array of invec parameters
 * \param[out] out_vec  Array of outvec parameters
 * \param[in]  encoded_key Key encoded with partition_id and key_id
 *
 * \return Return values as described in \ref psa_status_t
 */
psa_status_t tfm_crypto_asymmetric_sign_interface(psa_invec in_vec[],
                                                  psa_outvec out_vec[],
                                             mbedtls_svc_key_id_t *encoded_key);

/**
 * \brief This function acts as interface for the Asymmetric encryption module
 *
 * \param[in]  in_vec   Array of invec parameters
 * \param[out] out_vec  Array of outvec parameters
 * \param[in]  encoded_key Key encoded with partition_id and key_id
 *
 * \return Return values as described in \ref psa_status_t
 */
psa_status_t tfm_crypto_asymmetric_encrypt_interface(psa_invec in_vec[],
                                                     psa_outvec out_vec[],
                                             mbedtls_svc_key_id_t *encoded_key);

/**
 * \brief This function acts as interface for the Key derivation module
 *
 * \param[in]  in_vec   Array of invec parameters
 * \param[out] out_vec  Array of outvec parameters
 * \param[in]  encoded_key Key encoded with partition_id and key_id
 *
 * \return Return values as described in \ref psa_status_t
 */
psa_status_t tfm_crypto_key_derivation_interface(psa_invec in_vec[],
                                            psa_outvec out_vec[],
                                            mbedtls_svc_key_id_t *encoded_key);
/**
 * \brief This function acts as interface for the Random module
 *
 * \param[in]  in_vec   Array of invec parameters
 * \param[out] out_vec  Array of outvec parameters
 *
 * \return Return values as described in \ref psa_status_t
 */
psa_status_t tfm_crypto_random_interface(psa_invec in_vec[],
                                         psa_outvec out_vec[]);
/**
 * \brief This function acts as interface for the Hash module
 *
 * \param[in]  in_vec   Array of invec parameters
 * \param[out] out_vec  Array of outvec parameters
 *
 * \return Return values as described in \ref psa_status_t
 */
psa_status_t tfm_crypto_hash_interface(psa_invec in_vec[],
                                       psa_outvec out_vec[]);

#ifdef __cplusplus
}
#endif

#endif /* __TFM_CRYPTO_API_H__ */
