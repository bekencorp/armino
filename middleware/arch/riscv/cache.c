#include <stdio.h>
#include "platform.h"
#include <os/os.h>
#include <components/system.h>
#include "cache.h"
#include "mon_call.h"

/* CSR NDS_ICM_CFG */
#define ISET_MSK                                ((1ULL << 2) | (1ULL << 1) | (1ULL << 0))
#define IWAY_MSK                                ((1ULL << 5) | (1ULL << 4) | (1ULL << 3))
#define ISIZE_MSK                               ((1ULL << 8) | (1ULL << 7) | (1ULL << 6))

/* CSR NDS_DCM_CFG */
#define DSET_MSK                                ((1ULL << 2) | (1ULL << 1) | (1ULL << 0))
#define DWAY_MSK                                ((1ULL << 5) | (1ULL << 4) | (1ULL << 3))
#define DSIZE_MSK                               ((1ULL << 8) | (1ULL << 7) | (1ULL << 6))

/* CSR NDS_MCACHE_CTL */
#define DC_WARND_MSK                            (3UL << 13)
#define CCTL_SUEN_MSK                           (1ULL << 8)

/* CSR NDS_MMSC_CFG */
#define CCTLCSR_MSK                             (1ULL << 16)
#define VCCTL_MSK                               ((1ULL << 18) | (1ULL << 19))

/* AndeStar CCTL Command Machine mode */
/* Allow S/U mode */
#define CCTL_L1D_VA_INVAL                       0
/* Allow S/U mode */
#define CCTL_L1D_VA_WB                          1
/* Allow S/U mode */
#define CCTL_L1D_VA_WBINVAL                     2
#define CCTL_L1D_VA_LOCK                        3
#define CCTL_L1D_VA_UNLOCK                      4
#define CCTL_L1D_WBINVAL_ALL                    6
#define CCTL_L1D_WB_ALL                         7
/* Allow S/U mode */
#define CCTL_L1I_VA_INVAL                       8
#define CCTL_L1I_VA_LOCK                        11
#define CCTL_L1I_VA_UNLOCK                      12
#define CCTL_L1D_IX_INVAL                       16
#define CCTL_L1D_IX_WB                          17
#define CCTL_L1D_IX_WBINVAL                     18
#define CCTL_L1D_IX_RTAG                        19
#define CCTL_L1D_IX_RDATA                       20
#define CCTL_L1D_IX_WTAG                        21
#define CCTL_L1D_IX_WDATA                       22
#define CCTL_L1D_INVAL_ALL                      23
#define CCTL_L1I_IX_INVAL                       24
#define CCTL_L1I_IX_RTAG                        27
#define CCTL_L1I_IX_RDATA                       28
#define CCTL_L1I_IX_WTAG                        29
#define CCTL_L1I_IX_WDATA                       30

#define USE_CCTL                                0
#define BUF_SIZE                                0x100

/* SMU.SYSTEMCFG Configuration Register */
#define L2C_CTL_OFF                             8
#define L2C_CTL_MSK                             (0x1 << L2C_CTL_OFF)

/* Configuration Register */
#define L2C_SIZE_OFF                            7
#define L2C_SIZE_MSK                            (0x1F << L2C_SIZE_OFF)
#define L2C_SIZE_0KB                            (0x00 << L2C_SIZE_OFF)
#define L2C_SIZE_128KB                          (0x01 << L2C_SIZE_OFF)
#define L2C_SIZE_256KB                          (0x02 << L2C_SIZE_OFF)
#define L2C_SIZE_512KB                          (0x04 << L2C_SIZE_OFF)
#define L2C_SIZE_1024KB                         (0x08 << L2C_SIZE_OFF)
#define L2C_SIZE_2048KB                         (0x10 << L2C_SIZE_OFF)
#define L2C_LINE_SIZE                           32

/* Control Register */
#define L2C_ENABLE                              0x1

/* Prefetch Threshold */
#define PFTHRES_OFF                             1
#define PFTHRES_MSK                             (0x3 << PFTHRES_OFF)

/* Instruction Prefetch Depth */
#define IPFDPT_OFF                              3
#define IPFDPT_MSK                              (0x3 << IPFDPT_OFF)
#define IPFDPT_3REQ                             (0x3 << IPFDPT_OFF)

/* Data Prefetch Depth */
#define DPFDPT_OFF                              5
#define DPFDPT_MSK                              (0x3 << DPFDPT_OFF)
#define DPFDPT_3REQ                             (0x3 << DPFDPT_OFF)

/* Tag Ram output cycle */
#define TRAMOCTL_OFF                            8
#define TRAMOCTL_MSK                            (0x3 << TRAMOCTL_OFF)

/* Tag Ram setup cycle */
#define TRAMICTL_OFF                            10
#define TRAMICTL_MSK                            (0x1 << TRAMICTL_OFF)

/* Data Ram output cycle */
#define DRAMOCTL_OFF                            11
#define DRAMOCTL_MSK                            (0x3 << DRAMOCTL_OFF)

/* Data Ram setup cycle */
#define DRAMICTL_OFF                            13
#define DRAMICTL_MSK                            (0x1 << DRAMICTL_OFF)

/* CCTL Command Register */
#define CCTL_L2_IX_INVAL                        0x0
#define CCTL_L2_IX_WB                           0x1
#define CCTL_L2_IX_WBINVAL                      0x2
#define CCTL_L2_PA_INVAL                        0x8
#define CCTL_L2_PA_WB                           0x9
#define CCTL_L2_PA_WBINVAL                      0xA
#define CCTL_L2_TGT_WRITE                       0x10
#define CCTL_L2_TGT_READ                        0x11
#define CCTL_L2_WBINVAL_ALL                     0x12

/* CCTL Access Line Registers */
#define CCTL_L2_ACC_SET_OFF                     5
#define CCTL_L2_ACC_SET_MSK                     (0x7FFF << CCTL_L2_ACC_SET_OFF)
#define CCTL_L2_ACC_WAY_OFF                     28
#define CCTL_L2_ACC_WAY_MSK                     (0xF << CCTL_L2_ACC_WAY_OFF)
#define CCTL_L2_ACC_RAMID_OFF                   26
#define CCTL_L2_ACC_RAMID_MSK                   (0x3 << CCTL_L2_ACC_RAMID_OFF)

/* CCTL Status Register */
#define CCTL_L2_ST_IDLE                         0
#define CCTL_L2_ST_RUNNING                      1
#define CCTL_L2_ST_INVALID                      2
#define CCTL_ST_MASK                            0xF

#define SRAM_CACHE_CONFIG_BASE                  0x3fff0048

#if !CONFIG_CACHE_CUSTOM_SRAM_MAPPING
#if !CONFIG_CACHE_ENABLE
const unsigned int g_sram_addr_map[SRAM_BLOCK_COUNT] = {
    0x30060000,
    0x30020000,
    0x30040000,
    0x30000000
};
#else //#if CONFIG_CACHE_ENABLE
const unsigned int g_sram_addr_map[SRAM_BLOCK_COUNT] = {
    0x38000000,
    0x30020000,
    0x38020000,
    0x30000000
};
#endif //#if CONFIG_CACHE_ENABLE
#else  //#if !CONFIG_CACHE_CUSTOM_SRAM_MAPPING
extern unsigned int g_sram_addr_map[SRAM_BLOCK_COUNT];
#endif //#if !CONFIG_CACHE_CUSTOM_SRAM_MAPPING

void sram_dcache_map(void)
{
   int i = 0;
   unsigned int addr, data;

   for(i = 0; i < SRAM_BLOCK_COUNT; i++)
   {
        addr = SRAM_CACHE_CONFIG_BASE + (i * sizeof(data));
        data = *((volatile unsigned int *) (addr));
        *((volatile unsigned int *) (addr)) = (g_sram_addr_map[i]) + (data & 0x03ff);
   }
}

int show_cache_config_info(void)
{
	return 1;
}

void flush_dcache(void *va, long size)
{
	unsigned int  tmp = 0, line_size = 32;
	unsigned int  start_line_va, end_line_va;

	start_line_va = (((unsigned int)va) & (~(line_size - 1)));
	end_line_va = ((((unsigned int)va) + size + line_size - 1) & (~(line_size - 1)));

	// flush area: [start_line_va, end_line_va) .
	size = end_line_va - start_line_va;

    /* L1C DCache write back and invalidate */
    while (tmp < size)
	{
        /* Write back and invalid one cache line each time */
        write_csr(NDS_UCCTLBEGINADDR, (unsigned long)(start_line_va + tmp));
        tmp += line_size;
        write_csr(NDS_UCCTLCOMMAND, CCTL_L1D_VA_WBINVAL);
    }
}

void flush_all_dcache(void) 
{
    int i = 0;
	
    for(i = 0; i < SRAM_BLOCK_COUNT; i++)
    {
        if(g_sram_addr_map[i] & 0x08000000) 
		{
			mon_flush_dcache(g_sram_addr_map[i]);
			return;
        }
    }
}

void enable_dcache(int enable) 
{
    // RISC-V enable dcache as default
    return;
}