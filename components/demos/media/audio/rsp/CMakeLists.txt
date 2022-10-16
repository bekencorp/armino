if (CONFIG_AUDIO_RSP_TEST)
set(srcs
	aud_rsp_demo.c
	)
endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES bk_common)