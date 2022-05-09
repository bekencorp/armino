// Copyright 2020-2022 Beken
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

#ifndef _spinlock_h_
#define _spinlock_h_

#ifdef __cplusplus
extern "C" {
#endif

#include <common/bk_typedef.h>

typedef struct
{
	u32     locked;
	void  * owner_cpu;
} spinlock_t;

void spinlock_init(spinlock_t *slock);
void spinlock_acquire(spinlock_t *slock);
void spinlock_release(spinlock_t *slock);

#ifdef __cplusplus
}
#endif

#endif /* _spinlock_h_ */

