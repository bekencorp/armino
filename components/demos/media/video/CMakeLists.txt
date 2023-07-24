set(incs
	camera
	spidma
	video_transfer
	capture
	)
set(srcs)

if (CONFIG_CAMERA)
	list(APPEND srcs
		"camera/dvp_camera_driver.c"
		)
endif()

if (CONFIG_USB_UVC)
	list(APPEND srcs
		"camera/uvc_camera_driver.c"
		"uvc_display/uvc_display.c"
		)
endif()

if (CONFIG_SPIDMA)
	list(APPEND srcs
		"spidma/spidma_driver.c"
		)
endif()

if (CONFIG_CAMERA OR CONFIG_USB_UVC OR CONFIG_SPIDMA)
	list(APPEND srcs
		"video_transfer/video_transfer.c"
		"video_transfer/video_buffer.c"
		"dvp_display/dvp_display.c"
		)

if (CONFIG_CAMERA_TEST)
		list(APPEND srcs
		"capture/capture.c"
		)
endif()
endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES bk_common bk_log media)
