// Copyright 2022-2025 Beken
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

#ifndef __RISCV_HAL_H__
#define __RISCV_HAL_H__

#include "platform.h"

// For liteos_m_3.0.1 LTS riscv
#define RISCV_PLIC_VECTOR_CNT   (63)
#define RISCV_SYS_MAX_IRQ          (0)

#define MTIMER                                 (0xE6000000)
#define MTIMERCMP                          (0xE6000008)

#define HalIrqDisable(irq_no) 	clear_csr(NDS_MIE, (1<<irq_no))
#define HalIrqEnable(irq_no)        set_csr(NDS_MIE, (1<<irq_no))
#define HalSetLocalInterPri(irq_no, irq_pri)    __nds__plic_set_priority(irq_no, irq_pri)

#endif //#ifndef __RISCV_HAL_H__
