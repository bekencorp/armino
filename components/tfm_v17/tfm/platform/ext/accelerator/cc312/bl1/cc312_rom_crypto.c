/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "crypto.h"

#include <stdint.h>
#include <string.h>

#include "region_defs.h"
#include "dx_reg_base_host.h"
#include "otp.h"
#include "fih.h"
#include "cc3xx_aes.h"
#include "cc3xx_hash.h"
#include "cmsis_compiler.h"

#define KEY_DERIVATION_MAX_BUF_SIZE 128

fih_int bl1_sha256_init(void)
{
    fih_int fih_rc = FIH_FAILURE;

    fih_rc = fih_int_encode_zero_equality(cc3xx_hash_sha256_init());
    if(fih_not_eq(fih_rc, FIH_SUCCESS)) {
        FIH_RET(FIH_FAILURE);
    }

    return FIH_SUCCESS;
}

fih_int bl1_sha256_finish(uint8_t *hash)
{
    fih_int fih_rc = FIH_FAILURE;

    fih_rc = fih_int_encode_zero_equality(cc3xx_hash_sha256_finish(hash, 32));
    if(fih_not_eq(fih_rc, FIH_SUCCESS)) {
        FIH_RET(FIH_FAILURE);
    }

    return FIH_SUCCESS;
}

fih_int bl1_sha256_update(uint8_t *data, size_t data_length)
{
    size_t idx;
    fih_int fih_rc = FIH_FAILURE;

    for (idx = 0; idx + 0x8000 < data_length; idx += 0x8000) {
        fih_rc = FIH_FAILURE;
        fih_rc = fih_int_encode_zero_equality(cc3xx_hash_sha256_update(data + idx,
                                                                       0x8000));
        if(fih_not_eq(fih_rc, FIH_SUCCESS)) {
            FIH_RET(FIH_FAILURE);
        }
    }
    if (idx != (data_length - (data_length % 0x8000))) {
        FIH_RET(FIH_FAILURE);
    }

    fih_rc = fih_int_encode_zero_equality(cc3xx_hash_sha256_update(data + idx,
                                                                   data_length - idx));
    if(fih_not_eq(fih_rc, FIH_SUCCESS)) {
        FIH_RET(FIH_FAILURE);
    }

    return FIH_SUCCESS;
}

fih_int bl1_sha256_compute(const uint8_t *data,
                           size_t data_length,
                           uint8_t *hash)
{
    fih_int fih_rc = FIH_FAILURE;
    size_t idx = 0;

    if (data == NULL || hash == NULL) {
        FIH_RET(FIH_FAILURE);
    }

    fih_rc = fih_int_encode_zero_equality(cc3xx_hash_sha256_init());
    if(fih_not_eq(fih_rc, FIH_SUCCESS)) {
        FIH_RET(FIH_FAILURE);
    }

    for (idx = 0; idx + 0x8000 < data_length; idx += 0x8000) {
        fih_rc = FIH_FAILURE;
        fih_rc = fih_int_encode_zero_equality(cc3xx_hash_sha256_update(data + idx,
                                                                       0x8000));
        if(fih_not_eq(fih_rc, FIH_SUCCESS)) {
            FIH_RET(FIH_FAILURE);
        }
    }
    if (idx != (data_length - (data_length % 0x8000))) {
        FIH_RET(FIH_FAILURE);
    }

    fih_rc = fih_int_encode_zero_equality(cc3xx_hash_sha256_update(data + idx,
                                                                   data_length - idx));
    if(fih_not_eq(fih_rc, FIH_SUCCESS)) {
        FIH_RET(FIH_FAILURE);
    }
    fih_rc = fih_int_encode_zero_equality(cc3xx_hash_sha256_finish(hash, 32));
    if(fih_not_eq(fih_rc, FIH_SUCCESS)) {
        FIH_RET(FIH_FAILURE);
    }

    FIH_RET(FIH_SUCCESS);
}

static int32_t bl1_key_to_cc3xx_key(enum tfm_bl1_key_id_t key_id,
                                    cc3xx_aes_key_id_t *cc3xx_key_type,
                                    uint8_t *key_buf, size_t key_buf_size)
{
    int32_t rc;

    switch(key_id) {
    case TFM_BL1_KEY_HUK:
        *cc3xx_key_type = CC3XX_AES_KEY_ID_HUK;
        break;
    case TFM_BL1_KEY_GUK:
        *cc3xx_key_type = CC3XX_AES_KEY_ID_GUK;
        break;
    default:
        *cc3xx_key_type = CC3XX_AES_KEY_ID_USER_KEY;
        rc = bl1_otp_read_key(key_id, key_buf);
        if (rc) {
            memset(key_buf, 0, key_buf_size);
            return rc;
        }
        break;
    }

    return 0;
}

int32_t bl1_aes_256_ctr_decrypt(enum tfm_bl1_key_id_t key_id,
                                const uint8_t *key_material,
                                uint8_t *counter,
                                const uint8_t *ciphertext,
                                size_t ciphertext_length,
                                uint8_t *plaintext)
{
    cc3xx_aes_key_id_t cc3xx_key_type;
    uint8_t  __ALIGNED(4) key_buf[32];
    int32_t rc = 0;
    size_t idx = 0;
    const uint8_t *input_key = key_buf;

    if (ciphertext_length == 0) {
        return 0;
    }

    if (counter == NULL || ciphertext == NULL || plaintext == NULL) {
        return -1;
    }

    if (key_material == NULL) {
        rc = bl1_key_to_cc3xx_key(key_id, &cc3xx_key_type, key_buf,
                                  sizeof(key_buf));
        if (rc) {
            return rc;
        }
    } else {
        cc3xx_key_type = CC3XX_AES_KEY_ID_USER_KEY;
        input_key = key_material;
    }

    for (idx = 0; idx + 0x8000 < ciphertext_length; idx += 0x8000) {
        rc = cc3xx_aes(cc3xx_key_type, input_key, CC3XX_AES_KEYSIZE_256,
                       ciphertext + idx, 0x8000, counter, plaintext + idx,
                       CC3XX_AES_DIRECTION_ENCRYPT, CC3XX_AES_MODE_CTR);
        if (rc != CC3XX_ERR_SUCCESS) {
            return rc;
        }
    }

    /* Under CTR mode encryption and decryption are the same operation */
    return cc3xx_aes(cc3xx_key_type, input_key, CC3XX_AES_KEYSIZE_256,
                     ciphertext + idx, ciphertext_length - idx, counter,
                     plaintext + idx, CC3XX_AES_DIRECTION_ENCRYPT,
                     CC3XX_AES_MODE_CTR);
}

static int32_t aes_256_ecb_encrypt(enum tfm_bl1_key_id_t key_id,
                                   const uint8_t *plaintext,
                                   size_t ciphertext_length,
                                   uint8_t *ciphertext)
{
    cc3xx_aes_key_id_t cc3xx_key_type;
    uint8_t __ALIGNED(4) key_buf[32];
    int32_t rc = 0;

    if (ciphertext_length == 0) {
        return 0;
    }

    if (ciphertext == NULL || plaintext == NULL) {
        return -1;
    }

    rc = bl1_key_to_cc3xx_key(key_id, &cc3xx_key_type, key_buf, sizeof(key_buf));
    if (rc) {
        return rc;
    }

    return cc3xx_aes(cc3xx_key_type, key_buf, CC3XX_AES_KEYSIZE_256, plaintext,
                     ciphertext_length, NULL, ciphertext,
                     CC3XX_AES_DIRECTION_ENCRYPT, CC3XX_AES_MODE_ECB);
}

/* This is a counter-mode KDF complying with NIST SP800-108 where the PRF is a
 * combined sha256 hash and an ECB-mode AES encryption. ECB is acceptable here
 * since the input to the PRF is a hash, and the hash input is different every
 * time because of the counter being part of the input.
 */
int32_t bl1_derive_key(enum tfm_bl1_key_id_t input_key, const uint8_t *label,
                       size_t label_length, const uint8_t *context,
                       size_t context_length, uint8_t *output_key,
                       size_t output_length)
{
    uint8_t state[KEY_DERIVATION_MAX_BUF_SIZE];
    uint8_t state_size = label_length + context_length + sizeof(uint8_t)
                         + 2 * sizeof(uint32_t);
    uint8_t state_hash[32];
    uint32_t L = output_length;
    uint32_t n = (output_length + sizeof(state_hash) - 1) / sizeof(state_hash);
    uint32_t i = 1;
    size_t output_idx = 0;
    cc3xx_err_t rc;

    if (output_length == 0) {
        return 0;
    }

    if (label == NULL || label_length == 0 ||
        context == NULL || context_length == 0 ||
        output_key == NULL) {
        return -1;
    }

    if (state_size > KEY_DERIVATION_MAX_BUF_SIZE) {
        return -1;
    }

    memcpy(state + sizeof(uint32_t), label, label_length);
    memset(state + sizeof(uint32_t) + label_length, 0, sizeof(uint8_t));
    memcpy(state + sizeof(uint32_t) + label_length + sizeof(uint8_t),
           context, context_length);
    memcpy(state + sizeof(uint32_t) + label_length + sizeof(uint8_t) + context_length,
           &L, sizeof(uint32_t));

    for (i = 1; i < n; i++) {
        memcpy(state, &i, sizeof(uint32_t));

        /* Hash the state to make it a constant size */
        rc = bl1_sha256_compute(state, state_size, state_hash);
        if (rc != CC3XX_ERR_SUCCESS) {
            goto err;
        }

        /* Encrypt using ECB, which is fine because the state is different every
         * time and we're hashing it.
         */
        rc = aes_256_ecb_encrypt(input_key, state_hash, sizeof(state_hash),
                                 output_key + output_idx);
        if (rc != CC3XX_ERR_SUCCESS) {
            goto err;
        }

        output_idx += sizeof(state_hash);
    }

    /* For the last block, encrypt into the state buf and then memcpy out how
     * much we need
     */
    memcpy(state, &i, sizeof(uint32_t));

    rc = bl1_sha256_compute(state, state_size, state_hash);
    if (rc != CC3XX_ERR_SUCCESS) {
        goto err;
    }

    /* This relies on us being able to have overlapping input and output
     * pointers.
     */
    rc = aes_256_ecb_encrypt(input_key, state_hash, sizeof(state_hash),
                             state_hash);
    if (rc != CC3XX_ERR_SUCCESS) {
        goto err;
    }

    memcpy(output_key + output_idx, state_hash, output_length - output_idx);
    memset(state, 0, sizeof(state));
    memset(state_hash, 0, sizeof(state_hash));

    return 0;

err:
    memset(output_key, 0, output_length);
    memset(state, 0, sizeof(state));
    memset(state_hash, 0, sizeof(state_hash));
    return rc;
}
