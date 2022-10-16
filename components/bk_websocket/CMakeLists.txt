set(incs include include)
set(srcs)

if(CONFIG_WEBSOCKET)
	list(APPEND srcs
		bk_websocket_client.c
		http_parser.c
	)
endif()
armino_component_register(SRCS "${srcs}"
	INCLUDE_DIRS "${incs}"
	PRIV_REQUIRES mbedtls bk_wifi bk_common
	)
