/**
 * @if copyright_display
 *              Copyright (C), 2018-2018, Arm Technology (China) Co., Ltd.
 *              All rights reserved
 *
 *          The content of this file or document is CONFIDENTIAL and PROPRIETARY
 *              to Arm Technology (China) Co., Ltd. It is subject to the terms
 * of a
 *              License Agreement between Licensee and Arm Technology (China)
 * Co., Ltd
 *              restricting among other things, the use, reproduction,
 * distribution
 *              and transfer.  Each of the embodiments, including this
 * information and
 *              any derivative work shall retain this copyright notice.
 * @endif
 */
#include <stdint.h>
#include <stdio.h>
#include "secure_debug.h"
#include "secure_debug_app.h"

int secure_debug_agent_entry(void)
{
    sec_debug_stat_t result = HAL_OK;
    sec_debug_ctx_t sec_debug_ctx;

    printf("sec debug flag:%d\n", sec_debug_get_enable_flag());
    if (SEC_DEBUG_TRUE != sec_debug_get_enable_flag()) {
        printf(
            "secure debug enable flag NOT set, no need enable secure debug \n");
        return 0;
    }

    printf("please open secure debug tool to start secure debug enable \n");
    result = sec_debug_init(&sec_debug_ctx, NULL);//TODO wangzhilei
    if (SEC_DEBUG_STAT_OK != result) {
        printf("\n===sec_debug_init fail:0x%x\n", result);
        goto exit;
    }

    result = sec_debug_enable(sec_debug_ctx);
    if (SEC_DEBUG_STAT_OK != result) {
        printf("\n===sec_debug_enable fail:0x%x\n", result);
        goto exit;
    }

exit:
    sec_debug_cleanup(sec_debug_ctx);

    return result;
}
