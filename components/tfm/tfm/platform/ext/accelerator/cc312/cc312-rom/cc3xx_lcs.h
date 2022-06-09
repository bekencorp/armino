/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef CC3XX_LCS_H
#define CC3XX_LCS_H

#include "cc3xx_lcs_defs.h"
#include "cc3xx_error.h"

#ifdef __cplusplus
extern "C" {
#endif

cc3xx_err_t cc3xx_lcs_get(cc3xx_lcs_t* lcs);

const char* cc3xx_lcs_get_name(cc3xx_lcs_t lcs);

#ifdef __cplusplus
}
#endif

#endif /* CC3XX_LCS_H */
