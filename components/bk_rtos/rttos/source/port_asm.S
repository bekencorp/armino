;/*
; * uint32_t platform_is_in_irq_context( void );
; */
    .globl platform_is_in_irq_context
    .type platform_is_in_irq_context, %function
platform_is_in_irq_context:
    MRS     R0, CPSR
    AND     R1, R0, #0x1F
    CMP     R1, #0x12 ;//IRQ_MODE
    MOVEQ   R0, #1
    MOVNE   R0, #0
    BX      LR

;/*
; * uint32_t platform_is_in_fiq_context( void );
; */
    .globl platform_is_in_fiq_context
    .type platform_is_in_fiq_context, %function
platform_is_in_fiq_context:
    MRS     R0, CPSR
    AND     R1, R0, #0x1F
    CMP     R1, #0x11 ;//FIQ_MODE
    MOVEQ   R0, #1
    MOVNE   R0, #0
    BX      LR

;/*
; * void portENABLE_IRQ( void );
; */
    .globl portENABLE_IRQ
    .type portENABLE_IRQ, %function
portENABLE_IRQ:
    MRS     R0, CPSR
    BIC     R1, R0, #0x80
    MSR     CPSR_c, R1
    BX      LR

;/*
; * void portENABLE_FIQ( void );
; */
    .globl portENABLE_FIQ
    .type portENABLE_FIQ, %function
portENABLE_FIQ:
    MRS     R0, CPSR
    BIC     R1, R0, #0x40
    MSR     CPSR_c, R1
    BX      LR

;/*
; * int portDISABLE_IRQ( void );
; */
    .globl portDISABLE_IRQ
    .type portDISABLE_IRQ, %function
portDISABLE_IRQ:
    MRS     R2, CPSR
    ORR     R1, R2, #0x80
    MSR     CPSR_c, R1
    AND     R0, R2, #0x80
    BX      LR

;/*
; * int portDISABLE_FIQ( void );
; */
    .globl portDISABLE_FIQ
    .type portDISABLE_FIQ, %function
portDISABLE_FIQ:
    MRS     R2, CPSR
    ORR     R1, R2, #0x40
    MSR     CPSR_c, R1
    AND     R0, R2, #0x40
    BX      LR

