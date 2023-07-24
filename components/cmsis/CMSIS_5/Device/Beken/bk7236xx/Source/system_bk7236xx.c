/*
 * Copyright (c) 2009-2018 Arm Limited.
 * Copyright (c) 2018-2020 ArmChina.
 * All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "bk7236xx.h"
#if defined (__ARM_FEATURE_CMSE) &&  (__ARM_FEATURE_CMSE == 3U)
  #include "partition_bk7236xx.h"
#endif

/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/
#define  SYSTEM_CLOCK    CONFIG_XTAL_FREQ

/*----------------------------------------------------------------------------
  Exception / Interrupt Vector table
 *----------------------------------------------------------------------------*/
extern const VECTOR_TABLE_Type __VECTOR_TABLE[];

#if CONFIG_CPU_WITHOUT_BOOTLOADER
extern const VECTOR_TABLE_Type __VECTOR_TABLE_CPU0[];
#endif


/*----------------------------------------------------------------------------
  System initialization function
 *----------------------------------------------------------------------------*/
void SystemInit (void)
{
#if defined (__VTOR_PRESENT) && (__VTOR_PRESENT == 1U)
	SCB->VTOR = (uint32_t) &(__VECTOR_TABLE[0]);
#endif

#if CONFIG_CPU_WITHOUT_BOOTLOADER
	SCB->VTOR = (uint32_t) &(__VECTOR_TABLE_CPU0[0]);
#endif

#if defined (__FPU_USED) && (__FPU_USED == 1U)
	SCB->CPACR |= ((3U << 10U*2U) |           /* enable CP10 Full Access */
	             (3U << 11U*2U)  );         /* enable CP11 Full Access */
#endif

#ifdef UNALIGNED_SUPPORT_DISABLE
	SCB->CCR |= SCB_CCR_UNALIGN_TRP_Msk;
#endif

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
#if CONFIG_SPE
	TZ_SAU_Setup();
#endif
#endif

#if CONFIG_SPE
#if defined (__ITCM_PRESENT) && (__ITCM_PRESENT == 1U)
	TCM->ITCMCR |= SCB_ITCMCR_EN_Msk;
#endif

#if defined (__DTCM_PRESENT) && (__DTCM_PRESENT == 1U)
	TCM->DTCMCR |= SCB_DTCMCR_EN_Msk;
#endif
#endif

#if (CONFIG_ICACHE)
	if (SCB->CLIDR & SCB_CLIDR_IC_Msk)
		SCB_EnableICache();
#endif

}
// eof

