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

#include <driver/hal/hal_qspi_types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	uint8_t *cmd;
	uint8_t cmd_len;		// cmd is 1~8 bytes
} qspi_register_write_config_t;


typedef struct {
	uint8_t *cmd;
	uint8_t cmd_len;        // cmd is 1~8 bytes
	uint8_t *reg_rdata;
	uint8_t reg_rdata_len;  // rdata 0~64 bytes

	uint8_t data_line;
	uint8_t dummy_mode;
	uint8_t dummy_clk;
} qspi_register_read_config_t;


#ifdef __cplusplus
}
#endif


