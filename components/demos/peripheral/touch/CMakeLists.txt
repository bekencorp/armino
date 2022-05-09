if (CONFIG_TOUCH)
set(srcs
	touch_demo.c
	)
endif()

armino_component_register(SRCS "${srcs}" PRIV_REQUIRES bk_common)
