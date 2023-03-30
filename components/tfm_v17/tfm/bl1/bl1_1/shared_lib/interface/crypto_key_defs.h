/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef BL1_1_CRYPTO_KEY_DEFS_H
#define BL1_1_CRYPTO_KEY_DEFS_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef HUK_SIZE
#define HUK_SIZE (32)
#endif /* HUK_SIZE */

#ifndef GUK_SIZE
#define GUK_SIZE (32)
#endif /* GUK_SIZE */

#ifndef BL2_ENCRYPTION_KEY_SIZE
#define BL2_ENCRYPTION_KEY_SIZE (32)
#endif /* BL2_ENCRYPTION_KEY_SIZE */

enum tfm_bl1_key_id_t {
    TFM_BL1_KEY_HUK,
    TFM_BL1_KEY_GUK,
    TFM_BL1_KEY_BL2_ENCRYPTION,
    TFM_BL1_KEY_ROTPK_0,

    TFM_BL1_KEY_USER,
};

#ifdef __cplusplus
}
#endif

#endif /* BL1_1_CRYPTO_KEY_DEFS_H */
