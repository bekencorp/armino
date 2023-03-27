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
#include "bk_arch.h"
#include "bk_uart.h"
#include <components/log.h>
#include <common/bk_assert.h>

#define STACK_DUMP_MEMORY 0
#define STACK_CALLBACK_BUF_SIZE 32

extern char __executable_start;
extern char __etext;
extern char _itcm_ema_start, _itcm_ema_end;

static int addr_is_in_flash_txt(uint32_t addr)
{
	return ((addr > 0x100) && (addr < (uint32_t)&__etext));
}

static int addr_is_in_itcm_txt(uint32_t addr)
{
	return ((addr > (uint32_t)&_itcm_ema_start) && (addr < (uint32_t)&_itcm_ema_end));
}

static int code_addr_is_valid(uint32_t addr)
{
	return (addr_is_in_flash_txt(addr) || addr_is_in_itcm_txt(addr));
}

void stack_mem_dump(uint32_t stack_top, uint32_t stack_bottom)
{
	unsigned char *data;
	uint32_t cnt = 0;
	uint32_t sp = stack_top;
	uint32_t fp = stack_bottom;

	BK_DUMP_OUT(">>>>stack mem dump begin, stack_top=%08x, stack end=%08x\r\n", stack_top, stack_bottom);
	for (;  sp < fp; sp += sizeof(size_t)) {
		data = ((unsigned char *) sp);

		if ((cnt++ & 0x7) == 0) {
			BK_DUMP_OUT("\r\n");
		}

		BK_DUMP_OUT("%02x %02x %02x %02x ", data[0], data[1], data[2], data[3]);
	}
	BK_DUMP_OUT("\r\n");
	BK_DUMP_OUT("<<<<stack mem dump end. stack_top=%08x, stack end=%08x\r\n", stack_top, stack_bottom);
	BK_DUMP_OUT("\r\n");
}


/* The stack is grow from bottom to top
 *
 *   | . | <- bottom, (big address) (x8 fp)
 *   | . |
 *   | . |
 *   |   | <- top (x2 sp)
 *   |   |
 *   |   |
 *   |   | <- minimum_addr = (bottom - stack_size)
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

		if (code_addr_is_valid(pc)) {
			if (pc & 1)
				pc = pc - 1;

			call_stack_buf[call_stack_index] = pc;
			call_stack_index++;
		}
	}

	if (call_stack_index > 0) {
		int index;

		BK_DUMP_OUT("%-16s   [0x%-6x ~ 0x%-6x]   0x%-6x   %-4d   %-8d   ",
				  str_type, stack_minimum, stack_bottom, init_stack_top, stack_size, init_stack_top < stack_minimum);

		for (index = 0; index < call_stack_index; index++)
			BK_DUMP_OUT("%lx ", call_stack_buf[index]);
		BK_DUMP_OUT("\r\n");
	} else if (init_stack_top < stack_minimum) {
		BK_DUMP_OUT("%-16s   [0x%-6x ~ 0x%-6x]   0x%-6x   %-4d   %-8d   ",
				  str_type, stack_minimum, stack_bottom, init_stack_top, stack_size, init_stack_top < stack_minimum);
	}
}


