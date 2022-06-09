;/*
; * Copyright (c) 2009-2021 Arm Limited
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

                IMPORT |Image$$ARM_LIB_STACK$$ZI$$Limit|

; Vector Table Mapped to Address 0 at Reset

                PRESERVE8
                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       ;Core Interrupts
                DCD     |Image$$ARM_LIB_STACK$$ZI$$Limit|    ; Top of Stack
                DCD      Reset_Handler                       ;     Reset Handler
                DCD      NMI_Handler                         ; -14 NMI Handler
                DCD      HardFault_Handler                   ; -13 Hard Fault Handler
                DCD      0                                   ;     Reserved
                DCD      0                                   ;     Reserved
                DCD      0                                   ;     Reserved
                DCD      0                                   ;     Reserved
                DCD      0                                   ;     Reserved
                DCD      0                                   ;     Reserved
                DCD      0                                   ;     Reserved
                DCD      SVC_Handler                         ;  -5 SVCall Handler
                DCD      0                                   ;     Reserved
                DCD      0                                   ;     Reserved
                DCD      PendSV_Handler                      ;  -2 PendSV Handler
                DCD      SysTick_Handler                     ;  -1 SysTick Handler

                ;musca_b1_secure_enclave Interrupts
                DCD      ExternalIRQ_Handler                 ;   0:
                DCD      CC312_Handler                       ;   1:
                DCD      CC312_APB_C_Handler                 ;   2:
                DCD      Watchdog_Handler                    ;   3:
                DCD      0                                   ;   4: Reserved
                DCD      TFM_TIMER0_IRQ_Handler              ;   5:
                DCD      Timer1_Handler                      ;   6:
                DCD      0                                   ;   7: Reserved
                DCD      0                                   ;   8: Reserved
                DCD      GPIO_Handler                        ;   9:
                DCD      GPIO_0_Handler                      ;  10:
                DCD      GPIO_1_Handler                      ;  11:
                DCD      GPIO_2_Handler                      ;  12:
                DCD      GPIO_3_Handler                      ;  13:
                DCD      GPIO_4_Handler                      ;  14:
                DCD      GPIO_5_Handler                      ;  15:
                DCD      GPIO_6_Handler                      ;  16:
                DCD      GPIO_7_Handler                      ;  17:
                DCD      0                                   ;  18:
                DCD      0                                   ;  19:
                DCD      0                                   ;  20:
                DCD      MHU0_NR2R_Handler                   ;  21:
                DCD      MHU0_R2NR_Handler                   ;  22:
                DCD      MHU0_COMB_Handler                   ;  23:
                DCD      MHU0_MSG_0_Handler                  ;  24:
                DCD      MHU0_MSG_1_Handler                  ;  25:
                DCD      MHU1_NR2R_Handler                   ;  26:
                DCD      MHU1_R2NR_Handler                   ;  27:
                DCD      MHU1_COMB_Handler                   ;  28:
                DCD      MHU1_MSG_0_Handler                  ;  29:
                DCD      MHU1_MSG_1_Handler                  ;  30:
                DCD      0                                   ;  31:

__Vectors_End

__Vectors_Size  EQU     __Vectors_End - __Vectors

; Reset Handler
                AREA    |.text|, CODE, READONLY
Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  SystemInit
                IMPORT  __main
                CPSID   i              ; Disable IRQs
                LDR     R0, =SystemInit
                BLX     R0

                LDR     R0, =__main
                BX      R0
                ENDP
End_Of_Main
                B       .


; Dummy Exception Handlers (infinite loops which can be modified)
                MACRO
                Default_Handler $handler_name
$handler_name   PROC
                EXPORT  $handler_name             [WEAK]
                B       .
                ENDP
                MEND

                Default_Handler NMI_Handler
                Default_Handler HardFault_Handler
                Default_Handler SVC_Handler
                Default_Handler PendSV_Handler
                Default_Handler SysTick_Handler

                Default_Handler ExternalIRQ_Handler
                Default_Handler CC312_Handler
                Default_Handler CC312_APB_C_Handler
                Default_Handler Watchdog_Handler

                Default_Handler TFM_TIMER0_IRQ_Handler
                Default_Handler Timer1_Handler

                Default_Handler GPIO_Handler
                Default_Handler GPIO_0_Handler
                Default_Handler GPIO_1_Handler
                Default_Handler GPIO_2_Handler
                Default_Handler GPIO_3_Handler
                Default_Handler GPIO_4_Handler
                Default_Handler GPIO_5_Handler
                Default_Handler GPIO_6_Handler
                Default_Handler GPIO_7_Handler

                Default_Handler MHU0_NR2R_Handler
                Default_Handler MHU0_R2NR_Handler
                Default_Handler MHU0_COMB_Handler
                Default_Handler MHU0_MSG_0_Handler
                Default_Handler MHU0_MSG_1_Handler
                Default_Handler MHU1_NR2R_Handler
                Default_Handler MHU1_R2NR_Handler
                Default_Handler MHU1_COMB_Handler
                Default_Handler MHU1_MSG_0_Handler
                Default_Handler MHU1_MSG_1_Handler
                ALIGN

                END
