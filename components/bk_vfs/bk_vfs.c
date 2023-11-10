#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include "bk_vfs.h"
#include "bk_fdtable.h"
#include "bk_filesystem.h"
#include "bk_file_utils.h"

#ifndef ENOTSUP
#define ENOTSUP		EOPNOTSUPP
#endif

static void bk_set_errno(int _errno) {
	errno = -_errno;
}

static struct bk_file *get_file(int fd) {
	struct bk_file *file;

	if (bk_vfs_lock() != 0) {
		bk_set_errno(EFAULT);
		return NULL;
	}

	file = bk_fd_to_file(fd);
	if (!file) {
		bk_set_errno(EBADF);
		bk_vfs_unlock();
	}

	return file;
}

static void put_file(struct bk_file *file) {
	if (!file)
		return;

	bk_vfs_unlock();
}


int bk_vfs_open(const char *path, int oflag) {
	struct bk_file *file;
	char *full_path = NULL;
	struct bk_filesystem *fs;
	int ret = -1;
	int fd = -1;

	if (bk_vfs_lock() != 0) {
		return -1;
	}

	full_path = bk_normalize_path(path);
	if (!full_path)
		goto out;

	fs = bk_vfs_lookup(full_path);
	if (!fs) {
		goto out;
	}

	file = bk_file_get();	//acquire file
	if (!file) {
		bk_set_errno(ENFILE);
		goto out;
	}

	file->path = strdup(bk_sub_path(fs->mount_point, full_path));
	if (!file->path) {
		bk_set_errno(ENOMEM);
		goto out;
	}

	file->filesystem = fs;
	file->f_ops = fs->f_ops;
	file->flags = oflag;
	file->size = 0;
	file->pos = 0;

	if (file->f_ops && file->f_ops->open) {
		ret = file->f_ops->open(file, file->path, oflag);
		if (ret == 0)
			fd = bk_file_to_fd(file);
		else
			bk_file_put(file);
	} else {
		bk_set_errno(ENOTSUP);
		bk_file_put(file);
	}

out : 
	if (full_path)
		free(full_path);
	bk_vfs_unlock();

	return fd;
}

int bk_vfs_close(int fd) {
	struct bk_file *file;
	int ret = -1;

	file = get_file(fd);
	if (!file) {
		return -1;
	}

	if (file->f_ops && file->f_ops->close) {
		ret = file->f_ops->close(file);
	} else {
		bk_set_errno(ENOTSUP);
	}

	if (file->path) {
		free(file->path);
		file->path = NULL;
	}

	put_file(file);

	bk_file_put(file);	//release file
	return ret;
}

ssize_t bk_vfs_read(int fd, void *buf, size_t count) {
	struct bk_file *file;
	ssize_t ret = -1;

	file = get_file(fd);
	if (!file) {
		return -1;
	}

	if (file->f_ops && file->f_ops->read) {
		ret = file->f_ops->read(file, buf, count);
	} else {
		bk_set_errno(ENOTSUP);
	}

	put_file(file);

	return ret;
}

ssize_t bk_vfs_write(int fd, const void *buf, size_t count) {
	struct bk_file *file;
	ssize_t ret = -1;

	file = get_file(fd);
	if (!file) {
		return -1;
	}

	if (file->f_ops && file->f_ops->write) {
		ret = file->f_ops->write(file, buf, count);
	} else {
		bk_set_errno(ENOTSUP);
	}

	put_file(file);

	return ret;
}

off_t bk_vfs_lseek(int fd, off_t offset, int whence) {
	struct bk_file *file;
	off_t ret = -1;

	file = get_file(fd);
	if (!file) {
		return -1;
	}

	if (file->f_ops && file->f_ops->lseek) {
		ret = file->f_ops->lseek(file, offset, whence);
	} else {
		bk_set_errno(ENOTSUP);
	}

	put_file(file);

	return ret;
}

int bk_vfs_unlink(const char *pathname) {
	char *full_path = NULL;
	struct bk_filesystem *fs;
	int ret = -1;

	if (bk_vfs_lock() != 0) {
		return -1;
	}

	full_path = bk_normalize_path(pathname);
	if (!full_path) {
		goto out;
	}

	fs = bk_vfs_lookup(full_path);
	if (!fs) {
		goto out;
	}

	if (fs->f_ops->unlink) {
		ret = fs->f_ops->unlink(fs, bk_sub_path(fs->mount_point, full_path));
	} else {
		bk_set_errno(ENOTSUP);
	}

out:
	if (full_path)
		free(full_path);
	bk_vfs_unlock();

	return ret;
}

int bk_vfs_stat(const char *pathname, struct stat *statbuf) {
	char *full_path = NULL;
	struct bk_filesystem *fs;
	int ret = -1;

	if (bk_vfs_lock() != 0) {
		return -1;
	}

	full_path = bk_normalize_path(pathname);
	if (!full_path) {
		goto out;
	}

	fs = bk_vfs_lookup(full_path);
	if (!fs) {
		goto out;
	}

	if (fs->f_ops->stat) {
		ret = fs->f_ops->stat(fs, bk_sub_path(fs->mount_point, full_path), statbuf);
	} else {
		bk_set_errno(ENOTSUP);
	}

out:
	if (full_path)
		free(full_path);
	bk_vfs_unlock();

	return ret;
}

int bk_vfs_fstat(int fd, struct stat *statbuf) {
	return -1;
}

int bk_vfs_rename(const char *oldpath, const char *newpath) {
	char *full_path_old = NULL;
	char *full_path_new = NULL;
	struct bk_filesystem *fs_old;
	struct bk_filesystem *fs_new;
	int ret = -1;

	if (bk_vfs_lock() != 0) {
		return -1;
	}

	full_path_old = bk_normalize_path(oldpath);
	if (!full_path_old) {
		goto out;
	}

	fs_old = bk_vfs_lookup(full_path_old);
	if (!fs_old) {
		goto out;
	}

	if (!fs_old->f_ops->rename) {
		bk_set_errno(ENOTSUP);
		goto out;
	}

	full_path_new = bk_normalize_path(newpath);
	if (!full_path_new) {
		goto out;
	}

	fs_new = bk_vfs_lookup(full_path_new);
	if (!fs_new) {
		goto out;
	}

	if (fs_old == fs_new) {
		ret = fs_old->f_ops->rename(fs_old, 
				bk_sub_path(fs_old->mount_point, full_path_old),
				bk_sub_path(fs_new->mount_point, full_path_new));
	} else {
		bk_set_errno(ENOTSUP);
	}

out:
	if (full_path_old)
		free(full_path_old);
	if (full_path_new)
		free(full_path_new);
	bk_vfs_unlock();

	return ret;
}

int bk_vfs_fsync(int fd) {
	struct bk_file *file;
	int ret = -1;

	file = get_file(fd);
	if (!file) {
		return -1;
	}

	if (file->f_ops && file->f_ops->fsync) {
		ret = file->f_ops->fsync(file);
	} else {
		bk_set_errno(ENOTSUP);
	}

	put_file(file);

	return ret;
}

int bk_vfs_ftruncate(int fd, off_t offset) {
	struct bk_file *file;
	int ret = -1;

	file = get_file(fd);
	if (!file) {
		return -1;
	}

	if (file->f_ops && file->f_ops->ftruncate) {
		ret = file->f_ops->ftruncate(file, offset);
	} else {
		bk_set_errno(ENOTSUP);
	}

	put_file(file);

	return ret;
}

int bk_vfs_fcntl(int fd, int cmd, void *arg) {
	struct bk_file *file;
	int ret = -1;

	file = get_file(fd);
	if (!file) {
		return -1;
	}

	if (file->f_ops && file->f_ops->fcntl) {
		ret = file->f_ops->fcntl(file, cmd, arg);
	} else {
		bk_set_errno(ENOTSUP);
	}

	put_file(file);

	return ret;
}

int bk_vfs_mkdir(const char *pathname, mode_t mode) {
	char *full_path = NULL;
	struct bk_filesystem *fs;
	int ret = -1;

	if (bk_vfs_lock() != 0) {
		return -1;
	}

	full_path = bk_normalize_path(pathname);
	if (!full_path) {
		goto out;
	}

	fs = bk_vfs_lookup(full_path);
	if (!fs) {
		goto out;
	}

	if (fs->f_ops && fs->f_ops->mkdir) {
		ret = fs->f_ops->mkdir(fs, bk_sub_path(fs->mount_point, full_path));
	} else {
		bk_set_errno(ENOTSUP);
	}

out:
	if (full_path)
		free(full_path);
	bk_vfs_unlock();

	return ret;
}

int bk_vfs_rmdir(const char *pathname) {
	char *full_path = NULL;
	struct bk_filesystem *fs;
	int ret = -1;

	if (bk_vfs_lock() != 0) {
		return -1;
	}

	full_path = bk_normalize_path(pathname);
	if (!full_path) {
		goto out;
	}

	fs = bk_vfs_lookup(full_path);
	if (!fs) {
		goto out;
	}

	if (fs->f_ops && fs->f_ops->rmdir) {
		ret = fs->f_ops->rmdir(fs, bk_sub_path(fs->mount_point, full_path));
	} else {
		bk_set_errno(ENOTSUP);
	}

out:
	if (full_path)
		free(full_path);
	bk_vfs_unlock();

	return ret;
}


DIR *bk_vfs_opendir(const char *name) {
	bk_dir *dirp = NULL;
	char *full_path = NULL;
	struct bk_filesystem *fs;
	int ret = -1;

	if (bk_vfs_lock() != 0) {
		return NULL;
	}

	full_path = bk_normalize_path(name);
	if (!full_path) {
		goto out;
	}

	fs = bk_vfs_lookup(full_path);
	if (!fs) {
		goto out;
	}

	if (fs->f_ops && fs->f_ops->opendir) {
		dirp = (bk_dir *)malloc(sizeof(bk_dir));
		if (!dirp) {
			goto out;
		}
		dirp->filesystem = fs;
		ret = fs->f_ops->opendir(dirp, bk_sub_path(fs->mount_point, full_path));
		if (ret) {
			free(dirp);
			dirp = NULL;
		}
	} else {
		bk_set_errno(ENOTSUP);
	}

out:
	if (full_path)
		free(full_path);
	bk_vfs_unlock();

	return (DIR *)dirp;
}

int bk_vfs_closedir(DIR *dirp_) {
	bk_dir *dirp;
	int ret = -1;

	if (!dirp_)
		return -1;

	dirp = (bk_dir *)dirp_;
	
	if (bk_vfs_lock() != 0) {
		return -1;
	}

	if (dirp->filesystem && dirp->filesystem->f_ops->closedir) {
		ret = dirp->filesystem->f_ops->closedir(dirp);
	} else {
		bk_set_errno(ENOTSUP);
	}

	free(dirp);
	bk_vfs_unlock();

	return ret;
}

struct dirent *bk_vfs_readdir(DIR *dirp_) {
	bk_dir *dirp;
	int ret = -1;

	if (!dirp_)
		return NULL;
	
	dirp = (bk_dir *)dirp_;

	if (bk_vfs_lock() != 0) {
		return NULL;
	}

	if (dirp->filesystem && dirp->filesystem->f_ops->readdir) {
		ret = dirp->filesystem->f_ops->readdir(dirp, &dirp->dir_entry);
	} else {
		bk_set_errno(ENOTSUP);
	}

	bk_vfs_unlock();

	if (ret == 0)
		return &dirp->dir_entry;
	else
		return NULL;
}

void bk_vfs_seekdir(DIR *dirp, long loc) {
}

long bk_vfs_telldir(DIR *dirp) {
	return -1;
}

void bk_vfs_rewinddir(DIR *dirp) {
}

static char working_directory[MAX_PATH_LEN] = {"/"};

int bk_vfs_chdir(const char *path) {
	char *full_path;
	
	if (!path)
		return -1;

	if (strlen(path) > MAX_PATH_LEN - 1)
		return -1;

	full_path = bk_normalize_path(path);
	if (!full_path)
		return -1;

	strcpy(working_directory, full_path);
	free(full_path);

	return 0;
}

char *bk_vfs_getcwd(char *buf, size_t size) {
	if (!buf)
		return NULL;

	strncpy(buf, working_directory, size);
	return buf;
}

char *bk_vfs_refer_cwd(void) {
	return working_directory;
}

