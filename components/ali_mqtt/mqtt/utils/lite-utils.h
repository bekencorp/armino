/*
 * Copyright (c) 2014-2016 Alibaba Group. All rights reserved.
 * License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */


#ifndef __LITE_UTILS_H__
#define __LITE_UTILS_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
// #if defined(_PLATFORM_IS_LINUX_)
//     #include <assert.h>
// #endif

// #include "lite-list.h"
#include <components/log.h>
#include <common/bk_assert.h>
#include <os/mem.h>

#ifndef UTILS_printf
#define UTILS_printf                bk_printf
#endif

#define LITE_TRUE                   (1)
#define LITE_FALSE                  (0)



#define LITE_MINIMUM(a, b)          (((a) <= (b)) ? (a) : (b))
#define LITE_MAXIMUM(a, b)          (((a) >= (b)) ? (a) : (b))
#define LITE_isdigit(c)             (((c) <= '9' && (c) >= '0') ? (LITE_TRUE) : (LITE_FALSE))


#define LITE_ASSERT(expr)           BK_ASSERT(expr)




#endif  /* __LITE_UTILS_H__ */
