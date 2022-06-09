/*
 * Copyright (c) 2019-2020, Arm Limited. All rights reserved.
 * Copyright (c) 2021 STMicroelectronics. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include <stddef.h>
#include <string.h>
#include "crypto_hw.h"
#include "psa/crypto_types.h"
#include "tfm_plat_crypto_keys.h"
#include "tfm_plat_otp.h"
#include "mbedtls/hkdf.h"


int crypto_hw_accelerator_huk_derive_key(const uint8_t *label,
                                                 size_t label_size,
                                                 const uint8_t *context,
                                                 size_t context_size,
                                                 uint8_t *key,
                                                 size_t key_size)
{
    uint8_t huk_buf[32];
    int err = -1;
    int mbedtls_err;

    if (key == NULL) {
        return -1;
    }

    if (label == NULL && label_size != 0) {
        return -1;
    }

    if (context == NULL && context_size != 0) {
        return -1;
    }

    err = tfm_plat_otp_read(PLAT_OTP_ID_HUK, sizeof(huk_buf), huk_buf);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        goto out;
    }

    mbedtls_err = mbedtls_hkdf(mbedtls_md_info_from_type(MBEDTLS_MD_SHA256),
                               label, label_size, huk_buf, sizeof(huk_buf),
                               context, context_size, key, key_size);
    if (mbedtls_err) {
        err = -1;
        goto out;
    }

out:
    memset(huk_buf, 0, sizeof(huk_buf));

    return err;
}

