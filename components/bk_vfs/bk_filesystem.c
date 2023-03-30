#include <stdlib.h>
#include <string.h>

#include "bk_filesystem.h"
#include "bk_fdtable.h"
#include "bk_file_utils.h"

#define MAX_FS_TYPES 3

static struct filesystem_proto g_filesystem_impls[MAX_FS_TYPES];

#define MAX_FS_MOUNTS 4

static struct bk_filesystem g_filesystem_table[MAX_FS_MOUNTS];

int bk_filesystem_init(void) {
	int i;

	for (i = 0; i < MAX_FS_TYPES; i++) {
		g_filesystem_impls[i].fs_type = NULL;
	}

	for (i = 0; i < MAX_FS_MOUNTS; i++) {
		g_filesystem_table[i].mount_point = NULL;
	}

	return 0;
}

int bk_register_filesystem(const char *fs_type, struct bk_filesystem_ops *fs_ops, struct bk_file_ops *f_ops) {
	int i;

	if (!fs_type || !fs_ops || !f_ops)
		return -1;

	for (i = 0; i < MAX_FS_TYPES; i++) {
		if (g_filesystem_impls[i].fs_type == NULL) {
			g_filesystem_impls[i].fs_type = strdup(fs_type);
			g_filesystem_impls[i].fs_ops = fs_ops;
			g_filesystem_impls[i].f_ops = f_ops;
			return 0;
		}
	}

	return -1;
}

static struct filesystem_proto *bk_find_filesystem_impl(const char *fs_type) {
	int i;

	if (!fs_type)
		return NULL;

	for (i = 0; i < MAX_FS_TYPES; i++) {
		if (g_filesystem_impls[i].fs_type != NULL) {
			if (strcmp(g_filesystem_impls[i].fs_type, fs_type) == 0) {
				return &g_filesystem_impls[i];
			}
		}
	}

	return NULL;
}

int bk_vfs_mount(const char *source, const char *target,
                 const char *fs_type, unsigned long mount_flags,
                 const void *data) {
	struct filesystem_proto *impl;
	struct bk_filesystem *fs = NULL;
	int i;
	int ret;

	ret = bk_vfs_init();
	if (ret)
		return -1;

	impl = bk_find_filesystem_impl(fs_type);
	if (!impl) {
		return -1;
	}

	if (!impl->fs_ops->mount) {
		return -1;
	}

	for (i = 0; i < MAX_FS_MOUNTS; i++) {
		if (g_filesystem_table[i].mount_point == NULL) {
			fs = &g_filesystem_table[i];
			break;
		}
	}

	if (!fs) {
		return -1;
	}

	fs->mount_point = strdup(target);
	fs->fs_ops = impl->fs_ops;
	fs->f_ops = impl->f_ops;

	ret = fs->fs_ops->mount(fs, mount_flags, data);
	if (ret) {
		free(fs->mount_point);
		fs->mount_point = NULL;
		return -1;
	} else {
		return 0;
	}
}

int bk_vfs_umount(const char *target) {
	struct bk_filesystem *fs = NULL;
	int ret = -1;
	int i;

	for (i = 0; i < MAX_FS_MOUNTS; i++) {
		if (g_filesystem_table[i].mount_point != NULL) {
			if (strcmp(g_filesystem_table[i].mount_point, target) == 0) {
				fs = &g_filesystem_table[i];
				break;
			}
		}
	}

	if (!fs) {
		return -1;
	}

	if (fs->fs_ops->unmount) {
		ret = fs->fs_ops->unmount(fs);
	}
	
	free(fs->mount_point);
	fs->mount_point = NULL;

	return ret;
}

int bk_vfs_umount2(const char *target, int flags) {
	struct bk_filesystem *fs = NULL;
	int ret = -1;
	int i;

	for (i = 0; i < MAX_FS_MOUNTS; i++) {
		if (g_filesystem_table[i].mount_point != NULL) {
			if (strcmp(g_filesystem_table[i].mount_point, target) == 0) {
				fs = &g_filesystem_table[i];
				break;
			}
		}
	}

	if (!fs) {
		return -1;
	}

	if (fs->fs_ops->unmount2) {
		ret = fs->fs_ops->unmount2(fs, flags);
	}
	
	free(fs->mount_point);
	fs->mount_point = NULL;

	return ret;
}

struct bk_filesystem *bk_vfs_lookup(const char *path) {
	struct bk_filesystem *fs = NULL;
	int i;

	for (i = 0; i < MAX_FS_MOUNTS; i++) {
		if (g_filesystem_table[i].mount_point != NULL) {
			if (strncmp(g_filesystem_table[i].mount_point, path, strlen(g_filesystem_table[i].mount_point)) == 0) {
				fs = &g_filesystem_table[i];
				break;
			}
		}
	}

	return fs;
}

int bk_vfs_mkfs(const char *partition_name, const char *fs_type, const void *data) {
	struct filesystem_proto *impl;
	int ret;

	ret = bk_vfs_init();
	if (ret)
		return -1;

	impl = bk_find_filesystem_impl(fs_type);
	if (!impl) {
		return -1;
	}

	if (!impl->fs_ops->mkfs) {
		return -1;
	}

	return impl->fs_ops->mkfs(partition_name, data);
}

int bk_vfs_statfs(const char *path, struct statfs *buf) {
	struct bk_filesystem *fs;

	fs = bk_vfs_lookup(path);
	if (!fs || !fs->fs_ops || !fs->fs_ops->statfs)
		return -1;

	return fs->fs_ops->statfs(fs, buf);
}

