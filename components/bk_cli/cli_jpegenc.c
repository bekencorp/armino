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

#include <driver/jpeg_enc.h>
#include "cli.h"
#include "jpeg_hw.h"

static void cli_jpeg_help(void)
{
	CLI_LOGI("jpeg_driver init\r\n");
	CLI_LOGI("jpeg_driver deinit\r\n");
	CLI_LOGI("jpeg init\r\n");
	CLI_LOGI("jpeg deinit\r\n");
}

static void cli_jpeg_driver_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char *msg = NULL;
	if (argc < 2) {
		cli_jpeg_help();
		msg = CLI_CMD_RSP_ERROR;
		os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
		return;
	}

	if (os_strcmp(argv[1], "init") == 0) {
		BK_LOG_ON_ERR(bk_jpeg_enc_driver_init());
		msg = CLI_CMD_RSP_SUCCEED;
	} else if (os_strcmp(argv[1], "deinit") == 0) {
		BK_LOG_ON_ERR(bk_jpeg_enc_driver_deinit());
		msg = CLI_CMD_RSP_SUCCEED;
	} else {
		cli_jpeg_help();
		msg = CLI_CMD_RSP_ERROR;
	}

	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
}

static void cli_jpeg_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char *msg = NULL;
	if (argc < 2) {
		cli_jpeg_help();
		msg = CLI_CMD_RSP_ERROR;
	}

	if (os_strcmp(argv[1], "init") == 0) {
		jpeg_config_t jpeg_cfg = {0};
		BK_LOG_ON_ERR(bk_jpeg_enc_driver_init());
		BK_LOG_ON_ERR(bk_jpeg_enc_init(&jpeg_cfg));
		msg = CLI_CMD_RSP_SUCCEED;
	} else if (os_strcmp(argv[1], "deinit") == 0) {
		BK_LOG_ON_ERR(bk_jpeg_enc_deinit());
		msg = CLI_CMD_RSP_SUCCEED;
	} else if (os_strcmp(argv[1], "debug") == 0) {
		CLI_LOGI("jpeg_rx_fifo_data:%x\r\n", REG_READ(JPEG_R_RX_FIFO));
		msg = CLI_CMD_RSP_SUCCEED;
	} else {
		cli_jpeg_help();
		msg = CLI_CMD_RSP_ERROR;
	}

	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
}

#define JPEG_CMD_CNT (sizeof(s_jpeg_commands) / sizeof(struct cli_command))
static const struct cli_command s_jpeg_commands[] = {
	{"jpeg_driver", "jpeg_driver {init|deinit}", cli_jpeg_driver_cmd},
	{"jpeg", "jpeg {init|deint}", cli_jpeg_cmd},
};

int cli_jpeg_init(void)
{
	return cli_register_commands(s_jpeg_commands, JPEG_CMD_CNT);
}

