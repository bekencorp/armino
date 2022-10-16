#include <common/bk_include.h>
#include <os/mem.h>
#include <os/str.h>
#include <os/os.h>
#include <stdlib.h>
#include <components/system.h>
#include <driver/jpeg_enc.h>
#include <driver/lcd.h>
#include <driver/dma.h>
#include <driver/gpio.h>
#include <driver/psram.h>
#include "bk_cli.h"
#include "stdio.h"
#if (CONFIG_SDCARD_HOST)
#include "ff.h"
#include "diskio.h"
#include "test_fatfs.h"
#endif
#include "modules/image_scale.h"
//#include "BK7256_RegList.h"
#include "bk_general_dma.h"
#include <driver/jpeg_dec.h>
#include <driver/jpeg_dec_types.h>
#include <driver/lcd.h>
#include <./lcd/lcd_devices.h>
#if CONFIG_PWM
#include <driver/pwm.h>
#endif

#if (CONFIG_JPEG_DECODE)
#include <components/jpeg_decode.h>
#endif

#if (CONFIG_SDCARD_HOST)
char *g_filename = NULL;
FIL file;
#endif
extern bk_err_t bk_lcd_set_yuv_mode(pixel_format_t input_data_format);

#define TAG "lcd_jpegdec_display"

//extern MINOOR_ITCM void memcpy_word(uint32_t *dst, uint32_t *src, uint32_t size);

typedef enum
{
	LCD_STATE_CLOSE,
	LCD_STATE_OPEN,
} lcd_state_t;

typedef struct
{
	lcd_state_t state;
	uint16_t lcd_size_x;
	uint16_t lcd_size_y;
	uint16_t partical_xs;
	uint16_t partical_xe;
	uint16_t partical_ys;
	uint16_t partical_ye;
} lcd_jpeg_display_t;

lcd_jpeg_display_t lcd_jpeg_display = {0};
lcd_config_t lcd_config = {0};


#if (CONFIG_SDCARD_HOST)
static  __attribute__((section(".itcm_sec_code "))) void memcpy_word(uint32_t *dst, uint32_t *src, uint32_t size)
{
	uint32_t i = 0;

	for (i = 0; i < size; i++)
	{
		dst[i] = src[i];
	}
}
#endif


bk_err_t lcd_sdcard_read_to_mem(char *filename, uint32_t* paddr, uint32_t *total_len)
{
#if (CONFIG_SDCARD_HOST)
	char cFileName[FF_MAX_LFN];
	FIL file;
	FRESULT fr;
	FSIZE_t size_64bit = 0;
	unsigned int uiTemp = 0;
	uint8_t *sram_addr = NULL;
	uint32_t once_read_len = 1024*2;

	// step 1: read picture from sd to psram
	sprintf(cFileName, "%d:/%s", DISK_NUMBER_SDIO_SD, filename);
	sram_addr = os_malloc(once_read_len);
	if (sram_addr == NULL) {
		os_printf("sd buffer malloc failed\r\n");
		return BK_FAIL;
	}

	char *ucRdTemp = (char *)sram_addr;

	/*open pcm file*/
	fr = f_open(&file, cFileName, FA_OPEN_EXISTING | FA_READ);
	if (fr != FR_OK) {
//		os_printf("open %s fail.\r\n", filename);
		return BK_FAIL;
	}
	size_64bit = f_size(&file);
	uint32_t total_size = (uint32_t)size_64bit;// total byte
	os_printf("read file total_size = %d.\r\n", total_size);
	*total_len = total_size;

	while(1)
	{
		fr = f_read(&file, ucRdTemp, once_read_len, &uiTemp);
		if (fr != FR_OK) {
			os_printf("read file fail.\r\n");
			return BK_FAIL;
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
			
			memcpy_word(paddr, (uint32_t *)sram_addr, uiTemp/4);
		} 
		else
		{
			memcpy_word(paddr, (uint32_t *)sram_addr, once_read_len/4);
			paddr += (once_read_len/4);
		}
	}

	os_free(sram_addr);

	fr = f_close(&file);
	if (fr != FR_OK) {
		os_printf("close %s fail!\r\n", filename);
		return BK_FAIL;
	}
#else
		os_printf("Not support\r\n");
#endif

	return BK_OK;
}



static uint32_t get_string_to_ppi(char *string, uint32_t pre)
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

	if (os_strcmp(string, "800X480") == 0)
	{
		value = PPI_800X480;
	}

	return value;
}

static uint32_t get_ppi_from_cmd(int argc, char **argv, uint32_t pre)
{
	int i;
	uint32_t value = pre;

	for (i = 0; i < argc; i++)
	{
		value = get_string_to_ppi(argv[i], pre);

		if (value != pre)
		{
			break;
		}
	}

	return value;
}

static void lcd_complete_callback(void)
{
	bk_lcd_8080_start_transfer(0);
}


static void jpeg_dec_complete_callback(jpeg_dec_res_t *result)
{
	os_printf("the picture w/h	(%d,%d)\n", result->pixel_x, result->pixel_y);

	lcd_disp_framebuf_t lcd_disp = {0};
	lcd_disp.rect.x = 0;
	lcd_disp.rect.y = 0;
	lcd_disp.rect.width = result->pixel_x;
	lcd_disp.rect.height = result->pixel_y;
	lcd_disp.buffer = (uint32_t *)0x60200000;
	bk_lcd_fill_data( lcd_config.device->id, &lcd_disp);
}


void jpeg_dec_display_demo(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	bk_err_t ret = BK_OK;
	char *filename = NULL;
	unsigned char *jpeg_psram = (unsigned char *)0x60000000;
	uint32_t jpeg_len = 0;
	unsigned char *dec_buf = (unsigned char *) 0x60200000;

	if (os_strcmp(argv[1], "file_display") == 0)
	{
	
		if (LCD_STATE_CLOSE == lcd_jpeg_display.state)
		{
			//step 1 sd card jpeg read
			bk_psram_init();
		}

		filename = argv[2];
		os_printf("filename  = %s \r\n", filename);

		lcd_sdcard_read_to_mem(filename, (uint32_t *)jpeg_psram, &jpeg_len);
		os_printf("read jpeg complete\r\n");
		rtos_delay_milliseconds(100);
		if (LCD_STATE_CLOSE == lcd_jpeg_display.state)
		{
#if CONFIG_PWM
			lcd_driver_backlight_init(PWM_ID_1, 100);
#endif
			//step 2 lcd driver init
			media_ppi_t lcd_ppi = get_ppi_from_cmd(argc, argv, PPI_DEFAULT);
			lcd_config.device= get_lcd_device_by_ppi(lcd_ppi);
			lcd_jpeg_display.lcd_size_x =  ppi_to_pixel_x(lcd_config.device->ppi);
			lcd_jpeg_display.lcd_size_y =  ppi_to_pixel_y(lcd_config.device->ppi);
			if ((lcd_ppi == PPI_1024X600) || (lcd_ppi == PPI_480X800) || (lcd_ppi == PPI_480X272))
			{
				bk_lcd_driver_init(LCD_12M);
				bk_lcd_isr_register(RGB_OUTPUT_EOF, lcd_complete_callback);
				bk_lcd_rgb_init(lcd_config.device->id, lcd_jpeg_display.lcd_size_x, lcd_jpeg_display.lcd_size_y, PIXEL_FMT_RGB565);
			}
			else if (lcd_ppi == PPI_320X480)
			{
				bk_lcd_driver_init(LCD_20M);
				bk_lcd_8080_init(lcd_jpeg_display.lcd_size_x, lcd_jpeg_display.lcd_size_y, PIXEL_FMT_YUYV);
				bk_lcd_isr_register(I8080_OUTPUT_EOF, lcd_complete_callback);
				lcd_st7796s_init();
			}
			else
			{
				os_printf("lcd type not support. \r\n");
			}

#if CONFIG_JPEG_DECODE
			bk_lcd_set_yuv_mode(PIXEL_FMT_YUYV);
			ret = bk_jpeg_dec_sw_init();
			if (ret != kNoErr) {
				os_printf("init jpeg_decoder failed\r\n");
				return;
			}
#else
			bk_lcd_set_yuv_mode(PIXEL_FMT_VUYY);
			//hardware jpeg decode
			ret = bk_jpeg_dec_driver_init();
#endif
			lcd_jpeg_display.state = LCD_STATE_OPEN;
		}
#if CONFIG_JPEG_DECODE
		//software decode
		os_printf("start jpeg_dec.\r\n");
		bk_jpeg_dec_sw_register_finish_callback(jpeg_dec_complete_callback);
		ret = bk_jpeg_dec_sw_start(jpeg_len, jpeg_psram, dec_buf, NULL);
		if (ret != kNoErr) {
			os_printf("jpeg_decoder failed\r\n");
			return;
		}
#else
		bk_jpeg_dec_isr_register(DEC_END_OF_FRAME, jpeg_dec_complete_callback);
		os_printf("start hw jpeg_dec.\r\n");
		ret = bk_jpeg_dec_hw_start(jpeg_len, jpeg_psram, dec_buf);
		if (ret != BK_OK)
		{
			os_printf("jpegdec error code %x.\r\n", ret);
			return;
		}
#endif
	}
	else if (os_strcmp(argv[1], "close") == 0)
	{
		bk_jpeg_dec_driver_deinit();
		bk_lcd_rgb_deinit();
		lcd_jpeg_display.state = LCD_STATE_CLOSE;
	}
	else
	{
		os_printf(" lcd cmd fail\r\n");
		return;
	}
}


void sdcard_write_from_mem(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
#if (CONFIG_SDCARD_HOST)
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

		uint32_t paddr = os_strtoul(argv[4], NULL, 16) & 0xFFFFFFFF;
		char *ucRdTemp = (char *)paddr;
		os_printf("read from psram addr = %x \r\n", ucRdTemp);

		//	save data to sdcard
		sprintf(cFileName, "%d:/%s", DISK_NUMBER_SDIO_SD, filename);

		fr = f_open(&file, cFileName, FA_OPEN_APPEND | FA_WRITE);
		if (fr != FR_OK) {
			os_printf("open %s fail.\r\n", filename);
			return;
		}
		fr = f_write(&file, (char *)ucRdTemp, width * height * 2 , &uiTemp);
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
#else
		os_printf("Not support\r\n");
#endif
} 


void sdcard_read_to_psram(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
#if (CONFIG_SDCARD_HOST)
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
			return;
		}
		if(once_read_len != uiTemp)
		{
			if (uiTemp % 4)
			{
				uiTemp = (uiTemp / 4 + 1) * 4;
			}
			memcpy_word(paddr, (uint32_t *)sram_addr, uiTemp/4);
		} 
		else
		{
			memcpy_word(paddr, (uint32_t *)sram_addr, once_read_len/4);
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

#else
	os_printf("Not support\r\n");
#endif
}
