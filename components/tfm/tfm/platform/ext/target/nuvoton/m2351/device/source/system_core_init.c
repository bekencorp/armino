/*
 * Copyright (c) 2009-2019 Arm Limited. All rights reserved.
 * Copyright (c) 2020 Nuvoton Technology Corp. All rights reserved.
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
 *
 * This file is derivative of CMSIS V5.01 \Device\ARM\ARMCM33\Source\system_ARMCM33.c
 * https://github.com/ARM-software/CMSIS_5/tree/5.0.1
 * Git SHA: 8a1d9d6ee18b143ae5befefa14d89fb5b3f99c75
 */

#include <stdint.h>
#include "NuMicro.h"
#include "system_core_init.h"
#include "platform_description.h"

/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/

#define  SYSTEM_CLOCK    (64000000UL)

/*----------------------------------------------------------------------------
  Externals
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
  System Core Clock Variable
 *----------------------------------------------------------------------------*/
uint32_t SystemCoreClock = SYSTEM_CLOCK;
uint32_t PeripheralClock = SYSTEM_CLOCK;
uint32_t RefClock        = SYSTEM_CLOCK;
uint32_t CyclesPerUs     = SYSTEM_CLOCK / 1000000;
uint32_t PllClock        = SYSTEM_CLOCK;

/*----------------------------------------------------------------------------
  System Core Clock update function
 *----------------------------------------------------------------------------*/
void SystemCoreClockUpdate (void)
{
  SystemCoreClock = SYSTEM_CLOCK;
  PeripheralClock = SYSTEM_CLOCK;
  RefClock        = SYSTEM_CLOCK;
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

//#if __DOMAIN_NS == 0
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3L)

  /* Initial the system */
  SYS_UnlockReg();

  /* Enable HIRC and waiting for stable */
  CLK->PWRCTL |= CLK_PWRCTL_HIRCEN_Msk;
  while((CLK->STATUS & CLK_STATUS_HIRCSTB_Msk) == 0);

  /* Enable PLL and waiting for stable */
  CLK->PLLCTL = CLK_PLLCTL_64MHz_HIRC;
  while((CLK->STATUS & CLK_STATUS_PLLSTB_Msk) == 0);

  /* Set system to use PLL with HCLK_DIV = 1 */
  CLK->CLKSEL0 = CLK_CLKSEL0_HCLKSEL_PLL;
  CLK->CLKDIV0 = 0;

  /* Switch HCLK clock source to PLL */
  CLK->CLKSEL0 = (CLK->CLKSEL0 & (~CLK_CLKSEL0_HCLKSEL_Msk)) | CLK_CLKSEL0_HCLKSEL_PLL;

  /* Enable IP clock */
  CLK->APBCLK0 |= CLK_APBCLK0_UART0CKEN_Msk | CLK_APBCLK0_TMR0CKEN_Msk | CLK_APBCLK0_TMR2CKEN_Msk;

  /* Select UART clock source */
  CLK->CLKSEL1 = (CLK->CLKSEL1 & (~CLK_CLKSEL1_UART0SEL_Msk)) | CLK_CLKSEL1_UART0SEL_HIRC;

  /* Timer clock source */
  CLK->CLKSEL1 = (CLK->CLKSEL1 & (~CLK_CLKSEL1_TMR0SEL_Msk)) | CLK_CLKSEL1_TMR0SEL_HIRC;
  CLK->CLKSEL1 = (CLK->CLKSEL1 & (~CLK_CLKSEL1_TMR2SEL_Msk)) | CLK_CLKSEL1_TMR2SEL_HIRC;

  /* Set multi-function pins for UART0 RXD and TXD */
  SYS->GPB_MFPH = (SYS->GPB_MFPH & (~(UART0_RXD_PB12_Msk | UART0_TXD_PB13_Msk))) | UART0_RXD_PB12 | UART0_TXD_PB13;

  /* Set UART 0 to Non-secure */
  SCU_SET_PNSSET(UART0_Attr);

  /* Set SAU */
  SAU->RNR = 3;
  SAU->RBAR = 0x50000000;
  SAU->RLAR = (0x5FFFFFFF & SAU_RLAR_LADDR_Msk) | SAU_RLAR_ENABLE_Msk;

  /* Init ETM Trace */
  SYS->GPE_MFPH = (SYS->GPE_MFPH & (~(TRACE_CLK_PE12_Msk | TRACE_DATA0_PE11_Msk | TRACE_DATA1_PE10_Msk | TRACE_DATA2_PE9_Msk | TRACE_DATA3_PE8_Msk))) |
      TRACE_CLK_PE12 | TRACE_DATA0_PE11 | TRACE_DATA1_PE10 | TRACE_DATA2_PE9 | TRACE_DATA3_PE8;



#endif


  SystemCoreClock = SYSTEM_CLOCK;
}
