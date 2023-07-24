set(incs)
set(srcs)

if (CONFIG_H264 AND CONFIG_SOC_BK7236)

	list(APPEND incs
		include
	)

	list(APPEND srcs
		"src/h264_core.c"
		"src/h264_udp.c"
		"src/h264_tcp.c"
	)

	if (CONFIG_USB_DEVICE AND CONFIG_USB_UVC)
		list(APPEND srcs
			"src/h264_usb.c"
		)
	endif()
endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES media aud_intf bk_common)
