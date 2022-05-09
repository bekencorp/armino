#ifndef __AVI_ADP_H__
#define __AVI_ADP_H__

#include "dfs_posix.h"

#define AVILIB_MALLOC               sdram_malloc   // sdram_malloc         os_malloc       
#define AVILIB_REALLOC              sdram_realloc  // sdram_realloc        os_realloc
#define AVILIB_FREE                 sdram_free     // sdram_free           sdram_free
#define AVILIB_MEMSET
#define AVILIB_MEMCPY

#endif // __AVI_ADP_H__
