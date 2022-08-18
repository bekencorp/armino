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
#include <components/log.h>
#include "cli.h"
#include "media_cli.h"
#include "aud_api.h"

#include "media_cli_comm.h"
#include "media_app.h"

#include <driver/dvp_camera.h>

#define TAG "mcli"

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)

#define UNKNOW_ERROR (-686)
#define CMD_CONTAIN(value) cmd_contain(argc, argv, value)


uint32_t get_string_to_ppi(char *string, uint32_t pre)
{
	uint32_t value = pre;

	if (os_strcmp(string, "1280X720") == 0)
	{
		value = PPI_1280X720;
	}

	if (os_strcmp(string, "1024X600") == 0)
	{
		value = PPI_1024X600;
	}

	if (os_strcmp(string, "640X480") == 0)
	{
		value = PPI_640X480;
	}

	if (os_strcmp(string, "480X320") == 0)
	{
		value = PPI_480X320;
	}

	if (os_strcmp(string, "480X272") == 0)
	{
		value = PPI_480X272;
	}

	if (os_strcmp(string, "320X480") == 0)
	{
		value = PPI_320X480;
	}
	if (os_strcmp(string, "480X800") == 0)
	{
		value = PPI_480X800;
	}

	return value;
}

bool cmd_contain(int argc, char **argv, char *string)
{
	int i;
	bool ret = false;

	for (i = 0; i < argc; i++)
	{
		if (os_strcmp(argv[i], string) == 0)
		{
			ret = true;
		}
	}

	return ret;
}



void media_cli_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int ret = UNKNOW_ERROR;

	LOGI("%s\n", __func__);

	if (argc > 0)
	{
		if (os_strcmp(argv[1], "dvp") == 0)
		{
#if (defined(CONFIG_CAMERA) && !defined(CONFIG_SLAVE_CORE))
			media_ppi_t ppi = get_string_to_ppi(argv[4], PPI_DEFAULT);

			if (os_strcmp(argv[2], "open") == 0)
			{
				if (os_strcmp(argv[3], "yuv") == 0)
				{
					ret = media_app_camera_open(APP_CAMERA_YUV, ppi);
				}
				else
				{
					ret = media_app_camera_open(APP_CAMERA_DVP, ppi);
				}
			}

			if (os_strcmp(argv[2], "close") == 0)
			{
				if (os_strcmp(argv[3], "yuv") == 0)
				{
					ret = media_app_camera_close(APP_CAMERA_YUV);
				}
				else
				{
					ret = media_app_camera_close(APP_CAMERA_DVP);
				}
			}

			if (os_strcmp(argv[2], "auto_encode") == 0)
			{
				uint8_t auto_enable = 0;
				uint32_t up_size = 0, low_size = 0;

				auto_enable = os_strtoul(argv[3], NULL, 10) & 0xF;
				if (auto_enable)
				{
					up_size = os_strtoul(argv[4], NULL, 10) * 1024;
					low_size = os_strtoul(argv[5], NULL, 10) * 1024;
				}

				ret = bk_dvp_camera_encode_config(auto_enable, up_size, low_size);
			}
#endif
		}

#ifdef CONFIG_AUDIO
		if (os_strcmp(argv[1], "adc") == 0)
		{
			ret = adc_open();
		}

		if (os_strcmp(argv[1], "dac") == 0)
		{
			ret = dac_open();
		}
#endif

		if (os_strcmp(argv[1], "capture") == 0)
		{
			LOGI("capture\n");
#if defined(CONFIG_CAMERA) && !defined(CONFIG_SLAVE_CORE)

			if (argc >= 3)
			{
				ret = media_app_capture(argv[2]);
			}
			else
			{
				ret = media_app_capture("unknow.jpg");
			}
#endif
		}

		if (os_strcmp(argv[1], "lcd") == 0)
		{
#if defined(CONFIG_LCD) && !defined(CONFIG_SLAVE_CORE)
			media_ppi_t ppi = PPI_480X272;

			ppi = get_string_to_ppi(argv[3], PPI_480X272);

			if (CMD_CONTAIN("rotate"))
			{
				media_app_lcd_rotate(true);
			}

			if (os_strcmp(argv[2], "open") == 0)
			{
				ret = media_app_lcd_open(ppi);
			}

			if (os_strcmp(argv[2], "close") == 0)
			{
				ret = media_app_lcd_close();
			}

			if (os_strcmp(argv[2], "backlight") == 0)
			{
				uint8_t level = os_strtoul(argv[3], NULL, 10) & 0xFF;
				ret = media_app_lcd_set_backlight(level);
			}

			if (os_strcmp(argv[2], "step") == 0)
			{
				if (CMD_CONTAIN("enable"))
				{
					ret = media_app_lcd_step_mode(true);
				}

				if (CMD_CONTAIN("disable"))
				{
					ret = media_app_lcd_step_mode(false);
				}

				if (CMD_CONTAIN("trigger"))
				{
					ret = media_app_lcd_step_trigger();
				}
			}
#endif
		}
		if (os_strcmp(argv[1], "uvc") == 0)
		{
#if defined(CONFIG_USB_UVC) && !defined(CONFIG_SLAVE_CORE)
			media_ppi_t ppi = get_string_to_ppi(argv[3], PPI_DEFAULT);

			if (os_strcmp(argv[2], "open") == 0)
			{
				ret = media_app_camera_open(APP_CAMERA_UVC, ppi);
			}

			if (os_strcmp(argv[2], "start") == 0)
			{
				ret = media_app_uvc_start();
			}

			if (os_strcmp(argv[2], "stop") == 0)
			{
				ret = media_app_uvc_stop();
			}

			if (os_strcmp(argv[2], "param") == 0)
			{
				if (argc != 6)
				{
					LOGI("param number error\r\n");
					return;
				}

				uvc_camera_device_t config;

				config.width = os_strtoul(argv[3], NULL, 10);
				config.height = os_strtoul(argv[4], NULL, 10);
				config.fps = os_strtoul(argv[5], NULL, 10);
				ret = media_app_uvc_param_set(&config);
			}

			if (os_strcmp(argv[2], "close") == 0)
			{
				ret = media_app_camera_close(APP_CAMERA_UVC);
			}
#endif
		}

#ifdef CONFIG_MASTER_CORE
		if (os_strcmp(argv[1], "mb") == 0)
		{
			ret = media_app_mailbox_test();
		}
#endif

		if (os_strcmp(argv[1], "dump") == 0)
		{
#if defined(CONFIG_LCD) && !defined(CONFIG_SLAVE_CORE)

			if (CMD_CONTAIN("decoder"))
			{
				ret = media_app_dump_decoder_frame();
			}

			if (CMD_CONTAIN("jpeg"))
			{
				ret = media_app_dump_jpeg_frame();
			}

			if (CMD_CONTAIN("display"))
			{
				ret = media_app_dump_display_frame();
			}
#endif

		}
	}

	if (ret == UNKNOW_ERROR)
	{
		LOGE("%s unknow cmd\n", __func__);
	}
}



#define MEDIA_CMD_CNT   (sizeof(s_media_commands) / sizeof(struct cli_command))

static const struct cli_command s_media_commands[] =
{
	{"media", "meida...", media_cli_test_cmd},
};

int media_cli_init(void)
{
	return cli_register_commands(s_media_commands, MEDIA_CMD_CNT);
}
