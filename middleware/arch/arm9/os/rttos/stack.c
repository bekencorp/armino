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

#include <rtthread.h>
#include <string.h>
#include <stdint.h>
#include "boot.h"
#include "arm.h"
#include "bk_arch.h"
#include "stack_base.h"

/**
 * This function will initialize thread stack
 *
 * @param tentry the entry of thread
 * @param parameter the parameter of entry
 * @param stack_addr the beginning stack address
 * @param texit the function will be called when thread exit
 *
 * @return stack address
 */
uint8_t *rt_hw_stack_init(void *tentry, void *parameter,
                             uint8_t *stack_addr, void *texit)
{
    uint32_t *stk;

    //stk      = (uint32_t*)stack_addr;
    stack_addr += sizeof(uint32_t);
    stack_addr  = (uint8_t *)RT_ALIGN_DOWN((uint32_t)stack_addr, 8);
    stk  = (uint32_t *)stack_addr;

    *(--stk) = (uint32_t)tentry;         /* entry point */
    *(--stk) = (uint32_t)texit;          /* lr */
    *(--stk) = 0xdeadbeef;                  /* r12 */
    *(--stk) = 0xdeadbeef;                  /* r11 */
    *(--stk) = 0xdeadbeef;                  /* r10 */
    *(--stk) = 0xdeadbeef;                  /* r9 */
    *(--stk) = 0xdeadbeef;                  /* r8 */
    *(--stk) = 0xdeadbeef;                  /* r7 */
    *(--stk) = 0xdeadbeef;                  /* r6 */
    *(--stk) = 0xdeadbeef;                  /* r5 */
    *(--stk) = 0xdeadbeef;                  /* r4 */
    *(--stk) = 0xdeadbeef;                  /* r3 */
    *(--stk) = 0xdeadbeef;                  /* r2 */
    *(--stk) = 0xdeadbeef;                  /* r1 */
    *(--stk) = (uint32_t)parameter;      /* r0 : argument */
	/* cpsr */
	if ((uint32_t)tentry & 0x01)
		*(--stk) = BOOT_ARM_MODE_SVC | 0x20;			/* thumb mode */
	else
		*(--stk) = BOOT_ARM_MODE_SVC;		/* arm mode   */

    /* return task's current stack address */
    return (uint8_t *)stk;
}

static void rt_hw_thread_stack_print(rt_thread_t thread, rt_bool_t in_exception)
{
	uint32_t stack_bottom;
	uint32_t sp;

	if (NULL == thread) {
		thread = rt_thread_self();
	}

	stack_bottom = thread->stack_size + (uint32_t)thread ->stack_addr;

	if (in_exception) {
		sp = *(uint32_t*)(MCU_REG_BACKUP_SP_SVC);
	} else {
		sp = (uint32_t)thread->sp;
	}

	arch_parse_stack_backtrace(thread->name, sp, stack_bottom, thread->stack_size, true);
}

void rt_hw_stack_print(rt_thread_t thread)
{
	rt_hw_thread_stack_print(thread, false);
}

void rt_stack_print(int argc, char **argv)
{
    struct rt_object_information *info;
    struct rt_thread *thread;
    struct rt_list_node *node;
    size_t length;

    if (argc < 2)
    {
        rt_kprintf("usage: stack task_name|all_tasks\n");
        rt_hw_stack_print(NULL);
        return;
    }

    info = rt_object_get_information(RT_Object_Class_Thread);
    if (NULL == info)
    {
        rt_kprintf("can't find thread list\n");
        return;
    }

    length = strlen((const char *)argv[1]);
    if (length <= 0)
    {
        rt_kprintf("bad thread name\n");
        return;
    }
    if ((length == strlen("all_tasks")) && (0 == strncmp(argv[1], "all_tasks", length)))
    {
        for (node = info->object_list.next; node != &info->object_list; node = node->next)
        {
            thread = rt_list_entry(node, struct rt_thread, list);
            rt_hw_stack_print(thread);
        }
        return;
    }
    if (length > RT_NAME_MAX)
    {
        length = RT_NAME_MAX;
    }

    for (node = info->object_list.next; node != &info->object_list; node = node->next)
    {
        thread = rt_list_entry(node, struct rt_thread, list);
        if (0 != strncmp(argv[1], thread->name, length))
        {
            continue;
        }
        rt_hw_stack_print(thread);
        break;
    }
}

MSH_CMD_EXPORT_ALIAS(rt_stack_print, stack, rt_hw_stack_print);
