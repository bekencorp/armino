/*
 * Copyright (c) 2018-2021, Arm Limited. All rights reserved.
 * Copyright (c) 2019-2021, Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "cy_ipc_drv.h"
#include "cy_p64_watchdog.h"
#include "cy_prot.h"
#include "driver_dap.h"
#include "pc_config.h"
#include "spe_ipc_config.h"
#include "target_cfg.h"
#include "tfm_plat_defs.h"
#include "tfm_spm_log.h"
#include "tfm_hal_multi_core.h"

static enum tfm_plat_err_t handle_boot_wdt(void)
{
    /* Update watchdog timer to mark successfull start up of the image */
    SPMLOG_INFMSG("Checking boot watchdog\r\n");
    if (cy_p64_wdg_is_enabled()) {
        cy_p64_wdg_stop();
        cy_p64_wdg_free();
        SPMLOG_INFMSG("Disabled boot watchdog\r\n");
    }

    return TFM_PLAT_ERR_SUCCESS;
}

void tfm_hal_boot_ns_cpu(uintptr_t start_addr)
{
    smpu_print_config();

    /* Reset boot watchdog */
    handle_boot_wdt();

    if (cy_access_port_control(CY_CM4_AP, CY_AP_EN) == 0) {
        /* The delay is required after Access port was enabled for
        * debugger/programmer to connect and set TEST BIT */
        Cy_SysLib_Delay(100);
        SPMLOG_INFMSG("Enabled CM4_AP DAP control\r\n");
    }

    SPMLOG_INFMSGVAL("Starting Cortex-M4 at ", start_addr);
    Cy_SysEnableCM4(start_addr);
}

void tfm_hal_wait_for_ns_cpu_ready(void)
{
    uint32_t data;
    cy_en_ipcdrv_status_t status;

    Cy_IPC_Drv_SetInterruptMask(Cy_IPC_Drv_GetIntrBaseAddr(IPC_RX_INTR_STRUCT),
                                0, IPC_RX_INT_MASK);

    status = Cy_IPC_Drv_SendMsgWord(Cy_IPC_Drv_GetIpcBaseAddress(IPC_TX_CHAN),
                                 IPC_TX_NOTIFY_MASK, IPC_SYNC_MAGIC);
    while (1)
    {
        status = (cy_en_ipcdrv_status_t) Cy_IPC_Drv_GetInterruptStatusMasked(
                        Cy_IPC_Drv_GetIntrBaseAddr(IPC_RX_INTR_STRUCT));
        status >>= CY_IPC_NOTIFY_SHIFT;
        if (status & IPC_RX_INT_MASK) {
            Cy_IPC_Drv_ClearInterrupt(Cy_IPC_Drv_GetIntrBaseAddr(
                                      IPC_RX_INTR_STRUCT),
                                      0, IPC_RX_INT_MASK);

            status = Cy_IPC_Drv_ReadMsgWord(Cy_IPC_Drv_GetIpcBaseAddress(
                                            IPC_RX_CHAN),
                                            &data);
            if (status == CY_IPC_DRV_SUCCESS) {
                Cy_IPC_Drv_ReleaseNotify(Cy_IPC_Drv_GetIpcBaseAddress(IPC_RX_CHAN),
                                         IPC_RX_RELEASE_MASK);
                if (data == ~IPC_SYNC_MAGIC) {
                    SPMLOG_INFMSG("Cores sync success.\r\n");
                    break;
                }
            }
        }
    }
}

void tfm_hal_get_mem_security_attr(const void *p, size_t s,
                                   struct security_attr_info_t *p_attr)
{
    /*
     * FIXME
     * Need to check if the memory region is valid according to platform
     * specific memory mapping.
     */

    /*
     * TODO
     * Currently only check static memory region layout to get security
     * information.
     * Check of hardware SMPU configuration can be added.
     */
    tfm_get_mem_region_security_attr(p, s, p_attr);
}

void tfm_hal_get_secure_access_attr(const void *p, size_t s,
                                    struct mem_attr_info_t *p_attr)
{
    uint32_t pc;

    /*
     * FIXME
     * Need to check if the memory region is valid according to platform
     * specific memory mapping.
     */

    /*
     * TODO
     * Currently only check static memory region layout to get attributes.
     * Check of secure memory protection configuration from hardware can be
     * added.
     */
    tfm_get_secure_mem_region_attr(p, s, p_attr);

    pc = Cy_Prot_GetActivePC(CPUSS_MS_ID_CM0);
    /* Check whether the current active PC is configured as the expected one .*/
    if (pc == CY_PROT_SPM_DEFAULT) {
        p_attr->is_mpu_enabled = true;
    } else {
        p_attr->is_mpu_enabled = false;
    }
}

void tfm_hal_get_ns_access_attr(const void *p, size_t s,
                                struct mem_attr_info_t *p_attr)
{
    /*
     * FIXME
     * Need to check if the memory region is valid according to platform
     * specific memory mapping.
     */

    /*
     * TODO
     * Currently only check static memory region layout to get attributes.
     * Check of non-secure memory protection configuration from hardware can be
     * added.
     */
    tfm_get_ns_mem_region_attr(p, s, p_attr);
}
