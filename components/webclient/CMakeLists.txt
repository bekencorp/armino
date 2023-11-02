set(incs .)
set(srcs)

if (CONFIG_WEBCLIENT)
list(APPEND srcs
	webclient.c
	)
endif()

armino_component_register(SRCS "${srcs}"
	INCLUDE_DIRS "${incs}"
	PRIV_REQUIRES lwip_intf_v2_1
	)