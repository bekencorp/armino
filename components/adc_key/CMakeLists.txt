set(incs .)
set(srcs)

if (CONFIG_ADC_KEY)
	list(APPEND srcs
		adc_key_main.c
		)
		
endif()
armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES bk_cli bk_common bk_log)
