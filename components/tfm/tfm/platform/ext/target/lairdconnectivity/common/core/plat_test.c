/*
 * Copyright (c) 2020 Nordic Semiconductor ASA. All rights reserved.
 * Copyright (c) 2021 Laird Connectivity. All rights reserved.
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <string.h>
#include "tfm_plat_test.h"
#include "pal_plat_test.h"
#include <stdint.h>
#include <stdbool.h>
#include <hal/nrf_gpio.h>
#include <hal/nrf_timer.h>
#include <helpers/nrfx_reset_reason.h>
#include <hal/nrf_twim.h>
#include <drivers/include/nrfx_twim.h>
#include <drivers/include/nrfx_twi_twim.h>
#include <lcz_board.h>

#if defined(PSA_API_TEST_NS) && !defined(PSA_API_TEST_IPC)
#include <tfm_platform_api.h>
#include "tfm_spm_log.h"
#endif

#ifndef RTE_TWIM2
#error "RTE_TWIM2 must be defined to enable use of the I2C (TWI) GPIO port expander"
#endif

#define TIMER_RELOAD_VALUE          (1*1000*1000)

/* Area used by psa-arch-tests to keep state. */
#define PSA_TEST_SCRATCH_AREA_SIZE  (0x400)

static void timer_init(NRF_TIMER_Type * TIMER, uint32_t ticks)
{
    nrf_timer_mode_set(TIMER, NRF_TIMER_MODE_TIMER);
    nrf_timer_bit_width_set(TIMER, NRF_TIMER_BIT_WIDTH_32);
    nrf_timer_frequency_set(TIMER, NRF_TIMER_FREQ_1MHz);
    nrf_timer_cc_set(TIMER, NRF_TIMER_CC_CHANNEL0, ticks);
    /* Clear the timer once event is generated. */
    nrf_timer_shorts_enable(TIMER, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK);
}

static void timer_stop(NRF_TIMER_Type * TIMER)
{
    nrf_timer_task_trigger(TIMER, NRF_TIMER_TASK_STOP);
    nrf_timer_int_disable(TIMER, NRF_TIMER_INT_COMPARE0_MASK);
    nrf_timer_event_clear(TIMER, NRF_TIMER_EVENT_COMPARE0);
}

static void timer_start(NRF_TIMER_Type * TIMER)
{
    timer_stop(TIMER);

    nrf_timer_task_trigger(TIMER, NRF_TIMER_TASK_CLEAR);
    nrf_timer_int_enable(TIMER, NRF_TIMER_INT_COMPARE0_MASK);

    nrf_timer_task_trigger(TIMER, NRF_TIMER_TASK_START);
}

static void timer_event_clear(NRF_TIMER_Type *TIMER)
{
    nrf_timer_event_clear(TIMER, NRF_TIMER_EVENT_COMPARE0);
}

void tfm_plat_test_secure_timer_start(void)
{
    timer_init(NRF_TIMER0, TIMER_RELOAD_VALUE);
    timer_start(NRF_TIMER0);
}

void tfm_plat_test_secure_timer_clear_intr(void)
{
    timer_event_clear(NRF_TIMER0);
}

void tfm_plat_test_secure_timer_stop(void)
{
    timer_stop(NRF_TIMER0);
}

void tfm_plat_test_non_secure_timer_start(void)
{
    timer_init(NRF_TIMER1, TIMER_RELOAD_VALUE);
    timer_start(NRF_TIMER1);
}

void tfm_plat_test_non_secure_timer_stop(void)
{
    timer_stop(NRF_TIMER1);
}

void pal_timer_init_ns(uint32_t ticks)
{
    timer_init(NRF_TIMER1, ticks);
    NVIC_EnableIRQ(TIMER1_IRQn);
}

void pal_timer_start_ns(void)
{
    timer_start(NRF_TIMER1);
}

void pal_timer_stop_ns(void)
{
    timer_stop(NRF_TIMER1);
}

#if defined(PSA_API_TEST_NS) && !defined(PSA_API_TEST_IPC)
/* Watchdog timeout handler. */
void TIMER1_Handler(void)
{
    pal_timer_stop_ns();
    int ret = tfm_platform_system_reset();
    if (ret) {
        SPMLOG_ERRMSGVAL("Reset failed: ", ret);
    }
}
#endif

#ifdef PSA_API_TEST_ENABLED
uint32_t pal_nvmem_get_addr(void)
{
#ifdef NRF_TRUSTZONE_NONSECURE
    static bool psa_scratch_initialized = false;

    if (!psa_scratch_initialized) {
        uint32_t reset_reason = nrfx_reset_reason_get();
        nrfx_reset_reason_clear(reset_reason);

        int is_pinreset = reset_reason & NRFX_RESET_REASON_RESETPIN_MASK;
        if ((reset_reason == 0) || is_pinreset){
            /* PSA API tests expect this area to be initialized to all 0xFFs
             * after a power-on or pin reset.
             */
            memset((void*)PSA_TEST_SCRATCH_AREA_BASE, 0xFF, PSA_TEST_SCRATCH_AREA_SIZE);
        }
        psa_scratch_initialized = true;
    }
#endif /* NRF_TRUSTZONE_NONSECURE */
    return (uint32_t)PSA_TEST_SCRATCH_AREA_BASE;
}
#endif /* PSA_API_TEST_ENABLED */
