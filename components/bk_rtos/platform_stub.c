/**
 ******************************************************************************
 * @file    platform_init.c
 * @version V1.0.0
 * @date    05-May-2014
 * @brief   This file provide functions called by _BK_ to drive stm32f2xx
 *          platform: - e.g. power save, reboot, platform initialize
 ******************************************************************************
 *  UNPUBLISHED PROPRIETARY SOURCE CODE
 *  Copyright (c) 2016 BEKEN Inc.
 *
 *  The contents of this file may not be disclosed to third parties, copied or
 *  duplicated in any form, in whole or in part, without the prior written
 *  permission of BEKEN Corporation.
 ******************************************************************************
 */
#include <common/bk_include.h> 
#include <stdio.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/unistd.h>


#include <os/mem.h>

#include <os/os.h>
#include "common/bk_assert.h"


/************** wrap C library functions **************/
void *__wrap_malloc(size_t size)
{
	return os_malloc(size);
}

void *__wrap__malloc_r(void *p, size_t size)
{
	return os_malloc(size);
}

void __wrap_free(void *pv)
{
	os_free(pv);
}

void *__wrap_calloc(size_t a, size_t b)
{
	void *pvReturn;

	pvReturn = os_malloc(a * b);
	if (pvReturn)
    {
        os_memset(pvReturn, 0, a*b);
    }

	return pvReturn;
}

void *__wrap_realloc(void *pv, size_t size)
{
	return os_realloc(pv, size);
}

void __wrap__free_r(void *p, void *x)
{
	__wrap_free(x);
}

void *__wrap__realloc_r(void *p, void *x, size_t sz)
{
	return __wrap_realloc(x, sz);
}

void *__wrap_zalloc(size_t size)
{
	return os_zalloc(size);
}

int __wrap_strlen (char *src)
{
    int ret = 0;

    for (ret = 0; *src; src++)
        ret++;

    return ret;
}

int __wrap_strncmp(const char *s1, const char *s2, size_t n)
{
    BK_ASSERT(s1 && s2);

    if(0 == n) return 0;

    while(--n && *s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }

    return *s1 - *s2;
}

int __wrap_printf(const char *fmt, ...)
{
	char string[128] = {0};
	va_list ap;
	int len;

	va_start(ap, fmt);
	len = vsnprintf(string, sizeof(string) - 1, fmt, ap);
	va_end(ap);

	string[127] = 0;

	os_printf(string);
    return len;
}


void __assert_func(const char *file, int line, const char *func, const char *failedexpr)
{
	os_printf("%s %d func %s expr %s\n", file, line, func, failedexpr);
	BK_ASSERT(0);
}
// eof

