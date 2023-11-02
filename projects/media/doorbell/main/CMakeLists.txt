set(incs . include)
set(srcs app_main.c)

if (NOT CONFIG_SLAVE_CORE)
list(APPEND srcs
	src/doorbell_core.c
	src/doorbell_boarding.c
	src/doorbell_sdp.c
	src/doorbell_network.c
	src/doorbell_transmission.c
	src/doorbell_udp_service.c
	src/doorbell_tcp_service.c
	src/doorbell_devices.c
	src/doorbell_cmd.c
)
endif()

if (CONFIG_INTEGRATION_DOORBELL_CS2)
list(APPEND srcs
	src/doorbell_cs2_service.c
)
endif()

set(priv_req bk_init)


if ("${ARMINO_SOC}" STREQUAL "bk7256") 
list(APPEND priv_req media cs2p2p aud_intf)
endif()


if (CONFIG_OVERRIDE_FLASH_PARTITION)
	list(APPEND srcs
		vendor_flash.c
	)
endif()

armino_component_register(
	SRCS "${srcs}"
	INCLUDE_DIRS "${incs}"
	REQUIRES bluetooth bk_wifi bk_cli lwip_intf_v2_1 bk_rtos driver user_driver bk_system
	PRIV_REQUIRES "${priv_req}"
)
