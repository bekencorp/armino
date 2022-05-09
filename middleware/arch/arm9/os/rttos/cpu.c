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

#include <rthw.h>
#include <rtthread.h>

RT_WEAK void machine_reset(void)
{
    rt_kprintf("reboot system...\n");
    rt_hw_interrupt_disable();
    while(1);
}

RT_WEAK void machine_shutdown(void)
{
    rt_kprintf("shutdown...\n");
	rt_hw_interrupt_disable();
    while(1);
}

void rtos_shutdown()
{
    uint32_t level;
    rt_kprintf("shutdown...\n");

    level = rt_hw_interrupt_disable();
    machine_shutdown();
    while (level)
    {
        RT_ASSERT(0);
    }
}

#ifdef RT_USING_CPU_FFS
/**
 * This function finds the first bit set (beginning with the least significant bit)
 * in value and return the index of that bit.
 *
 * Bits are numbered starting at 1 (the least significant bit).  A return value of
 * zero from any of these functions means that the argument was zero.
 *
 * @return return the index of the first bit set. If value is 0, then this function
 * shall return 0.
 */
#if defined(__CC_ARM)
int __rt_ffs(int value)
{
    register uint32_t x;

    if (value == 0)
        return value;

    __asm
    {
        rsb x, value, #0
        and x, x, value
        clz x, x
        rsb x, x, #32
    }

    return x;
}
#elif defined(__GNUC__) || defined(__ICCARM__)
int __rt_ffs(int value)
{
    return __builtin_ffs(value);
}
#endif

#endif

