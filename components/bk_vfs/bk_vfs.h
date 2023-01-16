#ifndef __BK_VFS_H_
#define __BK_VFS_H_

#include <stddef.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/stat.h>
//#include <sys/statfs.h>
#include <stdarg.h>
#include <fcntl.h>

#include "bk_posix.h"	//for DIR

int bk_vfs_open(const char *path, int oflag);
int bk_vfs_close(int fd);
ssize_t bk_vfs_read(int fd, void *buf, size_t count);
ssize_t bk_vfs_write(int fd, const void *buf, size_t count);
off_t bk_vfs_lseek(int fd, off_t offset, int whence);

int bk_vfs_unlink(const char *pathname);
int bk_vfs_stat(const char *pathname, struct stat *statbuf);
int bk_vfs_fstat(int fd, struct stat *statbuf);
int bk_vfs_rename(const char *oldpath, const char *newpath);

int bk_vfs_fsync(int fd);
int bk_vfs_ftruncate(int fd, off_t offset);
int bk_vfs_fcntl(int fd, int cmd, void *arg);

int bk_vfs_mkdir(const char *pathname, mode_t mode);
int bk_vfs_rmdir(const char *pathname);

DIR *bk_vfs_opendir(const char *name);
int bk_vfs_closedir(DIR *dirp);
struct dirent *bk_vfs_readdir(DIR *dirp);
void bk_vfs_seekdir(DIR *dirp, long loc);
long bk_vfs_telldir(DIR *dirp);
void bk_vfs_rewinddir(DIR *dirp);

int bk_vfs_chdir(const char *path);
char *bk_vfs_getcwd(char *buf, size_t size);

char *bk_vfs_refer_cwd(void);	//extra

#endif
