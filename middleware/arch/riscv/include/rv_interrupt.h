// Copyright 2020-2022 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "core_v5.h"

#define PLIC_REG_BASE           (0xE4000000)

/*
 * CPU interrupt control
 */
#define HAL_MSI_ENABLE()                                set_csr(NDS_MIE, MIP_MSIP)
#define HAL_MSI_DISABLE()                               clear_csr(NDS_MIE, MIP_MSIP)

#define HAL_MTI_ENABLE()                                set_csr(NDS_MIE, MIP_MTIP)
#define HAL_MTI_DISABLE()                               clear_csr(NDS_MIE, MIP_MTIP)

#define HAL_MEI_ENABLE()                                set_csr(NDS_MIE, MIP_MEIP)
#define HAL_MEI_DISABLE()                               clear_csr(NDS_MIE, MIP_MEIP)

#define HAL_USI_ENABLE()                                set_csr(NDS_UIE, UIP_USIP)
#define HAL_USI_DISABLE()                               clear_csr(NDS_UIE, UIP_USIP)

#define HAL_UTI_ENABLE()                                set_csr(NDS_UIE, UIP_UTIP)
#define HAL_UTI_DISABLE()                               clear_csr(NDS_UIE, UIP_UTIP)

#define HAL_UEI_ENABLE()                                set_csr(NDS_UIE, UIP_UEIP)
#define HAL_UEI_DISABLE()                               clear_csr(NDS_UIE, UIP_UEIP)

#define HAL_M_INT_ENABLE()								set_csr(NDS_MSTATUS, MSTATUS_MIE)
#define HAL_M_INT_DISABLE()								clear_csr(NDS_MSTATUS, MSTATUS_MIE)

#define HAL_U_INT_ENABLE()								set_csr(NDS_USTATUS, USTATUS_UIE)
#define HAL_U_INT_DISABLE()								clear_csr(NDS_USTATUS, USTATUS_UIE)

#define NDS_PLIC_BASE        	PLIC_REG_BASE

#include "plic.h"

/*
 * Platform level interrupt controller access
 *
 */
#define IRQ_TARGET_M		0
#define IRQ_TARGET_U		1

#define HAL_IRQ_SET_PRIORITY(vector, priority)          __nds__plic_set_priority(vector, priority)

#define HAL_M_IRQ_ENABLE(vector)                        __nds__plic_enable_interrupt(IRQ_TARGET_M, vector)
#define HAL_M_IRQ_DISABLE(vector)                       __nds__plic_disable_interrupt(IRQ_TARGET_M, vector)
#define HAL_M_IRQ_THRESHOLD(threshold)                  __nds__plic_set_threshold(IRQ_TARGET_M, threshold)
#define HAL_M_IRQ_CLAIM()                               __nds__plic_claim_interrupt(IRQ_TARGET_M)
#define HAL_M_IRQ_COMPLETE(source)                      __nds__plic_complete_interrupt(IRQ_TARGET_M, source)

#define HAL_U_IRQ_ENABLE(vector)                        __nds__plic_enable_interrupt(IRQ_TARGET_U, vector)
#define HAL_U_IRQ_DISABLE(vector)                       __nds__plic_disable_interrupt(IRQ_TARGET_U, vector)
#define HAL_U_IRQ_THRESHOLD(threshold)                  __nds__plic_set_threshold(IRQ_TARGET_U, threshold)
#define HAL_U_IRQ_CLAIM()                               __nds__plic_claim_interrupt(IRQ_TARGET_U)
#define HAL_U_IRQ_COMPLETE(source)                      __nds__plic_complete_interrupt(IRQ_TARGET_U, source)

// #define IRQ_TARGET				IRQ_TARGET_M
#define IRQ_TARGET			IRQ_TARGET_U

#if IRQ_TARGET == IRQ_TARGET_M

#define HAL_SI_ENABLE()                                HAL_MSI_ENABLE()
#define HAL_SI_DISABLE()                               HAL_MSI_DISABLE()

#define HAL_TI_ENABLE()                                HAL_MTI_ENABLE()
#define HAL_TI_DISABLE()                               HAL_MTI_DISABLE()

#define HAL_EI_ENABLE()                                HAL_MEI_ENABLE()
#define HAL_EI_DISABLE()                               HAL_MEI_DISABLE()

#define HAL_INT_ENABLE()					HAL_M_INT_ENABLE()
#define HAL_INT_DISABLE()					HAL_M_INT_DISABLE()

#define HAL_IRQ_ENABLE(vector)                        HAL_M_IRQ_ENABLE(vector)
#define HAL_IRQ_DISABLE(vector)                       HAL_M_IRQ_DISABLE(vector)
#define HAL_IRQ_THRESHOLD(threshold)                  HAL_M_IRQ_THRESHOLD(threshold)
#define HAL_IRQ_CLAIM()                               HAL_M_IRQ_CLAIM()
#define HAL_IRQ_COMPLETE(source)                      HAL_M_IRQ_COMPLETE(source)

#endif

#if IRQ_TARGET == IRQ_TARGET_U

#define HAL_SI_ENABLE()                                HAL_USI_ENABLE()
#define HAL_SI_DISABLE()                               HAL_USI_DISABLE()

#define HAL_TI_ENABLE()                                HAL_UTI_ENABLE()
#define HAL_TI_DISABLE()                               HAL_UTI_DISABLE()

#define HAL_EI_ENABLE()                                HAL_UEI_ENABLE()
#define HAL_EI_DISABLE()                               HAL_UEI_DISABLE()

#define HAL_INT_ENABLE()					HAL_U_INT_ENABLE()
#define HAL_INT_DISABLE()					HAL_U_INT_DISABLE()

#define HAL_IRQ_ENABLE(vector)                        HAL_U_IRQ_ENABLE(vector)
#define HAL_IRQ_DISABLE(vector)                       HAL_U_IRQ_DISABLE(vector)
#define HAL_IRQ_THRESHOLD(threshold)                  HAL_U_IRQ_THRESHOLD(threshold)
#define HAL_IRQ_CLAIM()                               HAL_U_IRQ_CLAIM()
#define HAL_IRQ_COMPLETE(source)                      HAL_U_IRQ_COMPLETE(source)

#endif


#ifdef __cplusplus
}
#endif

#endif	/* __INTERRUPT_H__ */

