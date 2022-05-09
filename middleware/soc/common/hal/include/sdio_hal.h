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

#include "hal_config.h"
#include "sdio_hw.h"
#include "sdio_ll.h"
//#include <driver/hal/hal_sdio_types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	SDIO_HOST_MODE,
	SDIO_SLAVE_MODE,
}sdio_host_slave_mode_t;

void sdio_hal_set_host_slave_mode(sdio_host_slave_mode_t mode);

bool sdio_hal_slave_get_cmd_response_end_int(void);

void sdio_hal_slave_clear_cmd_response_end_int(void);

void sdio_hal_slave_cmd_start(uint32_t value);

uint32_t sdio_hal_get_read_ready(void);
uint32_t sdio_hal_get_write_ready(void);

uint32_t sdio_hal_slave_get_cmd_arg0(void);
uint32_t sdio_hal_slave_read_data(void);
void sdio_hal_slave_write_data(uint32_t value);

uint32_t sdio_hal_get_int_status(void);

uint32_t sdio_hal_slave_get_read_int_status(void);

void sdio_hal_slave_clear_read_int_status(void);

uint32_t sdio_hal_slave_get_rx_count(void);

	void sdio_hal_slave_notify_host_next_block(void);

	uint32 sdio_hal_slave_get_func_reg_value(void);

	void sdio_hal_slave_set_tx_length(uint32_t len);
	void sdio_hal_slave_clear_stop(void);
	void sdio_hal_slave_tx_transaction_en(void);
	void sdio_hal_slave_rx_clear_host_wait_write_data(void);

#ifdef __cplusplus
}
#endif

