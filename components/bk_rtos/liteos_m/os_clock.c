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

#include <common/bk_include.h>
#include "los_tick.h"

void os_tick_hdl(void)
{
}

int os_update_tick(uint32_t tick)
{
	return 0;
}

uint64_t os_get_tick(void)
{
	return LOS_TickCountGet();
}

uint32_t os_get_second(void)
{
	return (LOS_Tick2MS(LOS_TickCountGet()) / 1000);
}

