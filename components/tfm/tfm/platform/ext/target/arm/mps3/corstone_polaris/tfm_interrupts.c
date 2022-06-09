/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>

#include "cmsis.h"
#include "spm_ipc.h"
#include "tfm_hal_interrupt.h"
#include "tfm_peripherals_def.h"
#include "ffm/interrupt.h"
#include "load/interrupt_defs.h"

static struct irq_t timer0_irq = {0};

void TFM_TIMER0_IRQ_Handler(void)
{
    spm_handle_interrupt(timer0_irq.p_pt, timer0_irq.p_ildi);
}

enum tfm_hal_status_t tfm_timer0_irq_init(void *p_pt,
                                          struct irq_load_info_t *p_ildi)
{
    timer0_irq.p_ildi = p_ildi;
    timer0_irq.p_pt = p_pt;

    NVIC_SetPriority(TFM_TIMER0_IRQ, DEFAULT_IRQ_PRIORITY);
    NVIC_ClearTargetState(TFM_TIMER0_IRQ);
    NVIC_DisableIRQ(TFM_TIMER0_IRQ);

    return TFM_HAL_SUCCESS;
}
