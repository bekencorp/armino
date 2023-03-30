/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef CC3XX_INIT_H
#define CC3XX_INIT_H

#include "cc3xx_error.h"

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

cc3xx_err_t cc3xx_init(void);
cc3xx_err_t cc3xx_finish(void);

#ifdef __cplusplus
}
#endif

#endif /* CC3XX_INIT_H */
