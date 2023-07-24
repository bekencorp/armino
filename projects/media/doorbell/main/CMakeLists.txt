set(incs include)
set(srcs app_main.c)

if (NOT CONFIG_SLAVE_CORE)
list(APPEND srcs
	src/doorbell_core.c
	src/doorbell_boarding.c
	src/doorbell_sdp.c
	src/doorbell_wifi.c
	src/doorbell_transmission.c
	src/doorbell_udp_service.c
	src/doorbell_cs2_service.c
	src/doorbell_devices.c
	src/doorbell_cmd.c
)
endif()

armino_component_register(
	SRCS "${srcs}"
	INCLUDE_DIRS "${incs}"
	REQUIRES bluetooth bk_wifi bk_cli lwip_intf_v2_1 bk_rtos
	PRIV_REQUIRES  bk_init media cs2p2p
)
