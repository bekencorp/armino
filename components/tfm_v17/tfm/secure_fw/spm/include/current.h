/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifndef __CURRENT_H__
#define __CURRENT_H__

#include "thread.h"
#include "spm_ipc.h"

#if CONFIG_TFM_SPM_BACKEND_SFN != 1
/* Get current component */
#define GET_CURRENT_COMPONENT()  GET_CTX_OWNER(CURRENT_THREAD->p_context_ctrl)
/* Set current component */
#define SET_CURRENT_COMPONENT(p) THRD_UPDATE_CUR_CTXCTRL(&(p)->ctx_ctrl)
#else
extern struct partition_t *p_current_partition;
/* Get current component */
#define GET_CURRENT_COMPONENT()  p_current_partition
/* Set current component */
#define SET_CURRENT_COMPONENT(p) p_current_partition = p
#endif

#endif /* __CURRENT_H__ */
