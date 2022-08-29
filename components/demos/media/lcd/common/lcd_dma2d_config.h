#pragma once

#ifdef __cplusplus
extern "C" {
#endif


/* @brief Overview about lcds demo
 *
 */


typedef enum{
	YUYV = 0,
	UYVY,

} yuv_mode_t;

typedef struct{
	uint32_t src_addr; /*<  src image addr*/
	uint32_t dst_addr; /*<  dst image addr*/
	uint32_t x;         /*< src image crop x point coo*/
	uint32_t y;         /*< src image crop y point coo*/
	uint32_t src_width; /*< src image width*/
	uint32_t src_height; /*< src image height*/
	uint32_t dst_width;  /*< dst image width*/
	uint32_t dst_height;  /*< dst image height*/
}dma2d_crop_params_t;


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
void dma2d_lcd_fill(uint32_t frameaddr, uint16_t x, uint16_t y, uint16_t width, uint16_t high, uint16_t color);

/**
  * @brief dma2d reg to mem mode, dm2d_lcd_create_picture
           attention: data format is rgb565
  * @param1  the display framebuffer base addr, by defult psram 0x60000000
  * @param2  the image width
  * @param3  the image height
  * @param4  the image color
  * @return none
  */
void dm2d_lcd_create_picture(uint32_t frameaddr, uint16_t width, uint16_t height, uint16_t color);

/**
  * @brief lcd rgb yuyv bg img and rgb565 data blend
           attention:this api is for rgb lcd
  * @param1  bgimg_addr sd card read bg img addr, generally 0x60000000
  * @param2  fgimg_addr sd card read fg img addr, generally 0x60200000
  * @param3  alpha_value blending alpha_value , rang 0~255
  * @return none
  */
void yuv_blend(uint32_t bgimg_addr, uint32_t fgimg_addr);

/**
  * @brief lcd crop function
  *        crop src image to dst image
  * @param1 crop_params
  * @return none
  */
void dma2d_crop_image(dma2d_crop_params_t *crop_params);

/**
  * @brief dma2d mem to mem with blending mode
           attention: data format is rgb565
  * @param1  pFgaddr, foreground layer data addr
  * @param2  pBgaddr, background layer data addr
  * @param3  Pdst, blend destination data addr
  * @param4  fg_offline foreground layer blend from fg_offline addr, offsetline is calculate by pixel
  * @param5  bg_offline bg_offline layer blend from fg_offline addr,offsetline is calculate by pixel
  * @param6  dest_offline. blend complete,and display on dest addr edst_offline
  * @param7  xsize  blend image x size
  * @param8  ysize  blend image y size
  * @param9  alpha_value  foreground layer alpha_value
  * @return none
  */
void dma2d_blend_rgb565_data(void *p_fg_addr, void *p_bg_addr, void *p_dst_addr,
								uint32_t fg_offline, uint32_t bg_offline, uint32_t out_offset,
								uint16_t xsize, uint16_t ysize, int8_t alpha_value);

void bk_example_dma2d_rgb888_to_arg565pixel(uint32_t srcaddr, uint32_t dstaddr, uint32_t x_pixel, uint32_t y_pixel);


/*
 * @}
 */

#ifdef __cplusplus
	}
#endif


