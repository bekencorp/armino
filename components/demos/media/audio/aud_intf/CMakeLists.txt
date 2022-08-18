if (CONFIG_AUD_INTF AND CONFIG_AUD_INTF_VER_NEW)
if (CONFIG_AUDIO_TRANSFER)
if (NOT (CONFIG_AUD_TRAS_MODE_CPU1 AND (NOT CONFIG_DUAL_CORE)))

set(incs include)

set(srcs
	mb/aud_mb.c
	aud/aud_tras_drv.c
	)

if (NOT CONFIG_SLAVE_CORE)
	list(APPEND srcs
		core/aud_intf.c
	)

if (CONFIG_AUD_INTF_TEST)
	list(APPEND srcs
		demo/aud_intf_demo.c
	)
endif()

endif()

endif()
endif()
endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES bk_common media_common)
