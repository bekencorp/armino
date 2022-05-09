#ifndef _PARAM_MANAGE_H_
#define  _PARAM_MANAGE_H_
#include <common/sys_config.h>
#include <stdint.h>

#define FLAG_KEY_1             "WANGZHILEI"
#define TEST_KEY_2             "a"
#define TEST_KEY_3             "zhoujun long"
#define TEST_KEY_4             "adlkfajsdkfasdoikhkjn"
#define TEST_KEY_5             "xubin "
#define TEST_KEY_6             "liudehua"
#define TEST_KEY_7             "jiegou"
#define TEST_KEY_8             "wanghuixin"
#define TEST_KEY_9             "wangzihao"
#define TEST_KEY_10            "structure"

#define TEST_MAGIC             (0x54534243)

typedef struct _test_structure_
{
	char a;
	unsigned char b;
	short c;

	int magic;
	uint32_t resv;
}T_STRUCT_T;

#if EASY_FLASH_DEMO
#endif

#endif // _PARAM_MANAGE_H_
// eof
