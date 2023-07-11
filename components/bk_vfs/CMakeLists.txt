set(incs "")
set(srcs "")

if(CONFIG_VFS)
	list(APPEND incs .)
	list(APPEND srcs
		"bk_fdtable.c"
		"bk_filesystem.c"
		"bk_file_utils.c"
		"bk_partition.c"
		"bk_vfs.c"
		"fatfs_adapter.c"
		"littlefs_adapter.c"
		)
endif()

armino_component_register(SRCS "${srcs}"
		INCLUDE_DIRS "${incs}"
		PRIV_REQUIRES bk_common littlefs fatfs
		)
