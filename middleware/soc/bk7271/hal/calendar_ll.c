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

#include "calendar_ll.h"

static uint32_t s_init_seconds = 0;

void calendar_ll_init(calendar_hw_t *hw)
{
	hw->init_val = 0;
	s_init_seconds = hw->cnt_val_s;
}

uint32_t calendar_ll_get_second_cnt(calendar_hw_t *hw)
{
	/* bk7271 calendar rtc_cnt_s not equal zero when calendar init, e.g.0x28A37141
	 * software need clear init_seconds_value
	 */
	return (hw->cnt_val_s - s_init_seconds);
}

