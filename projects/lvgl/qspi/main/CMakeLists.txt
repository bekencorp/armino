set(incs .)

set(srcs
	qspi_main.c
)

if (CONFIG_LV_QSPI_DEMO)
list(APPEND srcs
	lv_example_meter.c
)
endif()

if (CONFIG_OVERRIDE_FLASH_PARTITION)
    list(APPEND srcs
        vnd_flash.c
    )
endif()

armino_component_register(
	SRCS "${srcs}"
	INCLUDE_DIRS "${incs}" 
	PRIV_REQUIRES media bk_init bk_cli
)
