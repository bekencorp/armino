/*
 * Copyright (c) 2018-2021, Arm Limited. All rights reserved.
 * Copyright (c) 2020, Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_PERIPHERALS_DEF_H__
#define __TFM_PERIPHERALS_DEF_H__

#include "platform_irq.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Quantized default IRQ priority, the value is:
 * (Number of configurable priority) / 4: (1UL << __NVIC_PRIO_BITS) / 4
 */
#define DEFAULT_IRQ_PRIORITY    (1UL << (__NVIC_PRIO_BITS - 2))

#define TFM_TIMER0_IRQ    (TIMER0_IRQn)
#define TFM_TIMER1_IRQ    (TIMER1_IRQn)

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
