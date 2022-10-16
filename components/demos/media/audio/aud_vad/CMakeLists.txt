if (CONFIG_AUDIO_VAD_TEST)
set(srcs
	aud_vad_demo.c
	)
endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES bk_common)