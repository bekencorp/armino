#include "cli.h"
#include <os/os.h>
#include <driver/pwm.h>
#include "pwm_hal.h"

#if CONFIG_PWM

static void cli_pwm_help(void)
{
	CLI_LOGI("pwm_driver init {26M|DCO}\n");
	CLI_LOGI("pwm_driver deinit\n");
	CLI_LOGI("pwm {chan} init {period_v} {duty_v} [duty1_v][duty2_v][duty3_v]\n");
	CLI_LOGI("pwm {chan} {start|stop|deinit}\n");
	CLI_LOGI("pwm {chan} duty {period_v} {duty1_v} [duty2_v] [duty3_v]\n");
	CLI_LOGI("pwm {chan} signal {low|high}\n");
	CLI_LOGI("pwm_group init {chan1} {chan2} {period} {chan1_duty} {chan2_duty}\n");
	CLI_LOGI("pwm_group {start|stop|deinit}\n");
	CLI_LOGI("pwm_group config {group} {period} {chan1_duty} {chan2_duty}\n");
	CLI_LOGI("pwm_int {chan} {reg|enable|disable}\n");
	CLI_LOGI("pwm_capture {chan} {init}\nn");
	CLI_LOGI("pwm_capture {chan} {start|stop|deinit}\n");
	CLI_LOGI("pwm_idle_test {idle_start|idle_stop}\n");
}

static void cli_pwm_timer_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
}

static void cli_pwm_counter_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
}

static void cli_pwm_carrier_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
}

static void cli_pwm_driver_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_pwm_help();
		return;
	}

	if (os_strcmp(argv[1], "init") == 0) {
		BK_LOG_ON_ERR(bk_pwm_driver_init());
		CLI_LOGI("pwm init\n");
	} else if (os_strcmp(argv[1], "deinit") == 0) {
		BK_LOG_ON_ERR(bk_pwm_driver_deinit());
		CLI_LOGI("pwm deinit\n");
	} else {
		cli_pwm_help();
		return;
	}
}

static void cli_pwm_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t chan;

	if (argc < 2) {
		cli_pwm_help();
		return;
	}

	chan = os_strtoul(argv[1], NULL, 10);

	if (os_strcmp(argv[2], "init") == 0) {
		CLI_RET_ON_INVALID_ARGC(argc, 5);
		pwm_init_config_t config = {0};

		config.period_cycle = os_strtoul(argv[3], NULL, 10);
		config.duty_cycle = os_strtoul(argv[4], NULL, 10);
		if (argc > 5)
			config.duty2_cycle = os_strtoul(argv[5], NULL, 10);
		if (argc > 6)
			config.duty3_cycle = os_strtoul(argv[6], NULL, 10);

		BK_LOG_ON_ERR(bk_pwm_init(chan, &config));
		CLI_LOGI("pwm init, chan=%d period=%x duty=%x\n", chan, config.period_cycle, config.duty_cycle);
	} else if (os_strcmp(argv[2], "start") == 0) {
		BK_LOG_ON_ERR(bk_pwm_start(chan));
		CLI_LOGI("pwm start, chan=%d\n", chan);
	} else if (os_strcmp(argv[2], "stop") == 0) {
		BK_LOG_ON_ERR(bk_pwm_stop(chan));
		CLI_LOGI("pwm stop, chan=%d\n", chan);
	} else if (os_strcmp(argv[2], "deinit") == 0) {
		BK_LOG_ON_ERR(bk_pwm_deinit(chan));
		CLI_LOGI("pwm deinit, chan=%d\n", chan);
	} else if (os_strcmp(argv[2], "signal") == 0) {
		if (argc != 4) {
			cli_pwm_help();
			return;
		}

		if (os_strcmp(argv[3], "low") == 0)
			BK_LOG_ON_ERR(bk_pwm_set_init_signal_low(chan));
		else
			BK_LOG_ON_ERR(bk_pwm_set_init_signal_high(chan));
		CLI_LOGI("pwm set signal, chan=%d\n", chan);
	} else if (os_strcmp(argv[2], "duty") == 0) {
		pwm_period_duty_config_t config = {0};
		if (argc < 5) {
			cli_pwm_help();
			return;
		}

		config.period_cycle = os_strtoul(argv[3], NULL, 10);
		config.duty_cycle = os_strtoul(argv[4], NULL, 10);
		if (argc > 5)
			config.duty2_cycle = os_strtoul(argv[5], NULL, 10);
		if (argc > 6)
			config.duty3_cycle = os_strtoul(argv[6], NULL, 10);
		BK_LOG_ON_ERR(bk_pwm_set_period_duty(chan, &config));
		CLI_LOGI("pwm duty, chan=%d period=%x t1=%x t2=%x t3=%x\n", chan, config.period_cycle,
				 config.duty_cycle, config.duty2_cycle, config.duty3_cycle);
	} else {
		cli_pwm_help();
		return;
	}
}

static void cli_pwm_group_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t group;

	if (argc < 2) {
		cli_pwm_help();
		return;
	}

	group = os_strtoul(argv[2], NULL, 10);

	if (os_strcmp(argv[1], "init") == 0) {
		if (argc != 7) {
			cli_pwm_help();
			return;
		}
		pwm_group_init_config_t config = {0};
		pwm_group_t group = 0;

		config.chan1 = os_strtoul(argv[2], NULL, 10);
		config.chan2 = os_strtoul(argv[3], NULL, 10);
		config.period_cycle = os_strtoul(argv[4], NULL, 10);
		config.chan1_duty_cycle = os_strtoul(argv[5], NULL, 10);
		config.chan2_duty_cycle = os_strtoul(argv[6], NULL, 10);
		BK_LOG_ON_ERR(bk_pwm_group_init(&config, &group));
		CLI_LOGI("pwm init, group=%d chan1=%d chan2=%d period=%x d1=%x d2=%x\n",
				 group, config.chan1, config.chan2, config.period_cycle,
				 config.chan1_duty_cycle, config.chan2_duty_cycle);
	} else if (os_strcmp(argv[1], "start") == 0) {
		BK_LOG_ON_ERR(bk_pwm_group_start(group));
		CLI_LOGI("pwm start, group=%d\n", group);
	} else if (os_strcmp(argv[1], "stop") == 0) {
		BK_LOG_ON_ERR(bk_pwm_group_stop(group));
		CLI_LOGI("pwm stop, group=%d\n", group);
	} else if (os_strcmp(argv[1], "deinit") == 0) {
		BK_LOG_ON_ERR(bk_pwm_group_deinit(group));
		CLI_LOGI("pwm deinit, group=%d\n", group);
	} else if (os_strcmp(argv[1], "config") == 0) {
		pwm_group_config_t config = {0};
		if (argc != 5) {
			cli_pwm_help();
			return;
		}

		config.period_cycle = os_strtoul(argv[2], NULL, 10);
		config.chan1_duty_cycle = os_strtoul(argv[3], NULL, 10);
		config.chan2_duty_cycle = os_strtoul(argv[4], NULL, 10);
		BK_LOG_ON_ERR(bk_pwm_group_set_config(group, &config));
		CLI_LOGI("pwm config, group=%x period=%x chan1_duty=%x chan2_duty=%x\n",
				 group, config.period_cycle, config.chan1_duty_cycle, config.chan2_duty_cycle);
	} else {
		cli_pwm_help();
		return;
	}
}


static void cli_pwm_isr(pwm_chan_t chan)
{
	CLI_LOGI("isr(%d)\n", chan);
}

static void cli_pwm_capture_isr(pwm_chan_t chan)
{
	CLI_LOGI("capture(%d), value=%x\n", chan, bk_pwm_capture_get_value(chan));
}

static void cli_pwm_int_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t chan;

	if (argc != 3) {
		cli_pwm_help();
		return;
	}

	chan = os_strtoul(argv[1], NULL, 10);

	if (os_strcmp(argv[2], "reg") == 0) {
		BK_LOG_ON_ERR(bk_pwm_register_isr(chan, cli_pwm_isr));
		CLI_LOGI("pwm chan%d register interrupt isr\n", chan);
	} else if (os_strcmp(argv[2], "enable") == 0) {
		BK_LOG_ON_ERR(bk_pwm_enable_interrupt(chan));
		CLI_LOGI("pwm chan%d enable interrupt\n", chan);
	} else if (os_strcmp(argv[2], "disable") == 0) {
		BK_LOG_ON_ERR(bk_pwm_disable_interrupt(chan));
		CLI_LOGI("pwm chan%d disable interrupt\n", chan);
	} else {
		cli_pwm_help();
		return;
	}
}

static void cli_pwm_capture_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t chan;

	if (argc < 2) {
		cli_pwm_help();
		return;
	}

	chan = os_strtoul(argv[1], NULL, 10);

	if (os_strcmp(argv[2], "init") == 0) {
		CLI_RET_ON_INVALID_ARGC(argc, 4);
		pwm_capture_init_config_t config = {0};
		if (os_strcmp(argv[3], "pos") == 0)
			config.edge = PWM_CAPTURE_POS;
		else if (os_strcmp(argv[3], "neg") == 0)
			config.edge = PWM_CAPTURE_NEG;
		else if (os_strcmp(argv[3], "edge") == 0)
			config.edge = PWM_CAPTURE_EDGE;
		else {
			cli_pwm_help();
			return;
		}

		config.isr = cli_pwm_capture_isr;
		BK_LOG_ON_ERR(bk_pwm_capture_init(chan, &config));
		CLI_LOGI("pwm_capture init, chan=%d\n", chan);
	} else if (os_strcmp(argv[2], "start") == 0) {
		BK_LOG_ON_ERR(bk_pwm_capture_start(chan));
		CLI_LOGI("pwm_capture start, chan=%d\n", chan);
	} else if (os_strcmp(argv[2], "stop") == 0) {
		BK_LOG_ON_ERR(bk_pwm_capture_stop(chan));
		CLI_LOGI("pwm_capture stop, chan=%d\n", chan);
	} else if (os_strcmp(argv[2], "deinit") == 0) {
		BK_LOG_ON_ERR(bk_pwm_capture_deinit(chan));
		CLI_LOGI("pwm_capture deinit, chan=%d\n", chan);
	} else {
		cli_pwm_help();
		return;
	}
}

static void cli_pwm_idle_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_pwm_help();
		return;
	}

	 if (os_strcmp(argv[1], "idle_start") == 0) {
		BK_LOG_ON_ERR(bk_pwm_driver_init());
		CLI_LOGI("pwm driver init\n");

		pwm_init_config_t config = {0};

		config.period_cycle = 10;
		config.duty_cycle = 5;
		config.duty2_cycle = 0;
		config.duty3_cycle = 0;

		for(int chan = 0; chan < SOC_PWM_CHAN_NUM_PER_UNIT; chan++) {
			BK_LOG_ON_ERR(bk_pwm_init(chan, &config));
			BK_LOG_ON_ERR(bk_pwm_start(chan));
			CLI_LOGI("pwm init, chan=%d period=%x duty=%x\n", chan, config.period_cycle, config.duty_cycle);
		}
	} else if (os_strcmp(argv[1], "idle_stop") == 0) {
			for(int chan = 0; chan < SOC_PWM_CHAN_NUM_PER_UNIT; chan++) {
			BK_LOG_ON_ERR(bk_pwm_stop(chan));
			BK_LOG_ON_ERR(bk_pwm_deinit(chan));
		}

	} else {
		cli_pwm_help();
		return;
	}
}

#define PWM_CMD_CNT (sizeof(s_pwm_commands) / sizeof(struct cli_command))
static const struct cli_command s_pwm_commands[] = {
	{"pwm_driver", "{init|deinit} [26M|DCO]}", cli_pwm_driver_cmd},
	{"pwm", "pwm {chan} {config|start|stop|init|deinit|signal} [...]", cli_pwm_cmd},
	{"pwm_int", "pwm_int {chan} {reg|enable|disable}", cli_pwm_int_cmd},
	{"pwm_duty", "pwm_duty {chan} {period} {d1} [d2] [d3]", cli_pwm_int_cmd},
	{"pwm_capture", "pwm_capture {chan} {config|start|stop|init|deinit}", cli_pwm_capture_cmd},
	{"pwm_group", "pwm_group {init|deinit|config|start|stop} [...]", cli_pwm_group_cmd},
	{"pwm_timer", "pwm_timer ", cli_pwm_timer_cmd},
	{"pwm_counter", "pwm_counter", cli_pwm_counter_cmd},
	{"pwm_carrier", "pwm_carrier", cli_pwm_carrier_cmd},
	{"pwm_idle_test", "{idle_start|idle_stop}", cli_pwm_idle_test_cmd},
};

int cli_pwm_init(void)
{
	BK_LOG_ON_ERR(bk_pwm_driver_init());
	return cli_register_commands(s_pwm_commands, PWM_CMD_CNT);
}

#endif
