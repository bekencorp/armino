/*
 * Copyright (c) 2018-2021, Arm Limited. All rights reserved.
 * Copyright (c) 2020, Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_PERIPHERALS_DEF_H__
#define __TFM_PERIPHERALS_DEF_H__

#include "NuMicro.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TFM_TIMER0_IRQ    (TMR0_IRQn)
#define TFM_TIMER1_IRQ    (TMR1_IRQn)

struct platform_data_t;

extern struct platform_data_t tfm_peripheral_std_uart;
extern struct platform_data_t tfm_peripheral_uart1;
extern struct platform_data_t tfm_peripheral_timer0;

#define TFM_PERIPHERAL_STD_UART  (&tfm_peripheral_std_uart)
#define TFM_PERIPHERAL_UART1     (&tfm_peripheral_uart1)
#define TFM_PERIPHERAL_TIMER0    (&tfm_peripheral_timer0)

#ifdef __cplusplus
}
#endif

#endif /* __TFM_PERIPHERALS_DEF_H__ */
