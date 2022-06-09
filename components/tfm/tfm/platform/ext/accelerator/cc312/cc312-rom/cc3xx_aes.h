/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef CC3XX_AES_H
#define CC3XX_AES_H

#include "cc3xx_error.h"

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    CC3XX_AES_DIRECTION_ENCRYPT = 0b0U,
    CC3XX_AES_DIRECTION_DECRYPT = 0b1U,
} cc3xx_aes_direction_t;

typedef enum {
    CC3XX_AES_MODE_ECB = 0b000U,
    CC3XX_AES_MODE_CTR = 0b010U,
} cc3xx_aes_mode_t;

typedef enum {
    CC3XX_AES_KEYSIZE_128 = 0b00U,
    CC3XX_AES_KEYSIZE_192 = 0b01U,
    CC3XX_AES_KEYSIZE_256 = 0b10U,
} cc3xx_aes_keysize_t;

typedef enum {
    /* In the TRM the huk is referred to as the rkek */
    CC3XX_AES_KEY_ID_HUK      = 0x0U,
    CC3XX_AES_KEY_ID_KRTL     = 0x1U,
    CC3XX_AES_KEY_ID_KCP      = 0x2U,
    CC3XX_AES_KEY_ID_KCE      = 0x3U,
    CC3XX_AES_KEY_ID_KPICV    = 0x4U,
    CC3XX_AES_KEY_ID_KCEICV   = 0x5U,
    CC3XX_AES_KEY_ID_USER_KEY = 0xFFU,
} cc3xx_aes_key_id_t;

cc3xx_err_t cc3xx_aes(cc3xx_aes_key_id_t key_id, const uint8_t *key,
                    cc3xx_aes_keysize_t key_size, const uint8_t* in, size_t
                    in_len, uint8_t* iv, uint8_t *out,
                    cc3xx_aes_direction_t direction, cc3xx_aes_mode_t mode);

#ifdef __cplusplus
}
#endif

#endif /* CC3XX_AES_H */
