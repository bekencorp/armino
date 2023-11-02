#include <common/bk_include.h>
#include "bk_arm_arch.h"
#include <string.h>

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include <os/os.h>
#include "bk_uart.h"
#include <os/mem.h>

INT32 os_memcmp(const void *s1, const void *s2, UINT32 n)
{
	return memcmp(s1, s2, (unsigned int)n);
}

void *os_memmove(void *out, const void *in, UINT32 n)
{
	configASSERT(NULL != in);
	return memmove(out, in, n);
}

void *os_memcpy(void *out, const void *in, UINT32 n)
{
	configASSERT(NULL != in);
	return memcpy(out, in, n);
}

void *os_memset(void *b, int c, UINT32 len)
{
	return (void *)memset(b, c, (unsigned int)len);
}

void *os_realloc(void *ptr, size_t size)
{
#ifdef FIX_REALLOC_ISSUE
	return pvPortRealloc(ptr, size);
#else
	void *tmp;

	if (platform_is_in_interrupt_context())
		os_printf("realloc_risk\r\n");

	tmp = (void *)pvPortMalloc(size);
	if (tmp && ptr) {
		os_memcpy(tmp, ptr, size);
		vPortFree(ptr);
	}

	return tmp;
#endif
}

void *bk_psram_realloc(void *ptr, size_t size)
{
	void *tmp;

	if (platform_is_in_interrupt_context())
		os_printf("psram_realloc_risk\r\n");

	tmp = psram_malloc(size);
	if (tmp && ptr) {
		if (size & 0x3) {
			os_memcpy_word((uint32_t *)tmp, (uint32_t *)ptr, ((size >> 2) + 1) << 2);
		} else {
			os_memcpy_word((uint32_t *)tmp, (uint32_t *)ptr, size);
		}
		os_free((void *)ptr);
	}

	return tmp;
}

int os_memcmp_const(const void *a, const void *b, size_t len)
{
	return memcmp(a, b, len);
}

#if !CONFIG_MALLOC_STATIS && !CONFIG_MEM_DEBUG
void *os_malloc(size_t size)
{
#if !CONFIG_FULLY_HOSTED
	if (platform_is_in_interrupt_context())
		os_printf("malloc_risk\r\n");
#endif

#if (CONFIG_SOC_BK7251)
	void *ptr = psram_malloc(size);
	if (ptr)
		return ptr;
#endif

	return (void *)pvPortMalloc(size);
}

void *os_zalloc(size_t size)
{
	void *n = (void *)os_malloc(size);

	if (n)
		os_memset(n, 0, size);
	return n;
}

void os_free(void *ptr)
{
#if !CONFIG_FULLY_HOSTED
	if (platform_is_in_interrupt_context())
		os_printf("free_risk\r\n");
#endif

	if (ptr)
		vPortFree(ptr);
}

void *os_malloc_debug(const char *func_name, int line, size_t size, int need_zero)
{
	if (need_zero) {
		return (void *)os_zalloc(size);
	}
	return (void *)os_malloc(size);
}


void *os_free_debug(const char *func_name, int line, void *pv)
{
	os_free(pv);
	return NULL;
}

#else

extern void xPortDumpMemStats(uint32_t start_tick, uint32_t ticks_since_malloc, const char* task);
extern void *psram_malloc_cm(const char *func_name, int line, size_t size, int need_zero);

void *os_malloc_debug(const char *func_name, int line, size_t size, int need_zero)
{
	if (platform_is_in_interrupt_context()) {
		BK_DUMP_OUT("Error: [%s] line(%d). malloc_risk.\r\n", func_name, line);
	}
	return pvPortMalloc_cm(func_name, line, size, need_zero);
}

void *psram_malloc_debug(const char *func_name, int line, size_t size, int need_zero)
{
	return psram_malloc_cm(func_name, line, size, need_zero);
}

void *os_free_debug(const char *func_name, int line, void *pv)
{
	if (platform_is_in_interrupt_context()) {
		BK_DUMP_OUT("Error: [%s] line(%d). free_risk.\r\n", func_name, line);
	}
	return vPortFree_cm(func_name, line, pv);
}

/****************************************************
*    Build CONFIG_MEM_DEBUG version                 *
*    Adapt third lib build with release SDK begin   *
*****************************************************/
#undef os_malloc
#undef os_free
#undef os_zalloc

void *os_malloc(size_t size)
{
	return (void *)os_malloc_debug((const char*)__FUNCTION__,__LINE__,size, 0);
}

void *os_zalloc(size_t size)
{
	return (void *)os_malloc_debug((const char*)__FUNCTION__,__LINE__,size, 1);
}

void os_free(void *ptr)
{
        os_free_debug((const char*)__FUNCTION__,__LINE__, ptr);
}

#undef psram_malloc
#undef psram_zalloc

void *psram_malloc(size_t size)
{
    return psram_malloc_debug((const char*)__FUNCTION__,__LINE__,size, 0);
}

void *psram_zalloc(size_t size)
{
    return psram_malloc_debug((const char*)__FUNCTION__,__LINE__,size, 1);
}

/****************************************************
*    Build CONFIG_MEM_DEBUG version                 *
*    Adapt third lib build with release SDK end     *
*****************************************************/

void os_dump_memory_stats(uint32_t start_tick, uint32_t ticks_since_malloc, const char* task)
{
	xPortDumpMemStats(start_tick, ticks_since_malloc, task);
}

#endif

void* os_malloc_wifi_buffer(size_t size)
{
#if (CONFIG_RTT) && (CONFIG_SOC_BK7251 || CONFIG_SOC_BK7271)
        return (void*)dtcm_malloc(size);
#elif (CONFIG_FREERTOS) && (CONFIG_SOC_BK7251)
        return (void*)pvPortMalloc(size);
#else
        return (void*)os_malloc(size);
#endif
}

extern void pvShowMemoryConfigInfo(void);
void os_show_memory_config_info(void)
{
	pvShowMemoryConfigInfo();
}
