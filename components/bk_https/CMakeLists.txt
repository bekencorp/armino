set(incs include)
set(srcs)

if(CONFIG_HTTPS)
	list(APPEND srcs
		bk_https.c
		http_header.c
		http_parser.c
		http_utils.c
		bk_rom_md5.c
		bk_ssl.c
	)
endif()
armino_component_register(SRCS "${srcs}"
	INCLUDE_DIRS "${incs}"
	REQUIRES mbedtls bk_wifi bk_common
	)