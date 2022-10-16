if (CONFIG_AUDIO_FLAC_TEST)
set(incs .
	)

set(srcs
	flac_dec_demo.c
	flac_enc_demo.c
	)
endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES bk_common)
