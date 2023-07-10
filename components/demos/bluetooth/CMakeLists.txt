set(incs)
set(srcs)

if (CONFIG_AT_CMD)
	list(APPEND srcs
		ble_boarding/ble_boarding_demo.c
		ble_boarding/ble_boarding_demo_dm_ble.c
		)
endif()

if (CONFIG_A2DP_SINK_DEMO)
	list(APPEND srcs
		a2dp_sink/a2dp_sink_demo.c
		)
endif()

if (CONFIG_HFP_HF_DEMO)
	list(APPEND srcs
		hfp_hf/hfp_hf_demo.c
		)
endif()

armino_component_register(SRCS "${srcs}"
	INCLUDE_DIRS "${incs}"
	PRIV_REQUIRES bk_common at aac_decoder
	)