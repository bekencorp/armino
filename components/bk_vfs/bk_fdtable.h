#ifndef __BK_FDTABLE_H_
#define __BK_FDTABLE_H_

struct bk_filesystem;
struct bk_file;

int bk_fdtable_init(void);

struct bk_file *bk_file_get(void);
void bk_file_put(struct bk_file *file);

struct bk_file *bk_fd_to_file(int fd);
int bk_file_to_fd(struct bk_file *file);

#endif
