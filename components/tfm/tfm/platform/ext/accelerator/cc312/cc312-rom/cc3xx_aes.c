/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "cc3xx_aes.h"

#include "cc3xx_reg_defs.h"
#include "cc3xx_dma.h"
#include "cc3xx_lcs.h"
#include "cc3xx_engine_state.h"

static cc3xx_err_t cc3xx_aes_dumpiv(cc3xx_aes_mode_t mode, uint8_t *iv) {
    switch (mode) {
    case CC3XX_AES_MODE_CTR:
        ((uint32_t*)iv)[3] = *CC3XX_REG_AES_AES_CTR_0_3;
        ((uint32_t*)iv)[2] = *CC3XX_REG_AES_AES_CTR_0_2;
        ((uint32_t*)iv)[1] = *CC3XX_REG_AES_AES_CTR_0_1;
        ((uint32_t*)iv)[0] = *CC3XX_REG_AES_AES_CTR_0_0;
        break;
    case CC3XX_AES_MODE_ECB:
        return CC3XX_ERR_SUCCESS;
    default:
        return CC3XX_ERR_NOT_IMPLEMENTED;
    }

    return CC3XX_ERR_SUCCESS;
}

static cc3xx_err_t cc3xx_aes_loadiv(cc3xx_aes_mode_t mode, const uint8_t *iv) {
    switch (mode) {
    case CC3XX_AES_MODE_CTR:
        *CC3XX_REG_AES_AES_CTR_0_3 = ((uint32_t*)iv)[3];
        *CC3XX_REG_AES_AES_CTR_0_2 = ((uint32_t*)iv)[2];
        *CC3XX_REG_AES_AES_CTR_0_1 = ((uint32_t*)iv)[1];
        *CC3XX_REG_AES_AES_CTR_0_0 = ((uint32_t*)iv)[0];
        break;
    case CC3XX_AES_MODE_ECB:
        return CC3XX_ERR_SUCCESS;
    default:
        return CC3XX_ERR_NOT_IMPLEMENTED;
    }

    return CC3XX_ERR_SUCCESS;
}

static cc3xx_err_t cc3xx_aes_check_key_lock(cc3xx_aes_key_id_t key_id)
{
    cc3xx_err_t err = CC3XX_ERR_SUCCESS;
    cc3xx_lcs_t lcs;

    switch (key_id) {
    case CC3XX_AES_KEY_ID_HUK:
        break;
    case CC3XX_AES_KEY_ID_KRTL:
        err = cc3xx_lcs_get(&lcs);
        if (err != CC3XX_ERR_SUCCESS) {
            return err;
        }
        /* The RTL key is only valid in certain states */
        if (! (lcs == (cc3xx_lcs_cm | cc3xx_lcs_dm))) {
            return CC3XX_ERR_INVALID_LCS;
        }
        break;
    case CC3XX_AES_KEY_ID_KCP:
        if (*CC3XX_REG_AO_HOST_AO_LOCK_BITS & (0x1U << 3)) {
            return CC3XX_ERR_INVALID_STATE;
        }
        break;
    case CC3XX_AES_KEY_ID_KCE:
        if (*CC3XX_REG_AO_HOST_AO_LOCK_BITS & (0x1U << 4)) {
            return CC3XX_ERR_INVALID_STATE;
        }
        break;
    case CC3XX_AES_KEY_ID_KPICV:
        if (*CC3XX_REG_AO_HOST_AO_LOCK_BITS & (0x1U << 1)) {
            return CC3XX_ERR_INVALID_STATE;
        }
        break;
    case CC3XX_AES_KEY_ID_KCEICV:
        if (*CC3XX_REG_AO_HOST_AO_LOCK_BITS & (0x1U << 2)) {
            return CC3XX_ERR_INVALID_STATE;
        }
        break;
    case CC3XX_AES_KEY_ID_USER_KEY:
        break;
    default:
        return CC3XX_ERR_SUCCESS;
    }

    return CC3XX_ERR_SUCCESS;
}

static cc3xx_err_t cc3xx_aes_setkey(cc3xx_aes_key_id_t key_id,
                                    const uint8_t *key,
                                    cc3xx_aes_keysize_t key_size)
{
    cc3xx_err_t err = CC3XX_ERR_SUCCESS;

    if (key_id != CC3XX_AES_KEY_ID_USER_KEY) {
        /* Check if the HOST_FATAL_ERROR mode is enabled */
        if (*CC3XX_REG_AO_HOST_AO_LOCK_BITS & 0x1U) {
            return CC3XX_ERR_INVALID_STATE;
        }

        /* Check if the key is masked / locked */
        err = cc3xx_aes_check_key_lock(key_id);
        if (err != CC3XX_ERR_SUCCESS) {
            return err;
        }

        /* Select the required key */
        *CC3XX_REG_HOST_RGF_HOST_CRYPTOKEY_SEL = key_id;

        /* Trigger the load into the key registers */
        *CC3XX_REG_AES_AES_SK = 0b1U;
    } else {
        switch (key_size) {
        case CC3XX_AES_KEYSIZE_256:
            *CC3XX_REG_AES_AES_KEY_0_7 = ((uint32_t*)key)[7];
            *CC3XX_REG_AES_AES_KEY_0_6 = ((uint32_t*)key)[6];
        case CC3XX_AES_KEYSIZE_192:
            *CC3XX_REG_AES_AES_KEY_0_5 = ((uint32_t*)key)[5];
            *CC3XX_REG_AES_AES_KEY_0_4 = ((uint32_t*)key)[4];
        case CC3XX_AES_KEYSIZE_128:
            *CC3XX_REG_AES_AES_KEY_0_3 = ((uint32_t*)key)[3];
            *CC3XX_REG_AES_AES_KEY_0_2 = ((uint32_t*)key)[2];
            *CC3XX_REG_AES_AES_KEY_0_1 = ((uint32_t*)key)[1];
            *CC3XX_REG_AES_AES_KEY_0_0 = ((uint32_t*)key)[0];
            break;
        default:
            return CC3XX_ERR_NOT_IMPLEMENTED;
        }
    }

    /* Set key size */
    *CC3XX_REG_AES_AES_CONTROL &= ~(0b11U << 12);
    *CC3XX_REG_AES_AES_CONTROL |= (key_size & 0b11U) << 12;

    return CC3XX_ERR_SUCCESS;
}

cc3xx_err_t cc3xx_aes(cc3xx_aes_key_id_t key_id, const uint8_t *key,
                      cc3xx_aes_keysize_t key_size, const uint8_t* in, size_t
                      in_len, uint8_t* iv, uint8_t *out,
                      cc3xx_aes_direction_t direction, cc3xx_aes_mode_t mode)
{
    cc3xx_err_t err = CC3XX_ERR_SUCCESS;

    if (cc3xx_engine_in_use) {
        /* Since the AES operation isn't restartable, just check that the engine
         * isn't in use when we begin.
         */
        return CC3XX_ERR_ENGINE_IN_USE;
    }

    /* Enable the aes engine clock */
    *CC3XX_REG_MISC_AES_CLK_ENABLE = 0x1U;

    /* Enable the DMA clock */
    *CC3XX_REG_MISC_DMA_CLK_ENABLE = 0x1U;

    /* Wait for the crypto engine to be ready */
    while (*CC3XX_REG_CC_CTL_CRYPTO_BUSY) {}

    /* Set the crypto engine to the AES engine */
    *CC3XX_REG_CC_CTL_CRYPTO_CTL = 0b00001U;

    /* Clear number of remaining bytes */
    *CC3XX_REG_AES_AES_REMAINING_BYTES = 0x0U;

    /* Set direction field of AES control register */
    *CC3XX_REG_AES_AES_CONTROL &= ~0b1U;
    *CC3XX_REG_AES_AES_CONTROL |= (direction & 0b1U);

    /* Set mode field of AES control register */
    *CC3XX_REG_AES_AES_CONTROL &= ~(0b111U << 2);
    *CC3XX_REG_AES_AES_CONTROL |= (mode & 0b111U) << 2;

    /* Set up the key */
    err = cc3xx_aes_setkey(key_id, key, key_size);
    if (err != CC3XX_ERR_SUCCESS) {
        goto out;
    }

    /* Set up the IV */
    err = cc3xx_aes_loadiv(mode, iv);
    if (err != CC3XX_ERR_SUCCESS) {
        goto out;
    }

    /* Set up output */
    cc3xx_dma_set_output(out, in_len);

    /* Use DMA to input the data. This completes the encryption */
    err = cc3xx_dma_input_data(in, in_len);

    /* Dump the IV so multipart works */
    err = cc3xx_aes_dumpiv(mode, iv);
    if (err != CC3XX_ERR_SUCCESS) {
        goto out;
    }

out:
    /* Set the crypto engine back to the default PASSTHROUGH engine */
    *CC3XX_REG_CC_CTL_CRYPTO_CTL = 0x0;

    /* Disable the hash engine clock */
    *CC3XX_REG_MISC_AES_CLK_ENABLE = 0x0U;

    /* Disable the DMA clock */
    *CC3XX_REG_MISC_DMA_CLK_ENABLE = 0x0U;

    return err;
}
