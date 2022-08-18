set(incs include/bk_private)

if (CONFIG_FLASH_ORIGIN_API)
	list(APPEND srcs
		BkDriverFlash.c
	)
endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES bk_common)
