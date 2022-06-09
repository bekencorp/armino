if (CONFIG_AUDIO_G711_TEST)
set(srcs
	g711_demo.c
	)
endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES bk_common)