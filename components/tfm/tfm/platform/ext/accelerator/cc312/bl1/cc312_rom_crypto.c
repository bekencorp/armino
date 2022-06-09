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

int32_t bl1_aes_256_ctr_decrypt(enum tfm_bl1_key_id_t key_id,
                                uint8_t *counter,
                                const uint8_t *ciphertext,
                                size_t ciphertext_length,
                                uint8_t *plaintext)
{
    cc3xx_aes_key_id_t cc3xx_key_type;
    uint8_t key_buf[32];
    int32_t rc = 0;
    size_t idx = 0;

    if (ciphertext_length == 0) {
        return 0;
    }

    if (counter == NULL || ciphertext == NULL || plaintext == NULL) {
        return -1;
    }

    switch(key_id) {
        case TFM_BL1_KEY_HUK:
            cc3xx_key_type = CC3XX_AES_KEY_ID_HUK;
            break;
        default:
            cc3xx_key_type = CC3XX_AES_KEY_ID_USER_KEY;
            rc = bl1_otp_read_key(key_id, key_buf);
            if (rc) {
                memset(key_buf, 0, sizeof(key_buf));
                return rc;
            }
            break;
    }

    for (idx = 0; idx + 0x8000 < ciphertext_length; idx += 0x8000) {
        rc = cc3xx_aes(cc3xx_key_type, key_buf, CC3XX_AES_KEYSIZE_256,
                       ciphertext + idx, 0x8000, counter, plaintext + idx,
                       CC3XX_AES_DIRECTION_ENCRYPT, CC3XX_AES_MODE_CTR);
        if (rc != CC3XX_ERR_SUCCESS) {
            return rc;
        }
    }

    return cc3xx_aes(cc3xx_key_type, key_buf, CC3XX_AES_KEYSIZE_256,
                     ciphertext + idx, ciphertext_length - idx, counter,
                     plaintext + idx, CC3XX_AES_DIRECTION_ENCRYPT,
                     CC3XX_AES_MODE_CTR);
}
