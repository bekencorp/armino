#include <common/bk_include.h>
#include <os/mem.h>
#include <os/str.h>
#include <os/os.h>
#include <stdlib.h>
#include <components/system.h>
#include "driver/lcd_disp_types.h"
#include <driver/lcd.h>
#include <driver/dma.h>
#include <driver/gpio.h>
#include <driver/psram.h>
#include "bk_cli.h"
#include "stdio.h"
#include <lcd_dma2d_config.h>
#include <st7796s.h>
//#include <BK7256_RegList.h>
#include "modules/image_scale.h"
#include <driver/dma2d.h>
#include <driver/media_types.h>
#include "lcd_disp_hal.h"
#include <driver/dvp_camera.h>
#include <driver/i2c.h>
#include <driver/jpeg_enc.h>
#include <components/dvp_camera.h>


extern const uint8_t fg_blend_image1[];

#define LCD_FRAMEADDR   0x60000000 /**<define frame base addr */
#define LCD_FRAMEADDR2   0x60200000 /**<define frame base addr */

#define LCD_SIZE_X 320
#define LCD_SIZE_Y 480

#define LCD_POINT_DISPLAY_XS 201
#define LCD_POINT_DISPLAY_XE 203
#define LCD_POINT_DISPLAY_YS 201
#define LCD_POINT_DISPLAY_YE 203

#define I8080_PARTICAL_XS 1
#define I8080_PARTICAL_XE 320
#define I8080_PARTICAL_YS 1 
#define I8080_PARTICAL_YE 480


//normal partical
#define PARTICAL_XS   0x65//101
#define PARTICAL_XE   0xdc//220
#define PARTICAL_YS   0x65//101
#define PARTICAL_YE   0x17c //380

//edge partical
#define EDGE_PARTICAL_XS   1
#define EDGE_PARTICAL_XE   220
#define EDGE_PARTICAL_YS   1
#define EDGE_PARTICAL_YE   380

static uint8_t  lcd_status = READY;
static volatile uint8_t *display_address = NULL;
static  uint8_t *decode_address = NULL;
static volatile uint8_t jpeg_frame_id = 0;
static  volatile uint8_t jpeg_end_flag = 0;

uint8_t g_disp_frame_done_flag = 0;
extern u64 riscv_get_mtimer(void);
extern void delay(INT32 num);


static void lcd_i8080_isr(void)
{
	if(lcd_status == JPEGDED)
	{
		display_address = psram_lcd->display[(!jpeg_frame_id)];
		bk_lcd_set_display_base_addr((uint32_t)display_address);
	}
	lcd_status = READY;
	bk_lcd_8080_ram_write(RAM_WRITE);
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
	bk_lcd_8080_start_transfer(1);
	bk_lcd_8080_ram_write(RAM_WRITE);
}
static void dma2d_lcd_fill_test(uint32_t frameaddr, uint16_t width, uint16_t height, uint32_t color)
{
	dma2d_config_t dma2d_config = {0};

	dma2d_config.init.mode   = DMA2D_R2M;                      /**< Mode Register to Memory */
	dma2d_config.init.color_mode	   = DMA2D_OUTPUT_ARGB8888;  /**< DMA2D Output color mode is ARGB4444 (16 bpp) */
	dma2d_config.init.output_offset  = 0;                      /**< offset in output */
	dma2d_config.init.red_blue_swap   = DMA2D_RB_REGULAR;       /**< No R&B swap for the output image */
	dma2d_config.init.alpha_inverted = DMA2D_REGULAR_ALPHA;     /**< No alpha inversion for the output image */
	bk_dma2d_driver_init(&dma2d_config);
	bk_dma2d_start_transfer(&dma2d_config, color, (uint32_t)frameaddr, width, height);
	while (bk_dma2d_is_transfer_busy()) {
	}
}


static void cpu_lcd_fill_test(uint32_t *addr, uint32_t color)
{
	uint32_t *p_addr = addr;
	for(int i=0; i<340*480; i++)
	{
		*(p_addr + i) = color;
	}
}


static void lcd_i8080_isr_test(void)
{}

void lcd_8080_display_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	bk_err_t ret = BK_OK;
	uint32_t ARR_DISP_partial_3x3[9] = {0xf800, 0xf800, 0xf800, 0xf800, 0xf800, 0xf800, 0xf800, 0xf800, 0xf800};

	if (os_strcmp(argv[1], "dma2d_fill") == 0)
	{
		os_printf("psram init \r\n");
		bk_psram_init();
		uint32_t frameaddr = os_strtoul(argv[2], NULL, 16) & 0xFFFFFFFF;
		uint32_t width = os_strtoul(argv[3], NULL, 10) & 0xFFFF;
		uint32_t high = os_strtoul(argv[4], NULL, 10) & 0xFFFF;
		uint32_t color = os_strtoul(argv[5], NULL, 16) & 0xFFFFFFFF;

		os_printf("frameaddr= %x \r\n", frameaddr);
		os_printf("(width, high) = (%x %x ) \r\n", width, high);
		os_printf("color = %x \r\n", color);
		dma2d_lcd_fill_test(frameaddr,width, high, color);
		os_printf("dma2d_lcd_fill ok \r\n");
	}  else if (os_strcmp(argv[1], "cpu_fill") == 0) {
		bk_psram_init();
		uint32_t addr = os_strtoul(argv[2], NULL, 16) & 0xffffffff;
		os_printf("addr = (%x\r\n", addr);
		uint32_t color = os_strtoul(argv[3], NULL, 16) & 0xffffffff;
		os_printf("color = (%x\r\n", color);
		cpu_lcd_fill_test((uint32_t*) addr, color);
	} else if (os_strcmp(argv[1], "anticlockwise_rotate") == 0) {
		bk_psram_init();
		unsigned char *pSrcImg = (unsigned char *) 0x60200000;
		unsigned char *pDstImg = (unsigned char *) 0x60000000;
		
		//addAON_GPIO_Reg0x3 = 2;
		image_16bit_rotate90_anticlockwise(pDstImg, pSrcImg, 480, 320);
		//addAON_GPIO_Reg0x3 = 0;
	} else if (os_strcmp(argv[1], "clockwise_rotate") == 0) {
		
		bk_psram_init();
		uint32_t src_width = os_strtoul(argv[2], NULL, 10) & 0xFFFF;
		uint32_t src_high = os_strtoul(argv[3], NULL, 10) & 0xFFFF;
		unsigned char *pSrcImg = (unsigned char *) 0x60200000;
		unsigned char *pDstImg = (unsigned char *) 0x60000000;
		
		//addAON_GPIO_Reg0x3 = 2;
		image_16bit_rotate90_clockwise(pDstImg, pSrcImg,src_width, src_high);
		//addAON_GPIO_Reg0x3 = 0;
	} else if (os_strcmp(argv[1], "vuyy_rotate") == 0) {
		uint32_t srcaddr = os_strtoul(argv[4], NULL, 16) & 0xFFFFFFFF;
		uint32_t dstaddr = os_strtoul(argv[5], NULL, 16) & 0xFFFFFFFF;
		unsigned char *pSrcImg = (unsigned char *) srcaddr;
		unsigned char *pDstImg = (unsigned char *) dstaddr;
		uint32_t src_width = os_strtoul(argv[2], NULL, 10) & 0xFFFF;
		uint32_t src_high = os_strtoul(argv[3], NULL, 10) & 0xFFFF;
		//addAON_GPIO_Reg0x3 = 2;
		vuyy_rotate_degree90(pSrcImg, pDstImg, src_width, src_high); 
		//addAON_GPIO_Reg0x3 = 0;
	} else if (os_strcmp(argv[1], "yuyv_rotate") == 0) {
		uint32_t srcaddr = os_strtoul(argv[4], NULL, 16) & 0xFFFFFFFF;
		uint32_t dstaddr = os_strtoul(argv[5], NULL, 16) & 0xFFFFFFFF;
		unsigned char *pSrcImg = (unsigned char *) srcaddr;
		unsigned char *pDstImg = (unsigned char *) dstaddr;
		uint32_t src_width = os_strtoul(argv[2], NULL, 10) & 0xFFFF;
		uint32_t src_high = os_strtoul(argv[3], NULL, 10) & 0xFFFF;
		//addAON_GPIO_Reg0x3 = 2;
		yuyv_rotate_degree90(pSrcImg, pDstImg, src_width, src_high); 
		//addAON_GPIO_Reg0x3 = 0;
	} else if (os_strcmp(argv[1], "init") == 0) {
		bk_psram_init();
		uint32_t clk = os_strtoul(argv[2], NULL, 16) & 0xffff;
		os_printf("lcd driver init. \r\n");
		uint32_t yuv_mode = os_strtoul(argv[3], NULL, 16) & 0xffff;
		os_printf("lcd driver init. \r\n");
		ret = bk_lcd_driver_init(clk);
		if (ret != BK_OK) {
			os_printf("bk_lcd_driver_init failed\r\n");
			return;
		}
		os_printf("i8080 lcd init. \r\n");
		ret = bk_lcd_8080_init(PIXEL_320, PIXEL_480, yuv_mode);
		if (ret != BK_OK) {
			os_printf("bk_lcd_8080_initinit failed\r\n");
			return;
		}
		ret = bk_lcd_isr_register(I8080_OUTPUT_EOF, lcd_i8080_isr_test);
		if (ret != BK_OK) {
			os_printf("bk_lcd_isr_register failed\r\n");
			return;
		}	
		ret = st7796s_init();
		if (ret != BK_OK) {
			os_printf("st7796s init failed\r\n");
			return;
		}
		os_printf("st7796 init ok. \r\n");
	} else if (os_strcmp(argv[1], "frame_disp") == 0) {
		uint32_t frameaddr = os_strtoul(argv[2], NULL, 16) & 0xFFFFFFFF;
		bk_lcd_set_display_base_addr(frameaddr);
		os_printf("bk_lcd_8080_start_transfer \r\n");
		bk_lcd_8080_start_transfer(1);
		os_printf("bk_lcd_8080_ram_write \r\n");
		bk_lcd_8080_ram_write(RAM_WRITE);
	} else if (os_strcmp(argv[1], "disp_continue") == 0) {
		os_printf("bk_lcd_8080_ram_write \r\n");
		bk_lcd_8080_ram_write(CONTINUE_WRITE);
	} else if (os_strcmp(argv[1], "point_display") == 0) {
		os_printf("close 8080 transfer \r\n");
		bk_lcd_8080_start_transfer(0);
		st7796s_set_display_mem_area(LCD_POINT_DISPLAY_XS, LCD_POINT_DISPLAY_XE, LCD_POINT_DISPLAY_YS,LCD_POINT_DISPLAY_YE);
		os_printf("size = %x %p\r\n", (sizeof(ARR_DISP_partial_3x3)/sizeof(ARR_DISP_partial_3x3[0])), ARR_DISP_partial_3x3);
		bk_lcd_8080_send_cmd((sizeof(ARR_DISP_partial_3x3)/sizeof(ARR_DISP_partial_3x3[0])), 0x2c, ARR_DISP_partial_3x3);
		os_printf(" bk_lcd_8080_send_cmd OK \r\n");
	} else if (os_strcmp(argv[1], "partical_display") == 0) {
		bk_lcd_set_partical_display(PARTICAL_XS, PARTICAL_XE, PARTICAL_YS, PARTICAL_YE);
		st7796s_set_display_mem_area(PARTICAL_XS, PARTICAL_XE, PARTICAL_YS, PARTICAL_YE);
		bk_lcd_8080_start_transfer(1);
		bk_lcd_8080_ram_write(RAM_WRITE);
	} else if (os_strcmp(argv[1], "close") == 0) {
		bk_lcd_8080_deinit();
	}
}

void lcd_8080_display_yuv(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int err = kNoErr;
	jpeg_config_t jpeg_config = {0};
	i2c_config_t i2c_config = {0};
	uint32_t fps = 25;
	uint32_t dev = 3; // gc0328c
	uint32_t camera_cfg = 0;
	uint32_t ppi = 481;

	os_printf("psram init. \r\n");
	bk_psram_init();
	
	uint32_t lcd_clk = os_strtoul(argv[1], NULL, 16) & 0xffff;
	BK_LOG_ON_ERR(bk_jpeg_enc_driver_init());
	bk_lcd_driver_init(lcd_clk);

	bk_lcd_isr_register(I8080_OUTPUT_EOF, lcd_i8080_isr);
	jpeg_config.sys_clk_div = 4;
	jpeg_config.mclk_div = 0;
	jpeg_config.x_pixel = X_PIXEL_320;
	jpeg_config.y_pixel = Y_PIXEL_480;
	bk_lcd_8080_init(PIXEL_320, PIXEL_480, ORGINAL_YUYV_DATA);
	err = st7796s_init();
	if (err != BK_OK) {
		os_printf("st7796s init failed\r\n");
		return;
	}
	os_printf("st7796 init ok. \r\n");

	bk_jpeg_enc_register_isr(END_OF_YUV, jpeg_enc_end_of_yuv, NULL);
	jpeg_config.yuv_mode = 1;
	camera_cfg = (ppi << 16) | fps;
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

void lcd_8080_display_480p_yuv(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int err = kNoErr;
	jpeg_config_t jpeg_config = {0};
	i2c_config_t i2c_config = {0};
	uint32_t fps = 25;
	uint32_t dev = 3; // gc0328c
	uint32_t camera_cfg = 0;

	os_printf("psram init. \r\n");
	bk_psram_init();
	BK_LOG_ON_ERR(bk_jpeg_enc_driver_init());
	
	uint32_t lcd_clk = os_strtoul(argv[1], NULL, 16) & 0xffff;
	bk_lcd_driver_init(lcd_clk);

	bk_lcd_isr_register(I8080_OUTPUT_EOF, lcd_i8080_isr);
	jpeg_config.sys_clk_div = 4;
	jpeg_config.mclk_div = 0;
	os_printf("no partical set jpeg yuv pixel adapt lcd size. \n");
	jpeg_config.x_pixel = X_PIXEL_640;
	jpeg_config.y_pixel = Y_PIXEL_480;
	bk_lcd_8080_init(PIXEL_640, PIXEL_480, ORGINAL_YUYV_DATA);
	bk_lcd_set_display_base_addr((uint32_t)psram_lcd->display[0]);
	bk_lcd_set_partical_display(I8080_PARTICAL_XS, I8080_PARTICAL_XE, I8080_PARTICAL_YS, I8080_PARTICAL_YE);
	err = st7796s_init();
	if (err != BK_OK) {
		os_printf("st7796s init failed\r\n");
		return;
	}
	os_printf("st7796 init ok. \r\n");

	bk_jpeg_enc_register_isr(END_OF_YUV, jpeg_enc_end_of_yuv, NULL);
	jpeg_config.yuv_mode = 1;
	camera_cfg = (PIXEL_480 << 16) | fps;
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

