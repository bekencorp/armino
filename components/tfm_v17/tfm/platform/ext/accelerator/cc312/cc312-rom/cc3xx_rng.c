/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "cc3xx_rng.h"

#include "cc3xx_error.h"
#include "cc3xx_reg_defs.h"

#include <stdint.h>
#include <stddef.h>
#include <string.h>

cc3xx_err_t cc3xx_rng_init()
{
    /* Enable clock */
    *CC3XX_REG_RNG_RNG_CLK_ENABLE = 0x1U;

    /* reset trng */
    *CC3XX_REG_RNG_RNG_SW_RESET = 0x1U;

    /* Apparently there's no way to tell that the reset has finished, so just do
     * these things repeatedly until they succeed (and hence the reset has
     * finished). Works because the reset value of SAMPLE_CNT1 is 0xFFFF.
     */
    do {
        /* Enable clock */
        *CC3XX_REG_RNG_RNG_CLK_ENABLE = 0x1U;

        /* Set subsampling ratio */
        *CC3XX_REG_RNG_SAMPLE_CNT1 = CC3XX_RNG_SUBSAMPLING_RATE;

    } while (*CC3XX_REG_RNG_SAMPLE_CNT1 != CC3XX_RNG_SUBSAMPLING_RATE);

    /* Temporarily disable the random source */
    *CC3XX_REG_RNG_RND_SOURCE_ENABLE = 0x0U;

    /* Clear the interrupts */
    *CC3XX_REG_RNG_RNG_ICR = 0x3FU;

    /* Mask all interrupts except EHR_VALID */
    *CC3XX_REG_RNG_RNG_IMR = 0x3EU;

    /* Select the oscillator ring (And set SOP_SEL to 0x1 as is mandatory) */
    *CC3XX_REG_RNG_TRNG_CONFIG = CC3XX_RNG_RING_OSCILLATOR_ID | (0x1U << 2);

    /* Set debug control register to no bypasses */
    *CC3XX_REG_RNG_TRNG_DEBUG_CONTROL = 0x0U;

    /* Enable the random source */
    *CC3XX_REG_RNG_RND_SOURCE_ENABLE = 0x1U;

    return CC3XX_ERR_SUCCESS;
}

cc3xx_err_t cc3xx_rng_finish()
{
    /* Disable the random source */
    *CC3XX_REG_RNG_RND_SOURCE_ENABLE = 0x0U;

    /* Disable clock */
    *CC3XX_REG_RNG_RNG_CLK_ENABLE = 0x0U;

    return CC3XX_ERR_SUCCESS;
}

cc3xx_err_t cc3xx_rng_get_random(uint8_t* buf, size_t length)
{
    uint32_t attempt_count = 0;

    /* The cc312 generates 192 bits of entropy, which is used as 24 bytes */
    for (int byte_am = 0; byte_am < length; byte_am += 24) {
        uint32_t tmp_buf[6];
        int copy_size = 0;

        /* Wait until the RNG has finished. Any status other than 0x1 indicates
         * that either the RNG hasn't finished or a statistical test has been
         * failed.
         */
        do {
            if (*CC3XX_REG_RNG_RNG_ISR & 0xEU) {
                /* At least one test has failed - the buffer contents aren't
                 * random.
                 */

                /* Reset EHR registers */
                *CC3XX_REG_RNG_RST_BITS_COUNTER = 0x1U;

                /* Clear the interrupt bits to restart generator */
                *CC3XX_REG_RNG_RNG_ICR = 0x3FU;

                attempt_count++;
            }
        } while ((! (*CC3XX_REG_RNG_RNG_ISR & 0x1U))
                 && attempt_count < CC3XX_RNG_MAX_ATTEMPTS);

        if (attempt_count == CC3XX_RNG_MAX_ATTEMPTS) {
            cc3xx_rng_finish();
            return CC3XX_ERR_GENERIC_ERROR;
        }

        tmp_buf[0] = *CC3XX_REG_RNG_EHR_DATA_0;
        tmp_buf[1] = *CC3XX_REG_RNG_EHR_DATA_1;
        tmp_buf[2] = *CC3XX_REG_RNG_EHR_DATA_2;
        tmp_buf[3] = *CC3XX_REG_RNG_EHR_DATA_3;
        tmp_buf[4] = *CC3XX_REG_RNG_EHR_DATA_4;
        tmp_buf[5] = *CC3XX_REG_RNG_EHR_DATA_5;

        /* Reset EHR register */
        *CC3XX_REG_RNG_RST_BITS_COUNTER = 0x1U;

        /* Clear the interrupt bits to restart generator */
        *CC3XX_REG_RNG_RNG_ICR = 0xFFFFFFFF;

        copy_size = length > byte_am + 24 ? 24 : (length - byte_am);
        memcpy(buf + byte_am, (uint8_t*)tmp_buf, copy_size);
    }

    return CC3XX_ERR_SUCCESS;
}
