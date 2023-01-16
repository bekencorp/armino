/*
 * Copyright (c) 2012-2017 Andes Technology Corporation
 * All rights reserved.
 *
 */
#include "core_v5.h"

	.section .nds_vector, "ax"

.option push
.option norelax

	.global reset_vector
	.extern xISRStackTop

reset_vector:
	/* Decide whether this is an NMI or cold reset */
//	csrw uscratch, t0
	csrr t0, ucause
	bnez t0, nmi_handler

	.global _start

_start:
	/* Initialize global pointer */
	.option push
	.option norelax
	la gp, __global_pointer$
	.option pop

	/* Initialize stack pointer */
	la t0, _stack
	mv sp, t0

#ifdef __nds_execit
	/* Initialize EXEC.IT table */
	la t0, _ITB_BASE_
	csrw uitb, t0
#endif

#ifdef __riscv_flen
	/* Initialize FCSR */
	fscsr zero
#endif

	la t0, init_trap_handler
	csrw utvec, t0

	/* Do system low level setup. It must be a leaf function */
	call __platform_init

	/* System reset handler */
	call reset_handler

	/* Infinite loop, if returned accidently */
1:	j 1b

	.weak __platform_init
__platform_init:
	ret

	.weak nmi_handler
nmi_handler:

#if 0
	csrrw t0, uscratch, x0
	addi sp, sp, -64
//	sw ra, 0(sp)
	
	sw x5, 4(sp)			
	sw x6, 8(sp)
	sw x7, 12(sp)

	sw x10, 16(sp)
	sw x11, 20(sp)
	sw x12, 24(sp)
	sw x13, 28(sp)
	sw x14, 32(sp)
	sw x15, 36(sp)
	sw x16, 40(sp)
	sw x17, 44(sp)
	sw x28, 48(sp)
	sw x29, 52(sp)
	sw x30, 56(sp)
	sw x31, 60(sp)
#endif

	csrrw a0, ucause, x0
	mv  a1, ra
	call user_nmi_handler
	li a0, TRAP_M_USER_NP_WDT
	csrw ucause, a0

#if 0
//	lw ra, 0(sp)
	lw x5, 4(sp)			
	lw x6, 8(sp)
	lw x7, 12(sp)

	lw x10, 16(sp)
	lw x11, 20(sp)
	lw x12, 24(sp)
	lw x13, 28(sp)
	lw x14, 32(sp)
	lw x15, 36(sp)
	lw x16, 40(sp)
	lw x17, 44(sp)
	lw x28, 48(sp)
	lw x29, 52(sp)
	lw x30, 56(sp)
	lw x31, 60(sp)

	addi sp, sp, 64
#endif

	csrr ra, utvec
	jr ra

.org  0x100
.word 0x32374B42
.word 0x00003635
.word 0x00000000
.word 0x00000000
.org  0x110
.word 0x64616568    /*head: sign head address*/
.word 0x00000000
.word 0x00000000
.word 0x00000000
.org  0x120
.word 0x00000000    /*version: provide to version number setting*/
.word 0x00000000
.word 0x00000000
.word 0x00000000

1:	j 1b

init_trap_handler:
	addi sp, sp, -120
	sw x1, 0(sp)
	sw x5, 4(sp)
	sw x6, 8(sp)
	sw x7, 12(sp)
	sw x10, 16(sp)
	sw x11, 20(sp)
	sw x12, 24(sp)
	sw x13, 28(sp)
	sw x14, 32(sp)
	sw x15, 36(sp)
	sw x16, 40(sp)
	sw x17, 44(sp)
	sw x28, 48(sp)
	sw x29, 52(sp)
	sw x30, 56(sp)
	sw x31, 60(sp)

	csrr a1, uepc
	csrr a2, ustatus
	sw a1, 64(sp)
	sw a2, 68(sp)

	mv a1, sp

	sw x8, 72(sp)
	sw x9, 76(sp)
	sw x18, 80(sp)
	sw x19, 84(sp)
	sw x20, 88(sp)
	sw x21, 92(sp)
	sw x22, 96(sp)
	sw x23, 100(sp)
	sw x24, 104(sp)
	sw x25, 108(sp)
	sw x26, 112(sp)
	sw x27, 116(sp)

	csrrw a0, ucause, x0

	call trap_handler

1:	j 1b

.option pop

	.section .text

	.global trap_entry
	.align 2

trap_entry:
	li t0, TRAP_M_USER_ASSERT
	csrw ucause, t0
	csrr t0, utvec
	jr t0

