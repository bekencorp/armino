#ifndef _BK_EASY_FLASH_H_
#define _BK_EASY_FLASH_H_

#include "easyflash.h"

#if CONFIG_EASY_FLASH
EfErrCode bk_save_env(void);
char *bk_get_env(const char *key);
EfErrCode bk_set_env(const char *key, const char *value);
EfErrCode bk_set_buf_env(const char *key, const char *buf, int len);
EfErrCode bk_get_buf_env(const char *key, const char *buf, int len);
#endif // CONFIG_EASY_FLASH

#endif // _BK_EASY_FLASH_H_
// eof

