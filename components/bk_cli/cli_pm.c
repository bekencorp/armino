// Copyright 2022-2023 Beken
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
#include <pm/pm.h>
#include <pm/policy.h>
#include <driver/device.h>
#include <driver/sys_pm.h>
#include <driver/gpio.h>

DEVICE_EXPORT(sys);

static void cli_pm_help(void)
{
	CLI_LOGI("pm help\r\n");
	CLI_LOGI("pm {suspend|resume|on|off} {dev}\r\n");
	CLI_LOGI("pm vote {dev1} {dev2} {on|off|suspend|resume|freq} [26M|120M|240M|320M]\r\n");
	CLI_LOGI("pm policy {lock|unlock} {normal|lv|deep} {dev}\n");
	CLI_LOGI("   policy force {normal|lv|deep}\n");
	CLI_LOGI("   policy latency\n");
	CLI_LOGI("   policy state\n");
	CLI_LOGI("pm wakesource {enable|disable} {dev}\r\n");
	CLI_LOGI("pm depend {dev1_id} {dev2_id} {type}\r\n");
#if CONFIG_PM_DEBUG
	CLI_LOGI("pm debug tree [dev]\r\n");
	CLI_LOGI("   debug list\r\n");
	CLI_LOGI("   debug stats\r\n");
	CLI_LOGI("   debug ctrl\r\n");
	CLI_LOGI("   debug log {0|1|2}\r\n");
#endif
#if CONFIG_PM_HAL_CTRL
	CLI_LOGI("pm buck {a|d|pa} {0|1}\r\n");
	CLI_LOGI("   buck {a|d|pa} pfm {0|1}\r\n");
	CLI_LOGI("   buck {a|d|pa} burst {0|1}\r\n");
	CLI_LOGI("pm ldo {aon|ana|io|core|digital} self_lp {0|1}\r\n");
	CLI_LOGI("   ldo {aon|io} volt {value}\r\n");
	CLI_LOGI("   ldo {ana} volt {trsw: 0|1} {rx_volt} {tx_volt}\r\n");
	CLI_LOGI("   ldo {core|digital} volt {lp: 0|1} {low_volt} {high_volt}\r\n");
	CLI_LOGI("   ldo {aon|ana|io|core|digital} {0|1}\r\n");
	CLI_LOGI("pm lv pd {0|1}\r\n");
	CLI_LOGI("   lv hf {0|1}\r\n");
	CLI_LOGI("   lv flash {0|1}\r\n");
	CLI_LOGI("   lv core {0|1}\r\n");
#endif
}

static void cli_pm_device_vote_freq(const device_t *dev1, const device_t *dev2, int argc, char **argv)
{
	pm_device_freq_t freq;

	CLI_RET_ON_INVALID_ARGC(argc, 6);

	if (os_strcmp(argv[5], "26M") == 0) {
		freq = PM_DEVICE_FREQ_26M;
	} else if (os_strcmp(argv[5], "120M") == 0) {
		freq = PM_DEVICE_FREQ_120M;
	} else if (os_strcmp(argv[5], "240M") == 0) {
		freq = PM_DEVICE_FREQ_240M;
	} else if (os_strcmp(argv[5], "320M") == 0) {
		freq = PM_DEVICE_FREQ_320M;
	} else {
		CLI_LOGI("Unknown cpu freq %s\r\n", argv[5]);
		return;
	}

	BK_LOG_ON_ERR(pm_device_vote_freq(dev1, dev2, freq));
}

static void cli_pm_device_depend(const device_t *dev1, int argc, char **argv)
{
	const device_t *dev2 = device_get_ptr_by_name(argv[3]);
	pm_depend_type_t type = 0;

	if (!dev2) {
		CLI_LOGI("Invalid device id %s\r\n", argv[3]);
		return;
	}

	if (os_strcmp(argv[4], "pd") == 0) {
		type = PM_DEPEND_TYPE_PD;
	} else if (os_strcmp(argv[4], "freq") == 0) {
		type = PM_DEPEND_TYPE_VOTE_FREQ;
	} else {
		CLI_LOGI("Unknown depend type %s\r\n", argv[4]);
		return;
	}

	uint32_t value = os_strtoul(argv[5], NULL, 10);
	BK_LOG_ON_ERR(pm_depend_add(dev1, dev2, type, value));
}

static void cli_pm_device_vote(const device_t *dev1, int argc, char **argv)
{
	const device_t *dev2 = device_get_ptr_by_name(argv[3]);

	CLI_RET_ON_INVALID_ARGC(argc, 5);

	if (!dev2) {
		CLI_LOGI("Invalid device id %s\r\n", argv[3]);
		return;
	}

	if (os_strcmp(argv[4], "on") == 0) {
		BK_LOG_ON_ERR(pm_device_vote_poweron(dev1, dev2));
	} else if (os_strcmp(argv[4], "off") == 0) {
		BK_LOG_ON_ERR(pm_device_vote_poweroff(dev1, dev2));
	} else if (os_strcmp(argv[4], "suspend") == 0) {
		BK_LOG_ON_ERR(pm_device_vote_suspend(dev1, dev2));
	} else if (os_strcmp(argv[4], "resume") == 0) {
		BK_LOG_ON_ERR(pm_device_vote_resume(dev1, dev2));
	} else if (os_strcmp(argv[4], "freq") == 0) {
		return cli_pm_device_vote_freq(dev1, dev2, argc, argv);
	} else {
		CLI_LOGI("Invalid vote type\r\n");
		return;
	}
}

static void cli_pm_device_cmd(int argc, char **argv)
{
	const device_t *device = device_get_ptr_by_name(argv[2]);
	if (!device) {
		CLI_LOGI("Invalid device id %s\r\n", argv[2]);
		return;
	}

	if (os_strcmp(argv[1], "suspend") == 0) {
		BK_LOG_ON_ERR(pm_device_suspend(device));
	} else if (os_strcmp(argv[1], "resume") == 0) {
		BK_LOG_ON_ERR(pm_device_resume(device));
	} else if (os_strcmp(argv[1], "on") == 0) {
		BK_LOG_ON_ERR(pm_device_poweron(device));
	} else if (os_strcmp(argv[1], "off") == 0) {
		BK_LOG_ON_ERR(pm_device_poweroff(device));
	} else if (os_strcmp(argv[1], "depend") == 0) {
		CLI_RET_ON_INVALID_ARGC(argc, 5);
		cli_pm_device_depend(device, argc, argv);
	} else if (os_strcmp(argv[1], "vote") == 0) {
		cli_pm_device_vote(device, argc, argv);
	} else {
		cli_pm_help();
	}
}

#if CONFIG_PM_DEBUG
static void cli_pm_tree_cmd(int argc, char **argv)
{
	if (argc >= 4) {
		const device_t *device = device_get_ptr_by_name(argv[3]);
		if (!device) {
			CLI_LOGI("Invalid device id %s\r\n", argv[3]);
			return;
		}
		BK_LOG_ON_ERR(pm_device_dump_tree(device));
	} else {
		BK_LOG_ON_ERR(pm_device_dump_all_tree());
	}
}

static void cli_pm_log_cmd(int argc, char **argv)
{
	if (argc >= 4) {
		uint32_t log_level = os_strtoul(argv[3], NULL, 10);
		pm_set_log_level(log_level);		
	}
}

static void cli_pm_debug_cmd(int argc, char **argv)
{
	CLI_RET_ON_INVALID_ARGC(argc, 3);

	if (os_strcmp(argv[2], "stats") == 0) {
		pm_stats_dump();
	} else if (os_strcmp(argv[2], "list") == 0) {
		BK_LOG_ON_ERR(pm_device_dump_all());
	} else if (os_strcmp(argv[2], "tree") == 0) {
		cli_pm_tree_cmd(argc, argv);
	} else if (os_strcmp(argv[2], "ctrl") == 0) {
#if CONFIG_PM_HAL_CTRL
		sys_pm_dump_ctrl();
#endif
	} else if (os_strcmp(argv[2], "log") == 0) {
		cli_pm_log_cmd(argc, argv);
	} else if (os_strcmp(argv[2], "policy") == 0) {
		pm_policy_dump();
	} else {
		cli_pm_help();
	}
}
#endif
	
static void cli_pm_policy_lock_unlock_cmd(int argc, char **argv, bool lock)
{
	CLI_RET_ON_INVALID_ARGC(argc, 4);
	pm_state_t state = PM_STATE_ACTIVE;

	if (os_strcmp(argv[3], "normal") == 0) {
		state = PM_STATE_NORMAL_SLEEP;
	} else if (os_strcmp(argv[3], "lv") == 0) {
		state = PM_STATE_LOW_VOLTAGE;
	} else if (os_strcmp(argv[3], "deep") == 0) {
		state = PM_STATE_DEEP_SLEEP;
	} else {
		CLI_LOGI("invalid pm state\r\n");
		return;
	}

	if (argc == 5) {
		const device_t *device = device_get_ptr_by_name(argv[4]);
		if (!device) {
			CLI_LOGI("Invalid device id %s\r\n", argv[4]);
			return;
		}

		if (lock) {
			BK_LOG_ON_ERR(pm_policy_device_lock_state(device, state));
		} else {
			BK_LOG_ON_ERR(pm_policy_device_unlock_state(device, state));
		}
	} else {
		if (lock) {
			BK_LOG_ON_ERR(pm_policy_lock_state(state));
		} else {
			BK_LOG_ON_ERR(pm_policy_unlock_state(state));
		}
	}
}

static void cli_pm_force_cmd(int argc, char **argv)
{
	pm_state_t state;
	uint32_t sleep_ms = 0;
	CLI_RET_ON_INVALID_ARGC(argc, 4);

	if (os_strcmp(argv[3], "normal") == 0) {
		state = PM_STATE_NORMAL_SLEEP;
	} else if (os_strcmp(argv[3], "lv") == 0) {
		state = PM_STATE_LOW_VOLTAGE;
	} else if (os_strcmp(argv[3], "deep") == 0) {
		state = PM_STATE_DEEP_SLEEP;
	} else {
		cli_pm_help();
		return;
	}

	if (argc == 5) {
		sleep_ms = os_strtoul(argv[4], NULL, 10);
	}
	pm_force_state_with_sleep_time(state, sleep_ms);
}

static void cli_pm_policy_cmd(int argc, char **argv)
{
	CLI_RET_ON_INVALID_ARGC(argc, 3);

	if (os_strcmp(argv[2], "lock") == 0) {
		cli_pm_policy_lock_unlock_cmd(argc, argv, true);
	} else if (os_strcmp(argv[2], "force") == 0) {
		cli_pm_force_cmd(argc, argv);
	} else if (os_strcmp(argv[2], "unlock") == 0) {
		cli_pm_policy_lock_unlock_cmd(argc, argv, false);
	} else if (os_strcmp(argv[2], "latency") == 0) {
	} else if (os_strcmp(argv[2], "state") == 0) {
	} else {
		cli_pm_help();
	}
}

static void cli_pm_wakesource_cmd(int argc, char **argv)
{
	CLI_RET_ON_INVALID_ARGC(argc, 4);
	const device_t *device = device_get_ptr_by_name(argv[3]);
	if (!device) {
		CLI_LOGI("Invalid device id %s\r\n", argv[3]);
		return;
	}

	if (os_strcmp(argv[2], "enable") == 0) {
		BK_LOG_ON_ERR(pm_device_wakesource_enable(device));
	} else if (os_strcmp(argv[2], "disable") == 0) {
		BK_LOG_ON_ERR(pm_device_wakesource_disable(device));
	} else {
		cli_pm_help();
		return;
	}
}

static void cli_pm_buck_cmd(int argc, char **argv)
{
	sys_buck_type_t buck;
	CLI_RET_ON_INVALID_ARGC(argc, 4);

	if (os_strcmp(argv[2], "a") == 0) {
		buck = SYS_BUCK_ANALOG;
	} else if (os_strcmp(argv[2], "d") == 0) {
		buck = SYS_BUCK_DIGITAL;
	} else if (os_strcmp(argv[2], "pa") == 0) {
		buck = SYS_BUCK_PA;
	} else {
		cli_pm_help();
		return;
	}

	if (argc == 4) {
		uint32_t ena = os_strtoul(argv[3], NULL, 10);
		BK_LOG_ON_ERR(sys_pm_set_buck(buck, !!ena));
	} else {
		uint32_t ena = os_strtoul(argv[4], NULL, 10);
	
		if (os_strcmp(argv[3], "pfm") == 0) {
			BK_LOG_ON_ERR(sys_pm_set_buck_pfm(buck, !!ena));
		} else if (os_strcmp(argv[3], "burst") == 0) {
			BK_LOG_ON_ERR(sys_pm_set_buck_burst(buck, !!ena));
		} else if (os_strcmp(argv[3], "mpo") == 0) {
			BK_LOG_ON_ERR(sys_pm_set_buck_mpo(buck, !!ena));
		} else {
			cli_pm_help();
		}
	}
}

static void cli_pm_ldo_cmd(int argc, char **argv)
{
}

static void cli_pm_lv_cmd(int argc, char **argv)
{
	CLI_RET_ON_INVALID_ARGC(argc, 4);

	uint32_t ena = os_strtoul(argv[3], NULL, 10);

	if (os_strcmp(argv[2], "pd") == 0) {
		sys_pm_set_lv_ctrl_pd(!!ena);
	} else if (os_strcmp(argv[2], "hf") == 0) {
		sys_pm_set_lv_ctrl_hf(!!ena);
	} else if (os_strcmp(argv[2], "flash") == 0) {
		sys_pm_set_lv_ctrl_flash(!!ena);
	} else if (os_strcmp(argv[2], "core") == 0) {
		sys_pm_set_lv_ctrl_core(!!ena);
	} else {
		cli_pm_help();
		return;
	}

}

static void cli_pm_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if ((os_strcmp(argv[1], "help") == 0) || (argc < 2)) {
		cli_pm_help();
	} else if (os_strcmp(argv[1], "policy") == 0) {
		cli_pm_policy_cmd(argc, argv);
	} else if (os_strcmp(argv[1], "wakesource") == 0) {
		cli_pm_wakesource_cmd(argc, argv);
#if CONFIG_PM_DEBUG
	} else if (os_strcmp(argv[1], "debug") == 0) {
		cli_pm_debug_cmd(argc, argv);
#endif
#if CONFIG_PM_HAL_CTRL
	} else if (os_strcmp(argv[1], "buck") == 0) {
		cli_pm_buck_cmd(argc, argv);
	} else if (os_strcmp(argv[1], "ldo") == 0) {
		cli_pm_ldo_cmd(argc, argv);
	} else if (os_strcmp(argv[1], "lv") == 0) {
		cli_pm_lv_cmd(argc, argv);
#endif
	} else {
		cli_pm_device_cmd(argc, argv);
		return;
	}
}

#define PM_CMD_CNT (sizeof(s_pm_commands) / sizeof(struct cli_command))
static const struct cli_command s_pm_commands[] = {
	{"pm2", "{off|on|suspend|resume|policy|depend|debug}", cli_pm_cmd},
};

int cli_pm_init(void)
{
	BK_LOG_ON_ERR(pm_init());
	return cli_register_commands(s_pm_commands, PM_CMD_CNT);
}

static int cli_pm_action_cb(const device_t *device, pm_device_action_t action)
{
	switch (action) {
	case PM_DEVICE_ACTION_SUSPEND:
		CLI_LOGD("%s suspend\r\n", device_get_name(device));
		break;
	case PM_DEVICE_ACTION_RESUME:
		CLI_LOGD("%s resume\r\n", device_get_name(device));
		break;
	case PM_DEVICE_ACTION_POWER_ON:
		CLI_LOGD("%s power on\r\n", device_get_name(device));
		break;
	case PM_DEVICE_ACTION_POWER_OFF:
		CLI_LOGD("%s power off\r\n", device_get_name(device));
		break;
	case PM_DEVICE_ACTION_UPDATE_FREQ:
		CLI_LOGD("%s power off\r\n", device_get_name(device));
		break;
	default:
		CLI_LOGD("unknown action %d\r\n", action);
		break;

	}

        return BK_OK;
}

//Define three application device for CLI test only
PM_DEVICE_DEFINE(pd1,  sys,  0, 0, cli_pm_action_cb);
PM_DEVICE_DEFINE(pd2,  sys,  0, 0, cli_pm_action_cb);
PM_DEVICE_DEFINE(d1,   pd1,  0, 0, cli_pm_action_cb);
PM_DEVICE_DEFINE(d2,   pd1,  0, 0, cli_pm_action_cb);
PM_DEVICE_DEFINE(d3,   pd1,  0, 0, cli_pm_action_cb);
PM_DEVICE_DEFINE(d4,   null, 0, 0, cli_pm_action_cb);
PM_DEVICE_DEFINE(d5,   null, 0, 0, cli_pm_action_cb);
PM_DEVICE_DEFINE(d6,   null, 0, 0, cli_pm_action_cb);
PM_DEVICE_DEFINE(app1, null, 0, 0, cli_pm_action_cb);
PM_DEVICE_DEFINE(app2, null, 0, 0, cli_pm_action_cb);

//Define GPIO wakesource
//TODO optimize and simplify it, maybe need to define a MACRO
static void gpio_isr(gpio_id_t id)
{
	CLI_LOGI("gpio%d isr\r\n", id);
}

static gpio_wakeup_config_t s_gpio_wakeup_config = {
	.id = 22,
	.int_type = GPIO_INT_TYPE_RISING_EDGE,
	.isr = gpio_isr,
	.valid = 0,
        };

PM_DEVICE_DEFINE(gpio, null, 0, &s_gpio_wakeup_config, default_gpio_pm_action_cb);
