#ifndef __CACHE_H__
#define __CACHE_H__

#define SRAM_BLOCK_COUNT                        (4)
#define SRAM_BLOCK_SIZE                         (0x20000)

void sram_dcache_map(void);

int show_cache_config_info(void);

void flush_dcache(void *va, long size);

void flush_all_dcache(void);

#endif //__CACHE_H__