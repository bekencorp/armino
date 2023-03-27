set(incs)
set(srcs)

if (CONFIG_AV_DEMO)
	list(APPEND incs
		include
	)

	list(APPEND srcs
		"src/av_audio_server.c"
		"src/av_audio_client.c"
		"src/av_audio_common.c"
		"src/av_audio_demo.c"
	)
endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES bk_common)
