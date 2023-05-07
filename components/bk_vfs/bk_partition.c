#include "bk_partition.h"
#include "bk_filesystem.h"

#if 0

struct bk_little_fs_partition g_lfs_partitions[2] = 
{
	{
		.part_type = LFS_FLASH,
		.mount_path = "/lfs_1",
		.part_flash.start_addr = 0x5678,
		.part_flash.size = 4096 * 10,
	},
	{
		.part_type = LFS_FLASH,
		.mount_path = "/lfs_2",
		.part_flash.start_addr = 0x8000,
		.part_flash.size = 4096 * 20,
	},
};

#endif

int bk_lfs_mount(struct bk_little_fs_partition *partitions, int count) {
	int ret;
	int i;

	for (i = 0; i < count; i++) {
		ret = bk_vfs_mount("SOURCE_NONE", partitions[i].mount_path, "littlefs", 0, &partitions[i]);
		if (ret)
			break;
	}

	return ret;
}

int bk_fatfs_mount(struct bk_fatfs_partition *partitions, int count) {
	int ret;
	int i;

	for (i = 0; i < count; i++) {
		ret = bk_vfs_mount(partitions[i].part_dev.device_name, partitions[i].mount_path, "fatfs", 0, &partitions[i]);
		if (ret)
			break;
	}

	return ret;
}

