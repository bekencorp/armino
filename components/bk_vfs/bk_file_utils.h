#ifndef __BK_FILE_UTILS_H_
#define __BK_FILE_UTILS_H_

int bk_vfs_init(void);

int bk_vfs_lock(void);
void bk_vfs_unlock(void);

char *bk_normalize_path(const char *path);
const char *bk_sub_path(const char *mount_point, const char *path);

#endif
