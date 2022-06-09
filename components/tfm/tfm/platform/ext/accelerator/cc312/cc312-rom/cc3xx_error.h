/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef CC3XX_ERROR_H
#define CC3XX_ERROR_H

#include <stdint.h>

typedef int cc3xx_err_t;

#ifdef __cplusplus
extern "C" {
#endif
enum cc3xx_error {
    CC3XX_ERR_SUCCESS = 0,
    CC3XX_ERR_BUFFER_OVERFLOW,
    CC3XX_ERR_INVALID_LCS,
    CC3XX_ERR_INVALID_DATA,
    CC3XX_ERR_INVALID_STATE,
    CC3XX_ERR_NOT_IMPLEMENTED,
    CC3XX_ERR_GENERIC_ERROR,
    CC3XX_ERR_ENGINE_IN_USE,
};

#ifdef __cplusplus
}
#endif

#endif /* CC3XX_ERROR_H */
