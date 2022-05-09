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

#include <soc/soc.h>
#include "mailbox_hw.h"


#ifdef __cplusplus
extern "C" {
#endif

#define MAILBOX_LL_REG_BASE(id)  (MAILBOX_BASE + 0x40 * (id))


typedef struct {
	s_mailbox_t *hw;
	mailbox_id_t id;
} mailbox_hal_t;

typedef enum {
	MAILBOX_BOX0 = 0,
	MAILBOX_BOX1 = 1,
	MAILBOX_NONE,
} mailbox_box_num_t;

static inline void mailbox_ll_addr_init(mailbox_hal_t *hal)
{
	if (hal->id == 0)
		hal->hw = (s_mailbox_t *)MAILBOX_LL_REG_BASE(hal->id);
	else if (hal->id == 1)
		hal->hw = (s_mailbox_t *)MAILBOX_LL_REG_BASE(hal->id);
	else if (hal->id == 2)
		hal->hw = (s_mailbox_t *)MAILBOX_LL_REG_BASE(hal->id);
	else if (hal->id == 3)
		hal->hw = (s_mailbox_t *)MAILBOX_LL_REG_BASE(hal->id);
}

static inline void mailbox_ll_set_identity(s_mailbox_t *hw, uint32_t id)
{
	return;
}

static inline uint32_t mailbox_ll_get_sender(s_mailbox_t *hw, uint32_t id)
{
	return BK_OK;
}

static inline uint32_t mailbox_ll_get_receiver(s_mailbox_t *hw, uint32_t id)
{
	return BK_OK;
}

static inline void mailbox_ll_box0_init(s_mailbox_t *hw)
{
	hw->box0_param0 = 0;
	hw->box0_param1 = 0;
	hw->box0_param2 = 0;
	hw->box0_param3 = 0;
}

static inline void mailbox_ll_box1_init(s_mailbox_t *hw)
{
	hw->box1_param0 = 0;
	hw->box1_param1 = 0;
	hw->box1_param2 = 0;
	hw->box1_param3 = 0;

}

static inline void mailbox_ll_write_param0(s_mailbox_t *hw, uint32_t param0, uint32_t box)
{
	if (box == 0)
		hw->box0_param0 = param0;
	else
		hw->box1_param0 = param0;
}
static inline void mailbox_ll_write_param1(s_mailbox_t *hw, uint32_t param1, uint32_t box)
{
	if (box == 0)
		hw->box0_param1 = param1;
	else
		hw->box1_param1 = param1;
}
static inline void mailbox_ll_write_param2(s_mailbox_t *hw, uint32_t param2, uint32_t box)
{
	if (box == 0)
		hw->box0_param2 = param2;
	else
		hw->box1_param2 = param2;
}
static inline void mailbox_ll_write_param3(s_mailbox_t *hw, uint32_t param3, uint32_t box)
{
	if (box == 0)
		hw->box0_param3 = param3;
	else
		hw->box1_param3 = param3;
}

static inline uint32_t mailbox_ll_read_param0(s_mailbox_t *hw, uint32_t box)
{
	if (box == 0)
		return hw->box0_param0;
	else
		return hw->box1_param0;
}
static inline uint32_t mailbox_ll_read_param1(s_mailbox_t *hw, uint32_t box)
{
	if (box == 0)
		return hw->box0_param1;
	else
		return hw->box1_param1;
}
static inline uint32_t mailbox_ll_read_param2(s_mailbox_t *hw, uint32_t box)
{
	if (box == 0)
		return hw->box0_param2;
	else
		return hw->box1_param2;

}
static inline uint32_t mailbox_ll_read_param3(s_mailbox_t *hw, uint32_t box)
{
	if (box == 0)
		return hw->box0_param3;
	else
		return hw->box1_param3;

}

static inline void mailbox_ll_box_trigger_ready(s_mailbox_t *hw, uint32_t box)
{
	if (box == 0)
		hw->box_ready.box0_ready = 0;
	else
		hw->box_ready.box1_ready = 0;
}

static inline void mailbox_ll_box_trigger(s_mailbox_t *hw, uint32_t box)
{
	if (box == 0)
		hw->box_ready.box0_ready = 1;
	else
		hw->box_ready.box1_ready = 1;
}

static inline uint32_t mailbox_ll_read_box_ready(s_mailbox_t *hw, uint32_t box)
{
	if (box == 0)
		return hw->box_ready.box0_ready;
	else
		return hw->box_ready.box1_ready;
}

static inline void mailbox_ll_box_clear(s_mailbox_t *hw, uint32_t box)
{
	if (box == 0)
		hw->box_clear.box0_clear = 1;
	else
		hw->box_clear.box1_clear = 1;
}

static inline void mailbox_ll_box_clear_ready(s_mailbox_t *hw, uint32_t box)
{
	if (box == 0)
		hw->box_clear.box0_clear = 0;
	else
		hw->box_clear.box1_clear = 0;
}

static inline uint32_t mailbox_ll_read_box_erasure(s_mailbox_t *hw, uint32_t box)
{
	if (box == 0)
		return hw->box_clear.box0_clear;
	else
		return hw->box_clear.box1_clear;

}

static inline void mailbox_ll_box_init(s_mailbox_t *hw)
{
	mailbox_ll_box0_init(hw);
	mailbox_ll_box1_init(hw);
	mailbox_ll_box_trigger_ready(hw, MAILBOX_BOX0);
	mailbox_ll_box_trigger_ready(hw, MAILBOX_BOX1);
	mailbox_ll_box_clear_ready(hw, MAILBOX_BOX0);
	mailbox_ll_box_clear_ready(hw, MAILBOX_BOX1);

}

#ifdef __cplusplus
}
#endif
