
if (CONFIG_DOORBELL AND CONFIG_DUAL_CORE)
set(incs include)
set(srcs
	"src/doorbell.c"
	)
endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES audio_transfer)
