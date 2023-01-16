if (CONFIG_AUDIO_ADPCM_TEST)
set(srcs
	adpcm_demo.c
	)
endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES bk_common)