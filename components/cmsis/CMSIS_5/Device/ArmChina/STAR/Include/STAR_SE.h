/**************************************************************************//**
 * @file     STAR_SE.h
 * @brief    CMSIS Core Peripheral Access Layer Header File for
 *           ArmChina STAR Device (configured for STAR with FPU, with DSP extension, with MemorySubSystem improvement, with TrustZone)
 * @version  V1.0.0
 * @date     25. June 2020
 ******************************************************************************/
/*
 * Copyright (c) 2009-2018 Arm Limited.
 * Copyright (c) 2018-2020 ArmChina.
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

#ifndef __STAR_SE_H
#define __STAR_SE_H

#ifdef __cplusplus
extern "C" {
#endif

//to do...............cache and tcm...............

//to do...............core_STAR.h ...............

/* -------------------------  Interrupt Number Definition  ------------------------ */

typedef enum IRQn
{
/* -------------------  Processor Exceptions Numbers  ----------------------------- */
  NonMaskableInt_IRQn           = -14,     /*  2 Non Maskable Interrupt */
  HardFault_IRQn                = -13,     /*  3 HardFault Interrupt */
  MemoryManagement_IRQn         = -12,     /*  4 Memory Management Interrupt */
  BusFault_IRQn                 = -11,     /*  5 Bus Fault Interrupt */
  UsageFault_IRQn               = -10,     /*  6 Usage Fault Interrupt */
  SecureFault_IRQn              =  -9,     /*  7 Secure Fault Interrupt */
  SVCall_IRQn                   =  -5,     /* 11 SV Call Interrupt */
  DebugMonitor_IRQn             =  -4,     /* 12 Debug Monitor Interrupt */
  PendSV_IRQn                   =  -2,     /* 14 Pend SV Interrupt */
  SysTick_IRQn                  =  -1,     /* 15 System Tick Interrupt */

/* -------------------  Processor Interrupt Numbers  ------------------------------ */
  Interrupt0_IRQn               =   0,
  Interrupt1_IRQn               =   1,
  Interrupt2_IRQn               =   2,
  Interrupt3_IRQn               =   3,
  Interrupt4_IRQn               =   4,
  Interrupt5_IRQn               =   5,
  Interrupt6_IRQn               =   6,
  Interrupt7_IRQn               =   7,
  Interrupt8_IRQn               =   8,
  Interrupt9_IRQn               =   9,
  Interrupt10_IRQn              =   10,
  Interrupt11_IRQn              =   11,
  Interrupt12_IRQn              =   12,
  Interrupt13_IRQn              =   13,
  Interrupt14_IRQn              =   14,
  Interrupt15_IRQn              =   15,
  Interrupt16_IRQn              =   16,
  Interrupt17_IRQn              =   17,
  Interrupt18_IRQn              =   18,
  Interrupt19_IRQn              =   19,
  Interrupt20_IRQn              =   20,
  Interrupt21_IRQn              =   21,
  Interrupt22_IRQn              =   22,
  Interrupt23_IRQn              =   23,
  Interrupt24_IRQn              =   24,
  Interrupt25_IRQn              =   25,
  Interrupt26_IRQn              =   26,
  Interrupt27_IRQn              =   27,
  Interrupt28_IRQn              =   28,
  Interrupt29_IRQn              =   29,
  Interrupt30_IRQn              =   30,
  Interrupt31_IRQn              =   31,
  Interrupt32_IRQn              =   32,
  Interrupt33_IRQn              =   33,
  Interrupt34_IRQn              =   34,
  Interrupt35_IRQn              =   35,
  Interrupt36_IRQn              =   36,
  Interrupt37_IRQn              =   37,
  Interrupt38_IRQn              =   38,
  Interrupt39_IRQn              =   39,
  Interrupt40_IRQn              =   40,
  Interrupt41_IRQn              =   41,
  Interrupt42_IRQn              =   42,
  Interrupt43_IRQn              =   43,
  Interrupt44_IRQn              =   44,
  Interrupt45_IRQn              =   45,
  Interrupt46_IRQn              =   46,
  Interrupt47_IRQn              =   47,
  Interrupt48_IRQn              =   48,
  Interrupt49_IRQn              =   49,
  Interrupt50_IRQn              =   50,
  Interrupt51_IRQn              =   51,
  Interrupt52_IRQn              =   52,
  Interrupt53_IRQn              =   53,
  Interrupt54_IRQn              =   54,
  Interrupt55_IRQn              =   55,
  Interrupt56_IRQn              =   56,
  Interrupt57_IRQn              =   57,
  Interrupt58_IRQn              =   58,
  Interrupt59_IRQn              =   59,
  Interrupt60_IRQn              =   60,
  Interrupt61_IRQn              =   61,
  Interrupt62_IRQn              =   62,
  Interrupt63_IRQn              =   63,

  InterruptMAX_IRQn
  /* Interrupts 64 .. 480 are left out */
} IRQn_Type;


/* ================================================================================ */
/* ================      Processor and Core Peripheral Section     ================ */
/* ================================================================================ */

/* -------  Start of section using anonymous unions and disabling warnings  ------- */
#if   defined (__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined (__ICCARM__)
  #pragma language=extended
#elif defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wc11-extensions"
  #pragma clang diagnostic ignored "-Wreserved-id-macro"
#elif defined (__GNUC__)
  /* anonymous unions are enabled by default */
#elif defined (__TMS470__)
  /* anonymous unions are enabled by default */
#elif defined (__TASKING__)
  #pragma warning 586
#elif defined (__CSMC__)
  /* anonymous unions are enabled by default */
#else
  #warning Not supported compiler type
#endif


/* --------  Configuration of Core Peripherals  ----------------------------------- */
#define __STAR_REV                0x0100U   /* Core revision r1p0 */
#define __SAUREGION_PRESENT       1U        /* SAU regions present */
#define __MPU_PRESENT             1U        /* MPU present */
#define __VTOR_PRESENT            1U        /* VTOR present */
#define __NVIC_PRIO_BITS          3U        /* Number of Bits used for Priority Levels */
#define __Vendor_SysTickConfig    0U        /* Set to 1 if different SysTick Config is used */
#define __FPU_PRESENT             1U        /* FPU present */
#define __DSP_PRESENT             1U        /* DSP extension present */
#define __ICACHE_PRESENT          1U        /* Defines whether ICACHE is present or not in this device */
#define __DCACHE_PRESENT          1U        /* Defines whether DCACHE is present or not in this device */
#define __ITCM_PRESENT            1U        /* Defines whether ITCM is present or not in this device */
#define __DTCM_PRESENT            1U        /* Defines whether DTCM is present or not in this device */


#include "core_star.h"                      /* Processor and core peripherals */
#include "system_STAR.h"                 /* System Header */


/* --------  End of section using anonymous unions and disabling warnings  -------- */
#if   defined (__CC_ARM)
  #pragma pop
#elif defined (__ICCARM__)
  /* leave anonymous unions enabled */
#elif (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))
  #pragma clang diagnostic pop
#elif defined (__GNUC__)
  /* anonymous unions are enabled by default */
#elif defined (__TMS470__)
  /* anonymous unions are enabled by default */
#elif defined (__TASKING__)
  #pragma warning restore
#elif defined (__CSMC__)
  /* anonymous unions are enabled by default */
#else
  #warning Not supported compiler type
#endif


#ifdef __cplusplus
}
#endif

#endif  /* STAR_SE_H */
