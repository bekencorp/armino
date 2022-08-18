set(incs)
set(srcs)

if (CONFIG_AT_CMD)
    list(APPEND incs
		include
		)
	list(APPEND srcs
		"src/at_common.c"
		"src/ble_at_command.c"
		"src/video_at_command.c"
		"src/wifi_at_command.c"
		)
	if (CONFIG_BT)
		list(APPEND srcs
			"src/bt_at_command.c")
	endif()
endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" REQUIRES bk_ble bk_bt bk_common bk_log video PRIV_REQUIRES bk_ble ali_mqtt)