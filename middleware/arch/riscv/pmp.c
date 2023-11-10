/*
 * Copyright (c) 2012-2021 Andes Technology Corporation
 * All rights reserved.
 */

#if 0

#include <stdio.h>
#include "platform.h"
#include "pmp.h"
#include <os/os.h>


#define SYS_DELAY_TIME_10US	    (170UL)

/* bk7256   */
#define MEM_MAPPING_MAX               0xFFFFFFF0
#define ADDR_MAPPING_MAX              0x80000000
#define DTCM_END                      0x20008000
#define SRAM_END                      0x38080000
#define FLASH_END                     0x00400000

/* PMP entry 0 : 0 ~ (_start), lock, TOR range, excutable, no writable, readable */
extern char _start;

/* PMP entry 1(code/RO data section) :  (_start) ~ (_data_lmastart), lock, TOR range, excutable, no writable, readable */
extern char _data_lmastart;

/* PMP entry 2(FLASH) : (_data_lmastart) ~ (FLASH_END), lock, TOR range, no excutable, writable, readable */

/* PMP entry 3(null section) : (FLASH_END) ~ (_itcm_ema_start), lock, TOR range, no excutable, no writable, no readable */
extern char _itcm_ema_start;

/* PMP entry 4(itcm section) : (_itcm_ema_start) ~ (_dtcm_ema_start), lock, TOR range, excutable, no writable, readable */
extern char _dtcm_ema_start;

/* PMP entry 5(dtcm/sram section) : (_dtcm_ema_start) ~ (SRAM_END), lock, TOR range, no excutable,  writable, readable */

/* PMP entry 6(max address section) : (SRAM_END) ~ (ADDR_MAPPING_MAX), lock, TOR range, no excutable,  writable, readable */



#if (!CONFIG_SLAVE_CORE)
const pmp_config_t pmp_tor_config_table[] = {\
	{ENTRY_PMPADDR0, PMPCFG_ALXWR(PMP_A_TOR, PMP_L_ON, PMP_X_ON, PMP_W_OFF, PMP_R_ON),  (void*)(&_start + 4)}, \
	{ENTRY_PMPADDR1, PMPCFG_ALXWR(PMP_A_TOR, PMP_L_ON, PMP_X_ON, PMP_W_OFF, PMP_R_ON),  (void*)(&_data_lmastart + 4) }, \
	{ENTRY_PMPADDR2, PMPCFG_ALXWR(PMP_A_TOR, PMP_L_ON, PMP_X_OFF, PMP_W_ON, PMP_R_ON),  (void*)(FLASH_END + 4) }, \
	{ENTRY_PMPADDR3, PMPCFG_ALXWR(PMP_A_TOR, PMP_L_ON, PMP_X_OFF, PMP_W_OFF, PMP_R_OFF),  (void*)(&_itcm_ema_start + 4) }, \
	{ENTRY_PMPADDR4, PMPCFG_ALXWR(PMP_A_TOR, PMP_L_ON, PMP_X_ON, PMP_W_OFF, PMP_R_ON),  (void*)(&_dtcm_ema_start + 4) }, \
	{ENTRY_PMPADDR5, PMPCFG_ALXWR(PMP_A_TOR, PMP_L_ON, PMP_X_OFF, PMP_W_ON, PMP_R_ON),  (void*)(SRAM_END +4) }, \
	{ENTRY_PMPADDR6, PMPCFG_ALXWR(PMP_A_TOR, PMP_L_ON, PMP_X_OFF, PMP_W_ON, PMP_R_ON),   (void*)(ADDR_MAPPING_MAX+4) } \
};
#else
const pmp_config_t pmp_tor_config_table[] = {\
};
#endif //#if (!CONFIG_SLAVE_CORE)

extern void sys_delay_sync(uint32_t time_count );

void pmp_tor_config(char entry, void* address, char pmp_cfg)
{
	long pmpcfg = pmp_cfg & 0xFF;

	void * va = (void *)((((uint32)(address)+3)/4) * 4);
	
	os_printf("pmp_tor_config[%d]: addr(0x%08x),TOR(0x%08x) cfg(0x%02x).\n", entry, va, TOR(va), pmpcfg);
	switch (entry) {
	case ENTRY_PMPADDR0:
		write_csr(NDS_PMPADDR0, TOR(va));
		break;
	case ENTRY_PMPADDR1:
		write_csr(NDS_PMPADDR1, TOR(va));
		break;
	case ENTRY_PMPADDR2:
		write_csr(NDS_PMPADDR2, TOR(va));
		break;
	case ENTRY_PMPADDR3:
		write_csr(NDS_PMPADDR3, TOR(va));
		break;
	case ENTRY_PMPADDR4:
		write_csr(NDS_PMPADDR4, TOR(va));
		break;
	case ENTRY_PMPADDR5:
		write_csr(NDS_PMPADDR5, TOR(va));
		break;
	case ENTRY_PMPADDR6:
		write_csr(NDS_PMPADDR6, TOR(va));
		break;
	case ENTRY_PMPADDR7:
		write_csr(NDS_PMPADDR7, TOR(va));
		break;
	case ENTRY_PMPADDR8:
		write_csr(NDS_PMPADDR8, TOR(va));
		break;
	case ENTRY_PMPADDR9:
		write_csr(NDS_PMPADDR9, TOR(va));
		break;
	case ENTRY_PMPADDR10:
		write_csr(NDS_PMPADDR10, TOR(va));
		break;
	case ENTRY_PMPADDR11:
		write_csr(NDS_PMPADDR11, TOR(va));
		break;
	case ENTRY_PMPADDR12:
		write_csr(NDS_PMPADDR12, TOR(va));
		break;
	case ENTRY_PMPADDR13:
		write_csr(NDS_PMPADDR13, TOR(va));
		break;
	case ENTRY_PMPADDR14:
		write_csr(NDS_PMPADDR14, TOR(va));
		break;
	case ENTRY_PMPADDR15:
		write_csr(NDS_PMPADDR15, TOR(va));
		break;
	}

#if __riscv_xlen == 64
	switch (entry >> 3){
	case ENTRY_PMPCFG0:
		write_csr(NDS_PMPCFG0, ((read_csr(NDS_PMPCFG0) & (~(0xFFLL << ((long)(entry%8) << 3)))) | (((long)pmpcfg) << ((long)(entry%8) << 3))));
		break;
	case ENTRY_PMPCFG1:
		write_csr(NDS_PMPCFG2, ((read_csr(NDS_PMPCFG2) & (~(0xFFLL << ((long)(entry%8) << 3)))) | (((long)pmpcfg) << ((long)(entry%8) << 3))));
		break;
	}
#else
	switch (entry >> 2) {
	case ENTRY_PMPCFG0:
		write_csr(NDS_PMPCFG0, ((read_csr(NDS_PMPCFG0) & (~((0xFF) << ((entry%4) << 3)))) | (((long)pmpcfg) << ((entry%4) << 3))));
		break;
	case ENTRY_PMPCFG1:
		write_csr(NDS_PMPCFG1, ((read_csr(NDS_PMPCFG1) & (~((0xFF) << ((entry%4) << 3)))) | (((long)pmpcfg) << ((entry%4) << 3))));
		break;
	case ENTRY_PMPCFG2:
		write_csr(NDS_PMPCFG2, ((read_csr(NDS_PMPCFG2) & (~((0xFF) << ((entry%4) << 3)))) | (((long)pmpcfg) << ((entry%4) << 3))));
		break;
	case ENTRY_PMPCFG3:
		write_csr(NDS_PMPCFG3, ((read_csr(NDS_PMPCFG3) & (~((0xFF) << ((entry%4) << 3)))) | (((long)pmpcfg) << ((entry%4) << 3))));
		break;
	}
#endif
}

void pmp_napot_config(char entry, void* va, unsigned long size, char pmpcfg)
{
	switch (entry) {
	case ENTRY_PMPADDR0:
		write_csr(NDS_PMPADDR0, NAPOT(va, size));
		break;
	case ENTRY_PMPADDR1:
		write_csr(NDS_PMPADDR1, NAPOT(va, size));
		break;
	case ENTRY_PMPADDR2:
		write_csr(NDS_PMPADDR2, NAPOT(va, size));
		break;
	case ENTRY_PMPADDR3:
		write_csr(NDS_PMPADDR3, NAPOT(va, size));
		break;
	case ENTRY_PMPADDR4:
		write_csr(NDS_PMPADDR4, NAPOT(va, size));
		break;
	case ENTRY_PMPADDR5:
		write_csr(NDS_PMPADDR5, NAPOT(va, size));
		break;
	case ENTRY_PMPADDR6:
		write_csr(NDS_PMPADDR6, NAPOT(va, size));
		break;
	case ENTRY_PMPADDR7:
		write_csr(NDS_PMPADDR7, NAPOT(va, size));
		break;
	case ENTRY_PMPADDR8:
		write_csr(NDS_PMPADDR8, NAPOT(va, size));
		break;
	case ENTRY_PMPADDR9:
		write_csr(NDS_PMPADDR9, NAPOT(va, size));
		break;
	case ENTRY_PMPADDR10:
		write_csr(NDS_PMPADDR10, NAPOT(va, size));
		break;
	case ENTRY_PMPADDR11:
		write_csr(NDS_PMPADDR11, NAPOT(va, size));
		break;
	case ENTRY_PMPADDR12:
		write_csr(NDS_PMPADDR12, NAPOT(va, size));
		break;
	case ENTRY_PMPADDR13:
		write_csr(NDS_PMPADDR13, NAPOT(va, size));
		break;
	case ENTRY_PMPADDR14:
		write_csr(NDS_PMPADDR14, NAPOT(va, size));
		break;
	case ENTRY_PMPADDR15:
		write_csr(NDS_PMPADDR15, NAPOT(va, size));
		break;
	}

#if __riscv_xlen == 64
	switch (entry >> 3){
	case ENTRY_PMPCFG0:
		write_csr(NDS_PMPCFG0, ((read_csr(NDS_PMPCFG0) & (~(0xFFLL << ((long)(entry%8) << 3)))) | (((long)pmpcfg) << ((long)(entry%8) << 3))));
		break;
	case ENTRY_PMPCFG1:
		write_csr(NDS_PMPCFG2, ((read_csr(NDS_PMPCFG2) & (~(0xFFLL << ((long)(entry%8) << 3)))) | (((long)pmpcfg) << ((long)(entry%8) << 3))));
		break;
	}
#else
	switch (entry >> 2) {
	case ENTRY_PMPCFG0:
		write_csr(NDS_PMPCFG0, ((read_csr(NDS_PMPCFG0) & (~((0xFF) << ((entry%4) << 3)))) | (((long)pmpcfg) << ((entry%4) << 3))));
		break;
	case ENTRY_PMPCFG1:
		write_csr(NDS_PMPCFG1, ((read_csr(NDS_PMPCFG1) & (~((0xFF) << ((entry%4) << 3)))) | (((long)pmpcfg) << ((entry%4) << 3))));
		break;
	case ENTRY_PMPCFG2:
		write_csr(NDS_PMPCFG2, ((read_csr(NDS_PMPCFG2) & (~((0xFF) << ((entry%4) << 3)))) | (((long)pmpcfg) << ((entry%4) << 3))));
		break;
	case ENTRY_PMPCFG3:
		write_csr(NDS_PMPCFG3, ((read_csr(NDS_PMPCFG3) & (~((0xFF) << ((entry%4) << 3)))) | (((long)pmpcfg) << ((entry%4) << 3))));
		break;
	}
#endif
}


void init_pmp_config()
{
	int i = 0;
	int config_count = 0;
	/*
	 * Check whether the CPU configured with PMP feature.
	 * Write 0x3 to the pmpcfg0 register, and read it back
	 * to check the number of PMP entries.
	 */
	write_csr(NDS_PMPCFG0, 0x3);
	if (!read_csr(NDS_PMPCFG0)) {
		os_printf("PMP entries is 0, CPU does NOT support PMP.\n");
		while(1);
	}

	/* Disable global interrupt but allow exception */
	clear_csr(NDS_MSTATUS, MSTATUS_MIE);

#if USE_NAPOT

	os_printf("PMP uses NAPOT scheme!.\n");

#else	/* USE_TOR */

	os_printf("PMP uses TOR scheme!.\n");
	config_count = sizeof(pmp_tor_config_table)/sizeof(pmp_config_t);

	os_printf("PMP config count(%d).\n", config_count);
	for (i = 0; i < config_count; i++)
	{
		pmp_tor_config(pmp_tor_config_table[i].pmp_entry,
					pmp_tor_config_table[i].pmp_addr,
					pmp_tor_config_table[i].pmp_config);

		//sys_delay_sync(SYS_DELAY_TIME_10US);
	}

#endif
}

void show_pmp_config()
{
	os_printf("==========PMP config info===============\r\n");

	int i = 0;
	int config_count = 0;
	long pmp_config0 = read_csr(NDS_PMPCFG0);
	if (!pmp_config0) {
		os_printf("PMP entries is 0, CPU does NOT support PMP.\n");
	}

#if USE_NAPOT

	os_printf("PMP uses NAPOT scheme!.\n");

#else	/* USE_TOR */

	os_printf("PMP uses TOR scheme!.\n");
	config_count = sizeof(pmp_tor_config_table)/sizeof(pmp_config_t);

	os_printf("PMP user config count(%d).\n", config_count);
	for (i = 0; i < config_count; i++)
	{
		os_printf("PMP config[%d], 0x%08x, 0x%0x.\r\n", i,
					(void *)pmp_tor_config_table[i].pmp_addr,
					pmp_tor_config_table[i].pmp_config);

	}

#endif
	os_printf("==========PMP actural config:\r\n");
	os_printf("==========NDS_PMPCFG0:  0x%08x.\r\n", pmp_config0);
	os_printf("==========NDS_PMPCFG1:  0x%08x.\r\n", read_csr(NDS_PMPCFG1));

	os_printf("==========NDS_PMPADDR0: 0x%08x.\r\n", read_csr(NDS_PMPADDR0));
	os_printf("==========NDS_PMPADDR1: 0x%08x.\r\n", read_csr(NDS_PMPADDR1));
	os_printf("==========NDS_PMPADDR2: 0x%08x.\r\n", read_csr(NDS_PMPADDR2));
	os_printf("==========NDS_PMPADDR3: 0x%08x.\r\n", read_csr(NDS_PMPADDR3));
	os_printf("==========NDS_PMPADDR4: 0x%08x.\r\n", read_csr(NDS_PMPADDR4));
	os_printf("==========NDS_PMPADDR5: 0x%08x.\r\n", read_csr(NDS_PMPADDR5));
	os_printf("==========NDS_PMPADDR6: 0x%08x.\r\n", read_csr(NDS_PMPADDR6));
	os_printf("==========NDS_PMPADDR7: 0x%08x.\r\n", read_csr(NDS_PMPADDR7));

}

#else

void show_pmp_config()
{
}

#include "sdkconfig.h"
#include "mon_call.h"

extern char _sram_start;
extern char _sram_end;
extern char _swap_start;

void init_u_mode_pmp_config()
{
#define SRAM_START_ADDR   0x30000000
#define SRAM_END_ADDR     0X38080000

#if CONFIG_DUAL_CORE  // #if config_multi_core
// #if CONFIG_MAILBOX  // #if config_multi_core

	u8    free_cfg = 0;

	mon_get_free_pmp_cfg(&free_cfg);

	if(free_cfg == 0)
		return;

    /* there codes are designed for simple configuration case as illustrated below */
    /* for more complex cases, these codes should be modified to suite the config. */
    /****************************************************/
    /* SRAM                                             */
    /* |--------------------|--------------|----------| */
    /* |------- CPU0 -------|----- CPU1 ---|-- SWAP --| */
    /* |------- CPU1 -------|----- CPU0 ---|-- SWAP --| */
    /****************************************************/

	u32    start_addr, size, tmp;
	u8     attr = OTP_R_ON;

	#if CONFIG_SYS_CPU0
	attr = OTP_R_ON;  // used for CPU0 dump.
	#else
	attr = 0;
	#endif

	start_addr = (u32)&_sram_start;
	tmp = start_addr & (~0x08000000);

	if(tmp > SRAM_START_ADDR)
	{
		size = start_addr - SRAM_START_ADDR;

		mon_pmp_cfg(SRAM_START_ADDR, size, attr);
	}
	else if(tmp == SRAM_START_ADDR)
	{
		start_addr = (u32)&_sram_end;  // _sram_end is aligned with 16 bytes set in SAG file.
		size = (u32)&_swap_start - start_addr;
		mon_pmp_cfg(start_addr, size, attr);
	}

#endif

}

#endif

