/**
 * @file lvgl_vendor.h
 */

#ifndef LVGL_VENDOR_H
#define LVGL_VENDOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"
#include "lv_color.h"
#include "driver/media_types.h"

typedef struct {
	uint32_t draw_pixel_size;
	lv_color_t *draw_buf_2_1;
	lv_color_t *draw_buf_2_2;
	lv_color_t *sram_frame_buf_1;    //If you use sram as draw buf, this is the same size as the screen.
	lv_color_t *sram_frame_buf_2;    //If you use sram as draw buf, this is the same size as the screen.

	//If you want to flush lcd by yourself, you can set this function and realize like this https://docs.lvgl.io/master/porting/disp.html
	void (*flush_cb)(struct _lv_disp_drv_t * disp_drv, const lv_area_t * area, void * color_p);
	
	media_rotate_t rotation;
	uint8_t color_depth;
} lv_vnd_config_t;

#define LV_NOTIFY_TIMER_DURATION       10

void lv_vendor_init(lv_vnd_config_t *config, uint16_t width, uint16_t height);
void lv_vendor_start(void);
void lv_vendor_stop(void);

void lv_vendor_disp_lock(void);
void lv_vendor_disp_unlock(void);



#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LVGL_VENDOR_H*/
