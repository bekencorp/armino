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

#include <os/os.h>
#include "cli.h"
#include <driver/aon_rtc.h>

static void alarm_auto_test_callback(aon_rtc_id_t id, uint8_t *name_p, void *param);


static void cli_aon_rtc_help(void)
{
	CLI_LOGI("aon_rtc_driver init\r\n");
	CLI_LOGI("aon_rtc_driver deinit\r\n");
#if 0	//remove it, only one HW can't be used for many APPs.
	CLI_LOGI("aon_rtc <id> create <tick> <period>\r\n");
	CLI_LOGI("aon_rtc <id> destroy\r\n");
	CLI_LOGI("aon_rtc_int <id> tick\r\n");
	CLI_LOGI("aon_rtc_int <id> upper\r\n");
#endif
	CLI_LOGI("aon_rtc_register {id} {name} {period_tick} {period_cnt}, {callback}\r\n");
	CLI_LOGI("aon_rtc_unregister {id} {name}\r\n");
	CLI_LOGI("aon_rtc_timing_test {id} {rounds} {cycles} {set tick val}\r\n");
}

#if 0	//remove it, only one HW can't be used for many APPs.
static void cli_aon_rtc_tick_isr(aon_rtc_id_t id, uint8_t *name_p, void *param)
{
	CLI_LOGI("aon_rtc_tick_isr(%d)\n", id);
}

static void cli_aon_rtc_upper_isr(aon_rtc_id_t id, uint8_t *name_p, void *param)
{
	CLI_LOGI("aon_rtc_upper_isr(%d)\n", id);
}
#endif

static void cli_aon_rtc_driver_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_aon_rtc_help();
		return;
	}

	if (os_strcmp(argv[1], "init") == 0) {
		BK_LOG_ON_ERR(bk_aon_rtc_driver_init());
		CLI_LOGI("aon_rtc driver init\n");
	} else if (os_strcmp(argv[1], "deinit") == 0) {
		BK_LOG_ON_ERR(bk_aon_rtc_driver_deinit());
		CLI_LOGI("aon_rtc driver deinit\n");
	} else {
		cli_aon_rtc_help();
		return;
	}
}

#if 0	//remove it, only one HW can't be used for many APPs.
static void cli_aon_rtc_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t aon_rtc_id;
	uint64_t tick;
	uint32_t period;

	aon_rtc_id = os_strtoul(argv[1], NULL, 10);

	if (os_strcmp(argv[2], "create") == 0) {
		tick = os_strtoul(argv[3], NULL, 10);
		period = os_strtoul(argv[4], NULL, 10);
		BK_LOG_ON_ERR(bk_aon_rtc_create(aon_rtc_id, tick, period));
		CLI_LOGI("aon_rtc id:%d bk_aon_rtc_create\n", aon_rtc_id);
	} else 	if (os_strcmp(argv[2], "destroy") == 0) {
		BK_LOG_ON_ERR(bk_aon_rtc_destroy(aon_rtc_id));
		CLI_LOGI("aon_rtc id:%d bk_aon_rtc_destroy\n", aon_rtc_id);
	} else if (os_strcmp(argv[2], "init") == 0) {
		BK_LOG_ON_ERR(bk_aon_rtc_tick_init());
		CLI_LOGI("aon_rtc id:%d bk_aon_rtc_tick_init\n", aon_rtc_id);
	} else if (os_strcmp(argv[2], "deinit") == 0) {
		BK_LOG_ON_ERR(bk_aon_rtc_destroy(aon_rtc_id));
		CLI_LOGI("aon_rtc id:%d bk_aon_rtc_deinit\n", aon_rtc_id);
	} else if (os_strcmp(argv[2], "set") == 0) {
		tick = os_strtoul(argv[3], NULL, 10);
		tick = 32 * 1000 * tick;
		BK_LOG_ON_ERR(bk_aon_rtc_create(0, tick, 1));
		CLI_LOGI("aon_rtc id:%d set rtc period = %d s.\n", aon_rtc_id, tick);
	} else if (os_strcmp(argv[2], "get") == 0) {
		tick = bk_aon_rtc_get_current_tick(aon_rtc_id);
		tick = tick / 32000;
		CLI_LOGI("aon_rtc id:%d get rtc tick time = %d s.\n", aon_rtc_id, tick);
	} else {
		cli_aon_rtc_help();
		return;
	}
}

static void cli_aon_rtc_int_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t aon_rtc_id;

	if (argc != 3) {
		cli_aon_rtc_help();
		return;
	}

	aon_rtc_id = os_strtoul(argv[1], NULL, 10);
	if (os_strcmp(argv[2], "tick") == 0) {
		BK_LOG_ON_ERR(bk_aon_rtc_register_tick_isr(aon_rtc_id, cli_aon_rtc_tick_isr, NULL));
		CLI_LOGI("aon_rtc id:%d register tick interrupt isr\n", aon_rtc_id);
	} else if (os_strcmp(argv[2], "upper") == 0) {
		BK_LOG_ON_ERR(bk_aon_rtc_register_upper_isr(aon_rtc_id, cli_aon_rtc_upper_isr, NULL));
		CLI_LOGI("aon_rtc id:%d register upper interrupt isr\n", aon_rtc_id);
	} else {
		cli_aon_rtc_help();
		return;
	}
}
#endif

static uint8_t alarm_param[12] = {'a', 'l', 'a', 'r', 'm', 'p', 'r', 'm', 0, 0, 0, 0};
static void alarm_callback(aon_rtc_id_t id, uint8_t *name_p, void *param)
{
	CLI_LOGI("id=%d, name=%s %s\r\n", id, name_p, alarm_param);
}

static void cli_aon_rtc_get_time(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t aon_rtc_id;
	uint64_t tick = 0;

	aon_rtc_id = os_strtoul(argv[1], NULL, 10);
	tick = bk_aon_rtc_get_current_tick(aon_rtc_id);

	//CLI_LOGI("id=%d, tick_h=%d tick_l=%d\r\n", aon_rtc_id, (uint32_t)(tick>>32), (uint32_t)tick);
	CLI_LOGI("id=%d, tick_h=%d tick_l=%d ms\r\n", aon_rtc_id, (uint32_t)((tick/32)>>32), (uint32_t)(tick/32));
}

static void cli_aon_rtc_register_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t aon_rtc_id;
	alarm_info_t alarm_info;

	if (argc != 5) {
		cli_aon_rtc_help();
		return;
	}

	aon_rtc_id = os_strtoul(argv[1], NULL, 10);
	strncpy((char *)alarm_info.name, argv[2], ALARM_NAME_MAX_LEN);
	alarm_info.period_tick = os_strtoul(argv[3], NULL, 10);
	alarm_info.period_cnt = os_strtoul(argv[4], NULL, 10);
	alarm_info.param_p = (void *)alarm_param;
	alarm_info.callback = alarm_callback;
	bk_alarm_register(aon_rtc_id, &alarm_info);
}

static void cli_aon_rtc_unregister_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t aon_rtc_id;
	uint8_t name[ALARM_NAME_MAX_LEN+1];

	if (argc != 3) {
		cli_aon_rtc_help();
		return;
	}

	aon_rtc_id = os_strtoul(argv[1], NULL, 10);
	strncpy((char *)name, argv[2], ALARM_NAME_MAX_LEN);

	bk_alarm_unregister(aon_rtc_id, name);
}

static alarm_info_t s_cli_alarm_info[] = 
{
	{"alarm_1", (1000 * AON_RTC_MS_TICK_CNT), 0xFFFFFFFF, alarm_auto_test_callback, NULL},
	{"alarm_2", (6000 * AON_RTC_MS_TICK_CNT), 0xFFFFFFFF, alarm_auto_test_callback, NULL},
	{"alarm_3", (12000 * AON_RTC_MS_TICK_CNT), 0xFFFFFFFF, alarm_auto_test_callback, NULL},
	{"alarm_4", (48000 * AON_RTC_MS_TICK_CNT), 0xFFFFFFFF, alarm_auto_test_callback, NULL},
	{"alarm_5", (3000 * AON_RTC_MS_TICK_CNT), 0xFFFFFFFF, alarm_auto_test_callback, NULL},
	{"alarm_6", (4000 * AON_RTC_MS_TICK_CNT), 0xFFFFFFFF, alarm_auto_test_callback, NULL},
};

static void alarm_auto_test_callback(aon_rtc_id_t id, uint8_t *name_p, void *param)
{
	uint32_t i = 0;
	uint32_t arr_size = sizeof(s_cli_alarm_info)/sizeof(alarm_info_t); 

	CLI_LOGI("id=%d, name=%s\r\n", id, name_p);
	for(i = 0; i < arr_size; i++)
	{
		if(os_strcmp((const char*)s_cli_alarm_info[i].name, (const char*)name_p) == 0)
		{
			//forbid unregister self in the callback
			//CLI_LOGI("Unregister name=%s\r\n", name_p);
			//bk_alarm_unregister(id, s_cli_alarm_info[i].name);

			CLI_LOGI("register name=%s\r\n", s_cli_alarm_info[(i+3)%arr_size].name);
			bk_alarm_register(id, &s_cli_alarm_info[(i+3)%arr_size]);

			break;
		}
	}
}

static void aon_rtc_autotest_start(aon_rtc_id_t id)
{
	uint32_t i = 0;

	for(i = 0; i < sizeof(s_cli_alarm_info)/sizeof(alarm_info_t); i++)
	{
		bk_alarm_register(id, &s_cli_alarm_info[i]);
	}
}

static void aon_rtc_autotest_stop(aon_rtc_id_t id)
{
	uint32_t i = 0;
	for(i = 0; i < sizeof(s_cli_alarm_info)/sizeof(alarm_info_t); i++)
	{
		bk_alarm_unregister(id, s_cli_alarm_info[i].name);
	}
}

static void cli_aon_rtc_auto_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t aon_rtc_id;

	if (argc != 3) {
		cli_aon_rtc_help();
		return;
	}

	aon_rtc_id = os_strtoul(argv[1], NULL, 10);
	if (os_strcmp(argv[2], "start") == 0)
	{
		aon_rtc_autotest_start(aon_rtc_id);
	}
	else if (os_strcmp(argv[2], "stop") == 0)
	{
		aon_rtc_autotest_stop(aon_rtc_id);
	}
	else
		cli_aon_rtc_help();
}

#if AON_RTC_DEBUG
static void cli_aon_rtc_timing_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t aon_rtc_id;
	uint32_t round, cnts, set_tick_val = 0xfffffff0;

	if (argc != 5) {
		cli_aon_rtc_help();
		return;
	}

	aon_rtc_id = os_strtoul(argv[1], NULL, 10);
	round = os_strtoul(argv[2], NULL, 10);
	cnts = os_strtoul(argv[3], NULL, 10);
	set_tick_val = os_strtoul(argv[4], NULL, 10);

	bk_aon_rtc_timing_test(aon_rtc_id, round, cnts, set_tick_val);
}
#endif

static void cli_aon_rtc_dump_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t aon_rtc_id;

	if (argc != 2) {
		cli_aon_rtc_help();
		return;
	}

	aon_rtc_id = os_strtoul(argv[1], NULL, 10);

	bk_aon_rtc_dump(aon_rtc_id);
}


#define AON_RTC_CMD_CNT (sizeof(s_aon_rtc_commands) / sizeof(struct cli_command))
static const struct cli_command s_aon_rtc_commands[] = {
	{"aon_rtc_driver", "aon_rtc_driver {init|deinit}", cli_aon_rtc_driver_cmd},
#if 0	//remove it, only one HW can't be used for many APPs.
	{"aon_rtc", "aon_rtc {id} {create|destroy}", cli_aon_rtc_cmd},
	{"aon_rtc_int", "aon_rtc_int {id} {tick|upper}", cli_aon_rtc_int_cmd},
#endif
	{"aon_rtc_get_time", "aon_rtc_get_time {id}", cli_aon_rtc_get_time},
	{"aon_rtc_register", "aon_rtc_register {id} {name} {period_tick} {period_cnt}, {callback}", cli_aon_rtc_register_cmd},
	{"aon_rtc_unregister", "aon_rtc_unregister {id} {name}", cli_aon_rtc_unregister_cmd},
	{"aon_rtc_auto_test", "{id} {start|stop}", cli_aon_rtc_auto_test_cmd},
#if AON_RTC_DEBUG
	{"aon_rtc_timing_test", "{id} {rounds} {cycles} {set tick val}", cli_aon_rtc_timing_test_cmd},
#endif
	{"aon_rtc_dump", "{id}", cli_aon_rtc_dump_cmd},
};

int cli_aon_rtc_init(void)
{
	BK_LOG_ON_ERR(bk_aon_rtc_driver_init());
	return cli_register_commands(s_aon_rtc_commands, AON_RTC_CMD_CNT);
}

