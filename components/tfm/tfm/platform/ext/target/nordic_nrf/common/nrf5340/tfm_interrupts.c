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
#include "tfm_peripherals_config.h"
#include "load/interrupt_defs.h"
#include "ffm/interrupt.h"
#include "tfm_spm_hal.h"


static enum tfm_hal_status_t irq_init(struct irq_t *irq, IRQn_Type irqn,
                                      void * p_pt,
                                      struct irq_load_info_t *p_ildi)
{
    irq->p_ildi = p_ildi;
    irq->p_pt = p_pt;

    NVIC_SetPriority(irqn, DEFAULT_IRQ_PRIORITY);
    NVIC_ClearTargetState(irqn);
    NVIC_DisableIRQ(irqn);

    return TFM_HAL_SUCCESS;
}

#if TFM_PERIPHERAL_FPU_SECURE
static struct irq_t fpu_irq = {0};

void FPU_IRQHandler(void)
{
    spm_handle_interrupt(fpu_irq.p_pt, fpu_irq.p_ildi);
}

enum tfm_hal_status_t tfm_fpu_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
{
    return irq_init(&fpu_irq, TFM_FPU_IRQ, p_pt, p_ildi);
}
#endif

#if TFM_PERIPHERAL_CLOCK_SECURE || TFM_PERIPHERAL_POWER_SECURE
static struct irq_t clock_power_irq = {0};

void CLOCK_POWER_IRQHandler(void)
{
    spm_handle_interrupt(clock_power_irq.p_pt, clock_power_irq.p_ildi);
}

enum tfm_hal_status_t tfm_clock_power_irq_init(void *p_pt,
                                               struct irq_load_info_t *p_ildi)
{
    return irq_init(&clock_power_irq, TFM_CLOCK_POWER_IRQ, p_pt, p_ildi);
}

enum tfm_hal_status_t tfm_clock_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
__attribute__((alias("tfm_clock_power_irq_init")));

enum tfm_hal_status_t tfm_power_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
__attribute__((alias("tfm_clock_power_irq_init")));
#endif

#if TFM_PERIPHERAL_SPIM0_SECURE || TFM_PERIPHERAL_SPIS0_SECURE || \
    TFM_PERIPHERAL_TWIM0_SECURE || TFM_PERIPHERAL_TWIS0_SECURE || \
    TFM_PERIPHERAL_UARTE0_SECURE
static struct irq_t serial0_irq = {0};

void SERIAL0_IRQHandler(void)
{
    spm_handle_interrupt(serial0.p_pt, serial0.p_ildi);
}

enum tfm_hal_status_t tfm_serial0_init(void *p_pt,
                                        struct irq_load_info_t *p_ildi)
{
    return irq_init(&serial0_irq, TFM_SERIAL0_IRQ, p_pt, p_ildi);
}

enum tfm_hal_status_t tfm_spim0_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
__attribute__((alias("tfm_serial0_init")));

enum tfm_hal_status_t tfm_spis0_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
__attribute__((alias("tfm_serial0_init")));

enum tfm_hal_status_t tfm_twim0_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
__attribute__((alias("tfm_serial0_init")));

enum tfm_hal_status_t tfm_twis0_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
__attribute__((alias("tfm_serial0_init")));

enum tfm_hal_status_t tfm_uarte0_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
__attribute__((alias("tfm_serial0_init")));
#endif

#if TFM_PERIPHERAL_SPIM1_SECURE || TFM_PERIPHERAL_SPIS1_SECURE || \
    TFM_PERIPHERAL_TWIM1_SECURE || TFM_PERIPHERAL_TWIS1_SECURE || \
    TFM_PERIPHERAL_UARTE1_SECURE
static struct irq_t serial1_irq = {0};

void SERIAL1_IRQHandler(void)
{
    spm_handle_interrupt(serial1_irq.p_pt, serial1_irq.p_ildi);
}

enum tfm_hal_status_t tfm_serial1_init(void *p_pt,
                                        struct irq_load_info_t *p_ildi)
{
    return irq_init(&serial1_irq, TFM_SERIAL1_IRQ, p_pt, p_ildi);
}

enum tfm_hal_status_t tfm_spim1_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
__attribute__((alias("tfm_serial1_init")));

enum tfm_hal_status_t tfm_spis1_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
__attribute__((alias("tfm_serial1_init")));

enum tfm_hal_status_t tfm_twim1_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
__attribute__((alias("tfm_serial1_init")));

enum tfm_hal_status_t tfm_twis1_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
__attribute__((alias("tfm_serial1_init")));

enum tfm_hal_status_t tfm_uarte1_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
__attribute__((alias("tfm_serial1_init")));
#endif

#if TFM_PERIPHERAL_SPIM2_SECURE || TFM_PERIPHERAL_SPIS2_SECURE || \
    TFM_PERIPHERAL_TWIM2_SECURE || TFM_PERIPHERAL_TWIS2_SECURE || \
    TFM_PERIPHERAL_UARTE2_SECURE
static struct irq_t serial2_irq = {0};

void SERIAL2_IRQHandler(void)
{
    spm_handle_interrupt(serial2_irq.p_pt, serial2_irq.p_ildi);
}

enum tfm_hal_status_t tfm_serial2_init(void *p_pt,
                                        struct irq_load_info_t *p_ildi)
{
    return irq_init(&serial2_irq, TFM_SERIAL2_IRQ, p_pt, p_ildi);
}

enum tfm_hal_status_t tfm_spim2_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
__attribute__((alias("tfm_serial2_init")));

enum tfm_hal_status_t tfm_spis2_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
__attribute__((alias("tfm_serial2_init")));

enum tfm_hal_status_t tfm_twim2_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
__attribute__((alias("tfm_serial2_init")));

enum tfm_hal_status_t tfm_twis2_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
__attribute__((alias("tfm_serial2_init")));

enum tfm_hal_status_t tfm_uarte2_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
__attribute__((alias("tfm_serial2_init")));
#endif

#if TFM_PERIPHERAL_SPIM3_SECURE || TFM_PERIPHERAL_SPIS3_SECURE || \
    TFM_PERIPHERAL_TWIM3_SECURE || TFM_PERIPHERAL_TWIS3_SECURE || \
    TFM_PERIPHERAL_UARTE3_SECURE
static struct irq_t serial3_irq = {0};

void SERIAL3_IRQHandler(void)
{
    spm_handle_interrupt(serial3_irq.p_pt, serial3_irq.p_ildi);
}

enum tfm_hal_status_t tfm_serial3_init(void *p_pt,
                                        struct irq_load_info_t *p_ildi)
{
    return irq_init(&serial3_irq, TFM_SERIAL3_IRQ, p_pt, p_ildi);
}

enum tfm_hal_status_t tfm_spim3_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
__attribute__((alias("tfm_serial3_init")));

enum tfm_hal_status_t tfm_spis3_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
__attribute__((alias("tfm_serial3_init")));

enum tfm_hal_status_t tfm_twim3_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
__attribute__((alias("tfm_serial3_init")));

enum tfm_hal_status_t tfm_twis3_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
__attribute__((alias("tfm_serial3_init")));

enum tfm_hal_status_t tfm_uarte3_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
__attribute__((alias("tfm_serial3_init")));
#endif

#if TFM_PERIPHERAL_SPIM4_SECURE
static struct irq_t spim4_irq = {0};

void SPIM4_IRQHandler(void)
{
    spm_handle_interrupt(spim4_irq.p_pt, spim4_irq.p_ildi);
}

enum tfm_hal_status_t tfm_spim4_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
{
    return irq_init(&spim4_irq, TFM_SPIM4_IRQ, p_pt, p_ildi);
}
#endif

#if TFM_PERIPHERAL_GPIOTE0_SECURE
static struct irq_t gpiote0_irq = {0};

void GPIOTE0_IRQHandler(void)
{
    spm_handle_interrupt(gpiote0_irq.p_pt, gpiote0_irq.p_ildi);
}

enum tfm_hal_status_t tfm_gpiote0_irq_init(void *p_pt,
                                           struct irq_load_info_t *p_ildi)
{
    return irq_init(&gpiote0_irq, TFM_GPIOTE0_IRQ, p_pt, p_ildi);
}
#endif

#if TFM_PERIPHERAL_SAADC_SECURE
static struct irq_t saadc_irq = {0};

void SAADC_IRQHandler(void)
{
    spm_handle_interrupt(saadc_irq.p_pt, saadc_irq.p_ildi);
}

enum tfm_hal_status_t tfm_saadc_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
{
    return irq_init(&saadc_irq, TFM_SAADC_IRQ, p_pt, p_ildi);
}
#endif

#if TFM_PERIPHERAL_TIMER0_SECURE
static struct irq_t timer0_irq = {0};

void TIMER0_IRQHandler(void)
{
    spm_handle_interrupt(timer0_irq.p_pt, timer0_irq.p_ildi);
}

enum tfm_hal_status_t tfm_timer0_irq_init(void *p_pt,
                                          struct irq_load_info_t *p_ildi)
{
    return irq_init(&timer0_irq, TFM_TIMER0_IRQ, p_pt, p_ildi);
}
#endif

#if TFM_PERIPHERAL_TIMER1_SECURE
static struct irq_t timer1_irq = {0};

void TIMER1_IRQHandler(void)
{
    spm_handle_interrupt(timer1_irq.p_pt, timer1_irq.p_ildi);
}

enum tfm_hal_status_t tfm_timer1_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
{
    return irq_init(&timer1_irq, TFM_TIMER1_IRQ, p_pt, p_ildi);
}
#endif

#if TFM_PERIPHERAL_TIMER2_SECURE
static struct irq_t timer2_irq = {0};

void TIMER2_IRQHandler(void)
{
    spm_handle_interrupt(timer2_irq.p_pt, timer2_irq.p_ildi);
}

enum tfm_hal_status_t tfm_timer2_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
{
    return irq_init(&timer2_irq, TFM_TIMER2_IRQ, p_pt, p_ildi);
}
#endif

#if TFM_PERIPHERAL_RTC0_SECURE
static struct irq_t rtc0_irq = {0};

void RTC0_IRQHandler(void)
{
    spm_handle_interrupt(rtc0_irq.p_pt, rtc0_irq.p_ildi);
}

enum tfm_hal_status_t tfm_rtc0_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
{
    return irq_init(&rtc0_irq, TFM_RTC0_IRQ, p_pt, p_ildi);
}
#endif

#if TFM_PERIPHERAL_RTC1_SECURE
static struct irq_t rtc1_irq = {0};

void RTC1_IRQHandler(void)
{
    spm_handle_interrupt(rtc1_irq.p_pt, rtc1_irq.p_ildi);
}

enum tfm_hal_status_t tfm_rtc1_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
{
    return irq_init(&rtc1_irq, TFM_RTC1_IRQ, p_pt, p_ildi);
}
#endif

#if TFM_PERIPHERAL_WDT0_SECURE
static struct irq_t wdt0_irq = {0};

void WDT0_IRQHandler(void)
{
    spm_handle_interrupt(wdt0_irq.p_pt, wdt0_irq.p_ildi);
}

enum tfm_hal_status_t tfm_wdt0_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
{
    return irq_init(&wdt0_irq, TFM_WDT0_IRQ, p_pt, p_ildi);
}
#endif

#if TFM_PERIPHERAL_WDT1_SECURE
static struct irq_t wdt1_irq = {0};

void WDT1_IRQHandler(void)
{
    spm_handle_interrupt(wdt1_irq.p_pt, wdt1_irq.p_ildi);
}

enum tfm_hal_status_t tfm_wdt1_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
{
    return irq_init(&wdt1_irq, TFM_WDT1_IRQ, p_pt, p_ildi);
}
#endif

#if  TFM_PERIPHERAL_COMP_SECURE || TFM_PERIPHERAL_LPCOMP_SECURE
static struct irq_t comp_lpcomp_irq = {0};

void COMP_LPCOMP_IRQHandler(void)
{
    spm_handle_interrupt(comp_lpcomp_irq.p_pt, comp_lpcomp_irq.p_ildi);
}

enum tfm_hal_status_t tfm_comp_lpcomp_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
{
    return irq_init(&comp_lpcomp_irq, TFM_COMP_LPCOMP_IRQ, p_pt, p_ildi);
}

enum tfm_hal_status_t tfm_lpcomp_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
__attribute__((alias("tfm_comp_lpcomp_irq_init")));

enum tfm_hal_status_t tfm_comp_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
__attribute__((alias("tfm_comp_lpcomp_irq_init")));

#endif

#if TFM_PERIPHERAL_EGU0_SECURE
static struct irq_t egu0_irq = {0};

void EGU0_IRQHandler(void)
{
    spm_handle_interrupt(egu0_irq.p_pt, egu0_irq.p_ildi);
}

enum tfm_hal_status_t tfm_egu0_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
{
    return irq_init(&egu0_irq, TFM_EGU0_IRQ, p_pt, p_ildi);
}
#endif

#if TFM_PERIPHERAL_EGU1_SECURE
static struct irq_t egu1_irq = {0};

void EGU1_IRQHandler(void)
{
    spm_handle_interrupt(egu1_irq.p_pt, egu1_irq.p_ildi);
}

enum tfm_hal_status_t tfm_egu1_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
{
    return irq_init(&egu1_irq, TFM_EGU1_IRQ, p_pt, p_ildi);
}
#endif

#if TFM_PERIPHERAL_EGU2_SECURE
static struct irq_t egu2_irq = {0};

void EGU2_IRQHandler(void)
{
    spm_handle_interrupt(egu2_irq.p_pt, egu2_irq.p_ildi);
}

enum tfm_hal_status_t tfm_egu2_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
{
    return irq_init(&egu2_irq, TFM_EGU2_IRQ, p_pt, p_ildi);
}
#endif

#if TFM_PERIPHERAL_EGU3_SECURE
static struct irq_t egu3_irq = {0};

void EGU3_IRQHandler(void)
{
    spm_handle_interrupt(egu3_irq.p_pt, egu3_irq.p_ildi);
}

enum tfm_hal_status_t tfm_egu3_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
{
    return irq_init(&egu3_irq, TFM_EGU3_IRQ, p_pt, p_ildi);
}
#endif

#if TFM_PERIPHERAL_EGU4_SECURE
static struct irq_t egu4_irq = {0};

void EGU4_IRQHandler(void)
{
    spm_handle_interrupt(egu4_irq.p_pt, egu4_irq.p_ildi);
}

enum tfm_hal_status_t tfm_egu4_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
{
    return irq_init(&egu4_irq, TFM_EGU4_IRQ, p_pt, p_ildi);
}
#endif

#if TFM_PERIPHERAL_EGU5_SECURE
static struct irq_t egu5_irq = {0};

void EGU5_IRQHandler(void)
{
    spm_handle_interrupt(egu5_irq.p_pt, egu5_irq.p_ildi);
}

enum tfm_hal_status_t tfm_egu5_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
{
    return irq_init(&egu5_irq, TFM_EGU5_IRQ, p_pt, p_ildi);
}
#endif

#if TFM_PERIPHERAL_PWM0_SECURE
static struct irq_t pwm0_irq = {0};

void PWM0_IRQHandler(void)
{
    spm_handle_interrupt(pwm0_irq.p_pt, pwm0_irq.p_ildi);
}

enum tfm_hal_status_t tfm_pwm0_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
{
    return irq_init(&pwm0_irq, TFM_PWM0_IRQ, p_pt, p_ildi);
}
#endif

#if TFM_PERIPHERAL_PWM1_SECURE
static struct irq_t pwm1_irq = {0};

void PWM1_IRQHandler(void)
{
    spm_handle_interrupt(pwm1_irq.p_pt, pwm1_irq.p_ildi);
}

enum tfm_hal_status_t tfm_pwm1_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
{
    return irq_init(&pwm1_irq, TFM_PWM1_IRQ, p_pt, p_ildi);
}
#endif

#if TFM_PERIPHERAL_PWM2_SECURE
static struct irq_t pwm2_irq = {0};

void PWM2_IRQHandler(void)
{
    spm_handle_interrupt(pwm2_irq.p_pt, pwm2_irq.p_ildi);
}

enum tfm_hal_status_t tfm_pwm2_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
{
    return irq_init(&pwm2_irq, TFM_PWM2_IRQ, p_pt, p_ildi);
}
#endif

#if TFM_PERIPHERAL_PWM3_SECURE
static struct irq_t pwm3_irq = {0};

void PWM3_IRQHandler(void)
{
    spm_handle_interrupt(pwm3_irq.p_pt, pwm3_irq.p_ildi);
}

enum tfm_hal_status_t tfm_pwm3_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
{
    return irq_init(&pwm3_irq, TFM_PWM3_IRQ, p_pt, p_ildi);
}
#endif

#if TFM_PERIPHERAL_PDM0_SECURE
static struct irq_t pdm0_irq = {0};

void PDM0_IRQHandler(void)
{
    spm_handle_interrupt(pdm0_irq.p_pt, pdm0_irq.p_ildi);
}

enum tfm_hal_status_t tfm_pdm0_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
{
    return irq_init(&pdm0_irq, TFM_PDM0_IRQ, p_pt, p_ildi);
}
#endif

#ifdef TFM_PERIPHERAL_IPC_SECURE
static struct irq_t ipc_irq = {0};

void IPC_IRQHandler(void)
{
    spm_handle_interrupt(ipc_irq.p_pt, ipc_irq.p_ildi);
}

enum tfm_hal_status_t tfm_ipc_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
{
    return irq_init(&ipc_irq, TFM_IPC_IRQ, p_pt, p_ildi);
}
#endif

#if TFM_PERIPHERAL_I2S0_SECURE
static struct irq_t i2s0_irq = {0};

void I2S0_IRQHandler(void)
{
    spm_handle_interrupt(i2s0_irq.p_pt, i2s0_irq.p_ildi);
}

enum tfm_hal_status_t tfm_i2s0_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
{
    return irq_init(&i2s0_irq, TFM_I2S0_IRQ, p_pt, p_ildi);
}
#endif

#if TFM_PERIPHERAL_QSPI_SECURE
static struct irq_t qspi_irq = {0};

void QSPI_IRQHandler(void)
{
    spm_handle_interrupt(qspi_irq.p_pt, qspi_irq.p_ildi);
}

enum tfm_hal_status_t tfm_qspi_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
{
    return irq_init(&qspi_irq, TFM_QSPI_IRQ, p_pt, p_ildi);
}
#endif

#if TFM_PERIPHERAL_NFCT_SECURE
static struct irq_t nfct_irq = {0};

void NFCT_IRQHandler(void)
{
    spm_handle_interrupt(nfct_irq.p_pt, nfct_irq.p_ildi);
}

enum tfm_hal_status_t tfm_nfct_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
{
    return irq_init(&nfct_irq, TFM_NFCT_IRQ, p_pt, p_ildi);
}
#endif

#if TFM_PERIPHERAL_QDEC0_SECURE
static struct irq_t qdec0_irq = {0};

void QDEC0_IRQHandler(void)
{
    spm_handle_interrupt(qdec0_irq.p_pt, qdec0_irq.p_ildi);
}

enum tfm_hal_status_t tfm_qdec0_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
{
    return irq_init(&qdec0_irq, TFM_QDEC0_IRQ, p_pt, p_ildi);
}
#endif

#if TFM_PERIPHERAL_QDEC1_SECURE
static struct irq_t qdec1_irq = {0};

void QDEC1_IRQHandler(void)
{
    spm_handle_interrupt(qdec1_irq.p_pt, qdec1_irq.p_ildi);
}

enum tfm_hal_status_t tfm_qdec1_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
{
    return irq_init(&qdec1_irq, TFM_QDEC1_IRQ, p_pt, p_ildi);
}
#endif

#if TFM_PERIPHERAL_USBD_SECURE
static struct irq_t usbd_irq = {0};

void USBD_IRQHandler(void)
{
    spm_handle_interrupt(usbd_irq.p_pt, usbd_irq.p_ildi);
}

enum tfm_hal_status_t tfm_usbd_irq_init(void *p_pt,
                                         struct irq_load_info_t *p_ildi)
{
    return irq_init(&usbd_irq, TFM_USBD_IRQ, p_pt, p_ildi);
}
#endif

#if TFM_PERIPHERAL_USBREG_SECURE
static struct irq_t usbreg_irq = {0};

void USBREGULATOR_IRQHandler(void)
{
    spm_handle_interrupt(usbreg_irq.p_pt, usbreg_irq.p_ildi);
}

enum tfm_hal_status_t tfm_usbreg_irq_init(void *p_pt,
                                          struct irq_load_info_t *p_ildi)
{
    return irq_init(&usbreg_irq, TFM_USBREG_IRQ, p_pt, p_ildi);
}
#endif

#ifdef PSA_API_TEST_IPC
enum tfm_hal_status_t ff_test_uart_irq_init(void *p_pt,
                                            struct irq_load_info_t *p_ildi)
__attribute__((alias("tfm_egu5_irq_init")));

#endif
