/*
 * Copyright (c) 2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __CONFIG_PARTITION_CRYPTO_H__
#define __CONFIG_PARTITION_CRYPTO_H__

#include "config_tfm.h"

/*
 * Heap size for the crypto backend
 * CRYPTO_ENGINE_BUF_SIZE needs to be >8KB for EC signing by attest module.
 */
#ifndef CRYPTO_ENGINE_BUF_SIZE
#pragma message("CRYPTO_ENGINE_BUF_SIZE is defaulted to 0x2080. Please check and set it explicitly.")
#define CRYPTO_ENGINE_BUF_SIZE                 0x2080
#endif

/* The CC312 needs at least 0x4000 of heap size to work properly */
#if defined(CRYPTO_HW_ACCELERATOR_CC312) && (CRYPTO_ENGINE_BUF_SIZE < 0x4000)
#pragma message("CRYPTO_ENGINE_BUF_SIZE is redefined to 0x4000.")
#undef  CRYPTO_ENGINE_BUF_SIZE
#define CRYPTO_ENGINE_BUF_SIZE                 0x4000
#endif

/* The max number of concurrent operations that can be active (allocated) at any time in Crypto */
#ifndef CRYPTO_CONC_OPER_NUM
#pragma message("CRYPTO_CONC_OPER_NUM is defaulted to 8. Please check and set it explicitly.")
#define CRYPTO_CONC_OPER_NUM                   8
#endif

/* Enable PSA Crypto random number generator module */
#ifndef CRYPTO_RNG_MODULE_ENABLED
#pragma message("CRYPTO_RNG_MODULE_ENABLED is defaulted to 1. Please check and set it explicitly.")
#define CRYPTO_RNG_MODULE_ENABLED              1
#endif

/* Enable PSA Crypto Key module */
#ifndef CRYPTO_KEY_MODULE_ENABLED
#pragma message("CRYPTO_KEY_MODULE_ENABLED is defaulted to 1. Please check and set it explicitly.")
#define CRYPTO_KEY_MODULE_ENABLED              1
#endif

/* Enable PSA Crypto AEAD module */
#ifndef CRYPTO_AEAD_MODULE_ENABLED
#pragma message("CRYPTO_AEAD_MODULE_ENABLED is defaulted to 1. Please check and set it explicitly.")
#define CRYPTO_AEAD_MODULE_ENABLED             1
#endif

/* Enable PSA Crypto MAC module */
#ifndef CRYPTO_MAC_MODULE_ENABLED
#pragma message("CRYPTO_MAC_MODULE_ENABLED is defaulted to 1. Please check and set it explicitly.")
#define CRYPTO_MAC_MODULE_ENABLED              1
#endif

/* Enable PSA Crypto Hash module */
#ifndef CRYPTO_HASH_MODULE_ENABLED
#pragma message("CRYPTO_HASH_MODULE_ENABLED is defaulted to 1. Please check and set it explicitly.")
#define CRYPTO_HASH_MODULE_ENABLED             1
#endif

/* Enable PSA Crypto Cipher module */
#ifndef CRYPTO_CIPHER_MODULE_ENABLED
#pragma message("CRYPTO_CIPHER_MODULE_ENABLED is defaulted to 1. Please check and set it explicitly.")
#define CRYPTO_CIPHER_MODULE_ENABLED           1
#endif

/* Enable PSA Crypto asymmetric key signature module */
#ifndef CRYPTO_ASYM_SIGN_MODULE_ENABLED
#pragma message("CRYPTO_ASYM_SIGN_MODULE_ENABLED is defaulted to 1. Please check and set it explicitly.")
#define CRYPTO_ASYM_SIGN_MODULE_ENABLED        1
#endif

/* Enable PSA Crypto asymmetric key encryption module */
#ifndef CRYPTO_ASYM_ENCRYPT_MODULE_ENABLED
#pragma message("CRYPTO_ASYM_ENCRYPT_MODULE_ENABLED is defaulted to 1. Please check and set it explicitly.")
#define CRYPTO_ASYM_ENCRYPT_MODULE_ENABLED     1
#endif

/* Enable PSA Crypto key derivation module */
#ifndef CRYPTO_KEY_DERIVATION_MODULE_ENABLED
#pragma message("CRYPTO_KEY_DERIVATION_MODULE_ENABLED is defaulted to 1. Please check and set it explicitly.")
#define CRYPTO_KEY_DERIVATION_MODULE_ENABLED   1
#endif

/* Default size of the internal scratch buffer used for PSA FF IOVec allocations */
#ifndef CRYPTO_IOVEC_BUFFER_SIZE
#pragma message("CRYPTO_IOVEC_BUFFER_SIZE is defaulted to 5120. Please check and set it explicitly.")
#define CRYPTO_IOVEC_BUFFER_SIZE               5120
#endif

/* Use stored NV seed to provide entropy */
#ifndef CRYPTO_NV_SEED
#pragma message("CRYPTO_NV_SEED is defaulted to 1. Please check and set it explicitly.")
#define CRYPTO_NV_SEED                         1
#endif

/*
 * Only enable multi-part operations in Hash, MAC, AEAD and symmetric ciphers,
 * to optimize memory footprint in resource-constrained devices.
 */
#ifndef CRYPTO_SINGLE_PART_FUNCS_DISABLED
#pragma message("CRYPTO_SINGLE_PART_FUNCS_DISABLED is defaulted to 0. Please check and set it explicitly.")
#define CRYPTO_SINGLE_PART_FUNCS_DISABLED      0
#endif

/* The stack size of the Crypto Secure Partition */
#ifndef CRYPTO_STACK_SIZE
#pragma message("CRYPTO_STACK_SIZE is defaulted to 0x1B00. Please check and set it explicitly.")
#define CRYPTO_STACK_SIZE                      0x1B00
#endif

/* Check invalid configs. */
#if CRYPTO_NV_SEED && defined(CRYPTO_HW_ACCELERATOR)
#error "Invalid config: CRYPTO_NV_SEED AND CRYPTO_HW_ACCELERATOR!"
#endif

#if (!CRYPTO_NV_SEED) && (!defined(CRYPTO_HW_ACCELERATOR))
#error "Invalid config: NOT CRYPTO_NV_SEED AND NOT CRYPTO_HW_ACCELERATOR!"
#endif

#endif /* __CONFIG_PARTITION_CRYPTO_H__ */
