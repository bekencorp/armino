#temp code
set(incs ../bk_common/include)
set(srcs log.c)
armino_component_register(SRCS "${srcs}"
	INCLUDE_DIRS "${incs}"
	PRIV_REQUIRES bk_common)
