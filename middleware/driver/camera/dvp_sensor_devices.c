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

#include <driver/int.h>
#include <os/mem.h>
#include <os/os.h>
#include <os/str.h>


#include <driver/i2c.h>

#include "gpio_map.h"

#include "dvp_sensor_devices.h"

#define DVP_I2C_TIMEOUT (2000)

const dvp_sensor_config_t *dvp_sensor_configs[] =
{
	&dvp_sensor_gc0328c,
	&dvp_sensor_hm1055,
	&dvp_sensor_gc2145,
	&dvp_sensor_ov2640,
	&dvp_sensor_gc0308,
};

const dvp_sensor_config_t **get_sensor_config_devices_list(void)
{
	return &dvp_sensor_configs[0];
}

int get_sensor_config_devices_num(void)
{
	return sizeof(dvp_sensor_configs) / sizeof(dvp_sensor_config_t *);
}

const dvp_sensor_config_t *get_sensor_config_interface_by_id(sensor_id_t id)
{
	uint32_t i, size = sizeof(dvp_sensor_configs) / sizeof(dvp_sensor_config_t *);

	for (i = 0; i < size; i++)
	{
		if (dvp_sensor_configs[i]->id == id)
		{
			return dvp_sensor_configs[i];
		}
	}

	return NULL;
}

const dvp_sensor_config_t *bk_dvp_get_sensor_auto_detect(void)
{
	uint32_t i, size = sizeof(dvp_sensor_configs) / sizeof(const dvp_sensor_config_t *);
	uint8_t count = 3;

	do {
		for (i = 0; i < size; i++)
		{
			if (dvp_sensor_configs[i]->detect() == true)
			{
				return dvp_sensor_configs[i];
			}
		}

		count--;

		rtos_delay_milliseconds(5);

	} while (count > 0);

	return NULL;
}


int dvp_camera_i2c_read_uint8(uint8_t addr, uint8_t reg, uint8_t *value)
{
	i2c_mem_param_t mem_param = {0};

	mem_param.dev_addr = addr;
	mem_param.mem_addr_size = I2C_MEM_ADDR_SIZE_8BIT;
	mem_param.data_size = 1;
	mem_param.timeout_ms = DVP_I2C_TIMEOUT;
	mem_param.mem_addr = reg;
	mem_param.data = value;

	return bk_i2c_memory_read(CAMERA_DVP_I2C_ID, &mem_param);
}

int dvp_camera_i2c_read_uint16(uint8_t addr, uint16_t reg, uint8_t *value)
{
	i2c_mem_param_t mem_param = {0};

	mem_param.dev_addr = addr;
	mem_param.mem_addr_size = I2C_MEM_ADDR_SIZE_16BIT;
	mem_param.data_size = 1;
	mem_param.timeout_ms = DVP_I2C_TIMEOUT;
	mem_param.mem_addr = reg;
	mem_param.data = value;

	return bk_i2c_memory_read(CAMERA_DVP_I2C_ID, &mem_param);
}

int dvp_camera_i2c_write_uint8(uint8_t addr, uint8_t reg, uint8_t value)
{
	i2c_mem_param_t mem_param = {0};
	mem_param.dev_addr = addr;
	mem_param.mem_addr_size = I2C_MEM_ADDR_SIZE_8BIT;
	mem_param.data_size = 1;
	mem_param.timeout_ms = DVP_I2C_TIMEOUT;
	mem_param.mem_addr = reg;
	mem_param.data = (uint8_t *)(&value);

	return bk_i2c_memory_write(CAMERA_DVP_I2C_ID, &mem_param);
}

int dvp_camera_i2c_write_uint16(uint8_t addr, uint16_t reg, uint8_t value)
{
	i2c_mem_param_t mem_param = {0};
	mem_param.dev_addr = addr;
	mem_param.mem_addr_size = I2C_MEM_ADDR_SIZE_16BIT;
	mem_param.data_size = 1;
	mem_param.timeout_ms = DVP_I2C_TIMEOUT;
	mem_param.mem_addr = reg;
	mem_param.data = (uint8_t *)(&value);

	return bk_i2c_memory_write(CAMERA_DVP_I2C_ID, &mem_param);
}


