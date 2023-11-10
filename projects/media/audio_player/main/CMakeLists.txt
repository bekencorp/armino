set(incs .
		include)

set(srcs
		app_main.c
		)

if (NOT CONFIG_SLAVE_CORE)
list(APPEND srcs
	audio_player.c
	audio_codec.c
	netstream_player.c
)
endif()


armino_component_register(
	SRCS "${srcs}"
	INCLUDE_DIRS "${incs}"
	PRIV_REQUIRES media bk_init bk_cli)
