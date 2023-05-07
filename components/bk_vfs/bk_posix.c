#include "bk_posix.h"
#include "bk_vfs.h"
#include "bk_filesystem.h"
#include "bk_file_utils.h"

#define FIX_GLOSS_LIB

#ifdef FIX_GLOSS_LIB
extern void bk_printf(const char *fmt, ...);
ssize_t _write(int fd, const void *buf, size_t count) {
	bk_printf("%s", buf);
	return 0;
}
#endif

int filesystem_init(void) {
	return bk_vfs_init();
}

int mount(const char *source, const char *target,
                 const char *filesystemtype, unsigned long mountflags,
                 const void *data) {
	return bk_vfs_mount(source, target, filesystemtype, mountflags, data);
}

int umount(const char *target) {
	return bk_vfs_umount(target);
}

int umount2(const char *target, int flags) {
	return bk_vfs_umount2(target, flags);
}

int mkfs(const char *partition_name, const char *fs_type, const void *data) {
	return bk_vfs_mkfs(partition_name, fs_type, data);
}

int open(const char *path, int oflag, ...) {
	return bk_vfs_open(path, oflag);
}

int close(int fd){
	return bk_vfs_close(fd);
}

ssize_t read(int fd, void *buf, size_t count) {
	return bk_vfs_read(fd, buf, count);
}

ssize_t write(int fd, const void *buf, size_t count) {
	return bk_vfs_write(fd, buf, count);
}

off_t lseek(int fd, off_t offset, int whence) {
	return bk_vfs_lseek(fd, offset, whence);
}

int unlink(const char *pathname) {
	return bk_vfs_unlink(pathname);
}

int stat(const char *pathname, struct stat *statbuf) {
	return bk_vfs_stat(pathname, statbuf);
}

int fstat(int fd, struct stat *statbuf) {
	return bk_vfs_fstat(fd, statbuf);
}

int fsync(int fd) {
	return bk_vfs_fsync(fd);
}

int rename(const char *oldpath, const char *newpath) {
	return bk_vfs_rename(oldpath, newpath);
}


int statfs(const char *path, struct statfs *buf) {
	return bk_vfs_statfs(path, buf);
}

int ftruncate(int fd, off_t length) {
	return bk_vfs_ftruncate(fd, length);
}

int access(const char *pathname, int mode) {
	struct stat stat_buf;

	if (stat(pathname, &stat_buf) < 0)
		return -1;

	//check stat_buf with mode

	return 0;
}


int fcntl(int fd, int cmd, ...) {
	void *arg;
	va_list ap;

	va_start(ap, cmd);
	arg = va_arg(ap, void *);
	va_end(ap);

	return bk_vfs_fcntl(fd, cmd, arg);
}

int ioctl(int fd, unsigned long request, ...) {
	return -1;	// TODO
}


int mkdir(const char *pathname, mode_t mode) {
	return bk_vfs_mkdir(pathname, mode);
}

int rmdir(const char *pathname) {
	return bk_vfs_rmdir(pathname);
}


DIR *opendir(const char *name) {
	return bk_vfs_opendir(name);
}

int closedir(DIR *dirp) {
	return bk_vfs_closedir(dirp);
}

struct dirent *readdir(DIR *dirp) {
	return bk_vfs_readdir(dirp);
}

void seekdir(DIR *dirp, long loc) {
	bk_vfs_seekdir(dirp, loc);
}

long telldir(DIR *dirp) {
	return bk_vfs_telldir(dirp);
}

void rewinddir(DIR *dirp) {
	bk_vfs_rewinddir(dirp);
}


int chdir(const char *path) {
	return bk_vfs_chdir(path);
}

char *getcwd(char *buf, size_t size) {
	return bk_vfs_getcwd(buf, size);
}

