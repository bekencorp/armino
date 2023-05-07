set(incs .
    )
set(srcs
    armino_main.c
    )

if (CONFIG_OVERRIDE_FLASH_PARTITION)
    list(APPEND srcs
        vendor_flash.c
    )
endif()
if (CONFIG_LV_USE_DEMO_MUSIC)
list(APPEND srcs
	lv_demo_music.c
	lv_demo_music_list.c
	lv_demo_music_main.c
	assets/img_lv_demo_music_btn_corner_large.c
	assets/img_lv_demo_music_btn_list_pause.c
	assets/img_lv_demo_music_btn_list_pause_large.c
	assets/img_lv_demo_music_btn_list_play.c
	assets/img_lv_demo_music_btn_list_play_large.c
	assets/img_lv_demo_music_btn_loop.c
	assets/img_lv_demo_music_btn_loop_large.c
	assets/img_lv_demo_music_btn_next.c
	assets/img_lv_demo_music_btn_next_large.c
	assets/img_lv_demo_music_btn_play.c
	assets/img_lv_demo_music_btn_pause.c
	assets/img_lv_demo_music_btn_pause_large.c
	assets/img_lv_demo_music_btn_play_large.c
	assets/img_lv_demo_music_btn_prev.c
	assets/img_lv_demo_music_btn_prev_large.c
	assets/img_lv_demo_music_btn_rnd.c
	assets/img_lv_demo_music_btn_rnd_large.c
	assets/img_lv_demo_music_corner_left.c
	assets/img_lv_demo_music_corner_left_large.c
	assets/img_lv_demo_music_corner_right.c
	assets/img_lv_demo_music_corner_right_large.c
	assets/img_lv_demo_music_cover_1.c
	assets/img_lv_demo_music_cover_1_large.c
	assets/img_lv_demo_music_cover_2.c
	assets/img_lv_demo_music_cover_2_large.c
	assets/img_lv_demo_music_cover_3.c
	assets/img_lv_demo_music_cover_3_large.c
	assets/img_lv_demo_music_icon_1.c
	assets/img_lv_demo_music_icon_1_large.c
	assets/img_lv_demo_music_icon_2.c
	assets/img_lv_demo_music_icon_2_large.c
	assets/img_lv_demo_music_icon_3.c
	assets/img_lv_demo_music_icon_3_large.c
	assets/img_lv_demo_music_icon_4.c
	assets/img_lv_demo_music_icon_4_large.c
	assets/img_lv_demo_music_list_border.c
	assets/img_lv_demo_music_list_border_large.c
	assets/img_lv_demo_music_logo.c
	assets/img_lv_demo_music_slider_knob.c
	assets/img_lv_demo_music_slider_knob_large.c
	assets/img_lv_demo_music_wave_bottom.c
	assets/img_lv_demo_music_wave_bottom_large.c
	assets/img_lv_demo_music_wave_top.c
	assets/img_lv_demo_music_wave_top_large.c
)
endif()

if (CONFIG_BLE)
list(APPEND srcs
    dm_ble_demo.c
)
endif()
armino_component_register(
	SRCS "${srcs}"
	INCLUDE_DIRS "${incs}" 
	PRIV_REQUIRES media bk_init bk_cli
)
