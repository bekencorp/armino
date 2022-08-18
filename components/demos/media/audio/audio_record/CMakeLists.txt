if (CONFIG_AUDIO_TEST AND CONFIG_AUDIO_RISCV_IP_V1_0 AND CONFIG_DUAL_CORE)

if (NOT CONFIG_SLAVE_CORE)
set(incs .)

set(srcs
	audio_record_demo.c
	)

endif()

endif()
armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES bk_common media_common)