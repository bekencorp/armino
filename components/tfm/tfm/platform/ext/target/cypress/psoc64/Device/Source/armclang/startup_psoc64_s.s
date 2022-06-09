;/*
; * Copyright (c) 2017-2021 ARM Limited
; * Copyright (c) 2019-2021, Cypress Semiconductor Corporation. All rights reserved.
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
; adapted from Cypress PSoC64 Peripheral-Driver-Library v1.3.1
; startup_psoc6_02_cm0plus.s
;
;/*
;//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
;*/


; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

; Address of the NMI handler in ROM
CY_NMI_HANLDER_ADDR    EQU    0x0000000D

; Address of CPU VTOR register
CY_CPU_VTOR_ADDR       EQU    0xE000ED08

                PRESERVE8

                IMPORT |Image$$ARM_LIB_STACK$$ZI$$Limit|

; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size
                EXPORT  __ramVectors

                IMPORT  HardFault_Handler
                IMPORT  SVC_Handler
                IMPORT  PendSV_Handler
                IMPORT  tfm_mailbox_irq_handler
                IMPORT  Cy_SysIpcPipeIsrCm0

__Vectors       ;Core Interrupts
                DCD     |Image$$ARM_LIB_STACK$$ZI$$Limit|  ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     CY_NMI_HANLDER_ADDR       ; NMI Handler
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

                ; External interrupts                           Description
                DCD     NvicMux0_IRQHandler                   ; CPU User Interrupt #0
                DCD     Cy_SysIpcPipeIsrCm0
                DCD     NvicMux2_IRQHandler                   ; CPU User Interrupt #2
                DCD     TFM_TIMER0_IRQ_Handler                ; Secure Timer IRQ
                DCD     NvicMux4_IRQHandler                   ; CPU User Interrupt #4
                DCD     NvicMux5_IRQHandler                   ; CPU User Interrupt #5
                DCD     NvicMux6_IRQHandler                   ; CPU User Interrupt #6
                DCD     tfm_mailbox_irq_handler               ; CPU User Interrupt #7
                DCD     Internal0_IRQHandler                  ; Internal SW Interrupt #0
                DCD     Internal1_IRQHandler                  ; Internal SW Interrupt #1
                DCD     Internal2_IRQHandler                  ; Internal SW Interrupt #2
                DCD     Internal3_IRQHandler                  ; Internal SW Interrupt #3
                DCD     Internal4_IRQHandler                  ; Internal SW Interrupt #4
                DCD     Internal5_IRQHandler                  ; Internal SW Interrupt #5
                DCD     Internal6_IRQHandler                  ; Internal SW Interrupt #6
                DCD     Internal7_IRQHandler                  ; Internal SW Interrupt #7

__Vectors_End

__Vectors_Size  EQU     __Vectors_End - __Vectors

                AREA    RESET_RAM, READWRITE, NOINIT
__ramVectors
                SPACE   __Vectors_Size

; Reset Handler
                AREA    |.text|, CODE, READONLY
Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  SystemInit
                IMPORT  __main
                CPSID   i              ; Disable IRQs

                EXTERN RAM_VECTORS_SUPPORT
                IF :DEF:RAM_VECTORS_SUPPORT
                ; Copy vectors from ROM to RAM
                LDR r1, =__Vectors
                LDR r0, =__ramVectors
                LDR r2, =__Vectors_Size
Vectors_Copy
                LDR r3, [r1]
                STR r3, [r0]
                ADDS r0, r0, #4
                ADDS r1, r1, #4
                SUBS r2, r2, #4
                CMP r2, #0
                BNE Vectors_Copy

                ; Update Vector Table Offset Register.
                LDR r0, =__ramVectors

                ELSE

                LDR     R0, =__Vectors

                ENDIF

                LDR r1, =CY_CPU_VTOR_ADDR
                STR r0, [r1]
                DSB 0xF

                LDR     R0, =SystemInit
                BLX     R0
                MOV     R3, SP
                MOVS    R1, #2

                MOV     SP, R3
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

                Default_Handler SysTick_Handler
                Default_Handler NvicMux0_IRQHandler
                Default_Handler NvicMux2_IRQHandler
                Default_Handler TFM_TIMER0_IRQ_Handler
                Default_Handler NvicMux4_IRQHandler
                Default_Handler NvicMux5_IRQHandler
                Default_Handler NvicMux6_IRQHandler
                Default_Handler Internal0_IRQHandler
                Default_Handler Internal1_IRQHandler
                Default_Handler Internal2_IRQHandler
                Default_Handler Internal3_IRQHandler
                Default_Handler Internal4_IRQHandler
                Default_Handler Internal5_IRQHandler
                Default_Handler Internal6_IRQHandler
                Default_Handler Internal7_IRQHandler

                ALIGN

                END
