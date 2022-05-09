if (CONFIG_AUDIO_AEC_TEST)
set(srcs
	aec_demo.c
	)
endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES bk_common)