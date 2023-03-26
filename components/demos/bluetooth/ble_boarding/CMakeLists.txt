if (CONFIG_AT_CMD)
set(srcs
	ble_boarding_demo.c
	ble_boarding_demo_dm_ble.c
	)
endif()

armino_component_register(SRCS "${srcs}"
	INCLUDE_DIRS "${incs}"
	PRIV_REQUIRES bk_common at
	)