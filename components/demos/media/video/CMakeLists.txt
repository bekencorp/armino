set(incs)
set(srcs)

if (CONFIG_CAMERA)
    list(APPEND incs
		dvp
		)
	list(APPEND srcs
		"dvp/dvp_camera_driver.c"
		)
endif()

if (CONFIG_SPIDMA)
    list(APPEND incs
		spidma
		)
	list(APPEND srcs
		"spidma/spidma_driver.c"
		)
endif()

if (CONFIG_JPEG_DECODE)
    list(APPEND incs
		jpeg_dec
		)
	list(APPEND srcs
		"jpeg_dec/tjpgd.c"
		"jpeg_dec/jdev.c"
		"uvc_lcd_display/uvc_lcd_display.c"
		)
endif()

if (CONFIG_USB_UVC)
    list(APPEND incs
		uvc
		)
	list(APPEND srcs
		"uvc/uvc_camera_driver.c"
		)
endif()

if (CONFIG_APP_DEMO_VIDEO_TRANSFER)
    list(APPEND incs
		video_transfer
		)
	list(APPEND srcs
		"video_transfer/video_transfer.c"
		)
endif()


armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" REQUIRES bk_common bk_log)