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

#include <common/bk_include.h>
#include "cli.h"
#include "shell_task.h"
#include <components/uvc_camera.h>
#include <os/mem.h>
#include <os/os.h>

#if CONFIG_USB_UVC

#if CONFIG_GENERAL_DMA
#include <driver/dma.h>
#endif

static void cli_uvc_help(void)
{
	CLI_LOGI("uvc {init|start|stop|read file_id|deinit}\r\n");
}
static void cli_uvc_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int err;
	//char cp1_cmd[] = "uvc_dma set\r\n";
	//uint16_t cp1_cmd_len = os_strlen(cp1_cmd);

	if (argc < 2) {
		cli_uvc_help();
		return;
	}

	if (os_strcmp(argv[1], "init") == 0) {
		//shell_cmd_forward(cp1_cmd, cp1_cmd_len);
		err = bk_uvc_init();//uvc_buff_set_open();
		if (err != kNoErr) {
			os_printf("uvc open failed!\r\n");
			return;
		}

		uint16_t ppi = os_strtoul(argv[2], NULL, 10) & 0xFFFF;
		uint8_t fps = os_strtoul(argv[3], NULL, 10) & 0xFF;
		err = bk_uvc_set_ppi_fps(ppi, fps);
		if (err != kNoErr) {
			os_printf("uvc set ppi and fps failed!\r\n");
			return;
		}
	} else if (os_strcmp(argv[1], "start") == 0) {
		err = bk_uvc_set_start();
		if (err != kNoErr) {
			os_printf("uvc set start failed!\r\n");
			return;
		}
	} else if (os_strcmp(argv[1], "stop") == 0) {
		err = bk_uvc_set_stop();
		if (err != kNoErr) {
			os_printf("uvc stop failed!\r\n");
		}
	} else if (os_strcmp(argv[1], "deinit") == 0) {
		err = bk_uvc_deinit();
		if (err != kNoErr) {
			os_printf("uvc close failed!\r\n");
		}
	}else if (os_strcmp(argv[1], "read") == 0) {
		uint8_t file_id = os_strtoul(argv[2], NULL, 10) & 0xFF;
		err = bk_uvc_save_frame(file_id);
		if (err != kNoErr) {
			os_printf("read failed!\r\n");
		}
	} else {
		cli_uvc_help();
		return;
	}
}

#define UVC_CMD_CNT (sizeof(s_uvc_commands) / sizeof(struct cli_command))
static const struct cli_command s_uvc_commands[] = {
	{"uvc", "uvc {start|stop|read file_id}", cli_uvc_cmd},
};

int cli_uvc_init(void)
{
    return cli_register_commands(s_uvc_commands, UVC_CMD_CNT);
}
#endif //CONFIG_USB_UVC



