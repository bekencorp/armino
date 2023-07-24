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

#include <common/bk_include.h>
#include <components/ate.h>
#include <os/mem.h>
#include <driver/flash.h>
#include <os/os.h>
#include "bk_pm_model.h"
#include "flash_driver.h"
#include "flash_hal.h"
#include "sys_driver.h"
#include "driver/flash_partition.h"
#include <modules/chip_support.h>


#if CONFIG_FLASH_QUAD_ENABLE
#include "flash_bypass.h"
extern UINT8 flash_get_line_mode(void);
extern void flash_set_line_mode(UINT8 mode);
#endif

typedef struct {
	flash_hal_t hal;
	uint32_t flash_id;
	const flash_config_t *flash_cfg;
} flash_driver_t;

#define FLASH_GET_PROTECT_CFG(cfg) ((cfg) & FLASH_STATUS_REG_PROTECT_MASK)
#define FLASH_GET_CMP_CFG(cfg)     (((cfg) >> FLASH_STATUS_REG_PROTECT_OFFSET) & FLASH_STATUS_REG_PROTECT_MASK)

#define FLASH_RETURN_ON_DRIVER_NOT_INIT() do {\
	if (!s_flash_is_init) {\
		return BK_ERR_FLASH_NOT_INIT;\
	}\
} while(0)

#define FLASH_RETURN_ON_WRITE_ADDR_OUT_OF_RANGE(addr, len) do {\
	if ((addr >= s_flash.flash_cfg->flash_size) ||\
		(len > s_flash.flash_cfg->flash_size) ||\
		((addr + len) > s_flash.flash_cfg->flash_size)) {\
		FLASH_LOGW("write error[addr:0x%x len:0x%x]\r\n", addr, len);\
		return BK_ERR_FLASH_ADDR_OUT_OF_RANGE;\
	}\
} while(0)

static const flash_config_t flash_config[] = {
	/* flash_id, status_reg_size, flash_size,    line_mode,           cmp_post, protect_post, protect_mask, protect_all, protect_none, protect_half, unprotect_last_block. quad_en_post, quad_en_val, coutinuous_read_mode_bits_val, mode_sel*/
	{0x1C7016,   1,               FLASH_SIZE_4M, FLASH_LINE_MODE_TWO, 0,        2,            0x1F,         0x1F,        0x00,         0x16,         0x01B,                0,            0,           0xA5,                          0x01}, //en_25qh32b
	{0x1C7015,   1,               FLASH_SIZE_2M, FLASH_LINE_MODE_TWO, 0,        2,            0x1F,         0x1F,        0x00,         0x0d,         0x0d,                 0,            0,           0xA5,                          0x01}, //en_25qh16b
	{0x0B4014,   2,               FLASH_SIZE_1M, FLASH_LINE_MODE_TWO, 14,       2,            0x1F,         0x1F,        0x00,         0x0C,         0x101,                9,            1,           0xA0,                          0x01}, //xtx_25f08b
	{0x0B4015,   2,               FLASH_SIZE_2M, FLASH_LINE_MODE_TWO, 14,       2,            0x1F,         0x1F,        0x00,         0x0D,         0x101,                9,            1,           0xA0,                          0x01}, //xtx_25f16b
#if CONFIG_FLASH_QUAD_ENABLE
	{0x0B4016,   2,               FLASH_SIZE_4M, FLASH_LINE_MODE_FOUR, 14,      2,            0x1F,         0x1F,        0x00,         0x0E,         0x101,                9,           1,            0xA0,                          0x02}, //xtx_25f32b
#else
	{0x0B4016,   2,               FLASH_SIZE_4M, FLASH_LINE_MODE_TWO, 14,       2,            0x1F,         0x1F,        0x00,         0x0E,         0x101,                9,            1,           0xA0,                          0x01}, //xtx_25f32b
#endif
	{0x0B4017,   2,               FLASH_SIZE_8M, FLASH_LINE_MODE_TWO, 14,       2,            0x1F,         0x05,        0x00,         0x0E,         0x109,                9,            1,           0xA0,                          0x01}, //xtx_25f64b
#if CONFIG_FLASH_QUAD_ENABLE
	{0x0B6018,   2,               FLASH_SIZE_16M, FLASH_LINE_MODE_FOUR,  0,	    2,            0x0F,         0x0F,        0x00,         0x0A,         0x00E,                9,            1,           0xA0,                          0x02}, //xt_25q128d
#else
	{0x0B6018,   1,               FLASH_SIZE_16M, FLASH_LINE_MODE_TWO,   0,     2,            0x0F,         0x0F,        0x00,         0x0A,         0x00E,                0,            0,           0xA0,                          0x01}, //xt_25q128d
#endif
	{0x0E4016,   2,               FLASH_SIZE_4M, FLASH_LINE_MODE_TWO, 14,       2,            0x1F,         0x1F,        0x00,         0x0E,         0x101,                9,            1,           0xA0,                          0x01}, //xtx_FT25H32
	{0x1C4116,   1,               FLASH_SIZE_4M, FLASH_LINE_MODE_TWO, 0,        2,            0x1F,         0x1F,        0x00,         0x0E,         0x00E,                0,            0,           0xA0,                          0x01}, //en_25qe32a (not support 4 line)
	{0xC84015,   2,               FLASH_SIZE_2M, FLASH_LINE_MODE_TWO, 14,       2,            0x1F,         0x1F,        0x00,         0x0D,         0x101,                9,            1,           0xA0,                          0x01}, //gd_25q16c
#if CONFIG_FLASH_QUAD_ENABLE
	{0xC84016,   2,               FLASH_SIZE_4M, FLASH_LINE_MODE_FOUR, 14,      2,            0x1F,         0x1F,        0x00,         0x0E,         0x00E,                9,            1,           0xA0,                          0x02}, //gd_25q32c
#else
	{0xC84016,   1,               FLASH_SIZE_4M, FLASH_LINE_MODE_TWO, 0,        2,            0x1F,         0x1F,        0x00,         0x0E,         0x00E,                0,            0,           0xA0,                          0x01}, //gd_25q32c
#endif
#if CONFIG_FLASH_QUAD_ENABLE
	{0xC86018,   2,               FLASH_SIZE_16M,FLASH_LINE_MODE_FOUR, 0,       2,            0x0F,         0x0F,        0x00,         0x0A,         0x00E,                9,            1,           0xA0,                          0x02}, //gd_25lq128e
#else
	{0xC86018,   1,               FLASH_SIZE_16M,FLASH_LINE_MODE_TWO,  0,       2,            0x0F,         0x0F,        0x00,         0x0A,         0x00E,                0,            0,           0xA0,                          0x01}, //gd_25lq128e
#endif
	{0xC86515,   2,               FLASH_SIZE_2M, FLASH_LINE_MODE_TWO, 14,       2,            0x1F,         0x1F,        0x00,         0x0D,         0x101,                9,            1,           0xA0,                          0x01}, //gd_25w16e
#if CONFIG_FLASH_QUAD_ENABLE
	{0xC86516,   2,               FLASH_SIZE_4M, FLASH_LINE_MODE_FOUR, 14,      2,            0x1F,         0x1F,        0x00,         0x0E,         0x00E,                9,            1,           0xA0,                          0x02}, //gd_25wq32e
#else
	{0xC86516,   1,               FLASH_SIZE_4M, FLASH_LINE_MODE_TWO, 0,        2,            0x1F,         0x1F,        0x00,         0x0E,         0x00E,                0,            0,           0xA0,                          0x01}, //gd_25wq32e
#endif
	{0xEF4016,   2,               FLASH_SIZE_4M, FLASH_LINE_MODE_TWO, 14,       2,            0x1F,         0x1F,        0x00,         0x00,         0x101,                9,            1,           0xA0,                          0x01}, //w_25q32(bfj)
	{0x204016,   2,               FLASH_SIZE_4M, FLASH_LINE_MODE_TWO, 14,       2,            0x1F,         0x1F,        0x00,         0x0E,         0x101,                9,            1,           0xA0,                          0x01}, //xmc_25qh32b
	{0xC22315,   1,               FLASH_SIZE_2M, FLASH_LINE_MODE_TWO, 0,        2,            0x0F,         0x0F,        0x00,         0x0A,         0x00E,                6,            1,           0xA5,                          0x01}, //mx_25v16b
	{0xEB6015,   2,               FLASH_SIZE_2M, FLASH_LINE_MODE_TWO, 14,       2,            0x1F,         0x1F,        0x00,         0x0D,         0x101,                9,            1,           0xA0,                          0x01}, //zg_th25q16b
	{0x000000,   2,               FLASH_SIZE_4M, FLASH_LINE_MODE_TWO, 0,        2,            0x1F,         0x00,        0x00,         0x00,         0x000,                0,            0,           0x00,                          0x01}, //default
};

static flash_driver_t s_flash = {0};
static bool s_flash_is_init = false;
static beken_mutex_t s_flash_mutex = NULL;
static PM_STATUS flash_ps_status;
static flash_ps_callback_t s_flash_ps_suspend_cb = NULL;
static flash_ps_callback_t s_flash_ps_resume_cb = NULL;
#if (CONFIG_SOC_BK7256XX)
static uint32_t s_hold_low_speed_status = 0;
#endif
#define FLASH_MAX_WAIT_CB_CNT (4)
static flash_wait_callback_t s_flash_wait_cb[FLASH_MAX_WAIT_CB_CNT] = {NULL};

bk_err_t bk_flash_register_wait_cb(flash_wait_callback_t wait_cb)
{
	uint32_t i = 0;

	for(i = 0; i < FLASH_MAX_WAIT_CB_CNT; i++)
	{
		if(s_flash_wait_cb[i] == NULL)
		{
			s_flash_wait_cb[i] = wait_cb;
			break;
		}
	}

	if(i == FLASH_MAX_WAIT_CB_CNT)
	{
		FLASH_LOGE("cb is full\r\n");
		return BK_ERR_FLASH_WAIT_CB_FULL;
	}

	return BK_OK;
}

bk_err_t bk_flash_unregister_wait_cb(flash_wait_callback_t wait_cb)
{
	uint32_t i = 0;

	for(i = 0; i < FLASH_MAX_WAIT_CB_CNT; i++)
	{
		if(s_flash_wait_cb[i] == wait_cb)
		{
			s_flash_wait_cb[i] = NULL;
			break;
		}
	}

	if(i == FLASH_MAX_WAIT_CB_CNT)
	{
		FLASH_LOGE("cb isn't registered\r\n");
		return BK_ERR_FLASH_WAIT_CB_NOT_REGISTER;
	}

	return BK_OK;
}

__attribute__((section(".itcm_sec_code"))) void flash_waiting_cb(void)
{
	uint32_t i = 0;

	for(i = 0; i < FLASH_MAX_WAIT_CB_CNT; i++)
	{
		if(s_flash_wait_cb[i])
		{
			s_flash_wait_cb[i]();
		}
	}
}

static UINT32 flash_ps_suspend(UINT32 ps_level)
{
	PM_STATUS *flash_ps_status_ptr = &flash_ps_status;

	switch (ps_level) {
	case NORMAL_PS:
	case LOWVOL_PS:
	case DEEP_PS:
	case IDLE_PS:
		if (s_flash_ps_suspend_cb) {
			s_flash_ps_suspend_cb();
		}
		if (FLASH_LINE_MODE_FOUR == bk_flash_get_line_mode())
			bk_flash_set_line_mode(FLASH_LINE_MODE_TWO);
		flash_ps_status_ptr->bits.unconditional_ps_sleeped = 1;
		flash_ps_status_ptr->bits.normal_ps_sleeped = 1;
		flash_ps_status_ptr->bits.lowvol_ps_sleeped = 1;
		flash_ps_status_ptr->bits.deep_ps_sleeped = 1;
		break;
	default:
		break;
	}
	return 0;
}

static UINT32 flash_ps_resume(UINT32 ps_level)
{
	PM_STATUS *flash_ps_status_ptr = &flash_ps_status;

	switch (ps_level) {
	case NORMAL_PS:
	case LOWVOL_PS:
	case DEEP_PS:
	case IDLE_PS:
		if (FLASH_LINE_MODE_FOUR == bk_flash_get_line_mode())
			bk_flash_set_line_mode(FLASH_LINE_MODE_FOUR);
		if (s_flash_ps_resume_cb) {
			s_flash_ps_resume_cb();
		}
		flash_ps_status_ptr->bits.unconditional_ps_sleeped = 0;
		flash_ps_status_ptr->bits.normal_ps_sleeped = 0;
		flash_ps_status_ptr->bits.lowvol_ps_sleeped = 0;
		flash_ps_status_ptr->bits.deep_ps_sleeped = 0;
		break;
	default:
		break;
	}
	return 0;
}

static PM_STATUS flash_ps_get_status(UINT32 flag)
{
	return flash_ps_status;
}

static DEV_PM_OPS_S flash_ps_ops = {
	.pm_init = NULL,
	.pm_deinit = NULL,
	.suspend = flash_ps_suspend,
	.resume = flash_ps_resume,
	.status = flash_ps_get_status,
	.get_sleep_time = NULL,
};

static void flash_init_common(void)
{
	int ret = rtos_init_mutex(&s_flash_mutex);
	BK_ASSERT(kNoErr == ret); /* ASSERT VERIFIED */
}

static void flash_deinit_common(void)
{
	int ret = rtos_deinit_mutex(&s_flash_mutex);
	BK_ASSERT(kNoErr == ret); /* ASSERT VERIFIED */
}

static void flash_get_current_config(void)
{
	bool cfg_success = false;

	for (uint32_t i = 0; i < (ARRAY_SIZE(flash_config) - 1); i++) {
		if (s_flash.flash_id == flash_config[i].flash_id) {
			s_flash.flash_cfg = &flash_config[i];
			cfg_success = true;
			break;
		}
	}

	if (!cfg_success) {
		s_flash.flash_cfg = &flash_config[ARRAY_SIZE(flash_config) - 1];
		for(int i = 0; i < 10; i++) {
			FLASH_LOGE("This flash is not identified, choose default config\r\n");
		}
	}
}

static uint32_t flash_get_protect_cfg(flash_protect_type_t type)
{
	switch (type) {
	case FLASH_PROTECT_NONE:
		return FLASH_GET_PROTECT_CFG(s_flash.flash_cfg->protect_none);
	case FLASH_PROTECT_ALL:
		return FLASH_GET_PROTECT_CFG(s_flash.flash_cfg->protect_all);
	case FLASH_PROTECT_HALF:
		return FLASH_GET_PROTECT_CFG(s_flash.flash_cfg->protect_half);
	case FLASH_UNPROTECT_LAST_BLOCK:
		return FLASH_GET_PROTECT_CFG(s_flash.flash_cfg->unprotect_last_block);
	default:
		return FLASH_GET_PROTECT_CFG(s_flash.flash_cfg->protect_all);
	}
}

static void flash_set_protect_cfg(uint32_t *status_reg_val, uint32_t new_protect_cfg)
{
	*status_reg_val &= ~(s_flash.flash_cfg->protect_mask << s_flash.flash_cfg->protect_post);
	*status_reg_val |= ((new_protect_cfg & s_flash.flash_cfg->protect_mask) << s_flash.flash_cfg->protect_post);
}

static uint32_t flash_get_cmp_cfg(flash_protect_type_t type)
{
	switch (type) {
	case FLASH_PROTECT_NONE:
		return FLASH_GET_CMP_CFG(s_flash.flash_cfg->protect_none);
	case FLASH_PROTECT_ALL:
		return FLASH_GET_CMP_CFG(s_flash.flash_cfg->protect_all);
	case FLASH_PROTECT_HALF:
		return FLASH_GET_CMP_CFG(s_flash.flash_cfg->protect_half);
	case FLASH_UNPROTECT_LAST_BLOCK:
		return FLASH_GET_CMP_CFG(s_flash.flash_cfg->unprotect_last_block);
	default:
		return FLASH_GET_CMP_CFG(s_flash.flash_cfg->protect_all);
	}
}

static void flash_set_cmp_cfg(uint32_t *status_reg_val, uint32_t new_cmp_cfg)
{
	*status_reg_val &= ~(FLASH_CMP_MASK << s_flash.flash_cfg->cmp_post);
	*status_reg_val |= ((new_cmp_cfg & FLASH_CMP_MASK) << s_flash.flash_cfg->cmp_post);
}

static bool flash_is_need_update_status_reg(uint32_t protect_cfg, uint32_t cmp_cfg, uint32_t status_reg_val)
{
	uint32_t cur_protect_val_in_status_reg = (status_reg_val >> s_flash.flash_cfg->protect_post) & s_flash.flash_cfg->protect_mask;
	uint32_t cur_cmp_val_in_status_reg = (status_reg_val >> s_flash.flash_cfg->cmp_post) & FLASH_CMP_MASK;

	if (cur_protect_val_in_status_reg != protect_cfg ||
		cur_cmp_val_in_status_reg != cmp_cfg) {
		return true;
	} else {
		return false;
	}
}

static void flash_set_protect_type(flash_protect_type_t type)
{
	uint32_t protect_cfg = flash_get_protect_cfg(type);
	uint32_t cmp_cfg = flash_get_cmp_cfg(type);
	uint32_t status_reg = flash_hal_read_status_reg(&s_flash.hal, s_flash.flash_cfg->status_reg_size);

	if (flash_is_need_update_status_reg(protect_cfg, cmp_cfg, status_reg)) {
		flash_set_protect_cfg(&status_reg, protect_cfg);
		flash_set_cmp_cfg(&status_reg, cmp_cfg);

		FLASH_LOGI("write status reg:%x, status_reg_size:%d\r\n", status_reg, s_flash.flash_cfg->status_reg_size);
		flash_hal_write_status_reg(&s_flash.hal, s_flash.flash_cfg->status_reg_size, status_reg);
	}
}

static void flash_set_qe(void)
{
	uint32_t status_reg;

	flash_hal_wait_op_done(&s_flash.hal);

	status_reg = flash_hal_read_status_reg(&s_flash.hal, s_flash.flash_cfg->status_reg_size);
	if (status_reg & (s_flash.flash_cfg->quad_en_val << s_flash.flash_cfg->quad_en_post)) {
		return;
	}

	status_reg |= s_flash.flash_cfg->quad_en_val << s_flash.flash_cfg->quad_en_post;
	flash_hal_write_status_reg(&s_flash.hal, s_flash.flash_cfg->status_reg_size, status_reg);
}

static void flash_set_qwfr(void)
{
	flash_hal_set_mode(&s_flash.hal, s_flash.flash_cfg->mode_sel);
}

static void flash_read_common(uint8_t *buffer, uint32_t address, uint32_t len)
{
	uint32_t addr = address & (~FLASH_ADDRESS_MASK);
	uint32_t buf[FLASH_BUFFER_LEN] = {0};
	uint8_t *pb = (uint8_t *)&buf[0];

	if (len == 0) {
		return;
	}

	uint32_t int_level = rtos_disable_int();
	flash_hal_wait_op_done(&s_flash.hal);
	while (len) {
		flash_hal_set_op_cmd_read(&s_flash.hal, addr);
		addr += FLASH_BYTES_CNT;
		for (uint32_t i = 0; i < FLASH_BUFFER_LEN; i++) {
			buf[i] = flash_hal_read_data(&s_flash.hal);
		}

		for (uint32_t i = address % FLASH_BYTES_CNT; i < FLASH_BYTES_CNT; i++) {
			*buffer++ = pb[i];
			address++;
			len--;
			if (len == 0) {
				break;
			}
		}
	}
	rtos_enable_int(int_level);
}

static void flash_read_word_common(uint32_t *buffer, uint32_t address, uint32_t len)
{
	uint32_t addr = address & (~FLASH_ADDRESS_MASK);
	uint32_t buf[FLASH_BUFFER_LEN] = {0};
	//nt8_t *pb = (uint8_t *)&buf[0];
	uint32_t *pb = (uint32_t *)&buf[0];

	if (len == 0) {
		return;
	}

	uint32_t int_level = rtos_disable_int();
	flash_hal_wait_op_done(&s_flash.hal);
	while (len) {
		flash_hal_set_op_cmd_read(&s_flash.hal, addr);
		addr += FLASH_BYTES_CNT;
		for (uint32_t i = 0; i < FLASH_BUFFER_LEN; i++) {
			buf[i] = flash_hal_read_data(&s_flash.hal);
		}

		for (uint32_t i = address % (FLASH_BYTES_CNT/4); i < (FLASH_BYTES_CNT/4); i++) {
			*buffer++ = pb[i];
			address++;
			len--;
			if (len == 0) {
				break;
			}
		}
	}
	rtos_enable_int(int_level);
}

bool flash_is_area_write_disable(uint32_t addr)
{
	uint32_t firmware_area_end_address = 0;
	bk_logic_partition_t * flash_pt = NULL;

#if CONFIG_HTTP_AB_PARTITION
	flash_pt = bk_flash_partition_get_info(BK_PARTITION_APPLICATION);
#else
	flash_pt = bk_flash_partition_get_info(BK_PARTITION_OTA);
#endif
	firmware_area_end_address = flash_pt->partition_start_addr;
	if (addr < firmware_area_end_address) {
		FLASH_LOGE("valid write/erase start address = 0x%x, but current address = 0x%x.\r\n", firmware_area_end_address, addr);
		BK_ASSERT(addr >= firmware_area_end_address);
		return true;
	}
	return false;
}

static bk_err_t flash_write_common(const uint8_t *buffer, uint32_t address, uint32_t len)
{
	uint32_t buf[FLASH_BUFFER_LEN];
	uint8_t *pb = (uint8_t *)&buf[0];
	uint32_t addr = address & (~FLASH_ADDRESS_MASK);

	FLASH_RETURN_ON_WRITE_ADDR_OUT_OF_RANGE(addr, len);

	if (address % FLASH_BYTES_CNT)
		flash_read_common(pb, addr, FLASH_BYTES_CNT);
	else
		os_memset(pb, 0xFF, FLASH_BYTES_CNT);

	flash_hal_wait_op_done(&s_flash.hal);
	while (len) {
		if (len < FLASH_BYTES_CNT) {
			flash_read_common(pb, addr, FLASH_BYTES_CNT);
			flash_hal_wait_op_done(&s_flash.hal);
		}
		for (uint32_t i = address % FLASH_BYTES_CNT; i < FLASH_BYTES_CNT; i++) {
			pb[i] = *buffer++;
			address++;
			len--;
			if (len == 0) {
				break;
			}
		}

		uint32_t int_level = rtos_disable_int();
		for (uint32_t i = 0; i < FLASH_BUFFER_LEN; i++) {
			flash_hal_write_data(&s_flash.hal, buf[i]);
		}
		flash_hal_set_op_cmd_write(&s_flash.hal, addr);
		rtos_enable_int(int_level);
		addr += FLASH_BYTES_CNT;
		os_memset(pb, 0xFF, FLASH_BYTES_CNT);
	}
	return BK_OK;
}

void flash_lock(void)
{
	rtos_lock_mutex(&s_flash_mutex);
}

void flash_unlock(void)
{
	rtos_unlock_mutex(&s_flash_mutex);
}

bk_err_t bk_flash_set_line_mode(flash_line_mode_t line_mode)
{
	flash_hal_clear_qwfr(&s_flash.hal);

	if (FLASH_LINE_MODE_TWO == line_mode) {
		flash_hal_set_dual_mode(&s_flash.hal);
	} else if (FLASH_LINE_MODE_FOUR == line_mode) {
		flash_hal_set_quad_m_value(&s_flash.hal, s_flash.flash_cfg->coutinuous_read_mode_bits_val);
		if (1 == s_flash.flash_cfg->quad_en_val) {
			flash_set_qe();
		}
		flash_set_qwfr();
	}

	return BK_OK;
}

bk_err_t bk_flash_driver_init(void)
{
	if (s_flash_is_init) {
		return BK_OK;
	}
#if CONFIG_FLASH_QUAD_ENABLE
#if CONFIG_FLASH_ORIGIN_API
	if (FLASH_LINE_MODE_FOUR == flash_get_line_mode())
		flash_set_line_mode(FLASH_LINE_MODE_TWO);
#endif
#endif
	os_memset(&s_flash, 0, sizeof(s_flash));
	flash_hal_init(&s_flash.hal);
	bk_flash_set_line_mode(FLASH_LINE_MODE_TWO);
	s_flash.flash_id = flash_hal_get_id(&s_flash.hal);
	FLASH_LOGI("id=0x%x\r\n", s_flash.flash_id);
	flash_get_current_config();
	flash_set_protect_type(FLASH_UNPROTECT_LAST_BLOCK);
#if (0 == CONFIG_JTAG)
	flash_hal_disable_cpu_data_wr(&s_flash.hal);
#endif
	bk_flash_set_line_mode(s_flash.flash_cfg->line_mode);
	flash_hal_set_default_clk(&s_flash.hal);

#if (CONFIG_SOC_BK7256XX)
	#if CONFIG_ATE_TEST
	bk_flash_clk_switch(FLASH_SPEED_LOW, 0);
	#else
	bk_flash_clk_switch(FLASH_SPEED_HIGH, 0);
	#endif
#endif

	flash_init_common();
	s_flash_is_init = true;

	return BK_OK;
}

bk_err_t bk_flash_driver_deinit(void)
{
	if (!s_flash_is_init) {
		return BK_OK;
	}
	flash_deinit_common();
	s_flash_is_init = false;

	return BK_OK;
}

__attribute__((section(".itcm_sec_code"))) bk_err_t bk_flash_erase_sector(uint32_t address)
{
	uint32_t erase_addr = address & FLASH_ERASE_SECTOR_MASK;

	flash_ps_suspend(NORMAL_PS);
	if (erase_addr >= s_flash.flash_cfg->flash_size) {
		FLASH_LOGW("erase error:invalid address 0x%x\r\n", erase_addr);
		return BK_ERR_FLASH_ADDR_OUT_OF_RANGE;
	}

	if (flash_is_area_write_disable(address)) {
		return BK_ERR_FLASH_ADDR_OUT_OF_RANGE;
	}

	uint32_t int_level = rtos_disable_int();
	flash_hal_erase_sector(&s_flash.hal, erase_addr);
	rtos_enable_int(int_level);
	flash_ps_resume(NORMAL_PS);

	return BK_OK;
}

bk_err_t bk_flash_read_bytes(uint32_t address, uint8_t *user_buf, uint32_t size)
{
	if (address >= s_flash.flash_cfg->flash_size) {
		FLASH_LOGW("read error:invalid address 0x%x\r\n", address);
		return BK_ERR_FLASH_ADDR_OUT_OF_RANGE;
	}
	flash_read_common(user_buf, address, size);

	return BK_OK;
}

bk_err_t bk_flash_read_word(uint32_t address, uint32_t *user_buf, uint32_t size)
{
	if (address >= s_flash.flash_cfg->flash_size) {
		FLASH_LOGW("read error:invalid address 0x%x\r\n", address);
		return BK_ERR_FLASH_ADDR_OUT_OF_RANGE;
	}
	flash_read_word_common(user_buf, address, size);

	return BK_OK;
}

bk_err_t bk_flash_write_bytes(uint32_t address, const uint8_t *user_buf, uint32_t size)
{
	flash_ps_suspend(NORMAL_PS);
	if (address >= s_flash.flash_cfg->flash_size) {
		FLASH_LOGW("write error:invalid address 0x%x\r\n", address);
		return BK_ERR_FLASH_ADDR_OUT_OF_RANGE;
	}

	if (flash_is_area_write_disable(address)) {
		return BK_ERR_FLASH_ADDR_OUT_OF_RANGE;
	}

	flash_write_common(user_buf, address, size);
	flash_ps_resume(NORMAL_PS);

	return BK_OK;
}

uint32_t bk_flash_get_id(void)
{
	flash_ps_suspend(NORMAL_PS);
	s_flash.flash_id = flash_hal_get_id(&s_flash.hal);
	flash_ps_resume(NORMAL_PS);
	return s_flash.flash_id;
}

flash_line_mode_t bk_flash_get_line_mode(void)
{
	return s_flash.flash_cfg->line_mode;
}

bk_err_t bk_flash_set_clk_dpll(void)
{
	flash_ps_suspend(NORMAL_PS);
	sys_drv_flash_set_dpll();
	flash_hal_set_clk_dpll(&s_flash.hal);
	flash_ps_resume(NORMAL_PS);

	return BK_OK;
}

bk_err_t bk_flash_set_clk_dco(void)
{
	flash_ps_suspend(NORMAL_PS);
	sys_drv_flash_set_dco();
	bool ate_enabled = ate_is_enabled();
	flash_hal_set_clk_dco(&s_flash.hal, ate_enabled);
	flash_ps_resume(NORMAL_PS);

	return BK_OK;
}

#if (CONFIG_SOC_BK7256XX)
bk_err_t bk_flash_set_clk(flash_clk_src_t flash_src_clk, uint8_t flash_dpll_div)
{
	if ((FLASH_CLK_DPLL == flash_src_clk) && (flash_dpll_div == 0)) {
		FLASH_LOGE("flash 120M clock not support.\r\n");
		return BK_FAIL;
	}
	if (FLASH_CLK_APLL == flash_src_clk) {
		FLASH_LOGE("flash apll clock not support.\r\n");
		return BK_FAIL;
	}

	uint32_t int_level = rtos_disable_int();
	if((sys_drv_flash_get_clk_sel() == flash_src_clk) && (sys_drv_flash_get_clk_div() == flash_dpll_div)) {
		rtos_enable_int(int_level);
		return BK_OK;
	}
	if (FLASH_CLK_DPLL == flash_src_clk) {
		sys_drv_flash_set_clk_div(flash_dpll_div);
	}
	sys_drv_flash_cksel(flash_src_clk);
	rtos_enable_int(int_level);

	return BK_OK;
}

bk_err_t bk_flash_clk_switch(uint32_t flash_speed_type, uint32_t modules)
{
	uint32_t int_level = rtos_disable_int();
	int chip_id = 0;

	switch (flash_speed_type) {
		case FLASH_SPEED_LOW:
			s_hold_low_speed_status |= modules;
			FLASH_LOGD("%s: set low, 0x%x 0x%x\r\n", __func__, s_hold_low_speed_status, modules);
			if (s_hold_low_speed_status) {
				bk_flash_set_clk(FLASH_CLK_XTAL, FLASH_DPLL_DIV_VALUE_TEN);
			}
			break;

		case FLASH_SPEED_HIGH:
			s_hold_low_speed_status &= ~(modules);
			FLASH_LOGD("%s: clear low bit, 0x%x 0x%x\r\n", __func__, s_hold_low_speed_status, modules);
			if (0 == s_hold_low_speed_status) {
				chip_id = bk_get_hardware_chip_id_version();
				//chipC version with GD flash switch to 80M for peformance
				if ((chip_id == CHIP_VERSION_C) && ((s_flash.flash_id >> FLASH_ManuFacID_POSI) == FLASH_ManuFacID_GD)) {
					bk_flash_set_clk(FLASH_CLK_DPLL, FLASH_DPLL_DIV_VALUE_SIX);
				} else {
					bk_flash_set_clk(FLASH_CLK_DPLL, FLASH_DPLL_DIV_VALUE_TEN);
				}
			}
			break;
	}
	rtos_enable_int(int_level);

	return BK_OK;
}
#endif

bk_err_t bk_flash_write_enable(void)
{
	flash_ps_suspend(NORMAL_PS);
	flash_hal_write_enable(&s_flash.hal);
	flash_ps_resume(NORMAL_PS);
	return BK_OK;
}

bk_err_t bk_flash_write_disable(void)
{
	flash_ps_suspend(NORMAL_PS);
	flash_hal_write_disable(&s_flash.hal);
	flash_ps_resume(NORMAL_PS);
	return BK_OK;
}

uint16_t bk_flash_read_status_reg(void)
{
	flash_ps_suspend(NORMAL_PS);
	uint16_t sr_data = flash_hal_read_status_reg(&s_flash.hal, s_flash.flash_cfg->status_reg_size);
	flash_ps_resume(NORMAL_PS);
	return sr_data;
}

bk_err_t bk_flash_write_status_reg(uint16_t status_reg_data)
{
	flash_ps_suspend(NORMAL_PS);
	flash_hal_write_status_reg(&s_flash.hal, s_flash.flash_cfg->status_reg_size, status_reg_data);
	flash_ps_resume(NORMAL_PS);
	return BK_OK;
}

flash_protect_type_t bk_flash_get_protect_type(void)
{
	uint32_t type = 0;
	uint16_t protect_value = 0;

	flash_ps_suspend(NORMAL_PS);
	protect_value = flash_hal_get_protect_value(&s_flash.hal, s_flash.flash_cfg->status_reg_size,
												s_flash.flash_cfg->protect_post, s_flash.flash_cfg->protect_mask,
												s_flash.flash_cfg->cmp_post);
	if (protect_value == s_flash.flash_cfg->protect_all)
		type = FLASH_PROTECT_ALL;
	else if (protect_value == s_flash.flash_cfg->protect_none)
		type = FLASH_PROTECT_NONE;
	else if (protect_value == s_flash.flash_cfg->protect_half)
		type = FLASH_PROTECT_HALF;
	else if (protect_value == s_flash.flash_cfg->unprotect_last_block)
		type = FLASH_UNPROTECT_LAST_BLOCK;
	else
		type = -1;

	flash_ps_resume(NORMAL_PS);
	return type;
}

bk_err_t bk_flash_set_protect_type(flash_protect_type_t type)
{
	flash_ps_suspend(NORMAL_PS);
	flash_set_protect_type(type);
	flash_ps_resume(NORMAL_PS);
	return BK_OK;
}

/* This API is not used in bk7256xx */
void flash_ps_pm_init(void)
{
	PM_STATUS *flash_ps_status_ptr = &flash_ps_status;

	bk_flash_set_clk_dco();
	bk_flash_get_id();

	flash_ps_status_ptr->bits.unconditional_ps_support = 1;
	flash_ps_status_ptr->bits.unconditional_ps_suspend_allow = 1;
	flash_ps_status_ptr->bits.unconditional_ps_resume_allow = 1;
	flash_ps_status_ptr->bits.unconditional_ps_sleeped = 0;
	flash_ps_status_ptr->bits.normal_ps_support = 1;
	flash_ps_status_ptr->bits.normal_ps_suspend_allow = 1;
	flash_ps_status_ptr->bits.normal_ps_resume_allow = 1;
	flash_ps_status_ptr->bits.normal_ps_sleeped = 0;
	flash_ps_status_ptr->bits.lowvol_ps_support = 1;
	flash_ps_status_ptr->bits.lowvol_ps_suspend_allow = 1;
	flash_ps_status_ptr->bits.lowvol_ps_resume_allow = 1;
	flash_ps_status_ptr->bits.lowvol_ps_sleeped = 0;
	flash_ps_status_ptr->bits.deep_ps_support = 1;
	flash_ps_status_ptr->bits.deep_ps_suspend_allow = 1;
	flash_ps_status_ptr->bits.deep_ps_resume_allow = 1;
	flash_ps_status_ptr->bits.deep_ps_sleeped = 0;

	dev_pm_register(PM_ID_FLASH, "flash", &flash_ps_ops);
}

bool bk_flash_is_driver_inited()
{
	return s_flash_is_init;
}

uint32_t bk_flash_get_current_total_size(void)
{
	return s_flash.flash_cfg->flash_size;
}

bk_err_t bk_flash_register_ps_suspend_callback(flash_ps_callback_t ps_suspend_cb)
{
	s_flash_ps_suspend_cb = ps_suspend_cb;
	return BK_OK;
}

bk_err_t bk_flash_register_ps_resume_callback(flash_ps_callback_t ps_resume_cb)
{
	s_flash_ps_resume_cb = ps_resume_cb;
	return BK_OK;
}

#define FLASH_OPERATE_SIZE_AND_OFFSET    (4096)
bk_err_t bk_spec_flash_write_bytes(bk_partition_t partition, const uint8_t *user_buf, uint32_t size,uint32_t offset)
{
	bk_logic_partition_t *bk_ptr = NULL;
	u8 *save_flashdata_buff  = NULL;
	flash_protect_type_t protect_type;
     
	bk_ptr = bk_flash_partition_get_info(partition);
	if((size + offset) > FLASH_OPERATE_SIZE_AND_OFFSET)
		return BK_FAIL;
	
	save_flashdata_buff= os_malloc(bk_ptr->partition_length);
	if(save_flashdata_buff == NULL)
	{
		os_printf("save_flashdata_buff malloc err\r\n");
		return BK_FAIL;
	}

	bk_flash_read_bytes((bk_ptr->partition_start_addr),(uint8_t *)save_flashdata_buff, bk_ptr->partition_length);
    
	protect_type = bk_flash_get_protect_type();
	bk_flash_set_protect_type(FLASH_PROTECT_NONE);
    
	bk_flash_erase_sector(bk_ptr->partition_start_addr);
	os_memcpy((save_flashdata_buff + offset), user_buf, size);
	bk_flash_write_bytes(bk_ptr->partition_start_addr ,(uint8_t *)save_flashdata_buff, bk_ptr->partition_length);	
    bk_flash_set_protect_type(protect_type);
        
	os_free(save_flashdata_buff);
	save_flashdata_buff = NULL;
    
	return BK_OK;

}


