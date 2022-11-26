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
#include <driver/qspi_oled.h>
#include <driver/psram.h>
#include <driver/dma2d.h> 
#include "gpio_driver.h"
#include <driver/gpio.h>
#include "cli.h"
#if (CONFIG_FATFS)
#include "ff.h"
#include "diskio.h"
#endif


#define READ_SDCARD_PICTURE_SIZE	1024 * 2
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

void cli_qspi_oled_display_pure_color_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	qspi_config_t qspi_oled_config = {0};

	if (argc != 2) {
		CLI_LOGI("qspi_oled_display_pure_color_test {start|stop} \r\n");
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		BK_LOG_ON_ERR(bk_qspi_driver_init());

		qspi_oled_config.src_clk = 5;
		qspi_oled_config.src_clk_div = 4;
		qspi_oled_config.clk_div = 10;
		BK_LOG_ON_ERR(bk_qspi_init(&qspi_oled_config));
		rtos_delay_milliseconds(5);

		bk_qspi_oled_init();
		bk_qspi_oled_quad_write_start();
		bk_qspi_oled_dma2d_fill(454, 454, 0xFF0000);
		bk_qspi_oled_quad_write_stop();
		rtos_delay_milliseconds(10);

		bk_qspi_oled_quad_write_start();
		bk_qspi_oled_dma2d_fill(454, 454, 0x0000FF);
		bk_qspi_oled_quad_write_stop();
	} else if (os_strcmp(argv[1], "stop") == 0) {
		bk_qspi_oled_deinit();
	} else {
		CLI_LOGI("qspi_oled_display_pure_color_test {start|stop} \r\n");
	}
}

void cli_qspi_oled_display_picture_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	qspi_config_t qspi_oled_config = {0};

	if (argc != 4) {
		CLI_LOGI("qspi_oled_display_picture_test {start|stop} {file_name} {psram_addr}\r\n");
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {	
		qspi_oled_config.src_clk = 5;
		qspi_oled_config.src_clk_div = 4;
		qspi_oled_config.clk_div = 10;
		BK_LOG_ON_ERR(bk_qspi_init(&qspi_oled_config));
		rtos_delay_milliseconds(5);

		bk_qspi_oled_init();

		uint32_t psram_addr = os_strtoul(argv[3], NULL, 16) & 0xFFFFFFFF;

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

		bk_qspi_oled_quad_write_start();
		bk_qspi_oled_dma2d_memcpy((uint32_t *)psram_addr, 454, 454);
		bk_qspi_oled_quad_write_stop();
		rtos_delay_milliseconds(10);

	} else if (os_strcmp(argv[1], "stop") == 0) {
		bk_qspi_oled_deinit();
	} else {
		CLI_LOGI("qspi_oled_display_picture_test {start|stop} {file_name} {psram_addr}\r\n");
	}

}


