/*
 * Copyright (c) 2021 Arm Limited
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "psa_adac_config.h"
#include "psa_adac_debug.h"
#include "adac_crypto_cc312.h"

#if defined (PSA_ADAC_RSA3072) || defined (PSA_ADAC_RSA4096)
#include "cc_rsa_build.h"
#include "rsa/cc_rsa_local.h"
#endif

#if defined (PSA_ADAC_EC_P256) || defined (PSA_ADAC_EC_P521)
#include "cc_ecpki_build.h"
#include "ec_wrst/cc_ecpki_local.h"
#if defined (PSA_ADAC_EC_P256)
#include "ec_wrst/ecc_domains/cc_ecpki_domain_secp256r1.h"
#endif
#if defined (PSA_ADAC_EC_P521)
#include "ec_wrst/ecc_domains/cc_ecpki_domain_secp521r1.h"
#endif
#endif

#include <string.h>

psa_status_t psa_adac_verify_signature(uint8_t key_type, uint8_t *key,
        size_t key_size, psa_algorithm_t hash_algo, const uint8_t *inputs[],
        size_t input_sizes[], size_t input_count, psa_algorithm_t sig_algo,
        uint8_t *sig, size_t sig_size)
{
    psa_status_t r;
    uint8_t hash[PSA_HASH_MAX_SIZE];
    size_t hash_size;
    CCError_t error = CC_FAIL;

    r = psa_adac_hash_multiple(hash_algo, inputs, input_sizes, input_count,
            hash, sizeof(hash), &hash_size);

    if (r != PSA_SUCCESS) {
        return r;
    }

    if ((key_type == RSA_3072_SHA256) || (key_type == RSA_4096_SHA256)) {
#if defined (PSA_ADAC_RSA3072) || defined (PSA_ADAC_RSA4096)
        CCRsaPubUserContext_t rsaPubUserContext;
        CCRsaUserPubKey_t pubKey;
        uint8_t F4[3] = {0x01, 0x0, 0x1};
        CCRsaHashOpMode_t hashOpMode =
            (hash_algo == PSA_ALG_SHA_256) ? CC_RSA_After_SHA256_mode :
            ((hash_algo == PSA_ALG_SHA_512) ? CC_RSA_After_SHA512_mode :
                            CC_RSA_HASH_NO_HASH_mode);
        PSA_ADAC_LOG_TRACE("cc312", "psa_adac_verify_signature Rsa%d\n", key_size);
        error = CC_RsaPubKeyBuild(&pubKey, F4, sizeof(F4), key, key_size);

        if (CC_OK != error) {
            PSA_ADAC_LOG_ERR("cc312", "Error in CC_RsaPubKeyBuild %lx\n", error);
        } else if (CC_OK != (error = CC_RsaVerifyInit(&rsaPubUserContext, &pubKey,
                        hashOpMode, CC_PKCS1_MGF1, 32, CC_PKCS1_VER21))) {
            PSA_ADAC_LOG_ERR("cc312", "Error in CC_RsaVerifyInit %lx\n", error);
        } else if (CC_OK != (error = CC_RsaVerifyUpdate(&rsaPubUserContext, hash,
                        hash_size))) {
            PSA_ADAC_LOG_ERR("cc312", "Error in CC_RsaVerifyUpdate %lx\n", error);
        } else if (CC_OK != (error = CC_RsaVerifyFinish(&rsaPubUserContext, sig)))
        {
            PSA_ADAC_LOG_ERR("cc312", "Error in CC_RsaVerifyFinish %lx\n", error);
        }
#else
        return PSA_ERROR_NOT_SUPPORTED;
#endif
    } else if ((key_type == ECDSA_P256_SHA256) ||
               (key_type == ECDSA_P521_SHA512)) {
#if defined (PSA_ADAC_EC_P256) || defined (PSA_ADAC_EC_P521)
        CCEcdsaVerifyUserContext_t ecdsaVerifyUserContext;
        CCEcpkiUserPublKey_t pubKey;

        CCEcpkiHashOpMode_t hashOpMode =
            (hash_algo == PSA_ALG_SHA_256) ? CC_ECPKI_AFTER_HASH_SHA256_mode :
            ((hash_algo == PSA_ALG_SHA_512) ? CC_ECPKI_AFTER_HASH_SHA512_mode :
                        CC_ECPKI_HASH_NumOfModes);

        if (key_type == ECDSA_P256_SHA256) {
#if defined (PSA_ADAC_EC_P256)
            if (key_size == ECDSA_P256_PUBLIC_KEY_SIZE) {
                uint8_t pub_key[ECDSA_P256_PUBLIC_KEY_SIZE + 1] = {0x04};
                memcpy(pub_key + 1, key, ECDSA_P256_PUBLIC_KEY_SIZE);
                PSA_ADAC_LOG_TRACE("cc312",
                        "psa_adac_verify_signature EcdsaP256\n");
                error = CC_EcpkiPubKeyBuild(CC_EcpkiGetSecp256r1DomainP(),
                        pub_key, sizeof(pub_key), &pubKey);
            } else {
                return PSA_ERROR_INVALID_ARGUMENT;
            }
#else
            return PSA_ERROR_NOT_SUPPORTED;
#endif
        } else if (key_type == ECDSA_P521_SHA512) {
#if defined (PSA_ADAC_EC_P521)
            if (key_size == ECDSA_P521_PUBLIC_KEY_SIZE) {
                uint8_t pub_key[ECDSA_P521_PUBLIC_KEY_SIZE + 1] = {0x04};
                memcpy(pub_key + 1, key, ECDSA_P521_PUBLIC_KEY_SIZE);
                PSA_ADAC_LOG_TRACE("cc312",
                        "psa_adac_verify_signature EcdsaP521\n");
                error = CC_EcpkiPubKeyBuild(CC_EcpkiGetSecp521r1DomainP(),
                                            pub_key, sizeof(pub_key), &pubKey);
            } else {
                return PSA_ERROR_INVALID_ARGUMENT;
            }
#else
            return PSA_ERROR_NOT_SUPPORTED;
#endif
        } else {
            return PSA_ERROR_INVALID_ARGUMENT;
        }

        if (CC_OK != error) {
            PSA_ADAC_LOG_ERR("cc312", "Error in CC_EcpkiPubKeyBuild %lx\n",
                                        error);
        } else if (CC_OK != (error = EcdsaVerifyInit(&ecdsaVerifyUserContext,
                        &pubKey, hashOpMode))) {
            PSA_ADAC_LOG_ERR("cc312", "Error in EcdsaVerifyInit %lx\n", error);
        } else if (CC_OK != (error = EcdsaVerifyUpdate(&ecdsaVerifyUserContext,
                        hash, hash_size))) {
            PSA_ADAC_LOG_ERR("cc312", "Error in EcdsaVerifyUpdate %lx\n",
                                        error);
        } else if (CC_OK != (error = EcdsaVerifyFinish(&ecdsaVerifyUserContext,
                        sig, sig_size))) {
            PSA_ADAC_LOG_ERR("cc312", "Error in EcdsaVerifyFinish %lx\n",
                                        error);
        }
#else
        return PSA_ERROR_NOT_SUPPORTED;
#endif
    } else {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    PSA_ADAC_LOG_DEBUG("cc312", "Signature verification: %s\n",
                                       error == CC_OK ? "success" : "failure");
    return (error == CC_OK) ? PSA_SUCCESS : PSA_ERROR_INVALID_SIGNATURE;
}
