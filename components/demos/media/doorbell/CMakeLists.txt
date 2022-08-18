set(incs)
set(srcs)

if (CONFIG_DOORBELL)
	list(APPEND incs 
		include
	)

	list(APPEND srcs
		"src/doorbell.c"
	)
endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES media audio_transfer aud_intf bk_common)
