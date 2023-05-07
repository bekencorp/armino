set(incs "")
set(srcs "")

if(CONFIG_LITTLEFS)
	list(APPEND incs .)
	list(APPEND srcs
		"lfs.c"
		"lfs_util.c"
		"lfs_flashbd.c"
		)
endif()

armino_component_register(SRCS "${srcs}"
		INCLUDE_DIRS "${incs}"
		PRIV_REQUIRES bk_common
		)
