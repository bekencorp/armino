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
#include <components/video_transfer.h>
#include <driver/i2c.h>
#include <driver/jpeg_dec.h>
#include "bk_cli.h"
#include "stdio.h"
#include <driver/dma2d.h>
#include <lcd_rgb_demo.h>
#include <lcd_dma2d_config.h>
#if (CONFIG_FATFS)
#include "ff.h"
#include "diskio.h"
#include "test_fatfs.h"
#endif
#include <driver/dvp_camera.h>
#include <driver/pwm.h>
#include <driver/media_types.h>

extern void delay(INT32 num);
#define           LCD_FRAMEADDR    0x60000000   /**<define frame base addr */
#define jpeg_dec_length    (20480*4-1)  //80k


typedef enum {
	READY = 0,           /**<  jpeg deca and display ready */
	MEMCPYING,           /**<  jepg data mem cpying */
	JPEGDE_START,        /**<  jepg dec start */
	JPEGDECING,          /**<  jepg decing */
	DISPLAYING,          /**<  jepg dec complete, lcd display */
	JPEGDED,
}lcd_satus_t;

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
volatile uint8_t jpeg_complete_flag = 0;
static uint8_t  lcd_status = READY;
static volatile uint8_t *display_address = NULL;
static  uint8_t *decode_address = NULL;
extern u64 riscv_get_mtimer(void);

static void lcd_rgb_isr(void)
{
	if(lcd_status == JPEGDED)
	{
		display_address = psram_lcd->display[(!jpeg_frame_id)];
		lcd_driver_set_display_base_addr((uint32_t)display_address);
	}
	lcd_status = READY;
}
static void jpeg_enc_end_of_yuv(jpeg_unit_t id, void *param)
{
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
static void jpeg_enc_end_of_frame_isr(jpeg_unit_t id, void *param)
{
	int err;
	bk_dma_stop(jpeg_dma_id);

	err = bk_jpeg_dec_hw_start(jpeg_dec_length, jpeg_dec_input_buf, jpeg_dec_output_buf);
	if (err != BK_OK) {
		return;
	}
}


//lcd diaplay jpeg decode complete
static void  lcd_rgb_jpeg_isr(void)
{

}

//jpeg decode complete
static void jpeg_dec_end_of_frame_cb(jpeg_dec_res_t *result)
{
	bk_dma_start(jpeg_dma_id);

	lcd_driver_set_display_base_addr((uint32_t)psram_lcd->display[0]);
	bk_lcd_rgb_display_en(1);
}

static bk_err_t dvp_camera_init(jpeg_mode_t mode, media_ppi_t ppi)
{
	int ret = kNoErr;
	jpeg_config_t jpeg_config = {0};
	i2c_config_t i2c_config = {0};
	const dvp_sensor_config_t *current_sensor = NULL;

	// step 1: enbale dvp power
	bk_dvp_camera_power_enable(1);

	bk_jpeg_enc_driver_init();

	// step 2: enable jpeg mclk for i2c communicate with dvp
	bk_jpeg_enc_mclk_enable();

	rtos_delay_milliseconds(5);

	// step 3: init i2c
	i2c_config.baud_rate = I2C_BAUD_RATE_100KHZ;
	i2c_config.addr_mode = I2C_ADDR_MODE_7BIT;
	bk_i2c_init(CONFIG_CAMERA_I2C_ID, &i2c_config);

	current_sensor = bk_dvp_get_sensor_auto_detect();
	if (current_sensor == NULL)
	{
		os_printf("NOT find camera\r\n");
		ret = kParamErr;
		return ret;
	}

	jpeg_config.mode = mode;
	jpeg_config.x_pixel = ppi_to_pixel_x(ppi) / 8;
	jpeg_config.y_pixel = ppi_to_pixel_y(ppi) / 8;
	jpeg_config.vsync = current_sensor->vsync;
	jpeg_config.hsync = current_sensor->hsync;
	jpeg_config.clk = current_sensor->clk;


	bk_jpeg_enc_register_isr(JPEG_VSYNC_NEGEDGE, jpeg_enc_end_of_yuv, NULL);

	if (mode == JPEG_YUV_MODE)
	{
		bk_jpeg_enc_register_isr(JPEG_EOY, jpeg_enc_end_of_yuv, NULL);
	}
	else
	{
		bk_jpeg_enc_register_isr(JPEG_EOF, jpeg_enc_end_of_frame_isr, NULL);
	}

	ret = bk_jpeg_enc_init(&jpeg_config);
	if (ret != kNoErr) {
		os_printf("jpeg init error\n");
		return ret;
	}

	current_sensor->init();
	current_sensor->set_ppi(current_sensor->def_ppi);
	current_sensor->set_fps(current_sensor->def_fps);

	bk_jpeg_enc_set_gpio(JPEG_ENABLE_DATA);

	return ret;
}


static void lcd_rgb_isr_test(void)
{
//	u64 cur_time = riscv_get_mtimer();
//	if ((u64)(cur_time/26000000) % 2 == 0) {
//		lcd_driver_set_display_base_addr(LCD_FRAMEADDR);
//	} else {
//		lcd_driver_set_display_base_addr(LCD_FRAMEADDR2);
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

		bk_lcd_isr_register(RGB_OUTPUT_EOF, lcd_rgb_isr_test);
		bk_lcd_rgb_init(LCD_DEVICE_ST7282, PIXEL_480, PIXEL_272, PIXEL_FMT_RGB565);
		lcd_driver_set_display_base_addr(LCD_FRAMEADDR);
		bk_lcd_rgb_display_en(1);
	} else if (os_strcmp(argv[1], "display_pause") == 0) {
		bk_lcd_rgb_display_en(0);
	}  else if (os_strcmp(argv[1], "display_resume") == 0) {
		lcd_driver_set_display_base_addr(LCD_FRAMEADDR);
		bk_lcd_rgb_display_en(1);
	} else if (os_strcmp(argv[1], "rgb565_partical_display") == 0) {
		//step1 SD card write psram a 640*480 rgb565data
		os_printf("lcd driver init. \r\n");
		bk_lcd_driver_init(LCD_12M);
		os_printf("lcd rgb reg init.\r\n");
		bk_lcd_rgb_init(LCD_DEVICE_ST7282, PIXEL_640, PIXEL_480, PIXEL_FMT_RGB565 );
		bk_lcd_set_partical_display(1, PARTICAL_XS, PARTICAL_XE, PARTICAL_YS, PARTICAL_YE);
		lcd_driver_set_display_base_addr(LCD_FRAMEADDR);
		bk_lcd_rgb_display_en(1);
	}  else if (os_strcmp(argv[1], "yuyv_partical_display") == 0) {
		//step1 SD card write psram a 640*480 yuyvdata
		//maybe should api: bk_lcd_set_pixel_reverse(1);
		os_printf("lcd driver init. \r\n");
		bk_lcd_driver_init(LCD_12M);
		os_printf("lcd rgb reg init.\r\n");
		bk_lcd_rgb_init(LCD_DEVICE_ST7282, PIXEL_640, PIXEL_480, PIXEL_FMT_YUYV);
		bk_lcd_set_partical_display(1, PARTICAL_XS, PARTICAL_XE, PARTICAL_YS, PARTICAL_YE);
		lcd_driver_set_display_base_addr(LCD_FRAMEADDR);
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
		if (bk_i2c_deinit(CONFIG_CAMERA_I2C_ID) !=BK_OK) {
			os_printf("deinit rgb \r\n");
		}
		os_printf("close rgb ok \r\n");
	}
}


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

#if (USE_LCD_REGISTER_CALLBACKS == 1)
	bk_lcd_isr_register(RGB_OUTPUT_EOF, lcd_rgb_jpeg_isr);
#else
	bk_lcd_isr_register(lcd_rgb_jpeg_isr);
#endif


	if (os_strcmp(argv[1], "480p") == 0) {
		xpixel = PIXEL_480;
		ypixel = PIXEL_272;
		bk_lcd_rgb_init(LCD_DEVICE_ST7282, xpixel, ypixel, PIXEL_FMT_VUYY);
		err = dvp_camera_init(JPEG_ENC_MODE, PPI_640X480);
	} else 	if (os_strcmp(argv[1], "720p") == 0) {
		xpixel = PIXEL_1280;
		ypixel = PIXEL_720;
		bk_lcd_rgb_init(LCD_DEVICE_ST7282, xpixel, ypixel, PIXEL_FMT_VUYY);
		err = dvp_camera_init(JPEG_ENC_MODE, PPI_1280X720);
	} else {
		os_printf("NOT SUPPORE pixel \n");
		return;
	}

	if (err != kNoErr)
	{
		os_printf("camera set failed\r\n");
	}

}

void lcd_rgb_display_yuv(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
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

	bk_lcd_driver_init(LCD_20M);

#if (USE_LCD_REGISTER_CALLBACKS == 1)
		bk_lcd_isr_register(RGB_OUTPUT_EOF, lcd_rgb_isr);
#else
		bk_lcd_isr_register(lcd_rgb_isr);
#endif
	lcd_type = os_strtoul(argv[2], NULL, 10) & 0xFFFF;

	if (os_strcmp(argv[1], "480p") == 0)
	{
		if (lcd_type == LCD_DEVICE_ST7282)  //480*272 lcd
		{
			if (os_strcmp(argv[3], "display_partical") == 0)
			{
				os_printf("set display module partical func. \n");
				xpixel = PIXEL_640;
				ypixel = PIXEL_480;
				bk_lcd_rgb_init(lcd_type, xpixel, ypixel, PIXEL_FMT_YUYV);
				bk_lcd_set_partical_display(1,PARTICAL_XS, PARTICAL_XE, PARTICAL_YS, PARTICAL_YE);
				dvp_camera_init(JPEG_YUV_MODE, PPI_640X480);
			}
			else if (os_strcmp(argv[3], "yuv_partical") == 0)
			{
				os_printf("set jpeg yuv module partical func. \n");
				xpixel = PIXEL_480;
				ypixel = PIXEL_272;
				bk_lcd_rgb_init(lcd_type, xpixel, ypixel, PIXEL_FMT_YUYV);
				offset_config.x_partial_offset_l = PARTICAL_XS;
				offset_config.x_partial_offset_r = PARTICAL_XE;
				offset_config.y_partial_offset_l = PARTICAL_YS;
				offset_config.y_partial_offset_r = PARTICAL_YE;
				dvp_camera_init(JPEG_YUV_MODE, PPI_640X480);
				bk_jpeg_enc_partial_display_init(&offset_config);
			}
			else
			{
				os_printf("no partical set jpeg yuv pixel adapt lcd size. \n");
				xpixel = PIXEL_480;
				ypixel = PIXEL_272;
				bk_lcd_rgb_init(lcd_type, xpixel, ypixel, PIXEL_FMT_YUYV);
				dvp_camera_init(JPEG_YUV_MODE, PPI_480X272);
			}
		}
		else
		{
			os_printf("lcd size not support.0:480*RGB*272 \n");
		}
	}
	else  if (os_strcmp(argv[1], "720p") == 0)
	{
		if (lcd_type == LCD_DEVICE_ST7282)	//480*272 lcd
		{
			if (os_strcmp(argv[3], "display_partical") == 0)
			{
				os_printf("set display module partical func. \n");
				xpixel = PIXEL_1280;
				ypixel = PIXEL_720;
				bk_lcd_rgb_init(lcd_type, xpixel, ypixel, PIXEL_FMT_YUYV);
				bk_lcd_set_partical_display(1, PARTICAL_XS, PARTICAL_XE, PARTICAL_YS, PARTICAL_YE);
				dvp_camera_init(JPEG_YUV_MODE, PPI_1280X720);
			}
			else if (os_strcmp(argv[3], "yuv_partical") == 0)
			{
				os_printf("set jpeg yuv module partical func. \n");
				xpixel = PIXEL_480;
				ypixel = PIXEL_272;
				bk_lcd_rgb_init(lcd_type, xpixel, ypixel, PIXEL_FMT_VUYY);
				offset_config.x_partial_offset_l = PARTICAL_XS;
				offset_config.x_partial_offset_r = PARTICAL_XE;
				offset_config.y_partial_offset_l = PARTICAL_YS;
				offset_config.y_partial_offset_r = PARTICAL_YE;
				dvp_camera_init(JPEG_YUV_MODE, PPI_1280X720);
				bk_jpeg_enc_partial_display_init(&offset_config);
			}
			else
			{
				os_printf("no partical set, not support. \n");
			}
		}
		else if (lcd_type == LCD_DEVICE_HX8282)
		{
			if (os_strcmp(argv[3], "display_partical") == 0)
			{
				os_printf("set display module partical func. \n");
				xpixel = PIXEL_1280;
				ypixel = PIXEL_720;
				bk_lcd_rgb_init(lcd_type, xpixel, ypixel, PIXEL_FMT_YUYV);
				bk_lcd_set_partical_display(1, LCD2_PARTICAL_XS, LCD2_PARTICAL_XE, LCD2_PARTICAL_YS, LCD2_PARTICAL_YE);
				dvp_camera_init(JPEG_YUV_MODE, PPI_1280X720);
			}
			else if (os_strcmp(argv[3], "yuv_partical") == 0)
			{
				os_printf("set jpeg yuv module partical func. \n");
				xpixel = PIXEL_1024;
				ypixel = PIXEL_600;
				bk_lcd_rgb_init(lcd_type, xpixel, ypixel, PIXEL_FMT_VUYY);
				offset_config.x_partial_offset_l = LCD2_PARTICAL_XS;
				offset_config.x_partial_offset_r = LCD2_PARTICAL_XE;
				offset_config.y_partial_offset_l = LCD2_PARTICAL_YS;
				offset_config.y_partial_offset_r = LCD2_PARTICAL_YE;
				dvp_camera_init(JPEG_YUV_MODE, PPI_1280X720);
				bk_jpeg_enc_partial_display_init(&offset_config);
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
}

void lcd_rgb_close(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (os_strcmp(argv[1], "yuv") == 0) {
		if (bk_lcd_rgb_deinit() !=BK_OK) {
			os_printf("deinit rgb \r\n");
		}
		os_printf("close rgb ok \r\n");

		if (bk_jpeg_enc_deinit() != BK_OK) {
			os_printf("deinit jpeg enc error\r\n");
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

		if (bk_jpeg_enc_deinit() != BK_OK) {
			os_printf("deinit jpeg enc error\r\n");
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

