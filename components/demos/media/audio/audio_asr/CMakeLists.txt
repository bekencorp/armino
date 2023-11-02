if (CONFIG_ASR_TEST)
set(srcs
	asr_demo.c
	)
endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES bk_common wanson media)
