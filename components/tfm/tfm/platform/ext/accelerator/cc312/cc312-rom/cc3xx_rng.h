/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef CC3XX_RNG_H
#define CC3XX_RNG_H

#include "cc3xx_error.h"
#include "cc3xx_rng_defs.h"

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

cc3xx_err_t cc3xx_rng_init();
cc3xx_err_t cc3xx_rng_get_random(uint8_t* buf, size_t length);
cc3xx_err_t cc3xx_rng_finish();

#ifdef __cplusplus
}
#endif

#endif /* CC3XX_RNG_H */
