#include "psa_sp_api.h"
#include "psa_service.h"

//TODO peter

void *psa_malloc(size_t size)
{
	void *tmp = mbedtls_calloc(1, size);
	if (!tmp) printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ malloc failed\r\n");
	return tmp;
}

#if 0
void *psa_realloc(void *buffer, size_t size)
{
}

void *psa_calloc(size_t nmemb, size_t size)
{
}
#endif

void psa_free(void *buffer)
{
	mbedtls_free(buffer);
}

void *psa_memmove(void *dst, const void *src, size_t size)
{
	printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ %s %d\r\n", __FUNCTION__, __LINE__);
}

#if 0
void *psa_memset(void *buffer, int c, size_t size)
{
}

int32_t psa_memcmp(const void *buffer1, const void *buffer2, size_t size)
{
}
#endif

int psa_vprintf(const char *format, va_list vargs)
{
}

void psa_udelay(uint32_t us)
{
}

void psa_mdelay(uint32_t msecs)
{
}
psa_status_t psa_msleep(uint32_t msecs)
{
}

ulong_t psa_get_tick(void)
{
	printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ %s %d\r\n", __FUNCTION__, __LINE__);
}

uint32_t psa_wait(uint32_t signal_mask, uint32_t timeout)
{
	printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ %s %d\r\n", __FUNCTION__, __LINE__);
}

void psa_notify(int32_t partition_id)
{
	printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ %s %d\r\n", __FUNCTION__, __LINE__);
}

void psa_clear(void)
{
	printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ %s %d\r\n", __FUNCTION__, __LINE__);
}

void psa_eoi(uint32_t irq_signal)
{
	printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ %s %d\r\n", __FUNCTION__, __LINE__);
}

unsigned int hal_irq_disable(void)
{
	printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ %s %d\r\n", __FUNCTION__, __LINE__);
}

void hal_irq_enable(unsigned int key)
{
	printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ %s %d\r\n", __FUNCTION__, __LINE__);
}


