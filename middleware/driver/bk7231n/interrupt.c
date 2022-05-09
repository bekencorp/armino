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
#include "icu_driver.h"
#include "interrupt_base.h"
#include <driver/int.h>

void fiq_isr(void)
{
	uint32_t fiq_status;

	fiq_status = icu_get_fiq_int_status();
	icu_clear_fiq_int_status(fiq_status);

	if (0 == fiq_status)
		ICU_LOGE("fiq:dead\r\n");

	group_isr(ARM_INT_GROUP_FIQ, fiq_status);
}

void irq_isr(void)
{
	uint32_t irq_status;

	irq_status = icu_get_irq_int_status();
	icu_clear_irq_int_status(irq_status);
	if (0 == irq_status)
		ICU_LOGE("irq:dead\r\n");

	group_isr(ARM_INT_GROUP_IRQ, irq_status);
}

void soc_isr_init(void)
{
	arch_isr_entry_init();
	bk_icu_driver_init();

	arch_register_group_isr(ARM_INT_GROUP_FIQ, fiq_isr);
	arch_register_group_isr(ARM_INT_GROUP_IRQ, irq_isr);
}

void soc_isr_deinit(void)
{
	bk_icu_driver_deinit();
	arch_register_group_isr(ARM_INT_GROUP_FIQ, NULL);
	arch_register_group_isr(ARM_INT_GROUP_IRQ, NULL);
}
