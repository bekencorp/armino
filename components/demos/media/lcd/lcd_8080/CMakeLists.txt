
if (CONFIG_LCD_TEST)
set(incs . ../common/.)
set(srcs
	lcd_8080_demo.c
	st7796s.c
	"../common/lcd_dma2d_config.c"
	"../common/lcd_sdcard_config.c"
	)
endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES bk_common)
