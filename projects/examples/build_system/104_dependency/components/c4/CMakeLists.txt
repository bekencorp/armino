set(incs include)
set(srcs c4.c)

armino_component_register(SRCS "${srcs}"
	INCLUDE_DIRS "${incs}"
	PRIV_REQUIRES c3
	PRIV_REQUIRES c2
	)
