#include <os/os.h>
#include <os/str.h>
#include "lvgl.h"
#include "lv_vendor.h"

u8 g_hour = 10;
u8 g_min = 10;
u8 g_sec = 54;


static lv_obj_t *meter;

static void set_value(void *indic, int32_t v)
{
	lv_meter_set_indicator_end_value(meter, indic, v);
}

static void timer_cb(struct _lv_timer_t *arg)
{
	char buff[32];
	lv_obj_t *time_label = (lv_obj_t *)arg->user_data;

	if (g_sec < 60)
	{
		g_sec += 1;
	}
	else
	{
		g_min += 1;
		g_sec = 0;
	}

	if (g_min >= 60)
	{
		g_hour += 1;
		g_min = 0;
	}

	if (g_hour >= 24)
	{
		g_hour = 0;
	}

	os_snprintf(buff, sizeof(buff), "%02d:%02d:%02d", g_hour, g_min, g_sec);
	lv_label_set_text(time_label, buff);
}

/**
 * A meter with multiple arcs
 */
void lv_example_meter(void)
{
	lv_vendor_disp_lock();

	lv_disp_set_bg_color(lv_disp_get_default(), lv_color_white());

	lv_obj_t *bg = lv_img_create(lv_scr_act());
#if CONFIG_VFS_DEMO_USE_FATFS
	lv_img_set_src(bg, "/bg.jpg");
#else
	lv_img_set_src(bg, "/dir1/bg.jpg");
#endif
	
	meter = lv_meter_create(bg);
	lv_obj_center(meter);
	lv_obj_set_size(meter, 200, 200);

	/*Remove the circle from the middle*/
	lv_obj_remove_style(meter, NULL, LV_PART_INDICATOR);

	/*Add a scale first*/
	lv_meter_scale_t *scale = lv_meter_add_scale(meter);
	lv_meter_set_scale_ticks(meter, scale, 11, 2, 10, lv_palette_main(LV_PALETTE_GREY));
	lv_meter_set_scale_major_ticks(meter, scale, 1, 2, 30, lv_color_hex3(0xeee), 15);
	lv_meter_set_scale_range(meter, scale, 0, 100, 270, 90);

	/*Add a three arc indicator*/
	lv_meter_indicator_t *indic1 = lv_meter_add_arc(meter, scale, 10, lv_palette_main(LV_PALETTE_RED), 0);
	lv_meter_indicator_t *indic2 = lv_meter_add_arc(meter, scale, 10, lv_palette_main(LV_PALETTE_GREEN), -10);
	lv_meter_indicator_t *indic3 = lv_meter_add_arc(meter, scale, 10, lv_palette_main(LV_PALETTE_BLUE), -20);

	/*Create an animation to set the value*/
	lv_anim_t a;
	lv_anim_init(&a);
	lv_anim_set_exec_cb(&a, set_value);
	lv_anim_set_values(&a, 0, 100);
	lv_anim_set_repeat_delay(&a, 100);
	lv_anim_set_playback_delay(&a, 100);
	lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);

	lv_anim_set_time(&a, 2000);
	lv_anim_set_playback_time(&a, 500);
	lv_anim_set_var(&a, indic1);
	lv_anim_start(&a);

	lv_anim_set_time(&a, 1000);
	lv_anim_set_playback_time(&a, 1000);
	lv_anim_set_var(&a, indic2);
	lv_anim_start(&a);

	lv_anim_set_time(&a, 1000);
	lv_anim_set_playback_time(&a, 2000);
	lv_anim_set_var(&a, indic3);
	lv_anim_start(&a);

	lv_vendor_disp_unlock();
}

void lv_example_meter_exit(void)
{
	lv_vendor_disp_lock();
	lv_anim_del_all();
	lv_obj_del(meter);
	lv_vendor_disp_unlock();
}

