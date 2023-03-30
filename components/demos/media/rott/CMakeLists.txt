
if (CONFIG_HW_ROTT_TEST)
set(incs)
set(srcs
	"rott_demo.c"
	)
endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES bk_common)
