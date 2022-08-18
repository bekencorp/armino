if (CONFIG_AUD_INTF AND CONFIG_AUD_INTF_VER_OLD)
if (CONFIG_AUDIO_TRANSFER)
if (NOT (CONFIG_AUD_TRAS_MODE_CPU1 AND (NOT CONFIG_DUAL_CORE)))

set(incs include)

set(srcs
	mb/audio_mailbox.c
	aud/audio_transfer_api.c
	aud/audio_transfer_driver.c
	)

if (NOT CONFIG_SLAVE_CORE)
	list(APPEND srcs
		core/audio_transfer.c
	)
endif()

endif()
endif()
endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES bk_common media_common)
