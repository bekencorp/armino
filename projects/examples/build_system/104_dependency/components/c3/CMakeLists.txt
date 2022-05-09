set(incs include)
set(srcs c3.c)

armino_component_register(SRCS "${srcs}"
	INCLUDE_DIRS "${incs}"
	REQUIRES c1 #Use REQURIES because bk_api_c3.h depends on bk_api_c1.h
	PRIV_REQUIRES c2 #Use PRIV_REQUIRES c1 because only c3.c depends on bk_api_c1.h
	)
