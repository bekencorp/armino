if (CONFIG_TOUCH_TEST)
set(srcs
	touch_demo.c
	)
endif()

armino_component_register(SRCS "${srcs}" PRIV_REQUIRES bk_common)
