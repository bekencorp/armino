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
#include <lcd_demo.h>
#include <st7796s.h>
#include "bk_cli.h"
#include "stdio.h"
//#include <BK7256_RegList.h>
#include <driver/dma2d.h>
#include "modules/image_scale.h"

#if (CONFIG_SDCARD_HOST)
#include "ff.h"
#include "diskio.h"
#include "test_fatfs.h"
#endif

static dma_transfer_t s_dma_transfer_param = {0};
static uint32_t*      jpeg_dec_buff =  (uint32_t *)0x60000000;
static uint32_t*      jpeg_buff = (uint32_t *)0x30000000;
dma_id_t lcd_dma_id = DMA_ID_MAX;
dma_id_t jpeg_dma_id = DMA_ID_MAX;

static void lcd_i8080_isr(void)
{
//	bk_gpio_set_output_high(GPIO_2);
#if(1)
	bk_lcd_rgb_display_en(0);
	bk_lcd_pixel_config(X_PIXEL_8080, Y_PIXEL_8080);
	bk_lcd_rgb_display_en(1);
	bk_lcd_8080_write_cmd(RAM_WRITE);
#else
	//use this appear asic bug, nest resion modify
	bk_lcd_8080_write_cmd(continue_write);
#endif
//	bk_gpio_set_output_low(GPIO_2);
}

static void lcd_rgb_isr(void)
{
	s_dma_transfer_param.lcd_isr_cnt++;
	if(s_dma_transfer_param.lcd_isr_cnt == 400) {
		s_dma_transfer_param.lcd_isr_cnt = 0;
		bk_dma_stop(lcd_dma_id);
	}
}

static void jpeg_enc_end_of_yuv_cb(jpeg_unit_t id, void *param)
{
	//bk_gpio_set_output_high(GPIO_2);

	BK_LOG_ON_ERR(bk_dma_start(lcd_dma_id));
	//bk_gpio_set_output_low(GPIO_2);
}


static void jpeg_enc_end_of_frame_cb(jpeg_unit_t id, void *param)
{
	//bk_gpio_set_output_high(GPIO_2);
	//jpeg enc off
	//bk_jpeg_enc_set_enable(0);
	bk_dma_stop(jpeg_dma_id);

	//jpeg dec on
	bk_jpeg_dec_init(jpeg_buff, jpeg_dec_buff);

	//bk_gpio_set_output_low(GPIO_2);
}


static void jpeg_dec_end_of_frame_cb()
{
	//lcd display open
	bk_dma_start(lcd_dma_id);

	//jpeg enc open
	//bk_jpeg_enc_set_enable(1);
	bk_dma_start(jpeg_dma_id);
}


static void dma_finish_isr(dma_id_t id)
{
	//bk_gpio_set_output_high(GPIO_3);
	s_dma_transfer_param.dma_int_cnt ++;
	if (s_dma_transfer_param.dma_int_cnt == s_dma_transfer_param.dma_transfer_cnt)
	{
		s_dma_transfer_param.dma_int_cnt = 0;
		bk_dma_set_src_start_addr(lcd_dma_id, (uint32_t)LCD_FRAMEADDR);
		s_dma_transfer_param.dma_frame_end_flag = 1;
		//BK_LOG_ON_ERR(bk_dma_start(lcd_dma_id));
	}
	else {
		bk_dma_set_src_start_addr(lcd_dma_id, ((uint32_t)LCD_FRAMEADDR + (uint32_t)(s_dma_transfer_param.dma_transfer_len * s_dma_transfer_param.dma_int_cnt)));
		BK_LOG_ON_ERR(bk_dma_enable_finish_interrupt(lcd_dma_id));
		bk_dma_start(lcd_dma_id);
	}
	//bk_gpio_set_output_low(GPIO_3);
}

static void dma_pre_config(uint32_t dma_ch)
{
	dma_config_t dma_config = {0};

	dma_config.mode = DMA_WORK_MODE_SINGLE;
	dma_config.chan_prio = 0;
	dma_config.src.dev = DMA_DEV_DTCM;
	dma_config.src.width = DMA_DATA_WIDTH_32BITS;
	dma_config.src.start_addr = (uint32_t) LCD_FRAMEADDR;
	dma_config.src.addr_inc_en = DMA_ADDR_INC_ENABLE;
	dma_config.dst.dev = DMA_DEV_DTCM;
	dma_config.dst.width = DMA_DATA_WIDTH_32BITS;
	dma_config.dst.start_addr = (uint32_t) LCD_FRAMEADDR + 0x50000;
	dma_config.dst.addr_inc_en = DMA_ADDR_INC_ENABLE;

	BK_LOG_ON_ERR(bk_dma_init(dma_ch, &dma_config));
	BK_LOG_ON_ERR(bk_dma_set_transfer_len(dma_ch, 640)); 
	BK_LOG_ON_ERR(bk_dma_register_isr(dma_ch, NULL, dma_finish_isr));
	BK_LOG_ON_ERR(bk_dma_enable_finish_interrupt(dma_ch));
	BK_LOG_ON_ERR(bk_dma_start(dma_ch));
}

static void dma_rgb_color_config(uint32_t dma_ch)
{
	dma_config_t dma_config = {0};

	dma_config.mode = DMA_WORK_MODE_REPEAT;
	dma_config.chan_prio = 0;
	dma_config.src.dev = DMA_DEV_DTCM;
	dma_config.src.width = DMA_DATA_WIDTH_32BITS;
	dma_config.src.start_addr = (uint32_t) LCD_FRAMEADDR;
	dma_config.src.end_addr = (uint32_t) LCD_FRAMEADDR + 80; //if unopen "//"code,data transfer a little, then dma halt
	dma_config.src.addr_loop_en = DMA_ADDR_LOOP_ENABLE; //
	dma_config.src.addr_inc_en = DMA_ADDR_INC_ENABLE;

	dma_config.dst.dev = DMA_DEV_LCD_DATA;
	dma_config.dst.addr_inc_en = DMA_ADDR_INC_ENABLE;   //
	dma_config.dst.addr_loop_en = DMA_ADDR_LOOP_ENABLE; //
	dma_config.dst.width = DMA_DATA_WIDTH_32BITS;
	dma_config.dst.start_addr = (uint32_t) REG_DISP_RGB_FIFO;
	dma_config.dst.end_addr = (uint32_t) REG_DISP_RGB_FIFO + 4; //

	BK_LOG_ON_ERR(bk_dma_init(dma_ch, &dma_config));
	BK_LOG_ON_ERR(bk_dma_set_transfer_len(dma_ch, 80));
}

void dma_rgb_config(uint32_t dma_ch)
{
	dma_config_t dma_config = {0};
	dma_config.mode = DMA_WORK_MODE_SINGLE;
	dma_config.chan_prio = 0;
	dma_config.src.dev = DMA_DEV_DTCM;
	dma_config.src.width = DMA_DATA_WIDTH_32BITS;
	dma_config.src.start_addr = (uint32) LCD_FRAMEADDR;
	dma_config.src.addr_inc_en = DMA_ADDR_INC_ENABLE;
	dma_config.dst.dev = DMA_DEV_LCD_DATA;
	dma_config.dst.width = DMA_DATA_WIDTH_32BITS;
	dma_config.dst.start_addr = (uint32) REG_DISP_RGB_FIFO;

	BK_LOG_ON_ERR(bk_dma_init(dma_ch, &dma_config));
	BK_LOG_ON_ERR(bk_dma_set_transfer_len(dma_ch, s_dma_transfer_param.dma_transfer_len));
	BK_LOG_ON_ERR(bk_dma_register_isr(dma_ch, NULL, dma_finish_isr));
	BK_LOG_ON_ERR(bk_dma_enable_finish_interrupt(dma_ch));
	BK_LOG_ON_ERR(bk_dma_start(dma_ch));
}

void dma_lcd_config(uint8_t is_8080_if, uint32_t dma_ch, uint32_t dma_src_mem_addr,uint32_t dma_dst_width)
{
	dma_config_t dma_config = {0};
	
	dma_config.mode = DMA_WORK_MODE_SINGLE;
	dma_config.chan_prio = 0;
	dma_config.src.dev = DMA_DEV_DTCM;
	dma_config.src.width = DMA_DATA_WIDTH_32BITS;
	dma_config.src.start_addr = (uint32) dma_src_mem_addr;
	dma_config.src.addr_inc_en = DMA_ADDR_INC_ENABLE;
	dma_config.dst.dev = DMA_DEV_LCD_DATA;
	dma_config.dst.width = dma_dst_width;
	if(is_8080_if) //8080 fifo
		dma_config.dst.start_addr = (uint32) REG_DISP_DAT_FIFO;
	else //rgb fifo
		dma_config.dst.start_addr = (uint32) REG_DISP_RGB_FIFO;

	BK_LOG_ON_ERR(bk_dma_init(dma_ch, &dma_config));
	BK_LOG_ON_ERR(bk_dma_set_transfer_len(dma_ch, s_dma_transfer_param.dma_transfer_len));
	BK_LOG_ON_ERR(bk_dma_register_isr(dma_ch, NULL, dma_finish_isr));
	BK_LOG_ON_ERR(bk_dma_enable_finish_interrupt(dma_ch));
}


void dma_jpeg_config(uint32_t dma_ch)
{
	dma_config_t dma_config = {0};
	dma_config.mode = DMA_WORK_MODE_SINGLE;
	dma_config.chan_prio = 0;
	dma_config.src.dev = DMA_DEV_JPEG;
	dma_config.src.width = DMA_DATA_WIDTH_32BITS;
	dma_config.dst.dev = DMA_DEV_DTCM;
	dma_config.dst.width = DMA_DATA_WIDTH_32BITS;
	dma_config.src.start_addr = JPEG_RX_FIFO;
	dma_config.dst.start_addr = (uint32_t)jpeg_buff;
	dma_config.dst.addr_inc_en = DMA_ADDR_INC_ENABLE;
	BK_LOG_ON_ERR(bk_dma_init(dma_ch, &dma_config));

	BK_LOG_ON_ERR(bk_dma_set_transfer_len(dma_ch, 65536));
	BK_LOG_ON_ERR(bk_dma_start(dma_ch));
}

void lcd_help(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	os_printf("lcd cmd include \r\n.");
	os_printf("lcd_8080_init \r\n.");
	os_printf("lcd_video=8,25 \r\n.");
	os_printf("lcd_video_jpeg_dec=13,25 \r\n.");
	os_printf("lcd_close=8080\r\n.");
	os_printf("lcd_close=yuv_display\r\n.");
	os_printf("lcd_close=jpeg_display\r\n.");
}

void lcd_8080_init(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	bk_err_t ret = BK_OK;

	lcd_dma_id = bk_dma_alloc(DMA_DEV_LCD_DATA);
	if ((lcd_dma_id < DMA_ID_0) || (lcd_dma_id >= DMA_ID_MAX)) {
		os_printf("malloc dma fail \r\n");
		return;
	}
	os_printf("malloc lcd dma ch is DMA_ch%x \r\n", lcd_dma_id);

//	bk_gpio_enable_output(GPIO_2);	//output
//	bk_gpio_enable_output(GPIO_3);	//output

	if (os_strcmp(argv[1], "start") == 0) {
		s_dma_transfer_param.dma_transfer_len = 61440;
		s_dma_transfer_param.dma_transfer_cnt = 5;
	
		os_printf("lcd power on. \r\n");
		bk_lcd_power_on_ctrl(1);

		os_printf("lcd driver init. \r\n");
		bk_lcd_driver_init(LCD_96M);

		os_printf("psram init. \r\n");
		bk_psram_init(0x00054043);

		os_printf("i8080 lcd init. \r\n");
		bk_lcd_8080_init(X_PIXEL_8080, Y_PIXEL_8080);

		os_printf("st7796 init. \r\n");
		ret = st7796s_init();
		if (ret != BK_OK) {
			os_printf("st7796s init failed\r\n");
			return;
		}

		bk_lcd_isr_register(I8080_OUTPUT_EOF, lcd_i8080_isr);
		os_printf("i8080 pre dma1 config. \r\n");
		dma_pre_config(lcd_dma_id);

		os_printf("wait dma pre transfer done. \r\n");
		while(s_dma_transfer_param.dma_frame_end_flag == 0);
		s_dma_transfer_param.dma_frame_end_flag = 0;

		os_printf("8080 lcd start transfer. \r\n");
		bk_lcd_8080_start_transfer(1);
		bk_lcd_8080_write_cmd(RAM_WRITE);
	}
	else {
		os_printf("cmd is : lcd_8080_init = start \r\n");
	}

}

void lcd_rgb_color(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t color_datas[20] = {0};
	uint32_t color  = 0;

	lcd_dma_id = bk_dma_alloc(DMA_DEV_LCD_DATA);
	if ((lcd_dma_id < DMA_ID_0) || (lcd_dma_id >= DMA_ID_MAX)) {
		os_printf("malloc dma fail \r\n");
		return;
	}
	os_printf("malloc lcd dma ch is DMA_ch%x \r\n", lcd_dma_id);

	os_printf("lcd power on. \r\n");
	bk_lcd_power_on_ctrl(1);

	os_printf("lcd driver init. \r\n");
	bk_lcd_driver_init(LCD_96M);

	os_printf("psram init. \r\n");
	bk_psram_init(0x00054043);

	color = os_strtoul(argv[1], NULL, 16) & 0xFFFFFFFF;
	for(int i = 0; i< 20; i++) {
		color_datas[i] = color;
	}
	os_printf("display color is %x\r\n", color_datas[19]);
	os_memcpy((void *)jpeg_dec_buff, &color_datas[0], 80);
	
	dma_rgb_color_config(lcd_dma_id);
	bk_lcd_rgb_init(5,X_PIXEL_RGB, Y_PIXEL_RGB, RGB565_DATA);
	bk_lcd_isr_register(RGB_OUTPUT_EOF, lcd_rgb_isr);

	os_printf("lcd rgb disply enable.\r\n");
	bk_lcd_rgb_display_en(1);
	BK_LOG_ON_ERR(bk_dma_start(lcd_dma_id));
}

void lcd_video(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int err = kNoErr;
	uint8_t rgb_clk_div = 0;
	jpeg_config_t jpeg_config = {0};
	i2c_config_t i2c_config = {0};
	uint32_t fps;
	uint32_t psram_mode = 0x00054043;
	uint32_t dev = 3; // gc0328c
	uint32_t camera_cfg = 0;

	s_dma_transfer_param.dma_int_cnt = 0;
	s_dma_transfer_param.dma_transfer_len = 65280;
	s_dma_transfer_param.dma_transfer_cnt = 4;


	lcd_dma_id = bk_dma_alloc(DMA_DEV_LCD_DATA);
	if ((lcd_dma_id < DMA_ID_0) || (lcd_dma_id >= DMA_ID_MAX)) {
		os_printf("malloc dma fail \r\n");
		return;
	}
	os_printf("malloc lcd dma ch is DMA_ch%x \r\n", lcd_dma_id);

	rgb_clk_div = os_strtoul(argv[1], NULL, 10) & 0xFFFF;
	os_printf("rgb_clk_div  = %d \r\n", rgb_clk_div);
	
	fps = os_strtoul(argv[2], NULL, 10) & 0xFFFF;
	os_printf("fps  = %dHz \r\n", fps);
	camera_cfg = (272 << 16) | fps;

	if (argc < 2) {
		os_printf("input param error\n");
		return;
	}
//	bk_gpio_enable_output(GPIO_2);	//output
//	bk_gpio_enable_output(GPIO_3);	//outpu
	BK_LOG_ON_ERR(bk_jpeg_enc_driver_init());

	err = bk_psram_init(psram_mode);
	if (err != kNoErr) {
		os_printf("psram init error\n");
		return;
	}

	os_printf("lcd system core.\r\n");
	bk_lcd_driver_init(LCD_96M);

	os_printf("lcd rgb reg init.\r\n");
	bk_lcd_rgb_init(rgb_clk_div, X_PIXEL_RGB, Y_PIXEL_RGB, ORGINAL_YUYV_DATA);
	bk_lcd_rgb_display_en(1);
	dma_rgb_config(lcd_dma_id);

	jpeg_config.yuv_mode = 1;
	jpeg_config.x_pixel = X_PIXEL_480;
	jpeg_config.y_pixel = Y_PIXEL_272;
	jpeg_config.sys_clk_div = 4;
	jpeg_config.mclk_div = 0;
	/*switch (fps) {
		case 5:
			fps = TYPE_5FPS;
			break;
		case 10:
			fps = TYPE_10FPS;
			break;
		case 20:
			fps = TYPE_20FPS;
			break;
		case 25:
			fps = TYPE_25FPS;
			break;
		case 30:
			fps = TYPE_30FPS;
			break;
		default:
			os_printf("input fps param error\n");
			return;
	}*/

	bk_jpeg_enc_register_isr(END_OF_YUV, jpeg_enc_end_of_yuv_cb, NULL);
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

	//err = bk_camera_set_ppi_fps(VGA_480_272, fps);
	err = bk_camera_set_param(dev, camera_cfg);
	if (err != kNoErr) {
		os_printf("set camera ppi and fps error\n");
		return;
	}

	bk_camera_sensor_config();
}

void lcd_video_jpeg_dec(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int err = kNoErr;
	uint8_t rgb_clk_div = 0;
	jpeg_config_t jpeg_config = {0};
	i2c_config_t i2c_config = {0};
	uint32_t fps;
	uint32_t psram_mode = 0x00054043;
	uint32_t dev = 3;// gc0328c
	uint32_t camera_cfg = 0;

	s_dma_transfer_param.dma_transfer_len = 65280;
	s_dma_transfer_param.dma_transfer_cnt = 4;
	s_dma_transfer_param.dma_frame_end_flag = 0;
	s_dma_transfer_param.dma_int_cnt = 0;

#if (debug)
	bk_gpio_enable_output(GPIO_2);	//output
	bk_gpio_enable_output(GPIO_3);	//outpu
	bk_gpio_enable_output(GPIO_4);	//output
#endif

	err=bk_jpeg_dec_driver_init();
	if (err != BK_OK)
		return;
	os_printf("jpegdec driver init successful.\r\n");

	rgb_clk_div = os_strtoul(argv[1], NULL, 10) & 0xFFFF;
	os_printf("rgb_clk_div  = %d \r\n", rgb_clk_div);
	
	fps = os_strtoul(argv[2], NULL, 10) & 0xFFFF;
	os_printf("fps  = %dHz \r\n", fps);

	jpeg_dma_id = bk_dma_alloc(DMA_DEV_JPEG);
	if ((jpeg_dma_id < DMA_ID_0) || (jpeg_dma_id >= DMA_ID_MAX)) {
		os_printf("malloc jpeg dma fail \r\n");
		return;
	}
	os_printf("malloc jpeg dma ch is DMA_ch%x \r\n", jpeg_dma_id);

	lcd_dma_id = bk_dma_alloc(DMA_DEV_LCD_DATA);
	if ((lcd_dma_id < DMA_ID_0) || (lcd_dma_id >= DMA_ID_MAX)) {
		os_printf("malloc lcd dma fail \r\n");
		return;
	}
	os_printf("malloc lcd dma ch is DMA_ch%x \r\n", lcd_dma_id);

	BK_LOG_ON_ERR(bk_jpeg_enc_driver_init());

	err = bk_psram_init(psram_mode);
	if (err != kNoErr) {
		os_printf("psram init error\n");
		return;
	}

	os_printf("lcd rgb reg init.\r\n");
	bk_lcd_driver_init(LCD_96M);
	bk_lcd_rgb_init(rgb_clk_div, X_PIXEL_RGB, Y_PIXEL_RGB, VUYY_DATA);
	bk_lcd_rgb_display_en(1);

	os_printf("dma_lcd_config \r\n");
	dma_rgb_config(lcd_dma_id);

	bk_jpeg_enc_register_isr(END_OF_FRAME, jpeg_enc_end_of_frame_cb, NULL);
	bk_jpeg_dec_complete_cb(jpeg_dec_end_of_frame_cb, NULL);
	
	os_printf("jpeg enc init.\r\n");
	jpeg_config.yuv_mode = 0;
	jpeg_config.x_pixel = X_PIXEL_480;
	jpeg_config.y_pixel = Y_PIXEL_272;
	jpeg_config.sys_clk_div = 4;
	jpeg_config.mclk_div = 0;
	camera_cfg = (272 << 16) | fps;
	/*
	switch (fps) {
		case 5:
			fps = TYPE_5FPS;
			break;
		case 10:
			fps = TYPE_10FPS;
			break;
		case 20:
			fps = TYPE_20FPS;
			break;
		case 25:
			fps = TYPE_25FPS;
			break;
		case 30:
			fps = TYPE_30FPS;
			break;
		default:
			os_printf("input fps param error\n");
			return;
	}*/

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

	//err = bk_camera_set_ppi_fps(VGA_480_272, fps);
	err = bk_camera_set_param(dev, camera_cfg);
	if (err != BK_OK) {
		os_printf("set camera ppi and fps error\n");
		return;
	}
	bk_camera_sensor_config();
}


void lcd_rgb_init(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	s_dma_transfer_param.dma_int_cnt = 0;
	s_dma_transfer_param.dma_frame_end_flag = 0;
	s_dma_transfer_param.lcd_isr_cnt = 0;
	s_dma_transfer_param.dma_transfer_len = 65280;
	s_dma_transfer_param.dma_transfer_cnt = 4;

	uint32_t rgb_clk_div = os_strtoul(argv[1], NULL, 10) & 0xFFFF;
	os_printf("rgb_clk_div  = %d \r\n", rgb_clk_div);

	uint32_t yuv_mode = os_strtoul(argv[2], NULL, 10) & 0xFFFF;
	os_printf("yuv_mode  = %d \r\n", yuv_mode);

	// if rgb lcd rgb565 datat  dma_dst_w=1 is 16bit, if is yuv data, dma_dst_w=2 is 32bit
	uint32_t dma_dst_w = os_strtoul(argv[3], NULL, 10) & 0xFFFF;
	os_printf("lcd dma_dst_w  = %d \r\n", dma_dst_w);

	os_printf("lcd power on. \r\n");
	bk_lcd_power_on_ctrl(1);
	os_printf("psram init. \r\n");
	bk_psram_init(0x00054043);
	bk_gpio_enable_output(GPIO_2);	//output
	bk_gpio_enable_output(GPIO_3);	//output

	lcd_dma_id = bk_dma_alloc(DMA_DEV_LCD_DATA);
	if ((lcd_dma_id < DMA_ID_0) || (lcd_dma_id >= DMA_ID_MAX)) {
		os_printf("malloc dma fail \r\n");
		return;
	}
	os_printf("malloc lcd dma ch is DMA_ch%x \r\n", lcd_dma_id);

	os_printf("lcd driver init. \r\n");
	bk_lcd_driver_init(LCD_96M);

	os_printf("lcd rgb reg init.\r\n");
	bk_lcd_rgb_init(rgb_clk_div,  X_PIXEL_RGB, Y_PIXEL_RGB, yuv_mode);
	bk_lcd_isr_register(RGB_OUTPUT_EOF, lcd_rgb_isr);
	dma_lcd_config(0, lcd_dma_id, (uint32_t)LCD_FRAMEADDR, dma_dst_w);
}


void lcd_algorithm_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int err = kNoErr;

	if (os_strcmp(argv[1], "sdwrite_from_psram") == 0) {
#if (CONFIG_SDCARD_HOST)
		char *filename = NULL;
		char cFileName[FF_MAX_LFN];
		FIL file;
		FRESULT fr;
		unsigned int uiTemp = 0;

		os_printf(" cmd argv[1] = %s\r\n", argv[1]);

		filename = argv[2]; //saved file name
		os_printf("filename  = %s \r\n", filename);

		uint32_t width = os_strtoul(argv[3], NULL, 10) & 0xFFFF;
		os_printf("image width	= %d \r\n", width);

		uint32_t height = os_strtoul(argv[4], NULL, 10) & 0xFFFF;
		os_printf("image height  = %d \r\n", height);

		uint32_t paddr = os_strtoul(argv[5], NULL, 16) & 0xFFFFFFFF;
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
		os_printf("write LCD data to file successful\r\n");
#else
		os_printf("Not support\r\n");
#endif
	} else if (os_strcmp(argv[1], "sdread_to_psram") == 0) {
#if (CONFIG_SDCARD_HOST)
		char *filename = NULL;
		char cFileName[FF_MAX_LFN];
		FIL file;
		FRESULT fr;
		FSIZE_t size_64bit = 0;
		unsigned int uiTemp = 0;

		os_printf(" cmd argv[1] = %s\r\n", argv[1]);

		filename = argv[2];
		os_printf("filename  = %s \r\n", filename);

		// step 1: read picture from sd to psram
		sprintf(cFileName, "%d:/%s", DISK_NUMBER_SDIO_SD, filename);

		uint32_t paddr = os_strtoul(argv[3], NULL, 16) & 0xFFFFFFFF;
		char *ucRdTemp = (char *)paddr;
		os_printf("write to psram addr:  %x \r\n", ucRdTemp);

		/*open pcm file*/
		fr = f_open(&file, cFileName, FA_OPEN_EXISTING | FA_READ);
		if (fr != FR_OK) {
			os_printf("open %s fail.\r\n", filename);
			return;
		}
		size_64bit = f_size(&file);
		uint32_t total_size = (uint32_t)size_64bit;// total byte
		os_printf("read file total_size = %d.\r\n", total_size);

		fr = f_read(&file, ucRdTemp, total_size, &uiTemp);
		if (fr != FR_OK) {
			os_printf("read file fail.\r\n");
			return;
		}
		os_printf("\r\n");
		fr = f_close(&file);
		if (fr != FR_OK) {
			os_printf("close %s fail!\r\n", filename);
			return;
		}
		os_printf("sd card read from psram ok.\r\n");
#else
		os_printf("Not support\r\n");
#endif
	} else if (os_strcmp(argv[1], "start") == 0) {
		uint32_t src_w = os_strtoul(argv[3], NULL, 10) & 0xFFFF;
		os_printf("image src_w  = %d \r\n", src_w);
		uint32_t src_h = os_strtoul(argv[4], NULL, 10) & 0xFFFF;
		os_printf("image src_h  = %d \r\n", src_h);

		uint32_t dst_w = os_strtoul(argv[5], NULL, 10) & 0xFFFF;
		os_printf("image dst_w  = %d \r\n", dst_w);
		uint32_t dst_h = os_strtoul(argv[6], NULL, 10) & 0xFFFF;
		os_printf("image src_h  = %d \r\n", dst_h);

		uint32_t srcaddr = os_strtoul(argv[7], NULL, 16) & 0xFFFFFFFF;
		uint32_t dstaddr = os_strtoul(argv[8], NULL, 16) & 0xFFFFFFFF;
		unsigned char *pSrcImg = (unsigned char *) srcaddr;
		unsigned char *pDstImg = (unsigned char *) dstaddr;

		bk_gpio_set_output_high(GPIO_2);
		if (os_strcmp(argv[2], "compress_only") == 0){
			err = image_16bit_scaling(pSrcImg, pDstImg, src_w, src_h, dst_w, dst_h);
		} else if (os_strcmp(argv[2], "compress_rotate") == 0) { 
			err = image_16bit_scaling_rotate(pSrcImg, pDstImg, src_w, src_h, dst_w, dst_h);
		} else if (os_strcmp(argv[2], "anticlockwise_rotate") == 0) {
			image_16bit_rotate90_anticlockwise(pDstImg, pSrcImg, src_w, src_h);
		} else if (os_strcmp(argv[2], "clockwise_rotate") == 0) {
			image_16bit_rotate90_clockwise(pDstImg, pSrcImg, src_w, src_h);
		} else if (os_strcmp(argv[2], "crop_compress") == 0) {
			err = image_scale_crop_compress(pSrcImg, pDstImg, src_w, src_h, dst_w, dst_h);
		} else if (os_strcmp(argv[2], "only_crop") == 0) {
			image_center_crop(pSrcImg, pDstImg, src_w, src_h, dst_w, dst_h);
		}  else if (os_strcmp(argv[2], "dma2d_crop") == 0) {
			dma2d_crop_params_t  crop_params;
			crop_params.dst_addr = (uint32_t)pDstImg;
			crop_params.src_addr = (uint32_t)pSrcImg;
			crop_params.x = (src_w - dst_w)/2;
			crop_params.y = (src_h - dst_h)/2;
			crop_params.src_width = src_w;
			crop_params.src_height = src_h;
			crop_params.dst_width = dst_w;
			crop_params.dst_height = dst_h;
			dma2d_crop_image(&crop_params);
		} else if (os_strcmp(argv[2], "crop_compress_rotate") == 0) {
			err = image_scale_crop_compress_rotate(pSrcImg, pDstImg, src_w, src_h, dst_w, dst_h);
		} else if (os_strcmp(argv[2], "uyvy_to_rgb565") == 0) {
			uyvy_to_rgb565_convert(pSrcImg, pDstImg, src_w, src_h);
		} else if (os_strcmp(argv[2], "yuyv_to_rgb565") == 0) {
			yuyv_to_rgb565_convert(pSrcImg, pDstImg, src_w, src_h);
		} else if (os_strcmp(argv[2], "rgb565_to_uyvy") == 0) {
			rgb565_to_uyvy_convert((uint16_t *)pSrcImg, (uint16_t *)pDstImg, src_w, src_h);
		} else if (os_strcmp(argv[2], "rgb565_to_yuyv") == 0) {
			rgb565_to_yuyv_convert((uint16_t *)pSrcImg, (uint16_t *)pDstImg, src_w, src_h);
		} else {
			os_printf("argv[2] error. \r\n");
			return ;
		}
	} else if (os_strcmp(argv[1], "8080_display") == 0) {
		bk_lcd_rgb_display_en(1);
		dma_lcd_config(1, lcd_dma_id,(uint32_t)LCD_FRAMEADDR, 1);
		bk_dma_start(lcd_dma_id);
	} else if (os_strcmp(argv[1], "rgb_display") == 0) {
		bk_lcd_rgb_display_en(1);
		bk_dma_start(lcd_dma_id);
	} else {
	os_printf("argv[1] error. \r\n\r\n");
	}
	bk_gpio_set_output_low(GPIO_2);
	if (err != BK_OK) {
		os_printf("img_down_scale error\n");
		return;
	}
}

static void dma2d_memcpy_rgb565_data(uint32_t src_addr, uint32_t dst_addr, uint32_t src_offset, uint32_t dst_offset, uint32_t x_pixel, uint32_t y_pixel)
{
	dma2d_config_t dma2d_config = {0};
	dma2d_config.init.mode		  = DMA2D_M2M;			   /**< Mode Memory To Memory */
	dma2d_config.init.color_mode	= DMA2D_OUTPUT_RGB565; /**< Output color mode is ARGB4444 : 16 bpp */
	dma2d_config.init.output_offset = dst_offset;				  /**< No offset on output */
	dma2d_config.init.red_blue_swap  = DMA2D_RB_REGULAR;	 /**< No R&B swap for the output image */
	dma2d_config.init.alpha_inverted = DMA2D_REGULAR_ALPHA; /**< No alpha inversion for the output image */
	
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].alpha_mode = DMA2D_NO_MODIF_ALPHA;	  /**< Keep original Alpha from ARGB4444 input */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_alpha = 0xFF;					  /**< Fully opaque */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_color_mode = DMA2D_INPUT_RGB565; 
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_offset = src_offset;					 /**< No offset in input */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].red_blue_swap   = DMA2D_RB_REGULAR;	   /**< No R&B swap for the input image */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].alpha_inverted = DMA2D_REGULAR_ALPHA;   /**< No alpha inversion for the input image */
	
	bk_dma2d_driver_init(&dma2d_config);
	bk_dma2d_layer_config(&dma2d_config, DMA2D_FOREGROUND_LAYER);
	
	bk_dma2d_start_transfer(&dma2d_config, (uint32_t)src_addr, (uint32_t)dst_addr, x_pixel, y_pixel); 
	while (bk_dma2d_is_transfer_busy()) {}
}


void dma2d_crop_image(dma2d_crop_params_t *crop_params)
{
	uint32_t src_offset = crop_params->src_width - crop_params->dst_width;
	uint16_t *src_crop_start_addr = (uint16_t *)crop_params->src_addr + (crop_params->y * crop_params->src_width) + crop_params->x;

	dma2d_config_t dma2d_config = {0};
	dma2d_config.init.mode		  = DMA2D_M2M;			   /**< Mode Memory To Memory */
	dma2d_config.init.color_mode	= DMA2D_OUTPUT_RGB565; /**< Output color mode is ARGB4444 : 16 bpp */
	dma2d_config.init.output_offset = 0;				  /**< No offset on output */
	dma2d_config.init.red_blue_swap  = DMA2D_RB_REGULAR;	 /**< No R&B swap for the output image */
	dma2d_config.init.alpha_inverted = DMA2D_REGULAR_ALPHA; /**< No alpha inversion for the output image */
	
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].alpha_mode = DMA2D_NO_MODIF_ALPHA;	  /**< Keep original Alpha from ARGB4444 input */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_alpha = 0xFF;					  /**< Fully opaque */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_color_mode = DMA2D_INPUT_RGB565; 
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_offset = src_offset;					 /**< No offset in input */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].red_blue_swap   = DMA2D_RB_REGULAR;	   /**< No R&B swap for the input image */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].alpha_inverted = DMA2D_REGULAR_ALPHA;   /**< No alpha inversion for the input image */
	
	bk_dma2d_driver_init(&dma2d_config);
	bk_dma2d_layer_config(&dma2d_config, DMA2D_FOREGROUND_LAYER);
	
	bk_dma2d_start_transfer(&dma2d_config, (uint32_t)src_crop_start_addr, (uint32_t)crop_params->dst_addr, crop_params->dst_width, crop_params->dst_height); 
	while (bk_dma2d_is_transfer_busy()) {}
}

static void dma2d_blend_rgb565_data(void *p_fg_addr, void *p_bg_addr, void *p_dst_addr,
								uint32_t fg_offline, uint32_t bg_offline, uint32_t out_offset,
								uint16_t xsize, uint16_t ysize, int8_t alpha_value)
{
	dma2d_config_t dma2d_config = {0};
	dma2d_config.init.mode= DMA2D_M2M_BLEND;
	dma2d_config.init.color_mode = DMA2D_OUTPUT_RGB565;
	dma2d_config.init.output_offset= out_offset; /**< output offset */
	dma2d_config.init.red_blue_swap   = DMA2D_RB_REGULAR;				 /**< No R&B swap for the output image */
	dma2d_config.init.alpha_inverted = DMA2D_REGULAR_ALPHA; 			/**< No alpha inversion for the output image */

	/**< Foreground layer Configuration */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].alpha_mode = DMA2D_REPLACE_ALPHA;	/**< Keep original Alpha from ARGB4444 input */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_alpha = alpha_value;           /**< 255 : fully opaque */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_color_mode = DMA2D_INPUT_RGB565; /**< Foreground color is RGB565 : 16 bpp */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_offset = fg_offline;					/**< No offset in input */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].red_blue_swap   = DMA2D_RB_REGULAR;	  /**< No R&B swap for the input image */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].alpha_inverted = DMA2D_REGULAR_ALPHA; /**< No alpha inversion for the input image */

	/**< Background layer Configuration */
	dma2d_config.layer_cfg[DMA2D_BACKGROUND_LAYER].alpha_mode  = DMA2D_REPLACE_ALPHA;
	dma2d_config.layer_cfg[DMA2D_BACKGROUND_LAYER].input_alpha	 = 0x80;							/**< 255 : fully opaque */
	dma2d_config.layer_cfg[DMA2D_BACKGROUND_LAYER].input_color_mode = DMA2D_INPUT_RGB565;		   /**< Background format is ARGB8888*/
	dma2d_config.layer_cfg[DMA2D_BACKGROUND_LAYER].input_offset = bg_offline;/**< Background input offset*/
	dma2d_config.layer_cfg[DMA2D_BACKGROUND_LAYER].red_blue_swap	= DMA2D_RB_REGULAR; 			 /**< No R&B swap for the input background image */
	dma2d_config.layer_cfg[DMA2D_BACKGROUND_LAYER].alpha_inverted  = DMA2D_REGULAR_ALPHA;			/**< No alpha inversion for the input background image */

	bk_dma2d_driver_init(&dma2d_config);
	bk_dma2d_layer_config(&dma2d_config, DMA2D_FOREGROUND_LAYER);
	bk_dma2d_layer_config(&dma2d_config, DMA2D_BACKGROUND_LAYER);
	bk_dma2d_start_blending(&dma2d_config, (uint32_t)p_fg_addr, (uint32_t)p_bg_addr, (uint32_t)p_dst_addr, xsize ,ysize);
	while (bk_dma2d_is_transfer_busy()) {}
}



void lcd_8080_yuv_blend_display(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	/*1: read 320*480yuv picture to psram 0x60200000*/
	/*cmd:lcd_8080_scale_down=sdread_to_psram,yuyv_320_480_yuv,0x60200000 */

	/*2: read clock picture rgb565 data to sram 0x30000000*/
	/*cmd:lcd_8080_scale_down=sdread_to_psram,yuyv_120_56_rgb,0x60400000 */

	if (os_strcmp(argv[1], "crop_blend") == 0) {
		uint32_t src_w = 320;
		uint32_t src_h = 480;
		uint32_t dst_w = 100;
		uint32_t dst_h = 60;
		uint32_t addr_temp = dst_w*dst_h*2;  //0x2ee2
		
		unsigned char *p_disp_addr = (unsigned char *) 0x60000000;
		unsigned char *p_bgImg = (unsigned char *) 0x60200000;
		unsigned char *p_fgImg = (unsigned char *) 0x60400000;
		unsigned char *p_crop = (unsigned char *) (p_fgImg + addr_temp);
		unsigned char *p_crop2rgb = (unsigned char *) (p_fgImg + 2 * addr_temp);
		uint32_t alpha_value = os_strtoul(argv[2], NULL, 10) & 0xFFFF;

		uint32_t x = src_w - dst_w;
		uint32_t y = src_h - dst_h;
		uint32_t bg_offset = src_w - dst_w;
		uint16_t *pbg_offstart = (uint16_t *)p_bgImg + (y * src_w) + x;
//		os_printf("pSrcImg = %x \r\n", p_bgImg);
//		os_printf("crop yuv to pDstImg = %x \r\n", p_crop);  ///0x60402ee2
//		os_printf("dma2d memcpy start addr = %x \r\n", pbg_offstart);
		bk_gpio_set_output_high(GPIO_2);

		/*crop yuv -- 0.74ms*/
		dma2d_memcpy_rgb565_data((uint32_t)pbg_offstart, (uint32_t)p_crop, bg_offset, 0, dst_w, dst_h);

		/*yuv to rgb565 --2.88ms*/
		//os_printf("yuv2rgb addr = %x \r\n",p_crop2rgb); //0x60405dc2
		yuyv_to_rgb565_convert(p_crop, p_crop2rgb, dst_w, dst_h);

		/*rgb blend --1.1ms*/
//		os_printf("rgb blend out addr = %x \r\n",p_crop2rgb); //0x60405dc2
		dma2d_blend_rgb565_data(p_fgImg, p_crop2rgb, p_crop2rgb, 0, 0, 0, dst_w, dst_h, alpha_value);

		/*rgb2yuv --1.9ms*/
		rgb565_to_yuyv_convert((uint16_t *)p_crop2rgb, (uint16_t *)p_crop, dst_w, dst_h);

		/*dma2d fill yuv to bg img --0.77ms*/
		dma2d_memcpy_rgb565_data((uint32_t)p_crop, (uint32_t)pbg_offstart, 0, bg_offset, dst_w, dst_h);
		bk_gpio_set_output_low(GPIO_2);

		/*yuv2rgb --73.4ms*/
		yuyv_to_rgb565_convert(p_bgImg, p_disp_addr, src_w, src_h);

		bk_lcd_rgb_display_en(1);  /* 28.8ms*/
		dma_lcd_config(1, lcd_dma_id,(uint32_t)p_disp_addr, 1);
		bk_dma_start(lcd_dma_id);
	}
}

void lcd_rgb_yuv_blend_display(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	/*1. rgb lcd init*/

	/*2: read 320*480yuv picture to psram 0x60000000*/
	/*cmd:lcd_8080_scale_down=sdread_to_psram,yuyv_320_480_yuv,0x60000000 */

	/*3: read clock picture rgb565 data to sram 0x60400000*/
	/*cmd:lcd_8080_scale_down=sdread_to_psram,yuyv_100_60_rgb,0x60400000 */

	/* cmd: lcd_rgb_scale_init=div,yuvmode,dmawidth*/
	if (os_strcmp(argv[1], "crop_blend") == 0) {
		uint32_t src_w = 480;
		uint32_t src_h = 272;
		uint32_t dst_w = 100;
		uint32_t dst_h = 60;
		unsigned char *p_bgImg = NULL;
		unsigned char *p_disp_addr = NULL;
		uint32_t addr_temp = dst_w*dst_h*2;

		if(os_strcmp(argv[4], "rgb_display") == 0) { /* if rgb lcd display rgb565 data */
			p_bgImg = (unsigned char *) 0x60200000;
			p_disp_addr = (unsigned char *) 0x60000000;
		} else {
			p_bgImg = (unsigned char *) 0x60000000;
		}
		unsigned char *p_fgImg = (unsigned char *) 0x60400000;
		unsigned char *p_crop = (unsigned char *) (p_fgImg + addr_temp);
		unsigned char *p_crop2rgb = (unsigned char *) (p_fgImg + 2 * addr_temp);
		uint32_t alpha_value = os_strtoul(argv[2], NULL, 10) & 0xFFFF;

		uint32_t x = src_w - dst_w;
		uint32_t y = src_h - dst_h;
		uint32_t bg_offset = src_w - dst_w;
		uint16_t *pbg_offstart = (uint16_t *)p_bgImg + (y * src_w) + x;

		bk_gpio_set_output_high(GPIO_2);

		/*crop yuv -- 0.74ms*/
		dma2d_memcpy_rgb565_data((uint32_t)pbg_offstart, (uint32_t)p_crop, bg_offset, 0, dst_w, dst_h);

		/*yuv to rgb565 --2.88ms*/
		if (os_strcmp(argv[3], "yuyv") == 0) {/* if bg image is yuyv data*/
			yuyv_to_rgb565_convert(p_crop, p_crop2rgb, dst_w, dst_h);
		} else if (os_strcmp(argv[3], "uyvy") == 0) {
			uyvy_to_rgb565_convert(p_crop, p_crop2rgb, dst_w, dst_h);
		} else {
			os_printf("yuv mode not support. \r\n");		/*rgb blend --1.1ms*/
		}
		dma2d_blend_rgb565_data(p_fgImg, p_crop2rgb, p_crop2rgb, 0, 0, 0, dst_w, dst_h, alpha_value);

		/*rgb2yuv --1.9ms*/
		if (os_strcmp(argv[3], "yuyv") == 0) {
			rgb565_to_yuyv_convert((uint16_t *)p_crop2rgb, (uint16_t *)p_crop, dst_w, dst_h);
		} else if (os_strcmp(argv[3], "uyvy") == 0) {
			rgb565_to_uyvy_convert((uint16_t *)p_crop2rgb, (uint16_t *)p_crop, dst_w, dst_h);
		} else {
			os_printf("yuv mode not support. \r\n");
		}

		/*dma2d fill yuv to bg img --0.77ms*/
		dma2d_memcpy_rgb565_data((uint32_t)p_crop, (uint32_t)pbg_offstart, 0, bg_offset, dst_w, dst_h);
		bk_gpio_set_output_low(GPIO_2);
		if(os_strcmp(argv[4], "rgb_display") == 0) {
			yuyv_to_rgb565_convert(p_bgImg, p_disp_addr, src_w, src_h);
		}
		bk_lcd_rgb_display_en(1);  /* 28.8ms*/
		bk_dma_start(lcd_dma_id);
	}
}


void lcd_close(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	bk_err_t ret = BK_OK;

	os_memset(&s_dma_transfer_param, 0, sizeof(s_dma_transfer_param));

	if (os_strcmp(argv[1], "8080") == 0) {
		os_printf("deinit lcd 8080 \r\n");
		bk_lcd_8080_deinit();
		os_printf("stop lcd dma and free lcd dma ch\r\n");
		bk_dma_deinit(lcd_dma_id);
		ret = bk_dma_free(DMA_DEV_LCD_DATA, lcd_dma_id);
		if (ret == BK_OK) {
			os_printf("free dma: %d success\r\n", lcd_dma_id);
		}

		bk_psram_deinit();
		os_printf("lcd power down. \r\n");
		bk_lcd_power_on_ctrl(0);
		os_printf("lcd 8080 closed. \r\n");
	}
	else if(os_strcmp(argv[1], "color_display") == 0) {
		if (bk_dma_deinit(lcd_dma_id) == BK_OK) {
			os_printf("deinit lcd dma \r\n");
		}
		if (bk_dma_free(DMA_DEV_LCD_DATA, lcd_dma_id) ==  BK_OK) {
			os_printf("free lcd dma: %d success\r\n", lcd_dma_id);
		}
		if (bk_lcd_rgb_deinit() == BK_OK) {
			os_printf("deinit rgb lcd \r\n");
		}
		bk_psram_deinit();
		os_printf("lcd power down. \r\n");
		bk_lcd_power_on_ctrl(0);
		os_printf("rgb lcd closed. \r\n");
	}
	else if (os_strcmp(argv[1], "yuv_display") == 0) {
		if (bk_dma_deinit(lcd_dma_id) == BK_OK) {
			os_printf("deinit lcd dma \r\n");
		}
		if (bk_dma_free(DMA_DEV_LCD_DATA, lcd_dma_id) ==  BK_OK) {
			os_printf("free lcd dma: %d success\r\n", lcd_dma_id);
		}
		if( bk_i2c_deinit(CONFIG_CAMERA_I2C_ID) !=  BK_OK) {
			os_printf("i2c deinit error\n");
		}
		os_printf("I2c deinit ok!\n");
		if (bk_jpeg_enc_dvp_deinit() == BK_OK) {
			os_printf("deinit jpeg enc \r\n");
		}
		if (bk_lcd_rgb_deinit() == BK_OK) {
			os_printf("deinit rgb lcd \r\n");
		}
		if (bk_psram_deinit() == BK_OK) {
			os_printf("rgb lcd closed. \r\n");
		}
	} 
	else if (os_strcmp(argv[1], "jpeg_display") == 0) {
		if (bk_dma_deinit(lcd_dma_id) == BK_OK) {
			os_printf("deinit lcd dma \r\n");
		}
		if (bk_dma_free(DMA_DEV_LCD_DATA, lcd_dma_id) == BK_OK) {
			os_printf("free lcd dma: %d success\r\n", lcd_dma_id);
		}
		if (bk_dma_deinit(jpeg_dma_id) == BK_OK) {
			os_printf("deinit jpeg dma  \r\n");
		}
		if (bk_dma_free(DMA_DEV_JPEG, jpeg_dma_id) == BK_OK) {
			os_printf("free jpeg dma: %d success\r\n", jpeg_dma_id);
		}
		if( bk_i2c_deinit(CONFIG_CAMERA_I2C_ID) !=  BK_OK) {
			os_printf("i2c deinit error\n");
		}
		if (bk_jpeg_enc_dvp_deinit() == BK_OK) {
			os_printf("deinit jpeg enc  \r\n");
		}
		os_printf("I2c deinit ok!\n");
		if (bk_jpeg_dec_driver_deinit() == BK_OK) {
			os_printf("deinit jpeg dec \r\n");
		}
		if (bk_lcd_rgb_deinit() == BK_OK) {
			os_printf("deinit rgb \r\n");
		}
		if (bk_psram_deinit() == BK_OK) {
			os_printf("rgb lcd closed. \r\n");
		}
	} else {
		os_printf("cmd: lcd_close=8080|yuv_display|jpeg_display\r\n");
	}
}

