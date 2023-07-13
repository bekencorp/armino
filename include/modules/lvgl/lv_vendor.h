/**
 * @file lvgl_vendor.h
 */

#ifndef LVGL_VENDOR_H
#define LVGL_VENDOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lv_color.h"
#include "driver/media_types.h"

typedef struct {
	uint32_t draw_pixel_size;
	lv_color_t *draw_buf_2_1;
	lv_color_t *draw_buf_2_2;
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
