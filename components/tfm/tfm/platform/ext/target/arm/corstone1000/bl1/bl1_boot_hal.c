/*
 * Copyright (c) 2019-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdio.h>
#include "cmsis.h"
#include "region.h"
#include "boot_hal.h"
#include "Driver_Flash.h"
#include "flash_layout.h"
#include "bootutil/fault_injection_hardening.h"
#include "bootutil/bootutil_log.h"
#include "firewall.h"
#include "watchdog.h"
#include "mpu_config.h"
#include "tfm_plat_otp.h"
#include "tfm_plat_provisioning.h"
#include "fwu_agent.h"

#if defined(CRYPTO_HW_ACCELERATOR) || \
    defined(CRYPTO_HW_ACCELERATOR_OTP_PROVISIONING)
#include "crypto_hw.h"
#endif

/* Flash device name must be specified by target */
extern ARM_DRIVER_FLASH FLASH_DEV_NAME;

extern ARM_DRIVER_FLASH FLASH_DEV_NAME_SE_SECURE_FLASH;

REGION_DECLARE(Image$$, ER_DATA, $$Base)[];
REGION_DECLARE(Image$$, ARM_LIB_HEAP, $$ZI$$Limit)[];

#define HOST_ADDRESS_SPACE_BASE         0x00000000
#define HOST_SHARED_RAM_BASE            0x02000000
#define HOST_XNVM_BASE                  0x08000000
#define HOST_BASE_SYSTEM_CONTROL_BASE   0x1A010000
#define HOST_FIREWALL_BASE              0x1A800000
#define HOST_INT_APBCOM                 0x1B900000
#define HOST_FPGA_SCC_REGISTERS         0x40000000
#define HOST_AXI_QSPI_CTRL_REG_BASE     0x40050000
#define HOST_ETHERNET_CTRL_REG_BASE     0x40100000
#define HOST_USB_CTRL_REG_BASE          0x40200000

#define HOST_SE_SECURE_FLASH_BASE_FVP       0x60010000
#define HOST_AXI_QSPI_CTRL_REG_BASE_SE_SECURE_FLASH    0x60010000

#define HOST_DRAM_BASE                  0x80000000
#define HOST_DRAM_UEFI_CAPSULE          0x80000000

#define SE_MID                          0

#define CORSTONE1000_HOST_ADDRESS_SPACE_SIZE  0x80000000U

#define COMP_AONPERIPH_PRE_CONFIGURED_RGNS 39
#define COMP_SYSPERIPH_PRE_CONFIGURED_RGNS 18


enum se_firewall_comp_id_t {
  SE_FCTRL = (0x00u),
  COMP_FC1,
};

enum host_firewall_host_comp_id_t {
  HOST_FCTRL = (0x00u),
  COMP_SYSPERIPH,
  COMP_DBGPERIPH,
  COMP_AONPERIPH,
  COMP_XNVM,
  COMP_CVM,
  COMP_HOSTCPU,
  COMP_EXTSYS0,
  COMP_EXTSYS1,
  COMP_EXPSLV0,
  COMP_EXPSLV1,
  COMP_EXPMST0,
  COMP_EXPMST1,
  COMP_OCVM,
  COMP_DEBUG,
};

static void setup_mpu(void)
{
    uint32_t size; /* region size */
    uint32_t rasr; /* region attribute and size register */
    uint32_t rbar; /* region base address register */
    uint32_t total_region;
    uint32_t rnr = FIREWALL_REGION_NUMBER; /* current region number */
    uint32_t base; /* start address */

    /* ARMv6-M only supports a unified MPU so reading DREGION is enough */
    total_region = ((MPU->TYPE & MPU_TYPE_DREGION_Msk) >>
                                MPU_TYPE_DREGION_Pos);

    /* Clear all regions before initialization */
    for (uint32_t i = 0; i < total_region; i++) {
        ARM_MPU_ClrRegion(i);
    }

    ARM_MPU_Disable();

    /* Access to host system address space */
    base = CORSTONE1000_HOST_ADDRESS_SPACE_BASE;
    size = get_rbar_size_field(CORSTONE1000_HOST_ADDRESS_SPACE_SIZE);

    /* Privilidged access only */
    rasr = ARM_MPU_RASR(XN_EXEC_OK, AP_RW_PRIV_ONLY, TEX, NOT_SHAREABLE,
            NOT_CACHEABLE, NOT_BUFFERABLE, SUB_REGION_DISABLE, size);
    rbar = base & MPU_RBAR_ADDR_Msk;

    ARM_MPU_SetRegionEx(rnr, rbar, rasr);

    arm_mpu_enable();
}

static void setup_se_firewall(void)
{
    enum rgn_mpl_t mpl_rights = 0;

#if !(PLATFORM_IS_FVP)
    /* Configure the SE firewall controller */
    fc_select((void *)CORSTONE1000_SE_FIREWALL_BASE, SE_FCTRL);
    fc_disable_bypass();

    fc_select_region(2);
    fc_disable_regions();
    fc_disable_mpe(RGN_MPE0);
    fc_prog_rgn(RGN_SIZE_2MB, CORSTONE1000_SE_FIREWALL_BASE);
    fc_init_mpl(RGN_MPE0);

    mpl_rights = (RGN_MPL_SECURE_READ_MASK |
                  RGN_MPL_SECURE_WRITE_MASK);
    fc_enable_mpl(RGN_MPE0, mpl_rights);
    fc_prog_mid(RGN_MPE0, SE_MID);
    fc_enable_mpe(RGN_MPE0);
    fc_enable_regions();

    fc_pe_enable();
#endif

    /* Configure the SE firewall component 1 */
    fc_select((void *)CORSTONE1000_SE_FIREWALL_BASE, COMP_FC1);
    fc_disable_bypass();
    fc_pe_disable();

    /* Boot Instruction Register region + shared RAM region: 64MB
     * + 32 MB XNVM region */
    fc_select_region(1);
    fc_disable_regions();
    fc_disable_mpe(RGN_MPE0);
    fc_prog_rgn(RGN_SIZE_256MB, CORSTONE1000_HOST_ADDRESS_SPACE_BASE);
    fc_prog_rgn_upper_addr(HOST_ADDRESS_SPACE_BASE);
    fc_enable_addr_trans();
    fc_init_mpl(RGN_MPE0);

    mpl_rights = (RGN_MPL_SECURE_READ_MASK |
                  RGN_MPL_SECURE_WRITE_MASK |
                  RGN_MPL_SECURE_EXECUTE_MASK);

    fc_enable_mpl(RGN_MPE0, mpl_rights);
    fc_prog_mid(RGN_MPE0, SE_MID);
    fc_enable_mpe(RGN_MPE0);
    fc_enable_regions();

    /* Host SCB Registers: 64KB */
    fc_select_region(2);
    fc_disable_regions();
    fc_disable_mpe(RGN_MPE0);
    fc_prog_rgn(RGN_SIZE_64KB, CORSTONE1000_HOST_BASE_SYSTEM_CONTROL_BASE);
    fc_prog_rgn_upper_addr(HOST_BASE_SYSTEM_CONTROL_BASE);
    fc_enable_addr_trans();
    fc_init_mpl(RGN_MPE0);

    mpl_rights = (RGN_MPL_SECURE_READ_MASK |
                  RGN_MPL_SECURE_WRITE_MASK);

    fc_enable_mpl(RGN_MPE0, mpl_rights);
    fc_prog_mid(RGN_MPE0, SE_MID);
    fc_enable_mpe(RGN_MPE0);
    fc_enable_regions();

    /* Host firewall: 2MB */
    fc_select_region(3);
    fc_disable_regions();
    fc_disable_mpe(RGN_MPE0);
    fc_prog_rgn(RGN_SIZE_2MB, CORSTONE1000_HOST_FIREWALL_BASE);
    fc_prog_rgn_upper_addr(HOST_FIREWALL_BASE);
    fc_enable_addr_trans();
    fc_init_mpl(RGN_MPE0);

    mpl_rights = (RGN_MPL_SECURE_READ_MASK |
                  RGN_MPL_SECURE_WRITE_MASK);

    fc_enable_mpl(RGN_MPE0, mpl_rights);
    fc_prog_mid(RGN_MPE0, SE_MID);
    fc_enable_mpe(RGN_MPE0);
    fc_enable_regions();

#if PLATFORM_IS_FVP
    /* SE Flash Write: 8MB */
    fc_select_region(4);
    fc_disable_regions();
    fc_disable_mpe(RGN_MPE0);
    fc_prog_rgn(RGN_SIZE_8MB, CORSTONE1000_HOST_SE_SECURE_FLASH_BASE_FVP);
    fc_prog_rgn_upper_addr(HOST_SE_SECURE_FLASH_BASE_FVP);
    fc_enable_addr_trans();
    fc_init_mpl(RGN_MPE0);

    mpl_rights = (RGN_MPL_SECURE_READ_MASK |
                  RGN_MPL_SECURE_WRITE_MASK);

    fc_enable_mpl(RGN_MPE0, mpl_rights);
    fc_prog_mid(RGN_MPE0, SE_MID);
    fc_enable_mpe(RGN_MPE0);
    fc_enable_regions();
#else
    /* SCC Register + QSPI Flash Write: 64KB */
    fc_select_region(4);
    fc_disable_regions();
    fc_disable_mpe(RGN_MPE0);
    fc_prog_rgn(RGN_SIZE_512KB, CORSTONE1000_HOST_FPGA_SCC_REGISTERS);
    fc_prog_rgn_upper_addr(HOST_FPGA_SCC_REGISTERS);
    fc_enable_addr_trans();
    fc_init_mpl(RGN_MPE0);

    mpl_rights = (RGN_MPL_SECURE_READ_MASK |
                  RGN_MPL_SECURE_WRITE_MASK);

    fc_enable_mpl(RGN_MPE0, mpl_rights);
    fc_prog_mid(RGN_MPE0, SE_MID);
    fc_enable_mpe(RGN_MPE0);
    fc_enable_regions();
#endif

    /* SDC600 APB COM PORT */
    fc_select_region(5);
    fc_disable_regions();
    fc_disable_mpe(RGN_MPE0);
    fc_prog_rgn(RGN_SIZE_64KB, CORSTONE1000_HOST_INT_APBCOM_BASE);
    fc_prog_rgn_upper_addr(HOST_INT_APBCOM);
    fc_enable_addr_trans();
    fc_init_mpl(RGN_MPE0);

    mpl_rights = (RGN_MPL_SECURE_READ_MASK |
                  RGN_MPL_SECURE_WRITE_MASK);

    fc_enable_mpl(RGN_MPE0, mpl_rights);
    fc_prog_mid(RGN_MPE0, SE_MID);
    fc_enable_mpe(RGN_MPE0);
    fc_enable_regions();

    /* DDR/DRAM/UEFI_CAPSULE: 32MB */
    fc_select_region(6);
    fc_disable_regions();
    fc_disable_mpe(RGN_MPE0);
    fc_prog_rgn(RGN_SIZE_512MB, CORSTONE1000_HOST_DRAM_UEFI_CAPSULE);
    fc_prog_rgn_upper_addr(HOST_DRAM_UEFI_CAPSULE);
    fc_enable_addr_trans();
    fc_init_mpl(RGN_MPE0);
    mpl_rights = (RGN_MPL_SECURE_READ_MASK |
                  RGN_MPL_SECURE_WRITE_MASK);

    fc_enable_mpl(RGN_MPE0, mpl_rights);
    fc_prog_mid(RGN_MPE0, SE_MID);
    fc_enable_mpe(RGN_MPE0);
    fc_enable_regions();

#if !(PLATFORM_IS_FVP)
    fc_select_region(7);
    fc_disable_regions();
    fc_disable_mpe(RGN_MPE0);
    fc_prog_rgn(RGN_SIZE_64KB, CORSTONE1000_HOST_AXI_QSPI_CTRL_REG_BASE_SE_SECURE_FLASH);
    fc_prog_rgn_upper_addr(HOST_AXI_QSPI_CTRL_REG_BASE_SE_SECURE_FLASH);
    fc_enable_addr_trans();
    fc_init_mpl(RGN_MPE0);

    mpl_rights = (RGN_MPL_SECURE_READ_MASK |
                  RGN_MPL_SECURE_WRITE_MASK);

    fc_enable_mpl(RGN_MPE0, mpl_rights);
    fc_prog_mid(RGN_MPE0, SE_MID);
    fc_enable_mpe(RGN_MPE0);
    fc_enable_regions();
#endif

    fc_pe_enable();
}

static void setup_host_firewall(void)
{
    enum rgn_mpl_t mpl_rights = 0;

#if !(PLATFORM_IS_FVP)
    /* Configure the host firewall controller */
    fc_select((void *)CORSTONE1000_HOST_FIREWALL_BASE, HOST_FCTRL);
    fc_disable_bypass();

    fc_select_region(2);
    fc_disable_regions();
    fc_disable_mpe(RGN_MPE0);
    fc_prog_rgn(RGN_SIZE_2MB, CORSTONE1000_HOST_FIREWALL_BASE);
    fc_init_mpl(RGN_MPE0);

    mpl_rights = (RGN_MPL_SECURE_READ_MASK |
                  RGN_MPL_SECURE_WRITE_MASK);

    fc_enable_mpl(RGN_MPE0, mpl_rights);
    fc_prog_mid(RGN_MPE0, SE_MID);
    fc_enable_mpe(RGN_MPE0);
    fc_enable_regions();

    fc_pe_enable();
#endif

    /* XNVM - Flash */
    fc_select((void *)CORSTONE1000_HOST_FIREWALL_BASE, COMP_XNVM);
    fc_disable_bypass();
    fc_pe_disable();

    fc_select_region(1);
    fc_disable_regions();
    fc_disable_mpe(RGN_MPE0);
    fc_prog_rgn(RGN_SIZE_32MB, HOST_XNVM_BASE);
    fc_init_mpl(RGN_MPE0);

#if !(PLATFORM_IS_FVP)
    mpl_rights = (RGN_MPL_ANY_MST_MASK |
                  RGN_MPL_SECURE_READ_MASK |
                  RGN_MPL_NONSECURE_READ_MASK);
#else
    mpl_rights = (RGN_MPL_ANY_MST_MASK |
                  RGN_MPL_SECURE_READ_MASK |
                  RGN_MPL_SECURE_WRITE_MASK |
                  RGN_MPL_NONSECURE_READ_MASK |
                  RGN_MPL_NONSECURE_WRITE_MASK);
#endif /* !(PLATFORM_IS_FVP) */

    fc_enable_mpl(RGN_MPE0, mpl_rights);
    fc_disable_mpl(RGN_MPE0, ~mpl_rights);

    fc_enable_mpe(RGN_MPE0);
    fc_enable_regions();
    fc_rgn_lock();

    fc_pe_enable();

    /* CVM - Shared RAM */
    fc_select((void *)CORSTONE1000_HOST_FIREWALL_BASE, COMP_CVM);
    fc_disable_bypass();
    fc_pe_disable();

    fc_select_region(1);
    fc_disable_regions();
    fc_disable_mpe(RGN_MPE0);
    fc_prog_rgn(RGN_SIZE_4MB, HOST_SHARED_RAM_BASE);
    fc_init_mpl(RGN_MPE0);

    mpl_rights = (RGN_MPL_ANY_MST_MASK | RGN_MPL_SECURE_READ_MASK |
                                         RGN_MPL_SECURE_WRITE_MASK |
                                         RGN_MPL_SECURE_EXECUTE_MASK |
                                         RGN_MPL_NONSECURE_READ_MASK |
                                         RGN_MPL_NONSECURE_WRITE_MASK |
                                         RGN_MPL_NONSECURE_EXECUTE_MASK);

    fc_enable_mpl(RGN_MPE0, mpl_rights);
    fc_disable_mpl(RGN_MPE0, ~mpl_rights);

    fc_enable_mpe(RGN_MPE0);
    fc_enable_regions();
    fc_rgn_lock();

    fc_pe_enable();

    /* DDR */
    fc_select((void *)CORSTONE1000_HOST_FIREWALL_BASE, COMP_OCVM);
    fc_disable_bypass();
    fc_pe_disable();

    fc_select_region(1);
    fc_disable_regions();
    fc_disable_mpe(RGN_MPE0);
    fc_prog_rgn(RGN_SIZE_2GB, HOST_DRAM_BASE);
    fc_init_mpl(RGN_MPE0);

    mpl_rights = (RGN_MPL_ANY_MST_MASK | RGN_MPL_SECURE_READ_MASK |
                                         RGN_MPL_SECURE_WRITE_MASK |
                                         RGN_MPL_NONSECURE_READ_MASK |
                                         RGN_MPL_NONSECURE_WRITE_MASK |
                                         RGN_MPL_NONSECURE_EXECUTE_MASK);

    fc_enable_mpl(RGN_MPE0, mpl_rights);
    fc_enable_mpe(RGN_MPE0);
    fc_enable_regions();
    fc_rgn_lock();

    fc_pe_enable();

    /* Host Expansion Master 0 */
    fc_select((void *)CORSTONE1000_HOST_FIREWALL_BASE, COMP_EXPMST0);
    fc_disable_bypass();
    fc_pe_disable();

    fc_select_region(1);
    fc_disable_regions();
    fc_disable_mpe(RGN_MPE0);
    fc_prog_rgn(RGN_SIZE_1MB, HOST_ETHERNET_CTRL_REG_BASE);
    fc_init_mpl(RGN_MPE0);

    mpl_rights = (RGN_MPL_ANY_MST_MASK | RGN_MPL_NONSECURE_READ_MASK |
                                         RGN_MPL_NONSECURE_WRITE_MASK |
                                         RGN_MPL_NONSECURE_EXECUTE_MASK);

    fc_enable_mpl(RGN_MPE0, mpl_rights);
    fc_enable_mpe(RGN_MPE0);
    fc_enable_regions();
    fc_rgn_lock();

    fc_select_region(2);
    fc_disable_regions();
    fc_disable_mpe(RGN_MPE0);
    fc_prog_rgn(RGN_SIZE_1MB, HOST_USB_CTRL_REG_BASE);
    fc_init_mpl(RGN_MPE0);

    mpl_rights = (RGN_MPL_ANY_MST_MASK | RGN_MPL_NONSECURE_READ_MASK |
                                         RGN_MPL_NONSECURE_WRITE_MASK |
                                         RGN_MPL_NONSECURE_EXECUTE_MASK);

    fc_enable_mpl(RGN_MPE0, mpl_rights);
    fc_enable_mpe(RGN_MPE0);
    fc_enable_regions();
    fc_rgn_lock();

#if !(PLATFORM_IS_FVP)
    fc_select_region(3);
    fc_disable_regions();
    fc_disable_mpe(RGN_MPE0);
    fc_prog_rgn(RGN_SIZE_64KB, HOST_AXI_QSPI_CTRL_REG_BASE);
    fc_init_mpl(RGN_MPE0);

    mpl_rights = (RGN_MPL_ANY_MST_MASK | RGN_MPL_SECURE_READ_MASK |
                                         RGN_MPL_SECURE_WRITE_MASK);

    fc_enable_mpl(RGN_MPE0, mpl_rights);
    fc_enable_mpe(RGN_MPE0);
    fc_enable_regions();
    fc_rgn_lock();

    fc_select_region(4);
    fc_disable_regions();
    fc_disable_mpe(RGN_MPE0);
    fc_prog_rgn(RGN_SIZE_4KB, HOST_FPGA_SCC_REGISTERS);
    fc_init_mpl(RGN_MPE0);

    mpl_rights = (RGN_MPL_ANY_MST_MASK | RGN_MPL_SECURE_READ_MASK |
                                         RGN_MPL_SECURE_WRITE_MASK);

    fc_enable_mpl(RGN_MPE0, mpl_rights);
    fc_enable_mpe(RGN_MPE0);
    fc_enable_regions();
    fc_rgn_lock();
#endif

    fc_pe_enable();

    /* Host Expansion Master 0 */
    fc_select((void *)CORSTONE1000_HOST_FIREWALL_BASE, COMP_EXPMST1);
    fc_disable_bypass();
    fc_pe_disable();

#if !(PLATFORM_IS_FVP)
    fc_select_region(1);
    fc_disable_regions();
    fc_disable_mpe(RGN_MPE0);
    fc_prog_rgn(RGN_SIZE_64KB, HOST_AXI_QSPI_CTRL_REG_BASE_SE_SECURE_FLASH);
    fc_init_mpl(RGN_MPE0);

    mpl_rights = (RGN_MPL_ANY_MST_MASK | RGN_MPL_SECURE_READ_MASK |
                                         RGN_MPL_SECURE_WRITE_MASK);

    fc_enable_mpl(RGN_MPE0, mpl_rights);
    fc_enable_mpe(RGN_MPE0);
    fc_enable_regions();
    fc_rgn_lock();
#else
    fc_select_region(1);
    fc_disable_regions();
    fc_disable_mpe(RGN_MPE0);
    fc_prog_rgn(RGN_SIZE_8MB, HOST_SE_SECURE_FLASH_BASE_FVP);
    fc_init_mpl(RGN_MPE0);

    mpl_rights = (RGN_MPL_ANY_MST_MASK | RGN_MPL_SECURE_READ_MASK |
                                         RGN_MPL_SECURE_WRITE_MASK);

    fc_enable_mpl(RGN_MPE0, mpl_rights);
    fc_enable_mpe(RGN_MPE0);
    fc_enable_regions();
    fc_rgn_lock();
#endif

    fc_pe_enable();

    /* Always ON Host Peripherals */
    fc_select((void *)CORSTONE1000_HOST_FIREWALL_BASE, COMP_AONPERIPH);
    fc_disable_bypass();
    fc_pe_disable();

    for (int i = 0; i < COMP_AONPERIPH_PRE_CONFIGURED_RGNS; i++) {
        fc_select_region(i);
        fc_disable_regions();
        fc_disable_mpe(RGN_MPE0);
        fc_init_mpl(RGN_MPE0);

        mpl_rights = (RGN_MPL_ANY_MST_MASK | RGN_MPL_SECURE_READ_MASK |
                                             RGN_MPL_SECURE_WRITE_MASK |
                                             RGN_MPL_SECURE_EXECUTE_MASK |
                                             RGN_MPL_NONSECURE_READ_MASK |
                                             RGN_MPL_NONSECURE_WRITE_MASK |
                                             RGN_MPL_NONSECURE_EXECUTE_MASK);

        fc_enable_mpl(RGN_MPE0, mpl_rights);
        fc_enable_mpe(RGN_MPE0);
        fc_enable_regions();
        fc_rgn_lock();
    }

    fc_pe_enable();

    /* Host System Peripherals */
    fc_select((void *)CORSTONE1000_HOST_FIREWALL_BASE, COMP_SYSPERIPH);
    fc_disable_bypass();
    fc_pe_disable();

    for (int i = 0; i < COMP_SYSPERIPH_PRE_CONFIGURED_RGNS; i++) {
        fc_select_region(i);
        fc_disable_regions();
        fc_disable_mpe(RGN_MPE0);
        fc_init_mpl(RGN_MPE0);

        mpl_rights = (RGN_MPL_ANY_MST_MASK | RGN_MPL_SECURE_READ_MASK |
                                             RGN_MPL_SECURE_WRITE_MASK |
                                             RGN_MPL_SECURE_EXECUTE_MASK |
                                             RGN_MPL_NONSECURE_READ_MASK |
                                             RGN_MPL_NONSECURE_WRITE_MASK |
                                             RGN_MPL_NONSECURE_EXECUTE_MASK);

        fc_enable_mpl(RGN_MPE0, mpl_rights);
        fc_enable_mpe(RGN_MPE0);
        fc_enable_regions();
        fc_rgn_lock();
    }

    fc_pe_enable();

    /* Host System Peripherals */
    fc_select((void *)CORSTONE1000_HOST_FIREWALL_BASE, COMP_DBGPERIPH);
    fc_disable_bypass();
    fc_pe_disable();

    fc_select_region(0);
    fc_disable_regions();
    fc_disable_mpe(RGN_MPE0);
    fc_init_mpl(RGN_MPE0);

    mpl_rights = (RGN_MPL_ANY_MST_MASK | RGN_MPL_SECURE_READ_MASK |
                                         RGN_MPL_SECURE_WRITE_MASK |
                                         RGN_MPL_SECURE_EXECUTE_MASK |
                                         RGN_MPL_NONSECURE_READ_MASK |
                                         RGN_MPL_NONSECURE_WRITE_MASK |
                                         RGN_MPL_NONSECURE_EXECUTE_MASK);

    fc_enable_mpl(RGN_MPE0, mpl_rights);
    fc_enable_mpe(RGN_MPE0);
    fc_enable_regions();
    fc_rgn_lock();

    fc_pe_enable();

    if (fw_get_lockdown_status() == FW_UNLOCKED)
       fw_lockdown(FW_FULL_LOCKDOWN);
}


__attribute__((naked)) void boot_clear_bl2_ram_area(void)
{
    __ASM volatile(
        ".syntax unified                             \n"
        "movs    r0, #0                              \n"
        "ldr     r1, =Image$$ER_DATA$$Base           \n"
        "ldr     r2, =Image$$ARM_LIB_HEAP$$ZI$$Limit \n"
        "subs    r2, r2, r1                          \n"
        "Loop:                                       \n"
        "subs    r2, #4                              \n"
        "blt     Clear_done                          \n"
        "str     r0, [r1, r2]                        \n"
        "b       Loop                                \n"
        "Clear_done:                                 \n"
        "bx      lr                                  \n"
         : : : "r0" , "r1" , "r2" , "memory"
    );
}

extern void add_bank_offset_to_image_offset(uint32_t bank_offset);

int32_t boot_platform_init(void)
{
    int32_t result;
    uint32_t bank_offset;

    result = corstone1000_watchdog_init();
    if (result != ARM_DRIVER_OK) {
        return 1;
    }

#if !(PLATFORM_IS_FVP)
    setup_mpu();
#endif
    setup_se_firewall();
#if !(PLATFORM_IS_FVP)
    setup_host_firewall();
#endif

    result = FLASH_DEV_NAME.Initialize(NULL);
    if (result != ARM_DRIVER_OK) {
        return 1;
    }
#if PLATFORM_DEFAULT_OTP
   result = FLASH_DEV_NAME_SE_SECURE_FLASH.Initialize(NULL);
   if (result != ARM_DRIVER_OK) {
       return 1;
   }
#endif

#ifdef CRYPTO_HW_ACCELERATOR
    result = crypto_hw_accelerator_init();
    if (result) {
        return 1;
    }
#endif /* CRYPTO_HW_ACCELERATOR */

    result = tfm_plat_otp_init();
    if (result != TFM_PLAT_ERR_SUCCESS) {
        BOOT_LOG_ERR("OTP system initialization failed");
        FIH_PANIC;
    }

    if (tfm_plat_provisioning_is_required()) {
        result = fwu_metadata_provision();
        if (result != FWU_AGENT_SUCCESS) {
            BOOT_LOG_ERR("Provisioning FWU Metadata failed");
            FIH_PANIC;
        }
    }

    bl1_get_boot_bank(&bank_offset);
    add_bank_offset_to_image_offset(bank_offset);

    return 0;
}

void boot_platform_quit(struct boot_arm_vector_table *vt)
{
    /* Clang at O0, stores variables on the stack with SP relative addressing.
     * When manually set the SP then the place of reset vector is lost.
     * Static variables are stored in 'data' or 'bss' section, change of SP has
     * no effect on them.
     */
    static struct boot_arm_vector_table *vt_cpy;
    int32_t result;

#ifdef CRYPTO_HW_ACCELERATOR
    result = crypto_hw_accelerator_finish();
    if (result) {
        while (1);
    }

    (void)fih_delay_init();
#endif /* CRYPTO_HW_ACCELERATOR */

    result = FLASH_DEV_NAME.Uninitialize();
    if (result != ARM_DRIVER_OK) {
        while (1);
    }

#if PLATFORM_DEFAULT_OTP
    result = FLASH_DEV_NAME_SE_SECURE_FLASH.Uninitialize();
    if (result != ARM_DRIVER_OK) {
        while (1);
    }
#endif

    vt_cpy = vt;

    __set_MSP(vt->msp);
    __DSB();
    __ISB();

    boot_jump_to_next_image(vt_cpy->reset);
}
