set(incs)
set(srcs)

if (CONFIG_CMSIS)
	list(APPEND srcs
		)

	list(APPEND incs
		)
endif()

armino_component_register(SRCS "${srcs}"
			INCLUDE_DIRS "${incs}"
			PRIV_REQUIRES bk_common)
