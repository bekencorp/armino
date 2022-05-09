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

#include <common/bk_include.h>
#include <common/bk_err.h>
#include <driver/hal/hal_int_types.h>


typedef enum {
	ARM_INT_GROUP_IRQ = 0,
	ARM_INT_GROUP_FIQ ,
}arm_int_group_t;

void arch_register_group_isr(uint32_t group, int_group_isr_t isr);
bk_err_t arch_isr_entry_init(void);


extern void intc_irq(void);
extern void intc_fiq(void);
extern void do_irq(void);
extern void do_fiq(void);
extern void do_swi(void);
extern void do_undefined(void);
extern void do_pabort(void);
extern void do_dabort(void);
extern void do_reserved(void);

