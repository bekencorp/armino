if (CONFIG_SBC_TEST)
set(srcs
	sbc_decode_demo.c
	)
endif()

armino_component_register(SRCS "${srcs}" 
	INCLUDE_DIRS "${incs}"
	PRIV_REQUIRES bk_common)