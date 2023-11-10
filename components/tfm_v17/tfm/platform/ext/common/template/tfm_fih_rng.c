/*
 * Copyright (c) 2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "cmsis.h"
#include "tfm_fih_rng.h"

fih_int tfm_fih_random_init(void)
{
    SysTick->LOAD  = 0x000000FE;                  /* Set reload register */
    SysTick->VAL   = 0UL;                         /* Load the SysTick Counter Value */
    SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                     SysTick_CTRL_ENABLE_Msk;     /* Enable SysTick Timer */

    return FIH_SUCCESS;
}

void tfm_fih_random_generate(uint8_t *rand)
{
    /* Repeat reading SysTick value to mitigate instruction skip */
    *rand = (uint8_t)SysTick->VAL;
    *rand = (uint8_t)SysTick->VAL;
    *rand = (uint8_t)SysTick->VAL;
    *rand = (uint8_t)SysTick->VAL;
    *rand = (uint8_t)SysTick->VAL;
}
