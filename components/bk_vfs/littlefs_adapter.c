#include <common/bk_include.h>

#if CONFIG_LITTLEFS

#include <fcntl.h>

#include "littlefs_adapter.h"
#include "bk_filesystem.h"
#include "bk_partition.h"

#include "lfs.h"

#define FS_LITTLEFS "littlefs"

#define USE_RAMBD 1
#define USE_FILEBD 2
#define USE_FLASHBD 3

#define USE_BD USE_FLASHBD

#if (USE_BD == USE_RAMBD)

#include "lfs_rambd.h"

static int setup_lfs_config(struct lfs_config *config, const struct bk_little_fs_partition *part) {
	static const struct lfs_rambd_config defaults = {
		.erase_value=-1,
		.buffer = NULL,
	};
	int ret;

	config->context = malloc(sizeof(lfs_rambd_t));
	if (!config->context)
		return -1;

	config->read = lfs_rambd_read;
	config->prog = lfs_rambd_prog;
	config->erase = lfs_rambd_erase;
	config->sync = lfs_rambd_sync;

	config->read_size = 16;
	config->prog_size = 16;
	config->block_size = 4096;
	config->block_count = 128;
	config->cache_size = 16;
	config->lookahead_size = 16;
	config->block_cycles = 500;

	ret = lfs_rambd_createcfg(config, &defaults);
	if (ret) {
		printf("create rambd failed : %d\n", ret);
		return -1;
	}

	return 0;
}


static void teardown_lfs_config(struct lfs_config *config) {
	lfs_rambd_destroy(config);

	if (config->context) {
		free(config->context);
		config->context = NULL;
	}
}

#elif (USE_BD == USE_FILEBD)

#include "lfs_filebd.h"

static int setup_lfs_config(struct lfs_config *config, const struct bk_little_fs_partition *part) {
	static const struct lfs_filebd_config defaults = {
		.erase_value=-1
	};
	int ret;

	if (!part || !part->part_file.file_path) {
		printf("no file_path\n");
		return -1;
	}

	config->context = malloc(sizeof(lfs_filebd_t));
	if (!config->context)
		return -1;

	config->read = lfs_filebd_read;
	config->prog = lfs_filebd_prog;
	config->erase = lfs_filebd_erase;
	config->sync = lfs_filebd_sync;

	config->read_size = 16;
	config->prog_size = 16;
	config->block_size = 4096;
	config->block_count = 128;
	config->cache_size = 16;
	config->lookahead_size = 16;
	config->block_cycles = 500;

	ret = lfs_filebd_createcfg(config, part->part_file.file_path, &defaults);
	if (ret) {
		printf("create filebd failed : %d\n", ret);
		return -1;
	}

	return 0;
}


static void teardown_lfs_config(struct lfs_config *config) {
	lfs_filebd_destroy(config);

	if (config->context) {
		free(config->context);
		config->context = NULL;
	}
}

#elif (USE_BD == USE_FLASHBD)

#include "lfs_flashbd.h"

static int setup_lfs_config(struct lfs_config *config, const struct bk_little_fs_partition *part) {
	static const struct lfs_flashbd_config defaults = {
		.erase_value=-1
	};
	lfs_flashbd_t *bd;
	int ret;

	if (!part || !part->part_flash.start_addr || !part->part_flash.size) {
		printf("no start_addr or size\n");
		return -1;
	}

	config->context = malloc(sizeof(lfs_flashbd_t));
	if (!config->context)
		return -1;

	bd = (lfs_flashbd_t *)config->context;
	bd->start_addr = part->part_flash.start_addr;
	bd->end_addr = part->part_flash.start_addr + part->part_flash.size;

	config->read = lfs_flashbd_read;
	config->prog = lfs_flashbd_prog;
	config->erase = lfs_flashbd_erase;
	config->sync = lfs_flashbd_sync;

	config->read_size = 16;
	config->prog_size = 16;
	config->block_size = 4096;
	config->block_count = part->part_flash.size / config->block_size;
	config->cache_size = 16;
	config->lookahead_size = 16;
	config->block_cycles = 500;

	ret = lfs_flashbd_createcfg(config, &defaults);
	if (ret) {
		printf("create flashbd failed : %d\n", ret);
		return -1;
	}

	return 0;
}


static void teardown_lfs_config(struct lfs_config *config) {
	lfs_flashbd_destroy(config);

	if (config->context) {
		free(config->context);
		config->context = NULL;
	}
}

#endif

static int _bk_lfs_mkfs(const char *partition_name, const void *data) {
	const struct bk_little_fs_partition *part;
	struct lfs_config config;
	lfs_t lfs;
	int ret;

	part = (const struct bk_little_fs_partition *)data;

	memset(&lfs, 0, sizeof(lfs_t));
	memset(&config, 0, sizeof(struct lfs_config));

	ret = setup_lfs_config(&config, part);
	if (ret) {
		return -1;
	}

	ret = lfs_format(&lfs, &config);
	if (ret) {
		teardown_lfs_config(&config);
		return -1;
	}

	return 0;
}

static int _bk_lfs_mount(struct bk_filesystem *fs, unsigned long mount_flags, const void *data) {
	const struct bk_little_fs_partition *part;
	static struct lfs_config* config;
	lfs_t *lfs;
	int ret;

	part = (const struct bk_little_fs_partition *)data;
	lfs = (lfs_t *)malloc(sizeof(lfs_t) + 8 + sizeof(struct lfs_config));
	if (!lfs)
		return -1;

	memset(lfs, 0, sizeof(lfs_t));
	config = (struct lfs_config *)(lfs + 1);
	memset(config, 0, sizeof(struct lfs_config));

	ret = setup_lfs_config(config, part);
	if (ret) {
		free(lfs);
		return -1;
	}

	ret = lfs_mount(lfs, config);
	if (ret) {
		teardown_lfs_config(config);
		free(lfs);
		return -1;
	}

	fs->fs_data = lfs;
	return 0;
}

static int _bk_lfs_unmount(struct bk_filesystem *fs) {
	lfs_t *lfs;
	int ret;

	lfs = (lfs_t *)fs->fs_data;
	ret = lfs_unmount(lfs);

	free(lfs);
	fs->fs_data = NULL;

	return ret;
}

static int _bk_lfs_unmount2(struct bk_filesystem *fs, int flag) {
	return _bk_lfs_unmount(fs);
}

static int _bk_lfs_statfs(struct bk_filesystem *fs, struct statfs *buf) {
	lfs_t *lfs;
	lfs_ssize_t in_use;

	lfs = (lfs_t *)fs->fs_data;
	in_use = lfs_fs_size(lfs);
	if (in_use < 0)
		return -1;

	buf->f_bsize = lfs->cfg->block_size;
	buf->f_blocks = lfs->cfg->block_count;
	buf->f_bfree = lfs->cfg->block_count - in_use;
	buf->f_bavail = lfs->cfg->block_count - in_use;
	return 0;
}

static struct bk_filesystem_ops g_lfs_fs_ops = {
	.mount = _bk_lfs_mount,
	.unmount = _bk_lfs_unmount,
	.unmount2 = _bk_lfs_unmount2,
	.mkfs = _bk_lfs_mkfs,
	.statfs = _bk_lfs_statfs,
};

static int file_flags_to_lfs(int oflags) {
    int lfsOpenFlag = 0;

    if (oflags & O_CREAT) {
        lfsOpenFlag |= LFS_O_CREAT;
    }

    if (oflags & O_EXCL) {
        lfsOpenFlag |= LFS_O_EXCL;
    }

    if (oflags & O_TRUNC) {
        lfsOpenFlag |= LFS_O_TRUNC;
    }

    if (oflags & O_APPEND) {
        lfsOpenFlag |= LFS_O_APPEND;
    }

    if (oflags & O_RDWR) {
        lfsOpenFlag |= LFS_O_RDWR;
    }

    if (oflags & O_WRONLY) {
        lfsOpenFlag |= LFS_O_WRONLY;
    }

    if (oflags == O_RDONLY) {
        lfsOpenFlag |= LFS_O_RDONLY;
    }

    return lfsOpenFlag;
}

static int _bk_lfs_open(struct bk_file *file, const char *path, int oflag) {
	lfs_t *lfs;
	lfs_file_t *lfs_file;
	int ret;

	lfs = (lfs_t *)file->filesystem->fs_data;

	lfs_file = (lfs_file_t *)malloc(sizeof(lfs_file_t));
	if (!lfs_file)
		return -1;

	memset(lfs_file, 0, sizeof(lfs_file_t));

	oflag = file_flags_to_lfs(oflag);
	ret = lfs_file_open(lfs, lfs_file, file->path, oflag);
	if (ret) {
		free(lfs_file);
		return -1;
	}

	file->f_data = lfs_file;
	return 0;
}

static int _bk_lfs_close(struct bk_file *file) {
	lfs_t *lfs;
	lfs_file_t *lfs_file;
	int ret;

	lfs = (lfs_t *)file->filesystem->fs_data;
	lfs_file = (lfs_file_t *)file->f_data;

	ret = lfs_file_close(lfs, lfs_file);

	free(file->f_data);
	file->f_data = NULL;

	return ret;	
}

static ssize_t _bk_lfs_read(struct bk_file *file, void *buf, size_t count) {
	lfs_t *lfs;
	lfs_file_t *lfs_file;
	int ret;

	lfs = (lfs_t *)file->filesystem->fs_data;
	lfs_file = (lfs_file_t *)file->f_data;

	ret = lfs_file_read(lfs, lfs_file, buf, count);

	return ret;
}

static ssize_t _bk_lfs_write(struct bk_file *file, const void *buf, size_t count) {
	lfs_t *lfs;
	lfs_file_t *lfs_file;
	int ret;

	lfs = (lfs_t *)file->filesystem->fs_data;
	lfs_file = (lfs_file_t *)file->f_data;

	ret = lfs_file_write(lfs, lfs_file, buf, count);

	return ret;
}

static off_t _bk_lfs_lseek(struct bk_file *file, off_t offset, int whence) {
	lfs_t *lfs;
	lfs_file_t *lfs_file;
	int ret;

	lfs = (lfs_t *)file->filesystem->fs_data;
	lfs_file = (lfs_file_t *)file->f_data;

	ret = lfs_file_seek(lfs, lfs_file, offset, whence);

	return ret;
}

static int _bk_lfs_unlink(struct bk_filesystem *fs, const char *pathname) {
	lfs_t *lfs;
	int ret;

	lfs = (lfs_t *)fs->fs_data;
	ret = lfs_remove(lfs, pathname);

	return ret;
}

static int _bk_lfs_stat(struct bk_filesystem *fs, const char *pathname, struct stat *statbuf) {
	struct lfs_info info;
	lfs_t *lfs;
	int ret;

	lfs = (lfs_t *)fs->fs_data;
	ret = lfs_stat(lfs, pathname, &info);
	if (ret == 0) {
		statbuf->st_size = info.size;
		if (info.type == LFS_TYPE_REG)
			statbuf->st_mode = S_IFREG;
		else
			statbuf->st_mode = S_IFDIR;
	}

	return ret;
}

static int _bk_lfs_rename(struct bk_filesystem *fs, const char *oldpath, const char *newpath) {
	lfs_t *lfs;
	int ret;

	lfs = (lfs_t *)fs->fs_data;
	ret = lfs_rename(lfs, oldpath, newpath);

	return ret;
}

static int _bk_lfs_fsync(struct bk_file *file) {
	lfs_t *lfs;
	lfs_file_t *lfs_file;
	int ret;

	lfs = (lfs_t *)file->filesystem->fs_data;
	lfs_file = (lfs_file_t *)file->f_data;

	ret = lfs_file_sync(lfs, lfs_file);

	return ret;
}

static int _bk_lfs_ftruncate(struct bk_file *file, off_t offset) {
	lfs_t *lfs;
	lfs_file_t *lfs_file;
	int ret;

	lfs = (lfs_t *)file->filesystem->fs_data;
	lfs_file = (lfs_file_t *)file->f_data;

	ret = lfs_file_truncate(lfs, lfs_file, offset);

	return ret;
}

static int _bk_lfs_fcntl(struct bk_file *file, int cmd, void *args) {
	return -1;
}

static int _bk_lfs_opendir(bk_dir *dir, const char *pathname) {
	lfs_t *lfs;
	lfs_dir_t *lfs_dir;
	int ret;

	lfs = (lfs_t *)dir->filesystem->fs_data;

	lfs_dir = (lfs_dir_t *)malloc(sizeof(lfs_dir_t));
	if (!lfs_dir)
		return -1;
	memset(lfs_dir, 0, sizeof(lfs_dir_t));

	ret = lfs_dir_open(lfs, lfs_dir, pathname);
	if (ret) {
		free(lfs_dir);
		return -1;
	}

	dir->dir_data = lfs_dir;
	return 0;
}

static int _bk_lfs_readdir(bk_dir *dir, struct dirent *entry) {
	lfs_t *lfs;
	lfs_dir_t *lfs_dir;
	struct lfs_info info;
	int ret;
	int len;

	lfs = (lfs_t *)dir->filesystem->fs_data;
	lfs_dir = (lfs_dir_t *)dir->dir_data;

	ret = lfs_dir_read(lfs, lfs_dir, &info);
	if (ret <= 0)	//error or no more entry
		return -1;

	entry->d_ino = 0;
	if (info.type == LFS_TYPE_REG)
		entry->d_type = DT_REG;
	else if (info.type == LFS_TYPE_DIR)
		entry->d_type = DT_DIR;
	else
		entry->d_type = DT_UNKNOWN;
	entry->d_reclen = sizeof(struct dirent);

	if (sizeof(entry->d_name) - 1 < strlen(info.name))
		len = sizeof(entry->d_name) - 1;
	else
		len = strlen(info.name);
	strncpy(entry->d_name, info.name, len + 1);

	return 0;
}

static int _bk_lfs_closedir(bk_dir *dir) {
	lfs_t *lfs;
	lfs_dir_t *lfs_dir;
	int ret;

	lfs = (lfs_t *)dir->filesystem->fs_data;
	lfs_dir = (lfs_dir_t *)dir->dir_data;

	ret = lfs_dir_close(lfs, lfs_dir);

	free(lfs_dir);
	dir->dir_data = NULL;

	return ret;
}

static int _bk_lfs_mkdir(struct bk_filesystem *fs, const char *pathname) {
	lfs_t *lfs;
	int ret;

	lfs = (lfs_t *)fs->fs_data;

	ret = lfs_mkdir(lfs, pathname);
	return ret;
}

static int _bk_lfs_rmdir(struct bk_filesystem *fs, const char *pathname) {
	lfs_t *lfs;
	int ret;

	lfs = (lfs_t *)fs->fs_data;

	ret = lfs_remove(lfs, pathname);
	return ret;
}

static struct bk_file_ops g_lfs_file_ops = {
	.open = _bk_lfs_open,
	.close = _bk_lfs_close,
	.read = _bk_lfs_read,
	.write = _bk_lfs_write,
	.lseek = _bk_lfs_lseek,
	.unlink = _bk_lfs_unlink,
	.stat = _bk_lfs_stat,
	.rename = _bk_lfs_rename,
	.fsync = _bk_lfs_fsync,
	.ftruncate = _bk_lfs_ftruncate,
	.fcntl = _bk_lfs_fcntl,

	.opendir = _bk_lfs_opendir,
	.readdir = _bk_lfs_readdir,
	.closedir = _bk_lfs_closedir,
	.mkdir = _bk_lfs_mkdir,
	.rmdir = _bk_lfs_rmdir,
};

int bk_lfs_init(void) {
	return bk_register_filesystem(FS_LITTLEFS, &g_lfs_fs_ops, &g_lfs_file_ops);
}


#else

int bk_lfs_init(void) {
	return 0;
}

#endif
