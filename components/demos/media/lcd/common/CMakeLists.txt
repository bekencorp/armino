
if (CONFIG_LCD_DEMO)
set(incs . ../common/.)
set(srcs
	"../common/lcd_dma2d_config.c"
	"../common/lcd_spi_config.c"
	#"../common/lcd_sdcard_config.c"
	)
endif()


armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES bk_common)