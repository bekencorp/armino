
#include <os/mem.h>
#include <os/str.h>
#include <os/os.h>
#include <driver/int.h>
#include <common/bk_err.h>
#include "sdkconfig.h"
#include <driver/dma2d.h>
#include <driver/psram.h>
#include <driver/dma.h>
#include "bk_general_dma.h"
#include "dma2d_demo.h"
#include <driver/dma2d_types.h>
#include <stdio.h>
#include <driver/lcd.h>
#if CONFIG_FATFS
#include "ff.h"
#include "diskio.h"
#endif
#include <driver/gpio.h>


static volatile uint32_t     transferErrorDetected = 0;    /**< Set to 1 if an error transfer is detected */
static volatile uint32_t     transferCompleteDetected = 0; /**< Set to 1 if the DMA2D transfer complete is detected */

#define TAG "DMA2D_DEMO"
#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

#if (USE_HAL_DMA2D_REGISTER_CALLBACKS == 1)
static void dma2d_transfer_error(void);
static void dma2d_transfer_complete(void);
static void  dma2d_config_error(void);

static void  dma2d_transfer_error(void)
{
	transferErrorDetected = 1;
	LOGI("dma2d_transfer_error \n");
	bk_dma2d_int_status_clear(DMA2D_TRANS_ERROR_STATUS);
//	bk_dma2d_int_enable(DMA2D_TRANS_ERROR, 0);
}
static void  dma2d_transfer_complete(void)
{
	transferCompleteDetected = 1;
	LOGI("dma2d_transfer_complete \n");
	bk_dma2d_int_status_clear(DMA2D_TRANS_COMPLETE_STATUS);
//	bk_dma2d_int_enable(DMA2D_TRANS_COMPLETE, 0);
}
static void  dma2d_config_error(void)
{
	LOGI("dma2d_config_error \n");
	bk_dma2d_int_status_clear(DMA2D_CFG_ERROR_STATUS);
}
#else
void dma2d_isr()
{
	uint32_t int_status;
	int_status = bk_dma2d_int_status_get();

	if (int_status & DMA2D_CFG_ERROR) {
		bk_dma2d_int_status_clear(DMA2D_CFG_ERROR_STATUS);
		bk_dma2d_int_enable(DMA2D_CFG_ERROR, 0);
		LOGI("transferErrorDetected \r\n");
	}

	/**< Transfer Error Interrupt management ***************************************/
	if (int_status & DMA2D_TRANS_ERROR_STATUS) {
		transferErrorDetected = 1;
		bk_dma2d_int_status_clear(DMA2D_TRANS_ERROR_STATUS);
		bk_dma2d_int_enable(DMA2D_TRANS_ERROR, 0);
		LOGI("transferErrorDetected \r\n");
	}

	if (int_status & DMA2D_TRANS_COMPLETE_STATUS) {
		transferCompleteDetected = 1;
		bk_dma2d_int_status_clear(DMA2D_TRANS_COMPLETE_STATUS);
		bk_dma2d_int_enable(DMA2D_TRANS_COMPLETE, 0);
	}
}
#endif


/**
  * @brief dma2d reg to mem mode,dma2d fill function
           attention: data format is rgb565
  * @param1  the display framebuffer base addr, by defult psram 0x60000000
  * @param2  the lcd_x 
  * @param3  the lcd_y 
  * @param4  the display width , width <= lcd_x - x
  * @param5  the display high , high <= lcd_y - y
  * @param6  color the fill color
  * @return none
  */

extern void bk_mem_dump_ex(const char * title, unsigned char * data, uint32_t data_len);

///dma2d_fill_test=0x60000000,RGB565,0xf800,320,480,320,480,0,0
///sdcard_write=rgb565_red_320_480.rgb,320,480,2,0x60000000
void dma2d_fill_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	LOGI("cmd params(9): dma2d_fill_test = addr,color_format,color,fill_w,fill_l ,lcd_w,lcd_h ,xpos,ypos\n");
	LOGI("color_format support ARGB8888|RGB888|RGB565 \n");
	bk_psram_init();
	bk_dma2d_driver_init();
	uint32_t addr = os_strtoul(argv[1], NULL, 16) & 0xFFFFFFFF;
	out_color_mode_t color_format;
	uint32_t color;
	uint16_t fill_w, fill_h;
	uint8_t pixel_byte; 
	if (os_strcmp(argv[2], "ARGB8888") == 0)
	{
		color_format = DMA2D_OUTPUT_ARGB8888;
		pixel_byte = 4;
	}
	else if (os_strcmp(argv[2], "RGB888") == 0)
	{
		color_format = DMA2D_OUTPUT_RGB888;
		pixel_byte = 3;
	}
	else //(os_strcmp(argv[2], "RGB565") == 0)
	{
		color_format = DMA2D_OUTPUT_RGB565;
		pixel_byte = 2;
	}
	color = os_strtoul(argv[3], NULL, 16) & 0xFFFFFFFF;
	fill_w = os_strtoul(argv[4], NULL, 10) & 0xFFFF;
	fill_h = os_strtoul(argv[5], NULL, 10) & 0xFFFF;
	uint16_t lcd_w = os_strtoul(argv[6], NULL, 10) & 0xFFFF;
	uint16_t lcd_h = os_strtoul(argv[7], NULL, 10) & 0xFFFF;
	uint16_t xpos = os_strtoul(argv[8], NULL, 10) & 0xFFFF;
	uint16_t ypos = os_strtoul(argv[9], NULL, 10) & 0xFFFF;
	LOGI("addr,color_format,color,fill_w,fill_l ,lcd_w,lcd_h ,xpos,ypos %x, %d, %x, %d, %d,%d, %d, %d, %d\n", addr,color_format,color,fill_w, fill_h ,lcd_w,lcd_h ,xpos,ypos);
	//os_memset((char *)addr, 0, lcd_w*lcd_h*pixel_byte);
	dma2d_fill_t fill;
	fill.frameaddr = (void *)addr;
	fill.color = color;
	fill.color_format = color_format;
	fill.pixel_byte = pixel_byte;
	fill.width = fill_w;
	fill.high = fill_h;
	fill.frame_xsize = lcd_w;
	fill.frame_ysize = lcd_h;
	fill.xpos = xpos;
	fill.ypos = ypos;
	dma2d_fill(&fill);

	bk_dma2d_int_enable(DMA2D_CFG_ERROR | DMA2D_TRANS_ERROR | DMA2D_TRANS_COMPLETE ,1);

#if (USE_HAL_DMA2D_REGISTER_CALLBACKS == 1)
	bk_dma2d_register_int_callback_isr(DMA2D_TRANS_ERROR_ISR, dma2d_transfer_error);
	bk_dma2d_register_int_callback_isr(DMA2D_TRANS_COMPLETE_ISR, dma2d_transfer_complete);
	bk_dma2d_register_int_callback_isr(DMA2D_CFG_ERROR_ISR, dma2d_config_error);
#else
	bk_dma2d_isr_register(dma2d_isr);
#endif 
	bk_dma2d_start_transfer();
#if 1
	while (transferCompleteDetected == 0) {
	}
	transferCompleteDetected = 0;
#else

	while (bk_dma2d_is_transfer_busy()) {
	}
#endif
//	bk_mem_dump_ex("dma2d_fill", (unsigned char *)addr, fill.frame_xsize*fill.frame_ysize*pixel_byte);
//	LOGI(" dmmp complete \n");
}

//dma2d_memcpy_test=0x60000000,0x60020000,RGB888,10,11,10,11,0,0,0,0
void dma2d_memcpy_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	bk_psram_init();
	bk_dma2d_driver_init();
	uint32_t input_addr = os_strtoul(argv[1], NULL, 16) & 0xFFFFFFFF;
	uint32_t output_addr = os_strtoul(argv[2], NULL, 16) & 0xFFFFFFFF;

	input_color_mode_t input_color_mode;
	uint8_t pixel_byte; 
	if (os_strcmp(argv[3], "ARGB8888") == 0)
	{
		input_color_mode = DMA2D_OUTPUT_ARGB8888;
		pixel_byte = 4;
	}
	else if (os_strcmp(argv[3], "RGB888") == 0)
	{
		input_color_mode = DMA2D_OUTPUT_RGB888;
		pixel_byte = 3;
	}
	else //(os_strcmp(argv[2], "RGB565") == 0)
	{
		input_color_mode = DMA2D_OUTPUT_RGB565;
		pixel_byte = 2;
	}
	uint16_t xsize= os_strtoul(argv[4], NULL, 10) & 0xFFFF;
	uint16_t ysize = os_strtoul(argv[5], NULL, 10) & 0xFFFF;
	uint16_t src_width = os_strtoul(argv[6], NULL, 10) & 0xFFFF;
	uint16_t src_height = os_strtoul(argv[7], NULL, 10) & 0xFFFF;
	uint16_t src_frame_xpos = os_strtoul(argv[8], NULL, 10) & 0xFFFF;
	uint16_t src_frame_ypos = os_strtoul(argv[9], NULL, 10) & 0xFFFF;
	uint16_t dst_frame_xpos = os_strtoul(argv[10], NULL, 10) & 0xFFFF;
	uint16_t dst_frame_ypos = os_strtoul(argv[11], NULL, 10) & 0xFFFF;
	uint16_t dst_width = src_width;
	uint16_t dst_height = src_height;
	bk_mem_dump_ex("src", (unsigned char *)input_addr, src_width*src_height*pixel_byte);
	bk_mem_dump_ex("dst", (unsigned char *)output_addr, src_width*src_height*pixel_byte);

	dma2d_memcpy_pfc_t dma2d_memcpy_pfc = {0};
	dma2d_memcpy_pfc.mode = DMA2D_M2M;
	dma2d_memcpy_pfc.input_addr = (char *)input_addr;
	dma2d_memcpy_pfc.output_addr = (char *)output_addr;
	dma2d_memcpy_pfc.input_color_mode = input_color_mode;
	dma2d_memcpy_pfc.output_color_mode = input_color_mode;
	dma2d_memcpy_pfc.src_pixel_byte = pixel_byte;
	dma2d_memcpy_pfc.dst_pixel_byte = pixel_byte;
	dma2d_memcpy_pfc.dma2d_width = xsize;
	dma2d_memcpy_pfc.dma2d_height = ysize;
	dma2d_memcpy_pfc.src_frame_width = src_width;
	dma2d_memcpy_pfc.src_frame_height = src_height;
	dma2d_memcpy_pfc.dst_frame_width = dst_width;
	dma2d_memcpy_pfc.dst_frame_height = dst_height;
	dma2d_memcpy_pfc.src_frame_xpos = src_frame_xpos;
	dma2d_memcpy_pfc.src_frame_ypos = src_frame_ypos;
	dma2d_memcpy_pfc.dst_frame_xpos = dst_frame_xpos;
	dma2d_memcpy_pfc.dst_frame_ypos = dst_frame_ypos;
	dma2d_memcpy_pfc.input_red_blue_swap = 0;
	dma2d_memcpy_pfc.output_red_blue_swap = 0;

	bk_dma2d_memcpy_or_pixel_convert(&dma2d_memcpy_pfc);
	bk_dma2d_int_enable(DMA2D_CFG_ERROR | DMA2D_TRANS_ERROR | DMA2D_TRANS_COMPLETE ,1);	
#if 1
#if (USE_HAL_DMA2D_REGISTER_CALLBACKS == 1)
		bk_dma2d_register_int_callback_isr(DMA2D_TRANS_ERROR_ISR, dma2d_transfer_error);
		bk_dma2d_register_int_callback_isr(DMA2D_TRANS_COMPLETE_ISR, dma2d_transfer_complete);
		bk_dma2d_register_int_callback_isr(DMA2D_CFG_ERROR_ISR, dma2d_config_error);
#else
		bk_dma2d_isr_register(dma2d_isr);
#endif 
#endif
	bk_dma2d_start_transfer();
#if 0
	while (transferCompleteDetected == 0) {
	}
	transferCompleteDetected = 0;
#else
	while (bk_dma2d_is_transfer_busy()) {
	}
#endif
	bk_mem_dump_ex("2 dst", (unsigned char *)output_addr, src_width*src_height*pixel_byte);

	for(int i = 0; i < ysize; i++)
	{
		int ret = os_memcmp((unsigned char *)(input_addr + ((src_frame_ypos + i) * src_width + src_frame_xpos)*pixel_byte), (unsigned char *)(output_addr + ((dst_frame_ypos + i) * dst_width + dst_frame_xpos)*pixel_byte), xsize*pixel_byte);

		if(ret == 0)
		{
			LOGI(" test pass \n");
		}
		else
		{
			LOGI(" test fail \n");
		}
	}

	//bk_mem_dump_ex("dma2d_fill", (unsigned char *)addr, fill.frame_xsize*fill.frame_ysize*pixel_byte);

}

/*
	sdcard_read=yuyv422_320_480.rgb,0x60000000
	dma2d_pfc_test=0x60000000,0x60200000,RGB565,320,480,320,480,0,0,0,0,ARGB8888
	sdcard_write=yuyv422_320_480_argb88882.rgb,320,480,4,0x60200000
*/
void dma2d_pfc_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	bk_psram_init();
	bk_dma2d_driver_init();
	uint32_t input_addr = os_strtoul(argv[1], NULL, 16) & 0xFFFFFFFF;
	uint32_t output_addr = os_strtoul(argv[2], NULL, 16) & 0xFFFFFFFF;

	input_color_mode_t input_color_mode;
	out_color_mode_t output_color_mode;
	uint8_t src_pixel_byte, dst_pixel_byte;
	
	if (os_strcmp(argv[3], "ARGB8888") == 0)
	{
		input_color_mode = DMA2D_INPUT_ARGB8888;
		src_pixel_byte = 4;
	}
	else if (os_strcmp(argv[3], "RGB888") == 0)
	{
		input_color_mode = DMA2D_INPUT_RGB888;
		src_pixel_byte = 3;
	}
	else //(os_strcmp(argv[2], "RGB565") == 0)
	{
		input_color_mode = DMA2D_INPUT_RGB565;
		src_pixel_byte = 2;
	}

	uint16_t xsize= os_strtoul(argv[4], NULL, 10) & 0xFFFF;
	uint16_t ysize = os_strtoul(argv[5], NULL, 10) & 0xFFFF;
	uint16_t src_width = os_strtoul(argv[6], NULL, 10) & 0xFFFF;
	uint16_t src_height = os_strtoul(argv[7], NULL, 10) & 0xFFFF;
	uint16_t src_frame_xpos = os_strtoul(argv[8], NULL, 10) & 0xFFFF;
	uint16_t src_frame_ypos = os_strtoul(argv[9], NULL, 10) & 0xFFFF;
	uint16_t dst_frame_xpos = os_strtoul(argv[10], NULL, 10) & 0xFFFF;
	uint16_t dst_frame_ypos = os_strtoul(argv[11], NULL, 10) & 0xFFFF;
	uint16_t dst_width = src_width;
	uint16_t dst_height = src_height;
	if (os_strcmp(argv[12], "ARGB8888") == 0)
	{
		output_color_mode = DMA2D_OUTPUT_ARGB8888;
		dst_pixel_byte = 4;
	}
	else if (os_strcmp(argv[12], "RGB888") == 0)
	{
		output_color_mode = DMA2D_OUTPUT_RGB888;
		dst_pixel_byte = 3;
	}
	else //(os_strcmp(argv[2], "RGB565") == 0)
	{
		output_color_mode = DMA2D_OUTPUT_RGB565;
		dst_pixel_byte = 2;
	}
	LOGI(" %x %x, %d, %d, %d, %d,%d, %d, %d, %d, %d\n", input_addr,output_addr,input_color_mode,xsize, ysize ,src_width,src_height ,src_frame_xpos,src_frame_ypos, dst_frame_xpos,dst_frame_ypos,output_color_mode);
	LOGI("dst_width dst_height %d %d\n",dst_width, dst_height);

	if(xsize < 16)
	{
		bk_mem_dump_ex("src", (unsigned char *)input_addr, src_width*src_height*src_pixel_byte);
		bk_mem_dump_ex("dst", (unsigned char *)output_addr, src_width*src_height*dst_pixel_byte);
	}
	dma2d_memcpy_pfc_t dma2d_memcpy_pfc = {0};
	dma2d_memcpy_pfc.mode = DMA2D_M2M_PFC;
	dma2d_memcpy_pfc.input_addr = (char *)input_addr;
	dma2d_memcpy_pfc.output_addr = (char *)output_addr;
	dma2d_memcpy_pfc.input_color_mode = input_color_mode;
	dma2d_memcpy_pfc.output_color_mode = output_color_mode;
	dma2d_memcpy_pfc.src_pixel_byte = src_pixel_byte;
	dma2d_memcpy_pfc.dst_pixel_byte = dst_pixel_byte;
	dma2d_memcpy_pfc.dma2d_width = xsize;
	dma2d_memcpy_pfc.dma2d_height = ysize;
	dma2d_memcpy_pfc.src_frame_width = src_width;
	dma2d_memcpy_pfc.src_frame_height = src_height;
	dma2d_memcpy_pfc.dst_frame_width = dst_width;
	dma2d_memcpy_pfc.dst_frame_height = dst_height;
	dma2d_memcpy_pfc.src_frame_xpos = src_frame_xpos;
	dma2d_memcpy_pfc.src_frame_ypos = src_frame_ypos;
	dma2d_memcpy_pfc.dst_frame_xpos = dst_frame_xpos;
	dma2d_memcpy_pfc.dst_frame_ypos = dst_frame_ypos;
	dma2d_memcpy_pfc.input_red_blue_swap = 0;
	dma2d_memcpy_pfc.output_red_blue_swap = 0;

	bk_dma2d_memcpy_or_pixel_convert(&dma2d_memcpy_pfc);
	bk_dma2d_int_enable(DMA2D_CFG_ERROR | DMA2D_TRANS_ERROR | DMA2D_TRANS_COMPLETE ,1); 
#if (USE_HAL_DMA2D_REGISTER_CALLBACKS == 1)
//		bk_dma2d_register_int_callback_isr(DMA2D_TRANS_ERROR_ISR, dma2d_transfer_error);
		bk_dma2d_register_int_callback_isr(DMA2D_TRANS_COMPLETE_ISR, dma2d_transfer_complete);
//		bk_dma2d_register_int_callback_isr(DMA2D_CFG_ERROR_ISR, dma2d_config_error);
#else
		bk_dma2d_isr_register(dma2d_isr);
#endif 

	bk_dma2d_start_transfer();
#if (USE_HAL_DMA2D_REGISTER_CALLBACKS == 1)
	while (transferCompleteDetected == 0) {
	}
	transferCompleteDetected = 0;
#else
	while (bk_dma2d_is_transfer_busy()) {
	}
#endif
	if(xsize < 16)
	{
		bk_mem_dump_ex("2 dst", (unsigned char *)output_addr, src_width*src_height*dst_pixel_byte);
	}
	else
	{
		bk_mem_dump_ex("3 dst", (unsigned char *)output_addr, 16*16*dst_pixel_byte);
		bk_mem_dump_ex("4 dst", (unsigned char *)(output_addr + (src_width*src_height-16)*dst_pixel_byte ), 16*dst_pixel_byte);
	}
}



/*
sdcard_read=rgb565_blue_320_480.rgb,0x60000000  //RGB565 FG
sdcard_read=rgb565_red_320_480.rgb,0x60050000     // RGB565 BG
dma2d_blend_test=0x60000000,0x60050000,0x60200000,RGB565,RGB565,RGB565,320,480, 1,1,0x80,0x80
sdcard_write=BLEND_565_320_480.rgb,320,480,2,0x60200000     --pass

sdcard_read=wifi_none_png_rgba.rgb,0x60050000   //ARGB
sdcard_read=wififull_pngrgba.rgb,0x60000000     //ARGB
dma2d_blend_test=0x60050000,0x60000000,0x60200000,ARGB8888,ARGB8888,ARGB8888,32,36,1,1,0x80,0x80
sdcard_write=BLEND_WIWI_ARGB.rgb,32,36,4,0x60200000  --pass

sdcard_read=wifi_none_png_rgba.rgb,0x60050000   //ARGB
sdcard_read=wififull_pngrgb565le.rgb,0x60000000 //RGB565
dma2d_blend_test=0x60050000,0x60000000,0x60200000,ARGB8888,RGB565,ARGB8888,32,36,1,1,0x80,0x80
sdcard_write=BLEND_RGB_ARGB.rgb,32,36,4,0x60200000  --passs

sdcard_read=wififull_pngrgb565le.rgb,0x60000000 //RGB565
sdcard_read=yuv_to_rgb888.rgb,0x60050000        //RGB888
dma2d_blend_test=0x60050000,0x60000000,0x60200000,RGB888,RGB565,ARGB8888,32,36,1,1,0x80,0x80
sdcard_write=BLEND_565_888_ARGB.rgb,32,36,4,0x60200000  --pass

sdcard_read=yuv_to_rgb888.rgb,0x60000000        //RGB888
sdcard_read=wifi_none_png_rgba.rgb,0x60050000   //ARGB
dma2d_blend_test=0x60050000,0x60000000,0x60200000,ARGB8888,RGB888, RGB565,32,36,1,1,0x80,0x80
sdcard_write=BLEND_ARGB_888_565.rgb,32,36,4,0x60200000

sdcard_read=yuv_to_rgb888.rgb,0x60000000        //RGB888
sdcard_read=wifi_none_png_rgba.rgb,0x60050000   //ARGB
dma2d_blend_test=0x60050000,0x60000000,0x60200000,ARGB8888,RGB888, RGB888,32,36,1,1,0x80,0x80
sdcard_write=BLEND_ARGB_888_888.rgb,32,36,3,0x60200000


大图片测试：
dma2d_fill_test=0x60200000,RGB565,0xf800,1280,720,1280,720,0,0
sdcard_write=1280_720_red.rgb,1280,720,2,0x60200000
dma2d_fill_test=0x60000000,RGB565,0x001f,1280,720,1280,720,0,0
sdcard_write=1280_720_blue.rgb,1280,720,2,0x60000000

//565+565=argb(ok)  rgb565(ok) rgb888(ok)
sdcard_read=1280_720_red.rgb,0x60200000
sdcard_read=1280_720_blue.rgb,0x60000000
dma2d_blend_test=0x60200000,0x60000000,0x60000000,RGB565,RGB565, RGB888,1280,720,1,1,0x80,0x80
sdcard_write=1280_720_blend.rgb,1280,720,2,0x60000000
sdcard_write=1280_720_blend_rgb888.rgb,1280,720,3,0x60000000

//argb+argb  == argb(ok) rgb565(ok)  rgb888(fail)
sdcard_read=1280_720_argb8888_red.rgb,0x60000000
sdcard_read=1280_720_argb8888_zi.rgb,0x60400000
dma2d_blend_test=0x60000000,0x60400000,0x60800000,ARGB8888,ARGB8888, ARGB8888,1280,720,1,1,0x80,0x80 
sdcard_write=1280_720_blend_argb888.rgb,1280,720,4,0x60800000

//argb+rgb565 ==argb8888(ok) rgb565(ok) rgb888(FAIL)
sdcard_read=1280_720_argb8888_red.rgb,0x60000000
sdcard_read=1280_720_blue.rgb,0x60400000
dma2d_blend_test=0x60000000,0x60400000,0x60800000,ARGB8888,RGB565, ARGB8888,1280,720,1,1,0x80,0x80 
sdcard_write=1280_720_blend_argb888.rgb,1280,720,4,0x60800000
*/
void dma2d_blend_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t input_fg_addr = os_strtoul(argv[1], NULL, 16) & 0xFFFFFFFF;
	uint32_t input_bg_addr = os_strtoul(argv[2], NULL, 16) & 0xFFFFFFFF;
	uint32_t output_blend_addr = os_strtoul(argv[3], NULL, 16) & 0xFFFFFFFF;
	blend_alpha_mode_t fg_alpha_mode, bg_alpha_mode;
	input_color_mode_t input_fg_mode, input_bg_mode;
	out_color_mode_t output_mode;
	uint8_t fg_pixel_byte, bg_pixel_byte, out_pixel_byte;
	
	if (os_strcmp(argv[4], "ARGB8888") == 0) {
		input_fg_mode = DMA2D_INPUT_ARGB8888;
		fg_pixel_byte = 4;
	} else if (os_strcmp(argv[4], "RGB888") == 0) {
		input_fg_mode = DMA2D_INPUT_RGB888;
		fg_pixel_byte = 3;
	} else { /*(os_strcmp(argv[2], "RGB565") == 0)*/
		input_fg_mode = DMA2D_INPUT_RGB565;

		fg_pixel_byte = 2;
	}
	if (os_strcmp(argv[5], "ARGB8888") == 0) {
		input_bg_mode = DMA2D_INPUT_ARGB8888;
		bg_pixel_byte = 4;
	} else if (os_strcmp(argv[5], "RGB888") == 0) {
		input_bg_mode = DMA2D_INPUT_RGB888;
		bg_pixel_byte = 3;
	} else /*(os_strcmp(argv[2], "RGB565") == 0)*/{
		input_bg_mode = DMA2D_INPUT_RGB565;

		bg_pixel_byte = 2;
	}
	if (os_strcmp(argv[6], "ARGB8888") == 0) {
		output_mode = DMA2D_OUTPUT_ARGB8888;
		out_pixel_byte = 4;
	} else if (os_strcmp(argv[6], "RGB888") == 0) {
		output_mode = DMA2D_OUTPUT_RGB888;
		out_pixel_byte = 3;
	} else /*(os_strcmp(argv[2], "RGB565") == 0)*/{
		output_mode = DMA2D_OUTPUT_RGB565;

		out_pixel_byte = 2;
	}
	uint16_t xsize= os_strtoul(argv[7], NULL, 10) & 0xFFFF;
	uint16_t ysize = os_strtoul(argv[8], NULL, 10) & 0xFFFF;
	fg_alpha_mode= os_strtoul(argv[9], NULL, 10) & 0xFF;
	bg_alpha_mode = os_strtoul(argv[10], NULL, 10) & 0xFF;
	uint8_t fg_alpha_value= os_strtoul(argv[11], NULL, 16) & 0xFF;
	uint8_t bg_alpha_value = os_strtoul(argv[12], NULL, 16) & 0xFF;


	LOGI(" %x %x, %x, %d, %d\n", input_fg_addr,input_bg_addr,output_blend_addr,xsize, ysize);
	
	bk_mem_dump_ex("input_fg_addr start ", (unsigned char *)input_fg_addr, 16*fg_pixel_byte);
	bk_mem_dump_ex("input_fg_addr end", (unsigned char *)(input_fg_addr + (xsize*ysize-16)*fg_pixel_byte), 16*fg_pixel_byte);
	bk_mem_dump_ex("input_bg_addr start", (unsigned char *)input_bg_addr, 16*bg_pixel_byte);
	bk_mem_dump_ex("input_bg_addr end", (unsigned char *)(input_bg_addr + (xsize*ysize-16)*bg_pixel_byte ), 16*bg_pixel_byte);

	bk_dma2d_driver_init();

	dma2d_blend_t dma2d_config;
	
	dma2d_config.pfg_addr = (char *)input_fg_addr;
	dma2d_config.pbg_addr = (char *)input_bg_addr;
	dma2d_config.pdst_addr = (char *)output_blend_addr;
	dma2d_config.fg_color_mode = input_fg_mode;
	dma2d_config.red_bule_swap = DMA2D_RB_REGULAR;
	dma2d_config.bg_color_mode = input_bg_mode;
	dma2d_config.dst_color_mode = output_mode;
	dma2d_config.fg_offline = 0;
	dma2d_config.bg_offline = 0;
	dma2d_config.dest_offline = 0;
	dma2d_config.xsize = xsize;
	dma2d_config.ysize = ysize;
	dma2d_config.fg_alpha_mode = fg_alpha_mode;
	dma2d_config.bg_alpha_mode = bg_alpha_mode;
	dma2d_config.fg_alpha_value = fg_alpha_value;
	dma2d_config.bg_alpha_value = bg_alpha_value;
	bk_dma2d_blend(&dma2d_config);

	bk_dma2d_int_enable(DMA2D_TRANS_COMPLETE, 1); 
	bk_dma2d_register_int_callback_isr(DMA2D_TRANS_COMPLETE_ISR, dma2d_transfer_complete);

	LOGI("dma2d_start_transfer\n");
	bk_dma2d_start_transfer();

	while (transferCompleteDetected == 0) {
	}
	transferCompleteDetected = 0;

	bk_mem_dump_ex("output_blend_addr start", (unsigned char *)output_blend_addr, 16*out_pixel_byte);
	bk_mem_dump_ex("output_blend_addr end", (unsigned char *)(output_blend_addr + (xsize*ysize-16)*out_pixel_byte ), 16*out_pixel_byte);
}

/*
sdcard_read=rgb565_blue_320_480.rgb,0x60000000  //RGB565 FG
sdcard_read=rgb565_red_320_480.rgb,0x60050000     // RGB565 BG
dma2d_blend_offset_test=0x60000000,0x60050000,0x60200000,RGB565,RGB565,RGB565,320,480, 1,1,0x80,0x80,20,20,20,20,40,40
sdcard_write=BLEND_565_320_480.rgb,320,480,2,0x60200000     --pass
*/
void dma2d_blend_offset_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t input_fg_addr = os_strtoul(argv[1], NULL, 16) & 0xFFFFFFFF;
	uint32_t input_bg_addr = os_strtoul(argv[2], NULL, 16) & 0xFFFFFFFF;
	uint32_t output_blend_addr = os_strtoul(argv[3], NULL, 16) & 0xFFFFFFFF;
	blend_alpha_mode_t fg_alpha_mode, bg_alpha_mode;
	input_color_mode_t input_fg_mode, input_bg_mode;
	out_color_mode_t output_mode;
	uint8_t fg_pixel_byte, bg_pixel_byte, out_pixel_byte;
	
	if (os_strcmp(argv[4], "ARGB8888") == 0) {
		input_fg_mode = DMA2D_INPUT_ARGB8888;
		fg_pixel_byte = 4;
	} else if (os_strcmp(argv[4], "RGB888") == 0) {
		input_fg_mode = DMA2D_INPUT_RGB888;
		fg_pixel_byte = 3;
	} else { /*(os_strcmp(argv[2], "RGB565") == 0)*/
		input_fg_mode = DMA2D_INPUT_RGB565;
		fg_pixel_byte = 2;
	}
	if (os_strcmp(argv[5], "ARGB8888") == 0) {
		input_bg_mode = DMA2D_INPUT_ARGB8888;
		bg_pixel_byte = 4;
	} else if (os_strcmp(argv[5], "RGB888") == 0) {
		input_bg_mode = DMA2D_INPUT_RGB888;
		bg_pixel_byte = 3;
	} else /*(os_strcmp(argv[2], "RGB565") == 0)*/{
		input_bg_mode = DMA2D_INPUT_RGB565;
		bg_pixel_byte = 2;
	}
	if (os_strcmp(argv[6], "ARGB8888") == 0) {
		output_mode = DMA2D_OUTPUT_ARGB8888;
		out_pixel_byte = 4;
	} else if (os_strcmp(argv[6], "RGB888") == 0) {
		output_mode = DMA2D_OUTPUT_RGB888;
		out_pixel_byte = 3;
	} else /*(os_strcmp(argv[2], "RGB565") == 0)*/{
		output_mode = DMA2D_OUTPUT_RGB565;
		out_pixel_byte = 2;
	}
	uint16_t xsize= os_strtoul(argv[7], NULL, 10) & 0xFFFF;
	uint16_t ysize = os_strtoul(argv[8], NULL, 10) & 0xFFFF;

	fg_alpha_mode= 1;
	bg_alpha_mode = 1;
	uint8_t fg_alpha_value= 0x80;
	uint8_t bg_alpha_value = 0x80;

	uint16_t fg_frame_xpos = os_strtoul(argv[9], NULL, 10) & 0xFFFF;
	uint16_t fg_frame_ypos = os_strtoul(argv[10], NULL, 10) & 0xFFFF;
	uint16_t bg_frame_xpos = fg_frame_xpos;
	uint16_t bg_frame_ypos = fg_frame_ypos;
	uint16_t dst_frame_xpos = fg_frame_xpos;
	uint16_t dst_frame_ypos = fg_frame_ypos;
	uint16_t frame_width = os_strtoul(argv[11], NULL, 10) & 0xFFFF;
	uint16_t frame_height = os_strtoul(argv[12], NULL, 10) & 0xFFFF;


//	LOGI(" %x %x, %x, %d, %d, %d, %d, %d ,%d\n", input_fg_addr,input_bg_addr,output_blend_addr,xsize, ysize, fg_frame_xpos, fg_frame_ypos, frame_width, frame_height);
//
//	bk_mem_dump_ex("input_fg_addr start ", (unsigned char *)input_fg_addr, 16*fg_pixel_byte);
//	bk_mem_dump_ex("input_fg_addr end", (unsigned char *)(input_fg_addr + (frame_width*frame_height-16)*fg_pixel_byte), 16*fg_pixel_byte);
//	bk_mem_dump_ex("input_bg_addr start", (unsigned char *)input_bg_addr, 16*bg_pixel_byte);
//	bk_mem_dump_ex("input_bg_addr end", (unsigned char *)(input_bg_addr + (frame_width*frame_height-16)*bg_pixel_byte ), 16*bg_pixel_byte);

//	bk_mem_dump_ex("dst_addr start", (unsigned char *)output_blend_addr, 16*out_pixel_byte);
//	bk_mem_dump_ex("dst_addr end", (unsigned char *)(output_blend_addr + (xsize*ysize-16)*out_pixel_byte ), 16*out_pixel_byte);

	bk_dma2d_driver_init();

	dma2d_offset_blend_t dma2d_config;
	
	dma2d_config.pfg_addr = (char *)input_fg_addr;
	dma2d_config.pbg_addr = (char *)input_bg_addr;
	dma2d_config.pdst_addr = (char *)output_blend_addr;
	dma2d_config.fg_color_mode = input_fg_mode;	
	dma2d_config.bg_color_mode = input_bg_mode;
	dma2d_config.dst_color_mode = output_mode;
	dma2d_config.fg_red_blue_swap = DMA2D_RB_REGULAR;
	dma2d_config.bg_red_blue_swap = DMA2D_RB_REGULAR;
	dma2d_config.dst_red_blue_swap = DMA2D_RB_REGULAR;
	
	dma2d_config.fg_frame_width = frame_width;
	dma2d_config.fg_frame_height = frame_height;
	dma2d_config.bg_frame_width = frame_width;
	dma2d_config.bg_frame_height = frame_height;
	dma2d_config.dst_frame_width = frame_width;
	dma2d_config.dst_frame_height = frame_height;

	dma2d_config.fg_frame_xpos = fg_frame_xpos;
	dma2d_config.fg_frame_ypos = fg_frame_ypos;
	dma2d_config.bg_frame_xpos = bg_frame_xpos;
	dma2d_config.bg_frame_ypos = bg_frame_ypos;
	dma2d_config.dst_frame_xpos = dst_frame_xpos;
	dma2d_config.dst_frame_ypos = dst_frame_ypos;
	
	dma2d_config.fg_pixel_byte = fg_pixel_byte;
	dma2d_config.bg_pixel_byte = bg_pixel_byte;
	dma2d_config.dst_pixel_byte = out_pixel_byte;
	
	dma2d_config.dma2d_width = xsize;
	dma2d_config.dma2d_height = ysize;
	dma2d_config.fg_alpha_mode = fg_alpha_mode;
	dma2d_config.bg_alpha_mode = bg_alpha_mode;
	dma2d_config.fg_alpha_value = fg_alpha_value;
	dma2d_config.bg_alpha_value = bg_alpha_value;
	bk_dma2d_offset_blend(&dma2d_config);

	bk_dma2d_int_enable(DMA2D_TRANS_COMPLETE, 1); 
	bk_dma2d_register_int_callback_isr(DMA2D_TRANS_COMPLETE_ISR, dma2d_transfer_complete);

//	LOGI("dma2d_start_transfer\n");
	bk_dma2d_start_transfer();

	while (transferCompleteDetected == 0) {
	}
	transferCompleteDetected = 0;

//	bk_mem_dump_ex("output_blend_addr start", (unsigned char *)output_blend_addr, 16*out_pixel_byte);
//	bk_mem_dump_ex("output_blend_addr end", (unsigned char *)(output_blend_addr + (frame_width*frame_height-16)*out_pixel_byte ), 16*out_pixel_byte);
}


#if 0  //OFFSET
#define PIXEL_X 800
#define PIXEL_Y 480

#define DMA2D_FG_XPIXE PIXEL_X
#define DMA2D_FG_YPIXE PIXEL_Y

#define DMA2D_BG_XPIXE PIXEL_X
#define DMA2D_BG_YPIXE PIXEL_Y

#define DMA2D_DST_XPIXE PIXEL_X
#define DMA2D_DST_YPIXE PIXEL_Y

#define DMA2D_XPIXE 401
#define DMA2D_YPIXE 201


#define FG_XPOS   120
#define FG_YPOS   200

#define BG_XPOS   250
#define BG_YPOS   120

#define DST_XPOS  250
#define DST_YPOS  120

#define DMA2D_FG_ADDR 0x60000000
#define DMA2D_BG_ADDR 0x60400000
#define DMA2D_DST_ADDR 0x60400000
#else  //no offset

#define PIXEL_X 800
#define PIXEL_Y 480
#define X_POS   0
#define Y_POS   0

#define DMA2D_FG_XPIXE PIXEL_X
#define DMA2D_FG_YPIXE PIXEL_Y

#define DMA2D_BG_XPIXE PIXEL_X
#define DMA2D_BG_YPIXE PIXEL_Y

#define DMA2D_DST_XPIXE PIXEL_X
#define DMA2D_DST_YPIXE PIXEL_Y

#define DMA2D_XPIXE PIXEL_X
#define DMA2D_YPIXE PIXEL_Y

#define DMA2D_FG_ADDR  0x60000000
#define DMA2D_BG_ADDR  0x60400000
#define DMA2D_DST_ADDR 0x60400000

#define FG_XPOS   X_POS
#define FG_YPOS   Y_POS

#define BG_XPOS   X_POS
#define BG_YPOS   Y_POS

#define DST_XPOS  X_POS
#define DST_YPOS  Y_POS
#endif
void dma2d_blend(uint32_t fg_addr, uint32_t bg_addr, uint32_t output_addr, uint8_t fg_alpha)
{
	bk_dma2d_driver_init();

	bk_gpio_set_output_high(GPIO_9);

	uint32_t input_fg_addr = fg_addr;
	uint32_t input_bg_addr = bg_addr;
	uint32_t output_blend_addr = output_addr;
	blend_alpha_mode_t fg_alpha_mode, bg_alpha_mode;
	input_color_mode_t input_fg_mode, input_bg_mode;
	out_color_mode_t output_mode;
	uint8_t fg_pixel_byte, bg_pixel_byte, out_pixel_byte;
	

	input_fg_mode = DMA2D_INPUT_ARGB8888;
	fg_pixel_byte = 4;

	input_bg_mode  = DMA2D_INPUT_RGB888;
	bg_pixel_byte = 3;
	
	output_mode = DMA2D_OUTPUT_ARGB8888;
	out_pixel_byte = 4;


	uint16_t xsize= DMA2D_XPIXE;
	uint16_t ysize = DMA2D_YPIXE;

	fg_alpha_mode= 1;
	bg_alpha_mode = 1;
	uint8_t fg_alpha_value= fg_alpha;
	uint8_t bg_alpha_value = 0x80;

	uint16_t fg_frame_xpos =  FG_XPOS;
	uint16_t fg_frame_ypos =  FG_YPOS;
	uint16_t bg_frame_xpos =  BG_XPOS;
	uint16_t bg_frame_ypos =  BG_YPOS;
	uint16_t dst_frame_xpos = DST_XPOS;
	uint16_t dst_frame_ypos = DST_YPOS;

	
	uint16_t fg_frame_width = DMA2D_FG_XPIXE;
	uint16_t fg_frame_height = DMA2D_FG_YPIXE;
	uint16_t bg_frame_width = DMA2D_BG_XPIXE;
	uint16_t bg_frame_height = DMA2D_BG_YPIXE;
	uint16_t dst_frame_width = DMA2D_DST_XPIXE;
	uint16_t dst_frame_height = DMA2D_DST_YPIXE;

	dma2d_offset_blend_t dma2d_config;
	
	dma2d_config.pfg_addr = (char *)input_fg_addr;
	dma2d_config.pbg_addr = (char *)input_bg_addr;
	dma2d_config.pdst_addr = (char *)output_blend_addr;
	dma2d_config.fg_color_mode = input_fg_mode;	
	dma2d_config.bg_color_mode = input_bg_mode;
	dma2d_config.dst_color_mode = output_mode;
	dma2d_config.fg_red_blue_swap = DMA2D_RB_REGULAR;
	dma2d_config.bg_red_blue_swap = DMA2D_RB_REGULAR;
	dma2d_config.dst_red_blue_swap = DMA2D_RB_REGULAR;
	
	dma2d_config.fg_frame_width = fg_frame_width;
	dma2d_config.fg_frame_height = fg_frame_height;
	dma2d_config.bg_frame_width = bg_frame_width;
	dma2d_config.bg_frame_height = bg_frame_height;
	dma2d_config.dst_frame_width = dst_frame_width;
	dma2d_config.dst_frame_height = dst_frame_height;

	dma2d_config.fg_frame_xpos = fg_frame_xpos;
	dma2d_config.fg_frame_ypos = fg_frame_ypos;
	dma2d_config.bg_frame_xpos = bg_frame_xpos;
	dma2d_config.bg_frame_ypos = bg_frame_ypos;
	dma2d_config.dst_frame_xpos = dst_frame_xpos;
	dma2d_config.dst_frame_ypos = dst_frame_ypos;
	
	dma2d_config.fg_pixel_byte = fg_pixel_byte;
	dma2d_config.bg_pixel_byte = bg_pixel_byte;
	dma2d_config.dst_pixel_byte = out_pixel_byte;
	
	dma2d_config.dma2d_width = xsize;
	dma2d_config.dma2d_height = ysize;
	dma2d_config.fg_alpha_mode = fg_alpha_mode;
	dma2d_config.bg_alpha_mode = bg_alpha_mode;
	dma2d_config.fg_alpha_value = fg_alpha_value;
	dma2d_config.bg_alpha_value = bg_alpha_value;
	bk_dma2d_offset_blend(&dma2d_config);

	//bk_dma2d_int_enable(DMA2D_TRANS_COMPLETE, 1);
	bk_dma2d_int_enable(DMA2D_CFG_ERROR | DMA2D_TRANS_ERROR | DMA2D_TRANS_COMPLETE ,1); 
	bk_dma2d_register_int_callback_isr(DMA2D_TRANS_COMPLETE_ISR, dma2d_transfer_complete);

	LOGI("dma2d_start_transfer\n");
	bk_dma2d_start_transfer();

	while (transferCompleteDetected == 0) {
	}
	transferCompleteDetected = 0;
	bk_gpio_set_output_low(GPIO_9);
}

void dma2d_pfc( uint32_t in_addr, uint32_t out_addr)
{
	bk_dma2d_driver_init();

	uint32_t input_addr = in_addr;
	uint32_t output_addr = out_addr;

	input_color_mode_t input_color_mode;
	out_color_mode_t output_color_mode;
	uint8_t src_pixel_byte, dst_pixel_byte;

	bk_gpio_set_output_high(GPIO_8);

	uint16_t xsize= DMA2D_XPIXE;
	uint16_t ysize = DMA2D_YPIXE;
	uint16_t src_width = DMA2D_FG_XPIXE;
	uint16_t src_height = DMA2D_FG_YPIXE ;
	uint16_t src_frame_xpos = FG_XPOS;
	uint16_t src_frame_ypos = FG_YPOS;
	uint16_t dst_frame_xpos = DST_XPOS;
	uint16_t dst_frame_ypos = DST_YPOS;
	uint16_t dst_width = DMA2D_DST_XPIXE;
	uint16_t dst_height = DMA2D_DST_YPIXE;

	input_color_mode = DMA2D_INPUT_RGB888;
	src_pixel_byte = 3;

	output_color_mode = DMA2D_OUTPUT_RGB565;
	dst_pixel_byte = 2;

	dma2d_memcpy_pfc_t dma2d_memcpy_pfc = {0};
	dma2d_memcpy_pfc.mode = DMA2D_M2M_PFC;
	dma2d_memcpy_pfc.input_addr = (char *)input_addr;
	dma2d_memcpy_pfc.output_addr = (char *)output_addr;
	dma2d_memcpy_pfc.input_color_mode = input_color_mode;
	dma2d_memcpy_pfc.output_color_mode = output_color_mode;
	dma2d_memcpy_pfc.src_pixel_byte = src_pixel_byte;
	dma2d_memcpy_pfc.dst_pixel_byte = dst_pixel_byte;
	dma2d_memcpy_pfc.dma2d_width = xsize;
	dma2d_memcpy_pfc.dma2d_height = ysize;
	dma2d_memcpy_pfc.src_frame_width = src_width;
	dma2d_memcpy_pfc.src_frame_height = src_height;
	dma2d_memcpy_pfc.dst_frame_width = dst_width;
	dma2d_memcpy_pfc.dst_frame_height = dst_height;
	dma2d_memcpy_pfc.src_frame_xpos = src_frame_xpos;
	dma2d_memcpy_pfc.src_frame_ypos = src_frame_ypos;
	dma2d_memcpy_pfc.dst_frame_xpos = dst_frame_xpos;
	dma2d_memcpy_pfc.dst_frame_ypos = dst_frame_ypos;
	dma2d_memcpy_pfc.input_red_blue_swap = 0;
	dma2d_memcpy_pfc.output_red_blue_swap = 0;

	bk_dma2d_memcpy_or_pixel_convert(&dma2d_memcpy_pfc);
	bk_dma2d_int_enable(DMA2D_CFG_ERROR | DMA2D_TRANS_ERROR | DMA2D_TRANS_COMPLETE ,1); 
	bk_dma2d_register_int_callback_isr(DMA2D_TRANS_COMPLETE_ISR, dma2d_transfer_complete);

	bk_dma2d_start_transfer();

	while (transferCompleteDetected == 0) {
	}
	transferCompleteDetected = 0;

	bk_gpio_set_output_low(GPIO_8);
}

void lcd_rgb_isr_dma2d(void)
{
	//LOGI("enter isr: lcd_rgb_isr_dma2d \r\n");
}


/*
rgb_dma2d=data_prepare,pfc,0x60000000,0x60400000

sdcard_write=rgb_dma2d_pfc.rgb,1280,720,3,0x60400000

rgb_dma2d=data_prepare,blend,0x60000000,0x60400000

rgb_dma2d=lcd_init
rgb_dma2d=lcd_init,lcd_disp,0x600000000,1
*/

//sdcard_read=test_biu_800_480_rgb888.rgb,0x60200000
void lcd_rgb_display_dma2d(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	
	uint32_t input_fg_addr; 
	uint32_t input_bg_addr; 
	uint32_t output_blend_addr;

	if (os_strcmp(argv[1], "data_prepare") == 0)
	{
		bk_gpio_driver_init();
		bk_gpio_enable_output(GPIO_9);
		bk_gpio_enable_output(GPIO_8);
		bk_gpio_pull_down(GPIO_9);
		bk_gpio_pull_down(GPIO_8);

		bk_psram_init();
		input_fg_addr = DMA2D_FG_ADDR;
		input_bg_addr  = DMA2D_BG_ADDR;

		if ((argv[2] != NULL) && (argv[3] != NULL))
		{
			input_fg_addr= os_strtoul(argv[2], NULL, 16) & 0xFFFFFFFF;
			input_bg_addr = os_strtoul(argv[3], NULL, 16) & 0xFFFFFFFF;
		}
		LOGI("input_fg_addr = %p, input_bg_addr = %p \r\n", input_fg_addr, input_bg_addr);

#if RGB565_800_480
		os_memcpy((uint8_t *)(input_fg_addr),(uint8_t *)&rgb565le_800_480[0], sizeof(rgb565le_800_480));       //beautiful
//		bk_mem_dump_ex("input_fg_addr", (unsigned char *)input_fg_addr, 100);
#endif

#if RGB565_800_480_2
		os_memcpy((uint8_t *)(input_bg_addr),(uint8_t *)&rgb565le_800_480_2[0], sizeof(rgb565le_800_480_2));   //fruit
//		bk_mem_dump_ex("input_bg_addr", (unsigned char *)input_bg_addr, 100);
#endif
		LOGI("data prepare ok \r\n");
	}
	if (os_strcmp(argv[1], "pfc") == 0)
	{
		input_fg_addr = DMA2D_FG_ADDR;
		input_bg_addr = DMA2D_BG_ADDR;
		if ((argv[2] != NULL) && (argv[3] != NULL))
		{
			input_fg_addr= os_strtoul(argv[2], NULL, 16) & 0xFFFFFFFF;
			input_bg_addr = os_strtoul(argv[3], NULL, 16) & 0xFFFFFFFF;
		}
		LOGI("src_addr = %p, dst_addr = %p \r\n", input_fg_addr, input_bg_addr);
		dma2d_pfc(input_fg_addr, input_bg_addr);
//		bk_mem_dump_ex("input_fg_addr", (unsigned char *)input_fg_addr, 100);
//		bk_mem_dump_ex("input_bg_addr", (unsigned char *)input_bg_addr, 100);
	}

	if (os_strcmp(argv[1], "blend") == 0)
	{
		input_fg_addr = DMA2D_FG_ADDR;
		input_bg_addr = DMA2D_BG_ADDR;
		output_blend_addr = DMA2D_DST_ADDR;

		uint8_t fg_alpha= os_strtoul(argv[2], NULL, 16) & 0xFF;
		if ((argv[3] != NULL) && (argv[4] != NULL) && (argv[5] != NULL))
		{
			input_fg_addr= os_strtoul(argv[3], NULL, 16) & 0xFFFFFFFF;
			input_bg_addr = os_strtoul(argv[4], NULL, 16) & 0xFFFFFFFF;
			output_blend_addr = os_strtoul(argv[5], NULL, 16) & 0xFFFFFFFF;
		}
		LOGI("src_addr fg= %p bg=%p, dst_addr = %p \r\n", input_fg_addr, input_bg_addr, output_blend_addr);
		dma2d_blend( input_fg_addr, input_bg_addr, output_blend_addr, fg_alpha);
//		bk_mem_dump_ex("output_blend_addr", (unsigned char *)output_blend_addr, 100);
	}

	if (os_strcmp(argv[1], "lcd_init") == 0)
	{
		uint16_t xsize = DMA2D_DST_XPIXE;
		uint16_t ysize = DMA2D_DST_YPIXE;
		if ((argv[2] != NULL) && (argv[3] != NULL))
		{
			xsize= os_strtoul(argv[2], NULL, 10) & 0xFFFF;
			ysize = os_strtoul(argv[3], NULL, 10) & 0xFFFF;
		}
		LOGI("xsize ysize = %d %d \r\n", xsize, ysize);

		lcd_ldo_power_enable(1);
		bk_lcd_driver_init(LCD_12M);
		bk_lcd_rgb_init(LCD_DEVICE_ST7282, xsize, ysize, PIXEL_FMT_RGB565);

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

		bk_lcd_isr_register(RGB_OUTPUT_EOF, lcd_rgb_isr_dma2d);
		
		LOGI("lcd_init ok \r\n");
	}
	else if (os_strcmp(argv[1], "lcd_disp") == 0)
	{
		uint32_t base_addr= os_strtoul(argv[2], NULL, 16) & 0xFFFFFFFF;
		uint8_t en= os_strtoul(argv[3], NULL, 10) & 0xFFFF;

		lcd_driver_set_display_base_addr(base_addr);
		bk_lcd_rgb_display_en(en);
	}
	else if (os_strcmp(argv[1], "check") == 0)
	{
		uint32_t sd_read_addr= os_strtoul(argv[2], NULL, 16) & 0xFFFFFFFF;
		uint32_t psramaddr= os_strtoul(argv[3], NULL, 16) & 0xFFFFFFFF;
		int cmp = 1;

		if (os_strcmp(argv[4], "RGB888") == 0)
			cmp = os_memcmp((char *) sd_read_addr, (char *) psramaddr, 480*800*3);
		else if (os_strcmp(argv[4], "ARGB8888") == 0)
			cmp = os_memcmp((char *) sd_read_addr, (char *) psramaddr, 480*800*4);
		else
			cmp = os_memcmp((char *) sd_read_addr, (char *) psramaddr, 480*800*2);
		if(cmp == 0)
		{		
			LOGI("memcmp is 0, pass pass\r\n");
		}
		else
		{
			LOGI("memcmp error\r\n");
		}
	}
	else if (os_strcmp(argv[1], "close_lcd") == 0)	 
	{
		if (bk_lcd_rgb_deinit() !=BK_OK) {
			LOGI("deinit rgb \r\n");
		}
		LOGI("close rgb ok \r\n");
	}
	else if (os_strcmp(argv[1], "close_dma2d") == 0)	 
	{
		if (bk_dma2d_driver_deinit() != BK_OK) {
			LOGI("dma2d driver init failed\r\n");
		}
	}
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
		
		uint32_t pixel_byte = os_strtoul(argv[4], NULL, 10) & 0xFFFF;
		os_printf(" pixel_byte  = %d \r\n", pixel_byte);

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
		fr = f_write(&file, (char *)ucRdTemp, width * height * pixel_byte , &uiTemp);
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


