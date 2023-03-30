/*
 * Copyright (c) 2022, Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>
#include "ffm/backend.h"
#include "ffm/stack_watermark.h"
#include "lists.h"
#include "load/spm_load_api.h"
#include "spm_ipc.h"
#include "tfm_spm_log.h"

/* Always output, regardless of log level.
 * If you don't want output, don't build this code
 */
#define SPMLOG(x) tfm_hal_output_spm_log((x), sizeof(x))
#define SPMLOG_VAL(x, y) spm_log_msgval((x), sizeof(x), y)

#define STACK_WATERMARK_VAL 0xdeadbeef

void watermark_stack(struct partition_t *p_pt)
{
    const struct partition_load_info_t *p_pldi = p_pt->p_ldinf;

    for (int i = 0; i < p_pldi->stack_size / 4; i++) {
        *((uint32_t *)LOAD_ALLOCED_STACK_ADDR(p_pldi) + i) = STACK_WATERMARK_VAL;
    }
}

/* Returns the number of bytes of stack that have been used by the specified partition */
static uint32_t used_stack(struct partition_t *p_pt)
{
    const struct partition_load_info_t *p_pldi = p_pt->p_ldinf;
    uint32_t unused_words = 0;

    for (const uint32_t *p = (uint32_t *)LOAD_ALLOCED_STACK_ADDR(p_pldi);
         p < (uint32_t *)(LOAD_ALLOCED_STACK_ADDR(p_pldi) + p_pldi->stack_size);
         p++) {
        if (*p != STACK_WATERMARK_VAL) {
            break;
        }
        unused_words++;
    }

    return p_pldi->stack_size - (unused_words * 4);
}

void dump_used_stacks(void)
{
    struct partition_t *p_pt;

    SPMLOG("Used stack sizes report\r\n");
    UNI_LIST_FOREACH(p_pt, PARTITION_LIST_ADDR, next) {
        SPMLOG_VAL("  Partition id: ", p_pt->p_ldinf->pid);
        SPMLOG_VAL("    Stack bytes: ", p_pt->p_ldinf->stack_size);
        SPMLOG_VAL("    Stack bytes used: ", used_stack(p_pt));
    }
}
