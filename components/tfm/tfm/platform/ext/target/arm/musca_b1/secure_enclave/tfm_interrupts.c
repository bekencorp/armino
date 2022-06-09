/*
 * Copyright (c) 2019-2022, Arm Limited. All rights reserved.
 * Copyright (c) 2021, Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>

#include "cmsis.h"
#include "load/interrupt_defs.h"
#include "platform_multicore.h"
#include "spm_ipc.h"
#include "ffm/interrupt.h"

static struct irq_t mbox_irq_info = {0};

void MHU0_MSG_0_Handler(void)
{
    uint32_t magic;

    platform_mailbox_fetch_msg_data(&magic);

    /* Sanity check based on predefined arbitrary value */
    if (magic == PSA_CLIENT_CALL_REQ_MAGIC) {
        spm_handle_interrupt(mbox_irq_info.p_pt, mbox_irq_info.p_ildi);
    }
}

enum tfm_hal_status_t mailbox_irq_init(void *p_pt,
                                      struct irq_load_info_t *p_ildi)
{
    mbox_irq_info.p_pt = p_pt;
    mbox_irq_info.p_ildi = p_ildi;

    return TFM_HAL_SUCCESS;
}
