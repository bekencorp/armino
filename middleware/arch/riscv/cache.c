#include <stdio.h>
#include "platform.h"
#include <os/os.h>
#include <components/system.h>
#include "cache.h"

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
    0x30020000,
    0x30040000,
    0x30060000,
    0x30000000
};
#else //#if CONFIG_CACHE_ENABLE
const unsigned int g_sram_addr_map[SRAM_BLOCK_COUNT] = {
    0x38000000,
    0x38020000,
    0x30000000,
    0x30020000
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
        addr = SRAM_CACHE_CONFIG_BASE + (i * SRAM_BLOCK_COUNT);
        data = *((volatile unsigned int *) (addr));
        *((volatile unsigned int *) (addr)) = (g_sram_addr_map[i]) + (data & 0x03ff);
   }
}

int show_cache_config_info(void)
{
    unsigned int iset, iway, isize, dset, dway, dsize;
    unsigned long icm_cfg = 0, dcm_cfg = 0;

    /* Check if support CCTL feature */
    if (read_csr(NDS_MMSC_CFG) & CCTLCSR_MSK) {
        os_printf("CPU supports CCTL operation\n");

        if (read_csr(NDS_MMSC_CFG) & VCCTL_MSK) {
            os_printf("CPU supports CCTL auto-increment\n");
        } else {
            os_printf("CPU does NOT support CCTL auto-increment\n");
        }

    } else {
        os_printf("CPU supports FENCE operation\n");
    }

    /* Enable I/D cache HW prefetcher and D-cache write-around (threshold: 4 cache lines) as default */
    // clear_csr(NDS_MCACHE_CTL, DC_WARND_MSK);
    // set_csr(NDS_MCACHE_CTL, (1 << 13) | (1 << 10) | (1 << 9));

    /* Get ICache ways, sets, line size */
    icm_cfg = read_csr(NDS_MICM_CFG);
    if ((icm_cfg & ISET_MSK) < 7) {
        iset = (unsigned int)(1 << ((icm_cfg & ISET_MSK) + 6));
    } else {
        iset = 0;
    }
    os_printf("The L1C ICache sets = %u\n", iset);

    iway = (unsigned int)(((icm_cfg & IWAY_MSK) >> 3) + 1);
    os_printf("The L1C ICache ways = %u\n", iway);

    if (((icm_cfg & ISIZE_MSK) >> 6) && (((icm_cfg & ISIZE_MSK) >> 6) <= 5)) {
        isize = (unsigned int)(1 << (((icm_cfg & ISIZE_MSK) >> 6) + 2));
    } else if (((icm_cfg & ISIZE_MSK) >> 6) >= 6) {
        os_printf("Warning L1C i cacheline size is reserved value\n");
        isize = 0;
    } else {
        isize = 0;
    }

    os_printf("The L1C ICache line size = %u\n", isize);
    if (isize == 0) {
        os_printf("This CPU doesn't have L1C ICache.\n");
        return 0;
    } else {
        /* Enable L1C ICache */
        // set_csr(NDS_MCACHE_CTL, (0x1 << 0));
    }

    /* Get DCache ways, sets, line size  */
    dcm_cfg = read_csr(NDS_MDCM_CFG);
    if ((dcm_cfg & DSET_MSK) < 7) {
        dset = (unsigned int)(1 << ((dcm_cfg & DSET_MSK) + 6));
    } else {
        dset = 0;
    }
    os_printf("The L1C DCache sets = %u\n", dset);

    dway = (unsigned int)(((dcm_cfg & DWAY_MSK) >> 3) + 1);
    os_printf("The L1C DCache ways = %u\n", dway);

    if (((dcm_cfg & DSIZE_MSK) >> 6) && (((dcm_cfg & DSIZE_MSK) >> 6) <= 5)) {
        dsize = (unsigned int)(1 << (((dcm_cfg & DSIZE_MSK) >> 6) + 2));
    } else if (((dcm_cfg & DSIZE_MSK) >> 6) >= 6) {
        os_printf("Warning L1C d cacheline size is reserved value\n");
        dsize = 0;
    } else {
        dsize = 0;
    }

    os_printf("The L1C DCache line size = %u\n", dsize);
    if (dsize == 0) {
        os_printf("This CPU doesn't have L1C DCache.\n");
        return 0;
    } else {
        /* Enable L1C DCache */
        // set_csr(NDS_MCACHE_CTL, (0x1 << 1));
    }

    if (read_csr(NDS_MCACHE_CTL) & 0x1) {
        os_printf("Enable L1C I cache\n");
    }

    if (read_csr(NDS_MCACHE_CTL) & 0x2) {
        os_printf("Enable L1C D cache\n");
    }

    if (!(read_csr(NDS_MCACHE_CTL) & 0x3)) {
        os_printf("Can't enable L1C I/D cache\n");
    }

    return 1;
}

void flush_dcache(void *va, long size)
{
    // os_printf("I/D cache flush\n");

    if(read_csr(NDS_MMSC_CFG) & CCTLCSR_MSK) {
        unsigned int i, tmp = 0, dsize = 0;

        dsize = (unsigned int)(1 << (((read_csr(NDS_MDCM_CFG) & DSIZE_MSK) >> 6) + 2));

        /* Check whether the CPU configured with CCTL auto-incremented feature. */
        if ((read_csr(NDS_MMSC_CFG) & VCCTL_MSK)) {
            unsigned long final_va = (unsigned long)(va + size);
            unsigned long next_va = (unsigned long)va;

            /* Write only once at the beginning, it will be updated by CCTL command CSR write operation */
            write_csr(NDS_MCCTLBEGINADDR, (unsigned long)va);

            /* L1C DCache write back and invalidate */
            while (next_va < final_va) {
                /* Write back and invalid one cache line each time */
                write_csr(NDS_MCCTLCOMMAND, CCTL_L1D_VA_WBINVAL);

                /* Read back from BEGINADDR csr for next va */
                next_va = read_csr(NDS_MCCTLBEGINADDR);
            }

        } else {
            /* L1C DCache write back and invalidate */
            for (i = 0; tmp < size; i++) {
                /* Write back and invalid one cache line each time */
                write_csr(NDS_MCCTLBEGINADDR, (unsigned long)(va + (i * dsize)));
                tmp += dsize;
                write_csr(NDS_MCCTLCOMMAND, CCTL_L1D_VA_WBINVAL);
            }
        }

    } else {
        /* FENCE.I go data writeback w/o data invalid but instruction invalid.
           As src code is copied to dst and go to execute dst instruction,
           you should use FENCE.I instead of FENCE */
        __nds__fencei();
    }
}

void flush_all_dcache(void) {
    int i = 0;
    for(i = 0; i < SRAM_BLOCK_COUNT; i++)
    {
        if(g_sram_addr_map[i] & 0x08000000) {
            flush_dcache((void *)g_sram_addr_map[i], SRAM_BLOCK_SIZE);
        }
    }
}