;/*
; * Copyright (c) 2016-2020 ARM Limited
; * Copyright (c) 2020-2021 IAR Systems AB
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
; This file is derivative of ../armclang/startup_cmsdk_bk7236_ns.s

;/*
;//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
;*/


; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

                MODULE   ?cstartup

                ;; Forward declaration of sections.
                SECTION  ARM_LIB_STACK_MSP:DATA:NOROOT(3)
                SECTION  ARM_LIB_STACK:DATA:NOROOT(3)

                SECTION  .intvec:CODE:NOROOT(2)

                EXTERN   __iar_program_start
                EXTERN   SystemInit
                PUBLIC   __vector_table
                PUBLIC   __Vectors
                PUBLIC   __Vectors_End
                PUBLIC   __Vectors_Size

                DATA

__vector_table  DCD     sfe(ARM_LIB_STACK_MSP)    ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     MemManage_Handler         ; MPU Fault Handler
                DCD     BusFault_Handler          ; Bus Fault Handler
                DCD     UsageFault_Handler        ; Usage Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     DebugMon_Handler          ; Debug Monitor Handler
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; Core IoT Interrupts
                DCD     NONSEC_WATCHDOG_RESET_Handler  ; - 0 Non-Secure Watchdog Reset Handler
                DCD     NONSEC_WATCHDOG_Handler        ; - 1 Non-Secure Watchdog Handler
                DCD     S32K_TIMER_Handler             ; - 2 S32K Timer Handler
                DCD     TIMER0_Handler                 ; - 3 TIMER 0 Handler
                DCD     TIMER1_Handler                 ; - 4 TIMER 1 Handler
                DCD     DUALTIMER_Handler              ; - 5 Dual Timer Handler
                DCD     0                              ; Reserved - 6
                DCD     0                              ; Reserved - 7
                DCD     0                              ; Reserved - 8
                DCD     0                              ; Reserved - 9
                DCD     0                              ; Reserved - 10
                DCD     0                              ; Reserved - 11
                DCD     0                              ; Reserved - 12
                DCD     0                              ; Reserved - 13
                DCD     0                              ; Reserved - 14
                DCD     0                              ; Reserved - 15
                DCD     0                              ; Reserved - 16
                DCD     0                              ; Reserved - 17
                DCD     0                              ; Reserved - 18
                DCD     0                              ; Reserved - 19
                DCD     0                              ; Reserved - 20
                DCD     0                              ; Reserved - 21
                DCD     0                              ; Reserved - 22
                DCD     0                              ; Reserved - 23
                DCD     0                              ; Reserved - 24
                DCD     0                              ; Reserved - 25
                DCD     0                              ; Reserved - 26
                DCD     0                              ; Reserved - 27
                DCD     0                              ; Reserved - 28
                DCD     0                              ; Reserved - 29
                DCD     0                              ; Reserved - 30
                DCD     0                              ; Reserved - 31
                ; External Interrupts
                DCD     UARTRX0_Handler           ; 32 UART 0 RX Handler
                DCD     UARTTX0_Handler           ; 33 UART 0 TX Handler
                DCD     UARTRX1_Handler           ; 34 UART 1 RX Handler
                DCD     UARTTX1_Handler           ; 35 UART 1 TX Handler
                DCD     UARTRX2_Handler           ; 36 UART 2 RX Handler
                DCD     UARTTX2_Handler           ; 37 UART 2 TX Handler
                DCD     UARTRX3_Handler           ; 38 UART 3 RX Handler
                DCD     UARTTX3_Handler           ; 39 UART 3 TX Handler
                DCD     UARTRX4_Handler           ; 40 UART 4 RX Handler
                DCD     UARTTX4_Handler           ; 41 UART 4 TX Handler
                DCD     UART0_Handler             ; 42 UART 0 combined Handler
                DCD     UART1_Handler             ; 43 UART 1 combined Handler
                DCD     UART2_Handler             ; 44 UART 0 combined Handler
                DCD     UART3_Handler             ; 45 UART 1 combined Handler
                DCD     UART4_Handler             ; 46 UART 0 combined Handler
                DCD     UARTOVF_Handler           ; 47 UART 0,1,2,3,4 Overflow Handler
                DCD     ETHERNET_Handler          ; 48 Ethernet Handler
                DCD     I2S_Handler               ; 49 I2S Handler
                DCD     TSC_Handler               ; 50 Touch Screen Handler
                DCD     SPI0_Handler              ; 51 SPI 0 Handler
                DCD     SPI1_Handler              ; 52 SPI 1 Handler
                DCD     SPI2_Handler              ; 53 SPI 2 Handler
                DCD     SPI3_Handler              ; 54 SPI 3 Handler
                DCD     SPI4_Handler              ; 55 SPI 4 Handler
                DCD     DMA0_ERROR_Handler        ; 56 DMA 0 Error Handler
                DCD     DMA0_TC_Handler           ; 57 DMA 0 Terminal Count Handler
                DCD     DMA0_Handler              ; 58 DMA 0 Combined Handler
                DCD     DMA1_ERROR_Handler        ; 59 DMA 1 Error Handler
                DCD     DMA1_TC_Handler           ; 60 DMA 1 Terminal Count Handler
                DCD     DMA1_Handler              ; 61 DMA 1 Combined Handler
                DCD     DMA2_ERROR_Handler        ; 62 DMA 2 Error Handler
                DCD     DMA2_TC_Handler           ; 63 DMA 2 Terminal Count Handler
                DCD     DMA2_Handler              ; 64 DMA 2 Combined Handler
                DCD     DMA3_ERROR_Handler        ; 65 DMA 3 Error Handler
                DCD     DMA3_TC_Handler           ; 66 DMA 3 Terminal Count Handler
                DCD     DMA3_Handler              ; 67 DMA 3 Combined Handler
                DCD     GPIO0_Handler             ; 68 GPIO 0 Comboned Handler
                DCD     GPIO1_Handler             ; 69 GPIO 1 Comboned Handler
                DCD     GPIO2_Handler             ; 70 GPIO 2 Comboned Handler
                DCD     GPIO3_Handler             ; 71 GPIO 3 Comboned Handler
                DCD     GPIO0_0_Handler           ; 72,
                DCD     GPIO0_1_Handler           ; 73,
                DCD     GPIO0_2_Handler           ; 74,
                DCD     GPIO0_3_Handler           ; 75,
                DCD     GPIO0_4_Handler           ; 76,
                DCD     GPIO0_5_Handler           ; 77,
                DCD     GPIO0_6_Handler           ; 78,
                DCD     GPIO0_7_Handler           ; 79,
                DCD     GPIO0_8_Handler           ; 80,
                DCD     GPIO0_9_Handler           ; 81,
                DCD     GPIO0_10_Handler          ; 82,
                DCD     GPIO0_11_Handler          ; 83,
                DCD     GPIO0_12_Handler          ; 84,
                DCD     GPIO0_13_Handler          ; 85,
                DCD     GPIO0_14_Handler          ; 86,
                DCD     GPIO0_15_Handler          ; 87,
                DCD     GPIO1_0_Handler           ; 88,
                DCD     GPIO1_1_Handler           ; 89,
                DCD     GPIO1_2_Handler           ; 90,
                DCD     GPIO1_3_Handler           ; 91,
                DCD     GPIO1_4_Handler           ; 92,
                DCD     GPIO1_5_Handler           ; 93,
                DCD     GPIO1_6_Handler           ; 94,
                DCD     GPIO1_7_Handler           ; 95,
__Vectors_End

__Vectors       EQU     __vector_table
__Vectors_Size  EQU     __Vectors_End - __Vectors

; Reset Handler

Reset_Handler
                LDR      R0, =sfe(ARM_LIB_STACK)       ; End of ARM_LIB_STACK
                MSR      PSP, R0
                MRS      R0, CONTROL    ; Get control value
                ORR      R0, R0, #2     ; Select switch to PSP
                MSR      CONTROL, R0
                LDR      R0, =__iar_program_start
                BX       R0

; Dummy Exception Handlers (infinite loops which can be modified)

Default_Handler2 MACRO handler_name
                PUBWEAK  handler_name
handler_name
                B       .
                ENDM

                Default_Handler2 NMI_Handler
                Default_Handler2 HardFault_Handler
                Default_Handler2 MemManage_Handler
                Default_Handler2 BusFault_Handler
                Default_Handler2 UsageFault_Handler
                Default_Handler2 SVC_Handler
                Default_Handler2 DebugMon_Handler
                Default_Handler2 PendSV_Handler
                Default_Handler2 SysTick_Handler

Default_Handler
; Core IoT Interrupts
                PUBWEAK NONSEC_WATCHDOG_RESET_Handler    ; - 0 Non-Secure Watchdog Reset Handler
                PUBWEAK NONSEC_WATCHDOG_Handler          ; - 1 Non-Secure Watchdog Handler
                PUBWEAK S32K_TIMER_Handler               ; - 2 S32K Timer Handler
                PUBWEAK TIMER0_Handler                   ; - 3 TIMER 0 Handler
                PUBWEAK TIMER1_Handler                   ; - 4 TIMER 1 Handler
                PUBWEAK DUALTIMER_Handler                ; - 5 Dual Timer Handler
; External Interrupts
                PUBWEAK UARTRX0_Handler              ; 32 UART 0 RX Handler
                PUBWEAK UARTTX0_Handler              ; 33 UART 0 TX Handler
                PUBWEAK UARTRX1_Handler              ; 34 UART 1 RX Handler
                PUBWEAK UARTTX1_Handler              ; 35 UART 1 TX Handler
                PUBWEAK UARTRX2_Handler              ; 36 UART 2 RX Handler
                PUBWEAK UARTTX2_Handler              ; 37 UART 2 TX Handler
                PUBWEAK UARTRX3_Handler              ; 38 UART 3 RX Handler
                PUBWEAK UARTTX3_Handler              ; 39 UART 3 TX Handler
                PUBWEAK UARTRX4_Handler              ; 40 UART 4 RX Handler
                PUBWEAK UARTTX4_Handler              ; 41 UART 4 TX Handler
                PUBWEAK UART0_Handler                ; 42 UART 0 combined Handler
                PUBWEAK UART1_Handler                ; 43 UART 1 combined Handler
                PUBWEAK UART2_Handler                ; 44 UART 2 combined Handler
                PUBWEAK UART3_Handler                ; 45 UART 3 combined Handler
                PUBWEAK UART4_Handler                ; 46 UART 4 combined Handler
                PUBWEAK UARTOVF_Handler              ; 47 UART 0,1,2,3,4 Overflow Handler
                PUBWEAK ETHERNET_Handler             ; 48 Ethernet Handler
                PUBWEAK I2S_Handler                  ; 49 I2S Handler
                PUBWEAK TSC_Handler                  ; 50 Touch Screen Handler
                PUBWEAK SPI0_Handler                 ; 51 SPI 0 Handler
                PUBWEAK SPI1_Handler                 ; 52 SPI 1 Handler
                PUBWEAK SPI2_Handler                 ; 53 SPI 2 Handler
                PUBWEAK SPI3_Handler                 ; 54 SPI 3 Handler
                PUBWEAK SPI4_Handler                 ; 55 SPI 4 Handler
                PUBWEAK DMA0_ERROR_Handler           ; 56 DMA 0 Error Handler
                PUBWEAK DMA0_TC_Handler              ; 57 DMA 0 Terminal Count Handler
                PUBWEAK DMA0_Handler                 ; 58 DMA 0 Combined Handler
                PUBWEAK DMA1_ERROR_Handler           ; 59 DMA 1 Error Handler
                PUBWEAK DMA1_TC_Handler              ; 60 DMA 1 Terminal Count Handler
                PUBWEAK DMA1_Handler                 ; 61 DMA 1 Combined Handler
                PUBWEAK DMA2_ERROR_Handler           ; 62 DMA 2 Error Handler
                PUBWEAK DMA2_TC_Handler              ; 63 DMA 2 Terminal Count Handler
                PUBWEAK DMA2_Handler                 ; 64 DMA 2 Combined Handler
                PUBWEAK DMA3_ERROR_Handler           ; 65 DMA 3 Error Handler
                PUBWEAK DMA3_TC_Handler              ; 66 DMA 3 Terminal Count Handler
                PUBWEAK DMA3_Handler                 ; 67 DMA 3 Combined Handler
                PUBWEAK GPIO0_Handler                ; 68 GPIO 0 Comboned Handler
                PUBWEAK GPIO1_Handler                ; 69 GPIO 1 Comboned Handler
                PUBWEAK GPIO2_Handler                ; 70 GPIO 2 Comboned Handler
                PUBWEAK GPIO3_Handler                ; 71 GPIO 3 Comboned Handler
                PUBWEAK GPIO0_0_Handler              ; 72 GPIO 1 has 16 individual Handlers
                PUBWEAK GPIO0_1_Handler              ; 73
                PUBWEAK GPIO0_2_Handler              ; 74
                PUBWEAK GPIO0_3_Handler              ; 75
                PUBWEAK GPIO0_4_Handler              ; 76
                PUBWEAK GPIO0_5_Handler              ; 77
                PUBWEAK GPIO0_6_Handler              ; 78
                PUBWEAK GPIO0_7_Handler              ; 79
                PUBWEAK GPIO0_8_Handler              ; 80
                PUBWEAK GPIO0_9_Handler              ; 81
                PUBWEAK GPIO0_10_Handler             ; 82
                PUBWEAK GPIO0_11_Handler             ; 83
                PUBWEAK GPIO0_12_Handler             ; 84
                PUBWEAK GPIO0_13_Handler             ; 85
                PUBWEAK GPIO0_14_Handler             ; 86
                PUBWEAK GPIO0_15_Handler             ; 87
                PUBWEAK GPIO1_0_Handler              ; 88 GPIO 1 has 8 individual Handlers
                PUBWEAK GPIO1_1_Handler              ; 89
                PUBWEAK GPIO1_2_Handler              ; 90
                PUBWEAK GPIO1_3_Handler              ; 91
                PUBWEAK GPIO1_4_Handler              ; 92
                PUBWEAK GPIO1_5_Handler              ; 93
                PUBWEAK GPIO1_6_Handler              ; 94
                PUBWEAK GPIO1_7_Handler              ; 95

; Core IoT Interrupts
NONSEC_WATCHDOG_RESET_Handler  ; - 0 Non-Secure Watchdog Reset Handler
NONSEC_WATCHDOG_Handler        ; - 1 Non-Secure Watchdog Handler
S32K_TIMER_Handler             ; - 2 S32K Timer Handler
TIMER0_Handler                 ; - 3 TIMER 0 Handler
TIMER1_Handler                 ; - 4 TIMER 1 Handler
DUALTIMER_Handler              ; - 5 Dual Timer Handler
; External Interrupts
UARTRX0_Handler           ; 32 UART 0 RX Handler
UARTTX0_Handler           ; 33 UART 0 TX Handler
UARTRX1_Handler           ; 34 UART 1 RX Handler
UARTTX1_Handler           ; 35 UART 1 TX Handler
UARTRX2_Handler           ; 36 UART 2 RX Handler
UARTTX2_Handler           ; 37 UART 2 TX Handler
UARTRX3_Handler           ; 38 UART 3 RX Handler
UARTTX3_Handler           ; 39 UART 3 TX Handler
UARTRX4_Handler           ; 40 UART 4 RX Handler
UARTTX4_Handler           ; 41 UART 4 TX Handler
UART0_Handler             ; 42 UART 0 combined Handler
UART1_Handler             ; 43 UART 1 combined Handler
UART2_Handler             ; 44 UART 2 combined Handler
UART3_Handler             ; 45 UART 3 combined Handler
UART4_Handler             ; 46 UART 4 combined Handler
UARTOVF_Handler           ; 47 UART 0,1,2,3,4 Overflow Handler
ETHERNET_Handler          ; 48 Ethernet Handler
I2S_Handler               ; 49 I2S Handler
TSC_Handler               ; 50 Touch Screen Handler
SPI0_Handler              ; 51 SPI 0 Handler
SPI1_Handler              ; 52 SPI 1 Handler
SPI2_Handler              ; 53 SPI 2 Handler
SPI3_Handler              ; 54 SPI 3 Handler
SPI4_Handler              ; 55 SPI 4 Handler
DMA0_ERROR_Handler        ; 56 DMA 0 Error Handler
DMA0_TC_Handler           ; 57 DMA 0 Terminal Count Handler
DMA0_Handler              ; 58 DMA 0 Combined Handler
DMA1_ERROR_Handler        ; 59 DMA 1 Error Handler
DMA1_TC_Handler           ; 60 DMA 1 Terminal Count Handler
DMA1_Handler              ; 61 DMA 1 Combined Handler
DMA2_ERROR_Handler        ; 62 DMA 2 Error Handler
DMA2_TC_Handler           ; 63 DMA 2 Terminal Count Handler
DMA2_Handler              ; 64 DMA 2 Combined Handler
DMA3_ERROR_Handler        ; 65 DMA 3 Error Handler
DMA3_TC_Handler           ; 66 DMA 3 Terminal Count Handler
DMA3_Handler              ; 67 DMA 3 Combined Handler
GPIO0_Handler             ; 68 GPIO 0 Comboned Handler
GPIO1_Handler             ; 69 GPIO 1 Comboned Handler
GPIO2_Handler             ; 70 GPIO 2 Comboned Handler
GPIO3_Handler             ; 71 GPIO 3 Comboned Handler
GPIO0_0_Handler           ; 72 GPIO 0 has 16 individual Handlers
GPIO0_1_Handler           ; 73
GPIO0_2_Handler           ; 74
GPIO0_3_Handler           ; 75
GPIO0_4_Handler           ; 76
GPIO0_5_Handler           ; 77
GPIO0_6_Handler           ; 78
GPIO0_7_Handler           ; 79
GPIO0_8_Handler           ; 80
GPIO0_9_Handler           ; 81
GPIO0_10_Handler          ; 82
GPIO0_11_Handler          ; 83
GPIO0_12_Handler          ; 84
GPIO0_13_Handler          ; 85
GPIO0_14_Handler          ; 86
GPIO0_15_Handler          ; 87
GPIO1_0_Handler           ; 88 GPIO 1 has 8 individual Handlers
GPIO1_1_Handler           ; 89
GPIO1_2_Handler           ; 90
GPIO1_3_Handler           ; 91
GPIO1_4_Handler           ; 92
GPIO1_5_Handler           ; 93
GPIO1_6_Handler           ; 94
GPIO1_7_Handler           ; 95
                B       .

                END
