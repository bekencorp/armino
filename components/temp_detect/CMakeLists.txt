set(incs .)

# if (CONFIG_TEMP_DETECT)
	set(srcs temp_detect.c)
# endif()

armino_component_register(SRCS "${srcs}"
	INCLUDE_DIRS "${incs}"
	PRIV_REQUIRES bk_common
	)
