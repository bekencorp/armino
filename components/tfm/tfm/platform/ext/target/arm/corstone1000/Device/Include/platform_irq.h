/*
 * Copyright (c) 2019-2021 Arm Limited. All rights reserved.
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

typedef enum IRQn
{
    Reset_IRQn                   = -15, /* Reset Vector                               */
    NonMaskableInt_IRQn          = -14, /* Non maskable Interrupt                     */
    HardFault_IRQn               = -13, /* Hard Fault, all classes of Fault           */
    SVCall_IRQn                  =  -5, /* System Service Call via SVC instruction    */
    PendSV_IRQn                  =  -2, /* Pendable request for system service        */
    SysTick_IRQn                 =  -1, /* System Tick Timer                          */
    /* SoC Specific Interrupt Numbers */
    SE_INTERRUPT_EXPANSION_IRQn  = 0,   /* Secure Enclave Intrrupt Expansion          */
    CRYPTO_ACCELERATOR_0_IRQn    = 1,   /* Crypto Accelerator Interrupt 0             */
    CRYPTO_ACCELERATOR_1_IRQn    = 2,   /* Crypto Accelerator Interrupt 1             */
    SE_WATCHDOG_TIMER_IRQn       = 3,   /* Secure Enclave Watchdog Timer Interrupt    */
    /* Reserved                  = 4,   */
    TIMER0_IRQn                  = 5,   /* CMSDK Timer 0 Interrupt                    */
    TIMER1_IRQn                  = 6,   /* CMSDK Timer 1 Interrupt                    */
    HS_FIREWALL_TAMPER_IRQn      = 7,   /* Host System Firewall Tamper Interrupt      */
    INTERRUPT_ROUTER_TAMPER_IRQn = 8,   /* Interrupt Router Tamper Interrupt          */
    SECURE_WATCHDOG_WS1_IRQn     = 9,   /* Secure Watchdog WS1 Interrupt              */
    SECNENCTOP_PPU_IRQn          = 10,  /* SECNENCTOP PPU Interrupt                   */
    UART_UARTINTR_IRQn           = 11,  /* UART UARTINTR Interrupt                    */
    SE_FIREWALL_INTERRUPT_IRQn   = 12,  /* Secure Enclave Firewall Interrupt          */
    SE_CTI_TRIGGER_OUT_2_IRQn    = 13,  /* Secure Enclave CTI Trigger Out 2           */
    SE_CTI_TRIGGER_OUT_3_IRQn    = 14,  /* Secure Enclave CTI Trigger Out 3           */
    /* Reserved                  = 15,  */
    /* Reserved                  = 16,  */
    /* Reserved                  = 17,  */
    /* Reserved                  = 18,  */
    /* Reserved                  = 19,  */
    /* Reserved                  = 20,  */
    SEH0_SENDER_COMBINED_IRQn    = 21,  /* SEH0 Sender Combined Interrupt             */
    /* Reserved                  = 22,  */
    HSE0_RECEIVER_COMBINED_IRQn  = 23,  /* HSE0 Receiver Combined Interrupt           */
    /* Reserved                  = 24,  */
    /* Reserved                  = 25,  */
    SEH1_SENDER_COMBINED_IRQn    = 26,  /* SEH1 Sender Combined Interrupt             */
    /* Reserved                  = 27,  */
    HSE1_RECEIVER_COMBINED_IRQn  = 28,  /* HSE1 Receiver Combined Interrupt           */
    /* Reserved                  = 29,  */
    /* Reserved                  = 30,  */
    /* Reserved                  = 31,  */
} IRQn_Type;

#ifdef __cplusplus
}
#endif

#endif  /* __PLATFORM_IRQ_H__ */
