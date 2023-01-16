#include "fatfs_adapter.h"
#include "bk_filesystem.h"

#define FS_FATFS "fatfs"

static int _bk_fatfs_mount(struct bk_filesystem *fs, unsigned long mount_flags, const void *data) {
	return -1;
}

static int _bk_fatfs_unmount(struct bk_filesystem *fs) {
	return -1;
}

static int _bk_fatfs_unmount2(struct bk_filesystem *fs, int flag) {
	return -1;
}

static int _bk_fatfs_mkfs(const char *partition_name, const void *data) {
	return -1;
}

static int _bk_fatfs_statfs(struct bk_filesystem *fs, struct statfs *buf) {
	return -1;
}

static struct bk_filesystem_ops g_fatfs_fs_ops = {
	.mount = _bk_fatfs_mount,
	.unmount = _bk_fatfs_unmount,
	.unmount2 = _bk_fatfs_unmount2,
	.mkfs = _bk_fatfs_mkfs,
	.statfs = _bk_fatfs_statfs,
};

static int _bk_fatfs_open(struct bk_file *file, const char *path, int oflag) {
	return -1;
}

static int _bk_fatfs_close(struct bk_file *file) {
	return -1;
}

static ssize_t _bk_fatfs_read(struct bk_file *file, void *buf, size_t count) {
	return -1;
}

static ssize_t _bk_fatfs_write(struct bk_file *file, const void *buf, size_t count) {
	return -1;
}

static off_t _bk_fatfs_lseek(struct bk_file *file, off_t offset, int whence) {
	return -1;
}

static int _bk_fatfs_unlink(struct bk_filesystem *fs, const char *pathname) {
	return -1;
}

static int _bk_fatfs_stat(struct bk_filesystem *fs, const char *pathname, struct stat *statbuf) {
	return -1;
}

static int _bk_fatfs_rename(struct bk_filesystem *fs, const char *oldpath, const char *newpath) {
	return -1;
}

static int _bk_fatfs_fsync(struct bk_file *file) {
	return -1;
}

static int _bk_fatfs_ftruncate(struct bk_file *file, off_t offset) {
	return -1;
}

static int _bk_fatfs_fcntl(struct bk_file *file, int cmd, void *args) {
	return -1;
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
};

int bk_fatfs_init(void) {
	return bk_register_filesystem(FS_FATFS, &g_fatfs_fs_ops, &g_fatfs_file_ops);
}

