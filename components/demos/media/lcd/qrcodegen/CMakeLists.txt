if (CONFIG_QRCODEGEN_TEST)
set(incs . )

set(srcs
	qrcodegen_demo.c
	)
endif()


armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES bk_common)