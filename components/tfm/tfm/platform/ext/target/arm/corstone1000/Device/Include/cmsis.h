/*
 * Copyright (c) 2017-2021, Arm Limited. All rights reserved.
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

#ifndef __CMSIS_H__
#define __CMSIS_H__

/* Configuration of the ARM Cortex-M0+ Processor and Core Peripherals */
#define __CM0PLUS_REV                   0x0001U /* CM0PLUS Core Revision */
#define __NVIC_PRIO_BITS                2       /* Number of Bits used for Priority Levels */
#define __Vendor_SysTickConfig          0       /* Set to 1 if different SysTick Config is used */
#define __VTOR_PRESENT                  1       /* Set to 1 if CPU supports Vector Table Offset Register */
#define __MPU_PRESENT                   1       /* MPU present */

#include "cmsis_compiler.h"
#include "platform_base_address.h"
#include "system_core_init.h"
#include "platform_irq.h"
#include "core_cm0plus.h"

#endif /*__CMSIS_H__ */
