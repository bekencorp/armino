#include <stdlib.h>
#include "cli.h"
#include "bk_gpio.h"
#include <driver/gpio.h>
#include <driver/hal/hal_gpio_types.h>
#include "gpio_driver.h"

static void cli_gpio_help(void)
{
	CLI_LOGI("gpio_driver [init/deinit] \r\n");

	CLI_LOGI("gpio    [set_config/input_pulldown/input_pullup////]    only in set_sonfig:[io_mode]    [pull mode] \r\n");
	CLI_LOGI("gpio    [output/input]    [gpio_pin]    [pullup/pulldown] \r\n");
	CLI_LOGI("gpio    [output_high/output_low/input_get]    [gpio_pin] \r\n");
	CLI_LOGI("gpio_map    [sdio_map/spi_map]     [mode]\r\n");
	CLI_LOGI("gpio_int    [index]    [inttype/start/stop]    [low/high_level/rising/falling edge]\r\n");
#if CONFIG_GPIO_DYNAMIC_WAKEUP_SUPPORT
	CLI_LOGI("gpio_wake    [index][low/high_level/rising/falling edge][enable/disable wakeup]\r\n");
	CLI_LOGI("gpio_low_power    [simulate][param]\r\n");
#endif
}

static void cli_gpio_driver_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2)
	{
		cli_gpio_help();
		return;
	}

	if (os_strcmp(argv[1], "init") == 0) {
		BK_LOG_ON_ERR(bk_gpio_driver_init());
		CLI_LOGI("gpio init\n");
	} else if(os_strcmp(argv[1], "deinit") == 0) {
		BK_LOG_ON_ERR(bk_gpio_driver_deinit());
		CLI_LOGI("gpio deinit\n");
	} else {
		cli_gpio_help();
		return;
	}
}



static void cli_gpio_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_gpio_help();
		return;
	}

	 if (os_strcmp(argv[1], "set_config") == 0) {
		gpio_id_t id;
		id = os_strtoul(argv[2], NULL, 10);

		gpio_config_t mode;

		mode.io_mode = os_strtoul(argv[3], NULL, 10);
		mode.pull_mode = os_strtoul(argv[4], NULL, 10);
		BK_LOG_ON_ERR(bk_gpio_set_config(id, &mode));

		CLI_LOGI("gpio io(output/disable/input): %x ,  pull(disable/down/up) : %x\n", mode.io_mode, mode.pull_mode);

	} else if (os_strcmp(argv[1], "output") == 0) {
		gpio_id_t id;

		id = os_strtoul(argv[2], NULL, 10);

		BK_LOG_ON_ERR(bk_gpio_disable_input(id));
		BK_LOG_ON_ERR(bk_gpio_enable_output(id));

		if (os_strcmp(argv[3], "pull_up") == 0) {
			BK_LOG_ON_ERR(bk_gpio_enable_pull(id));
			BK_LOG_ON_ERR(bk_gpio_pull_up(id));
		}else if (os_strcmp(argv[3], "pull_down") == 0) {
			BK_LOG_ON_ERR(bk_gpio_enable_pull(id));
			BK_LOG_ON_ERR(bk_gpio_pull_down(id));
		} else {
			BK_LOG_ON_ERR(bk_gpio_disable_pull(id));
		}

		CLI_LOGI("gpio output test: %x \n", id);

	}else if (os_strcmp(argv[1], "input") == 0) {
		gpio_id_t id;

		id = os_strtoul(argv[2], NULL, 10);

		BK_LOG_ON_ERR(bk_gpio_disable_output(id));
		BK_LOG_ON_ERR(bk_gpio_enable_input(id));

		if (os_strcmp(argv[3], "pull_up") == 0) {
			BK_LOG_ON_ERR(bk_gpio_enable_pull(id));
			BK_LOG_ON_ERR(bk_gpio_pull_up(id));
		}else if (os_strcmp(argv[3], "pull_down") == 0) {
			BK_LOG_ON_ERR(bk_gpio_enable_pull(id));
			BK_LOG_ON_ERR(bk_gpio_pull_down(id));
		} else {
			BK_LOG_ON_ERR(bk_gpio_disable_pull(id));
		}

		CLI_LOGI("gpio input test: %d \n", id);

	}else if (os_strcmp(argv[1], "output_high") == 0) {
		gpio_id_t id;

		id = os_strtoul(argv[2], NULL, 10);

		// must set gpio mode before gpio output test
		BK_LOG_ON_ERR(bk_gpio_set_output_high(id));

		CLI_LOGI("gpio output hgih\n");
	}else if (os_strcmp(argv[1], "output_low") == 0) {
		gpio_id_t id;

		id = os_strtoul(argv[2], NULL, 10);

		// must set gpio mode before gpio output test
		BK_LOG_ON_ERR(bk_gpio_set_output_low(id));

		CLI_LOGI("gpio output low\n");
	} else if (os_strcmp(argv[1], "input_get") == 0) {
		gpio_id_t id;

		id = os_strtoul(argv[2], NULL, 10);

		// must set gpio mode before gpio output test
		uint8_t input_value = bk_gpio_get_input(id);

		CLI_LOGI("gpio input value is %x\r\n", input_value);
	} else {
		cli_gpio_help();
		return;
	}
}


static void cli_gpio_map_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_gpio_help();
		return;
	}
	 uint8 mode = 0;

	 if (os_strcmp(argv[1], "devs") == 0) {
		gpio_id_t id = 0;

		id = os_strtoul(argv[2], NULL, 10);
		mode = os_strtoul(argv[3], NULL, 10);

		gpio_dev_map(id, mode);
	 }
#if ((CONFIG_SOC_BK7271) ||(CONFIG_SOC_BK7251) )
	 else if (os_strcmp(argv[1], "sdio_map") == 0) {

		mode = os_strtoul(argv[2], NULL, 10);
		BK_LOG_ON_ERR(gpio_sdio_sel(mode));
	 } else if (os_strcmp(argv[1], "spi_map") == 0){
		 mode = os_strtoul(argv[2], NULL, 10);
		 BK_LOG_ON_ERR(gpio_spi_sel(mode));
	 }
#if (CONFIG_SOC_BK7271)
	else if (os_strcmp(argv[1], "uart2_map") == 0) {

		mode = os_strtoul(argv[2], NULL, 10);
		BK_LOG_ON_ERR(gpio_uart2_sel(mode));
	 } else if (os_strcmp(argv[1], "pwms_map") == 0){
		 mode = os_strtoul(argv[2], NULL, 10);
		 uint32 channel = os_strtoul(argv[3], NULL, 10);
		BK_LOG_ON_ERR(gpio_pwms_sel(channel,mode));
	 }

#endif
#endif
	else {
		cli_gpio_help();
		return;
	}
}

#if CONFIG_GPIO_DYNAMIC_WAKEUP_SUPPORT
static void cli_gpio_set_wake_source_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 4) {
		cli_gpio_help();
		return;
	}

	gpio_id_t id = 0;
	uint32_t mode = 0;

	id = os_strtoul(argv[2], NULL, 10);
	mode = os_strtoul(argv[3], NULL, 10);

	if(os_strcmp(argv[1], "register") == 0)
		bk_gpio_register_wakeup_source(id, mode);
	else if(os_strcmp(argv[1], "unregister") == 0)
		bk_gpio_unregister_wakeup_source(id);
	else
		return cli_gpio_help();
}

static void cli_gpio_simulate_low_power_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 3) {
		cli_gpio_help();
		return;
	}

	uint32_t param = 0;

	param = os_strtoul(argv[2], NULL, 10);

	if(os_strcmp(argv[1], "simulate") == 0)
	{
		uint32_t int_level = 0;

		//if param == 0x534b4950 == "SKIP" == 1,397,442,896
		//means not switch GPIO to low power status just do save and restore
		CLI_LOGD("mode:%d,0x%x\n", param, param);
		int_level = rtos_disable_int();
		gpio_enter_low_power((void *)param);	
		rtos_enable_int(int_level);

		//TODO:simulate sytem entry low voltage.

		int_level = rtos_disable_int();
		gpio_exit_low_power((void *)param);
		rtos_enable_int(int_level);
		return;
	}
	else
		return cli_gpio_help();
}
#endif

static void cli_gpio_int_isr(gpio_id_t id)
{
	CLI_LOGI("gpio isr index:%d\n",id);
}

static void cli_gpio_int_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t id;
	gpio_config_t cfg;

	cfg.io_mode =GPIO_INPUT_ENABLE;

	if (argc < 2) {
		cli_gpio_help();
		return;
	}

	id = os_strtoul(argv[1], NULL, 10);

	if (os_strcmp(argv[2], "intype") == 0) {
		CLI_RET_ON_INVALID_ARGC(argc, 4);
		gpio_int_type_t int_type = 0;
		if (os_strcmp(argv[3], "low_level") == 0) {
			int_type = GPIO_INT_TYPE_LOW_LEVEL;
			cfg.pull_mode = GPIO_PULL_UP_EN;
			bk_gpio_set_config(id, &cfg);
		} else if (os_strcmp(argv[3], "high_level") == 0) {
			int_type = GPIO_INT_TYPE_HIGH_LEVEL;
			cfg.pull_mode = GPIO_PULL_DOWN_EN;
			bk_gpio_set_config(id, &cfg);
		}else if (os_strcmp(argv[3], "rising_edge") == 0) {
			int_type = GPIO_INT_TYPE_RISING_EDGE;
			cfg.pull_mode = GPIO_PULL_DOWN_EN;
			bk_gpio_set_config(id, &cfg);
		} else if (os_strcmp(argv[3], "falling_edge") == 0) {
			int_type = GPIO_INT_TYPE_FALLING_EDGE;
			cfg.pull_mode = GPIO_PULL_UP_EN;
			bk_gpio_set_config(id, &cfg);
		} else {
			cli_gpio_help();
			return;
		}
		bk_gpio_register_isr(id ,cli_gpio_int_isr);

		BK_LOG_ON_ERR(bk_gpio_set_interrupt_type(id, int_type));
		CLI_LOGI("gpio[%d] set int type:%x\n", id, int_type);
	} else if (os_strcmp(argv[2], "start") == 0) {
		BK_LOG_ON_ERR(bk_gpio_enable_interrupt(id));
	} else if (os_strcmp(argv[2], "stop") == 0) {
		BK_LOG_ON_ERR(bk_gpio_disable_interrupt(id));
		CLI_LOGI("gpio[%d] int  stop \r\n", id);

	} else {
		cli_gpio_help();
		return;
	}
}

#define GPIO_CMD_CNT (sizeof(s_gpio_commands) / sizeof(struct cli_command))
static const struct cli_command s_gpio_commands[] = {
	{"gpio_int", "gpio_int    [index]     [inttype/start/stop]     [low/high_level/rising/falling edge]", cli_gpio_int_cmd},
	{"gpio", "gpio     [set_mode/output_low/output_high/input/spi_mode]      [id]     [mode]", cli_gpio_cmd},
	{"gpio_driver", "gpio_driver    [init/deinit]}", cli_gpio_driver_cmd},
	{"gpio_map", "gpio_map     [sdio_map/spi_map]",cli_gpio_map_cmd},
#if CONFIG_GPIO_DYNAMIC_WAKEUP_SUPPORT
	{"gpio_wake", "gpio_wake [index][low/high_level/rising/falling edge][enable/disable wakeup]", cli_gpio_set_wake_source_cmd},
	{"gpio_low_power", "gpio_low_power [simulate][param]", cli_gpio_simulate_low_power_cmd}
#endif
};

int cli_gpio_init(void)
{
	BK_LOG_ON_ERR(bk_gpio_driver_init());
	return cli_register_commands(s_gpio_commands, GPIO_CMD_CNT);
}
