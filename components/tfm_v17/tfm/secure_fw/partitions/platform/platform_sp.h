/*
 * Copyright (c) 2018-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __PLATFORM_SP_H__
#define __PLATFORM_SP_H__

#include "tfm_platform_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief Initializes the secure partition.
 *
 * \return Returns values as specified by the \ref psa_status_t
 */
psa_status_t platform_sp_init(void);

/*!
 * \brief Resets the system.
 *
 * \return Returns values as specified by the \ref tfm_platform_err_t
 */
enum tfm_platform_err_t platform_sp_system_reset(void);

#ifdef __cplusplus
}
#endif

#endif /* __PLATFORM_SP_H__ */
