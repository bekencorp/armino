#if (CONFIG_SECURITY)
	set(srcs security_func.c)
#endif()
armino_component_register(SRCS "${srcs}" PRIV_REQUIRES bk_common)
