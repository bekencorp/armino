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

#include "cli.h"
#if (CONFIG_MEDIA)
#if (CONFIG_CAMERA)
#include "camera.h"
#endif
#include "frame_buffer.h"
#endif
#include "driver/i2c.h"
#if (CONFIG_YUV_BUF)
#include "driver/yuv_buf.h"
#endif

#if (CONFIG_JPEG_ENCODE)
#include "driver/jpeg_enc.h"
#endif

#define CAMERA_DVP_I2C_ID        1
#define GC0328C_WRITE_ADDRESS    (0x42)
#define DVP_I2C_TIMEOUT          2000

extern void image_save_dvp(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

#if (CONFIG_NET_WORK_VIDEO_TRANSFER)
extern void cmd_video_transfer(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
#endif

static void cli_dvp_camera_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (os_strcmp(argv[1], "jpeg") == 0) {
#if (CONFIG_MEDIA)
		frame_buffer_init();
#if (CONFIG_CAMERA)
		BK_LOG_ON_ERR(bk_dvp_camera_open(PPI_640X480, MEDIA_DVP_MJPEG));
#endif
#endif
	} else if (os_strcmp(argv[1], "clock") == 0) { /* enable mclk */
		BK_LOG_ON_ERR(bk_jpeg_enc_set_gpio(JPEG_ENABLE_CLK));
#if (CONFIG_YUV_BUF)
		BK_LOG_ON_ERR(bk_yuv_buf_enable_mclk());
		BK_LOG_ON_ERR(bk_yuv_buf_start(YUV_BUF_MODE_YUV));
#endif
	} else if (os_strcmp(argv[1], "i2c_init") == 0) { /* camera i2c init */
		i2c_config_t i2c_config = {0};
		i2c_config.baud_rate = I2C_BAUD_RATE_100KHZ;
		i2c_config.addr_mode = I2C_ADDR_MODE_7BIT;
		BK_LOG_ON_ERR(bk_i2c_init(CAMERA_DVP_I2C_ID, &i2c_config));
	} else if (os_strcmp(argv[1], "get_chip_id") == 0) { /* get chip id */
		i2c_mem_param_t mem_param = {0};
		uint8_t chip_id = 0;

		mem_param.dev_addr = GC0328C_WRITE_ADDRESS >> 1;
		mem_param.mem_addr_size = I2C_MEM_ADDR_SIZE_8BIT;
		mem_param.data_size = 1;
		mem_param.timeout_ms = DVP_I2C_TIMEOUT;
		mem_param.mem_addr = 0xF0;
		mem_param.data = &chip_id;

		BK_LOG_ON_ERR(bk_i2c_memory_read(CAMERA_DVP_I2C_ID, &mem_param));

		CLI_LOGI("gc0328c chip id:%x\r\n", chip_id);
	} else {

	}
}

#define DVP_CMD_CNT (sizeof(s_dvp_commands) / sizeof(struct cli_command))
static const struct cli_command s_dvp_commands[] = {
	{"dvp", "dvp init/save_image file_id/read/deinit", image_save_dvp},
	{"dvp_camera", "dvp_camera open", cli_dvp_camera_cmd},
#if (CONFIG_NET_WORK_VIDEO_TRANSFER)
	{"video_transfer", "video_transfer --help",    cmd_video_transfer},
#endif
};

int cli_dvp_init(void)
{
	return cli_register_commands(s_dvp_commands, DVP_CMD_CNT);
}

