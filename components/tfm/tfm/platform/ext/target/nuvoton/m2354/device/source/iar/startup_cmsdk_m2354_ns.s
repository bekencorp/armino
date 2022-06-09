;/*
; * Copyright (c) 2016-2020 ARM Limited
; * Copyright (c) 2020 Nuvoton Technology Corp. All rights reserved.
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
                EXTERN   SCU_IRQHandler
                PUBLIC   __vector_table
                PUBLIC   __Vectors
                PUBLIC   __Vectors_End
                PUBLIC   __Vectors_Size

                DATA

__vector_table  DCD     sfe(ARM_LIB_STACK_MSP)    ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; Interrupts
                DCD     BOD_IRQHandler		       ; 0
                DCD     IRC_IRQHandler             ; 1
                DCD     PWRWU_IRQHandler           ; 2
                DCD     SRAM_IRQHandler            ; 3
                DCD     CLKFAIL_IRQHandler         ; 4
                DCD     DEFAULT_IRQHandler         ; 5
                DCD     RTC_IRQHandler             ; 6
                DCD     TAMPER_IRQHandler          ; 7
                DCD     WDT_IRQHandler             ; 8
                DCD     WWDT_IRQHandler            ; 9
                DCD     EINT0_IRQHandler           ; 10
                DCD     EINT1_IRQHandler           ; 11
                DCD     EINT2_IRQHandler           ; 12
                DCD     EINT3_IRQHandler           ; 13
                DCD     EINT4_IRQHandler           ; 14
                DCD     EINT5_IRQHandler           ; 15
                DCD     GPA_IRQHandler             ; 16
                DCD     GPB_IRQHandler             ; 17
                DCD     GPC_IRQHandler             ; 18
                DCD     GPD_IRQHandler             ; 19
                DCD     GPE_IRQHandler             ; 20
                DCD     GPF_IRQHandler             ; 21
                DCD     QSPI0_IRQHandler           ; 22
                DCD     SPI0_IRQHandler            ; 23
                DCD     BRAKE0_IRQHandler          ; 24
                DCD     EPWM0_P0_IRQHandler        ; 25
                DCD     EPWM0_P1_IRQHandler        ; 26
                DCD     EPWM0_P2_IRQHandler        ; 27
                DCD     BRAKE1_IRQHandler          ; 28
                DCD     EPWM1_P0_IRQHandler        ; 29
                DCD     EPWM1_P1_IRQHandler        ; 30
                DCD     EPWM1_P2_IRQHandler        ; 31
                DCD     TFM_TIMER0_IRQ_Handler     ; 32
                DCD     TMR1_IRQHandler            ; 33
                DCD     TMR2_IRQHandler            ; 34
                DCD     TMR3_IRQHandler            ; 35
                DCD     UART0_IRQHandler           ; 36
                DCD     UART1_IRQHandler           ; 37
                DCD     I2C0_IRQHandler            ; 38
                DCD     I2C1_IRQHandler            ; 39
                DCD     PDMA0_IRQHandler           ; 40
                DCD     DAC_IRQHandler             ; 41
                DCD     EADC0_IRQHandler           ; 42
                DCD     EADC1_IRQHandler           ; 43
                DCD     ACMP01_IRQHandler          ; 44
                DCD     DEFAULT_IRQHandler         ; 45
                DCD     EADC2_IRQHandler           ; 46
                DCD     EADC3_IRQHandler           ; 47
                DCD     UART2_IRQHandler           ; 48
                DCD     UART3_IRQHandler           ; 49
                DCD     DEFAULT_IRQHandler         ; 50
                DCD     SPI1_IRQHandler            ; 51
                DCD     SPI2_IRQHandler            ; 52
                DCD     USBD_IRQHandler            ; 53
                DCD     USBH_IRQHandler            ; 54
                DCD     USBOTG_IRQHandler          ; 55
                DCD     CAN0_IRQHandler            ; 56
                DCD     DEFAULT_IRQHandler         ; 57
                DCD     SC0_IRQHandler             ; 58
                DCD     SC1_IRQHandler             ; 59
                DCD     SC2_IRQHandler             ; 60
                DCD     DEFAULT_IRQHandler         ; 61
                DCD     SPI3_IRQHandler            ; 62
                DCD     DEFAULT_IRQHandler         ; 63
                DCD     SDH0_IRQHandler            ; 64
                DCD     DEFAULT_IRQHandler         ; 65
                DCD     DEFAULT_IRQHandler         ; 66
                DCD     DEFAULT_IRQHandler         ; 67
                DCD     I2S0_IRQHandler            ; 68
                DCD     DEFAULT_IRQHandler         ; 69
                DCD     OPA0_IRQHandler            ; 70
                DCD     CRPT_IRQHandler            ; 71
                DCD     GPG_IRQHandler             ; 72
                DCD     EINT6_IRQHandler           ; 73
                DCD     UART4_IRQHandler           ; 74
                DCD     UART5_IRQHandler           ; 75
                DCD     USCI0_IRQHandler           ; 76
                DCD     USCI1_IRQHandler           ; 77
                DCD     BPWM0_IRQHandler           ; 78
                DCD     BPWM1_IRQHandler           ; 79
                DCD     DEFAULT_IRQHandler         ; 80
                DCD     DEFAULT_IRQHandler         ; 81
                DCD     I2C2_IRQHandler            ; 82
                DCD     DEFAULT_IRQHandler         ; 83
                DCD     QEI0_IRQHandler            ; 84
                DCD     QEI1_IRQHandler            ; 85
                DCD     ECAP0_IRQHandler           ; 86
                DCD     ECAP1_IRQHandler           ; 87
                DCD     GPH_IRQHandler             ; 88
                DCD     EINT7_IRQHandler           ; 89
                DCD     DEFAULT_IRQHandler         ; 90
                DCD     DEFAULT_IRQHandler         ; 91
                DCD     DEFAULT_IRQHandler         ; 92
                DCD     DEFAULT_IRQHandler         ; 93
                DCD     DEFAULT_IRQHandler         ; 94
                DCD     DEFAULT_IRQHandler         ; 95
                DCD     DEFAULT_IRQHandler         ; 96
                DCD     DEFAULT_IRQHandler         ; 97
                DCD     PDMA1_IRQHandler           ; 98
                DCD     SCU_IRQHandler             ; 99
                DCD     DEFAULT_IRQHandler         ; 100
                DCD     TRNG_IRQHandler            ; 101
__Vectors_End

__Vectors       EQU     __vector_table
__Vectors_Size  EQU     __Vectors_End - __Vectors

; Reset Handler

Reset_Handler
                MRS     R0, control    ; Get control value
                MOVS    R1, #1
                ORRS    R0, R0, R1     ; Select switch to unprivileged mode
                MOVS    R1, #2
                ORRS    R0, R0, R1     ; Select switch to PSP
                MSR     control, R0
                LDR     R0, =sfe(ARM_LIB_STACK) ; End of ARM_LIB_STACK
                MOV     SP, R0         ; Initialise PSP
                LDR     R0, =__iar_program_start
                BX      R0

; Dummy Exception Handlers (infinite loops which can be modified)

Default_Handler2 MACRO handler_name
                PUBWEAK  handler_name
handler_name
                B       .
                ENDM

                Default_Handler2 NMI_Handler
                Default_Handler2 HardFault_Handler
                Default_Handler2 SVC_Handler
                Default_Handler2 PendSV_Handler
                Default_Handler2 SysTick_Handler

; Interrupts
                PUBWEAK  BOD_IRQHandler	            ; 0
                PUBWEAK  IRC_IRQHandler             ; 1
                PUBWEAK  PWRWU_IRQHandler           ; 2
                PUBWEAK  SRAM_IRQHandler            ; 3
                PUBWEAK  CLKFAIL_IRQHandler         ; 4
               ;PUBWEAK  0                          ; 5
                PUBWEAK  RTC_IRQHandler             ; 6
                PUBWEAK  TAMPER_IRQHandler          ; 7
                PUBWEAK  WDT_IRQHandler             ; 8
                PUBWEAK  WWDT_IRQHandler            ; 9
                PUBWEAK  EINT0_IRQHandler           ; 10
                PUBWEAK  EINT1_IRQHandler           ; 11
                PUBWEAK  EINT2_IRQHandler           ; 12
                PUBWEAK  EINT3_IRQHandler           ; 13
                PUBWEAK  EINT4_IRQHandler           ; 14
                PUBWEAK  EINT5_IRQHandler           ; 15
                PUBWEAK  GPA_IRQHandler             ; 16
                PUBWEAK  GPB_IRQHandler             ; 17
                PUBWEAK  GPC_IRQHandler             ; 18
                PUBWEAK  GPD_IRQHandler             ; 19
                PUBWEAK  GPE_IRQHandler             ; 20
                PUBWEAK  GPF_IRQHandler             ; 21
                PUBWEAK  QSPI0_IRQHandler           ; 22
                PUBWEAK  SPI0_IRQHandler            ; 23
                PUBWEAK  BRAKE0_IRQHandler          ; 24
                PUBWEAK  EPWM0_P0_IRQHandler        ; 25
                PUBWEAK  EPWM0_P1_IRQHandler        ; 26
                PUBWEAK  EPWM0_P2_IRQHandler        ; 27
                PUBWEAK  BRAKE1_IRQHandler          ; 28
                PUBWEAK  EPWM1_P0_IRQHandler        ; 29
                PUBWEAK  EPWM1_P1_IRQHandler        ; 30
                PUBWEAK  EPWM1_P2_IRQHandler        ; 31
                PUBWEAK  TMR0_IRQHandler            ; 32
                PUBWEAK  TMR1_IRQHandler            ; 33
                PUBWEAK  TMR2_IRQHandler            ; 34
                PUBWEAK  TMR3_IRQHandler            ; 35
                PUBWEAK  UART0_IRQHandler           ; 36
                PUBWEAK  UART1_IRQHandler           ; 37
                PUBWEAK  I2C0_IRQHandler            ; 38
                PUBWEAK  I2C1_IRQHandler            ; 39
                PUBWEAK  PDMA0_IRQHandler           ; 40
                PUBWEAK  DAC_IRQHandler             ; 41
                PUBWEAK  EADC0_IRQHandler           ; 42
                PUBWEAK  EADC1_IRQHandler           ; 43
                PUBWEAK  ACMP01_IRQHandler          ; 44
               ;PUBWEAK  0                          ; 45
                PUBWEAK  EADC2_IRQHandler           ; 46
                PUBWEAK  EADC3_IRQHandler           ; 47
                PUBWEAK  UART2_IRQHandler           ; 48
                PUBWEAK  UART3_IRQHandler           ; 49
               ;PUBWEAK  0                          ; 50
                PUBWEAK  SPI1_IRQHandler            ; 51
                PUBWEAK  SPI2_IRQHandler            ; 52
                PUBWEAK  USBD_IRQHandler            ; 53
                PUBWEAK  USBH_IRQHandler            ; 54
                PUBWEAK  USBOTG_IRQHandler          ; 55
                PUBWEAK  CAN0_IRQHandler            ; 56
                PUBWEAK  CAN1_IRQHandler            ; 57
                PUBWEAK  SC0_IRQHandler             ; 58
                PUBWEAK  SC1_IRQHandler             ; 59
                PUBWEAK  SC2_IRQHandler             ; 60
                PUBWEAK  SC3_IRQHandler             ; 61
                PUBWEAK  SPI3_IRQHandler            ; 62
               ;PUBWEAK  0                          ; 63
                PUBWEAK  SDH0_IRQHandler            ; 64
               ;PUBWEAK  0                          ; 65
               ;PUBWEAK  0                          ; 66
               ;PUBWEAK  0                          ; 67
                PUBWEAK  I2S0_IRQHandler            ; 68
               ;PUBWEAK  0                          ; 69
                PUBWEAK  OPA0_IRQHandler            ; 70
                PUBWEAK  CRPT_IRQHandler            ; 71
                PUBWEAK  GPG_IRQHandler             ; 72
                PUBWEAK  EINT6_IRQHandler           ; 73
                PUBWEAK  UART4_IRQHandler           ; 74
                PUBWEAK  UART5_IRQHandler           ; 75
                PUBWEAK  USCI0_IRQHandler           ; 76
                PUBWEAK  USCI1_IRQHandler           ; 77
                PUBWEAK  BPWM0_IRQHandler           ; 78
                PUBWEAK  BPWM1_IRQHandler           ; 79
               ;PUBWEAK  0                          ; 80
               ;PUBWEAK  0                          ; 81
                PUBWEAK  I2C2_IRQHandler            ; 82
               ;PUBWEAK  0                          ; 83
                PUBWEAK  QEI0_IRQHandler            ; 84
                PUBWEAK  QEI1_IRQHandler            ; 85
                PUBWEAK  ECAP0_IRQHandler           ; 86
                PUBWEAK  ECAP1_IRQHandler           ; 87
                PUBWEAK  GPH_IRQHandler             ; 88
                PUBWEAK  EINT7_IRQHandler           ; 89
                PUBWEAK  SDH1_IRQHandler            ; 90
               ;PUBWEAK  0                          ; 91
               ;PUBWEAK  USBH_IRQHandler            ; 92
               ;PUBWEAK  0                          ; 93
               ;PUBWEAK  0                          ; 94
               ;PUBWEAK  0                          ; 95
               ;PUBWEAK  0                          ; 96
               ;PUBWEAK  0                          ; 97
                PUBWEAK  PDMA1_IRQHandler           ; 98
               ;PUBWEAK  SCU_IRQHandler             ; 99
                PUBWEAK  LCD_IRQHandler             ; 100
                PUBWEAK  TRNG_IRQHandler            ; 101

; Interrupts
BOD_IRQHandler		       ; 0
IRC_IRQHandler             ; 1
PWRWU_IRQHandler           ; 2
SRAM_IRQHandler            ; 3
CLKFAIL_IRQHandler         ; 4
;0                         ; 5
RTC_IRQHandler             ; 6
TAMPER_IRQHandler          ; 7
WDT_IRQHandler             ; 8
WWDT_IRQHandler            ; 9
EINT0_IRQHandler           ; 10
EINT1_IRQHandler           ; 11
EINT2_IRQHandler           ; 12
EINT3_IRQHandler           ; 13
EINT4_IRQHandler           ; 14
EINT5_IRQHandler           ; 15
GPA_IRQHandler             ; 16
GPB_IRQHandler             ; 17
GPC_IRQHandler             ; 18
GPD_IRQHandler             ; 19
GPE_IRQHandler             ; 20
GPF_IRQHandler             ; 21
QSPI0_IRQHandler           ; 22
SPI0_IRQHandler            ; 23
BRAKE0_IRQHandler          ; 24
EPWM0_P0_IRQHandler        ; 25
EPWM0_P1_IRQHandler        ; 26
EPWM0_P2_IRQHandler        ; 27
BRAKE1_IRQHandler          ; 28
EPWM1_P0_IRQHandler        ; 29
EPWM1_P1_IRQHandler        ; 30
EPWM1_P2_IRQHandler        ; 31
TFM_TIMER0_IRQ_Handler     ; 32
TMR1_IRQHandler            ; 33
TMR2_IRQHandler            ; 34
TMR3_IRQHandler            ; 35
UART0_IRQHandler           ; 36
UART1_IRQHandler           ; 37
I2C0_IRQHandler            ; 38
I2C1_IRQHandler            ; 39
PDMA0_IRQHandler           ; 40
DAC_IRQHandler             ; 41
EADC0_IRQHandler           ; 42
EADC1_IRQHandler           ; 43
ACMP01_IRQHandler          ; 44
;0                         ; 45
EADC2_IRQHandler           ; 46
EADC3_IRQHandler           ; 47
UART2_IRQHandler           ; 48
UART3_IRQHandler           ; 49
;0                         ; 50
SPI1_IRQHandler            ; 51
SPI2_IRQHandler            ; 52
USBD_IRQHandler            ; 53
USBH_IRQHandler            ; 54
USBOTG_IRQHandler          ; 55
CAN0_IRQHandler            ; 56
CAN1_IRQHandler            ; 57
SC0_IRQHandler             ; 58
SC1_IRQHandler             ; 59
SC2_IRQHandler             ; 60
SC3_IRQHandler             ; 61
SPI3_IRQHandler            ; 62
;0                         ; 63
SDH0_IRQHandler            ; 64
;0                         ; 65
;0                         ; 66
;0                         ; 67
I2S0_IRQHandler            ; 68
;0                         ; 69
OPA0_IRQHandler            ; 70
CRPT_IRQHandler            ; 71
GPG_IRQHandler             ; 72
EINT6_IRQHandler           ; 73
UART4_IRQHandler           ; 74
UART5_IRQHandler           ; 75
USCI0_IRQHandler           ; 76
USCI1_IRQHandler           ; 77
BPWM0_IRQHandler           ; 78
BPWM1_IRQHandler           ; 79
;0                         ; 80
;0                         ; 81
I2C2_IRQHandler            ; 82
;0                         ; 83
QEI0_IRQHandler            ; 84
QEI1_IRQHandler            ; 85
ECAP0_IRQHandler           ; 86
ECAP1_IRQHandler           ; 87
GPH_IRQHandler             ; 88
EINT7_IRQHandler           ; 89
SDH1_IRQHandler            ; 90
;0                         ; 91
;USBH_IRQHandler           ; 92
;0                         ; 93
;0                         ; 94
;0                         ; 95
;0                         ; 96
;0                         ; 97
PDMA1_IRQHandler           ; 98
;SCU_IRQHandler            ; 99
;0                         ; 100
TRNG_IRQHandler            ; 101
DEFAULT_IRQHandler
                B       .

                END
