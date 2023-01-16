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
#include <os/mem.h>
#include <os/str.h>
#include <os/os.h>
#include <driver/qspi.h>
#include <driver/qspi_types.h>
#include <driver/lcd_qspi.h>
#include <driver/lcd_qspi_types.h>
#include "cli.h"
#if (CONFIG_FATFS)
#include "ff.h"
#include "diskio.h"
#endif


#define READ_SDCARD_PICTURE_SIZE	1024 * 64
#define write_data(addr,val)                 *((volatile uint32_t *)(addr)) = val
#define get_addr_data(addr)                  *((volatile uint32_t *)(addr))

#if (CONFIG_FATFS)
static  __attribute__((section(".itcm_sec_code "))) void memcpy_word(uint32_t *dst, uint32_t *src, uint32_t size)
{
	uint32_t i = 0;

	for (i = 0; i < size; i++) {
		write_data((dst+i), get_addr_data(src+i));

	}
}
#endif

void cli_lcd_qspi_display_pure_color_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	qspi_config_t lcd_qspi_config = {0};
	uint32_t red = 0xFF0000;

	if (argc != 3) {
		CLI_LOGI("lcd_qspi_display_pure_color_test {start|stop} {device name} \r\n");
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		BK_LOG_ON_ERR(bk_qspi_driver_init());

		lcd_qspi_config.src_clk = 5;
		lcd_qspi_config.src_clk_div = 4;
		lcd_qspi_config.clk_div = 10;
		BK_LOG_ON_ERR(bk_qspi_init(&lcd_qspi_config));
		rtos_delay_milliseconds(5);

		lcd_qspi_device_t *device_config = NULL;

		device_config = bk_lcd_qspi_get_device_by_name(argv[2]);
		if (device_config == NULL) {
			os_printf("qspi oled device not found\r\n");
		} else {
			bk_lcd_qspi_init(device_config);
			bk_lcd_qspi_send_data(device_config, &red, 1);
		}

	} else if (os_strcmp(argv[1], "stop") == 0) {
//		bk_qspi_sha8601a_deinit();

	} else {
		CLI_LOGI("qspi_oled_display_pure_color_test {start|stop} \r\n");
	}
}

void cli_lcd_qspi_display_picture_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	qspi_config_t lcd_qspi_config = {0};

	if (argc != 4) {
		CLI_LOGI("lcd_qspi_display_picture_test {start|stop} {file name} {device name}\r\n");
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {	
		lcd_qspi_config.src_clk = 5;
		lcd_qspi_config.src_clk_div = 4;
		lcd_qspi_config.clk_div = 10;
		BK_LOG_ON_ERR(bk_qspi_init(&lcd_qspi_config));
		rtos_delay_milliseconds(5);

		uint32_t psram_addr = 0x64000000;

#if (CONFIG_FATFS)
		char cFileName[50];
		FIL file;
		FRESULT fr;
		FSIZE_t size_64bit = 0;
		uint32 uiTemp = 0;
		uint32_t total_size = 0;
		uint8_t *sram_addr = NULL;
		uint32_t once_read_len = READ_SDCARD_PICTURE_SIZE;
		uint32_t *paddr  = NULL;
		paddr = (uint32_t*)psram_addr;
	
		sprintf(cFileName, "%d:/%s", DISK_NUMBER_SDIO_SD, argv[2]);
		sram_addr = os_malloc(once_read_len);
		if (sram_addr == NULL) {
			os_printf("sram mem malloc failed!\r\n");
			return;
		}
		char *ucRdTemp = (char *)sram_addr;
	
		/*open pcm file*/
		fr = f_open(&file, cFileName, FA_OPEN_EXISTING | FA_READ);
		if (fr != FR_OK) {
			os_printf("open %s fail.\r\n", cFileName);
			return;
		}
		size_64bit = f_size(&file);
		total_size = (uint32_t)size_64bit;// total byte
		os_printf("read file total_size = %d.\r\n", total_size);
	
		while(1)
		{
			fr = f_read(&file, ucRdTemp, once_read_len, &uiTemp);
			if (fr != FR_OK) {
				os_printf("read file fail.\r\n");
				return;
			}

			if (uiTemp == 0)
			{
				os_printf("read file complete.\r\n");
				break;
			}

			if(once_read_len != uiTemp) {
				if (uiTemp % 4) {
					uiTemp = (uiTemp / 4 + 1) * 4;
				}
				memcpy_word(paddr, (uint32_t *)sram_addr, uiTemp/4);
			} else {
				memcpy_word(paddr, (uint32_t *)sram_addr, once_read_len/4);
				paddr += (once_read_len/4);
			}
		}
		os_free(sram_addr);
	
		fr = f_close(&file);
		if (fr != FR_OK) {
			os_printf("close %s fail!\r\n", cFileName);
			return;
		}
#else
		os_printf("Not support\r\n");
#endif

	lcd_qspi_device_t *device_config = NULL;
	
	device_config = bk_lcd_qspi_get_device_by_name(argv[3]);
	if (device_config == NULL) {
		os_printf("qspi oled device not found\r\n");
	} else {
		bk_lcd_qspi_init(device_config);
		bk_lcd_qspi_send_data(device_config, (uint32_t *)psram_addr, total_size);
	}

	} else if (os_strcmp(argv[1], "stop") == 0) {
//		bk_qspi_sha8601a_deinit();

	} else {
		CLI_LOGI("qspi_oled_display_picture_test {start|stop} {file_name} {psram_addr}\r\n");
	}

}


