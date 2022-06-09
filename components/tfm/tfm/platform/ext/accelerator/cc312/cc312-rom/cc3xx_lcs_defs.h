/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef CC3XX_LCS_DEFS_H
#define CC3XX_LCS_DEFS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t cc3xx_lcs_t;

/*
 * These do not correspond to the actual codes used in the CC3XX_REG_NVM_LCS_REG
 * register, but instead are defined so that a set of lifecycle states can be
 * checked using a mask.
 */
enum cc3xx_lcs {
    cc3xx_lcs_cm  = 1,
    cc3xx_lcs_dm  = 2,
    cc3xx_lcs_se  = 4,
    cc3xx_lcs_rma = 8,
};

/*
 * These are the definitions used in the register.
 */
#define CC3XX_LCS_CM_CODE  0b000
#define CC3XX_LCS_DM_CODE  0b001
#define CC3XX_LCS_SE_CODE  0b101
#define CC3XX_LCS_RMA_CODE 0b111

#ifdef __cplusplus
}
#endif

#endif /* CC3XX_LCS_DEFS_H */
