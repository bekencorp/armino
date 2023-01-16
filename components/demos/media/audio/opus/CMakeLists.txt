if (CONFIG_AUDIO_OPUS_TEST)
set(srcs
	opus_demo.c
	)
endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES bk_common)