// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "bk_arch.h"
#include "platform.h"
#include "boot.h"
#include "cache.h"
#include "aon_pmu_driver.h"
#include "sys_hal.h"

extern void reset_vector(void);
extern unsigned int g_sram_addr_map[SRAM_BLOCK_COUNT];

#if CONFIG_RESERVE_1K_SRAM_UNINITIAL
#define UNINITIALIZED_SRAM_SIZE         (0x400) // 1k
static bk_err_t arch_mem_power_ctrl(pm_power_module_state_e power_state)
{
	sys_hal_module_power_ctrl(POWER_MODULE_NAME_MEM3,power_state);
	return BK_OK;
}
uint32_t bk_arch_uninitialized_1k_sram_base_addr_get()
{
	return g_sram_addr_map[SRAM_BLOCK_MEM3]+SRAM_BLOCK_SIZE-UNINITIALIZED_SRAM_SIZE;
}
#endif

/* This must be a leaf function, no child function */
void __platform_init (void) __attribute__((naked));
void __platform_init(void)
{
	/* Do your platform low-level initial */

	__asm("ret");
}

void smem_reset_lastblock(void)
{
#define MEMSET(s, c, n)         __builtin_memset ((s), (c), (n))
	
#if CONFIG_RESERVE_1K_SRAM_UNINITIAL
	MEMSET((void *)g_sram_addr_map[SRAM_BLOCK_MEM3], 0x0, SRAM_BLOCK_SIZE-UNINITIALIZED_SRAM_SIZE);
#else
	MEMSET((void *)g_sram_addr_map[SRAM_BLOCK_MEM3], 0x0, SRAM_BLOCK_SIZE);
#endif
}

void stack_mem_dump(uint32_t stack_top, uint32_t stack_bottom);
void smem_dump_lastblock(void)
{
	unsigned int start_addr =  g_sram_addr_map[SRAM_BLOCK_MEM3];
	unsigned int end_addr = start_addr + SRAM_BLOCK_SIZE;
	
	stack_mem_dump(start_addr, end_addr );	
}

void c_startup(void)
{
#define MEMCPY(des, src, n)     __builtin_memcpy ((des), (src), (n))
#define MEMSET(s, c, n)         __builtin_memset ((s), (c), (n))

#if !CONFIG_SLAVE_CORE
	#if CONFIG_RESERVE_1K_SRAM_UNINITIAL
	arch_mem_power_ctrl(PM_POWER_MODULE_STATE_OFF);
	arch_mem_power_ctrl(PM_POWER_MODULE_STATE_ON);
	#endif
	/* Init last sram block for BT/wifi TX-buffer/wifi Rx-buffer/etc. */
	smem_reset_lastblock();
#endif

	extern char _data_lmastart, _data_start;
	extern char _itcm_lma_start, _itcm_ema_start, _itcm_lma_end;
	extern char _dtcm_lma_start, _dtcm_ema_start, _dtcm_lma_end;
	extern char _dtcm_bss_start, _dtcm_bss_end;
	extern char  _edata, _end;
	unsigned int size;

	/*Copy ITCM section from LMA to VMA*/
	size = &_itcm_lma_end - &_itcm_lma_start;
	if(size!=0)
	{
		MEMCPY(&_itcm_ema_start, &_itcm_lma_start, size);
	}

	/*Copy DTCM section from LMA to VMA*/
	size = &_dtcm_lma_end - &_dtcm_lma_start;
	if(size!=0)
	{
		MEMCPY(&_dtcm_ema_start, &_dtcm_lma_start, size);
	}

	/* Clear DTCM bss section */
	size = &_dtcm_bss_end - &_dtcm_bss_start;
	if(size!=0)
	{
		MEMSET(&_dtcm_bss_start, 0, size);
	}

#if CONFIG_CACHE_ENABLE && (!CONFIG_SLAVE_CORE)
	// copy cacheable data.
#endif

	/* Copy data section from LMA to VMA */
	size = &_edata - &_data_start;
	MEMCPY(&_data_start, &_data_lmastart, size);

	/* Clear bss section */
	size = &_end - &_edata;
	MEMSET(&_edata, 0, size);

#if CONFIG_SAVE_BOOT_TIME_POINT
	save_mtime_point(CPU_INIT_MEM_TIME);
#endif
}

#if 0
void system_init(void)
{
	/*
	 * Do your system reset handling here
	 */
	/* Reset the CPU reset vector for this program. */
	MCUIP_SMU->RESET_VECTOR = (unsigned int)(long)reset_vector;

	/* Enable PLIC features */
	if (read_csr(NDS_MMISC_CTL) & (1 << 1)) {
		/* External PLIC interrupt is vectored */
		__nds__plic_set_feature(NDS_PLIC_FEATURE_PREEMPT | NDS_PLIC_FEATURE_VECTORED);
	} else {
		/* External PLIC interrupt is NOT vectored */
		__nds__plic_set_feature(NDS_PLIC_FEATURE_PREEMPT);
	}

	/* Enable misaligned access and non-blocking load. */
	set_csr(NDS_MMISC_CTL, (1 << 8) | (1 << 6));
}
#endif

void arch_init(void)
{
	//arch_enable_align_fault();
}

void arch_init_vector(uint32_t trap_vect)
{
	write_csr(NDS_UTVEC, trap_vect);
}

