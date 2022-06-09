/*
 * Copyright (c) 2009-2020 ARM Limited. All rights reserved.
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

/*
 * This file is derivative of CMSIS V5.01 startup_ARMCM33.S
 * Git SHA: 8a1d9d6ee18b143ae5befefa14d89fb5b3f99c75
 */

    .syntax unified
    .arch   armv8-m.main

    .section .vectors
    .align  2
    .globl  __Vectors
__Vectors:
    .long   Image$$ARM_LIB_STACK_MSP$$ZI$$Limit /* Top of Stack */

    .long   Reset_Handler               /* Reset Handler */
    .long   NMI_Handler                 /* NMI Handler */
    .long   HardFault_Handler           /* Hard Fault Handler */
    .long   MemManage_Handler           /* MPU Fault Handler */
    .long   BusFault_Handler            /* Bus Fault Handler */
    .long   UsageFault_Handler          /* Usage Fault Handler */
    .long   0                           /* Reserved */
    .long   0                           /* Reserved */
    .long   0                           /* Reserved */
    .long   0                           /* Reserved */
    .long   SVC_Handler                 /* SVCall Handler */
    .long   DebugMon_Handler            /* Debug Monitor Handler */
    .long   0                           /* Reserved */
    .long   PendSV_Handler              /* PendSV Handler */
    .long   SysTick_Handler             /* SysTick Handler */

  /* External Interrupts */
    .long   FPU_IRQHandler
    .long   0                           /* Reserved */
    .long   0                           /* Reserved */
    .long   0                           /* Reserved */
    .long   0                           /* Reserved */
    .long   CLOCK_POWER_IRQHandler
    .long   0                           /* Reserved */
    .long   0                           /* Reserved */
    .long   SERIAL0_IRQHandler
    .long   SERIAL1_IRQHandler
    .long   SPIM4_IRQHandler
    .long   SERIAL2_IRQHandler
    .long   SERIAL3_IRQHandler
    .long   0                           /* Reserved */
    .long   SAADC_IRQHandler
    .long   TIMER0_IRQHandler
    .long   TIMER1_Handler
    .long   TIMER2_IRQHandler
    .long   0                           /* Reserved */
    .long   0                           /* Reserved */
    .long   RTC0_IRQHandler
    .long   RTC1_IRQHandler
    .long   0                           /* Reserved */
    .long   0                           /* Reserved */
    .long   WDT0_IRQHandler
    .long   WDT1_IRQHandler
    .long   COMP_LPCOMP_IRQHandler
    .long   EGU0_IRQHandler
    .long   EGU1_IRQHandler
    .long   EGU2_IRQHandler
    .long   EGU3_IRQHandler
    .long   EGU4_IRQHandler
    .long   EGU5_IRQHandler
    .long   PWM0_IRQHandler
    .long   PWM1_IRQHandler
    .long   PWM2_IRQHandler
    .long   PWM3_IRQHandler
    .long   0                           /* Reserved */
    .long   PDM0_IRQHandler
    .long   0                           /* Reserved */
    .long   I2S0_IRQHandler
    .long   0                           /* Reserved */
    .long   IPC_IRQHandler
    .long   QSPI_IRQHandler
    .long   0                           /* Reserved */
    .long   NFCT_IRQHandler
    .long   0                           /* Reserved */
    .long   GPIOTE1_IRQHandler
    .long   0                           /* Reserved */
    .long   0                           /* Reserved */
    .long   0                           /* Reserved */
    .long   QDEC0_IRQHandler
    .long   QDEC1_IRQHandler
    .long   0                           /* Reserved */
    .long   USBD_IRQHandler
    .long   USBREGULATOR_IRQHandler
    .long   0                           /* Reserved */
    .long   KMU_IRQHandler
    .long   0                           /* Reserved */
    .long   0                           /* Reserved */
    .long   0                           /* Reserved */
    .long   0                           /* Reserved */
    .long   0                           /* Reserved */
    .long   0                           /* Reserved */
    .long   0                           /* Reserved */
    .long   0                           /* Reserved */
    .long   0                           /* Reserved */
    .long   0                           /* Reserved */
    .long   0                           /* Reserved */

    .size   __Vectors, . - __Vectors

    .text
    .thumb
    .thumb_func
    .align  2
    .globl  Reset_Handler
    .type   Reset_Handler, %function
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
    ldr     r4, =__copy_table_start__
    ldr     r5, =__copy_table_end__

.L_loop0:
    cmp     r4, r5
    bge     .L_loop0_done
    ldr     r1, [r4]
    ldr     r2, [r4, #4]
    ldr     r3, [r4, #8]

.L_loop0_0:
    subs    r3, #4
    ittt    ge
    ldrge   r0, [r1, r3]
    strge   r0, [r2, r3]
    bge     .L_loop0_0

    adds    r4, #12
    b       .L_loop0

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
    ldr     r1, =__etext
    ldr     r2, =__data_start__
    ldr     r3, =__data_end__

.L_loop1:
    cmp     r2, r3
    ittt    lt
    ldrlt   r0, [r1], #4
    strlt   r0, [r2], #4
    blt     .L_loop1
#endif /* __STARTUP_COPY_MULTIPLE */

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
    ldr     r3, =__zero_table_start__
    ldr     r4, =__zero_table_end__

.L_loop2:
    cmp     r3, r4
    bge     .L_loop2_done
    ldr     r1, [r3]
    ldr     r2, [r3, #4]
    movs    r0, 0

.L_loop2_0:
    subs    r2, #4
    itt     ge
    strge   r0, [r1, r2]
    bge     .L_loop2_0

    adds    r3, #8
    b       .L_loop2
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
    ldr     r1, =__bss_start__
    ldr     r2, =__bss_end__

    movs    r0, 0
.L_loop3:
    cmp     r1, r2
    itt     lt
    strlt   r0, [r1], #4
    blt     .L_loop3
#endif /* __STARTUP_CLEAR_BSS_MULTIPLE || __STARTUP_CLEAR_BSS */

    mrs     r0, control    /* Get control value */
    orr     r0, r0, #2     /* Select switch to PSP */
    msr     control, r0
    ldr     r0, =Image$$ARM_LIB_STACK$$ZI$$Limit
    msr     psp, r0

/* Call _start function provided by libraries.
 * If those libraries are not accessible, define __START as your entry point.
 */
#ifndef __START
#define __START _start
#endif
    bl      __START

    .pool
    .size   Reset_Handler, . - Reset_Handler

    .align  1
    .thumb_func
    .weak   Default_Handler
    .type   Default_Handler, %function
Default_Handler:
    b   .
    .size   Default_Handler, . - Default_Handler

/*  Macro to define default handlers. */
    .macro  def_irq_handler handler_name
    .thumb_func
    .weak   \handler_name
\handler_name:
    b       \handler_name
    .endm

    def_irq_handler      NMI_Handler
    def_irq_handler      HardFault_Handler
    def_irq_handler      MemManage_Handler
    def_irq_handler      BusFault_Handler
    def_irq_handler      UsageFault_Handler
    def_irq_handler      SVC_Handler
    def_irq_handler      DebugMon_Handler
    def_irq_handler      PendSV_Handler
    def_irq_handler      SysTick_Handler

    def_irq_handler      FPU_IRQHandler
    def_irq_handler      CLOCK_POWER_IRQHandler
    def_irq_handler      SERIAL0_IRQHandler
    def_irq_handler      SERIAL1_IRQHandler
    def_irq_handler      SPIM4_IRQHandler
    def_irq_handler      SERIAL2_IRQHandler
    def_irq_handler      SERIAL3_IRQHandler
    def_irq_handler      SAADC_IRQHandler
    def_irq_handler      TIMER0_IRQHandler
    def_irq_handler      TIMER1_Handler
    def_irq_handler      TIMER2_IRQHandler
    def_irq_handler      RTC0_IRQHandler
    def_irq_handler      RTC1_IRQHandler
    def_irq_handler      WDT0_IRQHandler
    def_irq_handler      WDT1_IRQHandler
    def_irq_handler      COMP_LPCOMP_IRQHandler
    def_irq_handler      EGU0_IRQHandler
    def_irq_handler      EGU1_IRQHandler
    def_irq_handler      EGU2_IRQHandler
    def_irq_handler      EGU3_IRQHandler
    def_irq_handler      EGU4_IRQHandler
    def_irq_handler      EGU5_IRQHandler
    def_irq_handler      PWM0_IRQHandler
    def_irq_handler      PWM1_IRQHandler
    def_irq_handler      PWM2_IRQHandler
    def_irq_handler      PWM3_IRQHandler
    def_irq_handler      PDM0_IRQHandler
    def_irq_handler      I2S0_IRQHandler
    def_irq_handler      IPC_IRQHandler
    def_irq_handler      QSPI_IRQHandler
    def_irq_handler      NFCT_IRQHandler
    def_irq_handler      GPIOTE1_IRQHandler
    def_irq_handler      QDEC0_IRQHandler
    def_irq_handler      QDEC1_IRQHandler
    def_irq_handler      USBD_IRQHandler
    def_irq_handler      USBREGULATOR_IRQHandler
    def_irq_handler      KMU_IRQHandler

    .end
