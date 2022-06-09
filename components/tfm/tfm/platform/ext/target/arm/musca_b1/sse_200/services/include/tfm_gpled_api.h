/*
 * Copyright (c) 2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_GPLED_API_H__
#define __TFM_GPLED_API_H__

#include "tfm_platform_api.h"

/*!
 * \enum tfm_gpled_types_t
 *
 * \brief Platform GPLED service types
 *
 */
enum tfm_gled_types_t{
    // Use if RGB not available
    PLAT_GPLED_MONO,
    // Use if RGB is available
    PLAT_GPLED_RED,
    PLAT_GPLED_GREEN,
    PLAT_GPLED_BLUE
};

typedef int32_t tfm_gpled_type_t;

/*!
 * \enum tfm_gpled_states_t
 *
 * \brief Platform GPLED service states
 *
 */
enum tfm_gpled_states_t{
    PLAT_GPLED_OFF,
    PLAT_GPLED_ON
};

typedef int32_t tfm_gpled_state_t;

/**
 * \brief Initialize and configure platform specific GPLEDs.
 *
 * \return Returns values as specified by the \ref tfm_platform_err_t
 */
enum tfm_platform_err_t tfm_plat_gpled_init(void);

/**
 * \brief Sets the state of GPLEDs.
 *
 * \param[in] type     Values specified by the \ref tfm_gled_types_t.
 * \param[in] state    Values specified by the \ref tfm_gpled_states_t.
 *
 * \return Returns values as specified by the \ref tfm_platform_err_t
 */
enum tfm_platform_err_t tfm_plat_gpled_set(tfm_gpled_type_t type,
                                           tfm_gpled_state_t state);

#endif /* __TFM_GPLED_API_H__ */
