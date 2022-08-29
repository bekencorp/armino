
if (CONFIG_LCD_TEST)
set(incs . ../common/.)
set(srcs
	"../common/lcd_spi_config.c"
	"../lcd_dev_init/lcd_device_init.c"
	)
endif()


armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES bk_common)