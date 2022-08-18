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

#include <stdarg.h>
#include <string.h>
#include <driver/uart.h>
#include "bk_uart.h"
#include <common/sys_config.h>
#include <os/mem.h>
#include "printf_impl.h"
#if CONFIG_SHELL_ASYNCLOG
#include "bk_api_cli.h"
#endif

#if (CONFIG_SHELL_ASYNCLOG && CONFIG_SLAVE_CORE)
#define CPU1_TAG "cpu1"
#endif

#ifndef CONFIG_PRINTF_BUF_SIZE
#define CONFIG_PRINTF_BUF_SIZE    (128)
#endif

static char s_exception_mode_printf_buf[CONFIG_PRINTF_BUF_SIZE] = {0};
// static char s_task_mode_printf_buf[CONFIG_PRINTF_BUF_SIZE] = {0};

static uint8_t s_printf_enable = 1;
#if CONFIG_SHELL_ASYNCLOG
static volatile uint8_t s_printf_sync = 0;

typedef  struct
{
	char	mod_name[15];
	u8		disabled;
} __packed mod_disable_list_t;

static mod_disable_list_t mod_tag_list[20];

#endif

static beken_mutex_t s_printf_lock = NULL;

void printf_lock(void)
{
    rtos_lock_mutex(&s_printf_lock);
}

void printf_unlock(void)
{
    rtos_unlock_mutex(&s_printf_lock);
}

int printf_lock_init(void)
{
	int ret = rtos_init_mutex(&s_printf_lock);
	if (kNoErr != ret) {
		return BK_ERR_NO_MEM;
	}

#if CONFIG_SHELL_ASYNCLOG
	memset(&mod_tag_list[0], 0, sizeof(mod_tag_list));
	shell_set_log_level(LOG_LEVEL);
#endif

	return BK_OK;
}

int printf_lock_deinit(void)
{
	if (s_printf_lock)
		rtos_deinit_mutex(&s_printf_lock);

	s_printf_lock = NULL;
	return BK_OK;
}

static void exception_mode_printf(const char *fmt, va_list ap)
{
	vsnprintf(s_exception_mode_printf_buf, sizeof(s_exception_mode_printf_buf) - 1, fmt, ap);
	s_exception_mode_printf_buf[CONFIG_PRINTF_BUF_SIZE - 1] = 0;
	uart_write_string(bk_get_printf_port(), s_exception_mode_printf_buf);
}

#if (!CONFIG_ARCH_RISCV)
static void irq_printf(const char *fmt, va_list ap)
{
	char string[CONFIG_PRINTF_BUF_SIZE];

	vsnprintf(string, sizeof(string) - 1, fmt, ap);
	string[CONFIG_PRINTF_BUF_SIZE - 1] = 0;
	uart_write_string(bk_get_printf_port(), string);
}
#endif


static void task_printf(const char *fmt, va_list ap)
{
	char string[CONFIG_PRINTF_BUF_SIZE];

	vsnprintf(string, sizeof(string) - 1, fmt, ap);
	string[CONFIG_PRINTF_BUF_SIZE - 1] = 0;
	uart_write_string(bk_get_printf_port(), string);

}


static void bk_printf_sync(const char *fmt, va_list args)
{
	if(!printf_is_init())
		return;

#if (CONFIG_ARCH_RISCV)

	if (rtos_is_in_interrupt_context() || (!rtos_is_scheduler_started()))
		exception_mode_printf(fmt, args);
	else
		task_printf(fmt, args);

#else // #if CONFIG_ARCH_RISCV

	uint32_t cpsr_val = rtos_get_cpsr();
	uint32_t arm_mode = cpsr_val & /*ARM968_MODE_MASK*/0x1f;

	if ((/*ARM_MODE_FIQ*/17 == arm_mode)
		|| (/*ARM_MODE_ABT*/23 == arm_mode)
		|| (/*ARM_MODE_UND*/27 == arm_mode)
		|| (!rtos_is_scheduler_started()))
		exception_mode_printf(fmt, args);
	else if (/*ARM_MODE_IRQ*/18 == arm_mode)
		irq_printf(fmt, args);
	else
		task_printf(fmt, args);

#endif // #if CONFIG_ARCH_RISCV
}

void bk_printf_port(int level, char *tag, const char *fmt, va_list args)
{
	if (!rtos_is_scheduler_started()) {
		exception_mode_printf(fmt, args);
		return;
	}

#if CONFIG_SHELL_ASYNCLOG

	if(s_printf_sync == 0)
	{
	#if (CONFIG_SLAVE_CORE)
		shell_log_out_port(level, CPU1_TAG, fmt, args);
	#else
		shell_log_out_port(level, NULL, fmt, args);
	#endif
	}
	else
	{
		bk_printf_sync(fmt, args);
	}

#else
	bk_printf_sync(fmt, args);
#endif // #if CONFIG_SHELL_ASYNCLOG
}

void bk_printf(const char *fmt, ...)
{
	va_list args;

	if(!printf_is_init())
		return;

	if(!s_printf_enable)
	    return;

	va_start(args, fmt);

	bk_printf_port(BK_LOG_ERROR, NULL, fmt, args);

	va_end(args);

	return;
}

#if CONFIG_SHELL_ASYNCLOG
static int bk_mod_printf_disbled(char * tag)
{
	int		i, result;

	for(i = 0; i < ARRAY_SIZE(mod_tag_list); i++)
	{
		if(mod_tag_list[i].disabled)
		{
			result = strncmp(mod_tag_list[i].mod_name, tag, sizeof(mod_tag_list[i].mod_name) - 1);

			if(result == 0)
				return 1;
		}
	}

	return 0;
}
#endif // #if CONFIG_SHELL_ASYNCLOG

void bk_buf_printf_sync(char *buf, int buf_len)
{
	if (!printf_is_init())
		return;

	if (!s_printf_enable) {
	    return;
	}

	if (NULL == buf || buf_len <= 0)
		return;

	buf[buf_len -1] = '\0';

	uart_write_string(bk_get_printf_port(), buf);
}



void bk_printf_ex(int level, char *tag, const char *fmt, ...)
{
#if CONFIG_SHELL_ASYNCLOG

	va_list args;

	if(!printf_is_init())
		return;

	if(level > shell_get_log_level())  /* check here instead of in shell_log_out to reduce API instructions. */
		return;

	if(bk_mod_printf_disbled(tag))
		return;

	va_start(args, fmt);

	bk_printf_port(level, tag, fmt, args);

	va_end(args);

#endif
	return;
}

void bk_set_printf_enable(uint8_t enable)
{
#if CONFIG_SHELL_ASYNCLOG
	if(0 == enable) {
		shell_echo_set(0);
		shell_set_log_level(0);
	} else {
		shell_echo_set(1);
		shell_set_log_level(LOG_LEVEL);
	}
#endif
	s_printf_enable = enable;
}

void bk_set_printf_sync(uint8_t enable)
{
#if CONFIG_SHELL_ASYNCLOG
#if CONFIG_SLAVE_CORE
	s_printf_sync = 0;
#else
	s_printf_sync = enable;
#endif
#endif
}

int bk_get_printf_sync(void)
{
#if CONFIG_SHELL_ASYNCLOG
	return s_printf_sync;
#endif

	return 1;
}

void bk_disable_mod_printf(char *mod_name, uint8_t disable)
{
#if CONFIG_SHELL_ASYNCLOG

	int		i, j, result;

	if(disable == 0)
	{
		for(i = 0; i < ARRAY_SIZE(mod_tag_list); i++)
		{
			if(mod_tag_list[i].disabled)
			{
				result = strncmp(mod_tag_list[i].mod_name, mod_name, sizeof(mod_tag_list[i].mod_name) - 1);

				if(result == 0)
				{
					mod_tag_list[i].disabled = 0;
				}
			}
		}
	}
	else
	{
		j = ARRAY_SIZE(mod_tag_list);

		for(i = 0; i < ARRAY_SIZE(mod_tag_list); i++)
		{
			if(mod_tag_list[i].disabled)
			{
				result = strncmp(mod_tag_list[i].mod_name, mod_name, sizeof(mod_tag_list[i].mod_name) - 1);

				if(result == 0)
				{
					return;
				}
			}
			else
				j = i;
		}

		if(j >= ARRAY_SIZE(mod_tag_list)) /* no free slot to record this module name. */
			return;
		else
		{
			strncpy(mod_tag_list[j].mod_name, mod_name, sizeof(mod_tag_list[j].mod_name) - 1);
			mod_tag_list[j].mod_name[sizeof(mod_tag_list[j].mod_name) - 1] = 0;
			mod_tag_list[j].disabled = 1;
		}
	}

	return;

#endif
}

char * bk_get_disable_mod(int * idx)
{
#if CONFIG_SHELL_ASYNCLOG

	int 	i;

	for(i = *idx; i < ARRAY_SIZE(mod_tag_list); i++)
	{
		(*idx)++;
		if(mod_tag_list[i].disabled)
		{
			return &mod_tag_list[i].mod_name[0];
		}
	}

	return NULL;

#endif

	return NULL;
}

