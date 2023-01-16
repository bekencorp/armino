#ifndef __BK_POSIX_H_
#define __BK_POSIX_H_

#include <stddef.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/stat.h>
//#include <sys/statfs.h>
#include <stdarg.h>
#include <fcntl.h>

#include "bk_partition.h"

int filesystem_init(void);

int mount(const char *source, const char *target,
                 const char *filesystemtype, unsigned long mountflags,
                 const void *data);
int umount(const char *target);
int umount2(const char *target, int flags);
int mkfs(const char *partition_name, const char *fs_type, const void *data);

#define FT_REGULAR		0
#define FT_DIRECTORY	2

int open(const char *path, int oflag, ...);
int close(int fd);
ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);
off_t lseek(int fd, off_t offset, int whence);
int unlink(const char *pathname);
int stat(const char *pathname, struct stat *statbuf);
int fstat(int fd, struct stat *statbuf);
int fsync(int fd);
int rename(const char *oldpath, const char *newpath);

struct statfs {
	unsigned long f_bsize;
	unsigned long f_blocks;
	unsigned long f_bfree;
	unsigned long f_bavail;
};

int statfs(const char *path, struct statfs *buf);
int ftruncate(int fd, off_t length);
int access(const char *pathname, int mode);

int fcntl(int fd, int cmd, ...);
int ioctl(int fd, unsigned long request, ...);

#define MAX_PATH_LEN 128

#define DT_UNKNOWN 0
#define DT_REG 8
#define DT_DIR 4

struct dirent {
	uint8_t d_ino;
	uint8_t d_type;
	uint16_t d_reclen;
	char d_name[MAX_PATH_LEN];
};

typedef struct {
	struct bk_filesystem *filesystem;

	struct dirent dir_entry;

	void *dir_data;
} DIR;

int mkdir(const char *pathname, mode_t mode);
int rmdir(const char *pathname);

DIR *opendir(const char *name);
int closedir(DIR *dirp);
struct dirent *readdir(DIR *dirp);
void seekdir(DIR *dirp, long loc);
long telldir(DIR *dirp);
void rewinddir(DIR *dirp);

int chdir(const char *path);
char *getcwd(char *buf, size_t size);


#endif
