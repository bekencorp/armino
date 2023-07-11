/*
 * Copyright (c) 2021-2022 Beken Corporation
 * All rights reserved.
 *
 */

#include "core_v5.h"

#define MTIMER                     (0xE6000000)
#define MTIMERCMP                  (0xE6000008)

.global  arch_int_disable
.global  arch_int_enable
.global  arch_int_restore

.global  arch_fence
.global  arch_atomic_clear
.global  arch_atomic_set

.global  riscv_get_mtimercmp
.global  riscv_get_mtimer
.global  riscv_set_mtimercmp
.global  riscv_get_cycle
.global  riscv_get_instruct_cnt
.global  arch_get_int_status
.global  riscv_sys_call1
.global  riscv_sys_call2
.global  riscv_sys_call3
.global  riscv_sys_call4

//.section .text
.section .itcm_sec_code, "ax"

/*
 * u32 arch_get_int_status(void);
 */
arch_get_int_status:
    csrr    a0, uip           // return value
    csrr	t0, uie
    and 	a0, a0, t0
    ret

/*
 * u32 arch_int_disable(void);
 */
arch_int_disable:
	csrrci	a0, ustatus, USTATUS_UIE
    ret

/*
 * void arch_int_enable(void);
 */
arch_int_enable:
    csrsi  ustatus, USTATUS_UIE
    ret

/*
 * void arch_int_restore(u32 int_flag);
 */
arch_int_restore:
    csrw ustatus, a0
    ret

#if 1
/*
 * void arch_fence(void);
 */
arch_fence:
//	fence iorw, iorw
	ret

/*
 * void arch_atomic_clear(u32 * lock_addr);
 */
arch_atomic_clear:
	amoswap.w.rl x0, x0, (a0)
	ret

/*
 * void arch_atomic_set(u32 * lock_addr);
 */
arch_atomic_set:
	addi t0, x0, 1
swap_again: 
	amoswap.w.aq t0, t0, (a0)
	bnez t0, swap_again
	ret

#endif

/*
 * u64 riscv_get_mtimercmp(void);
 */
riscv_get_mtimercmp:
	li t0, MTIMERCMP
	lw a0, 0(t0)
	lw a1, 4(t0)
	ret

/*
 * u64 riscv_get_mtimer(void);
 */
riscv_get_mtimer:
	li t0, MTIMER
read_mtimer: 
	lw a1, 4(t0)
	lw a0, 0(t0)
	lw t1, 4(t0)
	bne a1, t1, read_mtimer
	ret

/*
 * void riscv_set_mtimercmp(u64 new_time);
 */
riscv_set_mtimercmp:
	li t0, MTIMERCMP
	li t1, -1
	sw t1, 4(t0)
	fence
	sw a0, 0(t0)
	sw a1, 4(t0)
	ret

/*
 * u64 riscv_get_cycle(void);
 */
riscv_get_cycle:
read_cycle_again:
	rdcycleh a1
	rdcycle  a0
	rdcycleh t0
	bne a1, t0, read_cycle_again
	ret

/*
 * u64 riscv_get_instruct_cnt(void);
 */
riscv_get_instruct_cnt:
read_instr_again:
	rdinstreth a1
	rdinstret  a0
	rdinstreth t0
	bne a1, t0, read_instr_again
	ret

/*
 * u32 riscv_sys_call(u32 id, ....);
 */
riscv_sys_call1:
	ecall
	ret

riscv_sys_call2:
	ecall
	ret

riscv_sys_call3:
	ecall
	ret

riscv_sys_call4:
	ecall
	ret


