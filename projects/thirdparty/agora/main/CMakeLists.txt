if (CONFIG_SLAVE_CORE)
	armino_component_register(SRCS "app_main.c" INCLUDE_DIRS "")
else()
set(incs . include)
set(srcs)

list(APPEND srcs
	app_main.c
	agora_rtc_demo.c
	agora_sdk/agora_rtc.c
	audio/audio_transfer.c
)

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}")
endif()