/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef CC3XX_KEY_DEFS_H
#define CC3XX_KEY_DEFS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    /* In the TRM the huk is referred to as the rkek */
    cc3xx_aes_key_id_huk      = 0x0U,
    cc3xx_aes_key_id_krtl     = 0x1U,
    cc3xx_aes_key_id_kcp      = 0x2U,
    cc3xx_aes_key_id_kce      = 0x3U,
    cc3xx_aes_key_id_kpicv    = 0x4U,
    cc3xx_aes_key_id_kceicv   = 0x5U,
    cc3xx_aes_key_id_user_key = 0xFFU,
} cc3xx_aes_key_id_t;

typedef enum {
    cc3xx_aes_keysize_128 = 0b00U,
    cc3xx_aes_keysize_192 = 0b01U,
    cc3xx_aes_keysize_256 = 0b10U,
} cc3xx_aes_keysize_t;


#ifdef __cplusplus
}
#endif

#endif /* CC3XX_KEY_DEFS_H */
