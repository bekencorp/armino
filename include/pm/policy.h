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

#pragma once

const pm_state_info_t *pm_policy_next_state(int32_t ticks);
/* Device lock/unlock don't require to be used in pair, also they
 * provide more detailed debug info, but need more memory.
 * 
 * Generally speaking, we should use them.
 * */
int pm_policy_device_unlock_state(const device_t *dev, pm_state_t state);
int pm_policy_device_lock_state(const device_t *dev, pm_state_t state);

/* Lock/unlock require to be used in pair, need less memory.
 */
int pm_policy_unlock_state(pm_state_t state);
int pm_policy_lock_state(pm_state_t state);
void pm_policy_dump(void);

//TODO support pm policy latency API
