/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "platform_multicore.h"
#include "cmsis.h"
#include "mhu_v2_x.h"
#include "device_definition.h"
#include "tfm_plat_psa_proxy_addr_trans.h"

#define SE_MEM_BASE         (0x38000000)
#define HOST_MEM_BASE       (0x1A200000)

int tfm_platform_ns_wait_for_s_cpu_ready(void)
{
    mhu_v2_x_driver_init(&SE_MHU_SENDER_DEV, MHU_REV_2_0);
    mhu_v2_x_driver_init(&SE_MHU_RECEIVER_DEV, MHU_REV_2_0);

    platform_mailbox_send_msg_data(IPC_SYNC_MAGIC);

    return PLATFORM_MAILBOX_SUCCESS;
}

int platform_mailbox_send_msg_data(uint32_t data)
{
    mhu_v2_x_initiate_transfer(&SE_MHU_SENDER_DEV);
    mhu_v2_x_channel_send(&SE_MHU_SENDER_DEV, 0, data);
    mhu_v2_x_close_transfer(&SE_MHU_SENDER_DEV);

    return PLATFORM_MAILBOX_SUCCESS;
}

int platform_mailbox_send_msg_ptr(const void *msg_ptr)
{
    mhu_v2_x_initiate_transfer(&SE_MHU_SENDER_DEV);
    mhu_v2_x_channel_send(&SE_MHU_SENDER_DEV, 0, (uint32_t)msg_ptr);
    mhu_v2_x_close_transfer(&SE_MHU_SENDER_DEV);

    return PLATFORM_MAILBOX_SUCCESS;
}

void platform_mailbox_wait_for_notify(void)
{
    while(NVIC_GetPendingIRQ(HostMHUR0_IRQComb_IRQn) == 0);
}

int platform_mailbox_fetch_msg_data(uint32_t *data_ptr)
{
    mhu_v2_x_channel_receive(&SE_MHU_RECEIVER_DEV, 0, data_ptr);
    mhu_v2_x_channel_clear(&SE_MHU_RECEIVER_DEV, 0);

    NVIC_ClearPendingIRQ(HostMHUR0_IRQComb_IRQn);

    return PLATFORM_MAILBOX_SUCCESS;
}

void * translate_addr_from_host_to_se(void *addr)
{
    return (addr + SE_MEM_BASE - HOST_MEM_BASE);
}

void * translate_addr_from_se_to_host(void *addr)
{
    return (addr + HOST_MEM_BASE - SE_MEM_BASE);
}
