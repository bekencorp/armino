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
#include <common/bk_compiler.h>
#include <os/mem.h>
#include "printf_impl.h"
#if CONFIG_SHELL_ASYNCLOG
#include "bk_api_cli.h"
#endif

#ifndef CONFIG_PRINTF_BUF_SIZE
#define CONFIG_PRINTF_BUF_SIZE    (136)
#endif

static uint8_t s_printf_enable = 1;
#if CONFIG_SHELL_ASYNCLOG
static volatile uint8_t s_printf_sync = 0;

typedef  struct
{
	char	mod_name[11];
	u8		disabled;
} __bk_packed mod_disable_list_t;

static mod_disable_list_t mod_tag_list[6];

static u8 whitelist_enabled = 0;

#endif

int printf_lock_init(void)
{
#if CONFIG_SHELL_ASYNCLOG
	memset(&mod_tag_list[0], 0, sizeof(mod_tag_list));
	shell_set_log_level(LOG_LEVEL);
#endif

	return BK_OK;
}

int printf_lock_deinit(void)
{
	return BK_OK;
}

static void bk_printf_sync(const char *fmt, va_list args)
{
	if(!printf_is_init())
		return;

	char string[CONFIG_PRINTF_BUF_SIZE];

	strcpy(string, "[SYNC]:");

	vsnprintf(&string[7], sizeof(string) - 8, fmt, args);
	string[CONFIG_PRINTF_BUF_SIZE - 1] = 0;
	uart_write_string(bk_get_printf_port(), string);
}

static void bk_printf_raw_sync(const char *fmt, va_list args)
{
	if(!printf_is_init())
		return;

	char string[CONFIG_PRINTF_BUF_SIZE];

	vsnprintf(&string[0], sizeof(string), fmt, args);
	string[CONFIG_PRINTF_BUF_SIZE - 1] = 0;
	uart_write_string(bk_get_printf_port(), string);
}

static const char * level_format[] = 
{
	"%s%s",
	"%s%s:E(%d):",
	"%s%s:W(%d):",
	"%s%s:I(%d):",
	"%s%s:D(%d):",
	"%s%s:V(%d):",
};

#include <os/str.h>

#define CPU_STR_LEN		6

static void bk_printf_port_ext(int level, char *tag, const char *fmt, va_list args)
{
#if CONFIG_SHELL_ASYNCLOG

#define MAX_TAG_LEN		8
#define MAX_INFO_LEN	16

	if(s_printf_sync == 0)
	{
		char prefix_str[CPU_STR_LEN + MAX_TAG_LEN + MAX_INFO_LEN + 1];
		char tag_str[MAX_TAG_LEN + 1];
		char cpu_str[CPU_STR_LEN + 1];

		int     fmt_idx = level;

		if(tag != NULL)
		{
			strncpy(tag_str, tag, MAX_TAG_LEN);
			tag_str[MAX_TAG_LEN] = 0;
		}
		else
		{
			tag_str[0] = 0;
			fmt_idx = 0;
		}

		int  cpu_id = shell_get_cpu_id();

		if(cpu_id >= 0)
		{
			os_snprintf(cpu_str, sizeof(cpu_str), "cpu%d:", cpu_id);
		}
		else
		{
			cpu_str[0] = 0;
		}

		os_snprintf(prefix_str, sizeof(prefix_str), level_format[fmt_idx], cpu_str, tag_str, rtos_get_time());

		shell_log_out_port(level, prefix_str, fmt, args);
	}
	else
	{
		bk_printf_sync(fmt, args);
	}

#else
	bk_printf_sync(fmt, args);
#endif // #if CONFIG_SHELL_ASYNCLOG
}

static void bk_printf_raw_port(int level, char *tag, const char *fmt, va_list args)
{
#if CONFIG_SHELL_ASYNCLOG

	if(s_printf_sync == 0)
	{
		shell_log_out_port(level, NULL, fmt, args);
	}
	else
	{
		bk_printf_raw_sync(fmt, args);
	}

#else
	bk_printf_raw_sync(fmt, args);
#endif // #if CONFIG_SHELL_ASYNCLOG
}

static void bk_printf_port(int level, char *tag, const char *fmt, va_list args)
{
#if CONFIG_SHELL_ASYNCLOG

	if(s_printf_sync == 0)
	{
		char cpu_str[CPU_STR_LEN + 1];
		int  cpu_id = shell_get_cpu_id();

		if(cpu_id >= 0)
		{
			os_snprintf(cpu_str, sizeof(cpu_str), "cpu%d:", cpu_id);
		}
		else
		{
			cpu_str[0] = 0;
		}

		shell_log_out_port(level, cpu_str, fmt, args);
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

	int    level = BK_LOG_INFO;

	if( !shell_level_check_valid(level) )  /* check here instead of in shell_log_out to reduce API instructions. */
		return;

	va_start(args, fmt);

	bk_printf_port_ext(level, NULL, fmt, args);

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

void bk_enable_white_list(int enabled)
{
	if(enabled)
		whitelist_enabled = 1;
	else
		whitelist_enabled = 0;
}

int bk_white_list_state(void)
{
	return whitelist_enabled;
}

#endif // #if CONFIG_SHELL_ASYNCLOG

void bk_buf_printf_sync(char *buf, int buf_len)
{
	return;
}

/*  ========= !! NOTE !! =========  */
/*          Obsoleted  API          */
/* use bk_printf_ext(...) instead.  */
void bk_printf_ex(int level, char *tag, const char *fmt, ...)   /* Obsoleted  API */
{
#if CONFIG_SHELL_ASYNCLOG

	va_list args;

	if(!printf_is_init())
		return;

	if( !shell_level_check_valid(level) )  /* check here instead of in shell_log_out to reduce API instructions. */
		return;

	if(bk_mod_printf_disbled(tag) ^ whitelist_enabled)
		return;

	va_start(args, fmt);

	bk_printf_port(level, tag, fmt, args);

	va_end(args);

#endif
	return;
}

void bk_printf_ext(int level, char *tag, const char *fmt, ...)
{
#if CONFIG_SHELL_ASYNCLOG

	va_list args;

	if(!printf_is_init())
		return;

	if( !shell_level_check_valid(level) )  /* check here instead of in shell_log_out to reduce API instructions. */
		return;

	if(bk_mod_printf_disbled(tag) ^ whitelist_enabled)
		return;

	va_start(args, fmt);

	bk_printf_port_ext(level, tag, fmt, args);

	va_end(args);

#endif
	return;
}

void bk_printf_raw(int level, char *tag, const char *fmt, ...)
{
#if CONFIG_SHELL_ASYNCLOG

	va_list args;

	if(!printf_is_init())
		return;

	if( !shell_level_check_valid(level) )  /* check here instead of in shell_log_out to reduce API instructions. */
		return;

	if(bk_mod_printf_disbled(tag) ^ whitelist_enabled)
		return;

	va_start(args, fmt);

	bk_printf_raw_port(level, tag, fmt, args);

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

	disable = disable ^ whitelist_enabled;

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

