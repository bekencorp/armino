/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "tfm_plat_crypto_nv_seed.h"
#include "tfm_plat_otp.h"
#include "psa/internal_trusted_storage.h"

int tfm_plat_crypto_provision_entropy_seed(void)
{
    enum tfm_plat_err_t plat_err;
    int err;
    uint8_t buf[64];

    /* If the seed is already provisioned, then return successfully */
    err = tfm_plat_crypto_nv_seed_read(buf, sizeof(buf));
    if (err == TFM_CRYPTO_NV_SEED_SUCCESS) {
        return err;
    }

    plat_err = tfm_plat_otp_read(PLAT_OTP_ID_ENTROPY_SEED, sizeof(buf), buf);
    if(plat_err != TFM_PLAT_ERR_SUCCESS) {
        return TFM_CRYPTO_NV_SEED_FAILED;
    }

    err = tfm_plat_crypto_nv_seed_write(buf, sizeof(buf));
    if (err != TFM_CRYPTO_NV_SEED_SUCCESS) {
        return err;
    }

    return TFM_CRYPTO_NV_SEED_SUCCESS;
}

int tfm_plat_crypto_nv_seed_read(unsigned char *buf, size_t buf_len)
{
    psa_storage_uid_t uid = NV_SEED_FILE_ID;
    psa_status_t status;
    size_t data_length = 0;

    status = psa_its_get(uid, 0, buf_len, buf, &data_length);

    if (status == PSA_SUCCESS && data_length == buf_len) {
        return TFM_CRYPTO_NV_SEED_SUCCESS;
    } else {
        return TFM_CRYPTO_NV_SEED_FAILED;
    }
}

int tfm_plat_crypto_nv_seed_write(const unsigned char *buf, size_t buf_len)
{
    psa_storage_uid_t uid = NV_SEED_FILE_ID;
    psa_status_t status;

    status = psa_its_set(uid, buf_len, buf, 0);

    if (status == PSA_SUCCESS) {
        return TFM_CRYPTO_NV_SEED_SUCCESS;
    } else {
        return TFM_CRYPTO_NV_SEED_FAILED;
    }
}
