// Copyright 2020-2021 Beken
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

#pragma once

typedef void (*hook_func)(void);

void arch_dump_exception_stack(void);
void stack_mem_dump(uint32_t stack_top, uint32_t stack_bottom);

void rtos_regist_wifi_dump_hook(hook_func wifi_func);
void rtos_regist_ble_dump_hook(hook_func ble_func);
void rtos_regist_plat_dump_hook(uint32_t reg_base_addr, uint32_t reg_size);