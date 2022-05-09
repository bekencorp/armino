set(incs .)

if (CONFIG_SARADC_CALI)
	set(srcs saradc_intf.c)
endif()

armino_component_register(SRCS "${srcs}"
	INCLUDE_DIRS "${incs}"
	REQUIRES temp_detect
	PRIV_REQUIRES bk_common bk_init
	)
