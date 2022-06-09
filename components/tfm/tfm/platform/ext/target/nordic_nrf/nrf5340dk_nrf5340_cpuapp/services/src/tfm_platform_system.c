/*
 * Copyright (c) 2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "platform/include/tfm_platform_system.h"
#include "cmsis.h"
#include "tfm_platform_hal_ioctl.h"
#include "tfm_ioctl_core_api.h"

void tfm_platform_hal_system_reset(void)
{
    /* Reset the system */
    NVIC_SystemReset();
}

enum tfm_platform_err_t tfm_platform_hal_ioctl(tfm_platform_ioctl_req_t request,
                                               psa_invec  *in_vec,
                                               psa_outvec *out_vec)
{
	/* Core IOCTL services */
	switch (request) {
	case TFM_PLATFORM_IOCTL_READ_SERVICE:
		return tfm_platform_hal_read_service(in_vec, out_vec);
	case TFM_PLATFORM_IOCTL_GPIO_SERVICE:
		return tfm_platform_hal_gpio_service(in_vec, out_vec);
	/* Board specific IOCTL services */

	/* Not a supported IOCTL service.*/
	default:
		return TFM_PLATFORM_ERR_NOT_SUPPORTED;
	}
}
