set(incs include)
set(srcs c2.c)

armino_component_register(SRCS "${srcs}"
	INCLUDE_DIRS "${incs}"
	PRIV_REQUIRES c1 # Use PRIV_REQUIRES here because c1_public_api()/c1_private_api() only used in c2.c
	)
