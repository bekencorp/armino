set(incs .
    )
set(srcs
    armino_main.c
    )

if (CONFIG_OVERRIDE_FLASH_PARTITION)
    list(APPEND srcs
        vendor_flash.c
    )
endif()



if (CONFIG_BLE)

list(APPEND srcs
    dm_ble_demo.c
)

endif()

armino_component_register(
	SRCS "${srcs}"
	INCLUDE_DIRS "${incs}"
	PRIV_REQUIRES media bk_init bk_cli
)
