/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>

#include "platform_multicore.h"
#include "tfm_ns_mailbox.h"
#include "tfm_plat_psa_proxy_addr_trans.h"

int32_t tfm_ns_mailbox_hal_notify_peer(void)
{
    platform_mailbox_send_msg_data(PSA_CLIENT_CALL_REQ_MAGIC);

    return MAILBOX_SUCCESS;
}

int32_t tfm_ns_mailbox_hal_init(struct ns_mailbox_queue_t *queue)
{
    uint32_t stage;

    if (!queue) {
        return MAILBOX_INVAL_PARAMS;
    }

    /* Wait until SPE mailbox library is ready to receive NSPE mailbox queue
     * address.
     */
    while (1) {
        platform_mailbox_wait_for_notify();

        platform_mailbox_fetch_msg_data(&stage);
        if (stage == NS_MAILBOX_INIT_ENABLE) {
            break;
        }
    }

    /* Send out the address */
    platform_mailbox_send_msg_ptr(
            (const void *)translate_addr_from_host_to_se(queue));

    /* Wait until SPE mailbox service is ready */
    while (1) {
        platform_mailbox_wait_for_notify();

        platform_mailbox_fetch_msg_data(&stage);
        if (stage == S_MAILBOX_READY) {
            break;
        }
    }

    return MAILBOX_SUCCESS;
}

void tfm_ns_mailbox_hal_enter_critical(void)
{
    /* Protection against concurrent access should be added
     * if more messages are sent parallel.
     */
}

void tfm_ns_mailbox_hal_exit_critical(void)
{
    /* Protection against concurrent access should be added
     * if more messages are sent parallel.
     */
}

void tfm_ns_mailbox_hal_enter_critical_isr(void)
{
    /* Protection against concurrent access should be added
     * if more messages are sent parallel.
     */
}

void tfm_ns_mailbox_hal_exit_critical_isr(void)
{
    /* Protection against concurrent access should be added
     * if more messages are sent parallel.
     */
}
