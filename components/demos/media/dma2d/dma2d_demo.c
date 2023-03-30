
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

#if CONFIG_FATFS
#include "ff.h"
#include "diskio.h"
#endif


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
	bk_dma2d_int_enable(DMA2D_TRANS_ERROR, 0);
}
static void  dma2d_transfer_complete(void)
{
	transferCompleteDetected = 1;
	LOGI("dma2d_transfer_complete \n");
	bk_dma2d_int_status_clear(DMA2D_TRANS_COMPLETE_STATUS);
	bk_dma2d_int_enable(DMA2D_TRANS_COMPLETE, 0);
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

#if 1
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
	uint8_t pixel_bit; 
	if (os_strcmp(argv[2], "ARGB8888") == 0)
	{
		color_format = DMA2D_OUTPUT_ARGB8888;
		pixel_bit = 4;
	}
	else if (os_strcmp(argv[2], "RGB888") == 0)
	{
		color_format = DMA2D_OUTPUT_RGB888;
		pixel_bit = 3;
	}
	else //(os_strcmp(argv[2], "RGB565") == 0)
	{
		color_format = DMA2D_OUTPUT_RGB565;
		pixel_bit = 2;
	}
	color = os_strtoul(argv[3], NULL, 16) & 0xFFFFFFFF;
	fill_w = os_strtoul(argv[4], NULL, 10) & 0xFFFF;
	fill_h = os_strtoul(argv[5], NULL, 10) & 0xFFFF;
	uint16_t lcd_w = os_strtoul(argv[6], NULL, 10) & 0xFFFF;
	uint16_t lcd_h = os_strtoul(argv[7], NULL, 10) & 0xFFFF;
	uint16_t xpos = os_strtoul(argv[8], NULL, 10) & 0xFFFF;
	uint16_t ypos = os_strtoul(argv[9], NULL, 10) & 0xFFFF;
	LOGI("addr,color_format,color,fill_w,fill_l ,lcd_w,lcd_h ,xpos,ypos %x, %d, %x, %d, %d,%d, %d, %d, %d\n", addr,color_format,color,fill_w, fill_h ,lcd_w,lcd_h ,xpos,ypos);
	//os_memset((char *)addr, 0, lcd_w*lcd_h*pixel_bit);
	dma2d_fill_t fill;
	fill.frameaddr = (void *)addr;
	fill.color = color;
	fill.color_format = color_format;
	fill.pixel_bit = pixel_bit;
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
	dma2d_start_transfer();
#if 1
	while (transferCompleteDetected == 0) {
	}
	transferCompleteDetected = 0;
#else

	while (bk_dma2d_is_transfer_busy()) {
	}
#endif
//	bk_mem_dump_ex("dma2d_fill", (unsigned char *)addr, fill.frame_xsize*fill.frame_ysize*pixel_bit);
//	LOGI(" dmmp complete \n");
}

//dma2d_memcpy_test=0x60000000,0x60020000,RGB888,4,4,4,4
void dma2d_memcpy_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	bk_psram_init();
	bk_dma2d_driver_init();
	uint32_t input_addr = os_strtoul(argv[1], NULL, 16) & 0xFFFFFFFF;
	uint32_t output_addr = os_strtoul(argv[2], NULL, 16) & 0xFFFFFFFF;

	input_color_mode_t input_color_mode;
	uint8_t pixel_bit; 
	if (os_strcmp(argv[3], "ARGB8888") == 0)
	{
		input_color_mode = DMA2D_OUTPUT_ARGB8888;
		pixel_bit = 4;
	}
	else if (os_strcmp(argv[3], "RGB888") == 0)
	{
		input_color_mode = DMA2D_OUTPUT_RGB888;
		pixel_bit = 3;
	}
	else //(os_strcmp(argv[2], "RGB565") == 0)
	{
		input_color_mode = DMA2D_OUTPUT_RGB565;
		pixel_bit = 2;
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
	bk_mem_dump_ex("src", (unsigned char *)input_addr, src_width*src_height*pixel_bit);
	bk_mem_dump_ex("dst", (unsigned char *)output_addr, src_width*src_height*pixel_bit);

	dma2d_memcpy_pfc_t dma2d_memcpy_pfc = {0};
	dma2d_memcpy_pfc.mode = DMA2D_M2M;
	dma2d_memcpy_pfc.input_addr = (char *)input_addr;
	dma2d_memcpy_pfc.output_addr = (char *)output_addr;
	dma2d_memcpy_pfc.input_color_mode = input_color_mode;
	dma2d_memcpy_pfc.output_color_mode = input_color_mode;
	dma2d_memcpy_pfc.src_pixel_bit = pixel_bit;
	dma2d_memcpy_pfc.dst_pixel_bit = pixel_bit;
	dma2d_memcpy_pfc.dma2d_height = xsize;
	dma2d_memcpy_pfc.dma2d_width = ysize;
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
	dma2d_start_transfer();
#if 0
	while (transferCompleteDetected == 0) {
	}
	transferCompleteDetected = 0;
#else
	while (bk_dma2d_is_transfer_busy()) {
	}
#endif
	bk_mem_dump_ex("2 dst", (unsigned char *)output_addr, src_width*src_height*pixel_bit);

	for(int i = 0; i < ysize; i++)
	{
		int ret = os_memcmp((unsigned char *)(input_addr + ((src_frame_ypos + i) * src_width + src_frame_xpos)*pixel_bit), (unsigned char *)(output_addr + ((dst_frame_ypos + i) * dst_width + dst_frame_xpos)*pixel_bit), xsize*pixel_bit);

		if(ret == 0)
		{
			LOGI(" test pass \n");
		}
		else
		{
			LOGI(" test fail \n");
		}
	}

	//bk_mem_dump_ex("dma2d_fill", (unsigned char *)addr, fill.frame_xsize*fill.frame_ysize*pixel_bit);

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
	uint8_t src_pixel_bit, dst_pixel_bit;
	
	if (os_strcmp(argv[3], "ARGB8888") == 0)
	{
		input_color_mode = DMA2D_INPUT_ARGB8888;
		src_pixel_bit = 4;
	}
	else if (os_strcmp(argv[3], "RGB888") == 0)
	{
		input_color_mode = DMA2D_INPUT_RGB888;
		src_pixel_bit = 3;
	}
	else //(os_strcmp(argv[2], "RGB565") == 0)
	{
		input_color_mode = DMA2D_INPUT_RGB565;
		src_pixel_bit = 2;
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
		dst_pixel_bit = 4;
	}
	else if (os_strcmp(argv[12], "RGB888") == 0)
	{
		output_color_mode = DMA2D_OUTPUT_RGB888;
		dst_pixel_bit = 3;
	}
	else //(os_strcmp(argv[2], "RGB565") == 0)
	{
		output_color_mode = DMA2D_OUTPUT_RGB565;
		dst_pixel_bit = 2;
	}
	LOGI(" %x %x, %d, %d, %d, %d,%d, %d, %d, %d, %d\n", input_addr,output_addr,input_color_mode,xsize, ysize ,src_width,src_height ,src_frame_xpos,src_frame_ypos, dst_frame_xpos,dst_frame_ypos,output_color_mode);
	LOGI("dst_width dst_height %d %d\n",dst_width, dst_height);

	if(xsize < 16)
	{
		bk_mem_dump_ex("src", (unsigned char *)input_addr, src_width*src_height*src_pixel_bit);
		bk_mem_dump_ex("dst", (unsigned char *)output_addr, src_width*src_height*dst_pixel_bit);
	}
	dma2d_memcpy_pfc_t dma2d_memcpy_pfc = {0};
	dma2d_memcpy_pfc.mode = DMA2D_M2M_PFC;
	dma2d_memcpy_pfc.input_addr = (char *)input_addr;
	dma2d_memcpy_pfc.output_addr = (char *)output_addr;
	dma2d_memcpy_pfc.input_color_mode = input_color_mode;
	dma2d_memcpy_pfc.output_color_mode = output_color_mode;
	dma2d_memcpy_pfc.src_pixel_bit = src_pixel_bit;
	dma2d_memcpy_pfc.dst_pixel_bit = dst_pixel_bit;
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

	dma2d_start_transfer();
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
		bk_mem_dump_ex("2 dst", (unsigned char *)output_addr, src_width*src_height*dst_pixel_bit);
	}
	else
	{
		bk_mem_dump_ex("3 dst", (unsigned char *)output_addr, 16*16*dst_pixel_bit);
		bk_mem_dump_ex("4 dst", (unsigned char *)(output_addr + (src_width*src_height-16)*dst_pixel_bit ), 16*dst_pixel_bit);
	}
}



/*
sdcard_read=rgb565_blue_320_480.rgb,0x60000000  //RGB565 FG
sdcard_read=yuyv422_320_480.rgb,0x60050000     // RGB565 BG
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
	uint8_t fg_pixel_bit, bg_pixel_bit, out_pixel_bit;
	
	if (os_strcmp(argv[4], "ARGB8888") == 0) {
		input_fg_mode = DMA2D_INPUT_ARGB8888;
		fg_pixel_bit = 4;
	} else if (os_strcmp(argv[4], "RGB888") == 0) {
		input_fg_mode = DMA2D_INPUT_RGB888;
		fg_pixel_bit = 3;
	} else { /*(os_strcmp(argv[2], "RGB565") == 0)*/
		input_fg_mode = DMA2D_INPUT_RGB565;
		fg_pixel_bit = 2;
	}
	if (os_strcmp(argv[5], "ARGB8888") == 0) {
		input_bg_mode = DMA2D_INPUT_ARGB8888;
		bg_pixel_bit = 4;
	} else if (os_strcmp(argv[5], "RGB888") == 0) {
		input_bg_mode = DMA2D_INPUT_RGB888;
		bg_pixel_bit = 3;
	} else /*(os_strcmp(argv[2], "RGB565") == 0)*/{
		input_bg_mode = DMA2D_INPUT_RGB565;
		bg_pixel_bit = 2;
	}
	if (os_strcmp(argv[6], "ARGB8888") == 0) {
		output_mode = DMA2D_OUTPUT_ARGB8888;
		out_pixel_bit = 4;
	} else if (os_strcmp(argv[6], "RGB888") == 0) {
		output_mode = DMA2D_OUTPUT_RGB888;
		out_pixel_bit = 3;
	} else /*(os_strcmp(argv[2], "RGB565") == 0)*/{
		output_mode = DMA2D_OUTPUT_RGB565;
		out_pixel_bit = 2;
	}
	uint16_t xsize= os_strtoul(argv[7], NULL, 10) & 0xFFFF;
	uint16_t ysize = os_strtoul(argv[8], NULL, 10) & 0xFFFF;
	fg_alpha_mode= os_strtoul(argv[9], NULL, 10) & 0xFF;
	bg_alpha_mode = os_strtoul(argv[10], NULL, 10) & 0xFF;
	uint8_t fg_alpha_value= os_strtoul(argv[11], NULL, 16) & 0xFF;
	uint8_t bg_alpha_value = os_strtoul(argv[12], NULL, 16) & 0xFF;


	LOGI(" %x %x, %x, %d, %d\n", input_fg_addr,input_bg_addr,output_blend_addr,xsize, ysize);
	
	bk_mem_dump_ex("input_fg_addr start ", (unsigned char *)input_fg_addr, 16*fg_pixel_bit);
	bk_mem_dump_ex("input_fg_addr end", (unsigned char *)(input_fg_addr + (xsize*ysize-16)*fg_pixel_bit), 16*fg_pixel_bit);
	bk_mem_dump_ex("input_bg_addr start", (unsigned char *)input_bg_addr, 16*bg_pixel_bit);
	bk_mem_dump_ex("input_bg_addr end", (unsigned char *)(input_bg_addr + (xsize*ysize-16)*bg_pixel_bit ), 16*bg_pixel_bit);

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
	dma2d_start_transfer();

	while (transferCompleteDetected == 0) {
	}
	transferCompleteDetected = 0;

	bk_mem_dump_ex("output_blend_addr start", (unsigned char *)output_blend_addr, 16*out_pixel_bit);
	bk_mem_dump_ex("output_blend_addr end", (unsigned char *)(output_blend_addr + (xsize*ysize-16)*out_pixel_bit ), 16*out_pixel_bit);

//	LOGI("=====================check fg addr data changed why ?? =====================\n");
//	bk_mem_dump_ex("input_fg_addr start ", (unsigned char *)input_fg_addr, 16*fg_pixel_bit);
//	bk_mem_dump_ex("input_fg_addr end", (unsigned char *)(input_fg_addr + (xsize*ysize-16)*fg_pixel_bit), 16*fg_pixel_bit);
//	bk_mem_dump_ex("input_bg_addr start", (unsigned char *)input_bg_addr, 16*bg_pixel_bit);
//	bk_mem_dump_ex("input_bg_addr end", (unsigned char *)(input_bg_addr + (xsize*ysize-16)*bg_pixel_bit ), 16*bg_pixel_bit);
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

#else
void dma2d_fill(uint32_t frameaddr, uint16_t x, uint16_t y, uint16_t width, uint16_t high, uint16_t color)
{
	void *pDiSt=&(((uint16_t *)frameaddr)[y*320+x]);

	dma2d_config_t dma2d_config = {0};

	dma2d_config.init.mode   = DMA2D_R2M; 			 /**< Mode Register to Memory */
	dma2d_config.init.color_mode	   = DMA2D_OUTPUT_RGB565;  /**< DMA2D Output color mode is ARGB4444 (16 bpp) */
	dma2d_config.init.output_offset  = 320 - width;					 /**< No offset in output */
	dma2d_config.init.red_blue_swap   = DMA2D_RB_REGULAR;		 /**< No R&B swap for the output image */
	dma2d_config.init.alpha_inverted = DMA2D_REGULAR_ALPHA;	 /**< No alpha inversion for the output image */
	bk_dma2d_driver_init(&dma2d_config);

/*	bk_dma2d_int_config(DMA2D_CFG_ERROR | DMA2D_TRANS_ERROR | DMA2D_TRANS_COMPLETE ,1);
#if (USE_HAL_DMA2D_REGISTER_CALLBACKS == 1)
	bk_dma2d_register_int_callback_isr(DMA2D_TRANS_ERROR_ISR, mda2d_r2m_transfer_error);
	bk_dma2d_register_int_callback_isr(DMA2D_TRANS_COMPLETE_ISR, mda2d_r2m_transfer_complete);
#else
	bk_dma2d_isr_register(dma2d_isr);
#endif */

	bk_dma2d_start_transfer(&dma2d_config, color, (uint32_t)pDiSt, width, high); 

/*	while (transferCompleteDetected == 0) {;}
	transferCompleteDetected = 0;*/
	while (bk_dma2d_is_transfer_busy()) {
	}
}


/**
  * @brief dma2d mem to mem mode, dma2d memcpy function
           attention: data format is rgb565
  * @param1  input pixelformat, copy data format
  * @param2  Psrc, source data addr
  * @param3  Pdst, destination data addr
  * @param4  xsize the display mem x size
  * @param5  ysize the display mem y size
  * @param6  src_offline. copy src mem data from offsetline addr,offsetline is calculate by pixel
  * @param6  dest_offline. src mem data copy to destination offsetline addr, offsetline is calculate by pixel
  * @return none
  */

static void dma2d_memcpy(uint32_t pixelformat, void *Psrc, void *Pdst, uint32_t xsize, uint32_t ysize, uint32_t src_offline, uint32_t dest_offline)
{
	dma2d_config_t dma2d_config = {0};

	/*##-1- Configure the DMA2D Mode, Output Color Mode and output offset #############*/
	dma2d_config.init.mode         = DMA2D_M2M;             /**< Mode Memory To Memory */
	dma2d_config.init.color_mode    = DMA2D_OUTPUT_RGB565; /**< Output color mode is ARGB4444 : 16 bpp */
	dma2d_config.init.output_offset = dest_offline;                   /**< No offset on output */
	dma2d_config.init.red_blue_swap   = DMA2D_RB_REGULAR;     /**< No R&B swap for the output image */
	dma2d_config.init.alpha_inverted = DMA2D_REGULAR_ALPHA;  /**< No alpha inversion for the output image */

	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].alpha_mode = DMA2D_NO_MODIF_ALPHA;      /**< Keep original Alpha from ARGB4444 input */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_alpha = 0xFF;                     /**< Fully opaque */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_color_mode = pixelformat; /**< Input color is ARGB4444 : 16 bpp */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_offset = src_offline;                     /**< No offset in input */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].red_blue_swap   = DMA2D_RB_REGULAR;      /**< No R&B swap for the input image */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].alpha_inverted = DMA2D_REGULAR_ALPHA;   /**< No alpha inversion for the input image */

	bk_dma2d_driver_init(&dma2d_config);
	bk_dma2d_layer_config(&dma2d_config, DMA2D_FOREGROUND_LAYER);

	bk_dma2d_start_transfer(&dma2d_config, (uint32_t)Psrc, (uint32_t)Pdst, xsize, ysize); 
	while (bk_dma2d_is_transfer_busy()) {}
}


static void dma2d_display(uint16_t index, void *pdst)
{
	void * pdist = (void *) pdst;
	uint16_t *pstart = (uint16_t *)love;
	pstart += (index/PICTURE_COUNT_ROW) * (PICTURE_WIDTH_PIXEL * PICTURE_HIGHT * PICTURE_COUNT_ROW);
	pstart += (index % PICTURE_COUNT_ROW) * PICTURE_WIDTH_PIXEL;
	uint32_t offlinesrc = (PICTURE_COUNT_ROW - 1) * PICTURE_WIDTH_PIXEL;
	uint32_t offlinedist = 320 - PICTURE_WIDTH_PIXEL;

	dma2d_memcpy(DMA2D_OUTPUT_RGB565, (void *)pstart, pdist, PICTURE_WIDTH_PIXEL, PICTURE_HIGHT, offlinesrc, offlinedist);
}

void dma2d_memcpy_display(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t pdst;
	
	pdst = os_strtoul(argv[1], NULL, 16) & 0xFFFFFFFF;
	os_printf("pdst = 0x%x . \r\n", pdst);
		
	//while(1) {
	for (int i = 0; i<	PICTURE_COUNT ; i++) {
			dma2d_display(i, (void *)pdst);
			//dma_start_transfer(lcd_dma_id);
		}
	//}
}

/**
  * @brief dma2d mem to mem with blending mode
           attention: data format is rgb565
  * @param1  pFgaddr, foreground layer data addr
  * @param2  pBgaddr, background layer data addr
  * @param3  Pdst, blend destination data addr
  * @param4  fg_offline foreground layer blend from fg_offline addr, offsetline is calculate by pixel
  * @param5  bg_offline bg_offline layer blend from fg_offline addr,offsetline is calculate by pixel
  * @param6  dest_offline. blend complete,and display on dest addr edst_offline
  * @param7  xsize the display lcd x size
  * @param8  ysize the display lcd y size
  * @param9  alpha_value  foreground layer alpha_value
  * @return none
  */
static void dma2d_blend(void *pFgaddr, void *pBgaddr, void *pDst,
							uint32_t fg_offline, uint32_t bg_offline, uint32_t dest_offline,
							uint16_t xsize, uint16_t ysize, int8_t alpha_value)
{
	dma2d_config_t dma2d_config = {0};

	/*##-1- Configure the DMA2D Mode, Output Color Mode and output offset #############*/
	dma2d_config.init.mode         = DMA2D_M2M_BLEND;                  /**< Mode Memory To Memory */
	dma2d_config.init.color_mode    = DMA2D_OUTPUT_RGB565;            /**< output format of DMA2D */
	dma2d_config.init.output_offset= dest_offline;   /**< output offset */
	dma2d_config.init.red_blue_swap   = DMA2D_RB_REGULAR;                /**< No R&B swap for the output image */
	dma2d_config.init.alpha_inverted = DMA2D_REGULAR_ALPHA;             /**< No alpha inversion for the output image */

	/**< Foreground layer Configuration */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].alpha_mode = DMA2D_REPLACE_ALPHA;    /**< Keep original Alpha from ARGB4444 input */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_alpha = alpha_value;                   /**< 127 : semi-transparent */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_color_mode = DMA2D_INPUT_RGB565; /**< Foreground color is RGB565 : 16 bpp */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_offset = fg_offline;                   /**< No offset in input */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].red_blue_swap   = DMA2D_RB_REGULAR;    /**< No R&B swap for the input image */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].alpha_inverted = DMA2D_REGULAR_ALPHA; /**< No alpha inversion for the input image */

	/**< Background layer Configuration */
	dma2d_config.layer_cfg[DMA2D_BACKGROUND_LAYER].alpha_mode  = DMA2D_REPLACE_ALPHA;
	dma2d_config.layer_cfg[DMA2D_BACKGROUND_LAYER].input_alpha   = 0x80;                            /**< 255 : fully opaque */
	dma2d_config.layer_cfg[DMA2D_BACKGROUND_LAYER].input_color_mode = DMA2D_INPUT_RGB565;          /**< Background format is ARGB8888*/
	dma2d_config.layer_cfg[DMA2D_BACKGROUND_LAYER].input_offset    = bg_offline;/**< Background input offset*/
	dma2d_config.layer_cfg[DMA2D_BACKGROUND_LAYER].red_blue_swap    = DMA2D_RB_REGULAR;              /**< No R&B swap for the input background image */
	dma2d_config.layer_cfg[DMA2D_BACKGROUND_LAYER].alpha_inverted  = DMA2D_REGULAR_ALPHA;           /**< No alpha inversion for the input background image */

	bk_dma2d_driver_init(&dma2d_config);
	bk_dma2d_layer_config(&dma2d_config, DMA2D_FOREGROUND_LAYER);
	bk_dma2d_layer_config(&dma2d_config, DMA2D_BACKGROUND_LAYER);

//	bk_dma2d_int_config(DMA2D_TRANS_ERROR | DMA2D_TRANS_COMPLETE ,1);
//	bk_dma2d_isr_register(dma2d_isr);

	/*## RGB565_240x130_1[] is the foreground layer and LCD_FRAME_BUFFER is the background layer */
	bk_dma2d_start_blending(&dma2d_config,
                                  (uint32_t)pFgaddr,                                                      /**< Foreground image */
                                  (uint32_t)pBgaddr,   /**< Background image */
                                  (uint32_t)pDst,  /**< Destination address */
                                  xsize ,                                                                    /**< width in pixels   */
                                  ysize);                                                                    /**< height in pixels   */


	while (bk_dma2d_is_transfer_busy()) {}
}

void bk_example_dma2d_rgb565_to_argb8888pixel(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t RedBlueSwapConfig = 0;

	RedBlueSwapConfig = os_strtoul(argv[1], NULL, 10) & 0xFFFF;
	os_printf("RedBlueSwapConfig = %d \r\n", RedBlueSwapConfig);

	dma2d_config_t dma2d_config = {0};

	/*##-1- Configure the DMA2D Mode, Output Color Mode and output offset #############*/
	dma2d_config.init.mode         = DMA2D_M2M_PFC;                  /**< DMA2D Mode memory to memory  with PFC*/
	dma2d_config.init.color_mode    = DMA2D_OUTPUT_ARGB8888;          /**< output format of DMA2D */
	dma2d_config.init.output_offset = 0; 
	dma2d_config.init.red_blue_swap   = DMA2D_RB_REGULAR;               /**< No R&B swap for the output image */
	dma2d_config.init.alpha_inverted = DMA2D_REGULAR_ALPHA;            /**< No alpha inversion for the output image */

	/**< Foreground layer Configuration */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].alpha_mode = DMA2D_NO_MODIF_ALPHA;    /**< Keep original Alpha from ARGB4444 input */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_alpha = 0xFF;                   /**< Fully opaque */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_color_mode = DMA2D_INPUT_RGB565; /**< Input color is RGB565 : 16 bpp */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_offset = 0x0;                   /**< No offset in input */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].red_blue_swap   = RedBlueSwapConfig;   /**< No R&B swap for the input image */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].alpha_inverted = DMA2D_REGULAR_ALPHA; /**< No alpha inversion for the input image */

	bk_dma2d_driver_init(&dma2d_config);
	bk_dma2d_layer_config(&dma2d_config, DMA2D_FOREGROUND_LAYER);

	bk_dma2d_start_transfer(&dma2d_config,
                                  (uint32_t)LCD_FRAMEADDR, /**< Source buffer in format RGB565 and size 320x240      */
                                  (uint32_t)(LCD_FRAMEADDR + 0x50000),/**< framebuf+2*(LCD_X_SIZE*sy+sx)*/
                                  IMAGE_SIZE_X,           /**< width in pixels  */
                                  IMAGE_SIZE_Y);        /**< height in pixels */

	while (bk_dma2d_is_transfer_busy()) {}
}

void bk_example_dma2d_argb8888_to_rgb565pixel(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t RedBlueSwapConfig = 0;

	RedBlueSwapConfig = os_strtoul(argv[1], NULL, 10) & 0xFFFF;
	os_printf("RedBlueSwapConfig = %d \r\n", RedBlueSwapConfig);

	dma2d_config_t dma2d_config = {0};

	/*##-1- Configure the DMA2D Mode, Output Color Mode and output offset #############*/
	dma2d_config.init.mode         = DMA2D_M2M_PFC;                  /**< DMA2D Mode memory to memory  with PFC*/
	dma2d_config.init.color_mode    = DMA2D_OUTPUT_RGB565;          /**< output format of DMA2D */
	dma2d_config.init.output_offset = 0; 
	dma2d_config.init.red_blue_swap   = DMA2D_RB_REGULAR;               /**< No R&B swap for the output image */
	dma2d_config.init.alpha_inverted = DMA2D_REGULAR_ALPHA;            /**< No alpha inversion for the output image */

	/**< Foreground layer Configuration */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].alpha_mode = DMA2D_NO_MODIF_ALPHA;    /**< Keep original Alpha from ARGB4444 input */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_alpha = 0xFF;                   /**< Fully opaque */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_color_mode = DMA2D_INPUT_ARGB8888; /**< Input color is RGB565 : 16 bpp */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_offset = 0x0;                   /**< No offset in input */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].red_blue_swap   = RedBlueSwapConfig;   /**< No R&B swap for the input image */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].alpha_inverted = DMA2D_REGULAR_ALPHA; /**< No alpha inversion for the input image */

	bk_dma2d_driver_init(&dma2d_config);
	bk_dma2d_layer_config(&dma2d_config, DMA2D_FOREGROUND_LAYER);

	bk_dma2d_start_transfer(&dma2d_config,
                                  (uint32_t)(LCD_FRAMEADDR + 0x50000), /**< Source buffer in format RGB565 and size 320x240      */
                                  (uint32_t)LCD_FRAMEADDR,/**< framebuf+2*(LCD_X_SIZE*sy+sx)*/
                                  IMAGE_SIZE_X,           /**< width in pixels  */
                                  IMAGE_SIZE_Y);        /**< height in pixels */
	while (bk_dma2d_is_transfer_busy()) {}
	//dma_start_transfer(lcd_dma_id);
}

void bk_example_alpha_blend(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	const uint8_t cnvFrames = 60;

	///calculate address of output
	offset_address_area_blended_image_in_lcd_buffer =  ((((LCD_Y_SIZE - DEMO_IMG_HEIGHT) / 2) * LCD_X_SIZE)
                                                    + ((LCD_X_SIZE - DEMO_IMG_WIDTH) / 2)) * 2;
	//uint16_t *pFb = &RGB565_320x480[0];
	//uint16_t *pDiSt = pFb + offset_address_area_blended_image_in_lcd_buffer;
	uint16_t *pDiSt = (void *)0x60000000 + offset_address_area_blended_image_in_lcd_buffer;

	uint16_t dist_offline = LCD_X_SIZE - DEMO_IMG_WIDTH;
	uint32_t nextimg = 1;
	uint32_t opa = 0;
	void * pFgaddr = 0;
	void * pBgaddr = 0;
//	dma_int_flag  = 0;

	os_printf("pFb addr = %x \r\n", offset_address_area_blended_image_in_lcd_buffer);
	os_printf("pDiSt addr = %x \r\n", pDiSt);

	while(1){
		if(nextimg){
				pFgaddr = (void*)yellow_color;
				pBgaddr = (void*)love;
		}
		else{
			pFgaddr = (void*)love;
			pBgaddr = (void*)yellow_color;
		}
		for(int i = 0; i < cnvFrames; i++) {
			opa = 255 * i / (cnvFrames-1);
			dma2d_blend(pFgaddr, pBgaddr, pDiSt,0, 0, dist_offline,DEMO_IMG_WIDTH, DEMO_IMG_HEIGHT, 
							opa);
			//dma_start_transfer(lcd_dma_id);
			//while(dma_int_flag == 0) {}
			//dma_int_flag = 0;
		}
		nextimg = !nextimg;
	}
}

void bk_example_fill_2p_alpha_blend(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	const uint8_t cnvFrames = 60;
	uint32_t nextimg = 1;
	uint32_t opa = 0;
	void * pFgaddr = 0;
	void * pBgaddr = 0;
//	dma_int_cnt  = 0;
//	dma_int_flag  = 0;

	uint16_t *pDiSt = (void *)LCD_FRAMEADDR;

	dma2d_fill(0x60050000, 0, 0, 320, 480, 0xffff);
	dma2d_fill(0x60050000, 20, 20, 280, 440, 0xffe0);
	dma2d_fill(0x60050000, 60, 60, 200, 360, 0x001f);
	dma2d_fill(0x60050000, 100, 100, 120, 280, 0xf60F);
	dma2d_fill(0x60050000, 140, 140, 40, 200, 0x07ff);
	
	dma2d_fill(0x60100000, 0,0,320,480,0xffff);
	dma2d_fill(0x60100000, 270,80,10,340,0xf800);
	dma2d_fill(0x60100000, 220,420,60,10,0xf800);
	dma2d_fill(0x60100000, 220,80,10,350,0xf800);
	dma2d_fill(0x60100000, 220,250,60,10,0xf800);
	dma2d_fill(0x60100000, 120,80,60,10,0xf800);
	dma2d_fill(0x60100000, 180,80,10,350,0xf800);
	dma2d_fill(0x60100000, 120,420,60,10,0xf800);
	dma2d_fill(0x60100000, 30,80, 60,10,0xf800);
	dma2d_fill(0x60100000, 80,80, 10,350,0xf800);

	while(1) {
		if(nextimg){
			pFgaddr = (void*)0x60050000;
			pBgaddr = (void*)0x60100000;
		}
		else {
			pFgaddr = (void*)0x60100000;
			pBgaddr = (void*)0x60050000;
		}
		for (int i = 0; i < cnvFrames; i++) {
			opa = 255 * i / (cnvFrames-1);
			dma2d_blend(pFgaddr, pBgaddr, pDiSt, 0, 0, 0, 320, 480, opa);
//			dma_start_transfer(lcd_dma_id);
//			while(dma_int_flag == 0) {}
//			dma_int_flag = 0;
		}
		nextimg = !nextimg;
	}
}

void bk_example_dma2d_deinit(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	bk_err_t ret = BK_OK;
//	dma_int_flag  = 0;

	ret = bk_dma2d_driver_deinit();
	if (ret != BK_OK) {
		os_printf("dma2d driver init failed\r\n");
		return;
	}
}
#endif


