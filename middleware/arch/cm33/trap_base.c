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
#include "sdkconfig.h"
#include "reset_reason.h"
#include <os/os.h>
#include "bk_arch.h"
#include "bk_rtos_debug.h"
#include <components/system.h>
#include <os/mem.h>
#include <components/log.h>
#include <common/bk_assert.h>
#include "arch_interrupt.h"
#include "stack_base.h"
#include "wdt_driver.h"


#define MAX_DUMP_SYS_MEM_COUNT       (8)

typedef struct sys_mem_info
{
    uint32_t mem_base_addr;
    uint32_t mem_size;
} sys_mem_info_t;

static unsigned int s_mem_count = 0;
static sys_mem_info_t s_dump_sys_mem_info[MAX_DUMP_SYS_MEM_COUNT] = {0};

static hook_func s_wifi_dump_func = NULL;
static hook_func s_ble_dump_func = NULL;

volatile unsigned int g_enter_exception = 0;
volatile unsigned int g_enter_nmi_vector = 0;

extern unsigned char __dtcm_start__;
extern unsigned char __dtcm_end__;

#if !CONFIG_SLAVE_CORE
extern unsigned char __iram_start__;
extern unsigned char __iram_end__;
#if CONFIG_CACHE_ENABLE
extern unsigned char _nocache_start;
extern unsigned char _nocache_end;
#endif //#if CONFIG_CACHE_ENABLE
#endif //#if !CONFIG_SLAVE_CORE

extern unsigned char __data_start__;
extern unsigned char __data_end__;
extern unsigned char _bss_start;
extern unsigned char _bss_end;
extern unsigned char _sstack;
extern unsigned char _estack;

static void rtos_dump_plat_memory(void) {
    // Dump DTCM
    stack_mem_dump((uint32_t)&__dtcm_start__, (uint32_t)&__dtcm_end__);

#if !CONFIG_SLAVE_CORE
    // Dump IRAM
    stack_mem_dump((uint32_t)&__iram_start__, (uint32_t)&__iram_end__);
#if CONFIG_CACHE_ENABLE
    stack_mem_dump((uint32_t)&_nocache_start, (uint32_t)&_nocache_end);
#endif //#if CONFIG_CACHE_ENABLE
#endif //#if !CONFIG_SLAVE_CORE

    // Dump data bss stack
    stack_mem_dump((uint32_t)&__data_start__, (uint32_t)&_bss_end);

    // Dump data bss stack
    stack_mem_dump((uint32_t)&_sstack, (uint32_t)&_estack);
}


unsigned int arch_is_enter_exception(void) {
    return g_enter_exception;
}

void rtos_regist_wifi_dump_hook(hook_func wifi_func)
{
    s_wifi_dump_func = wifi_func;
}

void rtos_regist_ble_dump_hook(hook_func ble_func)
{
    s_ble_dump_func = ble_func;
}

void rtos_regist_plat_dump_hook(uint32_t mem_base_addr, uint32_t mem_size)
{
    if (s_mem_count < MAX_DUMP_SYS_MEM_COUNT) {
        s_dump_sys_mem_info[s_mem_count].mem_base_addr = mem_base_addr;
        s_dump_sys_mem_info[s_mem_count].mem_size = mem_size;
        s_mem_count++;
    } else {
        BK_DUMP_OUT("rtos_regist_plat_dump_hook failed:s_mem_count(%d).\r\n", s_mem_count);
    }
}

void rtos_dump_plat_sys_mems(void) {
#if CONFIG_MEMDUMP_ALL
    rtos_dump_plat_memory();

    for (int i = 0; i < s_mem_count; i++) {
        uint32_t begin = s_dump_sys_mem_info[i].mem_base_addr;
        uint32_t end = begin + s_dump_sys_mem_info[i].mem_size;
        stack_mem_dump(begin, end);
    }
#endif
}

/**
 * this function will show registers of CPU
 *
 * @param mcause
 * @param context
 */
void arch_dump_cpu_registers(uint32_t mcause, SAVED_CONTEXT *context) {

    BK_DUMP_OUT("Current regs:\r\n");

    BK_DUMP_OUT("0 r0 x 0x%lx\r\n", context->r0);
    BK_DUMP_OUT("1 r1 x 0x%lx\r\n", context->r1);
    BK_DUMP_OUT("2 r2 x 0x%lx\r\n", context->r2);
    BK_DUMP_OUT("3 r3 x 0x%lx\r\n", context->r3);
    BK_DUMP_OUT("4 r4 x 0x%lx\r\n", context->r4);
    BK_DUMP_OUT("5 r5 x 0x%lx\r\n", context->r5);
    BK_DUMP_OUT("6 r6 x 0x%lx\r\n", context->r6);
    BK_DUMP_OUT("7 r7 x 0x%lx\r\n", context->r7);
    BK_DUMP_OUT("8 r8 x 0x%lx\r\n", context->r8);
    BK_DUMP_OUT("9 r9 x 0x%lx\r\n", context->r9);
    BK_DUMP_OUT("10 r10 x 0x%lx\r\n", context->r10);
    BK_DUMP_OUT("11 r11 x 0x%lx\r\n", context->r11);
    BK_DUMP_OUT("12 r12 x 0x%lx\r\n", context->r12);
    BK_DUMP_OUT("14 sp x 0x%lx\r\n", context->sp);
    BK_DUMP_OUT("15 lr x 0x%lx\r\n", context->lr);
    BK_DUMP_OUT("16 pc x 0x%lx\r\n", context->pc);
    BK_DUMP_OUT("17 xpsr x 0x%lx\r\n", context->xpsr);
    BK_DUMP_OUT("18 msp x 0x%lx\r\n", context->msp);
    BK_DUMP_OUT("19 psp x 0x%lx\r\n", context->psp);
    BK_DUMP_OUT("20 primask x 0x%lx\r\n", context->primask);
    BK_DUMP_OUT("21 basepri x 0x%lx\r\n", context->basepri);
    BK_DUMP_OUT("22 faultmask x 0x%lx\r\n", context->faultmask);
    BK_DUMP_OUT("23 control x 0x%lx\r\n", context->control);
    BK_DUMP_OUT("24 fpscr x 0x%lx\r\n", context->fpscr);

}

void rtos_dump_system(void)
{
#if CONFIG_DEBUG_FIRMWARE
    BK_LOG_FLUSH();
    bk_set_printf_sync(true);

    BK_DUMP_OUT("***********************************************************************************************\r\n");
    BK_DUMP_OUT("***********************************user except handler begin***********************************\r\n");
    BK_DUMP_OUT("***********************************************************************************************\r\n");

#if CONFIG_INT_WDT
    bk_wdt_feed();
#endif

    if(NULL != s_wifi_dump_func) {
        s_wifi_dump_func();
    }

    if(NULL != s_ble_dump_func) {
        s_ble_dump_func();
    }

    rtos_dump_plat_sys_mems();

#if CONFIG_FREERTOS && CONFIG_MEM_DEBUG
    os_dump_memory_stats(0, 0, NULL);
#endif

    rtos_dump_backtrace();
    rtos_dump_task_list();
#if CONFIG_FREERTOS
    rtos_dump_task_runtime_stats();
#endif

    BK_DUMP_OUT("***********************************************************************************************\r\n");
    BK_DUMP_OUT("************************************user except handler end************************************\r\n");
    BK_DUMP_OUT("***********************************************************************************************\r\n");
#endif //CONFIG_DEBUG_FIRMWARE
}

void user_except_handler(void)
{
    if (0 == g_enter_exception) {
        // Make sure the interrupt is disable
        uint32_t int_level = rtos_disable_int();

        /* Handled Trap */
        g_enter_exception = 1;

        rtos_dump_system();
#if !CONFIG_SLAVE_CORE
        bk_reboot_ex(RESET_SOURCE_REBOOT);
#endif
        while(g_enter_exception);

        rtos_enable_int(int_level);
    } else {
#if !CONFIG_SLAVE_CORE
        bk_wdt_force_reboot();
#endif
    }

}

