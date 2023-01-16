#ifndef __BK_FILESYSTEM_H_
#define __BK_FILESYSTEM_H_

#include <stddef.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/stat.h>
//#include <sys/statfs.h>
#include "bk_posix.h"	//for struct statfs, DIR
#include <stdarg.h>

struct bk_filesystem;
struct bk_file;

struct bk_filesystem_ops {
	int (*mount)	(struct bk_filesystem *fs, unsigned long mount_flags, const void *data);
	int (*unmount)	(struct bk_filesystem *fs);
	int (*unmount2)	(struct bk_filesystem *fs, int flag);

	int (*mkfs)		(const char *partition_name, const void *data);
	int (*statfs)	(struct bk_filesystem *fs, struct statfs *buf);
};

struct bk_filesystem {
	char *mount_point;
	const struct bk_filesystem_ops *fs_ops;
	struct bk_file_ops *f_ops;

	void *fs_data;
};

struct bk_file_ops {
	int (*open)		(struct bk_file *file, const char *path, int oflag);
	int (*close)	(struct bk_file *file);
	ssize_t (*read)	(struct bk_file *file, void *buf, size_t count);
	ssize_t (*write)(struct bk_file *file, const void *buf, size_t count);
	off_t (*lseek)	(struct bk_file *file, off_t offset, int whence);

	int (*unlink)	(struct bk_filesystem *fs, const char *pathname);
	int (*stat)		(struct bk_filesystem *fs, const char *pathname, struct stat *statbuf);
	int (*rename)	(struct bk_filesystem *fs, const char *oldpath, const char *newpath);

	int (*fsync)	(struct bk_file *file);
	int (*ftruncate)(struct bk_file *file, off_t offset);
	int (*fcntl)	(struct bk_file *file, int cmd, void *args);	//cmd : F_GETFL, F_SETFL, fs special

	int (*opendir)	(DIR *dir, const char *pathname);
	int (*readdir)	(DIR *dir, struct dirent *entry);
	int (*closedir)	(DIR *dir);

	int (*mkdir)	(struct bk_filesystem *fs, const char *pathname);
	int (*rmdir)	(struct bk_filesystem *fs, const char *pathname);
};

#define BK_FILE_MAGIC 0xADCB

struct bk_file {
	unsigned short magic;
	unsigned short type;

	char *path;

	struct bk_filesystem *filesystem;
	struct bk_file_ops *f_ops;

	uint32_t flags;
	size_t size;
	off_t pos;

	void *f_data;
};

struct filesystem_proto {
	const char *fs_type;
	struct bk_filesystem_ops *fs_ops;
	struct bk_file_ops *f_ops;
};

int bk_filesystem_init(void);
int bk_register_filesystem(const char *fs_type, struct bk_filesystem_ops *fs_ops, struct bk_file_ops *f_ops);

int bk_vfs_mount(const char *source, const char *target,
                 const char *fs_type, unsigned long mount_flags,
                 const void *data);
int bk_vfs_umount(const char *target);
int bk_vfs_umount2(const char *target, int flags);

struct bk_filesystem *bk_vfs_lookup(const char *path);

int bk_vfs_mkfs(const char *partition_name, const char *fs_type, const void *data);
int bk_vfs_statfs(const char *path, struct statfs *buf);

#endif
