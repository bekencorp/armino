/*
 * Copyright (c) 2016-2020 Arm Limited
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

#include "arm_watchdog_drv.h"

/* Watchdog control definitions */
#define ARM_WATCHDOG_CTRL_INTEN        (0x1UL << 0)
#define ARM_WATCHDOG_CTRL_RESEN        (0x1UL << 1)
#define ARM_WATCHDOG_INTCLR            1UL
#define ARM_WATCHDOG_RAWINTSTAT        1UL
#define ARM_WATCHDOG_MASKINTSTAT       1UL
#define ARM_WATCHDOG_UNLOCK_VALUE      0x1ACCE551
#define ARM_WATCHDOG_LOCK_VALUE        0x00000001
#define ARM_WATCHDOG_INTEGTESTEN       1UL
#define ARM_WATCHDOG_INTEGTESTOUTSET   1UL
#define ARM_WATCHDOG_MAX_VALUE         0xFFFFFFFF

/* Watchdog state definitions */
#define ARM_WATCHDOG_INITIALIZED  (1ul << 0)
#define ARM_WATCHDOG_ENABLED      (1ul << 1)

/* ARM watchdog memory mapped register access structure */
struct arm_watchdog_t {
    volatile uint32_t  load;          /* Offset: 0x000 (R/W) Load register */
    volatile uint32_t  value;         /* Offset: 0x004 (R/ ) Value register */
    volatile uint32_t  ctrl;          /* Offset: 0x008 (R/W) Control register */
    volatile uint32_t  intclr;        /* Offset: 0x00C ( /W) Clear interrupt
                                       *                     register */
    volatile uint32_t  rawintstat;    /* Offset: 0x010 (R/ ) Raw interrupt
                                       *                     status register */
    volatile uint32_t  maskintstat;   /* Offset: 0x014 (R/ ) Interrupt status
                                       *                     register */
    volatile uint32_t  reserved0[762];
    volatile uint32_t  lock;          /* Offset: 0xC00 (R/W) Lock register */
    volatile uint32_t  reserved1[191];
    volatile uint32_t  itcr;          /* Offset: 0xF00 (R/W) Integration test
                                       *                     control register */
    volatile uint32_t  itop;          /* Offset: 0xF04 ( /W) Integration Test
                                       *                     output set
                                       *                     register */
};

void arm_watchdog_init(struct arm_watchdog_dev_t* dev, uint32_t timeout)
{
    /*
     * The init function leaves the watchdog in a clean state:
     *  - initialized;
     *  - disabled;
     *  - locked.
     */
    if (arm_watchdog_is_enabled(dev)) {
        arm_watchdog_unlock(dev);
        (void)arm_watchdog_disable(dev);
    }
    arm_watchdog_lock(dev);

    if (timeout == 0)
        dev->data->timeout = ARM_WATCHDOG_MAX_VALUE;
    else
        dev->data->timeout = timeout;

    dev->data->state = ARM_WATCHDOG_INITIALIZED;
}

enum arm_watchdog_error_t arm_watchdog_feed(struct arm_watchdog_dev_t* dev)
{
    struct arm_watchdog_t* p_wdog = (struct arm_watchdog_t*)dev->cfg->base;

    if (!arm_watchdog_is_enabled(dev))
        return ARM_WATCHDOG_ERR_NOT_ENAB;

    if (arm_watchdog_is_locked(dev))
        return ARM_WATCHDOG_ERR_LOCKED;

    p_wdog->load = dev->data->timeout;

    return ARM_WATCHDOG_ERR_NONE;
}

enum arm_watchdog_error_t
arm_watchdog_clear_interrupt_and_refresh_counter(struct arm_watchdog_dev_t* dev)
{
    struct arm_watchdog_t* p_wdog = (struct arm_watchdog_t*)dev->cfg->base;

    if (!arm_watchdog_is_enabled(dev))
        return ARM_WATCHDOG_ERR_NOT_ENAB;

    if (arm_watchdog_is_locked(dev))
        return ARM_WATCHDOG_ERR_LOCKED;

    p_wdog->intclr = ARM_WATCHDOG_INTCLR;

    return ARM_WATCHDOG_ERR_NONE;
}

enum arm_watchdog_error_t arm_watchdog_enable(struct arm_watchdog_dev_t* dev)
{
    struct arm_watchdog_t* p_wdog = (struct arm_watchdog_t*)dev->cfg->base;

    if (!arm_watchdog_is_initialized(dev))
        return ARM_WATCHDOG_ERR_NOT_INIT;

    if (arm_watchdog_is_locked(dev))
        return ARM_WATCHDOG_ERR_LOCKED;

    p_wdog->load = dev->data->timeout;

    /* Starts the watchdog counter */
    p_wdog->ctrl = (ARM_WATCHDOG_CTRL_RESEN | ARM_WATCHDOG_CTRL_INTEN);
    dev->data->state |= ARM_WATCHDOG_ENABLED;

    return ARM_WATCHDOG_ERR_NONE;
}

uint32_t arm_watchdog_is_enabled(struct arm_watchdog_dev_t* dev)
{
    return (dev->data->state & ARM_WATCHDOG_ENABLED);
}

enum arm_watchdog_error_t arm_watchdog_disable(struct arm_watchdog_dev_t* dev)
{
    struct arm_watchdog_t* p_wdog = (struct arm_watchdog_t*)dev->cfg->base;

    if (!arm_watchdog_is_enabled(dev))
        return ARM_WATCHDOG_ERR_NOT_ENAB;

    if (arm_watchdog_is_locked(dev))
        return ARM_WATCHDOG_ERR_LOCKED;

    /* Stops the watchdog */
    p_wdog->ctrl &= ~(ARM_WATCHDOG_CTRL_RESEN | ARM_WATCHDOG_CTRL_INTEN);
    dev->data->state &= ~ARM_WATCHDOG_ENABLED;

    return ARM_WATCHDOG_ERR_NONE;
}

void arm_watchdog_lock(struct arm_watchdog_dev_t* dev)
{
    struct arm_watchdog_t* p_wdog = (struct arm_watchdog_t*)dev->cfg->base;

    /* Prevents writing to all of the registers */
    p_wdog->lock = ARM_WATCHDOG_LOCK_VALUE;
}

uint32_t arm_watchdog_is_locked(struct arm_watchdog_dev_t* dev)
{
    struct arm_watchdog_t* p_wdog = (struct arm_watchdog_t*)dev->cfg->base;

    /* The lock register can only return 0 or 1 when read */
    return p_wdog->lock;
}

void arm_watchdog_unlock(struct arm_watchdog_dev_t* dev)
{
    struct arm_watchdog_t* p_wdog = (struct arm_watchdog_t*)dev->cfg->base;

    p_wdog->lock = ARM_WATCHDOG_UNLOCK_VALUE;
}

inline uint32_t arm_watchdog_is_initialized(struct arm_watchdog_dev_t* dev)
{
    return (dev->data->state & ARM_WATCHDOG_INITIALIZED);
}

uint32_t arm_watchdog_get_remaining_time(struct arm_watchdog_dev_t* dev)
{
    struct arm_watchdog_t* p_wdog = (struct arm_watchdog_t*)dev->cfg->base;

    if (!arm_watchdog_is_enabled(dev))
        return 0;

    if (arm_watchdog_is_locked(dev))
        return 0;

    return p_wdog->value;
}

