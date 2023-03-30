/*
 * Copyright (c) 2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "pq_crypto.h"
#include "crypto.h"
#include "mbedtls/lms.h"
#include "otp.h"
#include "mbedtls/md.h"
#include <string.h>

struct mbedtls_md_info_t {
    const char *name;
    mbedtls_md_type_t type;
    unsigned char size;
    unsigned char block_size;
};

const mbedtls_md_info_t mbedtls_sha256_info = {
    "SHA256",
    MBEDTLS_MD_SHA256,
    32,
    64,
};

const mbedtls_md_info_t *mbedtls_md_info_from_type(mbedtls_md_type_t md_type)
{
    return &mbedtls_sha256_info;
}

void mbedtls_md_init(mbedtls_md_context_t *ctx)
{
    (void)ctx;

    return;
}

int mbedtls_md_setup(mbedtls_md_context_t *ctx,
                      const mbedtls_md_info_t *md_info, int hmac)
{
    (void)ctx;
    (void)md_info;
    (void)hmac;

    return 0;
}

int mbedtls_md_starts(mbedtls_md_context_t *ctx)
{
    (void)ctx;

    return fih_int_decode(bl1_sha256_init());
}

int mbedtls_md_update(mbedtls_md_context_t *ctx, const unsigned char *input,
                       size_t ilen)
{
    (void)ctx;

    return fih_int_decode(bl1_sha256_update((unsigned char *)input, ilen));
}

int mbedtls_md_finish(mbedtls_md_context_t *ctx, unsigned char *output)
{
    (void)ctx;

    return fih_int_decode(bl1_sha256_finish(output));
}

void mbedtls_md_free(mbedtls_md_context_t *ctx)
{
    return;
}

fih_int pq_crypto_verify(enum tfm_bl1_key_id_t key,
                         const uint8_t *data,
                         size_t data_length,
                         const uint8_t *signature,
                         size_t signature_length)
{
    int rc;
    fih_int fih_rc;
    mbedtls_lms_context ctx;
    uint8_t key_buf[MBEDTLS_LMS_PUBKEY_LEN];

    FIH_CALL(bl1_otp_read_key, fih_rc, key, key_buf);
    if (fih_not_eq(fih_rc, FIH_SUCCESS)) {
        FIH_RET(FIH_FAILURE);
    }

    mbedtls_lms_init(&ctx);

    rc = mbedtls_lms_set_algorithm_type(&ctx, MBEDTLS_LMS_SHA256_M32_H10,
                                        MBEDTLS_LMOTS_SHA256_N32_W8);
    fih_rc = fih_int_encode_zero_equality(rc);
    if (fih_not_eq(fih_rc, FIH_SUCCESS)) {
        fih_rc = FIH_FAILURE;
        goto out;
    }

    rc = mbedtls_lms_import_pubkey(&ctx, key_buf);
    fih_rc = fih_int_encode_zero_equality(rc);
    if (fih_not_eq(fih_rc, FIH_SUCCESS)) {
        fih_rc = FIH_FAILURE;
        goto out;
    }

    rc = mbedtls_lms_verify(&ctx, data, data_length, signature);
    fih_rc = fih_int_encode_zero_equality(rc);

out:
    mbedtls_lms_free(&ctx);
    FIH_RET(fih_rc);
}
