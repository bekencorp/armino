/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "cc3xx_lcs.h"

#include "cc3xx_lcs_defs.h"
#include "cc3xx_reg_defs.h"
#include "cc3xx_error.h"

cc3xx_err_t cc3xx_lcs_get(cc3xx_lcs_t* lcs){
    uint32_t code = 0;

    /* Wait until the NVM controller is idle */
    while (! (*CC3XX_REG_NVM_NVM_IS_IDLE & 0b1U)) {}

    if (! (*CC3XX_REG_NVM_LCS_IS_VALID & 0b1U)) {
        return CC3XX_ERR_INVALID_LCS;
    }

    code = *CC3XX_REG_NVM_LCS_REG & 0b111U;

    switch(code) {
        case CC3XX_LCS_CM_CODE:
            *lcs = cc3xx_lcs_cm;
            break;
        case CC3XX_LCS_DM_CODE:
            *lcs = cc3xx_lcs_dm;
            break;
        case CC3XX_LCS_SE_CODE:
            *lcs = cc3xx_lcs_se;
            break;
        case CC3XX_LCS_RMA_CODE:
            *lcs = cc3xx_lcs_rma;
            break;
        default:
            return CC3XX_ERR_INVALID_LCS;
    }

    return CC3XX_ERR_SUCCESS;
}

const char* cc3xx_lcs_get_name(cc3xx_lcs_t lcs) {
    switch(lcs) {
        case cc3xx_lcs_cm:
            return "LCS_CM";
        case cc3xx_lcs_dm:
            return "LCS_DM";
        case cc3xx_lcs_se:
            return "LCS_SE";
        case cc3xx_lcs_rma:
            return "LCS_RMA";
        default:
            return "LCS_UNKNOWN";
    }
}
