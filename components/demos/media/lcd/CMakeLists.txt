
if (CONFIG_LCD_TEST)
set(incs include)
set(srcs
	"src/lcd_demo.c"
	"src/st7796s.c"
	)
endif()


armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}")
