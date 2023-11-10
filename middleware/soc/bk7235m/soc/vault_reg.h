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

#define EIP_DEBUG		(*((volatile unsigned long *) (SOC_VAULT_REG_BASE + 0x0*4)))
#define EIP_CTRL		(*((volatile unsigned long *) (SOC_VAULT_REG_BASE + 0x1*4)))
#define EIP_STATUS		(*((volatile unsigned long *) (SOC_VAULT_REG_BASE + 0x2*4)))
#define OTP_VDD25_CTRL	(*((volatile unsigned long *) (SOC_VAULT_REG_BASE + 0x3*4)))

// Definition of static resources inside the device
#define EIP_FIRST_OFS		0x4B000000
#define EIP_LAST_OFS		0x4B003FFF

#ifdef __cplusplus
}
#endif

