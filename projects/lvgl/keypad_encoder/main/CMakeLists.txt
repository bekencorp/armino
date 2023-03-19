set(incs .)

set(srcs
	keypad_encoder_main.c
)

if (CONFIG_LV_USE_DEMO_KEYPAD_AND_ENCODER)
list(APPEND srcs
	lv_demo_keypad_encoder.c
)
endif()


armino_component_register(
	SRCS "${srcs}"
	INCLUDE_DIRS "${incs}" 
	PRIV_REQUIRES media bk_init bk_cli
)
