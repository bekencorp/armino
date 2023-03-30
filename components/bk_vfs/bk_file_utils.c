#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bk_file_utils.h"
#include "bk_fdtable.h"
#include "bk_filesystem.h"
#include "bk_vfs.h"

#include "littlefs_adapter.h"
#include "fatfs_adapter.h"

static int bk_lock_init(void) {
	static int mutex = -1;	//TODO	//recursive ???

	mutex = mutex;

	return 0;
}

int bk_vfs_init(void) {
	static int vfs_inited = 0;
	int ret;

	if (vfs_inited)
		return vfs_inited > 0 ? 0 : -1;
	vfs_inited = -1;
	
	ret = bk_lock_init();
	if (ret)
		return ret;

	ret = bk_fdtable_init();
	if (ret)
		return ret;
	
	ret = bk_filesystem_init();
	if (ret)
		return ret;

	ret = bk_lfs_init();
	if (ret)
		return ret;

	ret = bk_fatfs_init();
	if (ret)
		return ret;

	vfs_inited = 1;
	return 0;
}

int bk_vfs_lock(void) {
	return 0;
}

void bk_vfs_unlock(void) {
}

char *bk_normalize_path(const char *path) {
	char *new_path;
	int len;

	if (!path)
		return NULL;

	if (path[0] == '/') {
		new_path = strdup(path);
	} else {
		char *cwd = bk_vfs_refer_cwd();
		len = strlen(cwd) + strlen(path) + 2;
		new_path = malloc(len);
		if (!new_path)
			return NULL;

		sprintf(new_path, "%s/%s", cwd, path);
	}

	len = strlen(new_path);
	if (new_path[len - 1] == '/')	//remove tailing '/'
		new_path[len - 1] = '\0';

	return new_path;
}

const char *bk_sub_path(const char *mount_point, const char *path) {
	if (strlen(path) == strlen(mount_point))
		return "/";
	else
		return path + strlen(mount_point);
}
