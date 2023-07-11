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

#include "arch_delay.h"

void arch_delay_us(uint32_t us)
{
        uint32_t ret;
        uint32_t div;
        uint32_t clk = 0;
        uint32_t cell;
        SYS_CTRL_U param;

        ret = sddev_control(DD_DEV_TYPE_SCTRL, CMD_GET_SCTRL_CONTROL, &param);
        BK_ASSERT(SCTRL_SUCCESS == ret); /* ASSERT VERIFIED */

        div = param.bits.mclk_div;
        switch (param.bits.mclk_mux) {
        case MCLK_MODE_DCO:
        case MCLK_MODE_26M_XTAL:
                clk = 26000000;
                break;

        case MCLK_MODE_DPLL:
                clk = 480000000 / (2 << div);
                break;

        case MCLK_MODE_LPO:
                clk = 32000;
                break;

        default:
                break;
        }

        BK_ASSERT(clk); /* ASSERT VERIFIED */

        cell = 100 * clk / 26000;
        delay(ms_count * cell);
}

