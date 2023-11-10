/*
 * Copyright (c) 2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef TFM_BUILTIN_KEY_LOADER_H
#define TFM_BUILTIN_KEY_LOADER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "tfm_mbedcrypto_include.h"

#ifdef PLATFORM_DEFAULT_CRYPTO_KEYS
enum psa_drv_slot_number_t {
    TFM_BUILTIN_KEY_SLOT_HUK = 0,
    TFM_BUILTIN_KEY_SLOT_IAK,
#ifdef TFM_PARTITION_DELEGATED_ATTESTATION
    TFM_BUILTIN_KEY_SLOT_DAK_SEED,
#endif /* TFM_PARTITION_DELEGATED_ATTESTATION */
    TFM_BUILTIN_KEY_SLOT_MAX,
};
#else
#include "platform_builtin_key_loader_ids.h"
#endif

/**
 * \brief Load a key into the builtin key driver
 *
 * \note This should be called for all slots that are required on initial boot,
 *       from the tfm_plat_load_builtin_keys function.
 *
 * \param[in]  buf     Buffer containing key material.
 * \param[in]  key_len Size of the key in bytes.
 * \param[in]  attr    The attributes of the key.
 *
 * \return Returns error code specified in \ref psa_status_t
 */
psa_status_t tfm_builtin_key_loader_load_key(uint8_t *buf, size_t key_len,
                                             psa_key_attributes_t *attr);

/**
 * \brief Returns the length of a key from the builtin driver.
 *
 * \note This function is called by the psa crypto driver wrapper.
 *
 * \param[in]  key_id  The ID of the key to return the length of
 * \param[out] len     The length of the key.
 *
 * \return Returns error code specified in \ref psa_status_t
 */
psa_status_t tfm_builtin_key_loader_get_key_buffer_size(
        mbedtls_svc_key_id_t key_id, size_t *len);

/**
 * \brief Returns the attributes and key material of a key from the builtin
 *        driver
 *
 * \note This function is called by the psa crypto driver wrapper.
 *
 * \param[in] slot_number        The slot of the key
 * \param[out] attributes        The attributes of the key.
 * \param[out] key_buffer        The buffer to output the key material into.
 * \param[in] key_buffer_size    The size of the key material buffer.
 * \param[out] key_buffer_length The length of the key material returned.
 *
 * \return Returns error code specified in \ref psa_status_t
 */
psa_status_t tfm_builtin_key_loader_get_key_buffer(
        psa_drv_slot_number_t slot_number, psa_key_attributes_t *attributes,
        uint8_t *key_buffer, size_t key_buffer_size, size_t *key_buffer_length);

#ifdef __cplusplus
}
#endif

#endif /* TFM_BUILTIN_KEY_LOADER_H */
