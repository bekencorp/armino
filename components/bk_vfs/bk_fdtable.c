#include "os/os.h"
#include "bk_fdtable.h"
#include "bk_filesystem.h"

#define MAX_BK_FILE_NUMBER CONFIG_VFS_MAX_FD_NUM

static struct bk_file files[MAX_BK_FILE_NUMBER];

int bk_fdtable_init(void) {

	int i;
	
	for (i = 0; i < MAX_BK_FILE_NUMBER; i++) {
		files[i].magic = 0;
	}

	files[0].magic = BK_FILE_MAGIC;   //modify xhh, let fd don't return 0

	return 0;
}

struct bk_file * bk_file_get(void) {
	int i;

	for (i = 0; i < MAX_BK_FILE_NUMBER; i++) {
		if (files[i].magic != BK_FILE_MAGIC) {
			files[i].magic = BK_FILE_MAGIC;
			return &files[i];
		}
	}

	return NULL;
}

void bk_file_put(struct bk_file *file) {
	file->magic = 0;
	file->filesystem = NULL;
	file->f_ops = NULL;
	file->f_data = NULL;
}

struct bk_file *bk_fd_to_file(int fd) {
	if (fd < 0 || fd > MAX_BK_FILE_NUMBER)
		return NULL;

	if (files[fd].magic != BK_FILE_MAGIC)
		return NULL;

	return &files[fd];	
}

int bk_file_to_fd(struct bk_file *file) {
	if (!file)
		return -1;

	if (file < files || file > files + MAX_BK_FILE_NUMBER)
		return -1;

	return file - files;
}



