/*
 * Copyright (c) 2018-2022, Arm Limited. All rights reserved.
 * Copyright (c) 2022 Cypress Semiconductor Corporation (an Infineon
 *  * company) or an affiliate of Cypress Semiconductor Corporation. All rights
 *   * reserved.
 *   *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <inttypes.h>

#include "compiler_ext_defs.h"
#include "security_defs.h"
#include "region_defs.h"
#include "spm_ipc.h"
#include "svc_num.h"
#include "tfm_arch.h"
#include "tfm_hal_device_header.h"
#include "tfm_svcalls.h"
#include "utilities.h"
#include "core_ext.h"

#if !defined(__ARM_ARCH_8M_MAIN__) && !defined(__ARM_ARCH_8_1M_MAIN__)
#error "Unsupported ARM Architecture."
#endif

/* Delcaraction flag to control the scheduling logic in PendSV. */
uint32_t scheduler_lock = SCHEDULER_UNLOCKED;

/* IAR Specific */
#if defined(__ICCARM__)

#pragma required = scheduler_lock
#pragma required = tfm_core_svc_handler

#if CONFIG_TFM_SPM_BACKEND_IPC == 1
#pragma required = ipc_schedule
#endif /* CONFIG_TFM_SPM_BACKEND_IPC == 1*/

#if CONFIG_TFM_PSA_API_CROSS_CALL == 1

#pragma required = cross_call_entering_c
#pragma required = cross_call_exiting_c

#endif /* CONFIG_TFM_PSA_API_CROSS_CALL == 1*/

#endif

#if CONFIG_TFM_PSA_API_CROSS_CALL == 1

__naked void arch_non_preempt_call(uintptr_t fn_addr, uintptr_t frame_addr,
                                   uint32_t stk_base, uint32_t stk_limit)
{
    __asm volatile(
#if !defined(__ICCARM__)
        ".syntax unified                            \n"
#endif
        "   push   {r4-r6, lr}                      \n"
        "   cpsid  i                                \n"
        "   mov    r4, r2                           \n"
        "   mrs    r5, psplim                       \n"
        "   movs   r12, #0                          \n"
        "   cmp    r2, #0                           \n"
        "   itttt  ne                               \n" /* To callee stack */
        "   msrne  psplim, r12                      \n"
        "   movne  r4, sp                           \n"
        "   movne  sp, r2                           \n"
        "   msrne  psplim, r3                       \n"
        "   ldr    r2, =scheduler_lock              \n" /* To lock sched */
        "   movs   r3, #"M2S(SCHEDULER_LOCKED)"     \n"
        "   str    r3, [r2, #0]                     \n"
        "   cpsie  i                                \n"
        "   mov    r6, r1                           \n"
        "   bl     cross_call_entering_c            \n"
        "   cpsid  i                                \n"
        "   mov    r1, r6                           \n"
        "   bl     cross_call_exiting_c             \n"
        "   movs   r12, #0                          \n"
        "   cmp    r4, #0                           \n"
        "   ittt   ne                               \n" /* To caller stack */
        "   msrne  psplim, r12                      \n"
        "   movne  sp, r4                           \n"
        "   msrne  psplim, r5                       \n"
        "   ldr    r4, =scheduler_lock              \n" /* To unlock sched */
        "   movs   r5, #"M2S(SCHEDULER_UNLOCKED)"   \n"
        "   str    r5, [r4, #0]                     \n"
        "   cpsie  i                                \n"
        "   pop    {r4-r6, pc}                      \n"
    );
}

#endif /* CONFIG_TFM_PSA_API_CROSS_CALL == 1*/

#if CONFIG_TFM_SPM_BACKEND_IPC == 1
__attribute__((naked)) void PendSV_Handler(void)
{
    __ASM volatile(
#if !defined(__ICCARM__)
        ".syntax unified                                \n"
#endif
        "   movs    r0, #"M2S(EXC_RETURN_SECURE_STACK)" \n"
        "   ands    r0, lr                              \n" /* NS interrupted */
        "   beq     v8m_pendsv_exit                     \n" /* No schedule */
        "   push    {r0, lr}                            \n" /* Save R0, LR */
        "   bl      ipc_schedule                         \n"
        "   pop     {r2, lr}                            \n"
        "   cmp     r0, r1                              \n" /* curr, next ctx */
        "   beq     v8m_pendsv_exit                     \n" /* No schedule */
        "   cpsid   i                                   \n"
        "   mrs     r2, psp                             \n"
        "   stmdb   r2!, {r4-r11}                       \n" /* Save callee */
        "   stmia   r0, {r2, lr}                        \n" /* Save curr ctx:
                                                             * PSP, LR
                                                             */
        "   ldmia   r1!, {r2, lr}                       \n" /* Load next ctx:
                                                             * PSP, LR
                                                             */
        "   ldr     r3, [r1]                            \n" /* Load sp_limit */
        "   ldmia   r2!, {r4-r11}                       \n" /* Restore callee */
        "   msr     psp, r2                             \n"
        "   msr     psplim, r3                          \n"
        "   cpsie   i                                   \n"
        "v8m_pendsv_exit:                               \n"
        "   bx      lr                                  \n"
    );
}
#endif


__attribute__((naked)) void SVC_Handler(void)
{
    __ASM volatile(
    "MRS     r0, MSP                        \n"
    "MOV     r1, lr                         \n"
    "MRS     r2, PSP                        \n"
    "MRS     r3, PSPLIM                     \n"
    "PUSH    {r2, r3}                       \n" /* PSP PSPLIM */
    "PUSH    {r1, r2}                       \n" /* Orig_exc_return, dummy */
    "BL      tfm_core_svc_handler           \n"
    "MOV     lr, r0                         \n"
    "POP     {r1, r2}                       \n" /* Orig_exc_return, dummy */
    "POP     {r2, r3}                       \n" /* PSP PSPLIM */
    "AND     r0, #8                         \n" /* Mode bit */
    "AND     r1, #8                         \n"
    "SUBS    r0, r1                         \n" /* Compare EXC_RETURN values */
    "BGT     to_flih_func                   \n"
    "BLT     from_flih_func                 \n"
    "BX      lr                             \n"
    "to_flih_func:                          \n"
    "PUSH    {r2, r3}                       \n" /* PSP PSPLIM */
    "PUSH    {r4-r11}                       \n"
    "LDR     r4, ="M2S(STACK_SEAL_PATTERN)" \n" /* clear r4-r11 */
    "MOV     r5, r4                         \n"
    "MOV     r6, r4                         \n"
    "MOV     r7, r4                         \n"
    "MOV     r8, r4                         \n"
    "MOV     r9, r4                         \n"
    "MOV     r10, r4                        \n"
    "MOV     r11, r4                        \n"
    "PUSH    {r4, r5}                       \n" /* Seal stack before EXC_RET */
    "BX      lr                             \n"
    "from_flih_func:                        \n"
    "POP     {r4, r5}                       \n" /* Seal stack */
    "POP     {r4-r11}                       \n"
    "POP     {r1, r2}                       \n" /* PSP PSPLIM */
    "BX      lr                             \n"
    );
}

void tfm_arch_set_secure_exception_priorities(void)
{
    uint32_t VECTKEY;
    SCB_Type *scb = SCB;
    uint32_t AIRCR;

    /* Set PRIS flag in AIRCR */
    AIRCR = scb->AIRCR;
    VECTKEY = (~AIRCR & SCB_AIRCR_VECTKEYSTAT_Msk);
    scb->AIRCR = SCB_AIRCR_PRIS_Msk |
                 VECTKEY |
                 (AIRCR & ~SCB_AIRCR_VECTKEY_Msk);
    /* Set fault priority to less than 0x80 (with AIRCR.PRIS set) to prevent
     * Non-secure from pre-empting faults that may indicate corruption of Secure
     * state.
     */
    NVIC_SetPriority(MemoryManagement_IRQn, MemoryManagement_IRQnLVL);
    NVIC_SetPriority(BusFault_IRQn, BusFault_IRQnLVL);
    NVIC_SetPriority(SecureFault_IRQn, SecureFault_IRQnLVL);

    NVIC_SetPriority(SVCall_IRQn, SVCall_IRQnLVL);
    /*
     * Set secure PendSV priority to the lowest in SECURE state.
     */
    NVIC_SetPriority(PendSV_IRQn, PENDSV_PRIO_FOR_SCHED);
}

#ifdef TFM_FIH_PROFILE_ON
FIH_RET_TYPE(int32_t) tfm_arch_verify_secure_exception_priorities(void)
{
    SCB_Type *scb = SCB;

    if ((scb->AIRCR & SCB_AIRCR_PRIS_Msk) != SCB_AIRCR_PRIS_Msk) {
        FIH_RET(FIH_FAILURE);
    }
    fih_delay();
    if ((scb->AIRCR & SCB_AIRCR_PRIS_Msk) != SCB_AIRCR_PRIS_Msk) {
        FIH_RET(FIH_FAILURE);
    }
    if (fih_not_eq(fih_int_encode(NVIC_GetPriority(MemoryManagement_IRQn)),
                  fih_int_encode(MemoryManagement_IRQnLVL))) {
        FIH_RET(FIH_FAILURE);
    }
    if (fih_not_eq(fih_int_encode(NVIC_GetPriority(BusFault_IRQn)),
                  fih_int_encode(BusFault_IRQnLVL))) {
        FIH_RET(FIH_FAILURE);
    }
    if (fih_not_eq(fih_int_encode(NVIC_GetPriority(SecureFault_IRQn)),
                  fih_int_encode(SecureFault_IRQnLVL))) {
        FIH_RET(FIH_FAILURE);
    }
    if (fih_not_eq(fih_int_encode(NVIC_GetPriority(SVCall_IRQn)),
                  fih_int_encode(SVCall_IRQnLVL))) {
        FIH_RET(FIH_FAILURE);
    }
    if (fih_not_eq(fih_int_encode(NVIC_GetPriority(PendSV_IRQn)),
                  fih_int_encode(PENDSV_PRIO_FOR_SCHED))) {
        FIH_RET(FIH_FAILURE);
    }
    FIH_RET(FIH_SUCCESS);
}
#endif

void tfm_arch_config_extensions(void)
{
#if defined(CONFIG_TFM_ENABLE_CP10CP11)
    /*
     * Enable SPE privileged and unprivileged access to the FP Extension.
     * Note: On Armv8-M, if Non-secure access to the FPU is needed, Secure
     * access to the FPU must be enabled first in order to avoid No Coprocessor
     * (NOCP) usage fault when a Non-secure to Secure service call is
     * interrupted while CONTROL.FPCA=1 is set by Non-secure. This is needed
     * even if SPE will not use the FPU directly.
     */
    SCB->CPACR |= (3U << 10U*2U)     /* enable CP10 full access */
                  | (3U << 11U*2U);  /* enable CP11 full access */

    /*
     * Permit Non-secure access to the Floating-point Extension.
     * Note: It is still necessary to set CPACR_NS to enable the FP Extension
     * in the NSPE. This configuration is left to NS privileged software.
     */
    SCB->NSACR |= SCB_NSACR_CP10_Msk | SCB_NSACR_CP11_Msk;
#endif

#if (CONFIG_TFM_FLOAT_ABI >= 1)

#ifdef CONFIG_TFM_LAZY_STACKING
    /* Enable lazy stacking. */
    FPU->FPCCR |= FPU_FPCCR_LSPEN_Msk;
#else
    /* Disable lazy stacking. */
    FPU->FPCCR &= ~FPU_FPCCR_LSPEN_Msk;
#endif

    /*
     * If the SPE will ever use the floating-point registers for sensitive
     * data, then FPCCR.ASPEN, FPCCR.TS, FPCCR.CLRONRET and FPCCR.CLRONRETS
     * must be set at initialisation and not changed again afterwards.
     * Let SPE decide the S/NS shared setting (LSPEN and CLRONRET) to avoid the
     * possible side-path brought by flexibility. This is not needed
     * if the SPE will never use floating-point but enables the FPU only for
     * avoiding NOCP faults during interrupted NSPE to SPE calls.
     */
    FPU->FPCCR |= FPU_FPCCR_ASPEN_Msk
                  | FPU_FPCCR_TS_Msk
                  | FPU_FPCCR_CLRONRET_Msk
                  | FPU_FPCCR_CLRONRETS_Msk
                  | FPU_FPCCR_LSPENS_Msk;

    /* Prevent non-secure from modifying FPU’s power setting. */
    SCnSCB->CPPWR |= SCnSCB_CPPWR_SUS11_Msk | SCnSCB_CPPWR_SUS10_Msk;
#endif /* CONFIG_TFM_FLOAT_ABI >= 1 */

#if defined(__ARM_ARCH_8_1M_MAIN__)
    SCB->CCR |= SCB_CCR_TRD_Msk;
#endif
}

#if (CONFIG_TFM_FLOAT_ABI > 0)
__attribute__((naked, noinline, used)) void tfm_arch_clear_fp_data(void)
{
    __ASM volatile(
                    "eor  r0, r0, r0         \n"
                    "vmsr fpscr, r0          \n"
#if (defined(__ARM_ARCH_8_1M_MAIN__))
                    "vscclrm {s0-s31,vpr}    \n"
#else
                    "vmov s0, r0             \n"
                    "vmov s1, r0             \n"
                    "vmov s2, r0             \n"
                    "vmov s3, r0             \n"
                    "vmov s4, r0             \n"
                    "vmov s5, r0             \n"
                    "vmov s6, r0             \n"
                    "vmov s7, r0             \n"
                    "vmov s8, r0             \n"
                    "vmov s9, r0             \n"
                    "vmov s10, r0            \n"
                    "vmov s11, r0            \n"
                    "vmov s12, r0            \n"
                    "vmov s13, r0            \n"
                    "vmov s14, r0            \n"
                    "vmov s15, r0            \n"
                    "vmov s16, r0            \n"
                    "vmov s17, r0            \n"
                    "vmov s18, r0            \n"
                    "vmov s19, r0            \n"
                    "vmov s20, r0            \n"
                    "vmov s21, r0            \n"
                    "vmov s22, r0            \n"
                    "vmov s23, r0            \n"
                    "vmov s24, r0            \n"
                    "vmov s25, r0            \n"
                    "vmov s26, r0            \n"
                    "vmov s27, r0            \n"
                    "vmov s28, r0            \n"
                    "vmov s29, r0            \n"
                    "vmov s30, r0            \n"
                    "vmov s31, r0            \n"
#endif
                    "bx   lr                 \n"
                  );
}
#endif
