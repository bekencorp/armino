if (CONFIG_UVC_OTA_DEMO)
	set(incs
		uvc_ota
		)
	set(srcs)

	list(APPEND srcs
		"uvc_ota/uvc_ota_demo.c"
		)
endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES bk_common bk_log usb)