/*
 * Copyright (c) 2013-2020, Huawei Technologies Co., Ltd. All rights reserved.
 * Copyright (c) 2020-2021 Huawei Device Co., Ltd. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 *    conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 *    of conditions and the following disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 *    to endorse or promote products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _SOC_COMMON_H
#define _SOC_COMMON_H

#include "core_v5.h"

#define __start_and_irq_stack_top _stack
#define __except_stack_top _stack

#define LREG lw
#define SREG sw

#ifndef REGBYTES
#define REGBYTES 4
#endif

#define MCAUSE_INT_ID_MASK       (0xF)

#define EXC_SIZE_ON_STACK  (36 * REGBYTES)
#define INT_SIZE_ON_STACK  (32 * REGBYTES)

/* task TCB offset */
#define TASK_CB_KERNEL_SP       0x0
#define TASK_CB_STATUS          0x4

#define UINT32_CUT_MASK         0xFFFFFFFF
#define UINT8_CUT_MASK          0xFF
#define OS_MV_32_BIT            32

/************************ mstatus ************************/
#define RISCV_MSTATUS_UIE                   0x00000001
#define RISCV_MSTATUS_MIE                   0x00000008
#define RISCV_MSTATUS_UPIE                  0x00000010
#define RISCV_MSTATUS_MPIE                  0x00000080
#define RISCV_MSTATUS_MPP                   0x00001800

/************************ mie ***************************/
#define RISCV_MIE_USIE                      0x000000001
#define RISCV_MIE_MSIE                      0x000000008
#define RISCV_MIE_UTIE                      0x000000010
#define RISCV_MIE_MTIE                      0x000000080
#define RISCV_MIE_UEIE                      0x000000100
#define RISCV_MIE_MEIE                      0x000000800

/************************** mcause ***********************/
#define RISCV_MCAUSE_ECALL_U                8

#define RISCV_USER_SOFT_IRQ                 0
#define RISCV_MACH_SOFT_IRQ                 3
#define RISCV_USER_TIMER_IRQ                4
#define RISCV_MACH_TIMER_IRQ                7
#define RISCV_USER_EXT_IRQ                  8
#define RISCV_MACH_EXT_IRQ                  11


#define READ_CSR(reg) ({                                          \
    UINT32 _tmp;                                                  \
    __asm__ volatile("csrr %0, " #reg : "=r"(_tmp) : : "memory"); \
    _tmp;                                                         \
})

#define WRITE_CSR(reg, val) ({                                    \
    __asm__ volatile("csrw " #reg ", %0" : : "r"(val) : "memory"); \
})

#define SET_CSR(reg, val) ({                                       \
    __asm__ volatile("csrs " #reg ", %0" : : "r"(val) : "memory"); \
})

#define CLEAR_CSR(reg, val) ({                                     \
    __asm__ volatile("csrc " #reg ", %0" : : "r"(val) : "memory"); \
})

#define READ_CUSTOM_CSR(reg) ({                                         \
    UINT32 _tmp;                                                        \
    __asm__ volatile("csrr %0, %1" : "=r"(_tmp) : "i"(reg) : "memory"); \
    _tmp;                                                               \
})

#define WRITE_CUSTOM_CSR(reg, val) ({                             \
    __asm__ volatile("csrw %0, %1" : : "i"(reg), "r"(val) : "memory"); \
})

#define SET_CUSTOM_CSR(reg, val) ({                                \
    __asm__ volatile("csrs " #reg ", %0" : : "r"(val) : "memory"); \
})

#define CLEAR_CUSTOM_CSR(reg, val) ({                              \
    __asm__ volatile("csrc " #reg ", %0" : : "r"(val) : "memory"); \
})

#endif
