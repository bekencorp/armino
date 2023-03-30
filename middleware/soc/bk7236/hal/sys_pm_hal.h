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

#include <soc/soc.h>
#include "sys_pm_hal_debug.h"
#include "sys_pm_hal_ctrl.h"

//TODO may need to adjust the interrupt bits
#define WS_WIFI_INT0    (BIT(29) | BIT(30) | BIT(31))
#define WS_WIFI_INT1    (BIT(0) | BIT(1) | BIT(2) | BIT(3) | BIT(4) | BIT(5) | BIT(6))
#define WS_BT_INT       (BIT(8) | BIT(9))
#define WS_TOUCH_INT    BIT(20)
#define WS_USBPLUG_INT  BIT(21)
#define WS_RTC_INT      BIT(22)
#define WS_GPIO_INT     BIT(23)

#define WS_CPU0_INT_0_31_MASK  WS_WIFI_INT0
#define WS_CPU0_INT_32_64_MASK (WS_WIFI_INT1 | WS_BT_INT | WS_TOUCH_INT | WS_USBPLUG_INT | WS_RTC_INT | WS_GPIO_INT)
