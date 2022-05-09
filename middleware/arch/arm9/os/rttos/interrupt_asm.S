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

#define NOINT   0xC0

//rt_base_t rt_hw_interrupt_disable();
.globl rt_hw_interrupt_disable
.type rt_hw_interrupt_disable, %function
rt_hw_interrupt_disable:
	MRS     R0, CPSR
	ORR     R1, R0, #NOINT
	MSR     CPSR_c, R1
	BX      LR

//void rt_hw_interrupt_enable(rt_base_t level);
.globl rt_hw_interrupt_enable
.type rt_hw_interrupt_enable, %function
rt_hw_interrupt_enable:
	MSR     CPSR_c, R0
	BX      LR
