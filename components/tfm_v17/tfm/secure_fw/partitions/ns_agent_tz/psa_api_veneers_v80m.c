/*
 * Copyright (c) 2018-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>

#include "cmsis_compiler.h"
#include "config_impl.h"
#include "security_defs.h"
#include "svc_num.h"
#include "utilities.h"

#include "psa/client.h"

#if CONFIG_TFM_PSA_API_CROSS_CALL == 1
#include "spm_ipc.h"
#include "ffm/psa_api.h"
#elif CONFIG_TFM_PSA_API_SFN_CALL == 1
#include "tfm_psa_call_pack.h"
#endif

/*
 * This is the veneers of FF-M Client APIs for Armv8.0-m.
 * The interfaces are written in assembly, and the reasons:
 *
 * - On the 8.0 version of Armv8-M with security extension, a mandatory
 *   software solution needs to be applied because hardware reentrant
 *   detection was not involved until 8.1. It requires TF-M to take full
 *   control of stack usage to perform reentrant detection. Therefore
 *   TF-M shall implement these veneers in the assembly code.
 *
 * - The non-secure context is pushed in the stack. When SPM API returns,
 *   the pushed non-secure context is popped and overrides the returned
 *   context before returning to NSPE. Therefore it is unnecessary to
 *   explicitly clean up the context.
 */

#if defined(__ICCARM__)

#if CONFIG_TFM_PSA_API_CROSS_CALL == 1

#pragma required = tfm_spm_client_psa_framework_version
#pragma required = tfm_spm_client_psa_version
#pragma required = tfm_spm_client_psa_call
#pragma required = spm_interface_cross_dispatcher
/* Following PSA APIs are only needed by connection-based services */
#if CONFIG_TFM_CONNECTION_BASED_SERVICE_API == 1
#pragma required = tfm_spm_client_psa_connect
#pragma required = tfm_spm_client_psa_close
#endif /* CONFIG_TFM_CONNECTION_BASED_SERVICE_API */

#elif CONFIG_TFM_PSA_API_SFN_CALL == 1
#pragma required = psa_close_sfn
#pragma required = psa_connect_sfn
#pragma required = psa_call_pack_sfn
#pragma required = psa_framework_version_sfn
#pragma required = psa_version_sfn
#endif /* CONFIG_TFM_PSA_API_CROSS_CALL == 1 */

#endif

__attribute__((naked, used))
static void clear_caller_context(void)
{
    __ASM volatile(
#if (CONFIG_TFM_FP >= 1)
        "   vmov.f32   s0, #1.0                               \n"
        "   vmov.f32   s1, #1.0                               \n"
        "   vmov.f32   s2, #1.0                               \n"
        "   vmov.f32   s3, #1.0                               \n"
        "   vmov.f32   s4, #1.0                               \n"
        "   vmov.f32   s5, #1.0                               \n"
        "   vmov.f32   s6, #1.0                               \n"
        "   vmov.f32   s7, #1.0                               \n"
        "   vmov.f32   s8, #1.0                               \n"
        "   vmov.f32   s9, #1.0                               \n"
        "   vmov.f32   s10, #1.0                              \n"
        "   vmov.f32   s11, #1.0                              \n"
        "   vmov.f32   s12, #1.0                              \n"
        "   vmov.f32   s13, #1.0                              \n"
        "   vmov.f32   s14, #1.0                              \n"
        "   vmov.f32   s15, #1.0                              \n"
        "   vmrs   r12, fpscr                                 \n"
        "   movw   r1, #0x009f                                \n"
        "   movt   r1, #0xf000                                \n"
        "   bics   r12, r1                                    \n"
        "   vmsr   fpscr, r12                                 \n"
#endif
        "   movs    r3, #0x0                                  \n"
        "   msr    APSR_nzcvq, r3                             \n"
        "   bx     lr                                         \n"
    );
}

__tz_naked_veneer
uint32_t tfm_psa_framework_version_veneer(void)
{
    __ASM volatile(
#if !defined(__ICCARM__)
        ".syntax unified                                      \n"
#endif

        "   ldr    r2, [sp]                                   \n"
        "   ldr    r3, ="M2S(STACK_SEAL_PATTERN)"             \n"
        "   cmp    r2, r3                                     \n"
        "   bne    reent_panic1                               \n"
        "   push   {r4, lr}                                   \n"

#if CONFIG_TFM_PSA_API_CROSS_CALL == 1
        "   push   {r0-r3}                                    \n"
        "   ldr    r0, =tfm_spm_client_psa_framework_version  \n"
        "   mov    r1, sp                                     \n"
        "   bl     spm_interface_cross_dispatcher             \n"
        "   pop    {r0-r3}                                    \n"
#elif CONFIG_TFM_PSA_API_SFN_CALL == 1
        "   bl     psa_framework_version_sfn                  \n"
#else
        "   svc    "M2S(TFM_SVC_PSA_FRAMEWORK_VERSION)"       \n"
#endif
        "   bl     clear_caller_context                       \n"
        "   pop    {r1, r2}                                   \n"
        "   mov    lr, r2                                     \n"
        "   mov    r4, r1                                     \n"
        "   bxns   lr                                         \n"

        "reent_panic1:                                        \n"
        "   svc    "M2S(TFM_SVC_PSA_PANIC)"                   \n"
        "   b      .                                          \n"
    );
}

__tz_naked_veneer
uint32_t tfm_psa_version_veneer(uint32_t sid)
{
    __ASM volatile(
#if !defined(__ICCARM__)
        ".syntax unified                                      \n"
#endif

        "   ldr    r2, [sp]                                   \n"
        "   ldr    r3, ="M2S(STACK_SEAL_PATTERN)"             \n"
        "   cmp    r2, r3                                     \n"
        "   bne    reent_panic2                               \n"

        "   push   {r4, lr}                                   \n"
#if CONFIG_TFM_PSA_API_CROSS_CALL == 1
        "   push   {r0-r3}                                    \n"
        "   ldr    r0, =tfm_spm_client_psa_version            \n"
        "   mov    r1, sp                                     \n"
        "   bl     spm_interface_cross_dispatcher             \n"
        "   pop    {r0-r3}                                    \n"
#elif CONFIG_TFM_PSA_API_SFN_CALL == 1
        "   bl     psa_version_sfn                            \n"
#else
        "   svc    "M2S(TFM_SVC_PSA_VERSION)"                 \n"
#endif
        "   bl     clear_caller_context                       \n"
        "   pop    {r1, r2}                                   \n"
        "   mov    lr, r2                                     \n"
        "   mov    r4, r1                                     \n"
        "   bxns   lr                                         \n"

        "reent_panic2:                                        \n"
        "   svc    "M2S(TFM_SVC_PSA_PANIC)"                   \n"
        "   b      .                                          \n"
    );
}

__tz_naked_veneer
psa_status_t tfm_psa_call_veneer(psa_handle_t handle,
                                 uint32_t ctrl_param,
                                 const psa_invec *in_vec,
                                 psa_outvec *out_vec)
{
    __ASM volatile(
#if !defined(__ICCARM__)
        ".syntax unified                                      \n"
#endif

        "   push   {r2, r3}                                   \n"
        "   ldr    r2, [sp, #8]                               \n"
        "   ldr    r3, ="M2S(STACK_SEAL_PATTERN)"             \n"
        "   cmp    r2, r3                                     \n"
        "   bne    reent_panic4                               \n"
        "   pop    {r2, r3}                                   \n"
        "   push   {r4, lr}                                   \n"
#if CONFIG_TFM_PSA_API_CROSS_CALL == 1
        "   push   {r0-r3}                                    \n"
        "   ldr    r0, =tfm_spm_client_psa_call               \n"
        "   mov    r1, sp                                     \n"
        "   bl     spm_interface_cross_dispatcher             \n"
        "   pop    {r0-r3}                                    \n"
#elif CONFIG_TFM_PSA_API_SFN_CALL == 1
        "   bl     psa_call_pack_sfn                          \n"
#else
        "   svc    "M2S(TFM_SVC_PSA_CALL)"                    \n"
#endif
        "   bl     clear_caller_context                       \n"
        "   pop    {r1, r2}                                   \n"
        "   mov    lr, r2                                     \n"
        "   mov    r4, r1                                     \n"
        "   bxns   lr                                         \n"

        "reent_panic4:                                        \n"
        "   svc    "M2S(TFM_SVC_PSA_PANIC)"                   \n"
        "   b      .                                          \n"
    );
}

/* Following veneers are only needed by connection-based services */
#if CONFIG_TFM_CONNECTION_BASED_SERVICE_API == 1

__tz_naked_veneer
psa_handle_t tfm_psa_connect_veneer(uint32_t sid, uint32_t version)
{
    __ASM volatile(
#if !defined(__ICCARM__)
        ".syntax unified                                      \n"
#endif

        "   ldr    r2, [sp]                                   \n"
        "   ldr    r3, ="M2S(STACK_SEAL_PATTERN)"             \n"
        "   cmp    r2, r3                                     \n"
        "   bne    reent_panic3                               \n"

        "   push   {r4, lr}                                   \n"
#if CONFIG_TFM_PSA_API_CROSS_CALL == 1
        "   push   {r0-r3}                                    \n"
        "   ldr    r0, =tfm_spm_client_psa_connect            \n"
        "   mov    r1, sp                                     \n"
        "   bl     spm_interface_cross_dispatcher             \n"
        "   pop    {r0-r3}                                    \n"
#elif CONFIG_TFM_PSA_API_SFN_CALL == 1
        "   bl     psa_connect_sfn                            \n"
#else
        "   svc    "M2S(TFM_SVC_PSA_CONNECT)"                 \n"
#endif
        "   bl     clear_caller_context                       \n"
        "   pop    {r1, r2}                                   \n"
        "   mov    lr, r2                                     \n"
        "   mov    r4, r1                                     \n"
        "   bxns   lr                                         \n"

        "reent_panic3:                                        \n"
        "   svc    "M2S(TFM_SVC_PSA_PANIC)"                   \n"
        "   b      .                                          \n"
    );
}

__tz_naked_veneer
void tfm_psa_close_veneer(psa_handle_t handle)
{
    __ASM volatile(
#if !defined(__ICCARM__)
        ".syntax unified                                      \n"
#endif

        "   ldr    r2, [sp]                                   \n"
        "   ldr    r3, ="M2S(STACK_SEAL_PATTERN)"             \n"
        "   cmp    r2, r3                                     \n"
        "   bne    reent_panic5                               \n"

        "   push   {r4, lr}                                   \n"
#if CONFIG_TFM_PSA_API_CROSS_CALL == 1
        "   push   {r0-r3}                                    \n"
        "   ldr    r0, =tfm_spm_client_psa_close              \n"
        "   mov    r1, sp                                     \n"
        "   bl     spm_interface_cross_dispatcher             \n"
        "   pop    {r0-r3}                                    \n"
#elif CONFIG_TFM_PSA_API_SFN_CALL == 1
        "   bl     psa_close_sfn                              \n"
#else
        "   svc    "M2S(TFM_SVC_PSA_CLOSE)"                   \n"
#endif
        "   bl     clear_caller_context                       \n"
        "   pop    {r1, r2}                                   \n"
        "   mov    lr, r2                                     \n"
        "   mov    r4, r1                                     \n"
        "   bxns   lr                                         \n"

        "reent_panic5:                                        \n"
        "   svc    "M2S(TFM_SVC_PSA_PANIC)"                   \n"
        "   b      .                                          \n"
    );
}

#endif /* CONFIG_TFM_CONNECTION_BASED_SERVICE_API */
