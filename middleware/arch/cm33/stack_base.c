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

#include <string.h>
#include <stdint.h>
#include "boot.h"
#include "arm.h"
#include "bk_arch.h"
#include "bk_uart.h"
#include <components/system.h>
#include <os/os.h>

extern uint32_t und_stack_start;
extern uint32_t abt_stack_start;
extern uint32_t fiq_stack_start;
extern uint32_t irq_stack_start;
extern uint32_t sys_stack_start;
extern uint32_t svc_stack_start;

extern void fiq_handler();
extern char __flash_txt_end;
extern char _itcmcode_ram_begin, _itcmcode_ram_end;

#define STACK_CALLBACK_BUF_SIZE 32

#if STACK_DUMP_MEMORY
static void stack_mem_dump(uint32_t stack_top, uint32_t stack_bottom)
{
	uint32_t data = *((uint32_t *) stack_top);
	uint32_t cnt = 0;

	os_printf("cur stack_top=%08x, stack end=%08x\n", stack_top, stack_bottom);
	for (;  stack_top < stack_bottom; stack_top += sizeof(size_t)) {
		data = *((uint32_t *) stack_top);
		if ((cnt++ % 4) == 0)
			os_printf("\n%08x: ", stack_top);
		os_printf("%08x ", data);
	}
	os_printf("\n");
}
#endif

static bool code_addr_is_valid(uint32_t addr)
{
	//TODO
	return false;
}

/* The stack is grow from bottom to top
 *
 *   |   | <- minimum_addr = (bottom - stack_size)
 *   |   |
 *   |   |
 *   |   | <- top
 *   | . |
 *   | . |
 *   | . | <- bottom, (big address)
 *
 * */
void arch_parse_stack_backtrace(const char *str_type, uint32_t stack_top, uint32_t stack_bottom,
						   uint32_t stack_size, bool thumb_mode)
{
	uint32_t call_stack_buf[STACK_CALLBACK_BUF_SIZE] = {0};
	uint32_t stack_minimum = stack_bottom - stack_size;
	uint32_t pc;
	int call_stack_index = 0;
	uint32_t init_stack_top = stack_top;

#if STACK_DUMP_MEMORY
	stack_mem_dump(stack_top, stack_bottom);
#endif
	for (; stack_top < stack_bottom && (call_stack_index < STACK_CALLBACK_BUF_SIZE); stack_top += sizeof(size_t)) {
		pc = *((uint32_t *) stack_top);

		/* ARM9 using thumb instruction, so the pc must be an odd number */
		if (thumb_mode && (pc & 1) == 0)
			continue;

		if (code_addr_is_valid(pc)) {
			if (pc & 1)
				pc = pc - 1;

			call_stack_buf[call_stack_index] = pc;
			call_stack_index++;
		}
	}

	if (call_stack_index > 0) {
		int index;

		os_printf("%-16s   [0x%-6x ~ 0x%-6x]   0x%-6x   %-4d   %-8d   ",
				  str_type, stack_minimum, stack_bottom, init_stack_top, stack_size, init_stack_top < stack_minimum);
		for (index = 0; index < call_stack_index; index++)
			os_printf("%lx ", call_stack_buf[index]);
		os_printf("\n");
	} else if (init_stack_top < stack_minimum) {
		os_printf("%-16s   [0x%-6x ~ 0x%-6x]   0x%-6x   %-4d   %-8d   ",
				  str_type, stack_minimum, stack_bottom, init_stack_top, stack_size, init_stack_top < stack_minimum);
	}
}

void arch_dump_exception_stack(void)
{
	uint32_t stack_bottom;
	uint32_t sp;

	sp = *(uint32_t*)(MCU_REG_BACKUP_SP_FIQ);
	stack_bottom = (uint32_t)&fiq_stack_start;
	arch_parse_stack_backtrace("fiq", sp, stack_bottom, FIQ_STACK_SIZE, false);

	sp = *(uint32_t*)(MCU_REG_BACKUP_SP_IRQ);
	stack_bottom = (uint32_t)&irq_stack_start;
	arch_parse_stack_backtrace("irq", sp, stack_bottom, IRQ_STACK_SIZE, false);

	sp = *(uint32_t*)(MCU_REG_BACKUP_SP_UND);
	stack_bottom = (uint32_t)&und_stack_start;
	arch_parse_stack_backtrace("und", sp, stack_bottom, UND_STACK_SIZE, false);

	sp = *(uint32_t*)(MCU_REG_BACKUP_SP_ABT);
	stack_bottom = (uint32_t)&abt_stack_start;
	arch_parse_stack_backtrace("abt", sp, stack_bottom, ABT_STACK_SIZE, false);

	sp = *(uint32_t*)(MCU_REG_BACKUP_SP_SVC);
	stack_bottom = (uint32_t)&svc_stack_start;
	arch_parse_stack_backtrace("svc", sp, stack_bottom, SVC_STACK_SIZE, false);

	sp = *(uint32_t*)(MCU_REG_BACKUP_SP_SYS);
	stack_bottom = (uint32_t)&sys_stack_start;
	arch_parse_stack_backtrace("sys", sp, stack_bottom, SYS_STACK_SIZE, false);
}
