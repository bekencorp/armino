/**
 * Copyright (C), 2018-2018, Arm Technology (China) Co., Ltd.
 * All rights reserved
 *
 * The content of this file or document is CONFIDENTIAL and PROPRIETARY
 * to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 * License Agreement between Licensee and Arm Technology (China) Co., Ltd
 * restricting among other things, the use, reproduction, distribution
 * and transfer.  Each of the embodiments, including this information and,,
 * any derivative work shall retain this copyright notice.
 */
#include "hal.h"
#include "pal.h"
#include "string.h"
#include "bk7236.h"

/* the user button base address */
#define SEC_DEBUG_ENABLE_FLAG_BASE_ADDR (SOC_AON_GPIO_REG_BASE + 0x0 * 4)

bool hal_secure_debug_is_enabled(void)
{
    uint32_t flag = 0;

    /* the user button1 for secure debug */
    flag = ((*(volatile uint32_t *)(SEC_DEBUG_ENABLE_FLAG_BASE_ADDR)) & (0x1));
    if (flag) {
        return true;
    } else {
        return false;
    }
}

hal_ret_t hal_secure_debug_excute_debug_cmd(debug_cmd_t cmds)
{
    hal_ret_t ret = HAL_OK;
    uint32_t cmd;

    pal_memcpy(&cmd,&cmds,sizeof(uint32_t));

    if (cmd == 0) {
        PAL_LOG_ERR("Invalid secure debug cmd:%d\n",cmd);
        return HAL_ERR_GENERIC;
    }
    if (cmds.cmd0) {
        hal_jtag_enable();
        return HAL_OK;
    }

    return ret;
}
// eof

