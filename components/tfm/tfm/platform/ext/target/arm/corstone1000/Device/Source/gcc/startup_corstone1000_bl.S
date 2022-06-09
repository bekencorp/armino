/*
 * Copyright (c) 2009-2021 Arm Limited. All rights reserved.
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

#include "tfm_plat_config.h"

    .syntax  unified
    .arch    armv6-m

    .section .vectors
    .align   2
    .globl   __Vectors
__Vectors:
    .long    Image$$ARM_LIB_STACK$$ZI$$Limit    /*     Top of Stack */
    .long    Reset_Handler                      /*     Reset Handler */
    .long    NMI_Handler                        /* -14 NMI Handler */
    .long    HardFault_Handler                  /* -13 Hard Fault Handler */
    .long    0                                  /*     Reserved */
    .long    0                                  /*     Reserved */
    .long    0                                  /*     Reserved */
    .long    0                                  /*     Reserved */
    .long    0                                  /*     Reserved */
    .long    0                                  /*     Reserved */
    .long    0                                  /*     Reserved */
    .long    SVC_Handler                        /*  -5 SVCall Handler */
    .long    0                                  /*     Reserved */
    .long    0                                  /*     Reserved */
    .long    PendSV_Handler                     /*  -2 PendSV Handler */
    .long    SysTick_Handler                    /*  -1 SysTick Handler */

     /* SoC Specific Interrupts */
    .long    SE_INTERRUPT_EXPANSION_IRQHandler  /*   0: Secure Enclave Interrupt Expansion */
    .long    CRYPTO_ACCELERATOR_0_IRQHandler    /*   1: Crypto Accelerator Interrupt 0 */
    .long    CRYPTO_ACCELERATOR_1_IRQHandler    /*   2: Crypto Accelerator Interrupt 1 */
    .long    SE_WATCHDOG_TIMER_IRQHandler       /*   3: Secure Enclave Watchdog Timer Interrupt */
    .long    0                                  /*   4: Reserved */
    .long    TIMER0_IRQHandler                  /*   5: CMSDK Timer 0 Interrupt */
    .long    TIMER1_IRQHandler                  /*   6: CMSDK Timer 1 Interrupt */
    .long    HS_FIREWALL_TAMPER_IRQHandler      /*   7: Host System Firewall Tamper Interrupt */
    .long    INTERRUPT_ROUTER_TAMPER_IRQHandler /*   8: Interrupt Router Tamper Interrupt */
    .long    SECURE_WATCHDOG_WS1_IRQHandler     /*   9: Secure Watchdog WS1 Interrupt */
    .long    SECNENCTOP_PPU_IRQHandler          /*  10: SECNENCTOP PPU Interrupt */
    .long    UART_UARTINTR_IRQHandler           /*  11: UART UARTINTR Interrupt */
    .long    SE_FIREWALL_INTERRUPT_IRQHandler   /*  12: Secure Enclave Firewall Interrupt */
    .long    SE_CTI_TRIGGER_OUT_2_IRQHandler    /*  13: Secure Enclave CTI Trigger Out 2 */
    .long    SE_CTI_TRIGGER_OUT_3_IRQHandler    /*  14: Secure Enclave CTI Trigger Out 3 */
    .long    0                                  /*  15: Reserved */
    .long    0                                  /*  16: Reserved */
    .long    0                                  /*  17: Reserved */
    .long    0                                  /*  18: Reserved */
    .long    0                                  /*  19: Reserved */
    .long    0                                  /*  20: Reserved */
    .long    SEH0_SENDER_COMBINED_IRQHandler    /*  21: SEH0 Sender Combined Interrupt */
    .long    0                                  /*  22: Reserved */
    .long    HSE0_RECEIVER_COMBINED_IRQHandler  /*  23: HSE0 Receiver Combined Interrupt */
    .long    0                                  /*  24: Reserved */
    .long    0                                  /*  25: Reserved */
    .long    SEH1_SENDER_COMBINED_IRQHandler    /*  26: SEH1 Sender Combined Interrupt */
    .long    0                                  /*  27: Reserved */
    .long    HSE1_RECEIVER_COMBINED_IRQHandler  /*  28: HSE1 Receiver Combined Interrupt */
    .long    0                                  /*  29: Reserved */
    .long    0                                  /*  30: Reserved */
    .long    0                                  /*  31: Reserved */

    .size    __Vectors, . - __Vectors

    .text
    .thumb
    .thumb_func
    .align   2
    .globl   Reset_Handler
    .type    Reset_Handler, %function

Reset_Handler:
/*  Firstly it copies data from read only memory to RAM. There are two schemes
 *  to copy. One can copy more than one sections. Another can only copy
 *  one section.  The former scheme needs more instructions and read-only
 *  data to implement than the latter.
 *  Macro __STARTUP_COPY_MULTIPLE is used to choose between two schemes.  */

#ifdef __STARTUP_COPY_MULTIPLE
/*  Multiple sections scheme.
 *
 *  Between symbol address __copy_table_start__ and __copy_table_end__,
 *  there are array of triplets, each of which specify:
 *    offset 0: LMA of start of a section to copy from
 *    offset 4: VMA of start of a section to copy to
 *    offset 8: size of the section to copy. Must be multiply of 4
 *
 *  All addresses must be aligned to 4 bytes boundary.
 */
    ldr    r4, =__copy_table_start__
    ldr    r5, =__copy_table_end__

.L_loop0:
    cmp    r4, r5
    bge    .L_loop0_done
    ldr    r1, [r4]
    ldr    r2, [r4, #4]
    ldr    r3, [r4, #8]

.L_loop0_0:
    subs    r3, #4
    blt    .L_loop0_0_done
    ldr    r0, [r1, r3]
    str    r0, [r2, r3]
    b    .L_loop0_0

.L_loop0_0_done:
    adds    r4, #12
    b    .L_loop0

.L_loop0_done:
#else
/*  Single section scheme.
 *
 *  The ranges of copy from/to are specified by following symbols
 *    __etext: LMA of start of the section to copy from. Usually end of text
 *    __data_start__: VMA of start of the section to copy to
 *    __data_end__: VMA of end of the section to copy to
 *
 *  All addresses must be aligned to 4 bytes boundary.
 */
    ldr    r1, =__etext
    ldr    r2, =__data_start__
    ldr    r3, =__data_end__

    subs    r3, r2
    ble    .L_loop1_done

.L_loop1:
    subs    r3, #4
    ldr    r0, [r1,r3]
    str    r0, [r2,r3]
    bgt    .L_loop1

.L_loop1_done:
#endif /*__STARTUP_COPY_MULTIPLE */

/*  This part of work usually is done in C library startup code. Otherwise,
 *  define this macro to enable it in this startup.
 *
 *  There are two schemes too. One can clear multiple BSS sections. Another
 *  can only clear one section. The former is more size expensive than the
 *  latter.
 *
 *  Define macro __STARTUP_CLEAR_BSS_MULTIPLE to choose the former.
 *  Otherwise efine macro __STARTUP_CLEAR_BSS to choose the later.
 */
#ifdef __STARTUP_CLEAR_BSS_MULTIPLE
/*  Multiple sections scheme.
 *
 *  Between symbol address __copy_table_start__ and __copy_table_end__,
 *  there are array of tuples specifying:
 *    offset 0: Start of a BSS section
 *    offset 4: Size of this BSS section. Must be multiply of 4
 */
    ldr    r3, =__zero_table_start__
    ldr    r4, =__zero_table_end__

.L_loop2:
    cmp    r3, r4
    bge    .L_loop2_done
    ldr    r1, [r3]
    ldr    r2, [r3, #4]
    movs    r0, 0

.L_loop2_0:
    subs    r2, #4
    blt    .L_loop2_0_done
    str    r0, [r1, r2]
    b    .L_loop2_0
.L_loop2_0_done:

    adds    r3, #8
    b    .L_loop2
.L_loop2_done:
#elif defined (__STARTUP_CLEAR_BSS)
/*  Single BSS section scheme.
 *
 *  The BSS section is specified by following symbols
 *    __bss_start__: start of the BSS section.
 *    __bss_end__: end of the BSS section.
 *
 *  Both addresses must be aligned to 4 bytes boundary.
 */
    ldr    r1, =__bss_start__
    ldr    r2, =__bss_end__

    movs    r0, 0

    subs    r2, r1
    ble    .L_loop3_done

.L_loop3:
    subs    r2, #4
    str    r0, [r1, r2]
    bgt    .L_loop3
.L_loop3_done:
#endif /* __STARTUP_CLEAR_BSS_MULTIPLE || __STARTUP_CLEAR_BSS */

    bl    SystemInit

#ifndef __START
#define __START _start
#endif
    bl    __START

    .pool
    .size    Reset_Handler, . - Reset_Handler


/* Macro to define default exception/interrupt handlers.
 * Default handler are weak symbols with an endless loop.
 * They can be overwritten by real handlers.
 */
    .macro   Set_Default_Handler Handler_Name
    .weak    \Handler_Name
    .align    1
    .thumb_func
    .type    \Handler_Name, %function
\Handler_Name:
    b        .
    .endm


/* Default exception/interrupt handler */

    Set_Default_Handler  NMI_Handler
    Set_Default_Handler  HardFault_Handler
    Set_Default_Handler  SVC_Handler
    Set_Default_Handler  PendSV_Handler
    Set_Default_Handler  SysTick_Handler

    Set_Default_Handler SE_INTERRUPT_EXPANSION_IRQHandler
    Set_Default_Handler CRYPTO_ACCELERATOR_0_IRQHandler
    Set_Default_Handler CRYPTO_ACCELERATOR_1_IRQHandler
    Set_Default_Handler SE_WATCHDOG_TIMER_IRQHandler
    Set_Default_Handler TIMER0_IRQHandler
    Set_Default_Handler TIMER1_IRQHandler
    Set_Default_Handler HS_FIREWALL_TAMPER_IRQHandler
    Set_Default_Handler INTERRUPT_ROUTER_TAMPER_IRQHandler
    Set_Default_Handler SECURE_WATCHDOG_WS1_IRQHandler
    Set_Default_Handler SECNENCTOP_PPU_IRQHandler
    Set_Default_Handler UART_UARTINTR_IRQHandler
    Set_Default_Handler SE_FIREWALL_INTERRUPT_IRQHandler
    Set_Default_Handler SE_CTI_TRIGGER_OUT_2_IRQHandler
    Set_Default_Handler SE_CTI_TRIGGER_OUT_3_IRQHandler
    Set_Default_Handler SEH0_SENDER_COMBINED_IRQHandler
    Set_Default_Handler HSE0_RECEIVER_COMBINED_IRQHandler
    Set_Default_Handler SEH1_SENDER_COMBINED_IRQHandler
    Set_Default_Handler HSE1_RECEIVER_COMBINED_IRQHandler

    .end
