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

//TODO place all auto-generated Registers here!!!

#define PWM_R_BASE                  (SOC_PWM_REG_BASE)

#define PWM_R_CTRL                  (PWM_R_BASE)

#define PWM_R_INT_STATUS            (PWM_R_BASE + 4 * 0x1)

#define PWM_F_EN                    (BIT(0))
#define PWM_F_EN_M                  (BIT(0))
#define PWM_F_EN_V                  0x1
#define PWM_F_EN_S                  0
#define PWM_F_EN_MS(_ch)            (PWM_F_EN_S + 4 * (_ch))

#define PWM_F_INT_EN                (BIT(1))
#define PWM_F_INT_EN_M              (BIT(1))
#define PWM_F_INT_EN_V              0x1
#define PWM_F_INT_EN_S              0x1
#define PWM_F_INT_EN_MS(_ch)        (PWM_F_INT_EN_S + 4 * (_ch))

#define PWM_F_MODE                  0x3
#define PWM_F_MODE_M                ((PWM_F_CHAN_MODE_V) << (PWM_F_CHAN0_MODE_S))
#define PWM_F_MODE_V                0x3
#define PWM_F_MODE_S                0x2
#define PWM_F_MODE_MS(_ch)          (0x2 + 4 * (_ch))
#define PWM_V_MODE_PWM              0x0
#define PWM_V_MODE_TIMER            0x1
#define PWM_V_MODE_CAPTURE_EDGE_POS 0x2
#define PWM_V_MODE_CAPTURE_EDGE_NEG 0x3

#define PWM_R_INT_ST_CAPTURE_EDGE  (PWM_R_BASE + (0x4 * 1))

#define PWM_F_INT_ST                (BIT(0))
#define PWM_F_INT_ST_M              (BIT(0))
#define PWM_F_INT_ST_V              0x1
#define PWM_F_INT_ST_S              0
#define PWM_F_INT_ST_MS(_ch)        (PWM_F_INT_ST_S + 1 * (_ch))

#ifdef __cplusplus
}
#endif
