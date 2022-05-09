set(incs include/bk_private)
set(srcs
	BkDriverFlash.c
	)

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES bk_common)
