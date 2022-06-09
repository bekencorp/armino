/*
 * Copyright (c) 2017-2020 Arm Limited. All rights reserved.
 * Copyright (c) 2020 Nuvoton Technology Corp. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * \file device_definition.h
 * \brief The structure definitions in this file are exported based
 * on the peripheral definitions from device_cfg.h.
 * This file is meant to be used as a helper for baremetal
 * applications and/or as an example of how to configure the generic
 * driver structures.
 */

#ifndef __DEVICE_DEFINITION_H__
#define __DEVICE_DEFINITION_H__

#include "device_cfg.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ======= Peripheral configuration structure declarations ======= */

/* ARM GPIO driver structures */
#ifdef GPIO0_CMSDK_S
#include "gpio_cmsdk_drv.h"
extern struct gpio_cmsdk_dev_t GPIO0_CMSDK_DEV_S;
#endif

/* UART CMSDK driver structures */
#ifdef UART0_CMSDK_S
#include "uart_cmsdk_drv.h"
extern struct uart_cmsdk_dev_t ARM_UART0_DEV_S;
#endif
#ifdef UART0_CMSDK_NS
#include "uart_cmsdk_drv.h"
extern struct uart_cmsdk_dev_t ARM_UART0_DEV_NS;
#endif

#ifdef UART1_CMSDK_S
#include "uart_cmsdk_drv.h"
extern struct uart_cmsdk_dev_t ARM_UART1_DEV_S;
#endif
#ifdef UART1_CMSDK_NS
#include "uart_cmsdk_drv.h"
extern struct uart_cmsdk_dev_t ARM_UART1_DEV_NS;
#endif

/* CMSDK Timer driver structures */
#ifdef CMSDK_TIMER0_S
#include "timer_cmsdk_drv.h"
extern struct timer_cmsdk_dev_t CMSDK_TIMER0_DEV_S;
#endif
#ifdef CMSDK_TIMER0_NS
#include "timer_cmsdk_drv.h"
extern struct timer_cmsdk_dev_t CMSDK_TIMER0_DEV_NS;
#endif

#ifdef CMSDK_TIMER1_S
#include "timer_cmsdk_drv.h"
extern struct timer_cmsdk_dev_t CMSDK_TIMER1_DEV_S;
#endif
#ifdef CMSDK_TIMER1_NS
#include "timer_cmsdk_drv.h"
extern struct timer_cmsdk_dev_t CMSDK_TIMER1_DEV_NS;
#endif

#ifdef CMSDK_TIMER2_S
#include "timer_cmsdk_drv.h"
extern struct timer_cmsdk_dev_t CMSDK_TIMER2_DEV_S;
#endif
#ifdef CMSDK_TIMER2_NS
#include "timer_cmsdk_drv.h"
extern struct timer_cmsdk_dev_t CMSDK_TIMER2_DEV_NS;
#endif



#ifdef __cplusplus
}
#endif

#endif  /* __DEVICE_DEFINITION_H__ */
