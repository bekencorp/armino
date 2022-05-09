if (CONFIG_DEMOS_IPERF)
set(srcs
	iperf.c
	)
endif()

armino_component_register(SRCS "${srcs}" PRIV_REQUIRES bk_common)
