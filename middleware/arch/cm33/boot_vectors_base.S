/*
 * MPS2 CMSIS Library
 */
/*
 * Copyright (c) 2009-2018 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/*
 * This file is derivative of CMSIS V5.00 startup_ARMCM3.S
 */
#include "asm_macros.h"
    .text
    .syntax unified
    .thumb

    .section .ext_irq_table,"a",%progbits
    .align 2
    .globl __ext_irq_vector
__ext_irq_vector:
    /* External Interrupts */
    .long    UART0_RX_IRQHandler    /*   0:  UART 0 RX Interrupt            */
    .long    UART0_TX_IRQHandler    /*   1:  UART 0 TX Combined Interrupt   */
    .long    UART1_RX_IRQHandler    /*   2:  UART 1 RX Interrupt            */
    .long    UART1_TX_IRQHandler    /*   3:  UART 1 TX Combined Interrupt   */
    .long    UART2_RX_IRQHandler    /*   4:  UART 2 RX Interrupt            */
    .long    UART2_TX_IRQHandler    /*   5:  UART 2 TX Combined Interrupt   */

    .long    PORT0_IRQHandler        /*   6:  GPIO Port 0 combined Interrupt              */
    .long    PORT1_ALL_IRQHandler    /*   7:  GPIO Port 1 combined Interrupt              */
    .long    TIMER0_IRQHandler       /*   8:  TIMER 0 Interrupt                           */
    .long    TIMER1_IRQHandler       /*   9:  TIMER 1 Interrupt                           */
    .long    DUALTIMER_IRQHandler    /*   10: Dual Timer Interrupt                        */
    .long    MPS2_SPI0_IRQHandler    /*   11: SPI Interrupt (spi header)                  */
    .long    UARTOVF_IRQHandler      /*   12: UART 0,1,2 Overflow Interrupt               */
    .long    ETHERNET_IRQHandler     /*   13: Reserved for APB Slave                      */
    .long    I2S_IRQHandler          /*   14: I2S Interrupt                               */
    .long    DMA_DONE__IRQHandler    /*   15: I2S Interrupt                               */
    .long    PORT0_0_IRQHandler      /*   16: GPIO Port 0 pin 0 Handler                   */
    .long    PORT0_1_IRQHandler      /*   17: GPIO Port 0 pin 1 Handler                   */
    .long    PORT0_2_IRQHandler      /*   18: GPIO Port 0 pin 2 Handler                   */
    .long    PORT0_3_IRQHandler      /*   19: GPIO Port 0 pin 3 Handler                   */
    .long    PORT0_4_IRQHandler      /*   20: GPIO Port 0 pin 4 Handler                   */
    .long    PORT0_5_IRQHandler      /*   21: GPIO Port 0 pin 5 Handler                   */
    .long    PORT0_6_IRQHandler      /*   22: GPIO Port 0 pin 6 Handler                   */
    .long    PORT0_7_IRQHandler      /*   23: GPIO Port 0 pin 7 Handler                   */
    .long    PORT0_8_IRQHandler      /*   24: GPIO Port 0 pin 8 Handler                   */
    .long    PORT0_9_IRQHandler      /*   25: GPIO Port 0 pin 9 Handler                   */
    .long    PORT0_10_IRQHandler     /*   26: GPIO Port 0 pin 10 Handler                  */
    .long    PORT0_11_IRQHandler     /*   27: GPIO Port 0 pin 11 Handler                  */
    .long    PORT0_12_IRQHandler     /*   28: GPIO Port 0 pin 12 Handler                  */
    .long    PORT0_13_IRQHandler     /*   29: GPIO Port 0 pin 13 Handler                  */
    .long    PORT0_14_IRQHandler     /*   30: GPIO Port 0 pin 14 Handler                  */
    .long    PORT0_15_IRQHandler     /*   31: GPIO Port 0 pin 15 Handler                  */
    .long    DUBHE_S_IRQHandler      /*   32:                                             */
    .long    DUBHE_NS_IRQHandler     /*   33:                                             */

    .size    __ext_irq_vector, . - __ext_irq_vector

    .text
    /* External interrupts */
    def_irq_default_handler     UART0_RX_IRQHandler
    def_irq_default_handler     UART0_TX_IRQHandler
    def_irq_default_handler     UART1_RX_IRQHandler
    def_irq_default_handler     UART1_TX_IRQHandler
    def_irq_default_handler     UART2_RX_IRQHandler
    def_irq_default_handler     UART2_TX_IRQHandler

    def_irq_default_handler     PORT0_IRQHandler
    def_irq_default_handler     PORT1_ALL_IRQHandler
    def_irq_default_handler     TIMER0_IRQHandler
    def_irq_default_handler     TIMER1_IRQHandler
    def_irq_default_handler     DUALTIMER_IRQHandler
    def_irq_default_handler     MPS2_SPI0_IRQHandler
    def_irq_default_handler     UARTOVF_IRQHandler
    def_default_handler         ETHERNET_IRQHandler
    def_irq_default_handler     I2S_IRQHandler
    def_irq_default_handler     DMA_DONE__IRQHandler
    def_irq_default_handler     PORT0_0_IRQHandler
    def_irq_default_handler     PORT0_1_IRQHandler
    def_irq_default_handler     PORT0_2_IRQHandler
    def_irq_default_handler     PORT0_3_IRQHandler
    def_irq_default_handler     PORT0_4_IRQHandler
    def_irq_default_handler     PORT0_5_IRQHandler
    def_irq_default_handler     PORT0_6_IRQHandler
    def_irq_default_handler     PORT0_7_IRQHandler
    def_irq_default_handler     PORT0_8_IRQHandler
    def_irq_default_handler     PORT0_9_IRQHandler
    def_irq_default_handler     PORT0_10_IRQHandler
    def_irq_default_handler     PORT0_11_IRQHandler
    def_irq_default_handler     PORT0_12_IRQHandler
    def_irq_default_handler     PORT0_13_IRQHandler
    def_irq_default_handler     PORT0_14_IRQHandler
    def_irq_default_handler     PORT0_15_IRQHandler

    def_irq_default_handler     DUBHE_S_IRQHandler
    def_irq_default_handler     DUBHE_NS_IRQHandler
    .end

