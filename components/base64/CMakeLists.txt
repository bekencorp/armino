set(incs
	.
	)

set(srcs
	base_64.c
	)

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES bk_common)
