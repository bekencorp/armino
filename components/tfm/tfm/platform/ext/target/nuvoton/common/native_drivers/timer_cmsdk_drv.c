/*
 * Copyright (c) 2016-2019 Arm Limited. All rights reserved.
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

#include <stdint.h>
#include "NuMicro.h"
#include "timer_cmsdk_drv.h"


void timer_cmsdk_init(const struct timer_cmsdk_dev_t* dev)
{
    TIMER_T* tmr = (TIMER_T*)dev->cfg->base;

    if (dev->data->is_initialized == 0)
    {

        NVIC_EnableIRQ(TMR0_IRQn);
        NVIC_EnableIRQ(TMR2_IRQn);

        TIMER_Open(tmr, TIMER_PERIODIC_MODE, 1000);
        dev->data->is_initialized = 1;
    }
}

bool timer_cmsdk_is_initialized(const struct timer_cmsdk_dev_t* dev)
{
    return dev->data->is_initialized;
}

void timer_cmsdk_enable_external_input(const struct timer_cmsdk_dev_t* dev)
{
    TIMER_T* tmr = (TIMER_T*)dev->cfg->base;
    TIMER_EnableCapture(tmr, TIMER_CAPTURE_COUNTER_RESET_MODE, TIMER_CAPTURE_EVENT_RISING);
}

void timer_cmsdk_disable_external_input(const struct timer_cmsdk_dev_t* dev)
{
    TIMER_T* tmr = (TIMER_T*)dev->cfg->base;
    TIMER_DisableCapture(tmr);
}

bool timer_cmsdk_is_external_input_enabled(const struct timer_cmsdk_dev_t* dev)
{
    return ((((TIMER_T*)dev->cfg->base)->EXTCTL & TIMER_EXTCTL_CAPEN_Msk) != 0);
}

void timer_cmsdk_set_clock_to_internal(const struct timer_cmsdk_dev_t* dev)
{
    TIMER_T* tmr = (TIMER_T*)dev->cfg->base;
    TIMER_DisableEventCounter(tmr);
}

void timer_cmsdk_set_clock_to_external(const struct timer_cmsdk_dev_t* dev)
{
    TIMER_T* tmr = (TIMER_T*)dev->cfg->base;
    TIMER_EnableEventCounter(tmr, TIMER_COUNTER_EVENT_RISING);
}

bool timer_cmsdk_is_clock_external(const struct timer_cmsdk_dev_t* dev)
{
    return ((((TIMER_T *)dev->cfg->base)->CTL & TIMER_CTL_EXTCNTEN_Msk) != 0);
}

void timer_cmsdk_enable(const struct timer_cmsdk_dev_t* dev)
{
    TIMER_T* tmr = (TIMER_T*)dev->cfg->base;
    TIMER_Start(tmr);
}

void timer_cmsdk_disable(const struct timer_cmsdk_dev_t* dev)
{
    TIMER_T* tmr = (TIMER_T*)dev->cfg->base;
    TIMER_Stop(tmr);
}

bool timer_cmsdk_is_enabled(const struct timer_cmsdk_dev_t* dev)
{
    return ((((TIMER_T *)dev->cfg->base)->CTL & TIMER_CTL_CNTEN_Msk) != 0);
}

void timer_cmsdk_enable_interrupt(const struct timer_cmsdk_dev_t* dev)
{
    TIMER_T* tmr = (TIMER_T*)dev->cfg->base;
    TIMER_EnableInt(tmr);
}

void timer_cmsdk_disable_interrupt(const struct timer_cmsdk_dev_t* dev)
{
    TIMER_T* tmr = (TIMER_T*)dev->cfg->base;
    TIMER_DisableInt(tmr);
}

bool timer_cmsdk_is_interrupt_enabled(const struct timer_cmsdk_dev_t* dev)
{
    return ((((TIMER_T *)dev->cfg->base)->CTL & TIMER_CTL_INTEN_Msk) != 0);
}

bool timer_cmsdk_is_interrupt_active(const struct timer_cmsdk_dev_t* dev)
{
    return ((((TIMER_T *)dev->cfg->base)->INTSTS & TIMER_INTSTS_TIF_Msk) != 0);
}

void timer_cmsdk_clear_interrupt(const struct timer_cmsdk_dev_t* dev)
{
    TIMER_T* tmr = (TIMER_T*)dev->cfg->base;
    TIMER_ClearIntFlag(tmr);
}

uint32_t timer_cmsdk_get_current_value(const struct timer_cmsdk_dev_t* dev)
{
    TIMER_T* tmr = (TIMER_T*)dev->cfg->base;
    return TIMER_GetCounter(tmr);
}

void timer_cmsdk_set_reload_value(const struct timer_cmsdk_dev_t* dev,
                                uint32_t reload)
{
    TIMER_T* tmr = (TIMER_T*)dev->cfg->base;

    /* reload value must > 1 */
    if(reload < 2)
    {
        reload = 2;
    }

    TIMER_SET_CMP_VALUE(tmr, reload);
}

void timer_cmsdk_reset(const struct timer_cmsdk_dev_t* dev)
{
    TIMER_T* tmr = (TIMER_T*)dev->cfg->base;
    TIMER_ResetCounter(tmr);
}

uint32_t timer_cmsdk_get_reload_value(const struct timer_cmsdk_dev_t* dev)
{
    return ((TIMER_T*)dev->cfg->base)->CMP;
}

uint32_t timer_cmsdk_get_elapsed_value(const struct timer_cmsdk_dev_t* dev)
{
    TIMER_T *tmr;

    tmr = (TIMER_T*)dev->cfg->base;
    return tmr->CMP - tmr->CNT;
}
