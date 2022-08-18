#include <common/bk_include.h>
#include <os/mem.h>
#include <os/str.h>
#include <os/os.h>
#include <stdlib.h>
#include <driver/dma2d.h>
#include "modules/image_scale.h"
#include <lcd_dma2d_config.h>

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
void dma2d_lcd_fill(uint32_t frameaddr, uint16_t x, uint16_t y, uint16_t width, uint16_t high, uint16_t color)
{
	void *pDiSt=&(((uint16_t *)frameaddr)[y*320+x]);

	dma2d_config_t dma2d_config = {0};

	dma2d_config.init.mode   = DMA2D_R2M;                      /**< Mode Register to Memory */
	dma2d_config.init.color_mode	   = DMA2D_OUTPUT_RGB565;  /**< DMA2D Output color mode is ARGB4444 (16 bpp) */
	dma2d_config.init.output_offset  = 320 - width;            /**< offset in output */
	dma2d_config.init.red_blue_swap   = DMA2D_RB_REGULAR;       /**< No R&B swap for the output image */
	dma2d_config.init.alpha_inverted = DMA2D_REGULAR_ALPHA;     /**< No alpha inversion for the output image */
	bk_dma2d_driver_init(&dma2d_config);
	bk_dma2d_start_transfer(&dma2d_config, color, (uint32_t)pDiSt, width, high);
	while (bk_dma2d_is_transfer_busy()) {
	}
}

void dm2d_lcd_create_picture(uint32_t frameaddr, uint16_t width, uint16_t height, uint16_t color)
{
	dma2d_config_t dma2d_config = {0};

	dma2d_config.init.mode   = DMA2D_R2M;                      /**< Mode Register to Memory */
	dma2d_config.init.color_mode	   = DMA2D_OUTPUT_RGB565;  /**< DMA2D Output color mode is ARGB4444 (16 bpp) */
	dma2d_config.init.output_offset  = 0;                      /**< offset in output */
	dma2d_config.init.red_blue_swap   = DMA2D_RB_REGULAR;       /**< No R&B swap for the output image */
	dma2d_config.init.alpha_inverted = DMA2D_REGULAR_ALPHA;     /**< No alpha inversion for the output image */
	bk_dma2d_driver_init(&dma2d_config);
	bk_dma2d_start_transfer(&dma2d_config, color, (uint32_t)frameaddr, width, height);
	while (bk_dma2d_is_transfer_busy()) {
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

void bk_example_dma2d_rgb888_to_arg565pixel(uint32_t srcaddr, uint32_t dstaddr, uint32_t x_pixel, uint32_t y_pixel)
{
	uint32_t RedBlueSwapConfig = 0;

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
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_color_mode = DMA2D_INPUT_RGB888; /**< Input color is RGB565 : 16 bpp */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].input_offset = 0x0;                   /**< No offset in input */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].red_blue_swap   = RedBlueSwapConfig;   /**< No R&B swap for the input image */
	dma2d_config.layer_cfg[DMA2D_FOREGROUND_LAYER].alpha_inverted = DMA2D_REGULAR_ALPHA; /**< No alpha inversion for the input image */

	bk_dma2d_driver_init(&dma2d_config);
	bk_dma2d_layer_config(&dma2d_config, DMA2D_FOREGROUND_LAYER);

	bk_dma2d_start_transfer(&dma2d_config,
                                  (uint32_t)srcaddr, /**< Source buffer in format RGB565 and size 320x240      */
                                  (uint32_t)(dstaddr),/**< framebuf+2*(LCD_X_SIZE*sy+sx)*/
                                  x_pixel,           /**< width in pixels  */
                                  y_pixel);        /**< height in pixels */

	while (bk_dma2d_is_transfer_busy()) {}
}

void dma2d_blend_rgb565_data(void *p_fg_addr, void *p_bg_addr, void *p_dst_addr,
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


void yuv_blend(uint32_t bgimg_addr, uint32_t fgimg_addr)
{
	unsigned char *p_bgImg = (unsigned char *) bgimg_addr;
	unsigned char *p_fgImg = (unsigned char *) fgimg_addr;

	uint32_t src_w = 480;
	uint32_t src_h = 272;
	uint32_t dst_w = 100;
	uint32_t dst_h = 60;
//	uint32_t dst_w = 120;
//	uint32_t dst_h = 56;
	uint32_t alpha_value = 40;

	uint32_t x = src_w - dst_w;
	uint32_t y = src_h - dst_h;
	uint32_t bg_offset = src_w - dst_w;
	uint32_t addr_temp = src_h*src_w*2;
	uint32_t dst_addr_temp = dst_w*dst_h*2;
	uint16_t *pbg_offstart = (uint16_t *)p_bgImg + (y * src_w) + x;
	unsigned char *p_crop = (unsigned char *) (p_bgImg + addr_temp);
	unsigned char *p_crop2rgb = (unsigned char *) (p_crop + dst_addr_temp);

	/*crop yuv -- 0.74ms*/
	dma2d_memcpy_rgb565_data((uint32_t)pbg_offstart, (uint32_t)p_crop, bg_offset, 0, dst_w, dst_h);
//	if(yuv_mode == YUYV) {
	/*yuv to rgb565 --2.88ms*/
		yuyv_to_rgb565_convert(p_crop, p_crop2rgb, dst_w, dst_h);
//	} else if (yuv_mode == UYVY) {
//		uyvy_to_rgb565_convert(p_crop, p_crop2rgb, dst_w, dst_h);
//	} else {
//		os_printf("not support yuv mode");
//	}

	/*rgb blend --1.1ms*/
	dma2d_blend_rgb565_data(p_fgImg, p_crop2rgb, p_crop2rgb, 0, 0, 0, dst_w, dst_h, alpha_value);

//	if(yuv_mode == YUYV) { 	
	/*rgb2yuv --1.9ms*/
		rgb565_to_yuyv_convert((uint16_t *)p_crop2rgb, (uint16_t *)p_crop, dst_w, dst_h);
//	} else if (yuv_mode == UYVY) {
//		rgb565_to_uyvy_convert((uint16_t *)p_crop2rgb, (uint16_t *)p_crop, dst_w, dst_h);
//	} else {
//		os_printf("not support yuv mode");
//	}

	/*dma2d fill yuv to bg img --0.77ms*/
	dma2d_memcpy_rgb565_data((uint32_t)p_crop, (uint32_t)pbg_offstart, 0, bg_offset, dst_w, dst_h);
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



