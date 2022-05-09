if (CONFIG_AUDIO_TEST AND CONFIG_AUDIO_RISCV_IP_V1_0)
set(srcs
	aud_cp1.c
	)
endif()

if (CONFIG_AUDIO_TEST)
if (CONFIG_AUDIO_RISCV_IP_V1_0 AND CONFIG_DUAL_CORE)
if (NOT CONFIG_SOC_BK7256_CP1)
	list(APPEND srcs aud_cp0.c)
endif()
endif()
endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES bk_common)