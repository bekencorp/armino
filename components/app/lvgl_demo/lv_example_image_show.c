#include <common/bk_include.h>
#include <os/str.h>

#include "driver/lcd.h"
#include "lvgl.h"
#include "lcd_task.h"
#include "img_utility.h"
#include <components/jpeg_decode.h>

#if (CONFIG_LCD) && (CONFIG_LVGL) && (CONFIG_LVGL_DEMO)

#define DEMO_USE_NEXT_BG  0

lv_obj_t *background = NULL;
lv_obj_t *g_lab = NULL;
lv_obj_t *g_lab_image_name = NULL;
lv_obj_t *g_close_label = NULL;
u8 g_img_index = 0;
static beken_timer_t g_disp_timer = {0};

lv_img_dsc_t g_bg = {
      /*
      1. if set NULL, will use psram to malloc, so if you don't use, 
         you should use psram_free to free this memory.
      2. also this can use other psram or ram addr*/
      .data = NULL,     
    };

#if DEMO_USE_NEXT_BG
lv_img_dsc_t g_bg_next = {
      /*
      1. if set NULL, will use psram to malloc, so if you don't use, 
         you should use psram_free to free this memory.
      2. also this can use other psram or ram addr*/
      .data = NULL,     
    };

#endif
static int g_timer_flag = 0;
static void close_window(lv_event_t *event)
{
    void lv_example_img_display_exit(void);
    lv_example_img_display_exit();
}

static void _display_timer(void *param)
{
    char buff[32] = {0};
    char show_buff[128];
    int ret = 0;
    lv_obj_t *img = (lv_obj_t *)background;

#if !DEMO_USE_NEXT_BG
    if(g_img_index < 7)
        g_img_index += 1;
    else
        g_img_index = 0;

    bk_gui_disp_lock();
    os_snprintf(buff, sizeof(buff), "%d.jpg", g_img_index+1);
    ret = img_jpg_load_from_sd(buff, &g_bg);
    
    os_snprintf(show_buff, sizeof(show_buff), "%d.jpg", g_img_index+1);
    if(ret == BK_OK)
    {
        lv_obj_clear_flag(img, LV_OBJ_FLAG_HIDDEN);
        lv_img_set_src(img, &g_bg);
        lv_label_set_text(g_lab_image_name, show_buff);
    }
    else
    {
        lv_obj_add_flag(img, LV_OBJ_FLAG_HIDDEN);
        os_snprintf(show_buff, sizeof(show_buff), "can't find %d.jpg", g_img_index+1);
        lv_label_set_text(g_lab_image_name, show_buff);
    }
    bk_gui_disp_unlock();
#else
    static int next_img_status = 1;   //0异常 1正常
    if(0 == g_timer_flag)
    {
        if(g_img_index < 7)
            g_img_index += 1;
        else
            g_img_index = 0;
        
        os_snprintf(buff, sizeof(buff), "%d.jpg", g_img_index+1);
        if(g_img_index % 2 != 0)
            ret = img_jpg_load_from_sd(buff, &g_bg_next);
        else
            ret = img_jpg_load_from_sd(buff, &g_bg);
        if(BK_OK == ret)
        {
            next_img_status = 1;
        }
        else
        {
            next_img_status = 0;
        }

        g_timer_flag = 1;
    }
    else
    {
        g_timer_flag = 0;
        bk_gui_disp_lock();
        os_snprintf(show_buff, sizeof(show_buff), "%d.jpg", g_img_index+1);
        if(next_img_status)
        {
            lv_obj_clear_flag(img, LV_OBJ_FLAG_HIDDEN);
            if(g_img_index % 2 != 0)
                lv_img_set_src(img, &g_bg_next);
            else
                lv_img_set_src(img, &g_bg);
            
            lv_label_set_text(g_lab_image_name, show_buff);
        }
        else
        {
            lv_obj_add_flag(img, LV_OBJ_FLAG_HIDDEN);
            os_snprintf(show_buff, sizeof(show_buff), "can't find %d.jpg", g_img_index+1);
            lv_label_set_text(g_lab_image_name, show_buff);
        }
        bk_gui_disp_unlock();
    }
#endif
}

/**
 * A meter with multiple arcs
 */
void lv_example_img_display_init(int switch_time_ms)
{
    char buff[32] = {0};
    static lv_style_t img_style;
    
    if(!background)
    {
        bk_jpeg_dec_sw_init();
        
        g_bg.data = psram_malloc(LV_HOR_RES*LV_VER_RES*2);
        if(!g_bg.data)
        {
            bk_printf("[%s][%d] malloc failed\r\n", __FUNCTION__, __LINE__);
            return ;
        }
        
        bk_gui_disp_lock();
        
        os_snprintf(buff, sizeof(buff), "%d.jpg", g_img_index+1);
        if(BK_OK == img_jpg_load_from_sd(buff, &g_bg))
        {
            lv_style_init(&img_style);
            lv_style_set_border_width(&img_style, 0);
            lv_style_set_pad_all(&img_style, 0);
        
            background = lv_img_create(lv_scr_act());
            lv_img_set_src(background, &g_bg);
            lv_obj_set_size(background, LV_HOR_RES, LV_VER_RES);
            lv_obj_set_scrollbar_mode(background, LV_SCROLLBAR_MODE_OFF);
            lv_obj_add_style(background, &img_style, 0);

            g_lab_image_name = lv_label_create(background);
            lv_obj_set_style_text_color(g_lab_image_name, lv_color_white(), 0);
            lv_label_set_text(g_lab_image_name, buff);
            lv_obj_set_style_text_font(g_lab_image_name, &lv_font_montserrat_24, 0);
            lv_obj_set_style_bg_opa(g_lab_image_name, LV_OPA_50, 0);
            lv_obj_set_style_bg_color(g_lab_image_name, lv_color_black(), 0);
            lv_obj_align_to(g_lab_image_name, background, LV_ALIGN_TOP_MID, 0, 10);

            //wifi icon
            g_close_label = lv_label_create(background);
            lv_obj_set_style_text_color(g_close_label, lv_color_white(), 0);
            lv_label_set_text(g_close_label, LV_SYMBOL_CLOSE);
            lv_obj_set_style_text_font(g_close_label, &lv_font_montserrat_24, 0);
            lv_obj_set_style_bg_opa(g_close_label, LV_OPA_50, 0);
            lv_obj_set_style_bg_color(g_close_label, lv_color_black(), 0);
            lv_obj_align(g_close_label, LV_ALIGN_TOP_RIGHT, -20, 10);
            lv_obj_add_event_cb(g_close_label, close_window, LV_EVENT_PRESSED, NULL);
            
            if(!rtos_is_timer_init(&g_disp_timer))
            {
                rtos_init_timer(&g_disp_timer,
#if !DEMO_USE_NEXT_BG
                                 switch_time_ms,
#else
                                 switch_time_ms/2,
#endif
                                 _display_timer,
                                 (void *)background);
                
                rtos_start_timer(&g_disp_timer);
            }
            else
            {
                rtos_stop_timer(&g_disp_timer);
                rtos_reload_timer(&g_disp_timer);
            }
        }
        else
        {
            char show_buff[128];
            background = lv_label_create(lv_scr_act());
            os_snprintf(show_buff, sizeof(show_buff), "can't find %s", buff);
            lv_label_set_text(background, show_buff);
        }
        bk_gui_disp_unlock();
    }
}

void lv_example_img_display_exit(void)
{
    bk_gui_disp_lock();
    if(rtos_is_timer_init(&g_disp_timer))
    {
        rtos_stop_timer(&g_disp_timer);
    }

    if(background)
    {
        lv_obj_remove_style_all(background);
        lv_obj_del(background);
        background = NULL;
    }

    if(g_bg.data)
    {
        psram_free((void *)g_bg.data);
        g_bg.data = NULL;
    }

#if DEMO_USE_NEXT_BG
    if(g_bg_next.data)
    {
        psram_free((void *)g_bg_next.data);
        g_bg_next.data = NULL;
    }
#endif

    g_timer_flag = 0;
    g_img_index = 0;

    bk_jpeg_dec_sw_deinit();
    bk_gui_disp_unlock();
}

#endif

