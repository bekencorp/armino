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
#include <stdlib.h>

#include <common/bk_include.h>
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "bk_uart.h"
#include <os/mem.h>
#include "bk_arch.h"
#include <os/os.h>
#include "rtos_impl.h"
#include <components/log.h>
#include "bk_api_cli.h"
#include <common/bk_assert.h>

#define TAG "os"

uint32_t platform_cpsr_content(void);
uint32_t platform_sp_content(void);

extern void trap_entry(void);

uint32_t stack_get_stack_top(uint32_t mode)
{
	return 0;
}

uint32_t stack_check_free_space(const uint8_t *pucStackByte, uint8_t fill)
{
	uint32_t ulCount = 0U;

	while (*pucStackByte == (uint8_t) fill) {
		pucStackByte += 1;
		ulCount ++;
	}

	return ulCount;
}

void stack_thread_show(uint32_t *total_cnt, uint32_t *used_cnt)
{
	uint32_t all, used;
	UBaseType_t uxThdCnt, i;
	TaskStatus_t *pxTaskStatusArray, *pxTaskItem;
	unsigned portBASE_TYPE uxCurrentNumberOfTasks = uxTaskGetNumberOfTasks();

	BK_DUMP_OUT("%-16s   %-10s   %-22s   %-10s   %-13s   %-11s\r\n", "task", "stack_size", "address", "peak_used", "current_used", "available");

	pxTaskStatusArray = (TaskStatus_t *)os_malloc(uxCurrentNumberOfTasks * sizeof(TaskStatus_t));
	if (NULL == pxTaskStatusArray)
		goto tshow_exit;

	uxThdCnt = uxTaskGetSystemState(pxTaskStatusArray, uxCurrentNumberOfTasks, NULL);
	all = 0;
	used = 0;
	for (i = 0; i < uxThdCnt; i ++) {
		pxTaskItem = &pxTaskStatusArray[i];
		all += (uint32_t)pxTaskItem->uxStackSize;
		used += (uint32_t)pxTaskItem->ptrTopOfStack - (uint32_t)pxTaskItem->pxStackBase;

		BK_DUMP_OUT("%-16s   %-10d   0x%-08x- 0x%-08x   0x%-8x   0x%-11x   0x%-9x\r\n",
				  pxTaskItem->pcTaskName,
				  pxTaskItem->uxStackSize, pxTaskItem->pxStackBase,
				  (StackType_t *)((StackType_t)pxTaskItem->pxStackBase + pxTaskItem->uxStackSize),
				  pxTaskItem->uxStackSize - pxTaskItem->usStackHighWaterMark,
				  ((StackType_t)pxTaskItem->pxStackBase + pxTaskItem->uxStackSize) - (StackType_t)pxTaskItem->ptrTopOfStack,
				  pxTaskItem->usStackHighWaterMark);
	}
	*total_cnt = all;
	*used_cnt = used;

tshow_exit:
	return;
}

#define MODE_STACK_FILL_BYTE 0x5A
#define ARM_MODE_UNUSED ARM_MODE_ABT
#define STACK_ARM_MOD_SHOW(all, cnt, MOD)
#if 0 //temp modify
#define STACK_ARM_MOD_SHOW(all, cnt, MOD) do{\
		cnt = stack_check_free_space((uint8_t *)boot_stack_base_##MOD, MODE_STACK_FILL_BYTE);\
		cnt = MIN(cnt, boot_stack_len_##MOD);\
		posi_stack = stack_get_stack_top(ARM_MODE_##MOD);\
		all += boot_stack_len_##MOD;\
		\
		if(posi_stack)\
		{\
			used += boot_stack_base_##MOD + boot_stack_len_##MOD - posi_stack;\
			BK_DUMP_OUT("%-16s   %-10d   0x%-08x, 0x%-08x   %-10x   0x%-11x   0x%-9x\r\n",\
					  #MOD, boot_stack_len_##MOD,\
					  boot_stack_base_##MOD, boot_stack_base_##MOD + boot_stack_len_##MOD,\
					  boot_stack_len_##MOD - cnt, boot_stack_base_##MOD + boot_stack_len_##MOD - posi_stack,\
					  posi_stack - boot_stack_base_##MOD);\
		}\
		else\
		{\
			posi_stack = boot_stack_base_##MOD + cnt;\
			used += boot_stack_base_##MOD + boot_stack_len_##MOD - posi_stack;\
			BK_DUMP_OUT("%-16s   %-10d   0x%-08x- 0x%-08x   0x%-8x   ?0x%-10x   ?0x%-8x\r\n",\
					  #MOD, boot_stack_len_##MOD,\
					  boot_stack_base_##MOD, boot_stack_base_##MOD + boot_stack_len_##MOD,\
					  boot_stack_len_##MOD - cnt, boot_stack_base_##MOD + boot_stack_len_##MOD - posi_stack,\
					  posi_stack - boot_stack_base_##MOD);\
		}\
	}while(0)
#endif
void stack_arm_mode_show(uint32_t *total_cnt, uint32_t *used_cnt)
{
#if 0//temp modify
	extern uint32_t boot_stack_base_UNUSED;
	extern uint32_t boot_stack_len_UNUSED;
	extern uint32_t boot_stack_base_IRQ;
	extern uint32_t boot_stack_len_IRQ;
	extern uint32_t boot_stack_base_SVC;
	extern uint32_t boot_stack_len_SVC;
	extern uint32_t boot_stack_base_FIQ;
	extern uint32_t boot_stack_len_FIQ;
	extern uint32_t boot_stack_base_SYS;
	extern uint32_t boot_stack_len_SYS;

	uint32_t all, used, cnt, posi_stack;
	all = 0;
	used = 0;

	BK_DUMP_OUT("%-16s   %-10s   %-22s   %-10s   %-13s   %-11s\r\n",
		"arm_mode", "stack_size", "address", "peak_used", "current_used", "available");
	STACK_ARM_MOD_SHOW(all, cnt, SVC);
	STACK_ARM_MOD_SHOW(all, cnt, IRQ);
	STACK_ARM_MOD_SHOW(all, cnt, FIQ);
	STACK_ARM_MOD_SHOW(all, cnt, SYS);
	STACK_ARM_MOD_SHOW(all, cnt, UNUSED);

	*total_cnt = all;
	*used_cnt = used;
#endif	
}

void rtos_dump_stack_memory_usage(void)
{
	uint32_t total1 = 0, total2 = 0;
	uint32_t used1 = 0, used2 = 0;

	stack_thread_show(&total1, &used1);
	BK_DUMP_OUT("thread_stack:%, used:%d, the rest:%d\r\n\r\n", total1, used1, total1 - used1);
	stack_arm_mode_show(&total2, &used2);
	BK_DUMP_OUT("\r\narm_mode_stack:%, used:%d, the rest:%d\r\n", total2, used2, total2 - used2);
	BK_DUMP_OUT("total_stack_space:%, used:%d, the rest:%d\r\n", total1 + total2, used1 + used2,
			  total1 + total2 - (used1 + used2));
}

void rtos_dump_task_backtrace(beken_thread_t *task)
{
#if configBK_FREERTOS
	beken_thread_t t = NULL;
	uint32_t stack_bottom = 0;
	uint32_t stack_top = 0;
	uint32_t stack_size = 0;

	if (!task || !(*task)) {
		t = (beken_thread_t)xTaskGetCurrentTaskHandle();
	} else {
		t = *task;
	}

	xTaskGetStackInfo(t, &stack_top, &stack_bottom, &stack_size);
	arch_parse_stack_backtrace(pcTaskGetName(t), stack_top, stack_bottom, stack_size, true);
#endif
}


void rtos_dump_backtrace(void)
{
	uint32_t int_level = rtos_disable_int();
	
	BK_DUMP_OUT(">>>>dump task backtrace begin.\r\n");
	BK_DUMP_OUT("%-16s   %-21s   %-8s   %-4s   %-8s   %-s\r\n",
		"task", "stack_addr", "top", "size", "overflow", "backtrace");
	vTaskDumpAllThreadStack();
	BK_DUMP_OUT("<<<<dump task backtrace end.\r\n");
	BK_DUMP_OUT("\r\n");
	
	rtos_enable_int(int_level);
}

void rtos_dump_task_runtime_stats(void)
{
#if ((!CONFIG_SLAVE_CORE) && ( configGENERATE_RUN_TIME_STATS == 1 ) && ( configUSE_STATS_FORMATTING_FUNCTIONS > 0 ) )
	int num_of_tasks = 0;
	int buf_len = 0, info_len = 0;
	char *buf = NULL;
	uint32_t int_level = rtos_disable_int();

	BK_DUMP_OUT(">>>>dump task runtime begin.\r\n");
	//TODO optimize it
	//malloc a big enough memory
	num_of_tasks = uxTaskGetNumberOfTasks();
	buf_len = (num_of_tasks + 5) * 100;
	buf = (char*)os_malloc(buf_len);
	if (!buf) {
		BK_DUMP_OUT("dump runtime status oom.\r\n");
		return;
	}

	vTaskGetRunTimeStats(buf);
	
	buf[buf_len - 1] = '\0';
	info_len = strlen(buf);
	BK_DUMP_RAW_OUT(buf, info_len);
	
	os_free(buf);

	BK_DUMP_OUT("<<<<dump task runtime end.\r\n");
	BK_DUMP_OUT("\r\n");
	rtos_enable_int(int_level);
#else
	BK_DUMP_OUT("dump runtime stats not supported!\r\n");
#endif
}

void rtos_dump_task_list(void)
{
#if (!CONFIG_SLAVE_CORE)
	int num_of_tasks = 0;
	int buf_len = 0, info_len = 0;
	char *buf = NULL;

	BK_DUMP_OUT(">>>>dump task list begin.\r\n");
	num_of_tasks = uxTaskGetNumberOfTasks();
	if(num_of_tasks == 0) {
		BK_DUMP_OUT("no task running.\r\n");
		return;
	}
	buf_len = (num_of_tasks + 5) * 100;
	buf = (char*)os_malloc(buf_len);
	if (!buf) {
		BK_DUMP_OUT("dump task list oom.\r\n");
		return;
	}
	BK_DUMP_OUT("%-12s   %-5s   %-5s   %-5s   %-5s\r\n",
		"task", "state", "pri", "water", "no");
	vTaskList(buf);

	buf[buf_len - 1] = '\0';
	info_len = strlen(buf);
	BK_DUMP_RAW_OUT(buf, info_len);

	os_free(buf);

	BK_DUMP_OUT("<<<<dump task list end.\r\n");
	BK_DUMP_OUT("\r\n");
#endif
}

void rtos_stack_overflow(char *taskname)
{
	BK_DUMP_OUT("stack overflow: %s\r\n", taskname);
	BK_ASSERT(false);
	while(1);
}

