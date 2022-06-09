/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "cc3xx_init.h"

#include "cc3xx_reg_defs.h"

cc3xx_err_t cc3xx_init(void)
{
    /* Configure entire system to litte endian */
    *CC3XX_REG_HOST_RGF_HOST_RGF_ENDIAN = 0x0U;

    /* Disable the DFA countermeasures since they're unsupported, and lock the
     * config option.
     */
    *CC3XX_REG_AO_HOST_AO_LOCK_BITS &= ~(0b1U << 7);
    *CC3XX_REG_AO_HOST_AO_LOCK_BITS |= 0b1U << 8;
    *CC3XX_REG_AES_AES_DFA_IS_ON = 0b0U;

    /* Set AHB to secure */
    *CC3XX_REG_AHB_AHBM_HNONSEC = 0b00U;

    return CC3XX_ERR_SUCCESS;
}

cc3xx_err_t cc3xx_finish(void)
{
    return CC3XX_ERR_SUCCESS;
}
