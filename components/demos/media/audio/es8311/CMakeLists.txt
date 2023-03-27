if (CONFIG_ES8311_TEST)
set(srcs
	es8311_demo.c
	)
endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES bk_common)