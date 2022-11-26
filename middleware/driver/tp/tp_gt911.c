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


// macro define
#define GT911_WRITE_ADDRESS    (0x28) //(0XBA)
#define GT911_READ_ADDRESS     (0x29) //(0XBB)
#define GT911_PRODUCT_ID_CODE  (0x00313139)  // "911" ASCII code

#define GT911_ADDR_LEN          (1)
#define GT911_REG_LEN           (2)
#define GT911_MAX_TOUCH_NUM     (5)
#define GT911_POINT_INFO_NUM    (5)
#define GT911_POINT_INFO_SIZE   (8)
#define GT911_POINT_INFO_TOTAL_SIZE  (GT911_POINT_INFO_NUM * GT911_POINT_INFO_SIZE)

#define GT911_COMMAND_REG       (0x8040)

#define GT911_CONFIG_REG        (0x8047)

#define GT911_PRODUCT_ID        (0x8140)
#define GT911_FIRMWARE_VERSION  (0x8144)
#define GT911_X_RESOLUTION      (0x8146)
#define GT911_Y_RESOLUTION      (0x8148)
#define GT911_VENDOR_ID         (0x814A)

#define GT911_STATUS            (0x814E)

#define GT911_POINT1_REG        (0x814F)
#define GT911_POINT2_REG        (0x8157)
#define GT911_POINT3_REG        (0x815F)
#define GT911_POINT4_REG        (0x8167)
#define GT911_POINT5_REG        (0x816F)

#define GT911_CHECK_SUM         (0x80FF)

// config parameters and relative position or range. 
#define GT911_X_OUTPUT_MAX_POS (1)
#define GT911_Y_OUTPUT_MAX_POS (3)
#define GT911_TOUCH_NUMBER_POS (5)
#define GT911_TOUCH_NUMBER_MIN (1)
#define GT911_TOUCH_NUMBER_MAX (5)
#define GT911_MODULE_SWITCH1_POS (6)
#define GT911_REFRESH_RATE_POS (15)
#define GT911_REFRESH_RATE_MIN (5)
#define GT911_REFRESH_RATE_MAX (20)


#define TAG "gt911"
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)


#define SENSOR_I2C_READ(reg, buff, len)  cb->read_uint16((GT911_WRITE_ADDRESS >> 1), reg, buff, len)
#define SENSOR_I2C_WRITE(reg, buff, len)  cb->write_uint16((GT911_WRITE_ADDRESS >> 1), reg, buff, len)


// gt911
const uint8_t sensor_gt911_cfg_table[] =
{
	0X00,0X20,0X03,0XE0,0X01,0X05,0X3D,0X00,0X02,0X08,
	0X1E,0X08,0X50,0X3C,0X0F,0X05,0X00,0X00,0XFF,0X67,
	0X50,0X00,0X00,0X18,0X1A,0X1E,0X14,0X89,0X28,0X0A,
	0X30,0X2E,0XBB,0X0A,0X03,0X00,0X00,0X02,0X33,0X1D,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X32,0X00,0X00,
	0X2A,0X1C,0X5A,0X94,0XC5,0X02,0X07,0X00,0X00,0X00,
	0XB5,0X1F,0X00,0X90,0X28,0X00,0X77,0X32,0X00,0X62,
	0X3F,0X00,0X52,0X50,0X00,0X52,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0F,
	0X0F,0X03,0X06,0X10,0X42,0XF8,0X0F,0X14,0X00,0X00,
	0X00,0X00,0X1A,0X18,0X16,0X14,0X12,0X10,0X0E,0X0C,
	0X0A,0X08,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X29,0X28,0X24,0X22,0X20,0X1F,0X1E,0X1D,
	0X0E,0X0C,0X0A,0X08,0X06,0X05,0X04,0X02,0X00,0XFF,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
	0XFF,0XFF,0XFF,0XFF
};


bool gt911_detect(const tp_i2c_callback_t *cb)
{
	if (NULL == cb)
	{
		LOGE("%s, pointer is null!\r\n", __func__);
		return false;
	}

	uint32_t product_id = 0;

	if (BK_OK != SENSOR_I2C_READ(GT911_PRODUCT_ID, (uint8_t *)(&product_id), sizeof(product_id)))
	{
		LOGE("%s, read product id reg fail!\r\n", __func__);
		return false;
	}

	LOGI("%s, product id: 0X%08X\r\n", __func__, product_id);

	if (GT911_PRODUCT_ID_CODE == product_id)
	{
		LOGI("%s success\n", __func__);
		return true;
	}

	return false;
}

static int gt911_update_config(const tp_i2c_callback_t *cb, uint8_t *config, uint8_t size)
{
	uint8_t buff[2];

	buff[0] = 0;
	buff[1] = 0;
	for (uint8_t temp=0; temp<size; ++temp)
	{
		// checksum
		buff[0] += config[temp];
	}
	buff[0] = (~buff[0]) + 1;

	if (BK_OK != SENSOR_I2C_WRITE(GT911_CONFIG_REG, (uint8_t *)config, size))
	{
		LOGE("%s, write config regs fail!\r\n", __func__);
		return BK_FAIL;
	}

	if (BK_OK != SENSOR_I2C_WRITE(GT911_CHECK_SUM, (uint8_t *)buff, sizeof(buff)))
	{
		LOGE("%s, write checksum reg fail!\r\n", __func__);
		return BK_FAIL;
	}

	return BK_OK;
}

int gt911_init(const tp_i2c_callback_t *cb, tp_sensor_user_config_t *config)
{
	if ( (NULL == cb) || (NULL == config) )
	{
		LOGE("%s, pointer is null!\r\n", __func__);
		return BK_FAIL;
	}

	int ret = BK_OK;
	uint8_t *cfg_table = (uint8_t *)os_malloc(sizeof(sensor_gt911_cfg_table));

	if (NULL == cfg_table)
	{
		LOGE("%s, malloc fail!\r\n", __func__);
		ret = BK_FAIL;
		goto exit_;
	}

	// copy default config table.
	os_memcpy((uint8_t *)cfg_table, (uint8_t *)sensor_gt911_cfg_table, sizeof(sensor_gt911_cfg_table));

	// renew config parameters.
	// x output max.
	os_memcpy((uint8_t *)(&cfg_table[GT911_X_OUTPUT_MAX_POS]), (uint8_t *)(&config->x_size), 2);
	// y output max.
	os_memcpy((uint8_t *)(&cfg_table[GT911_Y_OUTPUT_MAX_POS]), (uint8_t *)(&config->y_size), 2);
	// interrupt trigger type.
	if (TP_INT_TYPE_RISING_EDGE == config->int_type)
	{
		cfg_table[GT911_MODULE_SWITCH1_POS] &= 0xFC;
		cfg_table[GT911_MODULE_SWITCH1_POS] |= 0x00;
	}
	else if (TP_INT_TYPE_FALLING_EDGE == config->int_type)
	{
		cfg_table[GT911_MODULE_SWITCH1_POS] &= 0xFC;
		cfg_table[GT911_MODULE_SWITCH1_POS] |= 0x01;
	}
	else if (TP_INT_TYPE_LOW_LEVEL == config->int_type)
	{
		cfg_table[GT911_MODULE_SWITCH1_POS] &= 0xFC;
		cfg_table[GT911_MODULE_SWITCH1_POS] |= 0x02;
	}	
	else if (TP_INT_TYPE_HIGH_LEVEL == config->int_type)
	{
		cfg_table[GT911_MODULE_SWITCH1_POS] &= 0xFC;
		cfg_table[GT911_MODULE_SWITCH1_POS] |= 0x03;
	}
	// refresh rate.
	if (config->refresh_rate > GT911_REFRESH_RATE_MAX)
	{
		cfg_table[GT911_REFRESH_RATE_POS] = GT911_REFRESH_RATE_MAX;
	}
	else if (config->refresh_rate < GT911_REFRESH_RATE_MIN)
	{
		cfg_table[GT911_REFRESH_RATE_POS] = GT911_REFRESH_RATE_MIN;
	}
	else
	{
		cfg_table[GT911_REFRESH_RATE_POS] = config->refresh_rate - GT911_REFRESH_RATE_MIN;
	}
	// touch number.
	if (config->tp_num > GT911_TOUCH_NUMBER_MAX)
	{
		cfg_table[GT911_TOUCH_NUMBER_POS] = GT911_TOUCH_NUMBER_MAX;
	}
	else if (config->tp_num < GT911_TOUCH_NUMBER_MIN)
	{
		cfg_table[GT911_TOUCH_NUMBER_POS] = GT911_TOUCH_NUMBER_MIN;
	}
	else
	{
		cfg_table[GT911_TOUCH_NUMBER_POS] = config->tp_num;
	}

	ret = gt911_update_config(cb, cfg_table, sizeof(sensor_gt911_cfg_table));
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

int gt911_read_status(const tp_i2c_callback_t *cb, uint8_t *status)
{
	if ( (NULL == cb) || (NULL == status) )
	{
		LOGE("%s, pointer is null!\r\n", __func__);
		return BK_FAIL;
	}

	if (BK_OK != SENSOR_I2C_READ(GT911_STATUS, (uint8_t *)(status), 1))
	{
		LOGE("%s, read status reg fail!\r\n", __func__);
		return BK_FAIL;
	}

	LOGD("%s, status=0x%02X\r\n", __func__, *status);

	return BK_OK;
}

int gt911_clear_status(const tp_i2c_callback_t *cb)
{
	if (NULL == cb)
	{
		LOGE("%s, pointer is null!\r\n", __func__);
		return BK_FAIL;
	}
	
	uint8_t status = 0;
	if (BK_OK != SENSOR_I2C_WRITE(GT911_STATUS, (uint8_t *)(&status), sizeof(status)))
	{
		LOGE("%s, write status reg fail!\r\n", __func__);
		return BK_FAIL;
	}

	return BK_OK;
}

// gt911 get tp info.
static int16_t pre_x[GT911_MAX_TOUCH_NUM] = {-1, -1, -1, -1, -1};
static int16_t pre_y[GT911_MAX_TOUCH_NUM] = {-1, -1, -1, -1, -1};
static int16_t pre_w[GT911_MAX_TOUCH_NUM] = {-1, -1, -1, -1, -1};
static uint8_t s_tp_dowm[GT911_MAX_TOUCH_NUM];

uint32_t gt911_get_time(void)
{
	uint32_t temp = 0;

	beken_time_get_time((beken_time_t *)&temp);

	return temp;
}

void gt911_touch_up(void *buf, int8_t id)
{
    tp_data_t *read_data = (tp_data_t *)buf;

    if(s_tp_dowm[id] == 1)
    {
        s_tp_dowm[id] = 0;
        read_data[id].event = TP_EVENT_TYPE_UP;
    }
    else
    {
        read_data[id].event = TP_EVENT_TYPE_NONE;
    }

    read_data[id].timestamp = gt911_get_time();
    read_data[id].width = pre_w[id];
    read_data[id].x_coordinate = pre_x[id];
    read_data[id].y_coordinate = pre_y[id];
    read_data[id].track_id = id;

    pre_x[id] = -1;  /* last point is none */
    pre_y[id] = -1;
    pre_w[id] = -1;
}

void gt911_touch_down(void *buf, int8_t id, int16_t x, int16_t y, int16_t w)
{
    tp_data_t *read_data = (tp_data_t *)buf;

    if (s_tp_dowm[id] == 1)
    {
        read_data[id].event = TP_EVENT_TYPE_MOVE;

    }
    else
    {
        read_data[id].event = TP_EVENT_TYPE_DOWN;
        s_tp_dowm[id] = 1;
    }

    read_data[id].timestamp = gt911_get_time();
    read_data[id].width = w;
    read_data[id].x_coordinate = x;
    read_data[id].y_coordinate = y;
    read_data[id].track_id = id;

    pre_x[id] = x; /* save last point */
    pre_y[id] = y;
    pre_w[id] = w;
}

void gt911_read_point(uint8_t *input_buff, void *buf, uint8_t num)
{
    uint8_t touch_num = 0;
    uint8_t *read_buf = input_buff;
    uint8_t read_index;
    int8_t read_id = 0;
    int16_t input_x = 0;
    int16_t input_y = 0;
    int16_t input_w = 0;

    static uint8_t pre_touch = 0;
    static int8_t pre_id[GT911_MAX_TOUCH_NUM] = {0};

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
                    gt911_touch_up(buf, up_id);
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

            gt911_touch_down(buf, read_id, input_x, input_y, input_w);
        }
    }
    else if (pre_touch)
    {
        for(read_index = 0; read_index < pre_touch; read_index++)
        {
            gt911_touch_up(buf, pre_id[read_index]);
        }
    }

    pre_touch = touch_num;
}

extern void bk_mem_dump_ex(const char *title, unsigned char *data, uint32_t data_len);

static uint8_t read_buff[GT911_POINT_INFO_TOTAL_SIZE];
int gt911_read_tp_info(const tp_i2c_callback_t *cb, uint8_t max_num, uint8_t *buff)
{
	if ( (NULL == cb) || (NULL == buff) )
	{
		LOGE("%s, pointer is null!\r\n", __func__);
		return BK_FAIL;
	}

	if( (0 == max_num) || (max_num > GT911_POINT_INFO_NUM) )
	{
		LOGE("%s, max_num %d is out range!\r\n", __func__, max_num);
		return BK_FAIL;
	}

	int ret = BK_OK;
	uint8_t temp_status = 0;

	if (BK_OK != gt911_read_status(cb, &temp_status) )
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
	if (BK_OK != SENSOR_I2C_READ(GT911_POINT1_REG, (uint8_t *)read_buff, sizeof(read_buff)))
	{
		LOGE("%s, read tp info fail!\r\n", __func__);
		ret = BK_FAIL;
		goto exit_;
	}

	LOGD("%s, pointer num is %d!\r\n", __func__, temp_status&0x0F);
	//bk_mem_dump_ex("tp_gt911", (unsigned char *)(read_buff), sizeof(read_buff));
	
	gt911_read_point(read_buff, buff, temp_status&0x0F);

exit_:
	if (BK_OK != gt911_clear_status(cb))
	{
		LOGE("%s, clear status fail!\r\n", __func__);
		ret = BK_FAIL;
	}

	return ret;
}

int gt911_reset(const tp_i2c_callback_t *cb, tp_reset_type_t type)
{
	if (NULL == cb)
	{
		LOGE("%s, pointer is null!\r\n", __func__);
		return BK_FAIL;
	}

	uint8_t temp;

	if (type == TP_RESET_TYPE_VALID)
	{
		temp = 2;
		if (BK_OK != SENSOR_I2C_WRITE(GT911_COMMAND_REG, (uint8_t *)(&temp), sizeof(temp)))
		{
			LOGE("%s, write command regs fail!\r\n", __func__);
			return BK_FAIL;
		}
	}
	else if (type == TP_RESET_TYPE_INVALID)
	{
		temp = 0;
		if (BK_OK != SENSOR_I2C_WRITE(GT911_COMMAND_REG, (uint8_t *)(&temp), sizeof(temp)))
		{
			LOGE("%s, write command regs fail!\r\n", __func__);
			return BK_FAIL;
		}
	}
	else
	{
		LOGE("%s, type not exist!\r\n", __func__);
		return BK_FAIL;
	}

	return BK_OK;
}

const tp_sensor_config_t tp_sensor_gt911 =
{
	.name = "gt911",
	// default config
	.def_ppi = PPI_800X480,
	.def_int_type = TP_INT_TYPE_FALLING_EDGE,
	.def_refresh_rate = 10,
	.def_tp_num = 5,
	// capability config
	.id = TP_ID_GT911,
	.address = (GT911_WRITE_ADDRESS >> 1),
	.detect = gt911_detect,
	.init = gt911_init,
	.read_tp_info = gt911_read_tp_info,
};

