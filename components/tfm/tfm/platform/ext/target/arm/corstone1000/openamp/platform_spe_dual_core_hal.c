/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 * Copyright (c) 2021, Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "tfm_spe_openamp_platform_interface.h"
#include "device_cfg.h"
#include "device_definition.h"
#include "load/interrupt_defs.h"
#include "mhu_v2_x.h"
#include "tfm_plat_defs.h"
#include "tfm_spm_log.h"
#include "cmsis.h"

#define MHU1_SEH_NOTIFY_CH 0
#define MHU1_SEH_NOTIFY_VAL 1234

static enum tfm_plat_err_t initialize_secure_enclave_to_host_mhu(void)
{
   enum mhu_v2_x_error_t status;

   status = mhu_v2_x_driver_init(&MHU1_SE_TO_HOST_DEV, MHU_REV_READ_FROM_HW);
   if (status != MHU_V_2_X_ERR_NONE) {
       SPMLOG_ERRMSGVAL("Secure-enclave to Host MHU driver initialization failed: ", status);
       return TFM_PLAT_ERR_SYSTEM_ERR;
   }
   SPMLOG_INFMSG("Secure-enclave to Host MHU Driver initialized successfully.\r\n");

   return TFM_PLAT_ERR_SUCCESS;
}

static enum tfm_plat_err_t initialize_host_to_secure_enclave_mhu(void)
{
   enum mhu_v2_x_error_t status;

   status = mhu_v2_x_driver_init(&MHU1_HOST_TO_SE_DEV, MHU_REV_READ_FROM_HW);
   if (status != MHU_V_2_X_ERR_NONE) {
       SPMLOG_ERRMSGVAL("Host to secure-enclave MHU driver initialization failed: ", status);
       return TFM_PLAT_ERR_SYSTEM_ERR;
   }
   SPMLOG_INFMSG("Host to secure-enclave MHU Driver initialized successfully.\r\n");

   NVIC_EnableIRQ(HSE1_RECEIVER_COMBINED_IRQn);

   return TFM_PLAT_ERR_SUCCESS;
}

static struct irq_t mbox_irq_info = {0};

void HSE1_RECEIVER_COMBINED_IRQHandler(void)
{
   spm_handle_interrupt(mbox_irq_info.p_pt, mbox_irq_info.p_ildi);

   mhu_v2_x_channel_clear(&MHU1_HOST_TO_SE_DEV, 0);
   NVIC_ClearPendingIRQ(HSE1_RECEIVER_COMBINED_IRQn);
}

enum tfm_hal_status_t mailbox_irq_init(void *p_pt,
                                       struct irq_load_info_t *p_ildi)
{
    mbox_irq_info.p_pt = p_pt;
    mbox_irq_info.p_ildi = p_ildi;

    return TFM_HAL_SUCCESS;
}

enum tfm_plat_err_t tfm_dual_core_hal_init(void)
{
    enum tfm_plat_err_t status;

    status = initialize_host_to_secure_enclave_mhu();
    if (status) {
        return status;
    }
    status = initialize_secure_enclave_to_host_mhu();

    return status;
}

enum tfm_plat_err_t tfm_hal_notify_peer(void)
{
    uint32_t access_ready;
    enum mhu_v2_x_error_t status;
    struct mhu_v2_x_dev_t* dev = &MHU1_SE_TO_HOST_DEV;

    status = mhu_v2_x_set_access_request(dev);
    if (status != MHU_V_2_X_ERR_NONE) {
        SPMLOG_ERRMSGVAL("mhu_v2_x_set_access_request failed : ", status);
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }

    do {
        status = mhu_v2_x_get_access_ready(dev, &access_ready);
        if (status != MHU_V_2_X_ERR_NONE) {
            SPMLOG_ERRMSGVAL("mhu_v2_x_get_access_ready failed : ", status);
            return TFM_PLAT_ERR_SYSTEM_ERR;
        }
    } while(!access_ready);

    status = mhu_v2_x_channel_send(dev, MHU1_SEH_NOTIFY_CH, MHU1_SEH_NOTIFY_VAL);

    if (status != MHU_V_2_X_ERR_NONE) {
        SPMLOG_ERRMSGVAL("mhu_v2_x_channel_send : ", status);
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }

    status = mhu_v2_x_reset_access_request(dev);
    if (status != MHU_V_2_X_ERR_NONE) {
        SPMLOG_ERRMSGVAL("mhu_v2_x_reset_access_request : ", status);
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }
    return TFM_PLAT_ERR_SUCCESS;
}

/*
 * The function is implemented to support libmetal's mutex and spinlock
 * implementation. The GCC does not support a respective builtin
 * functions for Cortex M0+. So below function provides the
 * missing link for libmetal compilation.
 * This function will prevent race condition between PendSV context (where
 * entries are inserted into unordered map) and service threads (where
 * entries are removed from the unordered map).
 */
bool  __atomic_compare_exchange_4(volatile void *mem, void *expected,
        uint32_t desired, bool var, int success, int failure)
{
    bool ret = false;
    volatile uint32_t *location = mem;
    volatile uint32_t *old_val = expected;
    /* unused variables */
    (void)var;
    (void)success;
    (void)failure;

    NVIC_DisableIRQ(PendSV_IRQn);

    do {
        if (*location != *old_val) {
            break;
        }
        *location = desired;
        ret = true;
    } while (0);

    NVIC_EnableIRQ(PendSV_IRQn);

    return ret;
}
