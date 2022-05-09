#ifndef _MEM_PUB_H_
#define _MEM_PUB_H_

#include <stdarg.h>
#include <common/bk_typedef.h>
#include <common/sys_config.h>
#ifdef __cplusplus
extern"C" {
#endif

INT32 os_memcmp(const void *s1, const void *s2, UINT32 n);
void *os_memmove(void *out, const void *in, UINT32 n);
void *os_memcpy(void *out, const void *in, UINT32 n);
void *os_memset(void *b, int c, UINT32 len);
void os_mem_init(void);
void *os_realloc(void *ptr, size_t size);
int os_memcmp_const(const void *a, const void *b, size_t len);

#if (CONFIG_MALLOC_STATIS || CONFIG_MEM_DEBUG)
void *os_malloc_debug(const char *func_name, int line, size_t size, int need_zero);
void *psram_malloc_debug(const char *func_name, int line, size_t size, int need_zero);
void *os_free_debug(const char *func_name, int line, void *pv);
void os_dump_memory_stats(uint32_t start_tick, uint32_t ticks_since_malloc, const char* task);

#define os_malloc(size)   os_malloc_debug((const char*)__FUNCTION__,__LINE__,size, 0)
#define os_free(p)        os_free_debug((const char*)__FUNCTION__,__LINE__, p)
#define os_zalloc(size)   os_malloc_debug((const char*)__FUNCTION__,__LINE__,size, 1)
#define psram_malloc(size)   psram_malloc_debug((const char*)__FUNCTION__,__LINE__,size, 0)
#define psram_zalloc(size)   psram_malloc_debug((const char*)__FUNCTION__,__LINE__,size, 1)
#else
void *os_malloc(size_t size);
void os_free(void *ptr);
void *os_zalloc(size_t size);
void *psram_malloc(size_t size);
void *psram_zalloc(size_t size);
#endif
//#define psram_free        os_free

void* os_malloc_wifi_buffer(size_t size);
void os_show_memory_config_info(void);
#ifdef __cplusplus
}
#endif

#endif // _MEM_PUB_H_

// EOF
