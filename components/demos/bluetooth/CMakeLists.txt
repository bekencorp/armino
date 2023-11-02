set(incs ble_boarding)
set(srcs)

if (CONFIG_AT_CMD)
	list(APPEND srcs
		ble_boarding/ble_boarding_demo.c
		ble_boarding/ble_boarding_demo_dm_ble.c
		ble_boarding/ble_boarding_doorbell.c
		)
endif()

if (CONFIG_A2DP_SINK_DEMO)
	list(APPEND srcs
		a2dp_sink/a2dp_sink_demo.c
		a2dp_sink/ring_buffer_node.c
		)
endif()

if (CONFIG_A2DP_SOURCE_DEMO AND CONFIG_FATFS)
    list(APPEND srcs
        a2dp_source/a2dp_source_demo_avrcp.c
        a2dp_source/a2dp_source_demo.c
		a2dp_source/a2dp_source_demo_cli.c
        a2dp_source/ring_buffer_particle.c
        )
endif()

if (CONFIG_A2DP_SOURCE_DEMO_AVRCP)
    list(APPEND srcs
        a2dp_source/a2dp_source_demo_avrcp.c
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