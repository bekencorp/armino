#ifndef __BK_STD_HEADER_H_
#define __BK_STD_HEADER_H_

#ifndef O_WRONLY
#define O_WRONLY 1
#endif

#ifndef O_DIRECTORY
#define O_DIRECTORY 0x200000
#endif

#ifndef STATFS
#define STATFS

struct statfs {
	unsigned long f_bsize;
	unsigned long f_blocks;
	unsigned long f_bfree;
	unsigned long f_bavail;
};

#endif

#define MAX_PATH_LEN 128

#ifndef DIRENT
#define DIRENT

#define DT_UNKNOWN 0
#define DT_REG 8
#define DT_DIR 4

struct dirent {
	uint8_t d_ino;
	uint8_t d_type;
	uint16_t d_reclen;
	char d_name[MAX_PATH_LEN];
};

#endif

#endif
