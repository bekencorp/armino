#include <common/bk_include.h>
#include "arm_arch.h"
#include <string.h>

#include "sys_rtos.h"
#include "uart_pub.h"
#include <os/mem.h>
#include <os/os.h>
#include "rtos_init.h"

//TODO remove it
void *beken_malloc( size_t xWantedSize );
void beken_free( void *pv );
void *beken_realloc( void *pv, size_t xWantedSize );

INT32 os_memcmp(const void *s1, const void *s2, UINT32 n)
{
    return memcmp(s1, s2, (unsigned int)n);
}

void *os_memmove(void *out, const void *in, UINT32 n)
{
    return memmove(out, in, n);
}

void *os_memcpy(void *out, const void *in, UINT32 n)
{
    return memcpy(out, in, n);
}

void *os_memset(void *b, int c, UINT32 len)
{
    return (void *)memset(b, c, (unsigned int)len);
}

int os_memcmp_const(const void *a, const void *b, size_t len)
{
    return memcmp(a, b, len);
}

void *os_realloc(void *ptr, size_t size)
{
	return beken_realloc(ptr, size);
}

#if (CONFIG_MALLOC_STATIS || CONFIG_MEM_DEBUG)

void *os_malloc_debug(const char *func_name, int line, size_t size, int need_zero)
{
	if (0 == need_zero) {
		return (void *)beken_malloc(size);
	} else {
		void *p = (void *)beken_malloc(size);
		if (NULL != p) {
			(void *)memset(p, 0, size);
		}
		return p;
	}
}

#else // #if (CONFIG_MALLOC_STATIS || CONFIG_MEM_DEBUG)

void *os_malloc(size_t size)
{
    return (void *)beken_malloc(size);
}

void * os_zalloc(size_t size)
{
	void *n = (void *)os_malloc(size);

	if (n)
		os_memset(n, 0, size);
	return n;
}

void *os_malloc_debug(const char *func_name, int line, size_t size, int need_zero)
{
	if (need_zero) {
		return (void *)os_zalloc(size);
	}
	return (void *)os_malloc(size);
}

#endif // #if (CONFIG_MALLOC_STATIS || CONFIG_MEM_DEBUG)



#if (CONFIG_MALLOC_STATIS || CONFIG_MEM_DEBUG)

void *os_free_debug(const char *func_name, int line, void *pv)
{
	beken_free(pv);
	return NULL;
}

#else // #if (CONFIG_MALLOC_STATIS || CONFIG_MEM_DEBUG)

void os_free(void *ptr)
{
	beken_free(ptr);
}

void *os_free_debug(const char *func_name, int line, void *pv)
{
	os_free(pv);
	return NULL;
}

#endif // #if (CONFIG_MALLOC_STATIS || CONFIG_MEM_DEBUG)

void* os_malloc_wifi_buffer(size_t size)
{
	return (void*)beken_malloc(size);
}

void os_show_memory_config_info(void)
{
}

#if (CONFIG_MALLOC_STATIS || CONFIG_MEM_DEBUG)
void *psram_malloc_debug(const char *func_name, int line, size_t size, int need_zero)
{
	return NULL;
}
#endif