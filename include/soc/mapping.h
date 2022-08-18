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

#include <sdkconfig.h>

#if CONFIG_SOC_BK7231N
#include "soc/bk7231n/mapping.h"
#elif CONFIG_SOC_BK7231U
#include "soc/bk7231u/mapping.h"
#elif CONFIG_SOC_BK7236
#include "soc/bk7236/mapping.h"
#elif CONFIG_SOC_BK7236A
#include "soc/bk7236a/mapping.h"
#elif CONFIG_SOC_BK7251
#include "soc/bk7251/mapping.h"
#elif CONFIG_SOC_BK7271
#include "soc/bk7271/mapping.h"

#elif CONFIG_SOC_BK7256 && !CONFIG_DUAL_CORE
#include "soc/bk7256/mapping.h"
#elif CONFIG_SOC_BK7256 && CONFIG_MASTER_CORE
#include "soc/bk7256/mapping.h"
#elif CONFIG_SOC_BK7256 && CONFIG_SLAVE_CORE
#include "soc/bk7256_cp1/mapping.h"

#elif CONFIG_SOC_BK7237 && !CONFIG_DUAL_CORE
#include "soc/bk7237/mapping.h"
#elif CONFIG_SOC_BK7237 && CONFIG_MASTER_CORE
#include "soc/bk7237/mapping.h"
#elif CONFIG_SOC_BK7237 && CONFIG_SLAVE_CORE
#include "soc/bk7237_cp1/mapping.h"

#elif CONFIG_SOC_BK7235 && !CONFIG_DUAL_CORE
#include "soc/bk7235/mapping.h"
#elif CONFIG_SOC_BK7235 && CONFIG_MASTER_CORE
#include "soc/bk7235/mapping.h"
#elif CONFIG_SOC_BK7235 && CONFIG_SLAVE_CORE
#include "soc/bk7235_cp1/mapping.h"

#endif

