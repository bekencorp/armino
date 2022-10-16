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

#include "sys_driver.h"
#include "psram_hal.h"

static void psram_delay(volatile uint32_t times)
{
	while(times--);
}

bk_err_t psram_hal_power_enable(uint8_t enable)
{
	if (enable)
		sys_drv_psram_power_enable();
	else
		sys_drv_psram_ldo_enable(0);

	return BK_OK;
}

bk_err_t psram_hal_config_init(void)
{
	int value;

	/*choose clk 160M*/
	sys_drv_psram_clk_sel(0);
	sys_drv_psram_set_clkdiv(1);

	/*psram clk enable*/
	sys_drv_dev_clk_pwr_up(CLK_PWR_ID_PSRAM, CLK_PWR_CTRL_PWR_UP);

	/*psram controller reset*/
	psram_hal_set_sf_reset(1);

	/*config psram mode*/
	value = 0xd8054043;
	psram_hal_set_mode_value(value);

	/*psram reset*/
	psram_hal_set_cmd_reset();
	psram_delay(5000);

	/*config cmd 0x00000000*/
	psram_hal_cmd_read(0x00000000);
	value = psram_hal_get_regb_value();
	value = (value & ~(0x7 << 10)) | (0x4 << 10);
	psram_hal_cmd_write(0x00000000, value);

	/*config cmd 0x00000004*/
	psram_hal_cmd_read(0x00000004);
	value = psram_hal_get_regb_value();
	value = (value & ~(0x7 << 13)) | (0x6 << 13);
	psram_hal_cmd_write(0x00000004, value);

	psram_delay(100);

	/*reset clk 320M*/
	sys_drv_psram_set_clkdiv(0);

	/*set psram bypass*/
	value = psram_hal_get_reg2_value();
	value |= (0x1 << 1);
	psram_hal_set_reg2_value(value);

	return BK_OK;
}

bk_err_t psram_hal_clk_power_enable(uint8_t enable)
{
	if (enable)
		sys_drv_dev_clk_pwr_up(CLK_PWR_ID_PSRAM, CLK_PWR_CTRL_PWR_UP);
	else
		sys_drv_dev_clk_pwr_up(CLK_PWR_ID_PSRAM, CLK_PWR_CTRL_PWR_DOWN);

	return BK_OK;
}

void psram_hal_continue_write(uint32_t start_addr, uint32_t *data_buf, uint32_t len)
{
	int i;
	uint32_t val;
	uint8_t *pb = NULL, *pd = NULL;

	while (len) {
		if (len < 4) {
			val = REG_READ(start_addr);
			pb = (uint8_t *)&val;
			pd = (uint8_t *)data_buf;
			for (i = 0; i < len; i++) {
				*pb++ = *pd++;
			}
			REG_WRITE(start_addr, val);
			len = 0;
		} else {
			val = *data_buf++;
			REG_WRITE(start_addr, val);
			start_addr += 4;
			len -= 4;
		}
	}
}

void psram_hal_continue_read(uint32_t start_addr, uint32_t *data_buf, uint32_t len)
{
	int i;
	uint32_t val;
	uint8_t *pb, *pd;

	while (len) {
		if (len < 4) {
			val = REG_READ(start_addr);
			pb = (uint8_t *)&val;
			pd = (uint8_t *)data_buf;
			for (i = 0; i < len; i++) {
				*pd++ = *pb++;
			}
			len = 0;
		} else {
			val = REG_READ(start_addr);
			*data_buf++ = val;
			start_addr += 4;
			len -= 4;
		}
	}
}

void psram_hal_set_clk(psram_clk_t clk)
{
	switch (clk) {
		case PSRAM_240M:
			sys_drv_psram_clk_sel(1);	 // clk sel: 0-320 1-480
			sys_drv_psram_set_clkdiv(0); //frq:  F/(2 + (1+div))
			break;
		case PSRAM_160M:
			sys_drv_psram_clk_sel(0);	 // clk sel: 0-320 1-480
			sys_drv_psram_set_clkdiv(1); //frq:  F/(2 + (1+div))
			break;
		case PSRAM_120M:
			sys_drv_psram_clk_sel(1);	 // clk sel: 0-320 1-480
			sys_drv_psram_set_clkdiv(1); //frq:  F/(2 + (1+div))
			break;
		case PSRAM_80M:
			sys_drv_psram_clk_sel(0);	 // clk sel: 0-320 1-480
			sys_drv_psram_set_clkdiv(1); //frq:  F/(2 + (1+div))
			break;
		default:
			break;
	}
}

void psram_hal_set_voltage(psram_voltage_t voltage)
{
	switch (voltage) {
		case PSRAM_OUT_3_2V:
			sys_drv_psram_psldo_vset(0, 1);
			break;
		case PSRAM_OUT_3V:
			sys_drv_psram_psldo_vset(0, 0);
			break;
		case PSRAM_OUT_2V:
			sys_drv_psram_psldo_vset(1, 1);
			break;
		case PSRAM_OUT_1_8V:
			sys_drv_psram_psldo_vset(1, 0);
			break;
		default:
			break;
	}
}
