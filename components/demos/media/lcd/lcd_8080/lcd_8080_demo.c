#include <common/bk_include.h>
#include <os/mem.h>
#include <os/str.h>
#include <os/os.h>
#include <stdlib.h>
#include <components/system.h>
#include <driver/lcd.h>
#include <driver/dma.h>
#include <driver/gpio.h>
#include <driver/psram.h>
#include "bk_cli.h"
#include "stdio.h"
#include <lcd_dma2d_config.h>
#include <st7796s.h>
#include "modules/image_scale.h"
#include <driver/dma2d.h>
#include <driver/media_types.h>
#include "lcd_disp_hal.h"
#include <driver/dvp_camera.h>
#include <driver/i2c.h>
#include <driver/jpeg_enc.h>

extern const uint8_t fg_blend_image1[];

#define JPEGDEC_FRAME_SIZE  0x200000 
#define DISPLAY_FRAME_COUNTS (2)

#define PSRAM_BASEADDR (0x60000000UL)

typedef enum {
	READY = 0,           /**<  jpeg deca and display ready */
	MEMCPYING,           /**<  jepg data mem cpying */
	JPEGDE_START,        /**<  jepg dec start */
	JPEGDECING,          /**<  jepg decing */
	DISPLAYING,          /**<  jepg dec complete, lcd display */
	JPEGDED,
}lcd_satus_t;

typedef struct
{
	uint8_t display[DISPLAY_FRAME_COUNTS][JPEGDEC_FRAME_SIZE];
} psram_lcd_t;

#define psram_lcd ((psram_lcd_t*)PSRAM_BASEADDR)

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

volatile uint8_t g_disp_frame_done_flag = 0;
extern u64 riscv_get_mtimer(void);
extern void delay(INT32 num);


static void lcd_i8080_isr(void)
{
	os_printf("8080 int \n");
	if(lcd_status == JPEGDED)
	{
		display_address = psram_lcd->display[(!jpeg_frame_id)];
		lcd_driver_set_display_base_addr((uint32_t)display_address);
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

static void dma2d_lcd_fill_test(uint32_t frameaddr, uint16_t width, uint16_t height, uint16_t color)
{
	uint32_t color_temp = (color << 16) | color;
	os_printf("displat color :%x\r\n", color_temp);
	bk_dma2d_driver_init();

	dma2d_config_t dma2d_config = {0};

	dma2d_config.init.mode   = DMA2D_R2M;                      /**< Mode Register to Memory */
	dma2d_config.init.color_mode	   = DMA2D_OUTPUT_ARGB8888;  /**< DMA2D Output color mode is ARGB4444 (16 bpp) */
	dma2d_config.init.output_offset  = 0;                      /**< offset in output */
	dma2d_config.init.red_blue_swap   = DMA2D_RB_REGULAR;       /**< No R&B swap for the output image */
	dma2d_config.init.alpha_inverted = DMA2D_REGULAR_ALPHA;     /**< No alpha inversion for the output image */
	bk_dma2d_init(&dma2d_config);
	if (width == 0)
	{
		height = height/2;
	} 
	else if(height == 0)
	{
		width = width/2;
	}
	else
	{
		width = width/2;
	}
	bk_dma2d_start_transfer(&dma2d_config, color_temp, (uint32_t)frameaddr, width, height);
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

static bk_err_t dvp_camera_init(jpeg_mode_t mode)
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
	jpeg_config.x_pixel = ppi_to_pixel_x(current_sensor->def_ppi) / 8;
	jpeg_config.y_pixel = ppi_to_pixel_y(current_sensor->def_ppi) / 8;
	jpeg_config.vsync = current_sensor->vsync;
	jpeg_config.hsync = current_sensor->hsync;
	jpeg_config.clk = current_sensor->clk;

	if (mode == JPEG_YUV_MODE)
	{
		bk_jpeg_enc_register_isr(JPEG_EOY, jpeg_enc_end_of_yuv, NULL);
	}
	else
	{
		//
	}

	ret = bk_jpeg_enc_init(&jpeg_config);
	if (ret != kNoErr) {
		os_printf("jpeg init error\n");
		return ret;
	}

	current_sensor->init();
	current_sensor->set_ppi(current_sensor->def_ppi);
	current_sensor->set_fps(current_sensor->def_fps);

	bk_jpeg_enc_set_gpio_enable(1, JPEG_GPIO_DATA);

	return ret;
}

static volatile uint8_t lcd_cnt;

static void lcd_i8080_isr_test(void)
{
	g_disp_frame_done_flag=1;
}
extern void bk_mem_dump_ex(const char *title, unsigned char *data, uint32_t data_len);

void lcd_8080_display_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	bk_err_t ret = BK_OK;
	lcd_cnt = 0;

	if (os_strcmp(argv[1], "dma2d_fill") == 0)
	{
		os_printf("psram init \r\n");
		bk_psram_init();
		uint32_t frameaddr = os_strtoul(argv[2], NULL, 16) & 0xFFFFFFFF;
		uint32_t width = os_strtoul(argv[3], NULL, 10) & 0xFFFF;
		uint32_t high = os_strtoul(argv[4], NULL, 10) & 0xFFFF;
		uint32_t color = os_strtoul(argv[5], NULL, 16) & 0xFFFF;

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
	} else if (os_strcmp(argv[1], "init") == 0) {
		bk_psram_init();
		bk_dma2d_driver_init();
		os_printf("lcd driver init. \r\n");
		os_printf("lcd driver init. \r\n");
		ret = bk_lcd_driver_init(LCD_20M);
		if (ret != BK_OK) {
			os_printf("bk_lcd_driver_init failed\r\n");
			return;
		}
		os_printf("i8080 lcd init. \r\n");
		ret = bk_lcd_8080_init(PIXEL_320, PIXEL_480, PIXEL_FMT_YUYV);
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
		lcd_driver_set_display_base_addr(frameaddr);
		os_printf("bk_lcd_8080_start_transfer \r\n");
		bk_lcd_8080_start_transfer(1);
		os_printf("bk_lcd_8080_ram_write \r\n");
		bk_lcd_8080_ram_write(RAM_WRITE);
	} else if (os_strcmp(argv[1], "disp_continue") == 0) {
		os_printf("bk_lcd_8080_ram_write \r\n");
		bk_lcd_8080_ram_write(CONTINUE_WRITE);
	} else if (os_strcmp(argv[1], "point_display") == 0) {
		uint16_t x= os_strtoul(argv[2], NULL, 10) & 0xFFFF;
		uint16_t y= os_strtoul(argv[3], NULL, 10) & 0xFFFF;
		uint16_t color = os_strtoul(argv[4], NULL, 16) & 0xFFFF;
		bk_lcd_draw_point(LCD_DEVICE_ST7796S, x,y,color);
	}  else if (os_strcmp(argv[1], "lcd_fill_color") == 0) {
		lcd_disp_framebuf_t lcd_disp = {0};
		uint16_t x, y, w, h;
		uint32_t color;
		
		color = os_strtoul(argv[2], NULL, 16) & 0xFFFF;
		x = os_strtoul(argv[3], NULL, 10) & 0xFFFF;
		y = os_strtoul(argv[4], NULL, 10) & 0xFFFF;
		w = os_strtoul(argv[5], NULL, 10) & 0xFFFF;
		h= os_strtoul(argv[6], NULL, 10) & 0xFFFF;
		os_printf("color, x,y,w,h = %x, %d ,%d ,%d ,%d\r\n", color, x, y, w, h);

		uint32_t data_addr = os_strtoul(argv[7], NULL, 16) & 0xFFFFFFFF;
		lcd_disp.rect.x = x;
		lcd_disp.rect.y = y;
		lcd_disp.rect.width = w;
		lcd_disp.rect.height = h;
		lcd_disp.buffer = (uint32_t *)data_addr;
		bk_lcd_fill_color(LCD_DEVICE_ST7796S, &lcd_disp, color);
		bk_lcd_8080_ram_write(RAM_WRITE);
	} else if (os_strcmp(argv[1], "lcd_fill_data") == 0) {
		lcd_disp_framebuf_t lcd_disp = {0};
		uint16_t x, y, w, h;

		x = os_strtoul(argv[2], NULL, 10) & 0xFFFF;
		y = os_strtoul(argv[3], NULL, 10) & 0xFFFF;
		w = os_strtoul(argv[4], NULL, 10) & 0xFFFF;
		h= os_strtoul(argv[5], NULL, 10) & 0xFFFF;
		os_printf("x,y,w,h = %d, %d ,%d ,%d \r\n", x,y,w,h);
		uint32_t data_addr = os_strtoul(argv[6], NULL, 16) & 0xFFFFFFFF;

		lcd_disp.rect.x = x;
		lcd_disp.rect.y = y;
		lcd_disp.rect.width = w;
		lcd_disp.rect.height = h;
		lcd_disp.buffer = (uint32_t *)data_addr;
		bk_lcd_fill_data(LCD_DEVICE_ST7796S, &lcd_disp);
//		bk_lcd_8080_ram_write(RAM_WRITE);
	} else if (os_strcmp(argv[1], "close") == 0) {
		bk_lcd_8080_deinit();
	}
}


void lcd_8080_display_yuv(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int err = kNoErr;

	os_printf("psram init. \r\n");
	bk_psram_init();

	uint32_t lcd_clk = os_strtoul(argv[1], NULL, 16) & 0xffff;
	bk_lcd_driver_init(lcd_clk);

	bk_lcd_isr_register(I8080_OUTPUT_EOF, lcd_i8080_isr);

	bk_lcd_8080_init(PIXEL_320, PIXEL_480, PIXEL_FMT_YUYV);
	bk_lcd_8080_int_enable(0,1);
	err = st7796s_init();
	if (err != BK_OK) {
		os_printf("st7796s init failed\r\n");
		return;
	}
	os_printf("st7796 init ok. \r\n");

	lcd_driver_set_display_base_addr((uint32_t)psram_lcd->display[0]);

	err = dvp_camera_init(JPEG_YUV_MODE);
	if (err != BK_OK) {
		os_printf("camera failed\r\n");
		return;
	}
}

void lcd_8080_display_480p_yuv(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int err = kNoErr;

	os_printf("psram init. \r\n");
	bk_psram_init();

	uint32_t lcd_clk = os_strtoul(argv[1], NULL, 16) & 0xffff;

	bk_lcd_driver_init(lcd_clk);

	bk_lcd_isr_register(I8080_OUTPUT_EOF, lcd_i8080_isr);

	bk_lcd_8080_init(PIXEL_640, PIXEL_480, PIXEL_FMT_YUYV);
	bk_lcd_8080_int_enable(0,1);
	lcd_driver_set_display_base_addr((uint32_t)psram_lcd->display[0]);
	bk_lcd_set_partical_display(1, I8080_PARTICAL_XS, I8080_PARTICAL_XE, I8080_PARTICAL_YS, I8080_PARTICAL_YE);
	err = st7796s_init();
	if (err != BK_OK) {
		os_printf("st7796s init failed\r\n");
		return;
	}
	os_printf("st7796 init ok. \r\n");

	err = dvp_camera_init(JPEG_YUV_MODE);
	if (err != BK_OK) {
		os_printf("camera failed\r\n");
		return;
	}
}

