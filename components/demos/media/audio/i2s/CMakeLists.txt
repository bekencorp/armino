if (CONFIG_I2S_TEST)
set(srcs
	i2s_demo.c
	)
endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES bk_common)