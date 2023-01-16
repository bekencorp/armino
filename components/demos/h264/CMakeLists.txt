set(incs)
set(srcs)

if (CONFIG_H264)
	list(APPEND incs 
		include
	)

	list(APPEND srcs
		"src/h264_core.c"
		"src/h264_udp.c"
		"src/h264_tcp.c"
	)

endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES media aud_intf bk_common)
