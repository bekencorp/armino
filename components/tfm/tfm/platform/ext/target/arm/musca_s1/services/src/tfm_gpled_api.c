/*
 * Copyright (c) 2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "tfm_ioctl_api.h"
#include "tfm_gpled_api.h"
#include "tfm_platform_api.h"
#include "platform_pins.h"

/*
 * Musca-S1 Platform specific GPLED pin defs
 */
#define RED_LED     AHB_GPIO0_2
#define GREEN_LED   AHB_GPIO0_3
#define BLUE_LED    AHB_GPIO0_4

enum tfm_platform_err_t tfm_plat_gpled_init(void)
{
    int status;
    uint32_t unused;

    status = tfm_platform_gpio_init(&unused);
    if(status != TFM_PLATFORM_ERR_SUCCESS) {
        return status;
    }

    status = tfm_platform_gpio_pin_config(RED_LED, 1, &unused);
    if(status != TFM_PLATFORM_ERR_SUCCESS) {
        return status;
    }

    status = tfm_platform_gpio_pin_config(GREEN_LED, 1, &unused);
    if(status != TFM_PLATFORM_ERR_SUCCESS) {
        return status;
    }

    status = tfm_platform_gpio_pin_config(BLUE_LED, 1, &unused);
    if(status != TFM_PLATFORM_ERR_SUCCESS) {
        return status;
    }

    return TFM_PLATFORM_ERR_SUCCESS;
}

enum tfm_platform_err_t tfm_plat_gpled_set(tfm_gpled_type_t type,
                                           tfm_gpled_state_t state)
{
    uint32_t unused;
    int pin_value;

    switch (state)
    {
    case PLAT_GPLED_ON:
        pin_value = 0;
        break;
    case PLAT_GPLED_OFF:
        pin_value = 1;
        break;
    default:
        return TFM_PLATFORM_ERR_NOT_SUPPORTED;
    }

    switch (type)
    {
    case PLAT_GPLED_RED:
        return tfm_platform_gpio_pin_write(RED_LED, pin_value, &unused);
    case PLAT_GPLED_GREEN:
        return tfm_platform_gpio_pin_write(GREEN_LED, pin_value, &unused);
    case PLAT_GPLED_BLUE:
        return tfm_platform_gpio_pin_write(BLUE_LED, pin_value, &unused);
    default:
        return TFM_PLATFORM_ERR_NOT_SUPPORTED;
    }
}
