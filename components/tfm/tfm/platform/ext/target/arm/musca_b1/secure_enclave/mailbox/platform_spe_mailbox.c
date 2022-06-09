/*
 * Copyright (c) 2019-2020, Arm Limited. All rights reserved.
 * Copyright (c) 2019, Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "platform_multicore.h"
#include "tfm_spe_mailbox.h"
#include "cmsis.h"

static void mailbox_ipc_config(void)
{
    /* 2 NVIC bits present on this platform, setting the almost lowest priority. */
    NVIC_SetPriority(PSA_CLIENT_CALL_NVIC_IRQn, 2);

    NVIC_EnableIRQ(PSA_CLIENT_CALL_NVIC_IRQn);
}

int32_t tfm_mailbox_hal_notify_peer(void)
{
    platform_mailbox_send_msg_data(PSA_CLIENT_CALL_REPLY_MAGIC);

    return MAILBOX_SUCCESS;
}

int32_t tfm_mailbox_hal_init(struct secure_mailbox_queue_t *s_queue)
{
    struct ns_mailbox_queue_t *ns_queue = NULL;

    /* Inform NSPE that NSPE mailbox initialization can start */
    platform_mailbox_send_msg_data(NS_MAILBOX_INIT_ENABLE);

    platform_mailbox_wait_for_notify();

    /* Receive the address of NSPE mailbox queue */
    platform_mailbox_fetch_msg_ptr((void **)&ns_queue);

    /*
     * FIXME
     * Necessary sanity check of the address of NPSE mailbox queue should
     * be implemented there.
     */
    s_queue->ns_queue = ns_queue;

    mailbox_ipc_config();

    /* Inform NSPE that SPE mailbox service is ready */
    platform_mailbox_send_msg_data(S_MAILBOX_READY);

    return MAILBOX_SUCCESS;
}

void tfm_mailbox_hal_enter_critical(void)
{
    /* Protection against concurrent access should be added
     * if more messages are sent parallel. */
}

void tfm_mailbox_hal_exit_critical(void)
{
    /* Protection against concurrent access should be added
     * if more messages are sent parallel. */
}
