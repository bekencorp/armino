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

#ifdef __cplusplus
extern "C" {
#endif

#include <soc/soc.h>

//MBOX0  cpu0  to cpu1
//MBOX1  cpu1  to cpu0
#define MAILBOX_BASE					(SOC_MBOX0_REG_BASE)

/*
IRQ_MAILBOX1						(49)
IRQ_MAILBOX0						(48)
IRQ_MAILBOX1_BIT					(1<<49)
IRQ_MAILBOX0_BIT					(1<<48)
*/


#ifdef __cplusplus
}
#endif
// eof

