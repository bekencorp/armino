
set(incs ".")
set(srcs app_main.c)

if (CONFIG_SPE_START)
	list(APPEND srcs spe_startup.c)
else()
	list(APPEND srcs startup.c)
endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS ${incs})
