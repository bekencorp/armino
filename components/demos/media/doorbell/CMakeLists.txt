set(incs)
set(srcs)

if (CONFIG_DOORBELL)
	list(APPEND incs 
		include
	)

	list(APPEND srcs
		"src/doorbell_core.c"
		"src/doorbell_udp.c"
		"src/doorbell_tcp.c"
	)
endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES media aud_intf bk_common)
