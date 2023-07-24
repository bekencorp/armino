// Copyright 2022-2023 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//	 http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <cli.h>
#include "media_app.h"
#include <driver/flash.h>

extern void delay_ms(UINT32 ms);
extern bk_err_t demo_H264_udp_init(void);
extern bk_err_t demo_H264_tcp_init(void);

static void video_print(uint8_t *buffer, int length)
{
	for (int i = 0; i < length; i++)
	{
		os_printf("%02x ", *(buffer+i));
	}
	os_printf("\r\n");
}

static void transfer_demo_usb(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
#if (CONFIG_USB_DEVICE && CONFIG_USB_UVC)
	if (os_strcmp(argv[1], "start") == 0)
	{
		media_ppi_t media_ppi = PPI_640X480;
		media_ppi = get_string_to_ppi(argv[3]);
		video_setup_t video_setup = {0};

		if (os_strcmp(argv[2], "h264") == 0)
		{
			media_app_camera_open(APP_CAMERA_DVP_H264_TRANSFER, media_ppi);
			video_setup.open_type = APP_CAMERA_DVP_H264_TRANSFER;
		}
		else if (os_strcmp(argv[2], "mjpeg") == 0)
		{
			media_app_camera_open(APP_CAMERA_DVP_JPEG, media_ppi);
			video_setup.open_type = APP_CAMERA_DVP_JPEG;
		}

		delay_ms(1000);
		media_app_usb_open(&video_setup);
	}
#endif
}

static void transfer_demo_wifi(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	// to do
}

#define VIDEO_TRANSFER_CMD_CNT (sizeof(video_transfer_cmd) / sizeof(struct cli_command))
static const struct cli_command video_transfer_cmd[] = {
	{"transfer_wifi", "transfer_wifi {tcp|udp|rtp}", transfer_demo_wifi},
#if (CONFIG_USB_DEVICE && CONFIG_USB_UVC)
	{"transfer_usb", "transfer_usb {start}", transfer_demo_usb},
#endif
};

int cli_video_transfer_init(void)
{
	return cli_register_commands(video_transfer_cmd, VIDEO_TRANSFER_CMD_CNT);
}
