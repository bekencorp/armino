#include <os/os.h>
#include <os/str.h>
#include "lvgl.h"
#include "lcd_task.h"

#if (CONFIG_LCD) && (CONFIG_LVGL) && (CONFIG_LVGL_DEMO)

#if CONFIG_BLEND_USE_GUI
u8 g_hour = 10;
u8 g_min = 10;
u8 g_sec = 54;
static void timer_cb(struct _lv_timer_t *arg)
{
    char buff[32];
    lv_obj_t *time_label = (lv_obj_t *)arg->user_data;

    if(g_sec < 60)
    {
        g_sec += 1;
    }
    else
    {
        g_min += 1;
        g_sec = 0;
    }

    if(g_min >= 60)
    {
        g_hour += 1;
        g_min = 0;
    }

    if(g_hour >= 24)
    {
        g_hour = 0;
    }
    
    os_snprintf(buff, sizeof(buff), "%02d:%02d:%02d", g_hour, g_min, g_sec);
    lv_label_set_text(time_label, buff);
}
#else
static lv_obj_t * meter;

static void set_value(void * indic, int32_t v)
{
    lv_meter_set_indicator_end_value(meter, indic, v);
}
#endif

/**
 * A meter with multiple arcs
 */
void lv_example_meter(int value)
{
#if CONFIG_BLEND_USE_GUI
    lv_obj_t *background;
    lv_obj_t *wifi_label;
    lv_obj_t *bat_label;
    lv_obj_t *time_label;
    char buff[32];
    
    //lv_obj_set_style_local_bg_opa
    bk_gui_disp_lock();

    #if (LV_COLOR_DEPTH == 32)
    static lv_style_t style_window;
    lv_style_init(&style_window);
    lv_style_set_bg_opa(&style_window, 0x00);
    lv_obj_add_style(lv_scr_act(), &style_window, 0);
    #endif
    
    background = lv_obj_create(lv_scr_act());
    lv_obj_set_style_radius(background, 0, 0);
    lv_obj_set_scrollbar_mode(background, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_bg_color(background, lv_color_black(), 0);
    lv_obj_set_style_border_width(background, 0, 0);
    lv_obj_set_style_pad_all(background, 0, 0);
    lv_obj_set_style_bg_opa(background, LV_OPA_20, 0);
    lv_obj_set_size(background, LV_HOR_RES, LV_VER_RES);
    
    //wifi icon
    wifi_label = lv_label_create(background);
    lv_obj_set_style_text_color(wifi_label, lv_color_white(), 0);
    lv_label_set_text(wifi_label, LV_SYMBOL_WIFI);
    lv_obj_set_style_text_font(wifi_label, &lv_font_montserrat_12, 0);
    lv_obj_align(wifi_label, LV_ALIGN_LEFT_MID, 10, 0);

    bat_label = lv_label_create(background);
    lv_obj_set_style_text_color(bat_label, lv_color_white(), 0);
    lv_label_set_text(bat_label, LV_SYMBOL_BATTERY_3);
    lv_obj_set_style_text_font(bat_label, &lv_font_montserrat_12, 0);
    lv_obj_align(bat_label, LV_ALIGN_RIGHT_MID, -10, 0);

    time_label = lv_label_create(background);
    lv_obj_set_style_text_color(time_label, lv_color_white(), 0);
    os_snprintf(buff, sizeof(buff), "%02d:%02d:%02d", g_hour, g_min, g_sec);
    lv_label_set_text(time_label, buff);
    lv_obj_set_style_text_font(time_label, &lv_font_montserrat_12, 0);
    lv_obj_align(time_label, LV_ALIGN_RIGHT_MID, -35, 0);

    lv_timer_create(timer_cb, 1000, (void *)time_label);

    bk_gui_disp_unlock();
#else
    void bk_gui_set_dma2d_swith(int onoff);

    bk_gui_set_dma2d_swith(0);
    
    bk_gui_disp_lock();

    lv_disp_set_bg_color(lv_disp_get_default(), lv_color_white());
    meter = lv_meter_create(lv_scr_act());
    lv_obj_center(meter);
    lv_obj_set_size(meter, 200, 200);

    /*Remove the circle from the middle*/
    lv_obj_remove_style(meter, NULL, LV_PART_INDICATOR);

    /*Add a scale first*/
    lv_meter_scale_t * scale = lv_meter_add_scale(meter);
    lv_meter_set_scale_ticks(meter, scale, 11, 2, 10, lv_palette_main(LV_PALETTE_GREY));
    lv_meter_set_scale_major_ticks(meter, scale, 1, 2, 30, lv_color_hex3(0xeee), 15);
    lv_meter_set_scale_range(meter, scale, 0, 100, 270, 90);

    /*Add a three arc indicator*/
    lv_meter_indicator_t * indic1 = lv_meter_add_arc(meter, scale, 10, lv_palette_main(LV_PALETTE_RED), 0);
    lv_meter_indicator_t * indic2 = lv_meter_add_arc(meter, scale, 10, lv_palette_main(LV_PALETTE_GREEN), -10);
    lv_meter_indicator_t * indic3 = lv_meter_add_arc(meter, scale, 10, lv_palette_main(LV_PALETTE_BLUE), -20);

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

    bk_gui_disp_unlock();
#endif
}

void lv_example_meter_exit(void)
{
    bk_gui_disp_lock();
    lv_anim_del_all();
    lv_obj_del(meter);
    bk_gui_disp_unlock();
}
#else
void lv_example_meter(void)
{
    bk_printf("%s %d compile null function\r\n", __FUNCTION__, __LINE__);
}
#endif

