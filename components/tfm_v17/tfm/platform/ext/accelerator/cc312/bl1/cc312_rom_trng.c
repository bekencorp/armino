/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "trng.h"
#include "cc3xx_rng.h"

int32_t bl1_trng_generate_random(uint8_t *output, size_t output_size)
{
    cc3xx_err_t err;

    if (output_size == 0) {
        return 0;
    }

    if (output == NULL) {
        return -1;
    }

    cc3xx_rng_init();
    err = cc3xx_rng_get_random(output, output_size);
    cc3xx_rng_finish();

    return err;
}

unsigned char fih_delay_random_uchar(void) {
    unsigned char out = 0;
    bl1_trng_generate_random(&out, sizeof(out));
    return out;
}
