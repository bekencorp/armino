#include <common/bk_include.h>
#include <os/mem.h>
#include <os/str.h>
#include <os/os.h>
#include <stdlib.h>
#include <components/system.h>
#include "driver/lcd_disp_types.h"
#include <driver/jpeg_enc.h>
#include <driver/lcd.h>
#include <driver/dma.h>
#include <driver/gpio.h>
#include <driver/psram.h>
#include <components/video_transfer.h>
#include <components/dvp_camera.h>
#include <driver/i2c.h>
#include <driver/jpeg_dec.h>
#include "bk_cli.h"
#include "stdio.h"
#include <driver/dma2d.h>
#include <lcd_rgb_demo.h>
#include <lcd_dma2d_config.h>
#if (CONFIG_SDCARD_HOST)
#include "ff.h"
#include "diskio.h"
#include "test_fatfs.h"
#endif
#include <driver/dvp_camera.h>
#include <driver/pwm.h>
#include <driver/media_types.h>

extern void delay(INT32 num);
#define           LCD_FRAMEADDR    0x60000000   /**<define frame base addr */

static volatile uint8_t jpeg_frame_id = 0;

static unsigned char *  jpeg_dec_input_buf  = (unsigned char *)0x30000000;
static unsigned char *  jpeg_dec_output_buf = (unsigned char *)0x60000000;
static dma_id_t   jpeg_dma_id   = DMA_ID_MAX;

//normal partical
#define PARTICAL_XS   101
#define PARTICAL_XE   580
#define PARTICAL_YS   101
#define PARTICAL_YE   372
//1280*720-->1024*600
#define LCD2_PARTICAL_XS   129
#define LCD2_PARTICAL_XE   1152
#define LCD2_PARTICAL_YS   61
#define LCD2_PARTICAL_YE   660

uint32_t line_int_cnt = 0;
volatile uint8_t jpeg_end_flag = 0;
volatile uint8_t jpeg_complete_flag = 0;
static uint8_t  lcd_status = READY;
static volatile uint8_t *display_address = NULL;
static  uint8_t *decode_address = NULL;
extern u64 riscv_get_mtimer(void);

static void lcd_rgb_isr(void)
{
	if(jpeg_end_flag == 1){
		bk_lcd_unmap_uart3_io_to_lcd_func();
	}
	if(lcd_status == JPEGDED)
	{
		display_address = psram_lcd->display[(!jpeg_frame_id)];
		bk_lcd_set_display_base_addr((uint32_t)display_address);
	}
	lcd_status = READY;
}
static void jpeg_enc_end_of_yuv(jpeg_unit_t id, void *param)
{
	if (jpeg_end_flag == 0)
		jpeg_end_flag = 1;

	if (lcd_status == READY) 
	{
		jpeg_frame_id = !jpeg_frame_id;
		decode_address = psram_lcd->display[!(!jpeg_frame_id)];
		bk_jpeg_set_em_base_addr(decode_address);

	}
	lcd_status = JPEGDED;
	bk_lcd_rgb_display_en(1);
}

//jpeg complete
static void jpeg_enc_end_of_frame_isr_480p(jpeg_unit_t id, void *param)
{
	int err;
	bk_dma_stop(jpeg_dma_id);
	
	bk_jpeg_dec_hw_init(JPEGDEC_X_PIXEL_480, PIXEL_272, jpeg_dec_input_buf, jpeg_dec_output_buf);
	err = bk_jpeg_dec_hw_start();
		if (err != BK_OK) {
			return;
		}
}

static void jpeg_enc_end_of_frame_isr_720p(jpeg_unit_t id, void *param)
{
	int err;

	bk_dma_stop(jpeg_dma_id);
	bk_jpeg_dec_hw_init(JPEGDEC_X_PIXEL_720, PIXEL_272, jpeg_dec_input_buf, jpeg_dec_output_buf);
	err = bk_jpeg_dec_hw_start();
		if (err != BK_OK) {
			return;
		}
}

//lcd diaplay jpeg decode complete
static void  lcd_rgb_jpeg_isr(void)
{
}

//jpeg decode complete
static void jpeg_dec_end_of_frame_cb(void)
{
	bk_dma_start(jpeg_dma_id);
	
	bk_lcd_set_display_base_addr((uint32_t)psram_lcd->display[0]);
	bk_lcd_rgb_display_en(1);
}

#define JPEG_DEC_TEST 0
#if (JPEG_DEC_TEST)

static void jpeg_dec_end_of_line_cb(void)
{
	line_int_cnt++;
	os_printf(" jpeg_dec_end_of_line_cb. \r\n");
}

void lcd_jpeg_dec_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int err = kNoErr;
	if (os_strcmp(argv[1], "frame_int_test") == 0) {
		os_printf("psram init. \r\n");
		bk_psram_init();

		err=bk_jpeg_dec_driver_init();
		if (err != BK_OK)
			return;
		os_printf("jpegdec driver init successful.\r\n");
		bk_jpeg_dec_isr_register(DEC_END_OF_FRAME, jpeg_dec_end_of_frame_cb);
		bk_jpeg_dec_hw_init(JPEGDEC_X_PIXEL_480, PIXEL_272, jpeg_dec_input_buf, jpeg_dec_output_buf);
		err= bk_jpeg_dec_hw_start();
		if (err != BK_OK) {
			os_printf("jpeg decode error .\r\n");
			return;
		}
	} else if (os_strcmp(argv[1], "line_int_test") == 0) {
		os_printf("psram init. \r\n");
		bk_psram_init();

		err=bk_jpeg_dec_driver_init();
		if (err != BK_OK)
			return;
		os_printf("jpegdec driver init successful.\r\n");
		bk_jpeg_dec_line_int_en(32);
		bk_jpeg_dec_isr_register(DEC_END_OF_FRAME, jpeg_dec_end_of_frame_cb);
		bk_jpeg_dec_isr_register(DEC_END_OF_LINE_NUM, jpeg_dec_end_of_line_cb);
		bk_jpeg_dec_hw_init(JPEGDEC_X_PIXEL_480, PIXEL_272, jpeg_dec_input_buf, jpeg_dec_output_buf);
		err= bk_jpeg_dec_hw_start();
		if (err != BK_OK) {
			os_printf("jpeg decode error .\r\n");
			return;
		}
	}
}
#endif
#define DISPLAY_TEST 0
#if (DISPLAY_TEST)
static void lcd_rgb_isr(void)
{
//	if(disp_pink){
//		u64 cur_time = riscv_get_mtimer();
//		if ((u64)(cur_time/26000000) % 2 == 0) {
//			bk_lcd_set_display_base_addr(LCD_FRAMEADDR);
//		} else {
//			bk_lcd_set_display_base_addr(LCD_FRAMEADDR2);
//		}
//	}
}

void lcd_rgb_display_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (os_strcmp(argv[1], "rgb565_display") == 0) {
		//step1 SD card write psram a 480*272 RGB565 data
		uint32_t rgb_freq = os_strtoul(argv[2], NULL, 10) & 0xFFFF;
		os_printf("rgb_freq= %x \r\n", rgb_freq);
		switch(rgb_freq) {
			case LCD_320M:
				bk_lcd_driver_init(LCD_320M);
				os_printf(" RGB clk is 320M. \r\n");
				break;
			case LCD_40M:
				bk_lcd_driver_init(LCD_40M);
				os_printf(" RGB clk is 40M. \r\n");
				break;
			case LCD_20M:
				bk_lcd_driver_init(LCD_20M);
				os_printf(" RGB clk is 20M. \r\n");
				break;
			case LCD_60M:
				bk_lcd_driver_init(LCD_60M);
				os_printf(" RGB clk is 60M. \r\n");
				break;
			case LCD_80M:
				bk_lcd_driver_init(LCD_80M);
				os_printf(" RGB clk is 80M. \r\n");
				break;
			case LCD_30M:
				bk_lcd_driver_init(LCD_30M);
				os_printf(" RGB clk is 30M. \r\n");
				break;
			
			case LCD_32M:
				bk_lcd_driver_init(LCD_32M);
				os_printf(" RGB clk is 32M. \r\n");
				break;
			case LCD_12M:
				bk_lcd_driver_init(LCD_12M);
				os_printf(" RGB clk is 12M. \r\n");
				break;
			case LCD_10M:
				bk_lcd_driver_init(LCD_10M);
				os_printf(" RGB clk is 10M. \r\n");
				break;
			case LCD_26M:
				bk_lcd_driver_init(LCD_26M);
				os_printf(" RGB clk is 26M. \r\n");
				break;
			case LCD_8M:
				bk_lcd_driver_init(LCD_8M);
				os_printf(" RGB clk is 8M. \r\n");
				break;
			default:
				break;
		}
		os_printf("lcd rgb reg init.\r\n");
		bk_lcd_isr_register(RGB_OUTPUT_EOF, lcd_rgb_isr);
		bk_lcd_rgb_init(LCD_TYPE_480_272, PIXEL_480, PIXEL_272, RGB565_DATA);
		bk_lcd_set_display_base_addr(LCD_FRAMEADDR);
		bk_lcd_rgb_display_en(1);
	} else if (os_strcmp(argv[1], "display_pause") == 0) {
		bk_lcd_rgb_display_en(0);
	}  else if (os_strcmp(argv[1], "display_resume") == 0) {
		bk_lcd_set_display_base_addr(LCD_FRAMEADDR);
		bk_lcd_rgb_display_en(1);
	} else if (os_strcmp(argv[1], "yuyv_display") == 0) {
		//step1 SD card write psram a 480*272 YUYV data

		os_printf("lcd driver init. \r\n");
		bk_lcd_driver_init(LCD_8M);
		os_printf("lcd rgb reg init.\r\n");
		bk_lcd_rgb_init(LCD_TYPE_480_272, PIXEL_480, PIXEL_272, ORGINAL_YUYV_DATA );
		bk_lcd_set_display_base_addr(LCD_FRAMEADDR);
		bk_lcd_rgb_display_en(1);
	} else if (os_strcmp(argv[1], "rgb565_partical_display") == 0) {
		//step1 SD card write psram a 640*480 rgb565data
		os_printf("lcd driver init. \r\n");
		bk_lcd_driver_init(LCD_8M);

		os_printf("lcd rgb reg init.\r\n");
		bk_lcd_rgb_init(LCD_TYPE_480_272, PIXEL_640, PIXEL_480, RGB565_DATA );
		bk_lcd_set_partical_display(PARTICAL_XS, PARTICAL_XE, PARTICAL_YS, PARTICAL_YE);
		bk_lcd_set_display_base_addr(LCD_FRAMEADDR);
		bk_lcd_rgb_display_en(1);
	}  else if (os_strcmp(argv[1], "yuyv_partical_display") == 0) {
		//step1 SD card write psram a 640*480 yuyvdata
		//maybe should api: bk_lcd_set_pixel_reverse(1);
		os_printf("lcd driver init. \r\n");
		bk_lcd_driver_init(LCD_80M);
		os_printf("lcd rgb reg init.\r\n");
		bk_lcd_rgb_init(POSEDGE_OUTPUT, PIXEL_640, PIXEL_480, ORGINAL_YUYV_DATA );
		bk_lcd_set_partical_display(PARTICAL_XS, PARTICAL_XE, PARTICAL_YS, PARTICAL_YE);
		bk_lcd_set_display_base_addr(LCD_FRAMEADDR);
		bk_lcd_rgb_display_en(1);
	} else if (os_strcmp(argv[1], "close") == 0) {
		if (bk_lcd_rgb_deinit() !=BK_OK) {
			os_printf("deinit rgb \r\n");
		}
		os_printf("close rgb ok \r\n");
		if (bk_jpeg_enc_deinit() !=BK_OK) {
			os_printf("deinit rgb \r\n");
		}
		os_printf("close rgb ok \r\n");
		if (bk_i2c_deinit() !=BK_OK) {
			os_printf("deinit rgb \r\n");
		}
		os_printf("close rgb ok \r\n");
	}
}
#endif


static void dma_jpeg_config(uint32_t dma_ch)
{
	uint32_t jpeg_fifo_addr;

	bk_jpeg_enc_get_fifo_addr(&jpeg_fifo_addr);

	dma_config_t dma_config = {0};
	dma_config.mode = DMA_WORK_MODE_SINGLE;
	dma_config.chan_prio = 0;
	dma_config.src.dev = DMA_DEV_JPEG;
	dma_config.src.width = DMA_DATA_WIDTH_32BITS;
	dma_config.dst.dev = DMA_DEV_DTCM;
	dma_config.dst.width = DMA_DATA_WIDTH_32BITS;
	dma_config.src.start_addr = jpeg_fifo_addr;
	dma_config.dst.start_addr = (uint32_t)jpeg_dec_input_buf;
	dma_config.dst.addr_inc_en = DMA_ADDR_INC_ENABLE;
	BK_LOG_ON_ERR(bk_dma_init(dma_ch, &dma_config));

	BK_LOG_ON_ERR(bk_dma_set_transfer_len(dma_ch, 65536));
	BK_LOG_ON_ERR(bk_dma_start(dma_ch));
}


//test 1 lcd display jpeg
void lcd_rgb_display_jpeg(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int err = kNoErr;
	jpeg_config_t jpeg_config = {0};
	i2c_config_t i2c_config = {0};
	uint32_t fps;
	uint32_t dev = 0; 
	uint32_t camera_cfg = 0;
	uint16_t xpixel = 0;
	uint16_t ypixel = 0;
#if CONFIG_PWM
	BK_LOG_ON_ERR(bk_pwm_driver_init());

	pwm_init_config_t config = {0};

	config.period_cycle = 10;
	config.duty_cycle = 9;
	BK_LOG_ON_ERR(bk_pwm_init(PWM_ID_1, &config));
	BK_LOG_ON_ERR(bk_pwm_start(PWM_ID_1));
#endif
	err=bk_jpeg_dec_driver_init();
	if (err != BK_OK)
		return;
	os_printf("jpegdec driver init successful.\r\n");
	os_printf("psram init. \r\n");
	bk_psram_init();

	jpeg_dma_id = bk_dma_alloc(DMA_DEV_JPEG);
	if ((jpeg_dma_id < DMA_ID_0) || (jpeg_dma_id >= DMA_ID_MAX)) {
		os_printf("malloc jpeg dma fail \r\n");
		return;
	}
	os_printf("malloc jpeg dma ch is DMA_ch%x \r\n", jpeg_dma_id);

	BK_LOG_ON_ERR(bk_jpeg_enc_driver_init());
	bk_jpeg_dec_isr_register(DEC_END_OF_FRAME, jpeg_dec_end_of_frame_cb);

	bk_lcd_driver_init(LCD_20M);
	bk_lcd_isr_register(RGB_OUTPUT_EOF, lcd_rgb_jpeg_isr);
	if (os_strcmp(argv[1], "480p") == 0) {
		xpixel = PIXEL_480;
		ypixel = PIXEL_272;
		dev = 3;// gc0328c
		jpeg_config.x_pixel = X_PIXEL_480;
		jpeg_config.y_pixel = Y_PIXEL_272;
		jpeg_config.sys_clk_div = 4;
		jpeg_config.mclk_div = 0;
		bk_jpeg_enc_register_isr(END_OF_FRAME, jpeg_enc_end_of_frame_isr_480p, NULL);
	} else 	if (os_strcmp(argv[1], "720p") == 0) {
		xpixel = PIXEL_1280;
		ypixel = PIXEL_720;
		dev = 6;
		jpeg_config.x_pixel = X_PIXEL_1280;
		jpeg_config.y_pixel = Y_PIXEL_720;
		jpeg_config.sys_clk_div = 3;
		jpeg_config.mclk_div = 0;
		bk_jpeg_enc_register_isr(END_OF_FRAME, jpeg_enc_end_of_frame_isr_720p, NULL);
	} else {
		os_printf("NOT SUPPORE pixel \n");
	}
	fps = os_strtoul(argv[2], NULL, 10) & 0xFFFF;

	bk_lcd_rgb_init(LCD_TYPE_480_272, xpixel, ypixel, UVYY_DATA);
	
	os_printf("jpeg enc init.\r\n");
	jpeg_config.yuv_mode = 0;
	camera_cfg = (ypixel << 16) | fps;
	err = bk_jpeg_enc_dvp_init(&jpeg_config);
	if (err != BK_OK) {
		os_printf("jpeg init error\r\n");
		return;
	}

	os_printf("jpeg dma config\r\n");
	dma_jpeg_config(jpeg_dma_id);

	i2c_config.baud_rate = 100000;// 400k
	i2c_config.addr_mode = 0;
	err = bk_i2c_init(CONFIG_CAMERA_I2C_ID, &i2c_config);
	if (err != BK_OK) {
		os_printf("i2c init error\r\n");
		return;
	}
	err = bk_camera_set_param(dev, camera_cfg);
	if (err != BK_OK) {
		os_printf("set camera ppi and fps error\n");
		return;
	}
	bk_camera_sensor_config();
}

void lcd_rgb_display_yuv(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int err = kNoErr;
	jpeg_config_t jpeg_config = {0};
	i2c_config_t i2c_config = {0};
	uint32_t fps = 20;
	uint32_t dev = 3; // gc0328c
	uint32_t camera_cfg = 0;
	uint16_t xpixel = 0;
	uint16_t ypixel = 0;
	uint8_t lcd_type = 0;
	 jpeg_partial_offset_config_t offset_config = {0};
	os_printf("psram init. \r\n");
	bk_psram_init();
#if CONFIG_PWM
	pwm_init_config_t config = {0};
	BK_LOG_ON_ERR(bk_pwm_driver_init());

	config.period_cycle = 10;
	config.duty_cycle = 9;
	BK_LOG_ON_ERR(bk_pwm_init(PWM_ID_1, &config));
	BK_LOG_ON_ERR(bk_pwm_start(PWM_ID_1));
#endif
	BK_LOG_ON_ERR(bk_jpeg_enc_driver_init());
	bk_lcd_driver_init(LCD_20M);
	bk_lcd_isr_register(RGB_OUTPUT_EOF, lcd_rgb_isr);
	lcd_type = os_strtoul(argv[2], NULL, 10) & 0xFFFF;

	if (os_strcmp(argv[1], "480p") == 0)
	{
		dev = 3;// gc0328c
		jpeg_config.sys_clk_div = 4;
		jpeg_config.mclk_div = 0;
		if (lcd_type == 0)  //480*272 lcd
		{
			if (os_strcmp(argv[3], "display_partical") == 0)
			{
				os_printf("set display module partical func. \n");
				jpeg_config.x_pixel = X_PIXEL_640;
				jpeg_config.y_pixel = Y_PIXEL_480;
				xpixel = PIXEL_640;
				ypixel = PIXEL_480;
				bk_lcd_rgb_init(lcd_type, xpixel, ypixel, ORGINAL_YUYV_DATA);
				bk_lcd_set_partical_display(PARTICAL_XS, PARTICAL_XE, PARTICAL_YS, PARTICAL_YE);
			}
			else if (os_strcmp(argv[3], "yuv_partical") == 0)
			{
				os_printf("set jpeg yuv module partical func. \n");
				jpeg_config.x_pixel = X_PIXEL_640;
				jpeg_config.y_pixel = Y_PIXEL_480;
				xpixel = PIXEL_480;
				ypixel = PIXEL_272;
				bk_lcd_rgb_init(lcd_type, xpixel, ypixel, ORGINAL_YUYV_DATA);
				offset_config.x_partial_offset_l = PARTICAL_XS;
				offset_config.x_partial_offset_r = PARTICAL_XE;
				offset_config.y_partial_offset_l = PARTICAL_YS;
				offset_config.y_partial_offset_r = PARTICAL_YE;
				bk_jpeg_enc_partial_display_init(&offset_config);
				bk_jpeg_enc_enable_int(JPEG_VSYNC_NEGEDGE_INT);
			}
			else
			{
				os_printf("no partical set jpeg yuv pixel adapt lcd size. \n");
				jpeg_config.x_pixel = X_PIXEL_480;
				jpeg_config.y_pixel = Y_PIXEL_272;
				xpixel = PIXEL_480;
				ypixel = PIXEL_272;
				bk_lcd_rgb_init(lcd_type, xpixel, ypixel, ORGINAL_YUYV_DATA);
			}
		} 
		else 
		{
			os_printf("lcd size not support.0:480*RGB*272 \n");
		}
	}
	else  if (os_strcmp(argv[1], "720p") == 0)
	{
		dev = 6;
		jpeg_config.sys_clk_div = 3;
		jpeg_config.mclk_div = 0;
		jpeg_config.x_pixel = X_PIXEL_1280;
		jpeg_config.y_pixel = Y_PIXEL_720;
		bk_jpeg_enc_set_auxs(3, 0xF);
		if (lcd_type == LCD_TYPE_480_272)	//480*272 lcd
		{
			if (os_strcmp(argv[3], "display_partical") == 0)
			{
				os_printf("set display module partical func. \n");
				xpixel = PIXEL_1280;
				ypixel = PIXEL_720;
				bk_lcd_rgb_init(lcd_type, xpixel, ypixel, ORGINAL_YUYV_DATA);
				bk_lcd_set_partical_display(PARTICAL_XS, PARTICAL_XE, PARTICAL_YS, PARTICAL_YE);
			}
			else if (os_strcmp(argv[3], "yuv_partical") == 0)
			{
				os_printf("set jpeg yuv module partical func. \n");
				xpixel = PIXEL_480;
				ypixel = PIXEL_272;
				bk_lcd_rgb_init(lcd_type, xpixel, ypixel, UYVY_DATA);
				offset_config.x_partial_offset_l = PARTICAL_XS;
				offset_config.x_partial_offset_r = PARTICAL_XE;
				offset_config.y_partial_offset_l = PARTICAL_YS;
				offset_config.y_partial_offset_r = PARTICAL_YE;
				bk_jpeg_enc_partial_display_init(&offset_config);
				bk_jpeg_enc_enable_int(JPEG_VSYNC_NEGEDGE_INT);
			}
			else
			{
				os_printf("no partical set, not support. \n");
			}
		} 
		else if (lcd_type == LCD_TYPE_1024_600) 
		{
			if (os_strcmp(argv[3], "display_partical") == 0)
			{
				os_printf("set display module partical func. \n");
				xpixel = PIXEL_1280;
				ypixel = PIXEL_720;
				bk_lcd_rgb_init(lcd_type, xpixel, ypixel, ORGINAL_YUYV_DATA);
				bk_lcd_set_partical_display(LCD2_PARTICAL_XS, LCD2_PARTICAL_XE, LCD2_PARTICAL_YS, LCD2_PARTICAL_YE);
			}
			else if (os_strcmp(argv[3], "yuv_partical") == 0)
			{
				os_printf("set jpeg yuv module partical func. \n");
				xpixel = PIXEL_1024;
				ypixel = PIXEL_600;
				bk_lcd_rgb_init(lcd_type, xpixel, ypixel, UYVY_DATA);
				offset_config.x_partial_offset_l = LCD2_PARTICAL_XS;
				offset_config.x_partial_offset_r = LCD2_PARTICAL_XE;
				offset_config.y_partial_offset_l = LCD2_PARTICAL_YS;
				offset_config.y_partial_offset_r = LCD2_PARTICAL_YE;
				bk_jpeg_enc_partial_display_init(&offset_config);
				bk_jpeg_enc_enable_int(JPEG_VSYNC_NEGEDGE_INT);
			}
			else
			{
				os_printf("no partical set ,not support. \n");
			}
		}	
	}
	else
	{
		os_printf("not support camera, 720p/480p \n");
	}

	if (os_strcmp(argv[3], "yuv_partical") == 0)
	{
		bk_jpeg_enc_register_isr(VSYNC_NEGEDGE, jpeg_enc_end_of_yuv, NULL);
	} 
	else
	{
		bk_jpeg_enc_register_isr(END_OF_YUV, jpeg_enc_end_of_yuv, NULL);
	}
	jpeg_config.yuv_mode = 1;
	camera_cfg = (ypixel << 16) | fps;
	bk_lcd_set_display_base_addr((uint32_t)psram_lcd->display[0]);
	err = bk_jpeg_enc_dvp_init(&jpeg_config);
	if (err != kNoErr) {
		os_printf("jpeg init error\n");
		return;
	}

	i2c_config.baud_rate = 100000;// 400k
	i2c_config.addr_mode = 0;
	err = bk_i2c_init(CONFIG_CAMERA_I2C_ID, &i2c_config);
	if (err != kNoErr) {
		os_printf("i2c init error\n");
		return;
	}

	err = bk_camera_set_param(dev, camera_cfg);
	if (err != kNoErr) {
		os_printf("set camera ppi and fps error\n");
		return;
	}
	bk_camera_sensor_config();

}

void lcd_rgb_close(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (os_strcmp(argv[1], "yuv") == 0) {
		if (bk_lcd_rgb_deinit() !=BK_OK) {
			os_printf("deinit rgb \r\n");
		}
		os_printf("close rgb ok \r\n");
		
		if (bk_jpeg_enc_dvp_deinit() != BK_OK) {
			os_printf("deinit jpeg enc error\r\n");
		}
		if (bk_jpeg_enc_deinit() !=BK_OK) {
			os_printf("deinit rgb \r\n");
		}
		os_printf("close rgb ok \r\n");
		if (bk_i2c_deinit(CONFIG_CAMERA_I2C_ID) !=BK_OK) {
			os_printf("deinit rgb \r\n");
		}
		os_printf("close rgb ok \r\n");
	}
	else if (os_strcmp(argv[1], "jpeg") == 0) {
		
		if (bk_dma_deinit(jpeg_dma_id) != BK_OK) {
			os_printf("deinit jpeg dma error \r\n");
		}
		if (bk_dma_free(DMA_DEV_JPEG, jpeg_dma_id) != BK_OK) {
			os_printf("free jpeg dma: %d error\r\n", jpeg_dma_id);
		}
		if (bk_lcd_rgb_deinit() !=BK_OK) {
			os_printf("deinit rgb \r\n");
		}
		os_printf("close rgb ok \r\n");
		
		if (bk_jpeg_enc_dvp_deinit() != BK_OK) {
			os_printf("deinit jpeg enc error\r\n");
		}
		if (bk_jpeg_enc_deinit() !=BK_OK) {
			os_printf("deinit rgb \r\n");
		}
		os_printf("close rgb ok \r\n");
		if (bk_i2c_deinit(CONFIG_CAMERA_I2C_ID) !=BK_OK) {
			os_printf("deinit rgb \r\n");
		}
		if( bk_jpeg_dec_driver_deinit() !=  BK_OK) {
			os_printf("jpeg dec deinit error\n");
		}
		os_printf("close rgb ok \r\n");
	}
}

