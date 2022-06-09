/*
 * Copyright (c) 2019-2020 Arm Limited. All rights reserved.
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

#ifndef __PLATFORM_IRQ_H__
#define __PLATFORM_IRQ_H__

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/* =============         Interrupt Number Definition         ================ */
/* ========================================================================== */

typedef enum IRQn
{
  Reset_IRQn                   = -15, /*    !< -15  Reset Vector, invoked on Power up and warm reset                           */
  NonMaskableInt_IRQn          = -14, /*    !< -14  Non maskable Interrupt, cannot be stopped or preempted                     */
  HardFault_IRQn               = -13, /*    !< -13  Hard Fault, all classes of Fault                                           */
  SVCall_IRQn                  =  -5, /*    !< -5 System Service Call via SVC instruction                                      */
  PendSV_IRQn                  =  -2, /*    !< -2 Pendable request for system service                                          */
  SysTick_IRQn                 =  -1, /*    !< -1 System Tick Timer                                                            */

  ExternalIRQ_IRQn             =   0,
  CC312_IRQn                   =   1,
  CC312_APB_C_IRQn             =   2,
  Watchdog_IRQn                =   3,
  /* reserved                  =   4, */
  Timer0_IRQn                  =   5,
  Timer1_IRQn                  =   6,
  /* reserved                  =   7, */
  /* reserved                  =   8, */
  GPIO_IRQn                    =   9,
  GPIO_0_IRQn                  =  10,
  GPIO_1_IRQn                  =  11,
  GPIO_2_IRQn                  =  12,
  GPIO_3_IRQn                  =  13,
  GPIO_4_IRQn                  =  14,
  GPIO_5_IRQn                  =  15,
  GPIO_6_IRQn                  =  16,
  GPIO_7_IRQn                  =  17,
  /* reserved                  =  18, */
  /* reserved                  =  19, */
  /* reserved                  =  20, */
  MHU0_NR2R_IRQn               =  21,
  MHU0_R2NR_IRQn               =  22,
  MHU0_COMB_IRQn               =  23,
  MHU0_MSG_0_IRQn              =  24,
  MHU0_MSG_1_IRQn              =  25,
  MHU1_NR2R_IRQn               =  26,
  MHU1_R2NR_IRQn               =  27,
  MHU1_COMB_IRQn               =  28,
  MHU1_MSG_0_IRQn              =  29,
  MHU1_MSG_1_IRQn              =  30,
  /* reserved                  =  31, */
} IRQn_Type;

#ifdef __cplusplus
}
#endif

#endif  /* __PLATFORM_IRQ_H__ */
