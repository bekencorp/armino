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

#include <components/log.h>

#define CHIP_SUPPORT_TAG "chip_support"
#define CHIP_SUPPORT_LOGI(...) BK_LOGI(CHIP_SUPPORT_TAG, ##__VA_ARGS__)
#define CHIP_SUPPORT_LOGW(...) BK_LOGW(CHIP_SUPPORT_TAG, ##__VA_ARGS__)
#define CHIP_SUPPORT_LOGE(...) BK_LOGE(CHIP_SUPPORT_TAG, ##__VA_ARGS__)
#define CHIP_SUPPORT_LOGD(...) BK_LOGD(CHIP_SUPPORT_TAG, ##__VA_ARGS__)
#define CHIP_SUPPORT_LOGD(...) BK_LOGD(CHIP_SUPPORT_TAG, ##__VA_ARGS__)

bool bk_is_chip_supported(void);

