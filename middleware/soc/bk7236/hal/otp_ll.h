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

// This is a generated file, if you need to modify it, use the script to
// generate and modify all the struct.h, ll.h, reg.h, debug_dump.c files!

#pragma once

#include <soc/soc.h>
#include "hal_port.h"
#include "otp_hw.h"
#include <driver/hal/hal_flash_types.h>
#include "system_hw.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OTP_LL_REG_BASE(_otp_unit_id)    (SOC_OTP_APB_BASE)

static inline uint32_t otp_ll_check_busy(otp_hw_t *hw)
{
	return hw->status.busy;
}

static inline void otp_ll_init(otp_hw_t *hw)
{
	sys_ll_set_cpu_device_clk_enable_otp_cken(1);
	sys_ll_set_cpu_power_sleep_wakeup_pwd_encp(0);
	while(otp_ll_check_busy(hw));
}

static inline void otp_ll_deinit()
{
	sys_ll_set_cpu_device_clk_enable_otp_cken(0);
}

static inline uint32_t otp_ll_read_tmlck()
{
	otp_hw_t *hw = (otp_hw_t*)(SOC_OTP_APB_BASE);
	return hw->module_flag.tmlck;
}

static inline void otp_ll_enable_tmlck(otp_hw_t *hw)
{
	hw->module_flag.tmlck = 0xF;
}

static inline void otp_ll_enable_puflck(otp_hw_t *hw)
{
	hw->module_flag.puflck = 0xF;
}

static inline uint32_t otp_ll_read_puflck(otp_hw_t *hw)
{
	return hw->module_flag.puflck;
}

static inline void otp_ll_enable_otplck(otp_hw_t *hw)
{
	hw->module_flag.otplck = 0xF;
}

static inline uint32_t otp_ll_read_otplck(otp_hw_t *hw)
{
	return hw->module_flag.otplck;
}

static inline void otp_ll_enable_shfwen(otp_hw_t *hw)
{
	hw->module_flag.shfwen = 0xF;
}

static inline uint32_t otp_ll_read_shfwen(otp_hw_t *hw)
{
	return hw->module_flag.shfwen;
}

static inline void otp_ll_enable_shfren(otp_hw_t *hw)
{
	hw->module_flag.shfwen = 0xF;
}

static inline uint32_t otp_ll_read_shfren(otp_hw_t *hw)
{
	return hw->module_flag.shfwen;
}

static inline void otp_ll_enable_pgmprt(otp_hw_t *hw)
{
	hw->module_flag.pgmprt = 0xF;
}

static inline uint32_t otp_ll_read_pgmprt(otp_hw_t *hw)
{
	return hw->module_flag.pgmprt;
}

static inline uint32_t otp_ll_read_enroll(otp_hw_t *hw)
{
	return hw->module_flag.enroll;
}

static inline void otp_ll_write_otp2_permission(otp_hw_t *hw, uint32_t location)
{
	hw->rolck[location / 256].v |= (0xF << (location % 256 / 32) * 4);
}

static inline uint32_t otp_ll_read_otp2_permission(otp_hw_t *hw, uint32_t location)
{
	return (hw->rolck[location / 256].v << (location % 256 / 32) * 4) & 0xF;
}

static inline void otp_ll_write_otp_security(otp_hw_t *hw, uint32_t location)
{
	hw->sectrl.v |= (0x7 << (location / 64 * 3));
}

static inline uint32_t otp_ll_read_otp_security(otp_hw_t *hw, uint32_t location)
{
	return hw->sectrl.v << (location / 64 * 3) & 0x7;
}

static inline void otp_ll_write_puf_security(otp_hw_t *hw, uint32_t location)
{
	hw->sectrl.v |= (0x7 << (location / 8 * 3 + 12));
}

static inline uint32_t otp_ll_read_puf_security(otp_hw_t *hw, uint32_t location)
{
	return hw->sectrl.v << (location / 8 * 3 + 12) & 0x7;
}

static inline void otp_ll_enable_security_protection(otp_hw_t *hw)
{
	hw->sectrl.secr_prot_en = 0xF;
}

static inline uint32_t otp_ll_read_security_protection(otp_hw_t *hw)
{
	return hw->sectrl.secr_prot_en;
}

static inline uint32_t otp_ll_read_puf_zeroized_flag(otp_hw_t *hw, uint32_t location)
{
	return hw->zeroized_puf.v << (location / 8 * 2) & 0x3;
}

static inline uint32_t otp_ll_read_otp_zeroized_flag(otp_hw_t *hw, uint32_t location)
{
	return hw->zeroized_otp[location / 128].v << (location % 128 / 8 * 2) & 0x3;
}

static inline void otp_ll_write_puf_permission(otp_hw_t *hw, uint32_t location)
{
	hw->puf_lckwd[location / 8].v |= (0xF << (location % 8) * 4);
}

static inline uint32_t otp_ll_read_puf_permission(otp_hw_t *hw, uint32_t location)
{
	return hw->puf_lckwd[location / 8].v << ((location % 8) * 4) & 0xF;
}

static inline void otp_ll_write_otp_permission(otp_hw_t *hw, uint32_t location)
{
	hw->otp_lckwd[location / 8].v |= (0xF << (location % 8) * 4);
}

static inline uint32_t otp_ll_read_otp_permission(otp_hw_t *hw, uint32_t location)
{
	return hw->otp_lckwd[location / 8].v << (location % 8 * 4) & 0xF;
}

static inline uint32_t otp_ll_read_random_number(otp_hw_t *hw)
{
	return hw->random_value.v;
}

static inline uint32_t otp_ll_read_intrpt_status(otp_hw_t *hw)
{
	return hw->intrpt.intrpt_st;
}

static inline void otp_ll_write_intrpt_enable(otp_hw_t *hw, uint32_t value)
{
	hw->intrpt.intrpt_en = value;
}

static inline uint32_t otp_ll_read_intrpt_enable(otp_hw_t *hw)
{
	return hw->intrpt.intrpt_en;
}

static inline void otp_ll_write_otp2_mask(otp_hw_t *hw, uint32_t location)
{
	hw->cde_mask[location / 512].v |= 0x3 << (location % 512 / 32 * 2);
}

static inline uint32_t otp_ll_read_otp2_mask(otp_hw_t *hw, uint32_t location)
{
	return hw->cde_mask[location / 512].v << (location % 512 / 32 * 2) & 0x3;
}

static inline void otp_ll_write_otp_mask(otp_hw_t *hw, uint32_t location)
{
	hw->otp_mask[location / 128].v |= 0x3 << (location % 128 / 8 * 2);
}

static inline uint32_t otp_ll_read_otp_mask(otp_hw_t *hw, uint32_t location)
{
	return hw->otp_mask[location / 128].v << (location % 128 / 8 * 2) & 0x3;
}

static inline void otp_ll_write_puf_mask(otp_hw_t *hw, uint32_t location)
{
	hw->puf_mask.v |= 0x3 << (location / 8 * 2);
}

static inline uint32_t otp_ll_read_puf_mask(otp_hw_t *hw, uint32_t location)
{
	return hw->puf_mask.v << (location / 8 * 2) & 0x3;
}

static inline void otp_ll_enable_mask_lck(otp_hw_t *hw)
{
	hw->puf_mask.lck_psmsk = 0xFF;
}

static inline uint32_t otp_ll_read_mask_lck(otp_hw_t *hw)
{
	return hw->puf_mask.lck_psmsk;
}

static inline uint32_t otp_ll_read_version(otp_hw_t *hw)
{
	return hw->version.v;
}

static inline uint32_t otp_ll_read_status(otp_hw_t *hw)
{
	return hw->status.v;
}

static inline uint32_t otp_ll_do_auto_repair(otp_hw_t *hw)
{
	if(otp_ll_read_tmlck(hw) == 0){
		hw->auto_repair.auto_repair = 0xE9;
	}
	while(otp_ll_check_busy(hw));
	return hw->status.v;
}

/*Temperature < 85 centigrade*/
static inline uint32_t otp_ll_init_off_check(otp_hw_t *hw)
{
	if(otp_ll_read_tmlck(hw) == 0){
		hw->ini_off_check.v = 0x5A;
	}
	while(otp_ll_check_busy(hw));
	return hw->status.v;
}

/*Temperature > 85 centigrade*/
static inline uint32_t otp_ll_init_off_check_high_temperature(otp_hw_t *hw) 
{
	if(otp_ll_read_tmlck(hw) == 0){
		hw->ini_off_check.v = 0x83;
	}
	while(otp_ll_check_busy(hw));
	return hw->status.v;
}

static inline uint32_t otp_ll_do_puf_quality_check(otp_hw_t *hw)
{
	hw->puf_qty_chk.v = 0x0;
	while(otp_ll_check_busy(hw));
	return hw->status.v;
}

static inline uint32_t otp_ll_do_puf_health_check(otp_hw_t *hw)
{
	hw->puf_qty_chk.v = 0xdc;
	while(otp_ll_check_busy(hw));
	return hw->status.v;
}

static inline void otp_ll_do_puf_enroll(otp_hw_t *hw)
{
	if(otp_ll_read_tmlck(hw) == 0 && otp_ll_read_puflck(hw) == 0){
		hw->puf_enroll.v = 0xA7;
	}
	while(otp_ll_check_busy(hw));
}

static inline void otp_ll_zeroize_puf(otp_hw_t *hw, uint32_t location)
{
	if(otp_ll_read_puflck(hw) == 0){
		switch (location / 8)
		{
		case 0:
			hw->puf_zeroize.v = 0x4B;
			break;

		case 1:
			hw->puf_zeroize.v = 0xAD;
			break;

		case 2:
			hw->puf_zeroize.v = 0xD2;
			break;

		case 3:
			hw->puf_zeroize.v = 0x34;
			break;

		default:
			break;
		}
	}
}

static inline void otp_ll_zeroize_otp(otp_hw_t *hw, uint32_t location)
{
	if(otp_ll_read_puflck(hw) == 0){
		hw->otp_zeroize.v = 0x80 + (location / 8);
	}
}

static inline void otp_ll_set_flag(otp_hw_t *hw, uint32_t flag)
{
	switch (flag)
	{
	case 0xb6:
		/* enable the propragm protect function */
		hw->set_flag.v = 0xB6;
		break;

	case 0x71:
		/* enable the Test Mode Lock function */
		hw->set_flag.v = 0x71;
		break;

	case 0x2c:
		/* enable the PUF Lock function */
		hw->set_flag.v = 0x2C;
		break;

	case 0x65:
		/* enable the OTP Lock function */
		hw->set_flag.v = 0x65;
		break;

	case 0x99:
		/* enable the shuffle function for OTP/PUF read */
		hw->set_flag.v = 0x99;
		break;

	case 0xc2:
		/* enable the shuffle function for OTP/PUF write */
		hw->set_flag.v = 0xC2;
		break;

	default:
		break;
	}
}

static inline uint32_t otp_ll_read_puf(otp_hw_t *hw, uint32_t location)
{
	return hw->puf[location].v;
}

static inline uint32_t otp_ll_read_puf_entropy(otp_hw_t *hw, uint32_t location)
{
	return hw->puf[location].v;
}

static inline uint32_t otp_ll_read_otp(otp_hw_t *hw, uint32_t location)
{
	return hw->otp[location].v;
}

static inline void otp_ll_write_otp(otp_hw_t *hw, uint32_t location, uint32_t value)
{
	hw->otp[location].v = value;
}

static inline void otp_ll_enable_pdstb(otp_hw_t *hw)
{
	hw->hardware.pdstb = 1;
}

static inline void otp_ll_enable_clkosc_en(otp_hw_t *hw)
{
	hw->hardware.clkosc_en = 1;
}

static inline void otp_ll_enable_fre_cont(otp_hw_t *hw)
{
	hw->hardware.fre_cont = 1;
}

static inline void otp_ll_sleep(otp_hw_t *hw)
{
	hw->hardware.v = 0;
}

#ifdef __cplusplus
}
#endif