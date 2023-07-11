
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
#include <driver/lcd.h>
#include <driver/gpio.h>
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

static volatile uint8_t g_rott_complete = 0; 

#if (USE_ROTT_REGISTER_CALLBACKS == 1)
void rott_complete_cb_test(void)
{
	LOGD(" rott_complete_cb_test \n");
	g_rott_complete = 1;
}
void rott_cfg_err_cb_test(void)
{
	LOGD(" rott_cfg_err_cb_test \n");
}
void rott_wtmk_cb_test(void)
{
	LOGD(" rott_wtmk_cb_test \n");
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


#if VUYY_1280_720
#define ROTT_XPIXE      1280
#define ROTT_YPIXE      720
#define ROTT_XBLOCK     80
#define ROTT_YBLOCK     60
#define ROTT_BLOCK_NUM  192
#else
#define ROTT_XPIXE      640
#define ROTT_YPIXE      480
#define ROTT_XBLOCK     80
#define ROTT_YBLOCK     60
#define ROTT_BLOCK_NUM  64
#endif

#define RGB565_480_800       0   //also need open dma2d_demo.h RGB565_480_800
#if RGB565_480_800
extern const unsigned char rgb565le_480_800[768000];

#define ROTT_XPIXE      480
#define ROTT_YPIXE      800
#define ROTT_XBLOCK     60
#define ROTT_YBLOCK     80
#define ROTT_BLOCK_NUM  80
#endif
extern void bk_mem_dump_ex(const char * title, unsigned char * data, uint32_t data_len);





/*
case 1:  bypass mode no rotate
rott=init
rott=bypass,yuyv
rott lcd_init,rgb565,640,480
rott lcd_disp 0x60000000,1

case 2:
rott lcd_disp 0x60000000,0
rott rotate 90 vuyy
rott lcd_init,rgb565 480 640
rott lcd_disp 0x60000000,1

case 3:
rott lcd_disp 0x60000000,0
rott rotate 90 rgb565
rott lcd_init,rgb565 480 800
rott lcd_disp 0x60000000,1

*/
void rott_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (os_strcmp(argv[1], "init") == 0)
	{
		bk_psram_init();
		
		bk_gpio_driver_init();
		bk_gpio_enable_output(GPIO_9);
		bk_gpio_enable_output(GPIO_8);
		bk_gpio_pull_down(GPIO_9);
		bk_gpio_pull_down(GPIO_8);

		bk_rott_driver_init();
		bk_rott_int_enable(ROTATE_COMPLETE_INT | ROTATE_CFG_ERR_INT | ROTATE_WARTERMARK_INT, 1);

#if (USE_ROTT_REGISTER_CALLBACKS == 1)
		bk_rott_isr_register(ROTATE_COMPLETE_INT, rott_complete_cb_test);
		bk_rott_isr_register( ROTATE_CFG_ERR_INT, rott_cfg_err_cb_test);
		bk_rott_isr_register(ROTATE_WARTERMARK_INT, rott_wtmk_cb_test);
#else
		bk_rott_isr_register(rott_isr_cb_test);
#endif
		LOGI("rott init ok\r\n");
	}

	/** =================================bypass mode============================*/
	if (os_strcmp(argv[1], "bypass") == 0)
	{
		bk_gpio_set_output_high(GPIO_8);

		uint32_t input_addr = 0x60400000;
		uint32_t output_addr = 0x60000000;

		pixel_format_t input_color_mode = PIXEL_FMT_YUYV;
		if (os_strcmp(argv[2], "yuyv") == 0)
		{
			input_color_mode = PIXEL_FMT_YUYV;
#if YUYV_640_480
			if (os_strcmp(argv[3], "flash") == 0)
				input_addr = (uint32_t)&yuyv_640_480[0];
			else
				os_memcpy((uint8_t *)(input_addr),(uint8_t *)&yuyv_640_480[0], sizeof(yuyv_640_480));
#endif
		}
		else if (os_strcmp(argv[2], "vuyy") == 0)
		{
#if VUYY_640_480
			if (os_strcmp(argv[3], "flash") == 0)
				input_addr = (uint32_t)&vuyy_640_480[0];
			else
				os_memcpy((uint8_t *)(input_addr),(uint8_t *)&vuyy_640_480[0], sizeof(vuyy_640_480));
#endif
#if VUYY_1280_720
			if (os_strcmp(argv[3], "flash") == 0)
				input_addr = (uint32_t)&yuyv_1280_720[0];
			else
				os_memcpy((uint8_t *)(input_addr),(uint8_t *)&yuyv_1280_720[0], sizeof(yuyv_1280_720));
#endif
			input_color_mode = PIXEL_FMT_VUYY;
		}
		else
			input_color_mode = PIXEL_FMT_UNKNOW;

		if (os_strcmp(argv[3], "flash") == 0)
			LOGI("input_addr, output_addr = %p %p  \n",input_addr, output_addr);
		uint16_t picture_xpixel = ROTT_XPIXE;
		uint16_t picture_ypixel = ROTT_YPIXE;
		uint16_t block_xpixel = ROTT_XBLOCK;
		uint16_t block_ypixel = ROTT_YBLOCK;
		uint16_t block_cnt = ROTT_BLOCK_NUM;

		bk_rott_wartermark_block_config(block_cnt >> 1);
		bk_rott_mode_config(ROTT_ONLY_YUV2RGB565);
		bk_rott_input_data_format(input_color_mode);
		bk_rott_block_rotate_config(picture_xpixel, picture_ypixel, block_xpixel, block_ypixel, block_cnt);
		bk_rott_wr_addr_config(input_addr, output_addr);
		bk_rott_enable();

		while (!g_rott_complete);
		g_rott_complete = 0;
		bk_gpio_set_output_low(GPIO_8);
		LOGI("bypass mode complete \n");
	}
	/** =================================rotate mode============================*/
	if (os_strcmp(argv[1], "rotate") == 0)
	{
		int ret;
		rott_config_t rott = {0};

		bk_gpio_set_output_high(GPIO_9);
		rott.input_addr = (uint8_t *)0x60400000;
		if (os_strcmp(argv[2], "90") == 0)
		{
			rott.rot_mode = ROTT_ROTATE90;
			LOGD("rott.rot_mode = ROTATE90_YUV2RGB565\n");
		}
		else if (os_strcmp(argv[2], "270") == 0)
		{
			rott.rot_mode = ROTT_ROTATE270;
			LOGD("rott.rot_mode = ROTATE270_YUV2RGB565\n");
		}
		else
			LOGD(" not support work mode\n");
		
		if (os_strcmp(argv[3], "yuyv") == 0)
		{
#if YUYV_640_480
			if (os_strcmp(argv[4], "flash") == 0)
				rott.input_addr = (void*)&yuyv_640_480[0];
			else
				os_memcpy((uint8_t *)(rott.input_addr),(uint8_t *)&yuyv_640_480[0], sizeof(yuyv_640_480));
#endif
			rott.input_fmt = PIXEL_FMT_YUYV;
			rott.input_flow = ROTT_INPUT_NORMAL;
			rott.output_flow = ROTT_OUTPUT_REVESE_HALFWORD_BY_HALFWORD;
		}
		else if (os_strcmp(argv[3], "vuyy") == 0)
		{
#if VUYY_640_480
			if (os_strcmp(argv[4], "flash") == 0)
				rott.input_addr = (void*)&vuyy_640_480[0];
			else
				os_memcpy((uint8_t *)(rott.input_addr),(uint8_t *)&vuyy_640_480[0], sizeof(vuyy_640_480));
#endif
#if VUYY_1280_720
			if (os_strcmp(argv[4], "flash") == 0)
				rott.input_addr = (void *)&yuyv_1280_720[0];
			else
				os_memcpy((uint8_t *)(rott.input_addr),(uint8_t *)&yuyv_1280_720[0], sizeof(yuyv_1280_720));
#endif
			rott.input_fmt = PIXEL_FMT_VUYY;
			rott.input_flow = ROTT_INPUT_NORMAL;
			rott.output_flow = ROTT_OUTPUT_NORMAL;
		}
		else  if (os_strcmp(argv[3], "rgb565") == 0)  //only rotate not pfc
		{
#if RGB565_480_800
			if (os_strcmp(argv[4], "flash") == 0)
				rott.input_addr = (void*)&rgb565le_480_800[0];
			else
			{
				os_memcpy((uint8_t *)(rott.input_addr),(uint8_t *)&rgb565le_480_800[0], sizeof(rgb565le_480_800));
			}
#endif
			rott.input_fmt = PIXEL_FMT_RGB565_LE;
			rott.input_flow = ROTT_INPUT_REVESE_HALFWORD_BY_HALFWORD;
			rott.output_flow = ROTT_OUTPUT_NORMAL;
		}

		rott.output_addr = (uint8_t *)0x60000000;
		rott.picture_xpixel = ROTT_XPIXE;
		rott.picture_ypixel = ROTT_YPIXE;
		rott.block_xpixel = ROTT_XBLOCK;
		rott.block_ypixel = ROTT_YBLOCK;
		rott.block_cnt = ROTT_BLOCK_NUM;
		ret = rott_config(&rott);
		if (ret != BK_OK)
			LOGE(" rott_config ERR\n");
		bk_rott_enable();

		while (!g_rott_complete);
		g_rott_complete = 0;
		bk_gpio_set_output_low(GPIO_9);
		LOGI("rotate complete \n");
	}
	if (os_strcmp(argv[1], "en") == 0)
	{
		bk_rott_enable();
	}
	if (os_strcmp(argv[1], "lcd_init") == 0)
	{
		uint16_t xsize = ROTT_XPIXE;
		uint16_t ysize = ROTT_YPIXE;
		pixel_format_t fmt = PIXEL_FMT_RGB565;
		if (os_strcmp(argv[2], "rgb565le") == 0)
		{
			fmt = PIXEL_FMT_RGB565_LE;
		}
		else if (os_strcmp(argv[2], "yuyv") == 0)
		{ 
			fmt = PIXEL_FMT_YUYV;
		}
		else if (os_strcmp(argv[2], "vuyy") == 0)
		{
			fmt = PIXEL_FMT_VUYY;
		}
		else
			fmt = PIXEL_FMT_RGB565;
		if ((argv[3] != NULL) && (argv[4] != NULL))
		{
			xsize= os_strtoul(argv[3], NULL, 10) & 0xFFFF;
			ysize = os_strtoul(argv[4], NULL, 10) & 0xFFFF;
		}

		lcd_ldo_power_enable(1);
		bk_lcd_driver_init(LCD_12M);
		bk_lcd_rgb_init(LCD_DEVICE_ST7282, xsize, ysize, fmt);

		uint16_t start_x = 1;
		uint16_t start_y = 1;
		uint16_t end_x = xsize;
		uint16_t end_y = ysize;

		if (xsize > PIXEL_480)
		{
			start_x = (xsize - PIXEL_480) / 2 + 1;
			end_x = start_x + PIXEL_480 - 1;
		}
		if (ysize > PIXEL_272)
		{
			start_y = (ysize - PIXEL_272) / 2 + 1;
			end_y = start_y + PIXEL_272 - 1;
		}
		bk_lcd_set_partical_display(1, start_x, end_x, start_y, end_y);
		LOGI("image exceed then lcd size, offset %d, %d, %d, %d\n", start_x, end_x, start_y, end_y);
		LOGI("lcd_init ok \r\n");
	}
	else if (os_strcmp(argv[1], "lcd_disp") == 0)
	{
		uint32_t base_addr= os_strtoul(argv[2], NULL, 16) & 0xFFFFFFFF;
		uint8_t en= os_strtoul(argv[3], NULL, 10) & 0xFFFF;

		lcd_driver_set_display_base_addr(base_addr);
		bk_lcd_rgb_display_en(en);
	}
}



