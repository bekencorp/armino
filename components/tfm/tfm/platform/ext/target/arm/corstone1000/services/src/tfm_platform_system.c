/*
 * Copyright (c) 2018-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "tfm_platform_system.h"
#include "platform_description.h"
#include "corstone1000_ioctl_requests.h"
#include "fwu_agent.h"

void tfm_platform_hal_system_reset(void)
{
    /* Reset the system */
    NVIC_SystemReset();
}

enum tfm_platform_err_t tfm_platform_hal_ioctl(tfm_platform_ioctl_req_t request,
                                               psa_invec  *in_vec,
                                               psa_outvec *out_vec)
{
    int32_t ret = TFM_PLATFORM_ERR_SUCCESS;
    int32_t result;

    switch(request) {

        case IOCTL_CORSTONE1000_FWU_FLASH_IMAGES:
            result = corstone1000_fwu_flash_image();
            if (!result) {
                NVIC_SystemReset();
            }
            break;

        case IOCTL_CORSTONE1000_FWU_HOST_ACK:
            corstone1000_fwu_host_ack();
            break;

        default:
            ret = TFM_PLATFORM_ERR_NOT_SUPPORTED;
            break;

    }

    return ret;
}
