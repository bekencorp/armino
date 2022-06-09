/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "cc3xx_hash.h"

#include "cc3xx_error.h"
#include "cc3xx_reg_defs.h"
#include "cc3xx_dma.h"
#include "cc3xx_engine_state.h"

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#define SHA256_BLOCK_SIZE  32

static uint8_t cc3xx_hash_partial_block_buf[SHA256_BLOCK_SIZE];
static size_t  cc3xx_hash_partial_block_size = 0;

/* Conveniently, the compiler is smart enough to make this a single-instruction
 * endianness reversal where the architecture has such an instruction
 */
static inline uint32_t bswap_32(uint32_t in) {
    uint32_t out = 0;

    out |= (in & 0xFF000000) >> 24;
    out |= (in & 0x00FF0000) >> 8;
    out |= (in & 0x0000FF00) << 8;
    out |= (in & 0x000000FF) << 24;

    return out;
}

cc3xx_err_t cc3xx_hash_sha256_init() {

    if (cc3xx_engine_in_use) {
        return CC3XX_ERR_ENGINE_IN_USE;
    }
    cc3xx_engine_in_use = true;

    /* Enable the hash engine clock */
    *CC3XX_REG_MISC_HASH_CLK_ENABLE = 0x1U;

    /* Enable the DMA clock */
    *CC3XX_REG_MISC_DMA_CLK_ENABLE = 0x1U;

    /* Wait for the crypto engine to be ready */
    while (*CC3XX_REG_CC_CTL_CRYPTO_BUSY) {}

    /* Set the crypto engine to the HASH engine */
    *CC3XX_REG_CC_CTL_CRYPTO_CTL = 0b00111U;

    /* Select HASH mode, not MAC */
    *CC3XX_REG_HASH_HASH_SEL_AES_MAC  = 0x0U;

    /* Configure the hash engine to SHA256 */
    *CC3XX_REG_HASH_HASH_CONTROL = 0b0010U;

    /* Enable padding */
    *CC3XX_REG_HASH_HASH_PAD_EN = 0x1U;

    /* Disable auto-padding to allow multipart operations */
    *CC3XX_REG_HASH_AUTO_HW_PADDING = 0x0U;

    /* Set already processed length to 0 */
    *CC3XX_REG_HASH_HASH_CUR_LEN_0 = 0x0U;
    *CC3XX_REG_HASH_HASH_CUR_LEN_1 = 0x0U;

    /* Set the registers to the magic initial values of sha256. CryptoCell
     * hardware requires the writes to happen in reverse order
     * (from H7 to H0).
     */
    *CC3XX_REG_HASH_HASH_H7 = 0x5be0cd19U;
    *CC3XX_REG_HASH_HASH_H6 = 0x1f83d9abU;
    *CC3XX_REG_HASH_HASH_H5 = 0x9b05688cU;
    *CC3XX_REG_HASH_HASH_H4 = 0x510e527fU;
    *CC3XX_REG_HASH_HASH_H3 = 0xa54ff53aU;
    *CC3XX_REG_HASH_HASH_H2 = 0x3c6ef372U;
    *CC3XX_REG_HASH_HASH_H1 = 0xbb67ae85U;
    *CC3XX_REG_HASH_HASH_H0 = 0x6a09e667U;

    return CC3XX_ERR_SUCCESS;
}

static void hash_uninit()
{
    /* Reset padding registers as required by the hardware */
    *CC3XX_REG_HASH_HASH_PAD_CFG = 0x0U;
    *CC3XX_REG_HASH_AUTO_HW_PADDING = 0x0U;

    /* Set the crypto engine back to the default PASSTHROUGH engine */
    *CC3XX_REG_CC_CTL_CRYPTO_CTL = 0x0U;

    /* Disable the hash engine clock */
    *CC3XX_REG_MISC_HASH_CLK_ENABLE = 0x0U;

    /* Disable the DMA clock */
    *CC3XX_REG_MISC_DMA_CLK_ENABLE = 0x0U;

    cc3xx_engine_in_use = false;
}

cc3xx_err_t cc3xx_hash_sha256_update(const uint8_t *buf, size_t length) {
    size_t current_processed_length = 0;
    size_t partial_block_size_free = SHA256_BLOCK_SIZE - cc3xx_hash_partial_block_size;
    size_t partial_block_size_increase = 0;
    size_t dma_input_size;
    cc3xx_err_t err = CC3XX_ERR_SUCCESS;

    /* If we have previously unprocessed data (because it didn't meet the block
     * size), then first fill and dispatch that.
     */
    if (cc3xx_hash_partial_block_size != 0) {
        if (length > partial_block_size_free) {
            partial_block_size_increase = partial_block_size_free;
        } else {
            partial_block_size_increase = length;
        }

        memcpy(cc3xx_hash_partial_block_buf + cc3xx_hash_partial_block_size,
               buf, partial_block_size_increase);
        current_processed_length += partial_block_size_increase;

        cc3xx_hash_partial_block_size += partial_block_size_increase;
        partial_block_size_free = SHA256_BLOCK_SIZE - cc3xx_hash_partial_block_size;
    }

    /* If the unprocessed data buffer is full then write it out */
    if (cc3xx_hash_partial_block_size == SHA256_BLOCK_SIZE) {
        err = cc3xx_dma_input_data(cc3xx_hash_partial_block_buf,
                                cc3xx_hash_partial_block_size);
        if (err != CC3XX_ERR_SUCCESS) {
            hash_uninit();
            return err;
        }
        cc3xx_hash_partial_block_size = 0;
    }

    /* DMA over all the data that can be done in entire blocks. At least one
     * byte must remain to go into the partial block.
     */
    if (current_processed_length + SHA256_BLOCK_SIZE < length) {
        dma_input_size = ((length - current_processed_length - 1)
                          / SHA256_BLOCK_SIZE) * SHA256_BLOCK_SIZE;
        err = cc3xx_dma_input_data(buf + current_processed_length,
                                   dma_input_size);
        if (err != CC3XX_ERR_SUCCESS) {
            hash_uninit();
            return err;
        }
        current_processed_length += dma_input_size;
    }

    /* Push any remaining data into the unprocessed data block. At this point
     * it's always true that either the current_processed_length is the length
     * (we put all the data into the unprocessed buffer) or that the unprocessed
     * buffer got filled up so that the buffer is empty, so this cannot
     * overflow.
     */
    if (current_processed_length < length) {
        memcpy(cc3xx_hash_partial_block_buf, buf + current_processed_length,
               length - current_processed_length);
        cc3xx_hash_partial_block_size = (length - current_processed_length);
    }

    return CC3XX_ERR_SUCCESS;
}

cc3xx_err_t cc3xx_hash_sha256_finish(uint8_t *res, size_t length) {
    cc3xx_err_t err = CC3XX_ERR_SUCCESS;

    if (length < SHA256_OUTPUT_SIZE) {
        hash_uninit();
        return CC3XX_ERR_BUFFER_OVERFLOW;
    }

    /* Process the final block */
    if (cc3xx_hash_partial_block_size > 0) {
        *CC3XX_REG_HASH_AUTO_HW_PADDING = 0x1U;
        err = cc3xx_dma_input_data(cc3xx_hash_partial_block_buf,
                                cc3xx_hash_partial_block_size);
        if (err) {
            hash_uninit();
            return err;
        }
        cc3xx_hash_partial_block_size = 0;
    }

    /* Special-case for hardware padding when the length is 0 */
    if (*CC3XX_REG_HASH_HASH_CUR_LEN_0 == 0
        && *CC3XX_REG_HASH_HASH_CUR_LEN_1 == 0) {
        *CC3XX_REG_HASH_HASH_PAD_CFG = 0x4U;
    }

    ((uint32_t*)res)[0] = bswap_32(*CC3XX_REG_HASH_HASH_H0);
    ((uint32_t*)res)[1] = bswap_32(*CC3XX_REG_HASH_HASH_H1);
    ((uint32_t*)res)[2] = bswap_32(*CC3XX_REG_HASH_HASH_H2);
    ((uint32_t*)res)[3] = bswap_32(*CC3XX_REG_HASH_HASH_H3);
    ((uint32_t*)res)[4] = bswap_32(*CC3XX_REG_HASH_HASH_H4);
    ((uint32_t*)res)[5] = bswap_32(*CC3XX_REG_HASH_HASH_H5);
    ((uint32_t*)res)[6] = bswap_32(*CC3XX_REG_HASH_HASH_H6);
    ((uint32_t*)res)[7] = bswap_32(*CC3XX_REG_HASH_HASH_H7);

    hash_uninit();

    return CC3XX_ERR_SUCCESS;
}
