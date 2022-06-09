;/*
; * Copyright (c) 2017-2021 ARM Limited
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
; */
;
; This file is derivative of CMSIS V5.01 startup_ARMv8MML.s
; Git SHA: 8a1d9d6ee18b143ae5befefa14d89fb5b3f99c75

;/*
;//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
;*/


; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

                MODULE   ?cstartup

                ;; Forward declaration of sections.
                SECTION  ARM_LIB_STACK:DATA:NOROOT(3)

                SECTION  .intvec:CODE:NOROOT(2)

                EXTERN   __iar_program_start
                EXTERN   SystemInit
                PUBLIC   __vector_table
                PUBLIC   __Vectors
                PUBLIC   __Vectors_End
                PUBLIC   __Vectors_Size

                DATA

__vector_table      ;Core Interrupts
                DCD     sfe(ARM_LIB_STACK)             ; Top of Stack
                DCD     Reset_Handler                  ; Reset Handler
                DCD     NMI_Handler                    ; NMI Handler
                DCD     HardFault_Handler              ; Hard Fault Handler
                DCD     MemManage_Handler              ; MPU Fault Handler
                DCD     BusFault_Handler               ; Bus Fault Handler
                DCD     UsageFault_Handler             ; Usage Fault Handler
                DCD     SecureFault_Handler            ; Secure Fault Handler
                DCD     0                              ; Reserved
                DCD     0                              ; Reserved
                DCD     0                              ; Reserved
                DCD     SVC_Handler                    ; SVCall Handler
                DCD     DebugMon_Handler               ; Debug Monitor Handler
                DCD     0                              ; Reserved
                DCD     PendSV_Handler                 ; PendSV Handler
                DCD     SysTick_Handler                ; SysTick Handler

    /* External Interrupts */
                DCD     WDT_BOD_IRQHandler  /* Windowed watchdog timer, Brownout detect, Flash interrupt */
                DCD     DMA0_IRQHandler  /* DMA0 controller */
                DCD     GINT0_IRQHandler  /* GPIO group 0 */
                DCD     GINT1_IRQHandler  /* GPIO group 1 */
                DCD     PIN_INT0_IRQHandler  /* Pin interrupt 0 or pattern match engine slice 0 */
                DCD     PIN_INT1_IRQHandler  /* Pin interrupt 1or pattern match engine slice 1 */
                DCD     PIN_INT2_IRQHandler  /* Pin interrupt 2 or pattern match engine slice 2 */
                DCD     PIN_INT3_IRQHandler  /* Pin interrupt 3 or pattern match engine slice 3 */
                DCD     UTICK0_IRQHandler  /* Micro-tick Timer */
                DCD     MRT0_IRQHandler  /* Multi-rate timer */
                DCD     CTIMER0_IRQHandler  /* Standard counter/timer CTIMER0 */
                DCD     CTIMER1_IRQHandler  /* Standard counter/timer CTIMER1 */
                DCD     SCT0_IRQHandler  /* SCTimer/PWM */
                DCD     CTIMER3_IRQHandler  /* Standard counter/timer CTIMER3 */
                DCD     FLEXCOMM0_IRQHandler  /* Flexcomm Interface 0 (USART, SPI, I2C, I2S, FLEXCOMM) */
                DCD     FLEXCOMM1_IRQHandler  /* Flexcomm Interface 1 (USART, SPI, I2C, I2S, FLEXCOMM) */
                DCD     FLEXCOMM2_IRQHandler  /* Flexcomm Interface 2 (USART, SPI, I2C, I2S, FLEXCOMM) */
                DCD     FLEXCOMM3_IRQHandler  /* Flexcomm Interface 3 (USART, SPI, I2C, I2S, FLEXCOMM) */
                DCD     FLEXCOMM4_IRQHandler  /* Flexcomm Interface 4 (USART, SPI, I2C, I2S, FLEXCOMM) */
                DCD     FLEXCOMM5_IRQHandler  /* Flexcomm Interface 5 (USART, SPI, I2C, I2S, FLEXCOMM) */
                DCD     FLEXCOMM6_IRQHandler  /* Flexcomm Interface 6 (USART, SPI, I2C, I2S, FLEXCOMM) */
                DCD     FLEXCOMM7_IRQHandler  /* Flexcomm Interface 7 (USART, SPI, I2C, I2S, FLEXCOMM) */
                DCD     ADC0_IRQHandler  /* ADC0  */
                DCD     Reserved39_IRQHandler  /* Reserved interrupt */
                DCD     ACMP_IRQHandler  /* ACMP  interrupts */
                DCD     Reserved41_IRQHandler  /* Reserved interrupt */
                DCD     Reserved42_IRQHandler  /* Reserved interrupt */
                DCD     USB0_NEEDCLK_IRQHandler  /* USB Activity Wake-up Interrupt */
                DCD     USB0_IRQHandler  /* USB device */
                DCD     RTC_IRQHandler  /* RTC alarm and wake-up interrupts */
                DCD     Reserved46_IRQHandler  /* Reserved interrupt */
                DCD     MAILBOX_IRQHandler  /* WAKEUP,Mailbox interrupt (present on selected devices) */
                DCD     PIN_INT4_IRQHandler  /* Pin interrupt 4 or pattern match engine slice 4 int */
                DCD     PIN_INT5_IRQHandler  /* Pin interrupt 5 or pattern match engine slice 5 int */
                DCD     PIN_INT6_IRQHandler  /* Pin interrupt 6 or pattern match engine slice 6 int */
                DCD     PIN_INT7_IRQHandler  /* Pin interrupt 7 or pattern match engine slice 7 int */
                DCD     CTIMER2_IRQHandler  /* Standard counter/timer CTIMER2 */
                DCD     CTIMER4_IRQHandler  /* Standard counter/timer CTIMER4 */
                DCD     OS_EVENT_IRQHandler  /* OSEVTIMER0 and OSEVTIMER0_WAKEUP interrupts */
                DCD     Reserved55_IRQHandler  /* Reserved interrupt */
                DCD     Reserved56_IRQHandler  /* Reserved interrupt */
                DCD     Reserved57_IRQHandler  /* Reserved interrupt */
                DCD     SDIO_IRQHandler  /* SD/MMC  */
                DCD     Reserved59_IRQHandler  /* Reserved interrupt */
                DCD     Reserved60_IRQHandler  /* Reserved interrupt */
                DCD     Reserved61_IRQHandler  /* Reserved interrupt */
                DCD     USB1_UTMI_IRQHandler  /* USB1_UTMI */
                DCD     USB1_IRQHandler  /* USB1 interrupt */
                DCD     USB1_NEEDCLK_IRQHandler  /* USB1 activity */
                DCD     SEC_HYPERVISOR_CALL_IRQHandler  /* SEC_HYPERVISOR_CALL interrupt */
                DCD     SEC_GPIO_INT0_IRQ0_IRQHandler  /* SEC_GPIO_INT0_IRQ0 interrupt */
                DCD     SEC_GPIO_INT0_IRQ1_IRQHandler  /* SEC_GPIO_INT0_IRQ1 interrupt */
                DCD     PLU_IRQHandler  /* PLU interrupt */
                DCD     SEC_VIO_IRQHandler  /* SEC_VIO interrupt */
                DCD     HASHCRYPT_IRQHandler  /* HASHCRYPT interrupt */
                DCD     CASER_IRQHandler  /* CASPER interrupt */
                DCD     PUF_IRQHandler  /* PUF interrupt */
                DCD     PQ_IRQHandler  /* PQ interrupt */
                DCD     DMA1_IRQHandler  /* DMA1 interrupt */
                DCD     LSPI_HS_IRQHandler  /* Flexcomm Interface 8 (SPI, , FLEXCOMM) */

__Vectors_End

__Vectors       EQU     __vector_table
__Vectors_Size  EQU     __Vectors_End - __Vectors

; Reset Handler
                PUBWEAK  Reset_Handler
                SECTION  .text:CODE:REORDER:NOROOT(2)
Reset_Handler
                CPSID   i              ; Disable IRQs
                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =__iar_program_start
                BX      R0
End_Of_Main
                B       .


; Dummy Exception Handlers (infinite loops which can be modified)
Default_Handler MACRO handler_name
                PUBWEAK  handler_name
handler_name
                B       .
                ENDM

                Default_Handler NMI_Handler
                Default_Handler HardFault_Handler
                Default_Handler MemManage_Handler
                Default_Handler BusFault_Handler
                Default_Handler UsageFault_Handler
                Default_Handler SecureFault_Handler
                Default_Handler SVC_Handler
                Default_Handler DebugMon_Handler
                Default_Handler PendSV_Handler
                Default_Handler SysTick_Handler

                Default_Handler WDT_BOD_IRQHandler
                Default_Handler DMA0_IRQHandler
                Default_Handler GINT0_IRQHandler
                Default_Handler GINT1_IRQHandler
                Default_Handler PIN_INT0_IRQHandler
                Default_Handler PIN_INT1_IRQHandler
                Default_Handler PIN_INT2_IRQHandler
                Default_Handler PIN_INT3_IRQHandler
                Default_Handler UTICK0_IRQHandler
                Default_Handler MRT0_IRQHandler
                Default_Handler CTIMER0_IRQHandler
                Default_Handler CTIMER1_IRQHandler
                Default_Handler SCT0_IRQHandler
                Default_Handler CTIMER3_IRQHandler
                Default_Handler FLEXCOMM0_IRQHandler
                Default_Handler FLEXCOMM1_IRQHandler
                Default_Handler FLEXCOMM2_IRQHandler
                Default_Handler FLEXCOMM3_IRQHandler
                Default_Handler FLEXCOMM4_IRQHandler
                Default_Handler FLEXCOMM5_IRQHandler
                Default_Handler FLEXCOMM6_IRQHandler
                Default_Handler FLEXCOMM7_IRQHandler
                Default_Handler ADC0_IRQHandler
                Default_Handler Reserved39_IRQHandler
                Default_Handler ACMP_IRQHandler
                Default_Handler Reserved41_IRQHandler
                Default_Handler Reserved42_IRQHandler
                Default_Handler USB0_NEEDCLK_IRQHandler
                Default_Handler USB0_IRQHandler
                Default_Handler RTC_IRQHandler
                Default_Handler Reserved46_IRQHandler
                Default_Handler MAILBOX_IRQHandler
                Default_Handler PIN_INT4_IRQHandler
                Default_Handler PIN_INT5_IRQHandler
                Default_Handler PIN_INT6_IRQHandler
                Default_Handler PIN_INT7_IRQHandler
                Default_Handler CTIMER2_IRQHandler
                Default_Handler CTIMER4_IRQHandler
                Default_Handler OS_EVENT_IRQHandler
                Default_Handler Reserved55_IRQHandler
                Default_Handler Reserved56_IRQHandler
                Default_Handler Reserved57_IRQHandler
                Default_Handler SDIO_IRQHandler
                Default_Handler Reserved59_IRQHandler
                Default_Handler Reserved60_IRQHandler
                Default_Handler Reserved61_IRQHandler
                Default_Handler USB1_UTMI_IRQHandler
                Default_Handler USB1_IRQHandler
                Default_Handler USB1_NEEDCLK_IRQHandler
                Default_Handler SEC_HYPERVISOR_CALL_IRQHandler
                Default_Handler SEC_GPIO_INT0_IRQ0_IRQHandler
                Default_Handler SEC_GPIO_INT0_IRQ1_IRQHandler
                Default_Handler PLU_IRQHandler
                Default_Handler SEC_VIO_IRQHandler
                Default_Handler HASHCRYPT_IRQHandler
                Default_Handler CASER_IRQHandler
                Default_Handler PUF_IRQHandler
                Default_Handler PQ_IRQHandler
                Default_Handler DMA1_IRQHandler
                Default_Handler LSPI_HS_IRQHandler

                END
