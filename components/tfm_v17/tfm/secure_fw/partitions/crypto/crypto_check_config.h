/*
 * Copyright (c) 2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifndef __CRYPTO_CHECK_CONFIG_H__
#define __CRYPTO_CHECK_CONFIG_H__

#include "config_crypto.h"

#if CRYPTO_RNG_MODULE_ENABLED  && \
    (!defined(MBEDTLS_CTR_DRBG_C) &&            \
     !defined(MBEDTLS_HMAC_DRBG_C) &&           \
     !defined(MBEDTLS_PSA_CRYPTO_EXTERNAL_RNG))
#error "CRYPTO_RNG_MODULE_ENABLED enables, but not all prerequisites (missing RNG)!"
#endif

#if CRYPTO_AEAD_MODULE_ENABLED &&                 \
    (!defined(PSA_WANT_ALG_CCM) && !defined(PSA_WANT_ALG_GCM) && \
     !defined(PSA_WANT_ALG_CHACHA20_POLY1305))
#error "CRYPTO_AEAD_MODULE_ENABLED enables, but not all prerequisites (missing AEAD algorithms)!"
#endif

#if CRYPTO_MAC_MODULE_ENABLED && \
    (!defined(PSA_WANT_ALG_CMAC) && !defined(PSA_WANT_ALG_HMAC))
#error "CRYPTO_MAC_MODULE_ENABLED enables, but not all prerequisites (missing MAC algorithms)!"
#endif

#if CRYPTO_CIPHER_MODULE_ENABLED && \
    (!defined(PSA_WANT_KEY_TYPE_AES) &&            \
     !defined(PSA_WANT_KEY_TYPE_CHACHA20) &&       \
     !defined(PSA_WANT_ALG_CBC_NO_PADDING) &&      \
     !defined(PSA_WANT_ALG_CBC_PKCS7) &&           \
     !defined(PSA_WANT_ALG_CCM) &&                 \
     !defined(PSA_WANT_ALG_GCM))
#error "CRYPTO_CIPHER_MODULE_ENABLED enables, but not all prerequisites (missing CIPHER algorithms)!"
#endif

#if CRYPTO_HASH_MODULE_ENABLED && \
    (!defined(PSA_WANT_ALG_RIPEMD160) &&         \
     !defined(PSA_WANT_ALG_SHA_224) &&           \
     !defined(PSA_WANT_ALG_SHA_256) &&           \
     !defined(PSA_WANT_ALG_SHA_384) &&           \
     !defined(PSA_WANT_ALG_SHA_512))
#error "CRYPTO_HASH_MODULE_ENABLED enables, but not all prerequisites (missing HASH algorithms)!"
#endif

#if CRYPTO_ASYM_SIGN_MODULE_ENABLED && \
    (!defined(PSA_WANT_ALG_RSA_PKCS1V15_SIGN) && \
     !defined(PSA_WANT_ALG_RSA_PSS) && \
     !defined(PSA_WANT_ALG_ECDSA) && \
     !defined(PSA_WANT_ALG_DETERMINISTIC_ECDSA))
#error "CRYPTO_ASYM_SIGN_MODULE_ENABLED enables, but not all prerequisites (missing asymmetric sign algorithms)!"
#endif

#if CRYPTO_ASYM_ENCRYPT_MODULE_ENABLED && \
    (!defined(PSA_WANT_ALG_RSA_PKCS1V15_CRYPT) && \
     !defined(PSA_WANT_ALG_RSA_OAEP))
#error "CRYPTO_ASYM_ENCRYPT_MODULE_ENABLED enables, but not all prerequisites (missing asymmetric encryption algorithms)!"
#endif

#if CRYPTO_KEY_DERIVATION_MODULE_ENABLED && \
    (!defined(PSA_WANT_ALG_HKDF) && \
     !defined(PSA_WANT_ALG_TLS12_PRF) && \
     !defined(PSA_WANT_ALG_TLS12_PSK_TO_MS))
#error "CRYPTO_KEY_DERIVATION_MODULE_ENABLED enables, but not all prerequisites (missing key derivation algorithms)!"
#endif

#endif /* __CRYPTO_CHECK_CONFIG_H__ */
