if (CONFIG_LCD_QSPI_TEST)
set(incs . 
	)

set(srcs
	lcd_qspi_demo.c
	)
endif()


armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES bk_common)