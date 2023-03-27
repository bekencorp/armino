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
#include "bk_misc.h"
#include <modules/chip_support.h>

// config 1: psram power and clk config, need wait clk stable
void psram_hal_power_clk_enable(uint8_t enable)
{
	if (enable)
	{
		// ldo power up
		sys_drv_psram_ldo_enable(1);

		// psram bus clk always open
		sys_drv_psram_psram_disckg(1);

		//psram 160M
		psram_hal_set_clk(PSRAM_160M);

		// when use psram 120M, need open this code
		//aon_pmu_hal_psram_iodrv_set(0x2);

		//setf_SYSTEM_Reg0xc_psram_cken;
		sys_drv_dev_clk_pwr_up(CLK_PWR_ID_PSRAM, CLK_PWR_CTRL_PWR_UP);//psram_clk_enable bit19=1
	}
	else
	{
		sys_drv_dev_clk_pwr_up(CLK_PWR_ID_PSRAM, CLK_PWR_CTRL_PWR_DOWN);//psram_clk_disable

		// power down
		sys_drv_psram_ldo_enable(0);
	}
}

// config 2: reset psram and wait psram ready
void psram_hal_reset(void)
{
	int chip_id = 0;
	psram_hal_set_sf_reset(1);

	chip_id = bk_get_hardware_chip_id_version();

	if (chip_id == CHIP_VERSION_C)
		psram_hal_set_mode_value(0xE8054043);
	else
		psram_hal_set_mode_value(0xa8054043);

	psram_hal_set_cmd_reset();
}

// config 3: psram config
void psram_hal_config(void)
{
	uint32_t val = 0;

	psram_hal_cmd_read(0x00000000);//1 0001 10001101

	val = psram_hal_get_regb_value();

	val = (val & ~(0x1F << 8)) | (0x4 << 10) | (0x3 << 8);

	psram_hal_cmd_write(0x00000000, val);

	psram_hal_cmd_read(0x00000000);//1 0001 10001101

	psram_hal_cmd_read(0x00000004);

	val = psram_hal_get_regb_value();
	val = (val & ~(0x7 << 13)) | (0x6 << 13);//write latency 110 166Mhz

	psram_hal_cmd_write(0x00000004, val);
}


void psram_hal_config_init(void)
{
	uint32_t chip_id = 0;
	uint32_t val = 0;

	// wait clk stable
	delay_us(5);

	psram_hal_set_sf_reset(1);

	chip_id = bk_get_hardware_chip_id_version();

	if (chip_id == CHIP_VERSION_C)
		psram_hal_set_mode_value(0xE8054043);
	else
		psram_hal_set_mode_value(0x98054043);//

	psram_hal_set_cmd_reset();
	delay_us(100);//40

	psram_hal_cmd_read(0x00000000);//1 0001 10001101

	val = psram_hal_get_regb_value();

	val = (val & ~(0x1F << 8)) | (0x4 << 10) | (0x3 << 8);

	psram_hal_cmd_write(0x00000000, val);

	psram_hal_cmd_read(0x00000000);//1 0001 10001101

	psram_hal_cmd_read(0x00000004);

	val = psram_hal_get_regb_value();
	val = (val & ~(0x7 << 13)) | (0x6 << 13);//write latency 110 166Mhz

	psram_hal_cmd_write(0x00000004, val);
}

void psram_hal_continue_write(uint32_t start_addr, uint32_t *data_buf, uint32_t len)
{
#if 0
	uint32_t i = 0;
	uint32_t data_len = len;
	uint32_t saddr=start_addr;
	uint32_t daddr=(uint32_t)data_buf;

	while(data_len) {
		if(data_len > 12) {
			hb_write_data_12w(saddr ,daddr);
			data_len = data_len - 12;
			saddr += 48;
			daddr += 48;
		} else if (data_len > 8) {
			hb_write_data_8w(saddr ,daddr);
			data_len = data_len - 8;
			saddr += 32;
			daddr += 32;
		} else {
			hb_write_data(saddr ,daddr);
			data_len --;
			saddr += 4;
			daddr += 4;
		}
		i++;
	}
#else
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
#endif
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
	switch (clk)
	{
		case PSRAM_240M:
			sys_drv_psram_clk_sel(1);	 // clk sel: 0-320 1-480
			sys_drv_psram_set_clkdiv(0); //frq:  F/(2 + (1+div))
			break;
		case PSRAM_160M:
			sys_drv_psram_clk_sel(0);	 // clk sel: 0-320 1-480
			sys_drv_psram_set_clkdiv(0); //frq:  F/(2 + (1+div))
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
	switch (voltage)
	{
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

char* psram_strcat(char* start_addr, const char *data_buf)
{
	int i;
	uint32_t val;
	uint8_t *pb;
	uint8_t *pd = (uint8_t *)data_buf;
	if(*pd == '\0')
	{
		return start_addr;
	}
	do
	{
		val = *(uint32_t *)(start_addr);
		pb = (uint8_t *)&val;
		for (i = 0; i < 4; i++) {
			if(*(pb+i) == '\0')
			{
				if(*pd == '\0')
				{
					*(pb+i) = *pd;
					break;
				}
				*(pb+i) = *pd++;
			}
		}
		*(uint32_t *)(start_addr) = val;

		if(*pd == '\0')
		{
			break;
		}
		start_addr += 4;
	} while(true);

	return start_addr;
}

