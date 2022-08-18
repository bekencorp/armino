#include <common/bk_include.h>
#include "bk_arm_arch.h"
#include <string.h>
#include <os/os.h>
#include "bk_uart.h"
#include <os/mem.h>

__weak INT32 os_memcmp(const void *s1, const void *s2, UINT32 n)
{
	return memcmp(s1, s2, (unsigned int)n);
}

__weak void *os_memmove(void *out, const void *in, UINT32 n)
{
	return memmove(out, in, n);
}

__weak void *os_memcpy(void *out, const void *in, UINT32 n)
{
	return memcpy(out, in, n);
}

__weak void *os_memset(void *b, int c, UINT32 len)
{
	return (void *)memset(b, c, (unsigned int)len);
}

__weak void *os_realloc(void *ptr, size_t size)
{
	return realloc(ptr, size);
}

__weak int os_memcmp_const(const void *a, const void *b, size_t len)
{
	return memcmp(a, b, len);
}

__weak void *os_malloc(size_t size)
{
	return malloc(size);
}

__weak void *os_zalloc(size_t size)
{
	return zalloc(size);
}

__weak void os_free(void *ptr)
{
	free(ptr);
}
