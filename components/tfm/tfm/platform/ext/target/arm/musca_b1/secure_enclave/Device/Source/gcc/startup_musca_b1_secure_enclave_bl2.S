;/*
; * Copyright (c) 2009-2020 Arm Limited. All rights reserved.
; *
; * Licensed under the Apache License, Version 2.0 (the "License");
; * you may not use this file except in compliance with the License.
; * You may obtain a copy of the License at
; *
; *     http://www.apache.org/licenses/LICENSE-2.0
; *
; * Unless required by applicable law or agreed to in writing, software
; * distributed under the License is distributed on an "AS IS" BASIS,
; * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; * See the License for the specific language governing permissions and
; * limitations under the License.
; *
; *
; * This file is derivative of CMSIS V5.00 startup_ARMCM33.S
; */

#include "tfm_plat_config.h"

    .syntax    unified
    .arch    armv6-m

    .section .vectors
    .align 2
    .globl    __Vectors
__Vectors:
    .long    Image$$ARM_LIB_STACK$$ZI$$Limit   /* Top of Stack */
    .long    Reset_Handler                     /*     Reset Handler */
    .long    NMI_Handler                       /* -14 NMI Handler */
    .long    HardFault_Handler                 /* -13 Hard Fault Handler */
    .long    0                                 /*     Reserved */
    .long    0                                 /*     Reserved */
    .long    0                                 /*     Reserved */
    .long    0                                 /*     Reserved */
    .long    0                                 /*     Reserved */
    .long    0                                 /*     Reserved */
    .long    0                                 /*     Reserved */
    .long    SVC_Handler                       /*  -5 SVCall Handler */
    .long    0                                 /*     Reserved */
    .long    0                                 /*     Reserved */
    .long    PendSV_Handler                    /*  -2 PendSV Handler */
    .long    SysTick_Handler                   /*  -1 SysTick Handler */

    /* Core interrupts */
    .long    ExternalIRQ_Handler               /*   0: */
    .long    CC312_Handler                     /*   1: */
    .long    CC312_APB_C_Handler               /*   2: */
    .long    Watchdog_Handler                  /*   3: */
    .long    0                                 /*   4: Reserved */
    .long    Timer0_Handler                    /*   5: */
    .long    Timer1_Handler                    /*   6: */
    .long    0                                 /*   7: Reserved */
    .long    0                                 /*   8: Reserved */
    .long    GPIO_Handler                      /*   9: */
    .long    GPIO_0_Handler                    /*  10: */
    .long    GPIO_1_Handler                    /*  11: */
    .long    GPIO_2_Handler                    /*  12: */
    .long    GPIO_3_Handler                    /*  13: */
    .long    GPIO_4_Handler                    /*  14: */
    .long    GPIO_5_Handler                    /*  15: */
    .long    GPIO_6_Handler                    /*  16: */
    .long    GPIO_7_Handler                    /*  17: */
    .long    0                                 /*  18: */
    .long    0                                 /*  19: */
    .long    0                                 /*  20: */
    .long    MHU0_NR2R_Handler                 /*  21: */
    .long    MHU0_R2NR_Handler                 /*  22: */
    .long    MHU0_COMB_Handler                 /*  23: */
    .long    MHU0_MSG_0_Handler                /*  24: */
    .long    MHU0_MSG_1_Handler                /*  25: */
    .long    MHU1_NR2R_Handler                 /*  26: */
    .long    MHU1_R2NR_Handler                 /*  27: */
    .long    MHU1_COMB_Handler                 /*  28: */
    .long    MHU1_MSG_0_Handler                /*  29: */
    .long    MHU1_MSG_1_Handler                /*  30: */
    .long    0                                 /*  31: */

    .size    __Vectors, . - __Vectors

    .text
    .thumb
    .thumb_func
    .align    2
    .globl    Reset_Handler
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


    .align    1
    .thumb_func
    .type    Default_Handler, %function
Default_Handler:
    b        Default_Handler
    .size    Default_Handler, . - Default_Handler

/* Macro to define default exception/interrupt handlers.
 * Default handler are weak symbols with an endless loop.
 * They can be overwritten by real handlers.
 */
    .macro   def_irq_handler  Handler_Name
    .weak    \Handler_Name
    .set     \Handler_Name, Default_Handler
    .endm

    def_irq_handler NMI_Handler
    def_irq_handler HardFault_Handler
    def_irq_handler SVC_Handler
    def_irq_handler PendSV_Handler
    def_irq_handler SysTick_Handler

    def_irq_handler ExternalIRQ_Handler
    def_irq_handler CC312_Handler
    def_irq_handler CC312_APB_C_Handler
    def_irq_handler Watchdog_Handler

    def_irq_handler Timer0_Handler
    def_irq_handler Timer1_Handler

    def_irq_handler GPIO_Handler
    def_irq_handler GPIO_0_Handler
    def_irq_handler GPIO_1_Handler
    def_irq_handler GPIO_2_Handler
    def_irq_handler GPIO_3_Handler
    def_irq_handler GPIO_4_Handler
    def_irq_handler GPIO_5_Handler
    def_irq_handler GPIO_6_Handler
    def_irq_handler GPIO_7_Handler

    def_irq_handler MHU0_NR2R_Handler
    def_irq_handler MHU0_R2NR_Handler
    def_irq_handler MHU0_COMB_Handler
    def_irq_handler MHU0_MSG_0_Handler
    def_irq_handler MHU0_MSG_1_Handler
    def_irq_handler MHU1_NR2R_Handler
    def_irq_handler MHU1_R2NR_Handler
    def_irq_handler MHU1_COMB_Handler
    def_irq_handler MHU1_MSG_0_Handler
    def_irq_handler MHU1_MSG_1_Handler

    .end
