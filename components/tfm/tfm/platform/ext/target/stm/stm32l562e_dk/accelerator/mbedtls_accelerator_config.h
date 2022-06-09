/*
 * Copyright (c) 2019, Arm Limited. All rights reserved.
 * Copyright (c) 2021 STMicroelectronics. All rights reserved.
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
#undef MBEDTLS_ECP_NIST_OPTIM
#define MBEDTLS_PLATFORM_ENTROPY
#define MBEDTLS_ENTROPY_C
#define MBEDTLS_ENTROPY_HARDWARE_ALT


/* Main Config */

#ifdef MBEDTLS_ECDSA_C
#define MBEDTLS_ECDSA_VERIFY_ALT
#define MBEDTLS_ECDSA_SIGN_ALT
#endif /* MBEDTLS_ECDSA_C */

#ifdef MBEDTLS_RSA_C
#define MBEDTLS_RSA_ALT
#endif /* MBEDTLS_RSA_C */
/* stm hardware */
#ifdef MBEDTLS_SHA1_C
#define MBEDTLS_SHA1_ALT
#endif /* MBEDTLS_SHA1_C */

/* stm hardware */
#ifdef MBEDTLS_SHA256_C
#define MBEDTLS_SHA256_ALT
#endif /* MBEDTLS_SHA256_C */

#if defined(MBEDTLS_ECP_C)
#define MBEDTLS_ECP_ALT
#endif /* MBEDTLS_ECP_C && MBEDTLS_MD_C */

#undef MBEDTLS_AES_SETKEY_DEC_ALT
#undef MBEDTLS_AES_DECRYPT_ALT

#ifdef MBEDTLS_CCM_C
#define MBEDTLS_CCM_ALT
#endif
#ifdef MBEDTLS_AES_C
#define MBEDTLS_AES_ALT
#endif
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MBEDTLS_ACCELERATOR_CONF_H */
