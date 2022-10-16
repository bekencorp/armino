/******************************************************************************
 * @file     startup_STAR.c
 * @brief    CMSIS Core Device Startup File for ArmChina-STAR Device
 * @version  V1.0.1
 * @date     24. April 2020
 ******************************************************************************/
/*
 * Copyright (c) 2009-2019 Arm Limited.
 * Copyright (c) 2019-2020 ArmChina.
 * All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "components/log.h"

#if defined (STAR_CU)
  #include "STAR_CU.h"
#elif defined (STAR_SP)
  #include "STAR_SP.h"
#elif defined (STAR_SE)
  #include "STAR_SE.h"
#else
  #error device not specified!
#endif

#define TAG "arch"

/*----------------------------------------------------------------------------
  External References
 *----------------------------------------------------------------------------*/
extern uint32_t __INITIAL_SP;
extern uint32_t __STACK_LIMIT;

extern __NO_RETURN void __PROGRAM_START(void);

/*----------------------------------------------------------------------------
  Internal References
 *----------------------------------------------------------------------------*/
__NO_RETURN void Reset_Handler  (void);
            void Default_Handler(void);

/*----------------------------------------------------------------------------
  Exception / Interrupt Handler
 *----------------------------------------------------------------------------*/
/* Exceptions */
void NMI_Handler            (void) __attribute__ ((weak, alias("Default_Handler")));
void HardFault_Handler      (void) __attribute__ ((weak));
void MemManage_Handler      (void) __attribute__ ((weak, alias("Default_Handler")));
void BusFault_Handler       (void) __attribute__ ((weak, alias("Default_Handler")));
void UsageFault_Handler     (void) __attribute__ ((weak, alias("Default_Handler")));
void SecureFault_Handler    (void) __attribute__ ((weak, alias("Default_Handler")));
void SVC_Handler            (void) __attribute__ ((weak, alias("Default_Handler")));
void DebugMon_Handler       (void) __attribute__ ((weak, alias("Default_Handler")));
void PendSV_Handler         (void) __attribute__ ((weak, alias("Default_Handler")));
void SysTick_Handler        (void) __attribute__ ((weak, alias("Default_Handler")));

void Interrupt0_Handler     (void) __attribute__ ((weak));
void Interrupt1_Handler     (void) __attribute__ ((weak));
void Interrupt2_Handler     (void) __attribute__ ((weak));
void Interrupt3_Handler     (void) __attribute__ ((weak));
void Interrupt4_Handler     (void) __attribute__ ((weak));
void Interrupt5_Handler     (void) __attribute__ ((weak));
void Interrupt6_Handler     (void) __attribute__ ((weak));
void Interrupt7_Handler     (void) __attribute__ ((weak));
void Interrupt8_Handler     (void) __attribute__ ((weak));
void Interrupt9_Handler     (void) __attribute__ ((weak));
void Interrupt10_Handler    (void) __attribute__ ((weak));
void Interrupt11_Handler    (void) __attribute__ ((weak));
void Interrupt12_Handler    (void) __attribute__ ((weak));
void Interrupt13_Handler    (void) __attribute__ ((weak));
void Interrupt14_Handler    (void) __attribute__ ((weak));
void Interrupt15_Handler    (void) __attribute__ ((weak));
void Interrupt16_Handler    (void) __attribute__ ((weak));
void Interrupt17_Handler    (void) __attribute__ ((weak));
void Interrupt18_Handler    (void) __attribute__ ((weak));
void Interrupt19_Handler    (void) __attribute__ ((weak));
void Interrupt20_Handler    (void) __attribute__ ((weak));
void Interrupt21_Handler    (void) __attribute__ ((weak));
void Interrupt22_Handler    (void) __attribute__ ((weak));
void Interrupt23_Handler    (void) __attribute__ ((weak));
void Interrupt24_Handler    (void) __attribute__ ((weak));
void Interrupt25_Handler    (void) __attribute__ ((weak));
void Interrupt26_Handler    (void) __attribute__ ((weak));
void Interrupt27_Handler    (void) __attribute__ ((weak));
void Interrupt28_Handler    (void) __attribute__ ((weak));
void Interrupt29_Handler    (void) __attribute__ ((weak));
void Interrupt30_Handler    (void) __attribute__ ((weak));
void Interrupt31_Handler    (void) __attribute__ ((weak));
void Interrupt32_Handler    (void) __attribute__ ((weak));
void Interrupt33_Handler    (void) __attribute__ ((weak));
void Interrupt34_Handler    (void) __attribute__ ((weak));
void Interrupt35_Handler    (void) __attribute__ ((weak));
void Interrupt36_Handler    (void) __attribute__ ((weak));
void Interrupt37_Handler    (void) __attribute__ ((weak));
void Interrupt38_Handler    (void) __attribute__ ((weak));
void Interrupt39_Handler    (void) __attribute__ ((weak));
void Interrupt40_Handler    (void) __attribute__ ((weak));
void Interrupt41_Handler    (void) __attribute__ ((weak));
void Interrupt42_Handler    (void) __attribute__ ((weak));
void Interrupt43_Handler    (void) __attribute__ ((weak));
void Interrupt44_Handler    (void) __attribute__ ((weak));
void Interrupt45_Handler    (void) __attribute__ ((weak));
void Interrupt46_Handler    (void) __attribute__ ((weak));
void Interrupt47_Handler    (void) __attribute__ ((weak));
void Interrupt48_Handler    (void) __attribute__ ((weak));
void Interrupt49_Handler    (void) __attribute__ ((weak));
void Interrupt50_Handler    (void) __attribute__ ((weak));
void Interrupt51_Handler    (void) __attribute__ ((weak));
void Interrupt52_Handler    (void) __attribute__ ((weak));
void Interrupt53_Handler    (void) __attribute__ ((weak));
void Interrupt54_Handler    (void) __attribute__ ((weak));
void Interrupt55_Handler    (void) __attribute__ ((weak));
void Interrupt56_Handler    (void) __attribute__ ((weak));
void Interrupt57_Handler    (void) __attribute__ ((weak));
void Interrupt58_Handler    (void) __attribute__ ((weak));
void Interrupt59_Handler    (void) __attribute__ ((weak));
void Interrupt60_Handler    (void) __attribute__ ((weak));
void Interrupt61_Handler    (void) __attribute__ ((weak));
void Interrupt62_Handler    (void) __attribute__ ((weak));
void Interrupt63_Handler    (void) __attribute__ ((weak));

/*----------------------------------------------------------------------------
  Exception / Interrupt Vector table
 *----------------------------------------------------------------------------*/

#if defined ( __GNUC__ )
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif

extern const VECTOR_TABLE_Type __VECTOR_TABLE[496];
       const VECTOR_TABLE_Type __VECTOR_TABLE[496] __VECTOR_TABLE_ATTRIBUTE = {
  (VECTOR_TABLE_Type)(&__INITIAL_SP),       /*     Initial Stack Pointer */
  Reset_Handler,                            /*     Reset Handler */
  NMI_Handler,                              /* -14 NMI Handler */
  HardFault_Handler,                        /* -13 Hard Fault Handler */
  MemManage_Handler,                        /* -12 MPU Fault Handler */
  BusFault_Handler,                         /* -11 Bus Fault Handler */
  UsageFault_Handler,                       /* -10 Usage Fault Handler */
  SecureFault_Handler,                      /*  -9 Secure Fault Handler */
  0,                                        /*     Reserved */
  0,                                        /*     Reserved */
  0,                                        /*     Reserved */
  SVC_Handler,                              /*  -5 SVCall Handler */
  DebugMon_Handler,                         /*  -4 Debug Monitor Handler */
  0,                                        /*     Reserved */
  PendSV_Handler,                           /*  -2 PendSV Handler */
  SysTick_Handler,                          /*  -1 SysTick Handler */

  /* Interrupts */
  Interrupt0_Handler,                       /*   0 Interrupt 0 */
  Interrupt1_Handler,                       /*   1 Interrupt 1 */
  Interrupt2_Handler,                       /*   2 Interrupt 2 */
  Interrupt3_Handler,                       /*   3 Interrupt 3 */
  Interrupt4_Handler,                       /*   4 Interrupt 4 */
  Interrupt5_Handler,                       /*   5 Interrupt 5 */
  Interrupt6_Handler,                       /*   6 Interrupt 6 */
  Interrupt7_Handler,                       /*   7 Interrupt 7 */
  Interrupt8_Handler,                       /*   8 Interrupt 8 */
  Interrupt9_Handler,                       /*   9 Interrupt 9 */
  Interrupt10_Handler,                      /*   10 Interrupt 10 */
  Interrupt11_Handler,                      /*   11 Interrupt 11 */
  Interrupt12_Handler,                      /*   12 Interrupt 12 */
  Interrupt13_Handler,                      /*   13 Interrupt 13 */
  Interrupt14_Handler,                      /*   14 Interrupt 14 */
  Interrupt15_Handler,                      /*   15 Interrupt 15 */
  Interrupt16_Handler,                      /*   16 Interrupt 16 */
  Interrupt17_Handler,                      /*   17 Interrupt 17 */
  Interrupt18_Handler,                      /*   18 Interrupt 18 */
  Interrupt19_Handler,                      /*   19 Interrupt 19 */
  Interrupt20_Handler,                      /*   20 Interrupt 20 */
  Interrupt21_Handler,                      /*   21 Interrupt 21 */
  Interrupt22_Handler,                      /*   22 Interrupt 22 */
  Interrupt23_Handler,                      /*   23 Interrupt 23 */
  Interrupt24_Handler,                      /*   24 Interrupt 24 */
  Interrupt25_Handler,                      /*   25 Interrupt 25 */
  Interrupt26_Handler,                      /*   26 Interrupt 26 */
  Interrupt27_Handler,                      /*   27 Interrupt 27 */
  Interrupt28_Handler,                      /*   28 Interrupt 28 */
  Interrupt29_Handler,                      /*   29 Interrupt 29 */
  Interrupt30_Handler,                      /*   30 Interrupt 30 */
  Interrupt31_Handler,                      /*   31 Interrupt 31 */
  Interrupt32_Handler,                      /*   32 Interrupt 32 */
  Interrupt33_Handler,                      /*   33 Interrupt 33 */
  Interrupt34_Handler,                      /*   34 Interrupt 34 */
  Interrupt35_Handler,                      /*   35 Interrupt 35 */
  Interrupt36_Handler,                      /*   36 Interrupt 36 */
  Interrupt37_Handler,                      /*   37 Interrupt 37 */
  Interrupt38_Handler,                      /*   38 Interrupt 38 */
  Interrupt39_Handler,                      /*   39 Interrupt 39 */
  Interrupt40_Handler,                      /*   40 Interrupt 40 */
  Interrupt41_Handler,                      /*   41 Interrupt 41 */
  Interrupt42_Handler,                      /*   42 Interrupt 42 */
  Interrupt43_Handler,                      /*   43 Interrupt 43 */
  Interrupt44_Handler,                      /*   44 Interrupt 44 */
  Interrupt45_Handler,                      /*   45 Interrupt 45 */
  Interrupt46_Handler,                      /*   46 Interrupt 46 */
  Interrupt47_Handler,                      /*   47 Interrupt 47 */
  Interrupt48_Handler,                      /*   48 Interrupt 48 */
  Interrupt49_Handler,                      /*   49 Interrupt 49 */
  Interrupt50_Handler,                      /*   50 Interrupt 50 */
  Interrupt51_Handler,                      /*   51 Interrupt 51 */
  Interrupt52_Handler,                      /*   52 Interrupt 52 */
  Interrupt53_Handler,                      /*   53 Interrupt 53 */
  Interrupt54_Handler,                      /*   54 Interrupt 54 */
  Interrupt55_Handler,                      /*   55 Interrupt 55 */
  Interrupt56_Handler,                      /*   56 Interrupt 56 */
  Interrupt57_Handler,                      /*   57 Interrupt 57 */
  Interrupt58_Handler,                      /*   58 Interrupt 58 */
  Interrupt59_Handler,                      /*   59 Interrupt 59 */
  Interrupt60_Handler,                      /*   60 Interrupt 60 */
  Interrupt61_Handler,                      /*   61 Interrupt 61 */
  Interrupt62_Handler,                      /*   62 Interrupt 62 */
  Interrupt63_Handler,                      /*   63 Interrupt 63 */
                                            /* Interrupts 64 .. 480 are left out */
};

#if defined ( __GNUC__ )
#pragma GCC diagnostic pop
#endif

/*----------------------------------------------------------------------------
  Reset Handler called on controller reset
 *----------------------------------------------------------------------------*/
__NO_RETURN void Reset_Handler(void)
{
  __set_MSPLIM((uint32_t)(&__STACK_LIMIT));

  SystemInit();                             /* CMSIS System Initialization */
  __PROGRAM_START();                        /* Enter PreMain (C library entry point) */
}


#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wmissing-noreturn"
#endif

/*----------------------------------------------------------------------------
  Hard Fault Handler
 *----------------------------------------------------------------------------*/
void HardFault_Handler(void)
{
  BK_LOGW(TAG, "HardFault_Handler\r\n");
  while(1);
}

/*----------------------------------------------------------------------------
  Default Handler for Exceptions / Interrupts
 *----------------------------------------------------------------------------*/
void Default_Handler(void)
{
  BK_LOGW(TAG, "Default_Handler\r\n");
  while(1);
}

#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
  #pragma clang diagnostic pop
#endif

