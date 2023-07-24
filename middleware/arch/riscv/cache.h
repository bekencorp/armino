#ifndef __CACHE_H__
#define __CACHE_H__


#define SRAM_BLOCK_SIZE                         (0x20000)

typedef enum {
	SRAM_BLOCK_MEM0 = 0,
	SRAM_BLOCK_MEM1 = 1,
	SRAM_BLOCK_MEM2 = 2,
	SRAM_BLOCK_MEM3 = 3,
	SRAM_BLOCK_COUNT  = 4,
}sram_bock_t;

void sram_dcache_map(void);

int show_cache_config_info(void);

void flush_dcache(void *va, long size);

void flush_all_dcache(void);

void enable_dcache(int enable);

#endif //__CACHE_H__