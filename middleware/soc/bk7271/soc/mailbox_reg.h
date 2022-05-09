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

#define MAILBOX_BASE						SOC_MAILBOX_REG_BASE

//MailBox cpu to dsp
//MailBox dsp to cpu
//MailBox cpu to bt
//MailBox bt to cpu

#define IRQ_MAILBOX_BT                      IRQ_MAILBOX2
#define IRQ_MAILBOX_DSP                     IRQ_MAILBOX1
#define IRQ_MAILBOX_BT_BIT                  IRQ_MAILBOX2_BIT
#define IRQ_MAILBOX_DSP_BIT                 IRQ_MAILBOX1_BIT

// eof

