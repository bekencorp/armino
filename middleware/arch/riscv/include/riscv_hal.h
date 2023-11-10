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

#include "rv_interrupt.h"
#include <common/bk_err.h>
/**
 * @brief get the uninitialized 1k sram base address
 *
 * get the uninitialized 1k sram base address
 *
 * @attention
 *-This API is used to get the uninitialized 1k sram base address
 *
 * @param
 * none
 * @return
 *-the base address of uninitialized 1k sram
 *
 */
bk_err_t bk_arch_uninitialized_1k_sram_base_addr_get();
#endif //#ifndef __RISCV_HAL_H__
