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

#define SOC_PWM_UNIT_NUM               1
#if (CONFIG_PWM_DEVICE0_SUPPORT && CONFIG_PWM_DEVICE1_SUPPORT)
#define SOC_PWM_GROUP_NUM              2
#elif (CONFIG_PWM_DEVICE0_SUPPORT || CONFIG_PWM_DEVICE1_SUPPORT)
#define SOC_PWM_GROUP_NUM              1
#endif
#define SOC_PWM_CHAN_NUM_PER_UNIT      6
#define SOC_PWM_CHAN_NUM_PER_GROUP     6

#ifdef __cplusplus
}
#endif
