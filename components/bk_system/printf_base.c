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

#include <stdarg.h>
#include <driver/uart.h>
#include "bk_uart.h"
#include <common/sys_config.h>
#include <os/mem.h>
#include "printf_impl.h"

static bool s_printf_init = false;

bool printf_is_init(void)
{
	return s_printf_init;
}

bk_err_t bk_printf_deinit(void)
{
#if (!CONFIG_SLAVE_CORE)
	bk_uart_deinit(CONFIG_UART_PRINT_PORT);
#endif
	printf_lock_deinit();
	return BK_OK;
}

bk_err_t bk_printf_init(void)
{
	int ret;

#if (!CONFIG_SLAVE_CORE)
        const uart_config_t config = {
                .baud_rate = UART_BAUD_RATE,
                .data_bits = UART_DATA_8_BITS,
                .parity = UART_PARITY_NONE,
                .stop_bits = UART_STOP_BITS_1,
                .flow_ctrl = UART_FLOWCTRL_DISABLE,
                .src_clk = UART_SCLK_XTAL_26M
        };
#endif

	ret = printf_lock_init();
        if (BK_OK != ret) {
                return ret;
        }

#if (!CONFIG_SLAVE_CORE)
        ret = bk_uart_init(CONFIG_UART_PRINT_PORT, &config);
        if (BK_OK != ret)
		goto _bk_printf_init_fail;

#endif
	s_printf_init = true;

	return BK_OK;

#if (!CONFIG_SLAVE_CORE)
_bk_printf_init_fail:
	bk_printf_deinit();
	return ret;
#endif
}

void bk_null_printf(const char *fmt, ...)
{
}
