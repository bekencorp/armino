/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "platform_multicore.h"
#include "cmsis.h"
#include "mhu_v2_x.h"
#include "device_definition.h"

void platform_init_mailbox_hw(void)
{
    mhu_v2_x_driver_init(&MHU0_SENDER_DEV, MHU_REV_2_0);
    mhu_v2_x_driver_init(&MHU0_RECEIVER_DEV, MHU_REV_2_0);
}

void platform_mailbox_send_msg_data(uint32_t data)
{
    mhu_v2_x_initiate_transfer(&MHU0_SENDER_DEV);
    mhu_v2_x_channel_send(&MHU0_SENDER_DEV, 0, data);
    mhu_v2_x_close_transfer(&MHU0_SENDER_DEV);
}

void platform_mailbox_fetch_msg_data(uint32_t *data_ptr)
{
    mhu_v2_x_channel_receive(&MHU0_RECEIVER_DEV, 0, data_ptr);
    mhu_v2_x_channel_clear(&MHU0_RECEIVER_DEV, 0);

    NVIC_ClearPendingIRQ(PSA_CLIENT_CALL_NVIC_IRQn);
}

void platform_mailbox_fetch_msg_ptr(void **msg_ptr)
{
    mhu_v2_x_channel_receive(&MHU0_RECEIVER_DEV, 0, (uint32_t*)(msg_ptr));
    mhu_v2_x_channel_clear(&MHU0_RECEIVER_DEV, 0);

    NVIC_ClearPendingIRQ(PSA_CLIENT_CALL_NVIC_IRQn);
}

void platform_mailbox_wait_for_notify(void)
{
    while(NVIC_GetPendingIRQ(PSA_CLIENT_CALL_NVIC_IRQn) == 0);
}
