/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __BACKEND_H__
#define __BACKEND_H__

#include <stdint.h>
#include "spm_ipc.h"
#include "tfm_arch.h"
#include "load/spm_load_api.h"
#include "psa/error.h"

#if CONFIG_TFM_SPM_BACKEND_IPC == 1
#include "backend_ipc.h"
#elif CONFIG_TFM_SPM_BACKEND_SFN == 1
#include "backend_sfn.h"
#else
#error "No backend selected, check configurations."
#endif

/*
 * Runtime model-specific component initialization routine. This
 * is an `assuredly` function, would panic if any error occurred.
 */
void backend_init_comp_assuredly(struct partition_t *p_pt,
                                 uint32_t service_setting);

/*
 * Runtime model-specific kick-off method for the whole system.
 * Returns a hardware-specific control value, which is transparent
 * to SPM common logic.
 */
uint32_t backend_system_run(void);

/* Runtime model-specific message handling mechanism. */
psa_status_t backend_messaging(struct service_t *p_serv,
                               struct conn_handle_t *handle);

/*
 * Runtime model-specific message replying.
 * Return the connection handle or the acked status code.
 */
psa_status_t backend_replying(struct conn_handle_t *handle, int32_t status);

/*
 * Runtime model-specific Partition wait operation.
 * Put the Partition to a status that waits for signals.
 */
psa_signal_t backend_wait(struct partition_t *p_pt, psa_signal_t signal_mask);

/*
 * Runtime model-specific Partition wake up operation.
 * Wakes up the Partition with the asserted signals in 'p_pt'.
 */
void backend_wake_up(struct partition_t *p_pt);


/* The component list, and a MACRO indicate this is not a common global. */
extern struct partition_head_t partition_listhead;
#define PARTITION_LIST_ADDR (&partition_listhead)

/* TODO: Put this into NS Agent related service when available. */
extern struct context_ctrl_t *p_spm_thread_context;
#define SPM_THREAD_CONTEXT p_spm_thread_context

#endif /* __BACKEND_H__ */
