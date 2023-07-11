// Copyright 2022-2023 Beken
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

//RISC-V has mtimer which clock is 26M and it doesn't change frequency.
//as delay_us is just compare risc-v timer which block system runs, so the param
//of us should be not too big. F.E: < 100*000

#include "arch_delay.h"

void arch_delay_us(uint32_t us)
{
        extern u64 riscv_get_mtimer(void);

        uint64_t tick_cnt = us * 26;
        uint64_t start_tick = riscv_get_mtimer();

        while((riscv_get_mtimer() - start_tick) < tick_cnt)
        {
                ;
        }
}
