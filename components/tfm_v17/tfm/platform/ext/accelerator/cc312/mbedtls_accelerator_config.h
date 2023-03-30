/*
 * Copyright (c) 2019-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef MBEDTLS_ACCELERATOR_CONF_H
#define MBEDTLS_ACCELERATOR_CONF_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* RNG Config */
#undef MBEDTLS_ENTROPY_NV_SEED
#undef MBEDTLS_NO_DEFAULT_ENTROPY_SOURCES

#define MBEDTLS_ENTROPY_C
#define MBEDTLS_CTR_DRBG_C

#define MBEDTLS_ENTROPY_HARDWARE_ALT
#define MBEDTLS_ENTROPY_FORCE_SHA256

/* Main Config */
#ifdef CC312_LEGACY_DRIVER_API_ENABLED
#ifdef MBEDTLS_DHM_C
#define MBEDTLS_DHM_ALT
#endif /* MBEDTLS_DHM_C */

#ifdef MBEDTLS_NIST_KW_C
#define MBEDTLS_NIST_KW_ALT
#endif /* MBEDTLS_NIST_KW_C */

#ifdef MBEDTLS_POLY1305_C
#define MBEDTLS_POLY1305_ALT
#endif /* MBEDTLS_POLY1305_C */
#endif /* CC312_LEGACY_DRIVER_API_ENABLED */

/****************************************************************/
/* Require built-in implementations based on PSA requirements */
/****************************************************************/
#if defined(MBEDTLS_PSA_CRYPTO_CONFIG)

#define PSA_WANT_KEY_TYPE_AES                   1

/* The CC312 does not support CFB mode */
#ifdef PSA_WANT_ALG_CFB
#undef PSA_WANT_ALG_CFB
#endif /* PSA_WANT_ALG_CFB */

#ifdef CC312_LEGACY_DRIVER_API_ENABLED

#ifdef PSA_WANT_KEY_TYPE_AES
#define MBEDTLS_AES_ALT
#define MBEDTLS_AES_SETKEY_ENC_ALT
#define MBEDTLS_AES_SETKEY_DEC_ALT
#define MBEDTLS_AES_ENCRYPT_ALT
#define MBEDTLS_AES_DECRYPT_ALT
#endif /* PSA_WANT_KEY_TYPE_AES */

#ifdef PSA_WANT_KEY_TYPE_ARIA
#define MBEDTLS_ARIA_ALT
#endif /* PSA_WANT_KEY_TYPE_ARIA */

#ifdef PSA_WANT_ALG_CCM
#define MBEDTLS_CCM_ALT
#endif /* PSA_WANT_ALG_CCM */

#ifdef PSA_WANT_KEY_TYPE_CHACHA20
#define MBEDTLS_CHACHA20_ALT
#ifdef PSA_WANT_ALG_CHACHA20_POLY1305
#define MBEDTLS_CHACHAPOLY_ALT
#endif /* PSA_WANT_ALG_CHACHA20_POLY1305 */
#endif /* PSA_WANT_KEY_TYPE_CHACHA20 */

#ifdef PSA_WANT_ALG_CMAC
#define MBEDTLS_CMAC_ALT
#endif /* PSA_WANT_ALG_CMAC */

#ifdef PSA_WANT_ALG_ECDH
#define MBEDTLS_ECDH_GEN_PUBLIC_ALT
#define MBEDTLS_ECDH_COMPUTE_SHARED_ALT
#endif /* PSA_WANT_ALG_ECDH */

#ifdef PSA_WANT_ALG_ECDSA
#define MBEDTLS_ECDSA_VERIFY_ALT
#define MBEDTLS_ECDSA_SIGN_ALT

#ifndef CRYPTO_HW_ACCELERATOR_OTP_PROVISIONING
#define MBEDTLS_ECDSA_GENKEY_ALT
#endif
#endif /* PSA_WANT_ALG_ECDSA */

#ifdef PSA_WANT_ALG_GCM
#define MBEDTLS_GCM_ALT
#endif /* PSA_WANT_ALG_GCM */

#ifdef PSA_WANT_ALG_SHA_1
#define MBEDTLS_SHA1_ALT
#define MBEDTLS_SHA1_PROCESS_ALT
#endif /* PSA_WANT_ALG_SHA_1 */

#ifdef PSA_WANT_ALG_SHA_256
#define MBEDTLS_SHA256_ALT
#define MBEDTLS_SHA256_PROCESS_ALT
#endif /* PSA_WANT_ALG_SHA_256 */

#if (defined(PSA_WANT_ALG_ECDH) ||  \
     defined(PSA_WANT_ALG_ECDSA) || \
     defined(PSA_WANT_KEY_TYPE_ECC_KEY_PAIR) || \
     defined(PSA_WANT_KEY_TYPE_ECC_PUBLIC_KEY)) &&  \
    (defined(PSA_WANT_ALG_DETERMINISTIC_ECDSA) ||   \
     defined(PSA_WANT_ALG_RSA_OAEP) ||  \
     defined(PSA_WANT_ALG_RSA_PKCS1V15_SIGN) || \
     defined(PSA_WANT_ALG_RSA_PSS))
#define MBEDTLS_ECJPAKE_ALT
#endif

#endif /* CC312_LEGACY_DRIVER_API_ENABLED */

#if defined(PSA_WANT_ALG_RSA_OAEP)           ||     \
    defined(PSA_WANT_ALG_RSA_PKCS1V15_CRYPT) ||     \
    defined(PSA_WANT_ALG_RSA_PKCS1V15_SIGN)  ||     \
    defined(PSA_WANT_ALG_RSA_PSS)            ||     \
    defined(PSA_WANT_KEY_TYPE_RSA_KEY_PAIR)  ||     \
    defined(PSA_WANT_KEY_TYPE_RSA_PUBLIC_KEY)
#ifdef CC312_LEGACY_DRIVER_API_ENABLED
#define MBEDTLS_RSA_ALT
#define MBEDTLS_PK_RSA_ALT_SUPPORT
#endif /* CC312_LEGACY_DRIVER_API_ENABLED */
#define MBEDTLS_GENPRIME
#endif

#else /* MBEDTLS_PSA_CRYPTO_CONFIG */
/****************************************************************/
/* Infer PSA requirements from Mbed TLS capabilities */
/****************************************************************/
#define MBEDTLS_AES_C

/* The CC312 does not support CFB mode */
#ifdef MBEDTLS_CIPHER_MODE_CFB
#undef MBEDTLS_CIPHER_MODE_CFB
#endif /* MBEDTLS_CIPHER_MODE_CFB */

#ifdef CC312_LEGACY_DRIVER_API_ENABLED

#ifdef MBEDTLS_AES_C
#define MBEDTLS_AES_ALT
#define MBEDTLS_AES_SETKEY_ENC_ALT
#define MBEDTLS_AES_SETKEY_DEC_ALT
#define MBEDTLS_AES_ENCRYPT_ALT
#define MBEDTLS_AES_DECRYPT_ALT
#endif /* MBEDTLS_AES_C */

#ifdef MBEDTLS_ARIA_C
#define MBEDTLS_ARIA_ALT
#endif /* MBEDTLS_ARIA_C */

#ifdef MBEDTLS_CCM_C
#define MBEDTLS_CCM_ALT
#endif /* MBEDTLS_CCM_C */

#ifdef MBEDTLS_CHACHA20_C
#define MBEDTLS_CHACHA20_ALT
#endif /* MBEDTLS_CHACHA20_C */

#ifdef MBEDTLS_CHACHAPOLY_C
#define MBEDTLS_CHACHAPOLY_ALT
#endif /* MBEDTLS_CHACHAPOLY_C */

#ifdef MBEDTLS_CMAC_C
#define MBEDTLS_CMAC_ALT
#endif /* MBEDTLS_CMAC_C */

#ifdef MBEDTLS_ECDH_C
#define MBEDTLS_ECDH_GEN_PUBLIC_ALT
#define MBEDTLS_ECDH_COMPUTE_SHARED_ALT
#endif /* MBEDTLS_ECDH_C */

#ifdef MBEDTLS_ECDSA_C
#define MBEDTLS_ECDSA_VERIFY_ALT
#define MBEDTLS_ECDSA_SIGN_ALT

#ifndef CRYPTO_HW_ACCELERATOR_OTP_PROVISIONING
#define MBEDTLS_ECDSA_GENKEY_ALT
#endif
#endif /* MBEDTLS_ECDSA_C */

#ifdef MBEDTLS_GCM_C
#define MBEDTLS_GCM_ALT
#endif /* MBEDTLS_GCM_C */

#ifdef MBEDTLS_SHA1_C
#define MBEDTLS_SHA1_ALT
#define MBEDTLS_SHA1_PROCESS_ALT
#endif /* MBEDTLS_SHA1_C */

#ifdef MBEDTLS_SHA256_C
#define MBEDTLS_SHA256_ALT
#define MBEDTLS_SHA256_PROCESS_ALT
#endif /* MBEDTLS_SHA256_C */

#if defined(MBEDTLS_ECP_C) && defined(MBEDTLS_MD_C)
#define MBEDTLS_ECJPAKE_ALT
#endif /* MBEDTLS_ECP_C && MBEDTLS_MD_C */

#endif /* CC312_LEGACY_DRIVER_API_ENABLED */

#ifdef MBEDTLS_RSA_C
#ifdef CC312_LEGACY_DRIVER_API_ENABLED
#define MBEDTLS_RSA_ALT
#define MBEDTLS_PK_RSA_ALT_SUPPORT
#endif /* CC312_LEGACY_DRIVER_API_ENABLED */
#define MBEDTLS_GENPRIME
#endif /* MBEDTLS_RSA_C */

#endif /* MBEDTLS_PSA_CRYPTO_CONFIG */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MBEDTLS_ACCELERATOR_CONF_H */
