if (CONFIG_QSPI_OLED_TEST)
set(incs . 
	)
set(srcs
	qspi_oled_demo.c
	)
endif()


armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES bk_common)