/*
 * Copyright (c) 2017-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_PLAT_CRYPTO_KEYS_H__
#define __TFM_PLAT_CRYPTO_KEYS_H__
/**
 * \note The interfaces defined in this file must be implemented for each
 *       SoC.
 */

#define MBEDTLS_PSA_CRYPTO_KEY_ID_ENCODES_OWNER

#include "tfm_mbedcrypto_include.h"

#include "tfm_plat_defs.h"

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Gets key usage for a given builtin key ID and owner.
 *
 * \param[in]  key_id        ID of key
 * \param[in]  user          Which user to get the usage permissions for
 * \param[out] usage         The permissions that the given user has for the key
 *
 * \return Returns error code specified in \ref tfm_plat_err_t
 */
enum tfm_plat_err_t tfm_plat_builtin_key_get_usage(psa_key_id_t key_id,
                                                   mbedtls_key_owner_id_t user,
                                                   psa_key_usage_t *usage);

/**
 * \brief Gets key lifetime and slot number for a given builtin key ID.
 *
 * \param[in]  key_id        ID of key
 * \param[out] lifetime      Lifetime and storage location of the key
 * \param[out] slot_number   Index of the slot which the key is stored in
 *
 * \return Returns error code specified in \ref tfm_plat_err_t
 */
enum tfm_plat_err_t tfm_plat_builtin_key_get_lifetime_and_slot(
    mbedtls_svc_key_id_t key_id,
    psa_key_lifetime_t *lifetime,
    psa_drv_slot_number_t *slot_number);

/**
 * \brief Load all builtin keys.
 *
 * \return Returns error code specified in \ref tfm_plat_err_t
 */
enum tfm_plat_err_t tfm_plat_load_builtin_keys(void);

#ifdef __cplusplus
}
#endif

#endif /* __TFM_PLAT_CRYPTO_KEYS_H__ */
