set(incs
	.
	)

if (CONFIG_HTTP)
set(srcs
	utils_httpc.c
	utils_net.c
	utils_timer.c
	lite-log.c
	)
endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES bk_common)
