#include "bk_private/legacy_init.h"
#include <components/system.h>
#include <os/os.h>
#include <components/shell_task.h>

#include <driver/qspi.h>
#include <driver/lcd_qspi.h>
#include <driver/lcd_qspi_types.h>
#include "bk_misc.h"


#include "lcd_act.h"
#include "media_app.h"
#include "lv_vendor.h"
#include "driver/drv_tp.h"

#include "cli.h"

extern void user_app_main(void);
extern void rtos_set_user_app_entry(beken_thread_function_t entry);

#define CMDS_COUNT  (sizeof(s_meter_commands) / sizeof(struct cli_command))

lcd_qspi_device_t *s_device_config = NULL;

extern void lv_example_meter(void);

void cli_meter_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	os_printf("%s\r\n", __func__);
}

static const struct cli_command s_meter_commands[] =
{
	{"meter", "meter", cli_meter_cmd},
};

int cli_meter_init(void)
{
	return cli_register_commands(s_meter_commands, CMDS_COUNT);
}


#ifdef CONFIG_CACHE_CUSTOM_SRAM_MAPPING
const unsigned int g_sram_addr_map[4] =
{
	0x38000000,
	0x30020000,
	0x38020000,
	0x30000000
};
#endif

void meter_init(void)
{
#ifdef CONFIG_LV_USE_DEMO_METER
	lv_vnd_config_t lv_vnd_config;

	os_printf("meter Start\r\n");

	cli_meter_init();

	qspi_config_t lcd_qspi_config = {0};
	BK_LOG_ON_ERR(bk_qspi_driver_init());
	lcd_qspi_config.src_clk = 5;
	lcd_qspi_config.src_clk_div = 4;
	lcd_qspi_config.clk_div = 1;
	BK_LOG_ON_ERR(bk_qspi_init(&lcd_qspi_config));
	delay_ms(2);

	s_device_config = bk_lcd_qspi_get_device_by_name("sh8601a");
	if (s_device_config == NULL)
	{
		os_printf("lcd qspi device not found\r\n");
	}
	else
	{
		bk_lcd_qspi_init(s_device_config);
	}

#ifdef CONFIG_LVGL_USE_PSRAM
#define PSRAM_DRAW_BUFFER ((0x60000000UL) + 5 * 1024 * 1024)
	lv_vnd_config.draw_pixel_size = ppi_to_pixel_x(s_device_config->ppi) * ppi_to_pixel_y(s_device_config->ppi);
	lv_vnd_config.draw_buf_2_1 = (lv_color_t *)PSRAM_DRAW_BUFFER;
	lv_vnd_config.draw_buf_2_2 = (lv_color_t *)(PSRAM_DRAW_BUFFER + lv_vnd_config.draw_pixel_size * sizeof(lv_color_t));
#else
	lv_vnd_config.draw_pixel_size = (75 * 1024) / sizeof(lv_color_t);
	lv_vnd_config.draw_buf_2_1 = LV_MEM_CUSTOM_ALLOC(lv_vnd_config.draw_pixel_size * sizeof(lv_color_t));
	lv_vnd_config.draw_buf_2_2 = NULL;
#endif
	lv_vnd_config.rotation = ROTATE_NONE;
	lv_vnd_config.color_depth = LV_COLOR_DEPTH;
	lv_vendor_init(&lv_vnd_config, ppi_to_pixel_x(s_device_config->ppi), ppi_to_pixel_y(s_device_config->ppi));
	lv_vendor_start();

#if (CONFIG_TP)
	drv_tp_open(ppi_to_pixel_x(s_device_config->ppi), ppi_to_pixel_y(s_device_config->ppi));
#endif

	lv_example_meter();
#endif
}

void user_app_main(void)
{
}

int main(void)
{
#if (!CONFIG_SLAVE_CORE)
	rtos_set_user_app_entry((beken_thread_function_t)user_app_main);
	// bk_set_printf_sync(true);
	// shell_set_log_level(BK_LOG_WARN);
#endif
	legacy_init();

	meter_init();

	return 0;
}