/*
 * Copyright (c) 2017-2022 Arm Limited. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "tfm_plat_crypto_keys.h"

#include "tfm_builtin_key_ids.h"

#include <stddef.h>
#include <string.h>

#include "region_defs.h"
#include "cmsis_compiler.h"
#include "tfm_plat_otp.h"
#include "psa_manifest/pid.h"
#include "tfm_builtin_key_loader.h"

#define TFM_NS_PARTITION_ID -1

#ifndef MBEDTLS_PSA_CRYPTO_KEY_ID_ENCODES_OWNER
#error "MBEDTLS_PSA_CRYPTO_KEY_ID_ENCODES_OWNER must be selected in Mbed TLS config file"
#endif

enum tfm_plat_err_t tfm_plat_builtin_key_get_usage(psa_key_id_t key_id,
                                                   mbedtls_key_owner_id_t user,
                                                   psa_key_usage_t *usage)
{
    *usage = 0;

    switch (key_id) {
    case TFM_BUILTIN_KEY_ID_HUK:
        /* Allow access to all partitions */
        *usage = PSA_KEY_USAGE_DERIVE;
        break;
    case TFM_BUILTIN_KEY_ID_IAK:
        switch(user) {
#ifdef TFM_PARTITION_INITIAL_ATTESTATION
        case TFM_SP_INITIAL_ATTESTATION:
            *usage = PSA_KEY_USAGE_SIGN_HASH;
#ifdef SYMMETRIC_INITIAL_ATTESTATION
            /* Needed to calculate the instance ID */
            *usage |= PSA_KEY_USAGE_EXPORT;
#endif /* SYMMETRIC_INITIAL_ATTESTATION */
            break;
#if defined(TEST_S_ATTESTATION) || defined(TEST_NS_ATTESTATION)
        /* So that the tests can validate created tokens */
#ifdef TEST_S_ATTESTATION
        case TFM_SP_SECURE_TEST_PARTITION:
#endif /* TEST_S_ATTESTATION */
#ifdef TEST_NS_ATTESTATION
        case TFM_NS_PARTITION_ID:
#endif /* TEST_NS_ATTESTATION */
            *usage = PSA_KEY_USAGE_VERIFY_HASH;
            break;
#endif /* TEST_S_ATTESTATION || TEST_NS_ATTESTATION */
#endif /* TFM_PARTITION_INITIAL_ATTESTATION */
        default:
            return TFM_PLAT_ERR_NOT_PERMITTED;
        }
        break;
    default:
        return TFM_PLAT_ERR_UNSUPPORTED;
    }

    return TFM_PLAT_ERR_SUCCESS;
}

enum tfm_plat_err_t tfm_plat_builtin_key_get_lifetime_and_slot(
    mbedtls_svc_key_id_t key_id,
    psa_key_lifetime_t *lifetime,
    psa_drv_slot_number_t *slot_number)
{
    switch (MBEDTLS_SVC_KEY_ID_GET_KEY_ID(key_id)) {
    case TFM_BUILTIN_KEY_ID_HUK:
        *slot_number = TFM_BUILTIN_KEY_SLOT_HUK;
        *lifetime = PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(
                        PSA_KEY_LIFETIME_PERSISTENT,
                        TFM_BUILTIN_KEY_LOADER_KEY_LOCATION);
        break;
#ifdef TFM_PARTITION_INITIAL_ATTESTATION
    case TFM_BUILTIN_KEY_ID_IAK:
        *slot_number = TFM_BUILTIN_KEY_SLOT_IAK;
        *lifetime = PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(
                        PSA_KEY_LIFETIME_PERSISTENT,
                        TFM_BUILTIN_KEY_LOADER_KEY_LOCATION);
#endif /* TFM_PARTITION_INITIAL_ATTESTATION */
        break;
    default:
        return TFM_PLAT_ERR_UNSUPPORTED;
    }

    return TFM_PLAT_ERR_SUCCESS;
}

static enum tfm_plat_err_t tfm_plat_get_huk(uint8_t *buf, size_t buf_len,
                                            size_t *key_len,
                                            size_t *key_bits,
                                            psa_algorithm_t *algorithm,
                                            psa_key_type_t *type)
{
    enum tfm_plat_err_t err;

    err = tfm_plat_otp_read(PLAT_OTP_ID_HUK, buf_len, buf);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

    err = tfm_plat_otp_get_size(PLAT_OTP_ID_HUK, key_len);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

    *key_bits = *key_len * 8;
    *algorithm = PSA_ALG_HKDF(PSA_ALG_SHA_256);
    *type = PSA_KEY_TYPE_DERIVE;

    return TFM_PLAT_ERR_SUCCESS;
}

#ifdef TFM_PARTITION_INITIAL_ATTESTATION
static enum tfm_plat_err_t tfm_plat_get_iak(uint8_t *buf, size_t buf_len,
                                            size_t *key_len,
                                            size_t *key_bits,
                                            psa_algorithm_t *algorithm,
                                            psa_key_type_t *type)
{
    enum tfm_plat_err_t err;
#ifndef SYMMETRIC_INITIAL_ATTESTATION
    psa_ecc_family_t curve_type;
#endif /* SYMMETRIC_INITIAL_ATTESTATION */

    err = tfm_plat_otp_read(PLAT_OTP_ID_IAK_LEN,
                            sizeof(size_t), (uint8_t*)key_len);
    if(err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }
    *key_bits = *key_len * 8;

    if (buf_len < *key_len) {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }

#ifdef SYMMETRIC_INITIAL_ATTESTATION
    err = tfm_plat_otp_read(PLAT_OTP_ID_IAK_TYPE,
                            sizeof(psa_algorithm_t), (uint8_t*)algorithm);
    if(err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

    *type = PSA_KEY_TYPE_HMAC;
#else /* SYMMETRIC_INITIAL_ATTESTATION */
    err = tfm_plat_otp_read(PLAT_OTP_ID_IAK_TYPE, sizeof(psa_ecc_family_t),
                            &curve_type);
    if(err != TFM_PLAT_ERR_SUCCESS) {
        return err;
    }

    *algorithm = PSA_ALG_ECDSA(PSA_ALG_SHA_256);
    *type = PSA_KEY_TYPE_ECC_KEY_PAIR(curve_type);
#endif /* SYMMETRIC_INITIAL_ATTESTATION */

    return tfm_plat_otp_read(PLAT_OTP_ID_IAK, *key_len, buf);
}
#endif /* TFM_PARTITION_INITIAL_ATTESTATION */

enum tfm_plat_err_t tfm_plat_load_builtin_keys(void)
{
    psa_status_t err;
    mbedtls_svc_key_id_t key_id = MBEDTLS_SVC_KEY_ID_INIT;
    psa_key_attributes_t attr = PSA_KEY_ATTRIBUTES_INIT;
    enum tfm_plat_err_t plat_err;
    uint8_t buf[32];
    size_t key_len;
    size_t key_bits;
    psa_algorithm_t algorithm;
    psa_key_type_t type;

    /* HUK */
    plat_err = tfm_plat_get_huk(buf, sizeof(buf), &key_len, &key_bits,
                                &algorithm, &type);
    if (plat_err != TFM_PLAT_ERR_SUCCESS) {
        return plat_err;
    }
    key_id.MBEDTLS_PRIVATE(key_id) = TFM_BUILTIN_KEY_ID_HUK;
    psa_set_key_id(&attr, key_id);
    psa_set_key_bits(&attr, key_bits);
    psa_set_key_algorithm(&attr, algorithm);
    psa_set_key_type(&attr, type);
    err = tfm_builtin_key_loader_load_key(buf, key_len, &attr);
    if (err != PSA_SUCCESS) {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }

#ifdef TFM_PARTITION_INITIAL_ATTESTATION
    /* IAK */
    plat_err = tfm_plat_get_iak(buf, sizeof(buf), &key_len, &key_bits,
                                &algorithm, &type);
    if (plat_err != TFM_PLAT_ERR_SUCCESS) {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }
    key_id.MBEDTLS_PRIVATE(key_id) = TFM_BUILTIN_KEY_ID_IAK;
    psa_set_key_id(&attr, key_id);
    psa_set_key_bits(&attr, key_bits);
    psa_set_key_algorithm(&attr, algorithm);
    psa_set_key_type(&attr, type);
    err = tfm_builtin_key_loader_load_key(buf, key_len, &attr);
    if (err != PSA_SUCCESS) {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }
#endif /* TFM_PARTITION_INITIAL_ATTESTATION */

    return TFM_PLAT_ERR_SUCCESS;
}
