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

extern void reset_vector(void);

/* This must be a leaf function, no child function */
void __platform_init (void) __attribute__((naked));
void __platform_init(void)
{
	/* Do your platform low-level initial */

	__asm("ret");
}

void c_startup(void)
{
#define MEMCPY(des, src, n)     __builtin_memcpy ((des), (src), (n))
#define MEMSET(s, c, n)         __builtin_memset ((s), (c), (n))
	/* Data section initialization */
	extern char  _edata, _end;
	unsigned int size;

#ifdef CFG_XIP
	extern char _data_lmastart, _data_start;
	extern char _itcm_lma_start, _itcm_ema_start, _itcm_lma_end;
	extern char _dtcm_lma_start, _dtcm_ema_start, _dtcm_lma_end;
	extern char _dtcm_bss_start, _dtcm_bss_end;

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
#endif

	/* Copy data section from LMA to VMA */
	size = &_edata - &_data_start;
	MEMCPY(&_data_start, &_data_lmastart, size);

	/* Clear bss section */
	size = &_end - &_edata;
	MEMSET(&_edata, 0, size);
}

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

void arch_init(void)
{
	//arch_enable_align_fault();
}
