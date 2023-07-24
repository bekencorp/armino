#include "os/os.h"
#include "lvgl.h"
#include "page_load_ctrol.h"
#include "ui.h"
#include "./common/ui_helpers.h"

//左右滑动的窗口管理
lv_obj_t *ui_tabview_screen;
lv_obj_t *ui_tabview;
lv_obj_t *ui_tab1;
lv_obj_t *ui_tab2;
lv_obj_t *ui_tab3;
lv_obj_t *ui_tab4;
lv_obj_t *ui_tab5;
lv_obj_t *ui_tab6;

void ui_event_UiPage6_screen(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);

    if(event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_TOP) 
    {
        lv_scr_load_anim(ui_tabview_screen, LV_SCR_LOAD_ANIM_MOVE_TOP, 200, 0, true);
    }
}

void ui_event_tabview_screen(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);

    if(event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_BOTTOM) 
    {
        ui_UiPage6_screen_init();
        lv_scr_load_anim(ui_UiPage6, LV_SCR_LOAD_ANIM_MOVE_BOTTOM, 200, 0, false);
    }
}

static void scroll_begin_event(lv_event_t * e)
{
    lv_obj_t * cont = lv_event_get_target(e);
    lv_event_code_t code = lv_event_get_code(e);

    lv_obj_t * tv = lv_obj_get_parent(cont);

    if(lv_event_get_code(e) == LV_EVENT_SCROLL_END) {
        lv_tabview_t * tabview = (lv_tabview_t *)tv;

        lv_coord_t s = lv_obj_get_scroll_x(cont);

        lv_point_t p;
        lv_obj_get_scroll_end(cont, &p);

        lv_coord_t w = lv_obj_get_content_width(cont);
        lv_coord_t t;

        if(lv_obj_get_style_base_dir(tv, LV_PART_MAIN) == LV_BASE_DIR_RTL)  t = -(p.x - w / 2) / w;
        else t = (p.x + w / 2) / w;

        if(s < 0) t = tabview->tab_cnt - 1;
        else if((t == (tabview->tab_cnt - 1)) && (s > p.x)) t = 0;

        bool new_tab = false;
        if(t != lv_tabview_get_tab_act(tv)) new_tab = true;
        lv_tabview_set_act(tv, t, LV_ANIM_ON);

        LOGI("scroll g_tab_act:%d, new_tab:%d\r\n", lv_tabview_get_tab_act(ui_tabview), new_tab);
    }
}

void hor_page_load_main(void)
{
    ui_tabview_screen = lv_obj_create(NULL);
    lv_obj_add_event_cb(ui_tabview_screen, ui_event_tabview_screen, LV_EVENT_ALL, NULL);

    ui_tabview = lv_tabview_create(ui_tabview_screen, LV_DIR_TOP, 0);
    lv_obj_set_style_bg_color(ui_tabview, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_tabview, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    //lv_obj_add_event_cb(lv_tabview_get_content(ui_tabview), scroll_begin_event, LV_EVENT_ALL, NULL);
    //lv_obj_add_event_cb(ui_tabview, get_tab_act_index_event_handler, LV_EVENT_VALUE_CHANGED, NULL);
    
    lv_obj_t *tab1 = lv_tabview_add_tab(ui_tabview, "Tab 1");
    lv_obj_t *tab2 = lv_tabview_add_tab(ui_tabview, "Tab 2");
    lv_obj_t *tab3 = lv_tabview_add_tab(ui_tabview, "Tab 3");
    lv_obj_t *tab4 = lv_tabview_add_tab(ui_tabview, "Tab 4");
    lv_obj_t *tab5 = lv_tabview_add_tab(ui_tabview, "Tab 5");
    lv_obj_t *tab6 = lv_tabview_add_tab(ui_tabview, "Tab 6");

    lv_obj_set_style_pad_all(tab1, 0, 0);
    lv_obj_set_style_radius(tab1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_UiPage1_screen_init(tab1);
    
    lv_obj_set_style_pad_all(tab2, 0, 0);
    lv_obj_set_style_border_width(tab2, 0, 0);
    ui_UiPage2_screen_init(tab2);
    
    lv_obj_set_style_pad_all(tab3, 0, 0);
    lv_obj_set_style_border_width(tab3, 0, 0);
    ui_UiArcPage_screen_init(tab3);
    
    lv_obj_set_style_pad_all(tab4, 0, 0);
    lv_obj_set_style_border_width(tab4, 0, 0);
    ui_UiPage3_screen_init(tab4);
    
    lv_obj_set_style_pad_all(tab5, 0, 0);
    lv_obj_set_style_border_width(tab5, 0, 0);
    ui_UiPage4_screen_init(tab5);
    
    lv_obj_set_style_pad_all(tab6, 0, 0);
    lv_obj_set_style_border_width(tab6, 0, 0);
    ui_UiPage5_screen_init(tab6);
    
    lv_disp_load_scr(ui_tabview_screen);
}

