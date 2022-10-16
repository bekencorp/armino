set(incs .)
set(srcs)

list(APPEND srcs ota.c)

armino_component_register(SRCS "${srcs}"
	INCLUDE_DIRS "${incs}"
	PRIV_REQUIRES bk_common bk_netif bk_ble bk_ps fatfs bk_init at bk_wifi compal media_common media bk_bt http
	)