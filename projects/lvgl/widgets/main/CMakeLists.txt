set(incs .)

set(srcs
	widgets_main.c
)

if (CONFIG_LV_USE_DEMO_WIDGETS)
list(APPEND srcs
	lv_demo_widgets.c
	assets/img_clothes.c
	assets/img_demo_widgets_avatar.c
	assets/img_lvgl_logo.c
)
endif()


armino_component_register(
	SRCS "${srcs}"
	INCLUDE_DIRS "${incs}" 
	PRIV_REQUIRES media bk_init bk_cli
)
