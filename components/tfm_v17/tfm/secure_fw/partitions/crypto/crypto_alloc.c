/*
 * Copyright (c) 2018-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "config_crypto.h"
#include "tfm_mbedcrypto_include.h"

#include "tfm_crypto_api.h"
#include "tfm_crypto_defs.h"


struct tfm_crypto_operation_s {
    uint32_t in_use;                /*!< Indicates if the operation is in use */
    int32_t owner;                  /*!< Indicates an ID of the owner of
                                     *   the context
                                     */
    enum tfm_crypto_operation_type type; /*!< Type of the operation */
    union {
        psa_cipher_operation_t cipher;    /*!< Cipher operation context */
        psa_mac_operation_t mac;          /*!< MAC operation context */
        psa_hash_operation_t hash;        /*!< Hash operation context */
        psa_key_derivation_operation_t key_deriv; /*!< Key derivation operation context */
        psa_aead_operation_t aead;        /*!< AEAD operation context */
    } operation;
};

static struct tfm_crypto_operation_s operations[CRYPTO_CONC_OPER_NUM] = {{0}};

/*
 * \brief Function used to clear the memory associated to a backend context
 *
 * \param[in] index Numerical index in the database of the backend contexts
 *
 * \return None
 *
 */
static void memset_operation_context(uint32_t index)
{
    /* Clear the contents of the backend context */
    (void)memset((uint8_t *)&(operations[index].operation), 0,
                 sizeof(operations[index].operation));
}

/*!
 * \defgroup alloc Function that implement allocation and deallocation of
 *                 contexts to be stored in the secure world for multipart
 *                 operations
 */

/*!@{*/
psa_status_t tfm_crypto_init_alloc(void)
{
    /* Clear the contents of the local contexts */
    (void)memset(operations, 0, sizeof(operations));
    return PSA_SUCCESS;
}

psa_status_t tfm_crypto_operation_alloc(enum tfm_crypto_operation_type type,
                                        uint32_t *handle,
                                        void **ctx)
{
    uint32_t i = 0;
    int32_t partition_id = 0;
    psa_status_t status;

    /* Handle must be initialised before calling a setup function */
    if (*handle != TFM_CRYPTO_INVALID_HANDLE) {
        return PSA_ERROR_BAD_STATE;
    }

    /* Init to invalid values */
    if (ctx == NULL) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }
    *ctx = NULL;

    status = tfm_crypto_get_caller_id(&partition_id);
    if (status != PSA_SUCCESS) {
        return status;
    }

    for (i = 0; i < CRYPTO_CONC_OPER_NUM; i++) {
        if (operations[i].in_use == TFM_CRYPTO_NOT_IN_USE) {
            operations[i].in_use = TFM_CRYPTO_IN_USE;
            operations[i].owner = partition_id;
            operations[i].type = type;
            *handle = i + 1;
            *ctx = (void *) &(operations[i].operation);
            return PSA_SUCCESS;
        }
    }

    return PSA_ERROR_NOT_PERMITTED;
}

psa_status_t tfm_crypto_operation_release(uint32_t *handle)
{
    uint32_t h_val = *handle;
    int32_t partition_id = 0;
    psa_status_t status;

    /* Handle shall be cleaned up always at first */
    *handle = TFM_CRYPTO_INVALID_HANDLE;

    if ((h_val == TFM_CRYPTO_INVALID_HANDLE) ||
        (h_val > CRYPTO_CONC_OPER_NUM)) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    status = tfm_crypto_get_caller_id(&partition_id);
    if (status != PSA_SUCCESS) {
        return status;
    }

    if ((operations[h_val - 1].in_use == TFM_CRYPTO_IN_USE) &&
        (operations[h_val - 1].owner == partition_id)) {

        memset_operation_context(h_val - 1);
        operations[h_val - 1].in_use = TFM_CRYPTO_NOT_IN_USE;
        operations[h_val - 1].type = TFM_CRYPTO_OPERATION_NONE;
        operations[h_val - 1].owner = 0;

        return PSA_SUCCESS;
    }

    return PSA_ERROR_INVALID_ARGUMENT;
}

psa_status_t tfm_crypto_operation_lookup(enum tfm_crypto_operation_type type,
                                         uint32_t handle,
                                         void **ctx)
{
    int32_t partition_id = 0;
    psa_status_t status;

    if ((handle == TFM_CRYPTO_INVALID_HANDLE) ||
        (handle > CRYPTO_CONC_OPER_NUM)) {
        return PSA_ERROR_BAD_STATE;
    }

    if (ctx == NULL) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    status = tfm_crypto_get_caller_id(&partition_id);
    if (status != PSA_SUCCESS) {
        return status;
    }

    if ((operations[handle - 1].in_use == TFM_CRYPTO_IN_USE) &&
        (operations[handle - 1].type == type) &&
        (operations[handle - 1].owner == partition_id)) {
        *ctx = (void *) &(operations[handle - 1].operation);
        return PSA_SUCCESS;
    }

    return PSA_ERROR_BAD_STATE;
}
/*!@}*/
