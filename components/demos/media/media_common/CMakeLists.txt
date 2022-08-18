if (CONFIG_DUAL_CORE AND CONFIG_MEDIA)
set(incs .)

set(srcs
	media_common.c
	)
endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES video audio_transfer)
