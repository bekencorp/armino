set(incs)
set(srcs)

if (CONFIG_CHERRY_USB)
	list(APPEND incs
		CherryUSB/common
		CherryUSB/core
		CherryUSB/osal
		CherryUSB/class/cdc
		CherryUSB/class/hid
		CherryUSB/class/msc
		CherryUSB/class/audio
		CherryUSB/class/video
		CherryUSB/class/hub
	)

	if (CONFIG_USB_DEVICE)
		list(APPEND srcs
			CherryUSB/core/usbd_core.c
			CherryUSB/port/beken_musb/usb_dc_beken_musb.c
		)

		list(APPEND incs
			CherryUSB/demo/beken/usb_device
		)
	endif()

	if (CONFIG_USB_UVC)
		list(APPEND srcs
			CherryUSB/class/video/usbd_video.c
			# CherryUSB/demo/beken/usb_device/video/video_static_mjpeg.c
			CherryUSB/demo/beken/usb_device/video/video_stream_h264.c
		)

		list(APPEND incs
			CherryUSB/demo
		)
	endif()
endif() # CONFIG_CHERRY_USB

armino_component_register(SRCS "${srcs}"
			INCLUDE_DIRS "${incs}")
