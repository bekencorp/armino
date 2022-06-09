/*
 * Copyright (c) 2018-2021, Arm Limited. All rights reserved.
 * Copyright (c) 2020, Cypress Semiconductor Corporation. All rights reserved.
 * Copyright (c) 2020, Nordic Semiconductor ASA. All rights reserved.
 * Copyright (c) 2021, Laird Connectivity. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_PERIPHERALS_DEF_H__
#define __TFM_PERIPHERALS_DEF_H__

#include <nrf.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TFM_FPU_IRQ            (FPU_IRQn)
#define TFM_CACHE_IRQ          (CACHE_IRQn)
#define TFM_POWER_IRQ          (CLOCK_POWER_IRQn)
#define TFM_CLOCK_IRQ          (CLOCK_POWER_IRQn)
#define TFM_CLOCK_POWER_IRQ    (CLOCK_POWER_IRQn)
#define TFM_SPIM0_IRQ          (SERIAL0_IRQn)
#define TFM_SPIS0_IRQ          (SERIAL0_IRQn)
#define TFM_TWIM0_IRQ          (SERIAL0_IRQn)
#define TFM_TWIS0_IRQ          (SERIAL0_IRQn)
#define TFM_UARTE0_IRQ         (SERIAL0_IRQn)
#define TFM_SERIAL0_IRQ        (SERIAL0_IRQn)
#define TFM_SPIM1_IRQ          (SERIAL1_IRQn)
#define TFM_SPIS1_IRQ          (SERIAL1_IRQn)
#define TFM_TWIM1_IRQ          (SERIAL1_IRQn)
#define TFM_TWIS1_IRQ          (SERIAL1_IRQn)
#define TFM_UARTE1_IRQ         (SERIAL1_IRQn)
#define TFM_SERIAL1_IRQ        (SERIAL1_IRQn)
#define TFM_SPIM4_IRQ          (SPIM4_IRQn)
#define TFM_SPIM2_IRQ          (SERIAL2_IRQn)
#define TFM_SPIS2_IRQ          (SERIAL2_IRQn)
#define TFM_TWIM2_IRQ          (SERIAL2_IRQn)
#define TFM_TWIS2_IRQ          (SERIAL2_IRQn)
#define TFM_UARTE2_IRQ         (SERIAL2_IRQn)
#define TFM_SERIAL2_IRQ        (SERIAL2_IRQn)
#define TFM_SPIM3_IRQ          (SERIAL3_IRQn)
#define TFM_SPIS3_IRQ          (SERIAL3_IRQn)
#define TFM_TWIM3_IRQ          (SERIAL3_IRQn)
#define TFM_TWIS3_IRQ          (SERIAL3_IRQn)
#define TFM_UARTE3_IRQ         (SERIAL3_IRQn)
#define TFM_SERIAL3_IRQ        (SERIAL3_IRQn)
#define TFM_SAADC_IRQ          (SAADC_IRQn)
#define TFM_TIMER0_IRQ         (TIMER0_IRQn)
#define TFM_TIMER1_IRQ         (TIMER1_IRQn)
#define TFM_TIMER2_IRQ         (TIMER2_IRQn)
#define TFM_RTC0_IRQ           (RTC0_IRQn)
#define TFM_RTC1_IRQ           (RTC1_IRQn)
#define TFM_WDT0_IRQ           (WDT0_IRQn)
#define TFM_WDT1_IRQ           (WDT1_IRQn)
#define TFM_COMP_IRQ           (COMP_LPCOMP_IRQn)
#define TFM_LPCOMP_IRQ         (COMP_LPCOMP_IRQn)
#define TFM_COMP_LPCOMP_IRQ    (COMP_LPCOMP_IRQn)
#define TFM_EGU0_IRQ           (EGU0_IRQn)
#define TFM_EGU1_IRQ           (EGU1_IRQn)
#define TFM_EGU2_IRQ           (EGU2_IRQn)
#define TFM_EGU3_IRQ           (EGU3_IRQn)
#define TFM_EGU4_IRQ           (EGU4_IRQn)
#define TFM_EGU5_IRQ           (EGU5_IRQn)
#define TFM_PWM0_IRQ           (PWM0_IRQn)
#define TFM_PWM1_IRQ           (PWM1_IRQn)
#define TFM_PWM2_IRQ           (PWM2_IRQn)
#define TFM_PWM3_IRQ           (PWM3_IRQn)
#define TFM_PDM0_IRQ           (PDM0_IRQn)
#define TFM_I2S0_IRQ           (I2S0_IRQn)
#define TFM_IPC_IRQ            (IPC_IRQn)
#define TFM_QSPI_IRQ           (QSPI_IRQn)
#define TFM_NFCT_IRQ           (NFCT_IRQn)
#define TFM_GPIOTE0_IRQ        (GPIOTE0_IRQn)
#define TFM_QDEC0_IRQ          (QDEC0_IRQn)
#define TFM_QDEC1_IRQ          (QDEC1_IRQn)
#define TFM_USBD_IRQ           (USBD_IRQn)
#define TFM_USBREG_IRQ         (USBREGULATOR_IRQn)

struct platform_data_t;

extern struct platform_data_t tfm_peripheral_dcnf;
extern struct platform_data_t tfm_peripheral_fpu;
extern struct platform_data_t tfm_peripheral_oscillators;
extern struct platform_data_t tfm_peripheral_regulators;
extern struct platform_data_t tfm_peripheral_clock;
extern struct platform_data_t tfm_peripheral_power;
extern struct platform_data_t tfm_peripheral_reset;
extern struct platform_data_t tfm_peripheral_spim0;
extern struct platform_data_t tfm_peripheral_spis0;
extern struct platform_data_t tfm_peripheral_twim0;
extern struct platform_data_t tfm_peripheral_twis0;
extern struct platform_data_t tfm_peripheral_uarte0;
extern struct platform_data_t tfm_peripheral_spim1;
extern struct platform_data_t tfm_peripheral_spis1;
extern struct platform_data_t tfm_peripheral_twim1;
extern struct platform_data_t tfm_peripheral_twis1;
extern struct platform_data_t tfm_peripheral_uarte1;
extern struct platform_data_t tfm_peripheral_spim4;
extern struct platform_data_t tfm_peripheral_spim2;
extern struct platform_data_t tfm_peripheral_spis2;
extern struct platform_data_t tfm_peripheral_twim2;
extern struct platform_data_t tfm_peripheral_twis2;
extern struct platform_data_t tfm_peripheral_uarte2;
extern struct platform_data_t tfm_peripheral_spim3;
extern struct platform_data_t tfm_peripheral_spis3;
extern struct platform_data_t tfm_peripheral_twim3;
extern struct platform_data_t tfm_peripheral_twis3;
extern struct platform_data_t tfm_peripheral_uarte3;
extern struct platform_data_t tfm_peripheral_saadc;
extern struct platform_data_t tfm_peripheral_timer0;
extern struct platform_data_t tfm_peripheral_timer1;
extern struct platform_data_t tfm_peripheral_timer2;
extern struct platform_data_t tfm_peripheral_rtc0;
extern struct platform_data_t tfm_peripheral_rtc1;
extern struct platform_data_t tfm_peripheral_dppi;
extern struct platform_data_t tfm_peripheral_wdt0;
extern struct platform_data_t tfm_peripheral_wdt1;
extern struct platform_data_t tfm_peripheral_comp;
extern struct platform_data_t tfm_peripheral_lpcomp;
extern struct platform_data_t tfm_peripheral_egu0;
extern struct platform_data_t tfm_peripheral_egu1;
extern struct platform_data_t tfm_peripheral_egu2;
extern struct platform_data_t tfm_peripheral_egu3;
extern struct platform_data_t tfm_peripheral_egu4;
extern struct platform_data_t tfm_peripheral_egu5;
extern struct platform_data_t tfm_peripheral_pwm0;
extern struct platform_data_t tfm_peripheral_pwm1;
extern struct platform_data_t tfm_peripheral_pwm2;
extern struct platform_data_t tfm_peripheral_pwm3;
extern struct platform_data_t tfm_peripheral_pdm0;
extern struct platform_data_t tfm_peripheral_i2s0;
extern struct platform_data_t tfm_peripheral_ipc;
extern struct platform_data_t tfm_peripheral_qspi;
extern struct platform_data_t tfm_peripheral_nfct;
extern struct platform_data_t tfm_peripheral_mutex;
extern struct platform_data_t tfm_peripheral_qdec0;
extern struct platform_data_t tfm_peripheral_qdec1;
extern struct platform_data_t tfm_peripheral_usbd;
extern struct platform_data_t tfm_peripheral_usbreg;
extern struct platform_data_t tfm_peripheral_nvmc;
extern struct platform_data_t tfm_peripheral_gpio0;
extern struct platform_data_t tfm_peripheral_gpio1;
extern struct platform_data_t tfm_peripheral_vmc;

#define TFM_PERIPHERAL_DCNF         (&tfm_peripheral_dcnf)
#define TFM_PERIPHERAL_FPU          (&tfm_peripheral_fpu)
#define TFM_PERIPHERAL_OSCILLATORS  (&tfm_peripheral_oscillators)
#define TFM_PERIPHERAL_REGULATORS   (&tfm_peripheral_regulators)
#define TFM_PERIPHERAL_CLOCK        (&tfm_peripheral_clock)
#define TFM_PERIPHERAL_POWER        (&tfm_peripheral_power)
#define TFM_PERIPHERAL_RESET        (&tfm_peripheral_reset)
#define TFM_PERIPHERAL_SPIM0        (&tfm_peripheral_spim0)
#define TFM_PERIPHERAL_SPIS0        (&tfm_peripheral_spis0)
#define TFM_PERIPHERAL_TWIM0        (&tfm_peripheral_twim0)
#define TFM_PERIPHERAL_TWIS0        (&tfm_peripheral_twis0)
#define TFM_PERIPHERAL_UARTE0       (&tfm_peripheral_uarte0)
#define TFM_PERIPHERAL_SPIM1        (&tfm_peripheral_spim1)
#define TFM_PERIPHERAL_SPIS1        (&tfm_peripheral_spis1)
#define TFM_PERIPHERAL_TWIM1        (&tfm_peripheral_twim1)
#define TFM_PERIPHERAL_TWIS1        (&tfm_peripheral_twis1)
#define TFM_PERIPHERAL_UARTE1       (&tfm_peripheral_uarte1)
#define TFM_PERIPHERAL_SPIM4        (&tfm_peripheral_spim4)
#define TFM_PERIPHERAL_SPIM2        (&tfm_peripheral_spim2)
#define TFM_PERIPHERAL_SPIS2        (&tfm_peripheral_spis2)
#define TFM_PERIPHERAL_TWIM2        (&tfm_peripheral_twim2)
#define TFM_PERIPHERAL_TWIS2        (&tfm_peripheral_twis2)
#define TFM_PERIPHERAL_UARTE2       (&tfm_peripheral_uarte2)
#define TFM_PERIPHERAL_SPIM3        (&tfm_peripheral_spim3)
#define TFM_PERIPHERAL_SPIS3        (&tfm_peripheral_spis3)
#define TFM_PERIPHERAL_TWIM3        (&tfm_peripheral_twim3)
#define TFM_PERIPHERAL_TWIS3        (&tfm_peripheral_twis3)
#define TFM_PERIPHERAL_UARTE3       (&tfm_peripheral_uarte3)
#define TFM_PERIPHERAL_SAADC        (&tfm_peripheral_saadc)
#define TFM_PERIPHERAL_TIMER0       (&tfm_peripheral_timer0)
#define TFM_PERIPHERAL_TIMER1       (&tfm_peripheral_timer1)
#define TFM_PERIPHERAL_TIMER2       (&tfm_peripheral_timer2)
#define TFM_PERIPHERAL_RTC0         (&tfm_peripheral_rtc0)
#define TFM_PERIPHERAL_RTC1         (&tfm_peripheral_rtc1)
#define TFM_PERIPHERAL_DPPI         (&tfm_peripheral_dppi)
#define TFM_PERIPHERAL_WDT0         (&tfm_peripheral_wdt0)
#define TFM_PERIPHERAL_WDT1         (&tfm_peripheral_wdt1)
#define TFM_PERIPHERAL_COMP         (&tfm_peripheral_comp)
#define TFM_PERIPHERAL_LPCOMP       (&tfm_peripheral_lpcomp)
#define TFM_PERIPHERAL_EGU0         (&tfm_peripheral_egu0)
#define TFM_PERIPHERAL_EGU1         (&tfm_peripheral_egu1)
#define TFM_PERIPHERAL_EGU2         (&tfm_peripheral_egu2)
#define TFM_PERIPHERAL_EGU3         (&tfm_peripheral_egu3)
#define TFM_PERIPHERAL_EGU4         (&tfm_peripheral_egu4)
#define TFM_PERIPHERAL_EGU5         (&tfm_peripheral_egu5)
#define TFM_PERIPHERAL_PWM0         (&tfm_peripheral_pwm0)
#define TFM_PERIPHERAL_PWM1         (&tfm_peripheral_pwm1)
#define TFM_PERIPHERAL_PWM2         (&tfm_peripheral_pwm2)
#define TFM_PERIPHERAL_PWM3         (&tfm_peripheral_pwm3)
#define TFM_PERIPHERAL_PDM0         (&tfm_peripheral_pdm0)
#define TFM_PERIPHERAL_I2S0         (&tfm_peripheral_i2s0)
#define TFM_PERIPHERAL_IPC          (&tfm_peripheral_ipc)
#define TFM_PERIPHERAL_QSPI         (&tfm_peripheral_qspi)
#define TFM_PERIPHERAL_NFCT         (&tfm_peripheral_nfct)
#define TFM_PERIPHERAL_MUTEX        (&tfm_peripheral_mutex)
#define TFM_PERIPHERAL_QDEC0        (&tfm_peripheral_qdec0)
#define TFM_PERIPHERAL_QDEC1        (&tfm_peripheral_qdec1)
#define TFM_PERIPHERAL_USBD         (&tfm_peripheral_usbd)
#define TFM_PERIPHERAL_USBREG       (&tfm_peripheral_usbreg)
#define TFM_PERIPHERAL_NVMC         (&tfm_peripheral_nvmc)
#define TFM_PERIPHERAL_GPIO0        (&tfm_peripheral_gpio0)
#define TFM_PERIPHERAL_GPIO1        (&tfm_peripheral_gpio1)
#define TFM_PERIPHERAL_VMC          (&tfm_peripheral_vmc)

#define TFM_PERIPHERAL_STD_UART     TFM_PERIPHERAL_UARTE1

#ifdef PSA_API_TEST_IPC
#define FF_TEST_UART_IRQ         (EGU5_IRQn)
#define FF_TEST_UART_IRQ_Handler (pal_interrupt_handler)

extern struct platform_data_t tfm_peripheral_FF_TEST_NVMEM_REGION;
extern struct platform_data_t tfm_peripheral_FF_TEST_SERVER_PARTITION_MMIO;
extern struct platform_data_t tfm_peripheral_FF_TEST_DRIVER_PARTITION_MMIO;

#define FF_TEST_UART_REGION           (&tfm_peripheral_std_uart)
#define FF_TEST_WATCHDOG_REGION       (&tfm_peripheral_timer0)
#define FF_TEST_NVMEM_REGION          (&tfm_peripheral_FF_TEST_NVMEM_REGION)
#define FF_TEST_SERVER_PARTITION_MMIO (&tfm_peripheral_FF_TEST_SERVER_PARTITION_MMIO)
#define FF_TEST_DRIVER_PARTITION_MMIO (&tfm_peripheral_FF_TEST_DRIVER_PARTITION_MMIO)
#endif /* PSA_API_TEST_IPC */

#ifdef __cplusplus
}
#endif

#endif /* __TFM_PERIPHERALS_DEF_H__ */
