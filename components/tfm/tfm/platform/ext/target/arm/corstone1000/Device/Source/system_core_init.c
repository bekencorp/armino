/*
 * Copyright (c) 2009-2021 Arm Limited. All rights reserved.
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

/*
 * This file is derivative of CMSIS V5.01:
 * \Device\ARM\ARMCM0plus\Source\system_ARMCM0plus.c
 */

#include "system_core_init.h"
#include "cmsis.h"

/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/
#define  SYSTEM_CLOCK       (50000000UL)
#define  PERIPHERAL_CLOCK   (50000000UL)

/*----------------------------------------------------------------------------
  Externals
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
  System Core Clock Variable
 *----------------------------------------------------------------------------*/
uint32_t SystemCoreClock = SYSTEM_CLOCK;
uint32_t PeripheralClock = PERIPHERAL_CLOCK;

/*----------------------------------------------------------------------------
  System Core Clock update function
 *----------------------------------------------------------------------------*/
void SystemCoreClockUpdate (void)
{
    SystemCoreClock = SYSTEM_CLOCK;
    PeripheralClock = PERIPHERAL_CLOCK;
}

/*----------------------------------------------------------------------------
  System initialization function
 *----------------------------------------------------------------------------*/
void SystemInit (void)
{
#if defined (__VTOR_PRESENT) && (__VTOR_PRESENT == 1U)
    extern uint32_t __Vectors;
    SCB->VTOR = (uint32_t) &__Vectors;
#endif
    SystemCoreClockUpdate();
}
