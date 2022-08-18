#ifndef _HEAP_FOUR_H_
#define _HEAP_FOUR_H_

#include <common/sys_config.h>

#define PRIVILEGED_FUNCTION __attribute__((section("privileged_functions")))

#if CONFIG_MALLOC_STATIS || CONFIG_MEM_DEBUG
void *pvPortMalloc_cm(const char *call_func_name, int line, size_t xWantedSize, int need_zero) PRIVILEGED_FUNCTION;
void *vPortFree_cm(const char *call_func_name, int line, void *pv ) PRIVILEGED_FUNCTION;
#define pvPortMalloc(size)    pvPortMalloc_cm((const char*)__FUNCTION__,__LINE__,size, 1)
#define vPortFree(p)       vPortFree_cm((const char*)__FUNCTION__,__LINE__,p)
#else
void *pvPortMalloc( size_t xWantedSize ) PRIVILEGED_FUNCTION;
void vPortFree( void *pv ) PRIVILEGED_FUNCTION;
#endif

void *pvPortRealloc( void *pv, size_t xWantedSize );

//void vPortFree( void *pv );
//void *pvPortRealloc( void *pv, size_t xWantedSize );
size_t xPortGetFreeHeapSize( void );
size_t xPortGetMinimumEverFreeHeapSize( void );

#endif
// eof

