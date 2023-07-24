set(incs .)

set(srcs
	lcd_qspi_meter_main.c
)

if (CONFIG_LV_QSPI_USE_DEMO_METER)
list(APPEND srcs
	lv_example_meter.c
)
endif()


armino_component_register(
	SRCS "${srcs}"
	INCLUDE_DIRS "${incs}" 
	PRIV_REQUIRES media bk_init bk_cli
)
