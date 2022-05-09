/*
 * Copyright (c) 2012-2021 Andes Technology Corporation
 * All rights reserved.
 *
 */

#ifndef __PMP_H__
#define __PMP_H__

/* Check platform support which PMP scheme!
 * CLIC platform only support NAPOT scheme! */
#define USE_NAPOT                               0
#define USE_TOR                                 !(USE_NAPOT)
#define SCHEME_NAPOT                            3
#define SCHEME_NA4                              2
#define SCHEME_TOR                              1
#define SCHEME_OFF                              0
#define NAPOT(base, size)                       (unsigned long)(((size) > 0) ? ((((unsigned long)(base) & (~((unsigned long)(size) - 1))) >> 2) | (((unsigned long)(size) - 1) >> 3)) : 0)
#define TOR(top)                                (unsigned long)((unsigned long)(top) >> 2)
#define PMP_L_OFF                               0
#define PMP_L_ON                                1
#define PMP_A_OFF                               0
#define PMP_A_TOR                               SCHEME_TOR
#define PMP_A_NA4                               SCHEME_NA4
#define PMP_A_NAPOT                             SCHEME_NAPOT
#define PMP_X_OFF                               0
#define PMP_X_ON                                1
#define PMP_W_OFF                               0
#define PMP_W_ON                                1
#define PMP_R_OFF                               0
#define PMP_R_ON                                1

/* attribute is used for PA */
#define PMPCFG_ALXWR(a,l,x,w,r)                 (((a) << 3) | ((l) << 7) | ((x) << 2) | ((w) << 1) | ((r) << 0))

/* Machine CSR */
#define MSTATUS_MPP_MSK                         ((1ULL << 11) | (1ULL << 12))
#define MSTATUS_MPP_USER                        (0ULL << 11)
#define MSTATUS_MPP_SV                          (1ULL << 11)
#define MSTATUS_MPP_MC                          ((1ULL << 11) | (1ULL << 12))

/* Supervisor CSR */
#define SSTATUS_SPP_MSK                         (1ULL << 8)

/* Hart mode for machine/supervisor/user */
#define HART_USER                               0x0
#define HART_SV                                 0x1
#define HART_MC                                 0x3

#define KB                                      (0x1 << 10)

enum
{
    ENTRY_PMPADDR0 = 0,
	ENTRY_PMPADDR1,
	ENTRY_PMPADDR2,
	ENTRY_PMPADDR3,
	ENTRY_PMPADDR4,
	ENTRY_PMPADDR5,
	ENTRY_PMPADDR6,
	ENTRY_PMPADDR7,
	ENTRY_PMPADDR8,
	ENTRY_PMPADDR9,
	ENTRY_PMPADDR10,
	ENTRY_PMPADDR11,
	ENTRY_PMPADDR12,
	ENTRY_PMPADDR13,
	ENTRY_PMPADDR14,
	ENTRY_PMPADDR15,
	ENTRY_PMPADDR_COUNT
};

enum
{
    ENTRY_PMPCFG0 = 0,
	ENTRY_PMPCFG1,
	ENTRY_PMPCFG2,
	ENTRY_PMPCFG3,
};

typedef struct {
	uint8_t pmp_entry;
	uint8_t pmp_config; 
	/*
	    union 
	    { 
	        struct 
	        { 
	           volatile uint8_t  R               : 1;  //[0], Read Access Control
	           volatile uint8_t  W               : 1;  //[1], Write Access Control
	           volatile uint8_t  X               : 1;  //[2], Instruction execution Control
	           volatile uint8_t  A               : 2;  //[4:3], Address matching mode
	           volatile uint8_t  reserved        : 2;  //[6:5],
	           volatile uint8_t  L               : 1;  //[7], Write Lock and permission enforcement bit for Machine mode
	        }; 
	        uint8_t value; 
	    }pmp_config;
    */
	void *pmp_addr;
} pmp_config_t;

#endif // __PMP_H__
