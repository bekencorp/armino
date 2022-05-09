#include <common/bk_include.h>
#include "bk_arm_arch.h"
#include <string.h>
#include <rtthread.h>

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

void *os_malloc(size_t size)
{
	return (void *)rt_malloc(size);
}

void *os_zalloc(size_t size)
{
	void *n = (void *)rt_malloc(size);
	if (n)
		os_memset(n, 0, size);
	return n;
}

void *os_realloc(void *ptr, size_t size)
{
	void *tmp;

	tmp = (void *)rt_malloc(size);
	if (tmp) {
		os_memcpy(tmp, ptr, size);
		rt_free(ptr);
	}

	return tmp;
}

void os_free(void *ptr)
{
	if (ptr)
		rt_free(ptr);
}

int os_memcmp_const(const void *a, const void *b, size_t len)
{
	return memcmp(a, b, len);
}

void* os_malloc_wifi_buffer(size_t size)
{
#if (CONFIG_SOC_BK7251 || CONFIG_SOC_BK7271)
        return (void*)dtcm_malloc(size);
#else
        return (void*)os_malloc(size);
#endif
}

void os_show_memory_config_info(void)
{
}
