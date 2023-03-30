/*
 * Copyright (c) 2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "tfm_builtin_key_loader.h"

#include "psa/error.h"
#include "tfm_mbedcrypto_include.h"
#include "tfm_crypto_defs.h"
#include "mbedtls/hkdf.h"
#include "psa_manifest/pid.h"
#include "tfm_plat_crypto_keys.h"

#include <string.h>

#ifndef TFM_BUILTIN_MAX_KEY_LEN
#define TFM_BUILTIN_MAX_KEY_LEN 48
#endif /* TFM_BUILTIN_MAX_KEY_LEN */

#ifndef TFM_BUILTIN_MAX_KEYS
#define TFM_BUILTIN_MAX_KEYS 8
#endif /* TFM_BUILTIN_MAX_KEYS */

struct tfm_builtin_key_t {
    uint8_t key[TFM_BUILTIN_MAX_KEY_LEN];
    size_t key_len;
    psa_key_attributes_t attr;
    uint32_t is_loaded;
    psa_drv_slot_number_t slot_number;
};

static struct tfm_builtin_key_t builtin_key_slots[TFM_BUILTIN_MAX_KEYS] = {0};

psa_status_t tfm_builtin_key_loader_load_key(uint8_t *buf, size_t key_len,
                                             psa_key_attributes_t *attr)
{
    psa_status_t err;
    psa_drv_slot_number_t slot_number;
    psa_key_lifetime_t lifetime;
    mbedtls_svc_key_id_t key_id;

    /* Set the owner to 0, as we handle permissions on a granular basis. Having
     * builtin keys being defined with different owners seems to cause a memory
     * leak in the MbedTLS core.
     */
    key_id = psa_get_key_id(attr);
    key_id.MBEDTLS_PRIVATE(owner) = 0;
    psa_set_key_id(attr, key_id);

    if (key_len > TFM_BUILTIN_MAX_KEY_LEN) {
        return PSA_ERROR_BUFFER_TOO_SMALL;
    }

    err = mbedtls_psa_platform_get_builtin_key(psa_get_key_id(attr), &lifetime,
                                               &slot_number);
    if (err != PSA_SUCCESS) {
        return err;
    }

    memcpy(&(builtin_key_slots[slot_number].attr), attr,
           sizeof(psa_key_attributes_t));
    memcpy(&(builtin_key_slots[slot_number].key), buf, key_len);
    builtin_key_slots[slot_number].key_len = key_len;
    builtin_key_slots[slot_number].is_loaded = 1;

    return PSA_SUCCESS;
}

psa_status_t tfm_builtin_key_loader_get_key_buffer_size(
        mbedtls_svc_key_id_t key_id, size_t *len)
{
    psa_status_t err;
    psa_drv_slot_number_t slot_number;
    psa_key_lifetime_t lifetime;

    err = mbedtls_psa_platform_get_builtin_key(key_id, &lifetime, &slot_number);
    if (err != PSA_SUCCESS) {
        return err;
    }

    *len = builtin_key_slots[slot_number].key_len;
    return PSA_SUCCESS;
}

static psa_status_t builtin_key_get_attributes(
        struct tfm_builtin_key_t *key_slot, psa_key_attributes_t *attr)
{
    enum tfm_plat_err_t plat_err;
    /* Preserve the key id and lifetime */
    mbedtls_svc_key_id_t key_id = psa_get_key_id(attr);
    psa_key_lifetime_t lifetime = psa_get_key_lifetime(attr);
    psa_key_usage_t usage = psa_get_key_usage_flags(attr);

    memcpy(attr, &(key_slot->attr), sizeof(psa_key_attributes_t));

    plat_err = tfm_plat_builtin_key_get_usage(
            MBEDTLS_SVC_KEY_ID_GET_KEY_ID(key_id),
            MBEDTLS_SVC_KEY_ID_GET_OWNER_ID(key_id), &usage);
    if (plat_err != TFM_PLAT_ERR_SUCCESS) {
        return PSA_ERROR_NOT_PERMITTED;
    }

    psa_set_key_id(attr, key_id);
    psa_set_key_lifetime(attr, lifetime);
    psa_set_key_usage_flags(attr, usage);

    return PSA_SUCCESS;
}

static psa_status_t derive_subkey_into_buffer(
        struct tfm_builtin_key_t *key_slot, mbedtls_key_owner_id_t owner,
        uint8_t *key_buffer, size_t key_buffer_size, size_t *key_buffer_length)
{
    int mbedtls_err;

#ifdef TFM_PARTITION_TEST_PS
    /* Hack to allow the PS tests to work, since they directly call
     * ps_system_prepare from the test partition which would otherwise derive a
     * different key.
     */
    if (owner == TFM_SP_PS_TEST) {
        owner = TFM_SP_PS;
    }
#endif /* TFM_PARTITION_TEST_PS */

    /* FIXME this should be moved to using the PSA APIs once key derivation is
     * implemented in the PSA driver wrapper. Using the external PSA apis
     * directly creates a keyslot and we'd need to read the data from it and
     * then destroy it, so isn't ideal. In order to avoid infinite recursion,
     * it'll be necessary to add a special case (probably if owner == 0) to make
     * sure the new PSA derivation request doesn't end up back here.
     */
    mbedtls_err = mbedtls_hkdf(mbedtls_md_info_from_type(MBEDTLS_MD_SHA256),
                               NULL, 0, key_slot->key, key_slot->key_len,
                               (uint8_t *)&owner, sizeof(owner), key_buffer,
                               key_buffer_size);
    if (mbedtls_err) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    *key_buffer_length = key_buffer_size;

    return PSA_SUCCESS;
}

static psa_status_t builtin_key_copy_to_buffer(
        struct tfm_builtin_key_t *key_slot, uint8_t *key_buffer,
        size_t key_buffer_size, size_t *key_buffer_length)
{
    memcpy(key_buffer, key_slot->key, key_slot->key_len);
    *key_buffer_length = key_slot->key_len;

    return PSA_SUCCESS;
}

psa_status_t tfm_builtin_key_loader_get_key_buffer(
        psa_drv_slot_number_t slot_number, psa_key_attributes_t *attributes,
        uint8_t *key_buffer, size_t key_buffer_size, size_t *key_buffer_length)
{
    psa_status_t err;
    struct tfm_builtin_key_t *key_slot;
    mbedtls_svc_key_id_t key_id;

    if ((uint32_t)slot_number >= TFM_BUILTIN_MAX_KEYS) {
        return PSA_ERROR_DOES_NOT_EXIST;
    }

    key_slot = &builtin_key_slots[slot_number];

    if (!key_slot->is_loaded) {
        return PSA_ERROR_DOES_NOT_EXIST;
    }

    if (attributes == NULL) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    err = builtin_key_get_attributes(key_slot, attributes);
    if (err != PSA_SUCCESS) {
        return err;
    }

    if (key_buffer_size < key_slot->key_len) {
        return PSA_ERROR_BUFFER_TOO_SMALL;
    }

    if (key_buffer == NULL || key_buffer_length == NULL) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    key_id = psa_get_key_id(attributes);

    /* If a key can be used for derivation, we derive a further subkey for each
     * owner to prevent multiple owners deriving the same keys as each other.
     * For keys for encryption and signing, it's assumed that if multiple
     * partitions have access to the key, there is a good reason and therefore
     * they both need access to the raw builtin key.
     */
    if (psa_get_key_usage_flags(attributes) & PSA_KEY_USAGE_DERIVE) {
        err = derive_subkey_into_buffer(key_slot,
                                        MBEDTLS_SVC_KEY_ID_GET_OWNER_ID(key_id),
                                        key_buffer, key_buffer_size,
                                        key_buffer_length);
    } else {
        err = builtin_key_copy_to_buffer(key_slot, key_buffer, key_buffer_size,
                                         key_buffer_length);
    }

    return err;
}
