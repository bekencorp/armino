set(incs
	dvp
	spidma
	jpeg_dec
	uvc
	video_transfer
	dvp_capture
	)
set(srcs)

if (CONFIG_CAMERA)
	list(APPEND srcs
		"dvp/dvp_camera_driver.c"
		"dvp/dvp_camera_config.c"
		)
endif()

if (CONFIG_SPIDMA)
	list(APPEND srcs
		"spidma/spidma_driver.c"
		)
endif()

if (CONFIG_JPEG_DECODE)
	list(APPEND srcs
		"jpeg_dec/tjpgd.c"
		"jpeg_dec/jdev.c"
		)
endif()

if (CONFIG_USB_UVC)
	list(APPEND srcs
		"uvc_display/uvc_display.c"
		)
endif()

if (CONFIG_CAMERA OR CONFIG_SPIDMA)
	list(APPEND srcs
		"video_transfer/video_transfer.c"
		"video_transfer/video_buffer.c"
		"dvp_display/dvp_display.c"
		)

if (CONFIG_DVP_TEST)
		list(APPEND srcs
		"dvp_capture/dvp_capture.c"
		)
endif()
endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES bk_common bk_log media)