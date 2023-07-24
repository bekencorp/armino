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
#include <driver/otp_types.h>
#include <driver/vault.h>
#include <string.h>
#include "otp_driver.h"
#include "otp_hal.h"

typedef struct {
	otp_hal_t hal;
} otp_driver_t;

static otp_driver_t s_otp = {0};

bk_err_t bk_otp_init()
{
	otp_hal_init(&s_otp.hal);
	return BK_OK;
}

bk_err_t bk_otp_deinit()
{
	otp_hal_sleep(&s_otp.hal);
	return BK_OK;
}

bk_err_t bk_otp_read_otp(uint32_t location, uint32_t* value)
{
	if(otp_hal_read_otp_permission(&s_otp.hal,location) == 0 && otp_hal_read_otp_mask(&s_otp.hal,location) == 0){
		*value = otp_hal_read_otp(&s_otp.hal,location);
		return BK_OK;
	} else {
		OTP_LOGE("OTP read error!");
		return BK_FAIL;
	}
	return BK_FAIL;
}

bk_err_t bk_otp_write_otp(uint32_t location, uint32_t value)
{
	otp_hal_write_otp(&s_otp.hal,location,value);
	return BK_OK;
}

bk_err_t bk_otp_init_puf()
{
	if(otp_hal_read_enroll(&s_otp.hal) == 0x0){
		otp_hal_do_puf_enroll(&s_otp.hal);
	} else {
		OTP_LOGE("Puf has inited\r\n");
		return BK_FAIL;
	}

	if(otp_hal_read_enroll(&s_otp.hal) == 0xF){
		return BK_OK;
	} else {
		OTP_LOGE("Error code:0x%x\r\n",otp_hal_read_status(&s_otp.hal));
		return BK_FAIL;
	}

	return BK_FAIL;
}

bk_err_t bk_otp_read_raw_entropy_output(uint32_t* value)
{
	otp_hal_enable_pdstb(&s_otp.hal);
	otp_hal_enable_clkosc_en(&s_otp.hal);
	otp_hal_enable_fre_cont(&s_otp.hal);
	*value = otp_hal_read_random_number(&s_otp.hal);
	return BK_OK;
}

bk_err_t bk_enable_security_protect()
{
	otp_hal_enable_security_protection(&s_otp.hal);
	return BK_OK;
}

bk_err_t bk_set_otp_permission(uint32_t location)
{
	otp_hal_write_otp_permission(&s_otp.hal,location);
	return BK_OK;
}

bk_err_t bk_set_puf_permission(uint32_t location)
{
	otp_hal_write_puf_permission(&s_otp.hal,location);
	return BK_OK;
}

bk_err_t bk_set_otp_mask(uint32_t location)
{
	otp_hal_write_otp_mask(&s_otp.hal,location);
	return BK_OK;
}

bk_err_t bk_set_otp2_mask(uint32_t location)
{
	otp_hal_write_otp2_mask(&s_otp.hal,location);
	return BK_OK;
}

bk_err_t bk_set_puf_mask(uint32_t location)
{
	otp_hal_write_puf_mask(&s_otp.hal,location);
	return BK_OK;
}

bk_err_t bk_set_lck_mask(uint32_t location)
{
	otp_hal_write_puf_mask(&s_otp.hal,location);
	return BK_OK;
}

bk_err_t bk_read_otp_mask(uint32_t location, uint32_t* value)
{
	*value = otp_hal_read_otp_mask(&s_otp.hal,location);
	return BK_OK;
}

bk_err_t bk_zeroize_otp(uint32_t location)
{
	otp_hal_zeroize_otp(&s_otp.hal,location);
	return BK_OK;
}

bk_err_t bk_zeroize_puf(uint32_t location)
{
	otp_hal_zeroize_puf(&s_otp.hal,location);
	return BK_OK;
}

bk_err_t bk_get_otp_zeroized_flag(uint32_t location, uint32_t* flag)
{
	*flag = otp_hal_read_otp_zeroized_flag(&s_otp.hal,location);
	return BK_OK;
}

bk_err_t bk_read_otp_permission(uint32_t location, uint32_t* value)
{
	*value = otp_hal_read_otp_permission(&s_otp.hal,location);
	return BK_OK;
}