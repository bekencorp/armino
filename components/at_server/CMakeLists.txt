set(incs "")
set(srcs "")

if (CONFIG_AT)
	list(APPEND incs
		.
		_at_server
		_at_server_port
		at_server_func
		atsvr_cmd
		)

	list(APPEND srcs
		"_at_server_port/atsvr_core.c"
		"_at_server_port/atsvr_port.c"
		"atsvr_cmd/atsvr_cmd.c"
		"atsvr_cmd/atsvr_wlan.c"
		"atsvr_cmd/atsvr_misc.c"
		"_at_server/_at_server.c"
		"_at_server/_at_server_port.c"
		"at_server_func/_atsvr_func.c"
		"at_server.c"
		)
endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES bk_common)
