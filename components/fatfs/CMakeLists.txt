set(incs "")
set(srcs "")

if (CONFIG_FATFS)
	list(APPEND incs .)
	list(APPEND srcs
		"ffunicode.c"
		"ffsystem.c"
		"disk_io.c"
		"ff.c"
		"test_fatfs.c"
		"driver_udisk.c"
		"cc936.c"
		"ccsbcs.c"
		)
endif()

armino_component_register(SRCS "${srcs}"
		INCLUDE_DIRS "${incs}"
		PRIV_REQUIRES bk_common bk_usb
		)
