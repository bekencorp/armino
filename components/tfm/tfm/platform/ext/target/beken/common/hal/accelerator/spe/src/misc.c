#include "psa_sp_api.h"
#include "psa_service.h"

void *psa_malloc(size_t size)
{
}

void *psa_realloc(void *buffer, size_t size)
{
}

void *psa_calloc(size_t nmemb, size_t size)
{
}

void psa_free(void *buffer)
{
}

void *psa_memmove(void *dst, const void *src, size_t size)
{
}

void *psa_memset(void *buffer, int c, size_t size)
{
}

int32_t psa_memcmp(const void *buffer1, const void *buffer2, size_t size)
{
}

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
}

uint32_t psa_wait(uint32_t signal_mask, uint32_t timeout)
{
}

void psa_notify(int32_t partition_id)
{
}

void psa_clear(void)
{
}

void psa_eoi(uint32_t irq_signal)
{
}

unsigned int hal_irq_disable(void)
{
}

void hal_irq_enable(unsigned int key)
{
}


