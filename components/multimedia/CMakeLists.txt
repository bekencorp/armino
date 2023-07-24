if (CONFIG_SOC_BK7258) # CONFIG_SOC_BK7258
if (CONFIG_MEDIA)
set(incs include)


if (CONFIG_MEDIA_APP)
	set(srcs
		cli/media_cli.c
		app/media_app.c
		comm/comm_act.c
		#comm/frame_buffer.c
		#utils/mlist.c
		#core/media_minor.c
		#transfer/transfer_app_act.c
	)
endif()  #CONFIG_MEDIA_APP

if (CONFIG_AUDIO)
	list(APPEND srcs
		#cli/aud_intf_demo.c
		app/aud_tras.c
	)
endif()

if (CONFIG_MEDIA_MAJOR)
	set(srcs
		#cli/media_cli.c
		#app/media_app.c
		#core/media_major.c
		core/media_ui.c
		#comm/comm_act.c
		comm/frame_buffer.c
		#utils/mlist.c
	)
endif() #CONFIG_MEDIA_MAJOR

if (CONFIG_MEDIA_MINOR) #CONFIG_MEDIA_MINOR
	set(srcs
		)
endif() #CONFIG_MEDIA_MINOR


if (CONFIG_DUAL_CORE)
	list(APPEND srcs
		mb/mailbox_act.c
	)
endif()

if (CONFIG_DVP_CAMERA OR CONFIG_USB_UVC)
	list(APPEND incs camera)
	list(APPEND srcs
		camera/camera_act.c
	)

if (CONFIG_DVP_CAMERA)
		list(APPEND srcs
		camera/dvp.c
	)
endif()

if (CONFIG_USB_UVC)
		list(APPEND srcs
		camera/uvc.c
	)
endif()

if (0)
		list(APPEND srcs
		camera/net_camera.c
	)
endif()

endif()#(CONFIG_DVP_CAMERA OR CONFIG_USB_UVC)

if (CONFIG_WIFI_TRANSFER)

if (CONFIG_MEDIA_APP)
	list(APPEND srcs
		transfer/transfer_app_act.c
	)
endif()

if (CONFIG_MEDIA_MAJOR)
	list(APPEND srcs
		transfer/transfer_major_act.c
	)
endif()

endif()#CONFIG_WIFI_TRANSFER

if (CONFIG_USB_TRANSFER)

if (CONFIG_MEDIA_APP)
	list(APPEND srcs
		transfer/usb_app_act.c
	)
endif()

if (CONFIG_MEDIA_MAJOR)
	list(APPEND srcs
		transfer/usb_major_act.c
	)
endif()

endif()#CONFIG_WIFI_TRANSFER

if (CONFIG_IMAGE_STORAGE)

if (CONFIG_MEDIA_APP)
	list(APPEND srcs
		storage/storage_app_act.c
	)
endif()

if (CONFIG_MEDIA_MAJOR)
	list(APPEND srcs
		storage/storage_major_act.c
	)
endif()

endif()#CONFIG_IMAGE_STORAGE

if (CONFIG_LCD)
	list(APPEND srcs
		lcd/lcd_act.c
		lcd/lcd_calc.c
	)
endif()

if (CONFIG_LVGL)
	list(APPEND srcs
		lcd/lvgl_draw.c
	)
endif()

if (CONFIG_AUDIO)
	list(APPEND srcs
		app/aud_intf.c
		aud/aud_tras_drv.c
	)
endif()

if (CONFIG_DUAL_CORE)
	list(APPEND incs mailbox)
	list(APPEND srcs
		mailbox/media_mailbox_list_util.c
	)

if (CONFIG_MEDIA_APP)
	list(APPEND srcs
		mailbox/media_app_mailbox.c
		#mailbox/media_app_test.c
	)
endif()#CONFIG_MEDIA_APP

if (CONFIG_MEDIA_MAJOR)
	list(APPEND srcs
		mailbox/media_major_mailbox.c
		#mailbox/media_major_test.c
	)
endif()#CONFIG_MEDIA_MAJOR

if (CONFIG_MEDIA_MINOR)
	list(APPEND srcs
		mailbox/media_minor_mailbox.c
		#mailbox/media_minor_test.c
	)
endif()#CONFIG_MEDIA_MINOR

endif()#CONFIG_DUAL_CORE

endif()# CONFIG_MEDIA
endif() # CONFIG_SOC_BK7258
armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES bk_cli)
