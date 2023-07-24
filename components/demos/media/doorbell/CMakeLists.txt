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

if (CONFIG_CS2_P2P_SERVER OR CONFIG_CS2_P2P_CLIENT)
    list(APPEND srcs
		src/doorbell_cs2_p2p_server.c
		src/doorbell_cs2_p2p_client.c
    )
endif()


if (CONFIG_AV_DEMO)
		list(APPEND srcs
		"src/doorbell_udp_client.c"
		"src/doorbell_udp_server.c"
		"src/doorbell_tcp_client.c"
		"src/doorbell_tcp_server.c"
	)
endif()

endif()

armino_component_register(
    SRCS "${srcs}"
    INCLUDE_DIRS "${incs}"
    REQUIRES cs2p2p_middleware
    PRIV_REQUIRES media multimedia bk_common aud_intf cs2p2p_middleware)