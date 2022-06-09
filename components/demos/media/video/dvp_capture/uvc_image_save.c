#include <stdio.h>
#include <os/mem.h>
#include <os/str.h>
#include <os/os.h>
#include "bk_cli.h"
#include <components/uvc_camera.h>
#include <driver/psram.h>

extern void delay(INT32 num);


static void uvc_help(void)
{
	os_printf("uvc_image {init|start|stop|save_image file_id|deinit}\r\n");
}

void image_save_uvc(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int err;

	if (argc < 2) {
		uvc_help();
		return;
	}

	if (os_strcmp(argv[1], "init") == 0) {
		uint32_t mode = 0x00054043;
		err = bk_psram_init(mode);
		if (err != BK_OK) {
			os_printf("psram init error\n");
			return;
		}

		err = bk_uvc_init();
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

		delay(2000);
		err = bk_psram_deinit();
		if (err != BK_OK) {
			os_printf("uvc deinit psram error!\r\n");
		}
	}else if (os_strcmp(argv[1], "save_image") == 0) {
		uint8_t file_id = os_strtoul(argv[2], NULL, 10) & 0xFF;
		err = bk_uvc_save_frame(file_id);
		if (err != kNoErr) {
			os_printf("read failed!\r\n");
		}
	} else {
		uvc_help();
		return;
	}
}
