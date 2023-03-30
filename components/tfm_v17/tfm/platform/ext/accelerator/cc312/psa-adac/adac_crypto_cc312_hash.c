/*
 * Copyright (c) 2020 Arm Limited
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "adac_crypto_cc312.h"

#include "psa_adac_config.h"
#include "psa_adac_debug.h"
#include <string.h>

#include "pal/cc_pal_types.h"
#include "entropy_poll.h"

static psa_status_t hash_check(const uint8_t *a, size_t la, const uint8_t *b,
        size_t lb)
{
    uint8_t chk = 1;

    if (la == lb) {
        chk = 0;
        for (size_t i = 0; i < la; i++) {
            chk |= (uint8_t) (a[i] ^ b[i]);
        }
    }

    return chk == 0 ? PSA_SUCCESS : PSA_ERROR_INVALID_SIGNATURE;
}

/* API was put in place because there was no implementation of PSA
 * Crypto API that supported PSA Crypto Drivers, but the goal is to
 * migrate to it when it's ready. */
psa_status_t psa_adac_hash(psa_algorithm_t alg, const uint8_t *input,
        size_t input_size, uint8_t *hash, size_t hash_size,
        size_t *hash_length)
{
    return psa_adac_hash_multiple(alg, &input, &input_size, 1, hash, hash_size,
                                        hash_length);
}

psa_status_t psa_adac_hash_multiple(psa_algorithm_t alg,
        const uint8_t *inputs[], size_t input_sizes[], size_t input_count,
        uint8_t hash[], size_t hash_size, size_t *hash_length)
{
    if (PSA_ALG_IS_VENDOR_DEFINED(alg)) {
        /* TODO: Add support for extra algorithms */
        return PSA_ERROR_NOT_SUPPORTED;
    }

    psa_status_t status = PSA_ERROR_NOT_SUPPORTED;
    if (alg == PSA_ALG_SHA_256) {
        if (hash_size < 32) {
            return PSA_ERROR_INVALID_ARGUMENT;
        }

        mbedtls_sha256_context ctx;
        mbedtls_sha256_init(&ctx);

        status = mbedtls_sha256_starts(&ctx, 0) == 0 ?
                        PSA_SUCCESS : PSA_ERROR_GENERIC_ERROR;

        for (size_t i = 0; (i < input_count) && (status == PSA_SUCCESS); i++) {
            status = mbedtls_sha256_update(&ctx, inputs[i], input_sizes[i]) == 0 ?
                     PSA_SUCCESS : PSA_ERROR_GENERIC_ERROR;
        }
        if (status == PSA_SUCCESS) {
            status = mbedtls_sha256_finish(&ctx, hash) == 0 ?
                                        PSA_SUCCESS : PSA_ERROR_GENERIC_ERROR;
        }
        if (status == PSA_SUCCESS) {
            *hash_length = 32;
        }
        mbedtls_sha256_free(&ctx);
    } else if (alg == PSA_ALG_SHA_512) {
#if defined (PSA_ADAC_EC_P521) || defined (PSA_ADAC_ED25519)
        if (hash_size < 64) {
            return PSA_ERROR_INVALID_ARGUMENT;
        }

        mbedtls_sha512_context ctx;
        mbedtls_sha512_init(&ctx);
        status = mbedtls_sha512_starts_ret(&ctx, 0) == 0 ?
                                PSA_SUCCESS : PSA_ERROR_GENERIC_ERROR;
        for (size_t i = 0; (i < input_count) && (status == PSA_SUCCESS); i++) {
            status = mbedtls_sha512_update_ret(&ctx, inputs[i], input_sizes[i]) == 0 ?
                     PSA_SUCCESS : PSA_ERROR_GENERIC_ERROR;
        }
        if (status == PSA_SUCCESS) {
            status = mbedtls_sha512_finish_ret(&ctx, hash) == 0 ?
                                PSA_SUCCESS : PSA_ERROR_GENERIC_ERROR;
        }
        if (status == PSA_SUCCESS) {
            *hash_length = 64;
        }
        mbedtls_sha512_free(&ctx);
#endif
    }

    return status;
}

psa_status_t psa_adac_hash_verify(psa_algorithm_t alg, const uint8_t input[],
        size_t input_length, uint8_t hash[], size_t hash_size)
{
    size_t len = 0;
    uint8_t computed_hash[64];
    psa_status_t status = PSA_ERROR_GENERIC_ERROR;

    if (PSA_ALG_IS_VENDOR_DEFINED(alg)) {
        // TODO: Add support for extra algorithms
        return PSA_ERROR_NOT_SUPPORTED;
    }

    status = psa_adac_hash_multiple(alg, &input, &input_length, 1,
                                 computed_hash, sizeof(computed_hash), &len);
    if (status == PSA_SUCCESS) {
        if (hash_size != len) {
            status = PSA_ERROR_INVALID_ARGUMENT;
        } else {
            status = hash_check(hash, len, computed_hash, len);
        }
    }

    return status;
}

psa_status_t psa_adac_hash_verify_multiple(psa_algorithm_t alg,
        const uint8_t input[], size_t input_length, uint8_t *hash[],
        size_t hash_size[], size_t hash_count)
{
    size_t len = 0;
    uint8_t computed_hash[64];
    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;

    if (PSA_ALG_IS_VENDOR_DEFINED(alg)) {
        // TODO: Add support for extra algorithms
        return PSA_ERROR_NOT_SUPPORTED;
    }

    status = psa_adac_hash_multiple(alg, &input, &input_length, 1,
            computed_hash, sizeof(computed_hash), &len);
    if (status == PSA_SUCCESS) {
        for (size_t i = 0; i < hash_count; i++) {
            status = hash_check(hash[i], hash_size[i], computed_hash, len);
            if (status == PSA_SUCCESS) {
                break;
            }
        }
    }

    return status;
}
