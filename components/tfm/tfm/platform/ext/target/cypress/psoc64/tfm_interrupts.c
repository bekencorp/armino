/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 * Copyright (c) 2021, Cypress Semiconductor Corp. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>

#include "cmsis.h"
#include "cy_ipc_drv.h"
#include "spe_ipc_config.h"
#include "spm_ipc.h"
#include "tfm_hal_interrupt.h"
#include "tfm_peripherals_def.h"
#include "ffm/interrupt.h"
#include "load/interrupt_defs.h"
#include "mailbox/platform_multicore.h"

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
    NVIC_DisableIRQ(TFM_TIMER0_IRQ);

    return TFM_HAL_SUCCESS;
}

static struct irq_t mbox_irq_info = {0};

static void mailbox_clear_intr(void)
{
    uint32_t status;

    status = Cy_IPC_Drv_GetInterruptStatusMasked(
                            Cy_IPC_Drv_GetIntrBaseAddr(IPC_RX_INTR_STRUCT));
    status >>= CY_IPC_NOTIFY_SHIFT;
    if ((status & IPC_RX_INT_MASK) == 0) {
        return;
    }

    Cy_IPC_Drv_ClearInterrupt(Cy_IPC_Drv_GetIntrBaseAddr(IPC_RX_INTR_STRUCT),
                              0, IPC_RX_INT_MASK);
}

void tfm_mailbox_irq_handler(void)
{
    uint32_t magic;

    mailbox_clear_intr();

    /* Read from mailbox */
    platform_mailbox_fetch_msg_data(&magic);
    if (magic == PSA_CLIENT_CALL_REQ_MAGIC) {
        spm_handle_interrupt(mbox_irq_info.p_pt, mbox_irq_info.p_ildi);
    }
}

enum tfm_hal_status_t mailbox_irq_init(void *p_pt,
                                       struct irq_load_info_t *p_ildi)
{
    mbox_irq_info.p_pt = p_pt;
    mbox_irq_info.p_ildi = p_ildi;

    NVIC_SetPriority(NvicMux7_IRQn, DEFAULT_IRQ_PRIORITY);
    NVIC_DisableIRQ(NvicMux7_IRQn);

    return TFM_HAL_SUCCESS;
}
#ifdef PSA_API_TEST_IPC

static struct irq_t ff_test_uart_irq;

void FF_TEST_UART_IRQ_Handler(void)
{
    spm_handle_interrupt(ff_test_uart_irq.p_pt, ff_test_uart_irq.p_ildi);
}

enum tfm_hal_status_t ff_test_uart_irq_init(void *p_pt,
                                            struct irq_load_info_t *p_ildi)
{
    ff_test_uart_irq.p_ildi = p_ildi;
    ff_test_uart_irq.p_pt = p_pt;

    NVIC_SetPriority(FF_TEST_UART_IRQ, DEFAULT_IRQ_PRIORITY);
    NVIC_ClearTargetState(FF_TEST_UART_IRQ);
    NVIC_DisableIRQ(FF_TEST_UART_IRQ);

    return TFM_HAL_SUCCESS;
}

#endif
