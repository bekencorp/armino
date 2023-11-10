/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/** \file cc3xx_psa_asymmetric_encryption.c
 *
 * This file contains the implementation of the entry points associated to the
 * asymmetric encryption capability as described by the PSA Cryptoprocessor
 * Driver interface specification
 *
 */

#include "cc3xx_psa_asymmetric_encryption.h"
#include "cc3xx_internal_rsa_util.h"
#include "cc3xx_internal_hash_util.h"
#include "cc3xx_internal_drbg_util.h"
#include "cc_pal_mem.h"
#include "cc_rsa_build.h"
#include "cc_rsa_error.h"
#include "cc_rsa_schemes.h"

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdio.h>
#include <stdlib.h>
#define mbedtls_printf printf
#define mbedtls_calloc calloc
#define mbedtls_free free
#endif

/* To be able to include the PSA style configuration */
#include "mbedtls/build_info.h"

static psa_status_t cc3xx_internal_rsa_encrypt(
    const psa_key_attributes_t *attributes, const uint8_t *key_buffer,
    size_t key_buffer_size, psa_algorithm_t alg, const uint8_t *input,
    size_t input_length, const uint8_t *label, size_t label_len,
    uint8_t *output, size_t output_size, size_t *output_length)
{
    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;
    psa_key_type_t key_type = psa_get_key_type(attributes);
    CCError_t error = CC_FATAL_ERROR;
    CCRsaUserPubKey_t *pUserPubKey = NULL;
    CCRsaPrimeData_t *pPrimeData = NULL;
    CCRndContext_t rndContext;
    CCRndContext_t *pRndContext = &rndContext;
    CCRsaHashOpMode_t hashOpMode = CC_RSA_HASH_OpModeLast;

    /* Zero output length by default */
    *output_length = 0;

    status = cc3xx_ctr_drbg_get_ctx(pRndContext);
    if (status != PSA_SUCCESS) {
        return status;
    }

    if (PSA_ALG_IS_RSA_OAEP(alg)) {
        status = cc3xx_psa_hash_mode_to_cc_hash_mode(alg, true, &hashOpMode);
        if (status != PSA_SUCCESS) {
            return status;
        }
    }

    pUserPubKey = mbedtls_calloc(1, sizeof(CCRsaUserPubKey_t));
    pPrimeData = mbedtls_calloc(1, sizeof(CCRsaPrimeData_t));
    if (pUserPubKey == NULL || pPrimeData == NULL) {
        goto cleanup;
    }

    if (PSA_KEY_TYPE_IS_KEY_PAIR(key_type)) {
        status = cc3xx_rsa_psa_priv_to_cc_pub(key_buffer,
                                             key_buffer_size,
                                             pUserPubKey);
    } else {
        status = cc3xx_rsa_psa_pub_to_cc_pub(key_buffer,
                                            key_buffer_size,
                                            pUserPubKey);
    }

    if (status != PSA_SUCCESS) {
        error = CC_FAIL;
        goto cleanup;
    }

    if (PSA_ALG_IS_RSA_OAEP(alg)) {

        error = CC_RsaOaepEncrypt(pRndContext,
                                  pUserPubKey,
                                  pPrimeData,
                                  hashOpMode,
                                  (unsigned char *)label,
                                  label_len,
                                  CC_PKCS1_MGF1,
                                  (unsigned char *)input,
                                  input_length,
                                  output);

    } else if (alg == PSA_ALG_RSA_PKCS1V15_CRYPT) {

        error = CC_RsaPkcs1V15Encrypt(pRndContext,
                                      pUserPubKey,
                                      pPrimeData,
                                      (unsigned char *)input,
                                      input_length,
                                      (unsigned char *)output);
    } else {
        CC_PAL_LOG_ERR("alg variable is corrupted");
        while (1) {
            /* Panic as the caller of this has already checked the
             * algorithm validity
             */
        };
    }

    if (error == CC_OK) {
        *output_length =  PSA_BITS_TO_BYTES(
            ((CCRsaPubKey_t *)pUserPubKey->PublicKeyDbBuff)->nSizeInBits);
    }

cleanup:
    if (error != CC_OK) {
        CC_PalMemSetZero(output, *output_length);
    }
    if (pUserPubKey) {
        CC_PalMemSetZero(pUserPubKey, sizeof(CCRsaUserPubKey_t));
        mbedtls_free(pUserPubKey);
    }
    if (pPrimeData) {
        CC_PalMemSetZero(pPrimeData, sizeof(CCRsaPrimeData_t));
        mbedtls_free(pPrimeData);
    }

    return cc3xx_rsa_cc_error_to_psa_error(error);
}

static psa_status_t cc3xx_internal_rsa_decrypt(
    const psa_key_attributes_t *attributes, const uint8_t *key_buffer,
    size_t key_buffer_size, psa_algorithm_t alg, const uint8_t *input,
    size_t input_length, const uint8_t *label, size_t label_length,
    uint8_t *output, size_t output_size, size_t *output_length)
{
    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;
    CCError_t error = CC_FATAL_ERROR;
    CCRsaUserPrivKey_t *pUserPrivKey = NULL;
    CCRsaPrimeData_t *pPrimeData = NULL;
    CCRsaHashOpMode_t hashOpMode = CC_RSA_HASH_OpModeLast;

    /* Zero output length by default */
    *output_length = 0;

    if (PSA_ALG_IS_RSA_OAEP(alg)) {
        status = cc3xx_psa_hash_mode_to_cc_hash_mode(alg, true, &hashOpMode);
        if (status != PSA_SUCCESS) {
            return status;
        }
    }

    pUserPrivKey = mbedtls_calloc(1, sizeof(CCRsaUserPrivKey_t));
    pPrimeData = mbedtls_calloc(1, sizeof(CCRsaPrimeData_t));
    if (pUserPrivKey == NULL || pPrimeData == NULL) {
        goto cleanup;
    }

    status = cc3xx_rsa_psa_priv_to_cc_priv(key_buffer, key_buffer_size,
                                          pUserPrivKey);
    if (status != PSA_SUCCESS) {
        error = CC_FAIL;
        goto cleanup;
    }

    *output_length = output_size;
    if (PSA_ALG_IS_RSA_OAEP(alg)) {

        error = CC_RsaOaepDecrypt(pUserPrivKey,
                                  pPrimeData,
                                  hashOpMode,
                                  (unsigned char *)label,
                                  label_length,
                                  CC_PKCS1_MGF1,
                                  (unsigned char *)input,
                                  input_length,
                                  output,
                                  output_length);

    } else if (alg == PSA_ALG_RSA_PKCS1V15_CRYPT) {

        error = CC_RsaPkcs1V15Decrypt(pUserPrivKey,
                                      pPrimeData,
                                      (unsigned char *)input,
                                      input_length,
                                      output,
                                      output_length);
    } else {
        CC_PAL_LOG_ERR("alg variable is corrupted");
        while (1) {
            /* Panic as the caller of this has already checked the
             * algorithm validity
             */
        };
    }

cleanup:
    if (error != CC_OK) {
        CC_PalMemSetZero(output, *output_length);
    }
    if (pUserPrivKey) {
        CC_PalMemSetZero(pUserPrivKey, sizeof(CCRsaUserPrivKey_t));
        mbedtls_free(pUserPrivKey);
    }
    if (pPrimeData) {
        CC_PalMemSetZero(pPrimeData, sizeof(CCRsaPrimeData_t));
        mbedtls_free(pPrimeData);
    }

    return cc3xx_rsa_cc_error_to_psa_error(error);
}

/** \defgroup psa_asym_encrypt PSA driver entry points for asymmetric cipher
 *
 *  Entry points for asymmetric cipher encryption and decryption as described
 *  by the PSA Cryptoprocessor Driver interface specification
 *
 *  @{
 */
psa_status_t cc3xx_asymmetric_encrypt(const psa_key_attributes_t *attributes,
                                      const uint8_t *key_buffer,
                                      size_t key_buffer_size,
                                      psa_algorithm_t alg,
                                      const uint8_t *input,
                                      size_t input_length,
                                      const uint8_t *salt, size_t salt_length,
                                      uint8_t *output, size_t output_size,
                                      size_t *output_length)
{
    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;
    size_t key_bits = psa_get_key_bits(attributes);
    psa_key_type_t type = psa_get_key_type(attributes);

    *output_length = 0;

#if defined(PSA_WANT_ALG_RSA_OAEP) || defined(PSA_WANT_ALG_RSA_PKCS1V15_CRYPT)
    if ((alg == PSA_ALG_RSA_PKCS1V15_CRYPT) || PSA_ALG_IS_RSA_OAEP(alg)) {
        /* Check that the output buffer is large enough */
        if (output_size <
                PSA_ASYMMETRIC_ENCRYPT_OUTPUT_SIZE(type, key_bits, alg)) {
            return PSA_ERROR_BUFFER_TOO_SMALL;
        }
        status = cc3xx_internal_rsa_encrypt(
            attributes, key_buffer, key_buffer_size, alg, input, input_length,
            salt, salt_length, output, output_size, output_length);
    } else
#endif /* PSA_WANT_ALG_RSA_OAEP || PSA_WANT_ALG_RSA_PKCS1V15_CRYPT */
#if defined(PSA_WANT_ALG_ECDSA_ANY)
    if (PSA_ALG_IS_ASYMMETRIC_ENCRYPTION(alg) || alg == PSA_ALG_ECDSA_ANY) {
        status = PSA_ERROR_NOT_SUPPORTED;
    } else
#endif /* PSA_WANT_ALG_ECDSA_ANY */
    {
        status = PSA_ERROR_INVALID_ARGUMENT;
    }

    return status;
}

psa_status_t cc3xx_asymmetric_decrypt(const psa_key_attributes_t *attributes,
                                      const uint8_t *key_buffer,
                                      size_t key_buffer_size,
                                      psa_algorithm_t alg,
                                      const uint8_t *input,
                                      size_t input_length,
                                      const uint8_t *salt, size_t salt_length,
                                      uint8_t *output, size_t output_size,
                                      size_t *output_length)
{
    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;

    *output_length = 0;

#if defined(PSA_WANT_ALG_RSA_OAEP) || defined(PSA_WANT_ALG_RSA_PKCS1V15_CRYPT)
    if ((alg == PSA_ALG_RSA_PKCS1V15_CRYPT) || PSA_ALG_IS_RSA_OAEP(alg)) {
        /* We don't perform a check on the output buffer size in the decrypt
         * case because the PSA_ASYMMETRIC_DECRYPT_OUTPUT_SIZE would return
         * only a sufficient value, while the necessary value could be smaller
         * hence too restrictive on the implementation.
         */
        status = cc3xx_internal_rsa_decrypt(
            attributes, key_buffer, key_buffer_size, alg, input, input_length,
            salt, salt_length, output, output_size, output_length);
    } else
#endif /* PSA_WANT_ALG_RSA_OAEP || PSA_WANT_ALG_RSA_PKCS1V15_CRYPT */
#if defined(PSA_WANT_ALG_ECDSA_ANY)
    if (PSA_ALG_IS_ASYMMETRIC_ENCRYPTION(alg) || alg == PSA_ALG_ECDSA_ANY) {
        status = PSA_ERROR_NOT_SUPPORTED;
    } else
#endif /* PSA_WANT_ALG_ECDSA_ANY */
    {
        status = PSA_ERROR_INVALID_ARGUMENT;
    }

    return status;
}
/** @} */ // end of psa_asym_encrypt
