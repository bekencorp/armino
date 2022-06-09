/*
 * Copyright (c) 2018-2021, Arm Limited. All rights reserved.
 * Copyright (c) 2019-2020, Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>

#include "tfm_hal_multi_core.h"
#include "platform_multicore.h"

void tfm_hal_boot_ns_cpu(uintptr_t start_addr)
{
    uint32_t translated_address = 0;

    platform_init_mailbox_hw();

    translated_address = (uint32_t) start_addr
            + 0x1A200000 /* eFlash 1 base from SSE-200's point of view */
            - 0x38000000 /* eFlash 1 base from SE's point of view */;

    platform_mailbox_send_msg_data(translated_address);
}

void tfm_hal_wait_for_ns_cpu_ready(void)
{
    uint32_t data = 0;

    while (data != IPC_SYNC_MAGIC) {
        platform_mailbox_wait_for_notify();
        platform_mailbox_fetch_msg_data(&data);
    }
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

}

void tfm_hal_get_ns_access_attr(const void *p, size_t s,
                                struct mem_attr_info_t *p_attr)
{
    /* Check static memory layout to get memory attributes */
    tfm_get_ns_mem_region_attr(p, s, p_attr);
}
