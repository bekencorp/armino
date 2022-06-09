/*
 * Copyright (c) 2018-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "device_definition.h"
#include "flash_common.h"
#include "platform_base_address.h"
#include "tfm_hal_multi_core.h"
#include "fwu_agent.h"

#define HOST_SYS_RST_CTRL_OFFSET 0x0
#define HOST_CPU_CORE0_WAKEUP_OFFSET 0x308
#define HOST_CPU_PE0_CONFIG_OFFSET 0x010
#define AA64nAA32_MASK (1 << 3)

void tfm_hal_boot_ns_cpu(uintptr_t start_addr)
{
    /* Switch the shared flash to XiP mode for the host */
    Select_XIP_Mode_For_Shared_Flash();

    volatile uint32_t *bir_base = (uint32_t *)CORSTONE1000_HOST_BIR_BASE;

    /* Program Boot Instruction Register to jump to BL2 (TF-A) base address
     * at 0x02354000, corresponding assembler instructions are as below.
     * 0x58000040     ldr       x0, Label
     * 0xD61F0000     br        x0
     * 0x02354000     Label:    .dword 0x020d3000
     */
    bir_base[0] = 0x58000040;
    bir_base[1] = 0xD61F0000;
    bir_base[2] = 0x02354000;

    volatile uint32_t *reset_ctl_reg = (uint32_t *)(CORSTONE1000_BASE_SCR_BASE
                                                    + HOST_SYS_RST_CTRL_OFFSET);
    volatile uint32_t *reset_ctl_wakeup_reg =
                        (uint32_t *)(CORSTONE1000_HOST_BASE_SYSTEM_CONTROL_BASE
                                     + HOST_CPU_CORE0_WAKEUP_OFFSET);

    volatile uint32_t *PE0_CONFIG =
                        (uint32_t *)(CORSTONE1000_HOST_BASE_SYSTEM_CONTROL_BASE
                                     + HOST_CPU_PE0_CONFIG_OFFSET);

    /* Select host CPU architecture as AArch64 */
    *PE0_CONFIG |= AA64nAA32_MASK; /* 0b1 – AArch64 */

    /* wakeup CORE0 before bringing it out of reset */
    *reset_ctl_wakeup_reg = 0x1;

    /* Clear HOST_SYS_RST_CTRL register to bring host out of RESET */
    *reset_ctl_reg = 0;

    (void) start_addr;
}

void tfm_hal_wait_for_ns_cpu_ready(void)
{
#if !(PLATFORM_IS_FVP)
    /* start the reset timer if firwmare update process is ongoing */
    host_acknowledgement_timer_to_reset();
#endif
}

void tfm_hal_get_mem_security_attr(const void *p, size_t s,
                                   struct security_attr_info_t *p_attr)
{
    /* Check static memory layout to get memory attributes */
    tfm_get_mem_region_security_attr(p, s, p_attr);
}

void tfm_hal_get_secure_access_attr(const void *p, size_t s,
                                    struct mem_attr_info_t *p_attr)
{
    /* Check static memory layout to get memory attributes */
    tfm_get_secure_mem_region_attr(p, s, p_attr);
#if TFM_LVL >= 2
    p_attr->is_mpu_enabled = true;
#endif
}

void tfm_hal_get_ns_access_attr(const void *p, size_t s,
                                struct mem_attr_info_t *p_attr)
{
    /* Check static memory layout to get memory attributes */
    tfm_get_ns_mem_region_attr(p, s, p_attr);
}
