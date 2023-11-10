/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef BL1_1_CRYPTO_H
#define BL1_1_CRYPTO_H

#include <stddef.h>
#include <stdint.h>

#define CTR_IV_LEN 16

#include "crypto_key_defs.h"
#include "fih.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Calculates a hash in stages. Note, there is no context here so only one hash
 * operation can be run at once.
 */
fih_int bl1_sha256_init(void);
fih_int bl1_sha256_update(uint8_t *data, size_t data_length);
fih_int bl1_sha256_finish(uint8_t *hash);

/* Calculates a SHA-256 hash of the input data */
fih_int bl1_sha256_compute(const uint8_t *data,
                           size_t data_length,
                           uint8_t *hash);

/* Performs AES-256-CTR decryption */
int32_t bl1_aes_256_ctr_decrypt(enum tfm_bl1_key_id_t key_id,
                                const uint8_t *key_material,
                                uint8_t *counter,
                                const uint8_t *ciphertext,
                                size_t ciphertext_length,
                                uint8_t *plaintext);

/* Derives key material from a BL1 key and some label and context. Any
 * cryptographically secure key derivation algorithm is acceptable.
 */
int32_t bl1_derive_key(enum tfm_bl1_key_id_t input_key, const uint8_t *label,
                       size_t label_length, const uint8_t *context,
                       size_t context_length, uint8_t *output_key,
                       size_t output_length);

#ifdef __cplusplus
}
#endif

#endif /* BL1_1_CRYPTO_H */
