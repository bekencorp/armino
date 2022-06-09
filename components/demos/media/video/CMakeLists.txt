set(incs
	dvp
	spidma
	jpeg_dec
	uvc
	video_transfer
	image_save
	)
set(srcs)

if (CONFIG_DUAL_CORE)
	list(APPEND srcs
		"uvc_capture_display/uvc_mailbox.c"
		)
if (NOT CONFIG_SOC_BK7256_CP1)
	list(APPEND srcs
			"uvc_capture_display/uvc_process_cpu0.c"
			)
endif()
endif()

if (CONFIG_CAMERA)
	list(APPEND srcs
		"dvp/dvp_camera_driver.c"
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
		"uvc_lcd_display/uvc_lcd_display.c"
		)
endif()

if (CONFIG_USB_UVC)
	list(APPEND srcs
		"uvc/uvc_camera_driver.c"
		"uvc_capture_display/uvc_process_cpu1.c"
		)
endif()

if (CONFIG_CAMERA OR CONFIG_SPIDMA)
if (CONFIG_APP_DEMO_VIDEO_TRANSFER)
	list(APPEND srcs
		"video_transfer/video_transfer.c"
		"dvp_capture/dvp_image_save.c"
		)
endif()
endif()


armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" REQUIRES bk_common bk_log)