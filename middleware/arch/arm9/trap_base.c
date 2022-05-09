// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdint.h>
#include "boot.h"
#include "bk_private/reset_reason.h"
#include <os/os.h>
#include "bk_arch.h"
#include "stack_base.h"
#include "bk_uart.h"
#include "bk_rtos_debug.h"
#include <os/os.h>

struct arm9_register
{
	uint32_t r0;
	uint32_t r1;
	uint32_t r2;
	uint32_t r3;
	uint32_t r4;
	uint32_t r5;
	uint32_t r6;
	uint32_t r7;
	uint32_t r8;
	uint32_t r9;
	uint32_t r10;
	uint32_t fp;
	uint32_t ip;
	uint32_t sp;
	uint32_t lr;
	uint32_t pc;
	uint32_t spsr;
	uint32_t cpsr;
};

/**
 * this function will show registers of CPU
 *
 * @param regs the registers point
 */

void arch_dump_cpu_registers (struct arm9_register *regs)
{
	os_printf("Current regs:\n");
	os_printf("r00:0x%08x r01:0x%08x r02:0x%08x r03:0x%08x\n",
		regs->r0, regs->r1, regs->r2, regs->r3);
	os_printf("r04:0x%08x r05:0x%08x r06:0x%08x r07:0x%08x\n",
		regs->r4, regs->r5, regs->r6, regs->r7);
	os_printf("r08:0x%08x r09:0x%08x r10:0x%08x\n",
		regs->r8, regs->r9, regs->r10);
	os_printf("fp :0x%08x ip :0x%08x\n",
		regs->fp, regs->ip);
	os_printf("sp :0x%08x lr :0x%08x pc :0x%08x\n",
		regs->sp, regs->lr, regs->pc);
	os_printf("SPSR:0x%08x\n", regs->spsr);
	os_printf("CPSR:0x%08x\n", regs->cpsr);

	const unsigned int *reg_fiq, *reg_irq, *reg_und, *reg_abt, *reg_svc, *reg_sys;
	int i;

	reg_fiq = (const unsigned int *)MCU_REG_BACKUP_BOTTOM_FIQ;
	reg_irq = (const unsigned int *)MCU_REG_BACKUP_BOTTOM_IRQ;
	reg_und = (const unsigned int *)MCU_REG_BACKUP_BOTTOM_UND;
	reg_abt = (const unsigned int *)MCU_REG_BACKUP_BOTTOM_ABT;
	reg_svc = (const unsigned int *)MCU_REG_BACKUP_BOTTOM_SVC;
	reg_sys = (const unsigned int *)MCU_REG_BACKUP_BOTTOM_SYS;

	os_printf("\nseparate regs:\n");
	os_printf(" reg      FIQ        IRQ        UND        ABT        SVC        SYS\r\n");
	os_printf("------ ---------- ---------- ---------- ---------- ---------- ----------\r\n");
	os_printf("%-6s 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x\r\n", "cpsr", reg_fiq[0], reg_irq[0],
		reg_und[0], reg_abt[0], reg_svc[0], reg_sys[0]);
	os_printf("%-6s 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x\r\n", "spsr", reg_fiq[1], reg_irq[1],
		reg_und[1], reg_abt[1], reg_svc[1], reg_sys[1]);
	os_printf("%-6s 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x\r\n", "r13-SP", reg_fiq[7], reg_irq[7],
		reg_und[7], reg_abt[7], reg_svc[7], reg_sys[7]);
	os_printf("%-6s 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x\r\n", "r14-LR", reg_fiq[8], reg_irq[8],
		reg_und[8], reg_abt[8], reg_svc[8], reg_sys[8]);

	//display backup R8~R12 of FIQ
	for (i=2; i<7; i++) {
		os_printf("r%-5d 0x%08x %-10s %-10s %-10s %-10s %-10s\r\n", i+6, reg_fiq[i],
			"-", "-", "-", "-", "-");
	}

	os_printf("\r\n");
}

static void arch_dump_exception_info(const char* exception_str, struct arm9_register *regs)
{
	os_printf("%s\n", exception_str);
	arch_dump_cpu_registers(regs);
	rtos_dump_task_list();
	rtos_dump_backtrace();
	arch_dump_exception_stack();
}

/**
 * When ARM7TDMI comes across an instruction which it cannot handle,
 * it takes the undefined instruction trap.
 *
 * @param regs system registers
 *
 * @note never invoke this function in application
 */
void boot_exception_undefine(struct arm9_register *regs)
{
    bk_misc_update_set_type(CRASH_UNDEFINED_VALUE);

    arch_dump_exception_info("undefined instruction", regs);
    rtos_shutdown();
}

/**
 * The software interrupt instruction (SWI) is used for entering
 * Supervisor mode, usually to request a particular supervisor
 * function.
 *
 * @param regs system registers
 *
 * @note never invoke this function in application
 */
void boot_exception_swi(struct arm9_register *regs)
{
    arch_dump_exception_info("software interrupt", regs);
    rtos_shutdown();
}

/**
 * An abort indicates that the current memory access cannot be completed,
 * which occurs during an instruction prefetch.
 *
 * @param regs system registers
 *
 * @note never invoke this function in application
 */
void boot_exception_prefetch_abort(struct arm9_register *regs)
{
    bk_misc_update_set_type(CRASH_PREFETCH_ABORT_VALUE);

    arch_dump_exception_info("prefetch abort", regs);
    rtos_shutdown();
}

/**
 * An abort indicates that the current memory access cannot be completed,
 * which occurs during a data access.
 *
 * @param regs system registers
 *
 * @note never invoke this function in application
 */
void boot_exception_data_abort(struct arm9_register *regs)
{
    bk_misc_update_set_type(CRASH_DATA_ABORT_VALUE);

    arch_dump_exception_info("data abort", regs);
    rtos_shutdown();
}

/**
 * Normally, system will never reach here
 *
 * @param regs system registers
 *
 * @note never invoke this function in application
 */
void boot_exception_reserved(struct arm9_register *regs)
{
    bk_misc_update_set_type(CRASH_UNUSED_VALUE);

    arch_dump_exception_info("not used", regs);
    rtos_shutdown();
}
