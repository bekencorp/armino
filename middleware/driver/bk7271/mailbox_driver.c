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

#include <components/log.h>
#include <common/bk_include.h>
#include "icu_driver.h"
#include "mailbox_driver.h"

void mailbox_interrupt_enable(mailbox_endpoint_t src)
{
	switch(src) {
		case MAILBOX_DSP:
			icu_enable_mailbox_dsp_interrupt();
			break;
		case MAILBOX_BT:
			icu_enable_mailbox_bt_interrupt();
			break;
		default:
			break;
		}
}

void mailbox_interrupt_disable(mailbox_endpoint_t src)
{
	switch(src) {
		case MAILBOX_DSP:
			icu_disable_mailbox_dsp_interrupt();
			break;
		case MAILBOX_BT:
			icu_disable_mailbox_bt_interrupt();
			break;
		default:
			break;
		}
}

