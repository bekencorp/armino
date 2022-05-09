set(incs "")
set(srcs "")

if (CONFIG_SDCARD_HOST)
	list(APPEND incs "fatfs_sdcard/")

	list(APPEND srcs
		"fatfs_sdcard/ffunicode.c"
		"fatfs_sdcard/ffsystem.c"
		"fatfs_sdcard/disk_io.c"
		"fatfs_sdcard/ff.c"
		"fatfs_sdcard/test_fatfs.c"
		)
endif()

if (CONFIG_USB_HOST)
	list(APPEND incs "fatfs_udisk/")

	list(APPEND srcs
		"fatfs_udisk/cc936.c"
		"fatfs_udisk/ccsbcs.c"
		"fatfs_udisk/disk_io.c"
		"fatfs_udisk/driver_udisk.c"
		"fatfs_udisk/ff.c"
		"fatfs_udisk/playmode.c"
		)
endif()

armino_component_register(SRCS "${srcs}"
		INCLUDE_DIRS "${incs}"
		PRIV_REQUIRES bk_common bk_usb
		)
