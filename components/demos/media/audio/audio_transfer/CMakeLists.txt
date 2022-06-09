if (CONFIG_AUDIO_TEST AND CONFIG_AUDIO_RISCV_IP_V1_0 AND CONFIG_DUAL_CORE)

set(incs . include)

set(srcs
	include/audio_mailbox.c
	)

if (NOT CONFIG_SOC_BK7256_CP1)
	list(APPEND srcs
				include/audio_transfer_cp0.c
				audio_transfer_cp0_demo.c
		)
endif()

if (CONFIG_SOC_BK7256_CP1)
	list(APPEND srcs
				include/audio_transfer_cp1.c
				audio_transfer_cp1_demo.c
		)
endif()

endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES bk_common)