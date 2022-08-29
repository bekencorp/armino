// Copyright 2022-2023 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

typedef enum _IRQn_Type {
    NonMaskableInt_IRQn         = -14,  /* Non Maskable Interrupt */
    HardFault_IRQn              = -13,  /* HardFault Interrupt */
    MemoryManagement_IRQn       = -12,  /* Memory Management Interrupt */
    BusFault_IRQn               = -11,  /* Bus Fault Interrupt */
    UsageFault_IRQn             = -10,  /* Usage Fault Interrupt */
    SecureFault_IRQn            = -9,   /* Secure Fault Interrupt */
    SVCall_IRQn                 = -5,   /* SV Call Interrupt */
    DebugMonitor_IRQn           = -4,   /* Debug Monitor Interrupt */
    PendSV_IRQn                 = -2,   /* Pend SV Interrupt */
    SysTick_IRQn                = -1,   /* System Tick Interrupt */
    NONSEC_WATCHDOG_RESET_IRQn  = 0,    /* Non-Secure Watchdog Reset
                                         * Interrupt */
    DMA_NS_IRQn                 = 1,    /* DMA NS Interrupt */
    ENCP_S_IRQn                 = 2,    /* ENCP S Interrupt */
    ENCP_NS_IRQn                = 3,    /* ENCP NS Interrupt */
    TIMER0_IRQn                 = 4,    /* TIMER 0 Interrupt */
    UART0_IRQn                  = 5,    /* Uart 0 Interrupt */
}IRQn_Type;
