set(incs
	.
	inc
	port
	)

set(srcs)

if (CONFIG_EASY_FLASH)
	list(APPEND srcs
		bk_ef.c
		src/easyflash.c
		src/ef_env.c
		src/ef_env_wl.c
		src/ef_iap.c
		src/ef_log.c
		src/ef_utils.c
		port/ef_port.c
		)
endif()

armino_component_register(SRCS "${srcs}"
	INCLUDE_DIRS "${incs}"
	REQUIRES base64
	PRIV_REQUIRES bk_common)
