set(incs include)
set(priv_incs c1_internal)
set(srcs c1.c c1_internal/c1_internal.c)

armino_component_register(SRCS "${srcs}"
	INCLUDE_DIRS "${incs}"
	PRIV_INCLUDE_DIRS "${priv_incs}"
	)
