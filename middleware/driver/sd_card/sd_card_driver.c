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

#include <os/mem.h>
#include <driver/sdio_host.h>
#include <driver/sd_card.h>
#include "sd_card_driver.h"

#define SD_CARD_MAX_VOLT_TRIAL_COUNT    0xff

#if (CONFIG_SOC_BK7256XX)	//Temp code, clock module should re-arch.
#define CMD_TIMEOUT_100K	2500
#define DATA_TIMEOUT_100K	10000

#define CMD_TIMEOUT_6_5_M	300000 //about 150ns per cycle (45ms)
#define DATA_TIMEOUT_6_5_M  3000000 //450ms

#define CMD_TIMEOUT_13M		600000 //about 77ns pr cycle (45ms)
#define DATA_TIMEOUT_13M	6000000 //450ms

#define CMD_TIMEOUT_26M		1200000//about 38ns pr cycle (45ms)
#define DATA_TIMEOUT_26M	12000000 //450ms
#else
#define CMD_TIMEOUT_200K	5000	//about 5us per cycle (25ms)
#define DATA_TIMEOUT_200K	20000 //100ms

#define CMD_TIMEOUT_6_5_M	300000 //about 150ns per cycle (45ms)
#define DATA_TIMEOUT_6_5_M  3000000 //450ms

#define CMD_TIMEOUT_13M		600000 //about 77ns pr cycle (45ms)
#define DATA_TIMEOUT_13M	6000000 //450ms

#define CMD_TIMEOUT_26M		1200000//about 38ns pr cycle (45ms)
#define DATA_TIMEOUT_26M	12000000 //450ms
#endif

typedef struct {
	sd_card_info_t sd_card; /**< sd card information */
	uint32_t cid[4]; /**< sd card CID register, it contains the card identification information */
	uint32_t csd[4];
	sdio_host_clock_freq_t clock_freq;
} sd_card_obj_t;

static sd_card_obj_t s_sd_card_obj = {0};

static uint32 sd_card_get_cmd_timeout_param(void)
{
	uint32 timeout_param;
	switch (s_sd_card_obj.clock_freq) {
#if CONFIG_SOC_BK7256XX	//Temp code, clock module should re-arch.
	case SDIO_HOST_CLK_100K:
		timeout_param = CMD_TIMEOUT_100K;	//CMD_TIMEOUT_200K;	//really is 100K
		break;
#else
	case SDIO_HOST_CLK_200K:
		timeout_param = CMD_TIMEOUT_200K;
		break;
#endif
	case SDIO_HOST_CLK_6_5M:
		timeout_param = CMD_TIMEOUT_6_5_M;
		break;
	case SDIO_HOST_CLK_13M:
		timeout_param = CMD_TIMEOUT_13M;
		break;
	case SDIO_HOST_CLK_26M:
	default:
		timeout_param = CMD_TIMEOUT_26M;
		break;
	}
	return timeout_param;
}

static uint32 sd_card_get_data_timeout_param(void)
{
	uint32 timeout_param;
	switch (s_sd_card_obj.clock_freq) {
#if CONFIG_SOC_BK7256XX	//Temp code, clock module should re-arch.
	case SDIO_HOST_CLK_100K:
		timeout_param = DATA_TIMEOUT_100K;
		break;
#else
	case SDIO_HOST_CLK_200K:
		timeout_param = DATA_TIMEOUT_200K;
		break;
#endif
	case SDIO_HOST_CLK_6_5M:
		timeout_param = DATA_TIMEOUT_6_5_M;
		break;
	case SDIO_HOST_CLK_13M:
		timeout_param = DATA_TIMEOUT_13M;
		break;
	case SDIO_HOST_CLK_26M:
	default:
		timeout_param = DATA_TIMEOUT_26M;
		break;
	}
	return timeout_param;
}

/* CMD0 */
static bk_err_t sd_card_cmd_go_idle_state(void)
{
	bk_err_t error_state = BK_OK;
	sdio_host_cmd_cfg_t cmd_cfg = {0};

	cmd_cfg.cmd_index = SD_CMD_GO_IDLE_STATE;
	cmd_cfg.argument = 0;
	cmd_cfg.response = SDIO_HOST_CMD_RSP_NONE;
	cmd_cfg.wait_rsp_timeout = sd_card_get_cmd_timeout_param();

	bk_sdio_host_send_command(&cmd_cfg);
	error_state = bk_sdio_host_wait_cmd_response(cmd_cfg.cmd_index);
	if (error_state != BK_OK) {
		SD_CARD_LOGW("CMD0 GO_IDLE_STATE err:-%x\r\n", -error_state);
		return error_state;
	}
	SD_CARD_LOGI("sdio host send CMD0(GO_IDLE_STATE) ok\r\n");

	return error_state;
}

/* CMD8 */
static bk_err_t sd_card_cmd_send_if_cond(void)
{
	bk_err_t error_state = 0;
	uint32_t rsp_arg = 0;
	uint8_t voltage_accpet, check_pattern = 0;
	sdio_host_cmd_cfg_t cmd_cfg = {0};

	/* Send CMD8 to verify SD card interface operation condition
	 * Argument:
	 *  - [31:12]: Reserved (shall be set to '0')
	 *  - [11:8]: Support Voltage (VHS) 0x1 (Range: 2.7-3.6 V)
	 *  - [7:0]: Check Pattern (recommand 0xAA)
	 * CMD Response: R7
	 */
	cmd_cfg.cmd_index = SD_CMD_SEND_IF_COND;
	cmd_cfg.argument = SD_CHECK_PATTERN;
	cmd_cfg.response = SDIO_HOST_CMD_RSP_SHORT;
	cmd_cfg.wait_rsp_timeout = sd_card_get_cmd_timeout_param();

	bk_sdio_host_send_command(&cmd_cfg);
	error_state = bk_sdio_host_wait_cmd_response(cmd_cfg.cmd_index);
	if (error_state != BK_OK) {
		SD_CARD_LOGW("CMD8 SEND_IF_COND err:-%x\r\n", -error_state);
		return error_state;
	}
	SD_CARD_LOGI("sdio host send CMD8(SEND_IF_COND) ok\r\n");

	/* Get Response R7 argument
	 * - [11:8]: Voltage accepted
	 * - [7:0]: Echo-back of check pattern
	 * Voltage accepted:
	 *  -0000b: Not Defined
	 *  -0001b: 2.7-3.6 V
	 */
	rsp_arg = bk_sdio_host_get_cmd_rsp_argument(SDIO_HOST_RSP0);
	check_pattern = rsp_arg & 0xff;
	voltage_accpet = (rsp_arg >> 8) & 0xf;

	if (check_pattern == 0xaa && voltage_accpet == 0x1) {
		SD_CARD_LOGI("sd card support voltage 2.7-3.6 V\r\n");
		return BK_OK;
	} else {
		SD_CARD_LOGW("sd card unsupport voltage\r\n");
		return BK_ERR_SDIO_HOST_INVALID_VOLT_RANGE;
	}

	return error_state;
}

/* CMD55 */
static bk_err_t sd_card_cmd_app_cmd(uint32_t argument)
{
	bk_err_t error_state = BK_OK;
	sdio_host_cmd_cfg_t cmd_cfg = {0};

	cmd_cfg.cmd_index = SD_CMD_APP_CMD;
	cmd_cfg.argument = argument;
	cmd_cfg.response = SDIO_HOST_CMD_RSP_SHORT;
	cmd_cfg.wait_rsp_timeout = sd_card_get_cmd_timeout_param();

	bk_sdio_host_send_command(&cmd_cfg);
	error_state = bk_sdio_host_wait_cmd_response(cmd_cfg.cmd_index);
	if (error_state != BK_OK) {
		SD_CARD_LOGW("CMD55 APP_CMD err:-%x\r\n", -error_state);
		return error_state;
	}
	SD_CARD_LOGI("sdio host send CMD55(APP_CMD) ok\r\n");

	return error_state;
}

/* ACMD41
 * Send the command asking the accessed card to send its operating condition register (OCR)
 */
static bk_err_t sd_card_cmd_sd_send_op_cond(uint32_t ocr)
{
	bk_err_t error_state = BK_OK;
	sdio_host_cmd_cfg_t cmd_cfg = {0};

	cmd_cfg.cmd_index = SD_CMD_SD_SEND_OP_COND;
	cmd_cfg.argument = ocr;
	cmd_cfg.response = SDIO_HOST_CMD_RSP_SHORT;
	cmd_cfg.wait_rsp_timeout = sd_card_get_cmd_timeout_param();

	bk_sdio_host_send_command(&cmd_cfg);
	error_state = bk_sdio_host_wait_cmd_response(cmd_cfg.cmd_index);
	// why acmd41 always return crc fail?
	// TODO
	if (error_state != BK_OK) {
		SD_CARD_LOGW("ACMD41 SD_SEND_OP_COND err:-%x\r\n", -error_state);
		return error_state;
	}
	SD_CARD_LOGI("sdio host send ACMD41(SD_SEND_OP_COND) ok\r\n");

	return error_state;
}

/* CMD2 */
static bk_err_t sd_card_cmd_all_send_cid(void)
{
	bk_err_t error_state = BK_OK;
	sdio_host_cmd_cfg_t cmd_cfg = {0};

	cmd_cfg.cmd_index = SD_CMD_ALL_SEND_CID;
	cmd_cfg.argument = 0;
	cmd_cfg.response = SDIO_HOST_CMD_RSP_LONG;
	cmd_cfg.wait_rsp_timeout = sd_card_get_cmd_timeout_param();

	bk_sdio_host_send_command(&cmd_cfg);
	error_state = bk_sdio_host_wait_cmd_response(cmd_cfg.cmd_index);
	if (error_state != BK_OK) {
		SD_CARD_LOGW("CMD2 ALL_SEND_CID err:-%x\r\n", -error_state);
		return error_state;
	}
	SD_CARD_LOGI("sdio host send CMD2(ALL_SEND_CID) ok\r\n");

	return error_state;
}

/* CMD3 */
static bk_err_t sd_card_cmd_send_relative_addr(uint16_t *rca)
{
	bk_err_t error_state = BK_OK;
	uint32_t rsp_arg = 0;
	sdio_host_cmd_cfg_t cmd_cfg = {0};

	cmd_cfg.cmd_index = SD_CMD_SEND_RELATIVE_ADDR;
	cmd_cfg.argument = 0;
	cmd_cfg.response = SDIO_HOST_CMD_RSP_SHORT;
	cmd_cfg.wait_rsp_timeout = sd_card_get_cmd_timeout_param();

	bk_sdio_host_send_command(&cmd_cfg);
	error_state = bk_sdio_host_wait_cmd_response(cmd_cfg.cmd_index);
	if (error_state != BK_OK) {
		SD_CARD_LOGW("CMD3 SEND_RELATIVE_ADDR err:-%x\r\n", -error_state);
		return error_state;
	}
	SD_CARD_LOGI("sdio host send CMD3(SEND_RELATIVE_ADDR) ok\r\n");

	/* R6
	 * BIT[0:15] card status bits
	 * BIT[16:31] new published RCA
	 */
	rsp_arg = bk_sdio_host_get_cmd_rsp_argument(SDIO_HOST_RSP0);
	*rca = (rsp_arg >> 16);

	return error_state;
}

/* CMD9 */
static bk_err_t sd_card_cmd_send_csd(uint32_t argument)
{
	bk_err_t error_state = BK_OK;
	sdio_host_cmd_cfg_t cmd_cfg = {0};

	cmd_cfg.cmd_index = SD_CMD_SEND_CSD;
	cmd_cfg.argument = argument;
	cmd_cfg.response = SDIO_HOST_CMD_RSP_SHORT;
	cmd_cfg.wait_rsp_timeout = sd_card_get_cmd_timeout_param();

	bk_sdio_host_send_command(&cmd_cfg);
	error_state = bk_sdio_host_wait_cmd_response(cmd_cfg.cmd_index);
	if (error_state != BK_OK) {
		SD_CARD_LOGW("CMD9 SEND_CSD err:-%x\r\n", -error_state);
		return error_state;
	}
	SD_CARD_LOGI("sdio host send CMD9(SEND_CSD) ok\r\n");

	return error_state;
}

/* CMD7 */
static bk_err_t sd_card_cmd_select_deselect_card(uint32_t addr)
{
	bk_err_t error_state = BK_OK;
	sdio_host_cmd_cfg_t cmd_cfg = {0};

	cmd_cfg.cmd_index = SD_CMD_SELECT_DESELECT_CARD;
	cmd_cfg.argument = addr;
	cmd_cfg.response = SDIO_HOST_CMD_RSP_SHORT;
	cmd_cfg.wait_rsp_timeout = sd_card_get_cmd_timeout_param();

	bk_sdio_host_send_command(&cmd_cfg);
	error_state = bk_sdio_host_wait_cmd_response(cmd_cfg.cmd_index);
	if (error_state != BK_OK) {
		SD_CARD_LOGW("CMD7 SELECT_DESELECT_CARD err:-%x\r\n", -error_state);
		return error_state;
	}
	SD_CARD_LOGI("sdio host send CMD7(SELECT_DESELECT_CARD) ok\r\n");

	return error_state;
}

/* CMD16 */
static bk_err_t sd_card_cmd_set_block_len(uint32_t block_size)
{
	bk_err_t error_state = BK_OK;
	sdio_host_cmd_cfg_t cmd_cfg = {0};

	cmd_cfg.cmd_index = SD_CMD_SET_BLOCKLEN;
	cmd_cfg.argument = block_size;
	cmd_cfg.response = SDIO_HOST_CMD_RSP_SHORT;
	cmd_cfg.wait_rsp_timeout = sd_card_get_cmd_timeout_param();

	bk_sdio_host_send_command(&cmd_cfg);
	error_state = bk_sdio_host_wait_cmd_response(cmd_cfg.cmd_index);
	if (error_state != BK_OK) {
		SD_CARD_LOGW("CMD16 SET_BLOCK_LEN err:-%x\r\n", -error_state);
		return error_state;
	}
	SD_CARD_LOGI("sdio host send CMD16(SET_BLOCK_LEN) ok, set_block_len:%d\r\n", block_size);

	return error_state;
}

/* CMD25 */
static bk_err_t sd_card_cmd_write_multiple_block(uint32_t write_addr)
{
	bk_err_t error_state = BK_OK;
	sdio_host_cmd_cfg_t cmd_cfg = {0};

	cmd_cfg.cmd_index = SD_CMD_WRITE_MULTIPLE_BLOCK;
	cmd_cfg.argument = write_addr;
	cmd_cfg.response = SDIO_HOST_CMD_RSP_SHORT;
	cmd_cfg.wait_rsp_timeout = sd_card_get_cmd_timeout_param();

	bk_sdio_host_send_command(&cmd_cfg);
	error_state = bk_sdio_host_wait_cmd_response(cmd_cfg.cmd_index);
	if (error_state != BK_OK) {
		SD_CARD_LOGW("CMD25 WRITE_MULTIPLE_BLOCK err:-%x\r\n", -error_state);
		return error_state;
	}
	SD_CARD_LOGI("sdio host send CMD25(WRITE_MULTIPLE_BLOCK) ok\r\n");

	return error_state;
}

/* CMD24 */
static bk_err_t sd_card_cmd_write_single_block(uint32_t write_addr)
{
	bk_err_t error_state = BK_OK;
	sdio_host_cmd_cfg_t cmd_cfg = {0};

	cmd_cfg.cmd_index = SD_CMD_WRITE_BLOCK;
	cmd_cfg.argument = write_addr;
	cmd_cfg.response = SDIO_HOST_CMD_RSP_SHORT;
	cmd_cfg.wait_rsp_timeout = sd_card_get_cmd_timeout_param();

	bk_sdio_host_send_command(&cmd_cfg);
	error_state = bk_sdio_host_wait_cmd_response(cmd_cfg.cmd_index);
	if (error_state != BK_OK) {
		SD_CARD_LOGW("CMD24 WRITE_BLOCK err:-%x\r\n", -error_state);
		return error_state;
	}
	SD_CARD_LOGI("sdio host send CMD24(WRITE_BLOCK) ok\r\n");

	return error_state;
}

/* CMD18 */
static bk_err_t sd_card_cmd_read_multiple_block(uint32 addr)
{
	bk_err_t error_state = BK_OK;
	sdio_host_cmd_cfg_t cmd_cfg = {0};

	cmd_cfg.cmd_index = SD_CMD_READ_MULTIPLE_BLOCK;
	cmd_cfg.argument = addr;
	cmd_cfg.response = SDIO_HOST_CMD_RSP_SHORT;
	cmd_cfg.wait_rsp_timeout = sd_card_get_cmd_timeout_param();

	bk_sdio_host_send_command(&cmd_cfg);
	error_state = bk_sdio_host_wait_cmd_response(cmd_cfg.cmd_index);
	if (error_state != BK_OK) {
		SD_CARD_LOGW("CMD18 READ_MULTIPLE_BLOCK err:-%x\r\n", -error_state);
		return error_state;
	}
	SD_CARD_LOGI("sdio host send CMD18(READ_MULTIPLE_BLOCK) ok\r\n");

	return error_state;
}

/* CMD17 */
static bk_err_t sd_card_cmd_read_single_block(uint32 addr)
{
	bk_err_t error_state = BK_OK;
	sdio_host_cmd_cfg_t cmd_cfg = {0};

	cmd_cfg.cmd_index = SD_CMD_READ_SINGLE_BLOCK;
	cmd_cfg.argument = addr;
	cmd_cfg.response = SDIO_HOST_CMD_RSP_SHORT;
	cmd_cfg.wait_rsp_timeout = sd_card_get_cmd_timeout_param();

	bk_sdio_host_send_command(&cmd_cfg);
	error_state = bk_sdio_host_wait_cmd_response(cmd_cfg.cmd_index);
	if (error_state != BK_OK) {
		SD_CARD_LOGW("CMD17 READ_SINGLE_BLOCK err:-%x\r\n", -error_state);
		return error_state;
	}
	SD_CARD_LOGI("sdio host send CMD17(READ_SINGLE_BLOCK) ok\r\n");

	return error_state;
}

/* CMD12 */
static bk_err_t sd_card_cmd_stop_transmission(void)
{
	bk_err_t error_state = BK_OK;
	sdio_host_cmd_cfg_t cmd_cfg = {0};

	cmd_cfg.cmd_index = SD_CMD_STOP_TRANSMISSION;
	cmd_cfg.argument = 0;
	cmd_cfg.response = SDIO_HOST_CMD_RSP_SHORT;
	cmd_cfg.wait_rsp_timeout = sd_card_get_cmd_timeout_param();

	bk_sdio_host_send_command(&cmd_cfg);
	error_state = bk_sdio_host_wait_cmd_response(cmd_cfg.cmd_index);
	if (error_state != BK_OK) {
		SD_CARD_LOGW("CMD12 STOP_TRANSMISSION err:-%x\r\n", -error_state);
		return error_state;
	}
	SD_CARD_LOGI("sdio host send CMD12(STOP_TRANSMISSION) ok\r\n");

	return error_state;
}

/* CMD32 */
static bk_err_t sd_card_cmd_set_erase_start_addr(uint32_t start_addr)
{
	bk_err_t error_state = BK_OK;
	sdio_host_cmd_cfg_t cmd_cfg = {0};

	cmd_cfg.cmd_index = SD_CMD_ERASE_WR_BLK_START;
	cmd_cfg.argument = start_addr;
	cmd_cfg.response = SDIO_HOST_CMD_RSP_SHORT;
	cmd_cfg.wait_rsp_timeout = sd_card_get_cmd_timeout_param();

	bk_sdio_host_send_command(&cmd_cfg);
	error_state = bk_sdio_host_wait_cmd_response(cmd_cfg.cmd_index);
	if (error_state != BK_OK) {
		SD_CARD_LOGW("CMD32 ERASE_WR_BLK_START err:-%x\r\n", -error_state);
		return error_state;
	}
	SD_CARD_LOGI("sdio host send CMD32(ERASE_WR_BLK_START) ok\r\n");

	return error_state;
}

/* CMD33 */
static bk_err_t sd_card_cmd_set_erase_end_addr(uint32_t end_addr)
{
	bk_err_t error_state = BK_OK;
	sdio_host_cmd_cfg_t cmd_cfg = {0};

	cmd_cfg.cmd_index = SD_CMD_ERASE_WR_BLK_END;
	cmd_cfg.argument = end_addr;
	cmd_cfg.response = SDIO_HOST_CMD_RSP_SHORT;
	cmd_cfg.wait_rsp_timeout = sd_card_get_cmd_timeout_param();

	bk_sdio_host_send_command(&cmd_cfg);
	error_state = bk_sdio_host_wait_cmd_response(cmd_cfg.cmd_index);
	if (error_state != BK_OK) {
		SD_CARD_LOGW("CMD33 ERASE_WR_BLK_END err:-%x\r\n", -error_state);
		return error_state;
	}
	SD_CARD_LOGI("sdio host send CMD33(ERASE_WR_BLK_END) ok\r\n");

	return error_state;
}

/* CMD38 */
static bk_err_t sd_card_cmd_erase(void)
{
	bk_err_t error_state = BK_OK;
	sdio_host_cmd_cfg_t cmd_cfg = {0};

	cmd_cfg.cmd_index = SD_CMD_ERASE;
	cmd_cfg.argument = 0;
	cmd_cfg.response = SDIO_HOST_CMD_RSP_SHORT;
	cmd_cfg.wait_rsp_timeout = sd_card_get_cmd_timeout_param();

	bk_sdio_host_send_command(&cmd_cfg);
	error_state = bk_sdio_host_wait_cmd_response(cmd_cfg.cmd_index);
	if (error_state != BK_OK) {
		SD_CARD_LOGW("CMD38 ERASE err:-%x\r\n", -error_state);
		return error_state;
	}
	SD_CARD_LOGI("sdio host send CMD38(ERASE) ok\r\n");

	return error_state;
}

/* CMD13 */
static bk_err_t sd_card_cmd_send_status(uint32_t argument, uint32_t *card_status)
{
	bk_err_t error_state = BK_OK;
	sdio_host_cmd_cfg_t cmd_cfg = {0};

	cmd_cfg.cmd_index = SD_CMD_SEND_STATUS;
	cmd_cfg.argument = argument;
	cmd_cfg.response = SDIO_HOST_CMD_RSP_SHORT;
	cmd_cfg.wait_rsp_timeout = sd_card_get_cmd_timeout_param();

	bk_sdio_host_send_command(&cmd_cfg);
	error_state = bk_sdio_host_wait_cmd_response(cmd_cfg.cmd_index);
	if (error_state != BK_OK) {
		SD_CARD_LOGW("CMD13 SEND_STATUS err:-%x\r\n", -error_state);
		return error_state;
	}
	SD_CARD_LOGI("sdio host send CMD13(SEND_STATUS) ok\r\n");

	*card_status = bk_sdio_host_get_cmd_rsp_argument(SDIO_HOST_RSP0);

	return error_state;
}

static bk_err_t sd_card_power_on(void)
{
	bk_err_t error_state;
	uint32_t count = 0, valid_volt = 0;
	uint32_t response = 0;

	/* CMD0: GO_IDLE_STATE */
	error_state = sd_card_cmd_go_idle_state();
	if (error_state != BK_OK) {
		return error_state;
	}
    rtos_delay_milliseconds(50);

	/* CMD8: SEND_IF_COND: Command available on V2.0 cards */
	error_state = sd_card_cmd_send_if_cond();
	if (error_state != BK_OK) {
		s_sd_card_obj.sd_card.card_version = SD_CARD_V1_X;
	} else {
		s_sd_card_obj.sd_card.card_version = SD_CARD_V2_X;
	}

	if (s_sd_card_obj.sd_card.card_version == SD_CARD_V2_X) {
		/* SEND CMD55 APP_CMD with RCA as 0 */
		error_state = sd_card_cmd_app_cmd(0);
		if (error_state != BK_OK) {
			SD_CARD_LOGW("unsupported feature\r\n");
			return BK_ERR_SDIO_HOST_UNSUPPORTED_FEATURE;
		}
	}

	while ((count < SD_CARD_MAX_VOLT_TRIAL_COUNT) && (valid_volt == 0)) {
		/* Send CMD55 APP_CMD with RCA as 0 */
		error_state = sd_card_cmd_app_cmd(0);
		if (error_state != BK_OK) {
			return error_state;
		}

		/* Send ACMD41 */
		error_state = sd_card_cmd_sd_send_op_cond(SD_DEFAULT_OCR);
		if (error_state != BK_OK) {
			SD_CARD_LOGW("unsupported feature\r\n");
			return BK_ERR_SDIO_HOST_UNSUPPORTED_FEATURE;
		}
		/* Get command response */
		response = bk_sdio_host_get_cmd_rsp_argument(SDIO_HOST_RSP0);

		/* Get operating voltage
		 * OCR register:
		 * BIT[31]: card power up status bit
		 */
		valid_volt = (((response >> 31) == 1) ? 1 : 0);

		count++;
		rtos_delay_milliseconds(2);
	}

	if (count >= SD_CARD_MAX_VOLT_TRIAL_COUNT) {
		SD_CARD_LOGW("send cmd55 and acmd41 retry time out\r\n");
		return BK_ERR_SDIO_HOST_INVALID_VOLT_RANGE;
	}

	/* OCR register BIT[30] */
	if ((response & SD_OCR_HIGH_CAPACITY) == SD_OCR_HIGH_CAPACITY) {
		SD_CARD_LOGI("card capacity SDHC_SDXC\r\n");
		s_sd_card_obj.sd_card.card_type = SD_CARD_TYPE_SDHC_SDXC;
	} else {
		s_sd_card_obj.sd_card.card_type = SD_CARD_TYPE_SDSC;
		SD_CARD_LOGI("card capacity SDSC\r\n");
	}

	return BK_OK;
}

static bk_err_t sd_card_init_card(void)
{
	bk_err_t error_state = BK_OK;
	uint16_t sd_rca = 1;

	/* Send CMD2 ALL_SEND_CID */
	error_state = sd_card_cmd_all_send_cid();
	if (error_state != BK_OK) {
		return error_state;
	} else {
		/* Get card identification number data */
		s_sd_card_obj.cid[0] = bk_sdio_host_get_cmd_rsp_argument(SDIO_HOST_RSP0);
		s_sd_card_obj.cid[1] = bk_sdio_host_get_cmd_rsp_argument(SDIO_HOST_RSP1);
		s_sd_card_obj.cid[2] = bk_sdio_host_get_cmd_rsp_argument(SDIO_HOST_RSP2);
		s_sd_card_obj.cid[3] = bk_sdio_host_get_cmd_rsp_argument(SDIO_HOST_RSP3);
	}

	rtos_delay_milliseconds(2);

	/* Send CMD3 SET_RELATIVE_ADDR with argument 0 */
	/* SD Card publishes its RCA */
	error_state = sd_card_cmd_send_relative_addr(&sd_rca);
	if (error_state != BK_OK) {
		return error_state;
	}
	s_sd_card_obj.sd_card.relative_card_addr = sd_rca;

	rtos_delay_milliseconds(2);

	/* Send CMD9 SEDN_CSD with argument as card's RCA */
	error_state = sd_card_cmd_send_csd((uint32_t)(s_sd_card_obj.sd_card.relative_card_addr << 16));
	if (error_state != BK_OK) {
		return error_state;
	} else {
		/* Get card specific data */
		s_sd_card_obj.csd[0] = bk_sdio_host_get_cmd_rsp_argument(SDIO_HOST_RSP0);
		s_sd_card_obj.csd[1] = bk_sdio_host_get_cmd_rsp_argument(SDIO_HOST_RSP1);
		s_sd_card_obj.csd[2] = bk_sdio_host_get_cmd_rsp_argument(SDIO_HOST_RSP2);
		s_sd_card_obj.csd[3] = bk_sdio_host_get_cmd_rsp_argument(SDIO_HOST_RSP3);
	}

	/* Get the card class */
	s_sd_card_obj.sd_card.class = ((bk_sdio_host_get_cmd_rsp_argument(SDIO_HOST_RSP1)) >> 20);
	SD_CARD_LOGI("sd card class:%d\r\n", s_sd_card_obj.sd_card.class);

	/* TODO: Get CSD parameters */


	/* Send CMD7, Select the card */
	error_state = sd_card_cmd_select_deselect_card((uint32_t)(s_sd_card_obj.sd_card.relative_card_addr << 16));
	if (error_state != BK_OK) {
		return error_state;
	}
	rtos_delay_milliseconds(2);

	/* improve sdio clock freq for sd card data transfer mode */
#if CONFIG_SOC_BK7256XX
	s_sd_card_obj.clock_freq = SDIO_HOST_CLK_26M;
	bk_sdio_host_set_clock_freq(SDIO_HOST_CLK_26M);
	SD_CARD_LOGI("sdio clock freq:%d->%d\r\n", CONFIG_SDIO_HOST_DEFAULT_CLOCK_FREQ, SDIO_HOST_CLK_26M);
#else
	s_sd_card_obj.clock_freq = SDIO_HOST_CLK_13M;
	bk_sdio_host_set_clock_freq(SDIO_HOST_CLK_13M);
	SD_CARD_LOGI("sdio clock freq:%d->%d\r\n", CONFIG_SDIO_HOST_DEFAULT_CLOCK_FREQ, SDIO_HOST_CLK_13M);
#endif
	rtos_delay_milliseconds(2);

	/* All cards are initialized */
	return error_state;
}

bk_err_t bk_sd_card_init(void)
{
	bk_err_t error_state = BK_OK;
	sdio_host_config_t sdio_cfg = {0};

	sdio_cfg.clock_freq = CONFIG_SDIO_HOST_DEFAULT_CLOCK_FREQ;
	sdio_cfg.bus_width = SDIO_HOST_BUS_WIDTH_1LINE;
	s_sd_card_obj.clock_freq = sdio_cfg.clock_freq;

	/* Initialize SDIO peripheral interface with default configuration */
	BK_RETURN_ON_ERR(bk_sdio_host_init(&sdio_cfg));
	SD_CARD_LOGI("sdio host init ok, clock_freq:%d\r\n", sdio_cfg.clock_freq);
	rtos_delay_milliseconds(30);

	/* Identify card operating voltage */
	error_state = sd_card_power_on();
	if (error_state != BK_OK) {
		return error_state;
	}

	rtos_delay_milliseconds(2);

	/* Card initialization */
	error_state = sd_card_init_card();
	if (error_state != BK_OK) {
		return error_state;
	}

	rtos_delay_milliseconds(2);

	/* Set block size for card */
	error_state = sd_card_cmd_set_block_len(SD_BLOCK_SIZE);
	if (error_state != BK_OK) {
		return error_state;
	}

	return error_state;
}

bk_err_t bk_sd_card_deinit(void)
{
	BK_RETURN_ON_ERR(bk_sdio_host_deinit());
	os_memset(&s_sd_card_obj, 0, sizeof(s_sd_card_obj));
	return BK_OK;
}

bk_err_t bk_sd_card_erase(uint32_t start_block_addr, uint32_t end_block_addr)
{
	bk_err_t error_state = BK_OK;
	uint32_t start_addr = start_block_addr;
	uint32_t end_addr = end_block_addr;

	/* Check if the card command class supports erase command */
	if ((s_sd_card_obj.sd_card.class & SD_SUPPORT_ERASE_CMD) == 0) {
		SD_CARD_LOGI("the card command class not support erase command\r\n");
		//return BK_FAIL;
	}
	if (s_sd_card_obj.sd_card.card_type != SD_CARD_TYPE_SDHC_SDXC) {
		start_addr *= 512;
		end_addr *= 512;
	}

	/* Send CMD32 to set erase start addr */
	error_state = sd_card_cmd_set_erase_start_addr(start_addr);
	if (error_state != BK_OK) {
		return error_state;
	}
	rtos_delay_milliseconds(2);

	/* Send CMD33 to set erase end addr */
	error_state = sd_card_cmd_set_erase_end_addr(end_addr);
	if (error_state != BK_OK) {
		return error_state;
	}
	rtos_delay_milliseconds(2);

	/* Send CMD38 ERASE */
	error_state = sd_card_cmd_erase();
	if (error_state != BK_OK) {
		return error_state;
	}

	return BK_OK;
}

bk_err_t bk_sd_card_write_blocks(const uint8_t *data, uint32_t block_addr, uint32_t block_num)
{
	BK_RETURN_ON_NULL(data);

	bk_err_t error_state = BK_OK;
	uint32_t addr = block_addr;
	sdio_host_data_config_t data_config = {0};

	if (s_sd_card_obj.sd_card.card_type != SD_CARD_TYPE_SDHC_SDXC) {
		addr *= 512;
	}

	if (block_num > 1) {
		/* Send write multiple block command */
		error_state = sd_card_cmd_write_multiple_block(addr);
	} else {
		/* Send write single block command */
		error_state = sd_card_cmd_write_single_block(addr);
	}
	if (error_state != BK_OK) {
		return error_state;
	}

	/* config sdio data */
	data_config.data_timeout = sd_card_get_data_timeout_param();
	data_config.data_len = SD_BLOCK_SIZE * block_num;
	data_config.data_block_size = SD_BLOCK_SIZE;
	data_config.data_dir = SDIO_HOST_DATA_DIR_WR;
	bk_sdio_host_config_data(&data_config);

	bk_sdio_host_write_fifo(data, data_config.data_len);

	/* Send stop transmission command in case of mutiple block write */
	if (block_num > 1) {
		error_state = sd_card_cmd_stop_transmission();
	}

	return error_state;
}

bk_err_t bk_sd_card_read_blocks(uint8_t *data, uint32_t block_addr, uint32_t block_num)
{
	bk_err_t error_state = BK_OK;
	uint32_t addr = block_addr;
	uint32_t read_data = 0, index = 0;
	sdio_host_data_config_t data_config = {0};

	data_config.data_timeout = sd_card_get_data_timeout_param();
	data_config.data_len = SD_BLOCK_SIZE * block_num;
	data_config.data_block_size = SD_BLOCK_SIZE;
	data_config.data_dir = SDIO_HOST_DATA_DIR_RD;

	bk_sdio_host_config_data(&data_config);
	SD_CARD_LOGI("sdio host config data ok, data_len:%d\r\n", data_config.data_len);
	/* such delay is important, otherwise sdio recv_end_int will not triggered */
	rtos_delay_milliseconds(2);

	if (s_sd_card_obj.sd_card.card_type != SD_CARD_TYPE_SDHC_SDXC) {
		addr *= 512;
	}

	if (block_num > 1) {
		/* Send read multiple block command */
		error_state = sd_card_cmd_read_multiple_block(addr);
	} else {
		/* Send read single block command */
		error_state = sd_card_cmd_read_single_block(addr);
	}
	if (error_state != BK_OK) {
		return error_state;
	}
	rtos_delay_milliseconds(2);

	while (bk_sdio_host_wait_receive_data() == BK_OK) {
		do {
			/* Read data from SDIO Rx fifo */
			read_data = bk_sdio_host_read_fifo();
			data[index++] = read_data & 0xff;
			data[index++] = (read_data >> 8) & 0xff;
			data[index++] = (read_data >> 16) & 0xff;
			data[index++] = (read_data >> 24) & 0xff;
			SD_CARD_LOGD("read_data:%x, index:%d\r\n", read_data, index);
		} while ((index % SD_BLOCK_SIZE) != 0);
		if (index >= data_config.data_len) {
			break;
		}
	}

	/* Send stop transmission command
	 * notice: need reset command_and_data state after recv data(REG0x9 BIT[20] for bk7271)
	 * otherwise wait_for_cmd_rsp failed
	 */
	if (block_num > 1) {
		error_state = sd_card_cmd_stop_transmission();
	}

	return error_state;
}

bk_err_t bk_sd_card_get_card_info(sd_card_info_t *card_info)
{
	*card_info = s_sd_card_obj.sd_card;
	return BK_OK;
}

sd_card_state_t bk_sd_card_get_card_state(void)
{
	sd_card_state_t card_state = SD_CARD_TRANSFER;
	bk_err_t error_state = BK_OK;
	uint32_t resp1 = 0;

	/* Send status command(CMD13) */
	error_state = sd_card_cmd_send_status((uint32_t)(s_sd_card_obj.sd_card.relative_card_addr << 16), &resp1);
	if (error_state != BK_OK) {

	}
	/* The response format R1 contains a 32-bit field named card status.
	 * BIT[12:9] current_state
	 */
	card_state = (sd_card_state_t)((resp1 >> 0x9) & 0x0f);

	return card_state;
}

