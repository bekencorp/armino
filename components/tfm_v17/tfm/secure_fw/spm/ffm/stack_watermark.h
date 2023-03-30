/*
 * Copyright (c) 2022, Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __STACK_WATERMARK_H__
#define __STACK_WATERMARK_H__

#include "spm_ipc.h"

#ifdef CONFIG_TFM_STACK_WATERMARKS
void watermark_stack(struct partition_t *p_pt);
void dump_used_stacks(void);
#else
#define watermark_stack(p_pt)
#define dump_used_stacks()
#endif

#endif /* __STACK_WATERMARK_H__ */
