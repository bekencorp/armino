set(incs)
set(srcs)

if(CONFIG_OPENSSL)

set(incs
	include/internal
	include/platform
	include/openssl
	)

set(srcs
	library/ssl_cert.c
	library/ssl_lib.c
	library/ssl_methods.c
	library/ssl_pkey.c
	library/ssl_bio.c
	library/ssl_err.c
	library/ssl_stack.c
	library/ssl_x509.c
	platform/ssl_pm.c
	platform/ssl_port.c
	)
endif()

armino_component_register(SRCS "${srcs}"
	INCLUDE_DIRS "${incs}"
	REQUIRES mbedtls
	PRIV_REQUIRES bk_common
	)