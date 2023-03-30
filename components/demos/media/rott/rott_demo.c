
#include <os/mem.h>
#include <os/str.h>
#include <os/os.h>
#include <driver/int.h>
#include <common/bk_err.h>
#include "sdkconfig.h"
#include <driver/psram.h>
#include <stdio.h>

#include <driver/media_types.h>
#include <driver/rott_types.h>
#include <driver/rott_driver.h>
#include "rott_demo.h"

#if CONFIG_FATFS
#include "ff.h"
#include "diskio.h"
#endif


#define TAG "ROTT_DEMO"
#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)


#if (USE_ROTT_REGISTER_CALLBACKS == 1)
void rott_complete_cb_test(void)
{

}
void rott_cfg_err_cb_test(void)
{

}
void rott_wtmk_cb_test(void)
{

}
#else

void rott_isr_cb_test(void)
{
	if( bk_rott_int_status_get() & ROTATE_COMPLETE_INT)
	{
		bk_rott_int_status_clear(ROTATE_COMPLETE_INT);
	}
	if( bk_rott_int_status_get() & ROTATE_WARTERMARK_INT)
	{
		bk_rott_int_status_clear(ROTATE_WARTERMARK_INT);
	}
	if( bk_rott_int_status_get() & ROTATE_CFG_ERR_INT)
	{
		bk_rott_int_status_clear(ROTATE_CFG_ERR_INT);
	}

}
#endif

extern void bk_mem_dump_ex(const char * title, unsigned char * data, uint32_t data_len);


void rott_only_yuv2rgb565_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	bk_psram_init();

	uint32_t input_addr = os_strtoul(argv[1], NULL, 16) & 0xFFFFFFFF;
	uint32_t output_addr = os_strtoul(argv[2], NULL, 16) & 0xFFFFFFFF;
	pixel_format_t input_color_mode;

	if (os_strcmp(argv[3], "INPUT_RGB565") == 0)
	{
		input_color_mode = PIXEL_FMT_RGB565;
	}
	else if (os_strcmp(argv[3], "YUYV") == 0)
	{
		input_color_mode = PIXEL_FMT_YUYV;
	}
	else 
	{
		input_color_mode = PIXEL_FMT_RGB565;
	}

	uint16_t picture_line_pixel = os_strtoul(argv[4], NULL, 10) & 0xFFFF;
	uint16_t picture_clum_pixel = os_strtoul(argv[5], NULL, 10) & 0xFFFF;
	uint16_t block_line_pixel = os_strtoul(argv[6], NULL, 10) & 0xFFFF;
	uint16_t block_clum_pixel = os_strtoul(argv[7], NULL, 10) & 0xFFFF;
	uint16_t block_cnt = os_strtoul(argv[8], NULL, 10) & 0xFFFF;

	bk_rott_driver_init();

	bk_rott_int_enable(ROTATE_COMPLETE_INT | ROTATE_CFG_ERR_INT | ROTATE_WARTERMARK_INT, 1);

	
#if (USE_ROTT_REGISTER_CALLBACKS == 1)
	bk_rott_isr_register(ROTATE_COMPLETE_INT, rott_complete_cb_test);
	bk_rott_isr_register( ROTATE_CFG_ERR_INT, rott_cfg_err_cb_test);
	bk_rott_isr_register(ROTATE_WARTERMARK_INT, rott_wtmk_cb_test);
#else
	bk_rott_isr_register(rott_isr_cb_test);
#endif

	bk_rott_wartermark_block_config(block_cnt << 2);
	bk_rott_mode_config(NO_ROTATE_ONLY_YUV2RGB565);
	bk_rott_input_data_format(input_color_mode);
	bk_rott_input_data_reverse(INPUT_NORMAL, OUTPUT_NORMAL);
	bk_rott_block_rotate_config(picture_line_pixel, picture_clum_pixel, block_line_pixel, block_clum_pixel, block_cnt);
	bk_rott_wr_addr_config(input_addr, output_addr);
	bk_rott_enable(1);
}


void rott_rotate_yuv2rgb565_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{

}

void sdcard_write_from_mem_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
#if CONFIG_FATFS
		char *filename = NULL;
		char cFileName[FF_MAX_LFN];
		FIL file;
		FRESULT fr;
		unsigned int uiTemp = 0;

		filename = argv[1]; //saved file name
		os_printf("filename  = %s \r\n", filename);

		uint32_t width = os_strtoul(argv[2], NULL, 10) & 0xFFFF;
		os_printf("image width	= %d \r\n", width);

		uint32_t height = os_strtoul(argv[3], NULL, 10) & 0xFFFF;
		os_printf("image height  = %d \r\n", height);
		
		uint32_t pixel_bit = os_strtoul(argv[4], NULL, 10) & 0xFFFF;
		os_printf(" pixel_bit  = %d \r\n", pixel_bit);

		uint32_t paddr = os_strtoul(argv[5], NULL, 16) & 0xFFFFFFFF;
		char *ucRdTemp = (char *)paddr;
		os_printf("read from psram addr = %x \r\n", ucRdTemp);

		//	save data to sdcard
		sprintf(cFileName, "%d:/%s", DISK_NUMBER_SDIO_SD, filename);

		fr = f_open(&file, cFileName, FA_CREATE_ALWAYS | FA_WRITE);
		if (fr != FR_OK) {
			os_printf("open %s fail.\r\n", filename);
			return;
		}
		fr = f_write(&file, (char *)ucRdTemp, width * height * pixel_bit , &uiTemp);
		if (fr != FR_OK) {
			os_printf("write %s fail.\r\n", filename);
			return;
		}
		os_printf("\n");
		fr = f_close(&file);
		if (fr != FR_OK) {
			os_printf("close %s fail!\r\n", filename);
			return;
		}
		os_printf("sd card write data to file successful\r\n");
#endif
} 
void sdcard_read_to_mem_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
#if CONFIG_FATFS
	char cFileName[FF_MAX_LFN];
	FIL file;
	FRESULT fr;
	FSIZE_t size_64bit = 0;
	unsigned int uiTemp = 0;
	uint8_t *sram_addr = NULL;
	uint32_t once_read_len = 1024*2;
	char *filename = NULL;
	uint32_t *paddr  = NULL;

	filename = argv[1];
	os_printf("filename  = %s \r\n", filename);
	
	uint32_t psram_addr = os_strtoul(argv[2], NULL, 16) & 0xFFFFFFFF;
	paddr = (uint32_t*)psram_addr;

	// step 1: read picture from sd to psram
	sprintf(cFileName, "%d:/%s", DISK_NUMBER_SDIO_SD, filename);
	sram_addr = os_malloc(once_read_len);
	char *ucRdTemp = (char *)sram_addr;

	/*open pcm file*/
	fr = f_open(&file, cFileName, FA_OPEN_EXISTING | FA_READ);
	if (fr != FR_OK) {
		os_printf("open %s fail.\r\n", filename);
		return;
	}
	size_64bit = f_size(&file);
	uint32_t total_size = (uint32_t)size_64bit;// total byte
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
		if(once_read_len != uiTemp)
		{
			if (uiTemp % 4)
			{
				uiTemp = (uiTemp / 4 + 1) * 4;
			}
			os_memcpy_word(paddr, (uint32_t *)sram_addr, uiTemp);
		} 
		else
		{
			os_memcpy_word(paddr, (uint32_t *)sram_addr, once_read_len);
			paddr += (once_read_len/4);
		}
	}
	os_printf("\r\n");
	os_free(sram_addr);
	fr = f_close(&file);
	if (fr != FR_OK) {
		os_printf("close %s fail!\r\n", filename);
		return;
	}
#endif
}


