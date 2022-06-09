/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef CC3XX_HASH_H
#define CC3XX_HASH_H

#include "cc3xx_error.h"

#include <stdint.h>
#include <stddef.h>

#define SHA256_OUTPUT_SIZE 32

#ifdef __cplusplus
extern "C" {
#endif

cc3xx_err_t cc3xx_hash_sha256_init();
cc3xx_err_t cc3xx_hash_sha256_update(const uint8_t *buf, size_t length);
cc3xx_err_t cc3xx_hash_sha256_finish(uint8_t *res, size_t length);

#ifdef __cplusplus
}
#endif

#endif /* CC3XX_HASH_H */
