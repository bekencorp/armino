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
#include <driver/gpio.h>
#include <driver/gpio_types.h>
#include <driver/tp.h>
#include <driver/tp_types.h>
#include <driver/i2c.h>
#include <string.h>


#define TAG "gt1151"
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)


// external statement.
extern void bk_mem_dump_ex(const char *title, unsigned char *data, uint32_t data_len);


// macro define
#define GT1151_WRITE_ADDRESS    (0x28)
#define GT1151_READ_ADDRESS     (0x29)
#define GT1151_PRODUCT_ID_CODE  (0x38353131)  // "1151" ASCII code

#define GT1151_ADDR_LEN          (1)
#define GT1151_REG_LEN           (2)
#define GT1151_MAX_TOUCH_NUM     (5)
#define GT1151_POINT_INFO_NUM    (TP_SUPPORT_MAX_NUM)
#define GT1151_POINT_INFO_SIZE   (8)
#define GT1151_POINT_INFO_TOTAL_SIZE  (GT1151_POINT_INFO_NUM * GT1151_POINT_INFO_SIZE)

#define GT1151_COMMAND_REG       (0x8040)

#define GT1151_CONFIG_REG        (0x8050)

#define GT1151_PRODUCT_ID        (0x8140)
#define GT1151_FIRMWARE_VERSION  (0x8144)
#define GT1151_VENDOR_ID         (0x814A)

#define GT1151_STATUS            (0x814E)

#define GT1151_POINT1_REG        (0x814F)
#define GT1151_POINT2_REG        (0x8157)
#define GT1151_POINT3_REG        (0x815F)
#define GT1151_POINT4_REG        (0x8167)
#define GT1151_POINT5_REG        (0x816F)

#define GT1151_CHECK_SUM         (0x813C)

// config parameters and relative position or range. 
#define GT1151_X_OUTPUT_MAX_POS (1)
#define GT1151_Y_OUTPUT_MAX_POS (3)
#define GT1151_TOUCH_NUMBER_POS (5)
#define GT1151_TOUCH_NUMBER_MIN (1)
#define GT1151_TOUCH_NUMBER_MAX (5)
#define GT1151_MODULE_SWITCH1_POS (6)
#define GT1151_REFRESH_RATE_POS (15)
#define GT1151_REFRESH_RATE_MIN (5)
#define GT1151_REFRESH_RATE_MAX (20)
#define GT1151_CHECK_SUM_POS (236)

#define GT1151_REGS_DEBUG_EN (0)

#define SENSOR_I2C_READ(reg, buff, len)  cb->read_uint16((GT1151_WRITE_ADDRESS >> 1), reg, buff, len)
#define SENSOR_I2C_WRITE(reg, buff, len)  cb->write_uint16((GT1151_WRITE_ADDRESS >> 1), reg, buff, len)

// gt1151
#define GT1151_CFG_TABLE_SIZE sizeof(sensor_gt1151_cfg_table)
const uint8_t sensor_gt1151_cfg_table[] =
{
	0x00,0x20,0x03,0xE0,0x01,0x05,0x35,0x04,0x00,0x08,
	0x09,0x0F,0x55,0x37,0x33,0x11,0x00,0x03,0x08,0x56,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x48,0x00,0x00,
	0x3C,0x08,0x0A,0x28,0x1E,0x50,0x00,0x00,0x82,0xB4,
	0xD2,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x85,0x25,0x11,0x41,0x43,0x31,
	0x0D,0x00,0xAD,0x22,0x24,0x7D,0x1D,0x1D,0x32,0xDF,
	0x4F,0x44,0x0F,0x80,0x2C,0x50,0x50,0x00,0x00,0x00,
	0x00,0xD3,0x00,0x00,0x00,0x00,0x0F,0x28,0x1E,0xFF,
	0xF0,0x37,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x50,0xB4,0xC0,0x94,0x53,0x2D,
	0x0A,0x02,0xBE,0x60,0xA2,0x71,0x8F,0x82,0x80,0x92,
	0x74,0xA3,0x6B,0x01,0x0F,0x14,0x03,0x1E,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x0C,0x0D,0x0E,0x0F,0x10,
	0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,
	0x1D,0x1F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x19,0x18,0x17,
	0x15,0x14,0x13,0x12,0x0C,0x08,0x06,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,
	0xC4,0x09,0x23,0x23,0x50,0x5D,0x54,0x4B,0x3C,0x0F,
	0x32,0xFF,0xE4,0x04,0x40,0x00,0x8A,0x05,0x40,0x00,
	0xAA,0x00,0x22,0x22,0x00,0x00,0xAA,0x05,0x00
};


bool gt1151_detect(const tp_i2c_callback_t *cb)
{
	if (NULL == cb)
	{
		LOGE("%s, pointer is null!\r\n", __func__);
		return false;
	}

	uint32_t product_id = 0;

	if (BK_OK != SENSOR_I2C_READ(GT1151_PRODUCT_ID, (uint8_t *)(&product_id), sizeof(product_id)))
	{
		LOGE("%s, read product id reg fail!\r\n", __func__);
		return false;
	}

	LOGI("%s, product id: 0X%08X\r\n", __func__, product_id);

	if (GT1151_PRODUCT_ID_CODE == product_id)
	{
		LOGI("%s success\n", __func__);
		return true;
	}

	return false;
}

static int gt1151_obtain_config(const tp_i2c_callback_t *cb, uint8_t *config, uint8_t size)
{
	#ifdef GT1151_REGS_RECONFIG_EN
		os_memcpy((uint8_t *)config, (uint8_t *)sensor_gt1151_cfg_table, size);
	#else
		if (BK_OK != SENSOR_I2C_READ(GT1151_CONFIG_REG, (uint8_t *)config, size))
		{
			LOGE("%s, obtain config regs fail!\r\n", __func__);
			return BK_FAIL;
		}

		#if (GT1151_REGS_DEBUG_EN > 0)
			bk_mem_dump_ex("tp_gt1151", (unsigned char *)(config), size);
		#endif

		uint16_t check_sum = 0;
		uint16_t temp;

		for (uint8_t index=0; index<size-3; index+=2)
		{
			// checksum
			temp = config[index];
			temp <<= 8;
			temp += config[index+1];
			check_sum += temp;
		}
		check_sum = (~check_sum) + 1;

		uint16_t temp_check_sum = config[GT1151_CHECK_SUM_POS];
		temp_check_sum <<= 8;
		temp_check_sum += config[GT1151_CHECK_SUM_POS+1];

		if (temp_check_sum != check_sum)
		{
			LOGE("%s, check sum 0X%02x and 0X%02x is not equal!\r\n", __func__, temp_check_sum, check_sum);
			return BK_FAIL;
		}
	#endif

	return BK_OK;
}

static int gt1151_update_config(const tp_i2c_callback_t *cb, uint8_t *config, uint8_t size)
{
	uint16_t checksum = 0;
	uint16_t temp;

	for (uint8_t index=0; index<size-3; index+=2)
	{
		// checksum
		temp = config[index];
		temp <<= 8;
		temp |= config[index+1];
		checksum += temp;
	}
	checksum = (~checksum) + 1;
	config[GT1151_CHECK_SUM_POS] = (checksum >> 8) & 0xFF;
	config[GT1151_CHECK_SUM_POS + 1] = checksum & 0xFF;
	config[GT1151_CHECK_SUM_POS + 2] = 1;
	
	if (BK_OK != SENSOR_I2C_WRITE(GT1151_CONFIG_REG, (uint8_t *)config, size))
	{
		LOGE("%s, write config regs fail!\r\n", __func__);
		return BK_FAIL;
	}

	return BK_OK;
}

int gt1151_init(const tp_i2c_callback_t *cb, tp_sensor_user_config_t *config)
{
	if ( (NULL == cb) || (NULL == config) )
	{
		LOGE("%s, pointer is null!\r\n", __func__);
		return BK_FAIL;
	}

	int ret = BK_OK;
	uint8_t *cfg_table = (uint8_t *)os_malloc(GT1151_CFG_TABLE_SIZE);

	if (NULL == cfg_table)
	{
		LOGE("%s, malloc fail!\r\n", __func__);
		ret = BK_FAIL;
		goto exit_;
	}

	// obtain config table.
	os_memset((uint8_t *)cfg_table, 0x00, GT1151_CFG_TABLE_SIZE);
	ret = gt1151_obtain_config(cb, cfg_table, GT1151_CFG_TABLE_SIZE);
	if (BK_OK != ret)
	{
		LOGE("%s, obtain config fail!\r\n", __func__);
		goto exit_;
	}

#if 0
	// renew config parameters.
	// x output max.
	os_memcpy((uint8_t *)(&cfg_table[GT1151_X_OUTPUT_MAX_POS]), (uint8_t *)(&config->x_size), 2);
	// y output max.
	os_memcpy((uint8_t *)(&cfg_table[GT1151_Y_OUTPUT_MAX_POS]), (uint8_t *)(&config->y_size), 2);
	// interrupt trigger type.
	if (TP_INT_TYPE_RISING_EDGE == config->int_type)
	{
		cfg_table[GT1151_MODULE_SWITCH1_POS] &= 0xFC;
		cfg_table[GT1151_MODULE_SWITCH1_POS] |= 0x00;
	}
	else if (TP_INT_TYPE_FALLING_EDGE == config->int_type)
	{
		cfg_table[GT1151_MODULE_SWITCH1_POS] &= 0xFC;
		cfg_table[GT1151_MODULE_SWITCH1_POS] |= 0x01;
	}
	else if (TP_INT_TYPE_LOW_LEVEL == config->int_type)
	{
		cfg_table[GT1151_MODULE_SWITCH1_POS] &= 0xFC;
		cfg_table[GT1151_MODULE_SWITCH1_POS] |= 0x02;
	}	
	else if (TP_INT_TYPE_HIGH_LEVEL == config->int_type)
	{
		cfg_table[GT1151_MODULE_SWITCH1_POS] &= 0xFC;
		cfg_table[GT1151_MODULE_SWITCH1_POS] |= 0x03;
	}
	// refresh rate.
	if (config->refresh_rate > GT1151_REFRESH_RATE_MAX)
	{
		cfg_table[GT1151_REFRESH_RATE_POS] = GT1151_REFRESH_RATE_MAX;
	}
	else if (config->refresh_rate < GT1151_REFRESH_RATE_MIN)
	{
		cfg_table[GT1151_REFRESH_RATE_POS] = GT1151_REFRESH_RATE_MIN;
	}
	else
	{
		cfg_table[GT1151_REFRESH_RATE_POS] = config->refresh_rate - GT1151_REFRESH_RATE_MIN;
	}
	// touch number.
	if (config->tp_num > GT1151_TOUCH_NUMBER_MAX)
	{
		cfg_table[GT1151_TOUCH_NUMBER_POS] = GT1151_TOUCH_NUMBER_MAX;
	}
	else if (config->tp_num < GT1151_TOUCH_NUMBER_MIN)
	{
		cfg_table[GT1151_TOUCH_NUMBER_POS] = GT1151_TOUCH_NUMBER_MIN;
	}
	else
	{
		cfg_table[GT1151_TOUCH_NUMBER_POS] = config->tp_num;
	}
#endif

	ret = gt1151_update_config(cb, cfg_table, GT1151_CFG_TABLE_SIZE);
	if (BK_OK != ret)
	{
		LOGE("%s, update config fail!\r\n", __func__);
	}

exit_:
	if (NULL != cfg_table)
	{
		os_free(cfg_table);
	}

	return ret;
}

int gt1151_read_status(const tp_i2c_callback_t *cb, uint8_t *status)
{
	if ( (NULL == cb) || (NULL == status) )
	{
		LOGE("%s, pointer is null!\r\n", __func__);
		return BK_FAIL;
	}

	if (BK_OK != SENSOR_I2C_READ(GT1151_STATUS, (uint8_t *)(status), 1))
	{
		LOGE("%s, read status reg fail!\r\n", __func__);
		return BK_FAIL;
	}

	LOGD("%s, status=0x%02X\r\n", __func__, *status);

	return BK_OK;
}

int gt1151_clear_status(const tp_i2c_callback_t *cb)
{
	if (NULL == cb)
	{
		LOGE("%s, pointer is null!\r\n", __func__);
		return BK_FAIL;
	}
	
	uint8_t status = 0;
	if (BK_OK != SENSOR_I2C_WRITE(GT1151_STATUS, (uint8_t *)(&status), sizeof(status)))
	{
		LOGE("%s, write status reg fail!\r\n", __func__);
		return BK_FAIL;
	}

	return BK_OK;
}

// gt1151 get tp info.
static int16_t pre_x[GT1151_MAX_TOUCH_NUM] = {-1, -1, -1, -1, -1};
static int16_t pre_y[GT1151_MAX_TOUCH_NUM] = {-1, -1, -1, -1, -1};
static int16_t pre_w[GT1151_MAX_TOUCH_NUM] = {-1, -1, -1, -1, -1};
static uint8_t s_tp_down[GT1151_MAX_TOUCH_NUM];

void gt1151_touch_up(void *buf, int8_t id)
{
    tp_data_t *read_data = (tp_data_t *)buf;

    if(s_tp_down[id] == 1)
    {
        s_tp_down[id] = 0;
        read_data[id].event = TP_EVENT_TYPE_UP;
    }
    else
    {
        read_data[id].event = TP_EVENT_TYPE_NONE;
    }

    read_data[id].timestamp = rtos_get_time();
    read_data[id].width = pre_w[id];
    read_data[id].x_coordinate = pre_x[id];
    read_data[id].y_coordinate = pre_y[id];
    read_data[id].track_id = id;

    pre_x[id] = -1;  /* last point is none */
    pre_y[id] = -1;
    pre_w[id] = -1;
}

void gt1151_touch_down(void *buf, int8_t id, int16_t x, int16_t y, int16_t w)
{
    tp_data_t *read_data = (tp_data_t *)buf;

    if (s_tp_down[id] == 1)
    {
        read_data[id].event = TP_EVENT_TYPE_MOVE;

    }
    else
    {
        read_data[id].event = TP_EVENT_TYPE_DOWN;
        s_tp_down[id] = 1;
    }

    read_data[id].timestamp = rtos_get_time();
    read_data[id].width = w;
    read_data[id].x_coordinate = x;
    read_data[id].y_coordinate = y;
    read_data[id].track_id = id;

    pre_x[id] = x; /* save last point */
    pre_y[id] = y;
    pre_w[id] = w;
}

void gt1151_read_point(uint8_t *input_buff, void *buf, uint8_t num)
{
    uint8_t touch_num = 0;
    uint8_t *read_buf = input_buff;
    uint8_t read_index;
    int8_t read_id = 0;
    int16_t input_x = 0;
    int16_t input_y = 0;
    int16_t input_w = 0;

    static uint8_t pre_touch = 0;
    static int8_t pre_id[GT1151_MAX_TOUCH_NUM] = {0};

	touch_num = num;

    if (pre_touch > touch_num)                                       /* point up */
    {
        for (read_index = 0; read_index < pre_touch; read_index++)
        {
            uint8_t j;

            for (j = 0; j < touch_num; j++)                          /* this time touch num */
            {
                read_id = read_buf[j * 8] & 0x0F;

                if (pre_id[read_index] == read_id)                   /* this id is not free */
                    break;

                if (j >= touch_num - 1)
                {
                    uint8_t up_id;
                    up_id = pre_id[read_index];
                    gt1151_touch_up(buf, up_id);
                }
            }
        }
    }

    if (touch_num)                                                 /* point down */
    {
        uint8_t off_set;

        for (read_index = 0; read_index < touch_num; read_index++)
        {
            off_set = read_index * 8;
            read_id = read_buf[off_set] & 0x0f;
            pre_id[read_index] = read_id;
            input_x = read_buf[off_set + 1] | (read_buf[off_set + 2] << 8);	/* x */
            input_y = read_buf[off_set + 3] | (read_buf[off_set + 4] << 8);	/* y */
            input_w = read_buf[off_set + 5] | (read_buf[off_set + 6] << 8);	/* size */

            gt1151_touch_down(buf, read_id, input_x, input_y, input_w);
        }
    }
    else if (pre_touch)
    {
        for(read_index = 0; read_index < pre_touch; read_index++)
        {
            gt1151_touch_up(buf, pre_id[read_index]);
        }
    }

    pre_touch = touch_num;
}

static uint8_t read_buff[GT1151_POINT_INFO_TOTAL_SIZE];
int gt1151_read_tp_info(const tp_i2c_callback_t *cb, uint8_t max_num, uint8_t *buff)
{
	if ( (NULL == cb) || (NULL == buff) )
	{
		LOGE("%s, pointer is null!\r\n", __func__);
		return BK_FAIL;
	}

	if ( (0 == max_num) || (max_num > GT1151_POINT_INFO_NUM) )
	{
		LOGE("%s, max_num %d is out range!\r\n", __func__, max_num);
		return BK_FAIL;
	}

	int ret = BK_OK;
	uint8_t temp_status = 0;

	if (BK_OK != gt1151_read_status(cb, &temp_status) )
	{
		LOGE("%s, read tp info fail!\r\n", __func__);
		ret = BK_FAIL;
		goto exit_;
	}

	if (0 == temp_status)
	{
		LOGE("%s, no data!\r\n", __func__);
		ret = BK_FAIL;
		goto exit_;
	}

	if ( (temp_status & 0x80) == 0 )
	{
		LOGE("%s, data is not ready!\r\n", __func__);
		ret = BK_FAIL;
		goto exit_;
	}

	os_memset(read_buff, 0x00, sizeof(read_buff));
	if (BK_OK != SENSOR_I2C_READ(GT1151_POINT1_REG, (uint8_t *)read_buff, sizeof(read_buff)))
	{
		LOGE("%s, read tp info fail!\r\n", __func__);
		ret = BK_FAIL;
		goto exit_;
	}

	// original registers datas.
	LOGD("%s, status=0x%02X, pointer num is %d!\r\n", __func__, temp_status, temp_status&0x0F);
	#if (GT911_REGS_DEBUG_EN > 0)
		bk_mem_dump_ex("tp_gt1151", (unsigned char *)(read_buff), sizeof(read_buff));
	#endif
	
	gt1151_read_point(read_buff, buff, temp_status&0x0F);

exit_:
	if (BK_OK != gt1151_clear_status(cb))
	{
		LOGE("%s, clear status fail!\r\n", __func__);
		ret = BK_FAIL;
	}

	return ret;
}

const tp_sensor_config_t tp_sensor_gt1151 =
{
	.name = "gt1151",
	// default config
	.def_ppi = PPI_800X480,
	.def_int_type = TP_INT_TYPE_FALLING_EDGE,
	.def_refresh_rate = 10,
	.def_tp_num = 5,
	// capability config
	.id = TP_ID_GT1151,
	.address = (GT1151_WRITE_ADDRESS >> 1),
	.detect = gt1151_detect,
	.init = gt1151_init,
	.read_tp_info = gt1151_read_tp_info,
};
