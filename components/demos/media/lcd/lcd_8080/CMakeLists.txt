
if (CONFIG_LCD_TEST)
set(incs . ../common/.)
set(srcs
	lcd_8080_demo.c
	st7796s.c
	)
endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES bk_common)
