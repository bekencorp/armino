if (CONFIG_MEDIA)
set(incs include)

if (NOT CONFIG_SLAVE_CORE)
	set(srcs
		core/media_major.c
		cli/media_cli.c
		comm/comm_act.c
		comm/frame_buffer.c
		app/media_app.c
		utils/mlist.c
	)

if (CONFIG_CAMERA)
	list(APPEND srcs
		camera/camera_act.c
		camera/dvp.c
	)
endif()

if (CONFIG_WIFI_TRANSFER)
	list(APPEND srcs
		transfer/transfer_act.c
		storage/storage_act.c
	)
endif()

if (CONFIG_LCD)
	list(APPEND srcs
		lcd/lcd_act.c
	)
endif()

if (CONFIG_AUDIO)
	list(APPEND srcs
		aud/aud_api.c
		aud/aud_act.c
		aud/adc_ccb.c
		aud/dac_ccb.c
	)
endif()

if (CONFIG_USB_UVC)
	list(APPEND srcs
		camera/uvc_act.c
		camera/uvc.c
	)
endif()

if (CONFIG_DUAL_CORE)
	list(APPEND srcs
		mb/mailbox_act.c
	)
endif()

else()
	set(srcs
		core/media_minor.c
		cli/media_cli.c
		comm/comm_act.c
		comm/frame_buffer.c
		utils/mlist.c
	)


if (CONFIG_CAMERA)
	list(APPEND srcs
		camera/camera_act.c
		camera/dvp.c
	)
endif()

if (CONFIG_WIFI_TRANSFER)
	list(APPEND srcs
		transfer/transfer_act.c
		storage/storage_act.c
	)
endif()

if (CONFIG_AUDIO)
	list(APPEND srcs
		aud/aud_api.c
		aud/aud_act.c
		aud/adc_ccb.c
		aud/dac_ccb.c
	)
endif()

if (CONFIG_LCD)
	list(APPEND srcs
		lcd/lcd_act.c
		lcd/lcd_calc.c
	)
endif()

if (CONFIG_USB_UVC)
	list(APPEND srcs
		camera/uvc_act.c
		camera/uvc.c
	)
endif()

if (CONFIG_DUAL_CORE)
	list(APPEND srcs
		mb/mailbox_act.c
	)
endif()

endif()

endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES bk_cli)