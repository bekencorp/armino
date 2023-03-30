#include <common/bk_include.h>

#if CONFIG_FATFS

#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include "fatfs_adapter.h"
#include "bk_filesystem.h"
#include "bk_partition.h"

#include "ff.h"

#define FS_FATFS "fatfs"

typedef struct bk_fatfs {
	FATFS ffs;
	char vol_str[3];
} BK_FATFS;

static const char *devices [] = {	//keep in sync with disk_io.c
	FATFS_DEV_RAM,
	FATFS_DEV_SDCARD,
	FATFS_DEV_UDISK,
	FATFS_DEV_FLASH,
	NULL
};

static int _get_device_idx(const char *device_name) {
	int i = 0;

	while(devices[i] != NULL) {
		if (strcmp(device_name, devices[i]) == 0)
			return i;
		i++;
	}

	return -1;
}

static int _bk_fatfs_mount(struct bk_filesystem *fs, unsigned long mount_flags, const void *data) {
	BK_FATFS *bk_ffs;
	FATFS *ffs;
	struct bk_fatfs_partition *part;
	int ret;
	int idx;

	part = (struct bk_fatfs_partition *)data;

	idx = _get_device_idx(part->part_dev.device_name);
	if (idx < 0)
		return -1;

	bk_ffs = (BK_FATFS *)malloc(sizeof(BK_FATFS));
	if (!bk_ffs)
		return -1;
	ffs = (FATFS *)bk_ffs;

	bk_ffs->vol_str[0] = idx + '0';
	bk_ffs->vol_str[1] = ':';
	bk_ffs->vol_str[2] = '\0';

	ret = f_mount(ffs, bk_ffs->vol_str, 1);
	if (ret) {
		free(bk_ffs);
		return -1;
	}

	fs->fs_data = bk_ffs;
	return 0;
}

static int _bk_fatfs_unmount(struct bk_filesystem *fs) {
	BK_FATFS *bk_ffs;
	int ret;

	bk_ffs = (BK_FATFS *)fs->fs_data;
	//ret = f_unmount(bk_ffs->vol_str);	// TODO : FR_NOT_ENABLED
	ret = f_unmount(1, bk_ffs->vol_str, 0);

	free(bk_ffs);
	fs->fs_data = NULL;

	return ret;
}

static int _bk_fatfs_unmount2(struct bk_filesystem *fs, int flag) {
	return _bk_fatfs_unmount(fs);
}

static int _bk_fatfs_mkfs(const char *partition_name, const void *data) {
	#define TEMP_SIZE 1024
	struct bk_fatfs_partition *part;
	char *temp;
	char vol_str[3];
	int ret;
	int idx;

	part = (struct bk_fatfs_partition *)data;

	idx = _get_device_idx(part->part_dev.device_name);
	if (idx < 0)
		return -1;

	temp = malloc(TEMP_SIZE);
	if (!temp)
		return -1;

	vol_str[0] = idx + '0';
	vol_str[1] = ':';
	vol_str[2] = '\0';

	//ret = f_mkfs(vol_str, NULL, temp, TEMP_SIZE);
	ret = f_mkfs(vol_str, FM_ANY, 65536, temp, TEMP_SIZE);

	free(temp);
	return ret;
}

static int _bk_fatfs_statfs(struct bk_filesystem *fs, struct statfs *buf) {
	BK_FATFS *bk_ffs;
	FATFS *ffs;
	uint32_t free_clust;
	int ret;

	bk_ffs = (BK_FATFS *)fs->fs_data;

	ret = f_getfree(bk_ffs->vol_str, &free_clust, &ffs);
	if (ret)
		return -1;

#if FF_MAX_SS != FF_MIN_SS
	buf->f_bsize = ffs->ssize * ffs->csize;
#else
	buf->f_bsize = FF_MIN_SS * ffs->csize;
#endif
	buf->f_blocks = ffs->n_fatent - 2;
	buf->f_bfree = free_clust;
	buf->f_bavail = free_clust;

	return 0;
}

static struct bk_filesystem_ops g_fatfs_fs_ops = {
	.mount = _bk_fatfs_mount,
	.unmount = _bk_fatfs_unmount,
	.unmount2 = _bk_fatfs_unmount2,
	.mkfs = _bk_fatfs_mkfs,
	.statfs = _bk_fatfs_statfs,
};

static int file_flags_to_fatfs(int oflags) {
    int fatfs_open_flags = 0;

	fatfs_open_flags |= FA_READ;

    if (oflags & O_WRONLY) {
        fatfs_open_flags |= FA_WRITE;
    }

    if ((oflags & O_ACCMODE) & O_RDWR) {
        fatfs_open_flags |= FA_WRITE;
    }
    /* Creates a new file if the file is not existing, otherwise, just open it. */
    if (oflags & O_CREAT) {
        fatfs_open_flags |= FA_OPEN_ALWAYS;
        /* Creates a new file. If the file already exists, the function shall fail. */
        if (oflags & O_EXCL) {
            fatfs_open_flags |= FA_CREATE_NEW;
        }
    }
    /* Creates a new file. If the file already exists, its length shall be truncated to 0. */
    if (oflags & O_TRUNC) {
        fatfs_open_flags |= FA_CREATE_ALWAYS;
    }

    if (oflags & O_APPEND) {
        fatfs_open_flags |= FA_OPEN_APPEND;
    }

    return fatfs_open_flags;
}

static char *get_full_name(struct bk_filesystem *fs, const char *path) {
	BK_FATFS *bk_ffs;
	char *full_path;

	bk_ffs = (BK_FATFS *)fs->fs_data;

	full_path = malloc(strlen(path) + 2 + 1);	// 2 for "x:", 1 for '\0'
	if (!full_path)
		return NULL;

	strcpy(full_path, bk_ffs->vol_str);
	strcpy(full_path + 2, path);
	return full_path;
}

static int _bk_fatfs_open(struct bk_file *file, const char *path, int oflag) {
	FIL *fil;
	int ret;
	char *full_name;

	fil = (FIL *)malloc(sizeof(FIL));
	if (!fil)
		return -1;

	full_name = get_full_name(file->filesystem, path);
	if (!full_name)
		return -1;

	oflag = file_flags_to_fatfs(oflag);
	ret = f_open(fil, full_name, oflag);
	free(full_name);

	if (ret) {
		free(fil);
		return -1;
	}

	file->f_data = fil;
	return 0;
}

static int _bk_fatfs_close(struct bk_file *file) {
	FIL *fil;
	int ret;

	fil = (FIL *)file->f_data;

	ret = f_close(fil);

	free(file->f_data);
	file->f_data = NULL;

	return ret;
}

static ssize_t _bk_fatfs_read(struct bk_file *file, void *buf, size_t count) {
	FIL *fil;
	int ret;
	UINT len;

	fil = (FIL *)file->f_data;

	ret = f_read(fil, buf, count, &len);
	if (ret)
		return -1;

	return len;
}

static ssize_t _bk_fatfs_write(struct bk_file *file, const void *buf, size_t count) {
	FIL *fil;
	int ret;
	UINT len;

	fil = (FIL *)file->f_data;

	ret = f_write(fil, buf, count, &len);
	if (ret)
		return -1;

	return len;
}

static off_t _bk_fatfs_lseek(struct bk_file *file, off_t offset, int whence) {
	FIL *fil;
	int ret;
	off_t pos;

	fil = (FIL *)file->f_data;

	if (whence == SEEK_SET) {
		pos = 0;
	} else if (whence == SEEK_CUR) {
		pos = f_tell(fil);
	} else if (whence == SEEK_END) {
		pos = f_size(fil);
	} else {
		return -1;
	}

	ret = f_lseek(fil, pos + offset);
	if (ret)
		return -1;

	pos = f_tell(fil);
	return pos;
}

static int _bk_fatfs_unlink(struct bk_filesystem *fs, const char *pathname) {
	int ret;
	char *full_name;

	full_name = get_full_name(fs, pathname);
	if (!full_name)
		return -1;

	ret = f_unlink(full_name);
	free(full_name);

	return ret;
}

static int _bk_fatfs_stat(struct bk_filesystem *fs, const char *pathname, struct stat *statbuf) {
	int ret;
	char *full_name;
	FILINFO file_info = {0};

	full_name = get_full_name(fs, pathname);
	if (!full_name)
		return -1;

	ret = f_stat(full_name, &file_info);
	free(full_name);

	if (ret == 0) {
		statbuf->st_size = file_info.fsize;
		if (file_info.fattrib & AM_DIR)
			statbuf->st_mode = S_IFDIR;
		else
			statbuf->st_mode = S_IFREG;
	}

	return 0;
}

static int _bk_fatfs_rename(struct bk_filesystem *fs, const char *oldpath, const char *newpath) {
	int ret;
	char *full_name_old;
	char *full_name_new;

	full_name_old = get_full_name(fs, oldpath);
	if (!full_name_old)
		return -1;

	full_name_new = get_full_name(fs, newpath);
	if (!full_name_new) {
		free(full_name_old);
		return -1;
	}

	ret = f_rename(full_name_old, full_name_new);
	free(full_name_old);
	free(full_name_new);

	return ret;
}

static int _bk_fatfs_fsync(struct bk_file *file) {
	FIL *fil;
	int ret;

	fil = (FIL *)file->f_data;

	ret = f_sync(fil);

	return ret;
}

static int _bk_fatfs_ftruncate(struct bk_file *file, off_t offset) {	// TODO
	FIL *fil;
	int ret;
	struct stat statbuf;

	fil = (FIL *)file->f_data;

	ret = _bk_fatfs_stat(file->filesystem, file->path, &statbuf);
	if (ret)
		return -1;

	if (offset > statbuf.st_size) {
#if FF_USE_EXPAND
		ret = f_expand(fil, offset, 1);
#else
		ret = -1;
#endif
	} else if (offset < statbuf.st_size) {
		fil->fptr = offset;
		ret = f_truncate(fil);
	} else {
		ret = 0;
	}

	return ret;
}

static int _bk_fatfs_fcntl(struct bk_file *file, int cmd, void *args) {
	return -1;
}

static int _bk_fatfs_opendir(bk_dir *dir, const char *pathname) {
	DIR *dp;
	int ret;
	char *full_name;

	full_name = get_full_name(dir->filesystem, pathname);
	if (!full_name)
		return -1;

	dp = (DIR *)malloc(sizeof(DIR));
	if (!dp)
		return -1;

	ret = f_opendir(dp, full_name);
	free(full_name);

	if (ret) {
		free(dp);
		return -1;
	}

	dir->dir_data = dp;
	return 0;
}

static int _bk_fatfs_readdir(bk_dir *dir, struct dirent *entry) {
	DIR *dp;
	int ret;
	FILINFO file_info = {0};
	int len;

	dp = (DIR *)dir->dir_data;
	ret = f_readdir(dp, &file_info);
	if (ret || (file_info.fname[0] == '\0'))
		return -1;

	entry->d_ino = 0;
	if (file_info.fattrib & AM_DIR)
		entry->d_type = DT_DIR;
	else
		entry->d_type = DT_REG;
	entry->d_reclen = sizeof(struct dirent);

	if (sizeof(entry->d_name) - 1 < strlen(file_info.fname))
		len = sizeof(entry->d_name) - 1;
	else
		len = strlen(file_info.fname);
	strncpy(entry->d_name, file_info.fname, len + 1);

	return 0;
}

static int _bk_fatfs_closedir(bk_dir *dir) {
	DIR *dp;
	int ret;

	dp = (DIR *)dir->dir_data;
	ret = f_closedir(dp);

	free(dp);
	dir->dir_data = NULL;

	return ret;
}

static int _bk_fatfs_mkdir(struct bk_filesystem *fs, const char *pathname) {
	int ret;
	char *full_name;

	full_name = get_full_name(fs, pathname);
	if (!full_name)
		return -1;

	ret = f_mkdir(full_name);
	free(full_name);

	return ret;
}

static int _bk_fatfs_rmdir(struct bk_filesystem *fs, const char *pathname) {
	int ret;
	char *full_name;

	full_name = get_full_name(fs, pathname);
	if (!full_name)
		return -1;

	ret = f_rmdir(full_name);
	free(full_name);

	return ret;
}

static struct bk_file_ops g_fatfs_file_ops = {
	.open = _bk_fatfs_open,
	.close = _bk_fatfs_close,
	.read = _bk_fatfs_read,
	.write = _bk_fatfs_write,
	.lseek = _bk_fatfs_lseek,
	.unlink = _bk_fatfs_unlink,
	.stat = _bk_fatfs_stat,
	.rename = _bk_fatfs_rename,
	.fsync = _bk_fatfs_fsync,
	.ftruncate = _bk_fatfs_ftruncate,
	.fcntl = _bk_fatfs_fcntl,

	.opendir = _bk_fatfs_opendir,
	.readdir = _bk_fatfs_readdir,
	.closedir = _bk_fatfs_closedir,
	.mkdir = _bk_fatfs_mkdir,
	.rmdir = _bk_fatfs_rmdir,
};

int bk_fatfs_init(void) {
	return bk_register_filesystem(FS_FATFS, &g_fatfs_fs_ops, &g_fatfs_file_ops);
}

#else

int bk_fatfs_init(void) {
	return 0;
}

#endif
