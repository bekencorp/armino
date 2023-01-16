#ifndef __BK_PARTITION_H_
#define __BK_PARTITION_H_

#include <stdint.h>

enum LFS_PARTITION_TYPE {
	LFS_FILE,
	LFS_MEM,
	LFS_FLASH,
};

struct bk_little_fs_partition {
	int part_type;

	const char *mount_path;

	union {
		struct part_file_s {
			const char *file_path;
		} part_file;
		struct part_mem_s {
			uint32_t start_addr;
			uint32_t size;
		} part_mem;
		struct part_flash_s {
			uint32_t start_addr;
			uint32_t size;
		} part_flash;
	};
};

int bk_lfs_mount(struct bk_little_fs_partition *parts, int count);

enum FATFS_PARTITION_TYPE {
	FATFS_SDCARD,
	FATFS_UDISK,
};

struct bk_fatfs_partition {
	int part_type;

	const char *mount_path;

	const char *device_name;
};

int bk_fatfs_mount(struct bk_fatfs_partition *parts, int count);

#endif

