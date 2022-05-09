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

//TODO more optimization for APIs in this headers

#ifdef __cplusplus
extern "C" {
#endif

uint8_t phy_open_cca(void);
uint8_t phy_close_cca(void);
uint8_t phy_show_cca(void);

void cmd_rfcali_cfg_mode(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
void cmd_rfcali_cfg_rate_dist(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
void cmd_rfcali_cfg_tssi_g(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
void cmd_rfcali_cfg_tssi_b(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
void cmd_rfcali_show_data(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
void cmd_cali(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
void cmd_psram(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

void tx_evm_cmd_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
void rx_sens_cmd_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

int bkreg_run_command(const char *content, int cnt);

//TODO move to better place
void bk7011_cal_bias(void);

int manual_cal_get_macaddr_from_flash(uint8_t *mac_ptr);
int manual_cal_write_macaddr_to_flash(uint8_t *mac_ptr);
void manual_cal_temp_pwr_unint(void);

int do_evm(void *cmdtp, int flag, int argc, char *const argv[]);
int do_rx_sensitivity(void *cmdtp, int flag, int argc, char *const argv[]);

#define LA_SAMPLE_TEST 1    
void cmd_la_sample_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);


#ifdef __cplusplus
}
#endif
