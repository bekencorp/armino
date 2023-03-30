/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "cc3xx_dma.h"

#include "cc3xx_reg_defs.h"
#include "cc3xx_engine_state.h"

cc3xx_err_t cc3xx_dma_copy_data(void* dest, const void* src, size_t length)
{
    cc3xx_err_t err;

    if (cc3xx_engine_in_use) {
        /* Since the copy operation isn't restartable, just check that the
         * engine isn't in use when we begin.
         */
        return CC3XX_ERR_ENGINE_IN_USE;
    }

    /* Enable the DMA clock */
    *CC3XX_REG_MISC_DMA_CLK_ENABLE = 0x1U;

    /* Wait for the crypto engine to be ready */
    while (*CC3XX_REG_CC_CTL_CRYPTO_BUSY) {}

    /* Set the crypto engine to the PASSTHROUGH engine */
    *CC3XX_REG_CC_CTL_CRYPTO_CTL = 0b00000U;

    /* Set output target */
    err = cc3xx_dma_set_output(dest, length);
    if (err != CC3XX_ERR_SUCCESS) {
        goto out;
    }

    /* This starts the copy */
    err = cc3xx_dma_input_data(src, length);

out:
    /* Disable the DMA clock */
    *CC3XX_REG_MISC_DMA_CLK_ENABLE = 0x0U;

    return err;
}

cc3xx_err_t cc3xx_dma_input_data(const void* buf, size_t length)
{
    /* There's a max size of data the DMA can deal with */
    if (length >= 0x10000) {
        return CC3XX_ERR_INVALID_DATA;
    }

    /* Mask a sensible set of the host interrupts */
    *CC3XX_REG_HOST_RGF_HOST_RGF_IMR = 0xFF0U;

    /* Reset the AXI_ERROR and SYM_DMA_COMPLETED interrupts */
    *CC3XX_REG_HOST_RGF_HOST_RGF_ICR |= 0x900U;

    /* Set the data source */
    *CC3XX_REG_DIN_SRC_LLI_WORD0 = (uint32_t)buf;
    /* Writing the length triggers the DMA */
    *CC3XX_REG_DIN_SRC_LLI_WORD1 = length;

    /* Wait for the DMA to complete (The SYM_DMA_COMPLETED interrupt to be
     * asserted)
     */
    while (!(*CC3XX_REG_HOST_RGF_HOST_RGF_IRR & 0x800U)){
        if (*CC3XX_REG_HOST_RGF_HOST_RGF_IRR & 0x100U) {
            return CC3XX_ERR_GENERIC_ERROR;
        }
    }

    /* Reset the SYM_DMA_COMPLETED interrupt */
    *CC3XX_REG_HOST_RGF_HOST_RGF_ICR = 0x800U;

    return CC3XX_ERR_SUCCESS;
}

cc3xx_err_t cc3xx_dma_set_output(void* buf, size_t length)
{
    /* Set the data target */
    *CC3XX_REG_DOUT_DST_LLI_WORD0 = (uint32_t)buf;
    /* And the length */
    *CC3XX_REG_DOUT_DST_LLI_WORD1 = length;

    return CC3XX_ERR_SUCCESS;
}
