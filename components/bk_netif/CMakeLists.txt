set(incs)
set(srcs)

if(CONFIG_BK_NETIF)
	list(APPEND srcs
		bk_netif.c
	)
	list(APPEND incs
		include
	)
endif()

armino_component_register(SRCS "${srcs}"
	INCLUDE_DIRS "${incs}"
	PRIV_REQUIRES bk_common)
