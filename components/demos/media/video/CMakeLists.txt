set(incs
	dvp
	spidma
	jpeg_dec
	uvc
	video_common
	video_transfer
	video_transfer_frame_buff
	dvp_capture
	"../lcd/common"
	"../lcd/lcd_8080"
	)
set(srcs)

if (CONFIG_DUAL_CORE AND CONFIG_PSRAM)
	list(APPEND srcs
		"uvc_capture_display/uvc_mailbox.c"
		)
if (NOT CONFIG_SLAVE_CORE)
	list(APPEND srcs
			"uvc_capture_display/uvc_process_cpu0.c"
			)
endif()
endif()

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
		"uvc/uvc_camera_driver.c"
		"uvc_capture_display/uvc_process_cpu1.c"
		)
endif()

if (CONFIG_CAMERA OR CONFIG_SPIDMA)
if (CONFIG_APP_DEMO_VIDEO_TRANSFER)
	list(APPEND srcs
		"video_transfer/video_transfer.c"
		"dvp_capture/dvp_capture.c"
		)
endif()
endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES bk_common bk_log)