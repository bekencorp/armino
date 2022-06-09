/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_SPE_OPENAMP_PLATFORM_INTERFACE_H__
#define __TFM_SPE_OPENAMP_PLATFORM_INTERFACE_H__

#include "tfm_openamp_lib.h"
#include "tfm_plat_defs.h"

/**
 * \brief Platform specific initialization of SPE openamp.
 *
 * \retval TFM_PLAT_ERR_SUCCESS     Operation succeeded.
 * \retval Other return code    Operation failed with an error code.
 */
enum tfm_plat_err_t tfm_dual_core_hal_init(void);

/**
 * \brief Notify NSPE that a PSA client call return result is replied.
 *        Implemented by platform specific inter-processor communication driver.
 *
 * \retval TFM_PLAT_ERR_SUCCESS      The notification is successfully sent out.
 * \retval Other return code    Operation failed with an error code.
 */
enum tfm_plat_err_t tfm_hal_notify_peer(void);

#endif /* __TFM_SPE_OPENAMP_PLATFORM_INTERFACE_H__ */
