set(incs)
set(srcs)

if (CONFIG_CM_BACKTRACE)
	list(APPEND incs
		cm_backtrace
		cm_backtrace/Languages/en-US
	)

	list(APPEND srcs
		cm_backtrace/cm_backtrace.c
	)
endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES os_source)

