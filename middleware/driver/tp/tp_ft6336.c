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


#define TAG "ft6336"
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)


// external statement.
extern void bk_mem_dump_ex(const char *title, unsigned char *data, uint32_t data_len);


// macro define
#define FT6336_WRITE_ADDRESS    (0x70)
#define FT6336_READ_ADDRESS     (0x71)
#define FT6336_PRODUCT_ID_CODE  (0x64)

#define FT6336_ADDR_LEN          (1)
#define FT6336_REG_LEN           (1)
#define FT6336_MAX_TOUCH_NUM     (2)
#define FT6336_POINT_INFO_NUM    (TP_SUPPORT_MAX_NUM)
#define FT6336_POINT_INFO_SIZE   (6)
#define FT6336_POINT_INFO_TOTAL_SIZE  (FT6336_POINT_INFO_NUM * FT6336_POINT_INFO_SIZE)

#define FT6336_DEV_MODE_REG      (0x00)
#define FT6336_STATUS            (0x02)
#define FT6336_POINT1_REG        (0x03)
#define FT6336_POINT2_REG        (0x09)
#define FT6336_TH_GROUP_REG      (0x80)
#define FT6336_PERIODACTIVE_REG  (0x88)
#define FT6336_LIB_VER_H_REG     (0xA1)
#define FT6336_LIB_VER_L_REG     (0xA2)
#define FT6336_CIPHER_REG        (0xA3)
#define FT6336_G_MODE_REG        (0xA4)

#define FT6336_REGS_DEBUG_EN (0)

#define SENSOR_I2C_READ(reg, buff, len)  cb->read_uint8((FT6336_WRITE_ADDRESS >> 1), reg, buff, len)
#define SENSOR_I2C_WRITE(reg, buff, len)  cb->write_uint8((FT6336_WRITE_ADDRESS >> 1), reg, buff, len)


bool ft6336_detect(const tp_i2c_callback_t *cb)
{
	if (NULL == cb)
	{
		LOGE("%s, pointer is null!\r\n", __func__);
		return false;
	}

	uint8_t product_id = 0;

	if (BK_OK != SENSOR_I2C_READ(FT6336_CIPHER_REG, (uint8_t *)(&product_id), sizeof(product_id)))
	{
		LOGE("%s, read product id reg fail!\r\n", __func__);
		return false;
	}

	LOGI("%s, product id: 0X%02X\r\n", __func__, product_id);

	if (FT6336_PRODUCT_ID_CODE == product_id)
	{
		LOGI("%s success\n", __func__);
		return true;
	}

	return false;
}

int ft6336_init(const tp_i2c_callback_t *cb, tp_sensor_user_config_t *config)
{
	if ( (NULL == cb) || (NULL == config) )
	{
		LOGE("%s, pointer is null!\r\n", __func__);
		return BK_FAIL;
	}

	int ret = BK_OK;
	uint8_t temp;

	temp = 0x00;
	if (BK_OK != SENSOR_I2C_WRITE(FT6336_DEV_MODE_REG, (uint8_t *)&temp, sizeof(temp)))
	{
		LOGE("%s, write device mode config regs fail!\r\n", __func__);
		return BK_FAIL;
	}

	temp = 0x01;
	if (BK_OK != SENSOR_I2C_WRITE(FT6336_G_MODE_REG, (uint8_t *)&temp, sizeof(temp)))
	{
		LOGE("%s, write G mode config regs fail!\r\n", __func__);
		return BK_FAIL;
	}

	return ret;
}

int ft6336_read_status(const tp_i2c_callback_t *cb, uint8_t *status)
{
	if ( (NULL == cb) || (NULL == status) )
	{
		LOGE("%s, pointer is null!\r\n", __func__);
		return BK_FAIL;
	}

	if (BK_OK != SENSOR_I2C_READ(FT6336_STATUS, (uint8_t *)(status), 1))
	{
		LOGE("%s, read status reg fail!\r\n", __func__);
		return BK_FAIL;
	}

	LOGD("%s, status=0x%02X\r\n", __func__, *status);

	return BK_OK;
}

// ft6336 get tp info.
void ft6336_read_point(uint8_t *input_buff, void *buf, uint8_t num)
{
	uint8_t touch_num = num;
	uint8_t *read_buf = input_buff;
	tp_data_t *read_data = (tp_data_t *)buf;
	uint8_t read_index;
	uint8_t event_flag;
	uint8_t read_id;
	uint16_t input_x;
	uint16_t input_y;
	uint16_t input_w;
	uint8_t off_set;
	
	for (read_index = 0; read_index < touch_num; read_index++)
	{
		off_set = read_index * FT6336_POINT_INFO_SIZE;
		event_flag = (read_buf[off_set] >> 6) & 0x03;

		if (event_flag != 0x03)
		{
			read_id = (read_buf[off_set + 2] >> 4) & 0x0F;

			if (read_id >= FT6336_MAX_TOUCH_NUM)
			{
				LOGE("%s, touch ID %d is out range!\r\n", __func__, read_id);
				return;
			}
			
			input_x = ((read_buf[off_set] & 0x0F) << 8) | read_buf[off_set + 1];			/* x */
			input_y = ((read_buf[off_set + 2] & 0x0F) << 8) | (read_buf[off_set + 3]);		/* y */
			input_w = (read_buf[off_set + 4] << 8) | read_buf[off_set + 5]; 				/* weight | area */

			if (event_flag == 0x00)
			{
				read_data[read_id].event = TP_EVENT_TYPE_DOWN;
			}
			else if (event_flag == 0x01)
			{
				read_data[read_id].event = TP_EVENT_TYPE_UP;
			}
			else if (event_flag == 0x02)
			{
				read_data[read_id].event = TP_EVENT_TYPE_MOVE;
			}
			read_data[read_id].timestamp = rtos_get_time();
			read_data[read_id].width = input_w;
			read_data[read_id].x_coordinate = input_x;
			read_data[read_id].y_coordinate = input_y;
			read_data[read_id].track_id = read_id;
		}
	}
}

static uint8_t read_buff[FT6336_POINT_INFO_TOTAL_SIZE];
int ft6336_read_tp_info(const tp_i2c_callback_t *cb, uint8_t max_num, uint8_t *buff)
{
	if ( (NULL == cb) || (NULL == buff) )
	{
		LOGE("%s, pointer is null!\r\n", __func__);
		return BK_FAIL;
	}

	if ( (0 == max_num) || (max_num > FT6336_POINT_INFO_NUM) )
	{
		LOGE("%s, max_num %d is out range!\r\n", __func__, max_num);
		return BK_FAIL;
	}

	int ret = BK_OK;
	uint8_t temp_status = 0;
	uint8_t read_num = 0;

	if (BK_OK != ft6336_read_status(cb, &temp_status) )
	{
		LOGE("%s, read tp info fail!\r\n", __func__);
		ret = BK_FAIL;
		goto exit_;
	}

	os_memset(read_buff, 0x00, sizeof(read_buff));
	if (BK_OK != SENSOR_I2C_READ(FT6336_POINT1_REG, (uint8_t *)read_buff, sizeof(read_buff)))
	{
		LOGE("%s, read tp info fail!\r\n", __func__);
		ret = BK_FAIL;
		goto exit_;
	}

	// original registers datas.
	LOGD("%s, status=0x%02X, pointer num is %d!\r\n", __func__, temp_status, temp_status&0x0F);
	#if (FT6336_REGS_DEBUG_EN > 0)
		bk_mem_dump_ex("tp_ft6336", (unsigned char *)(read_buff), sizeof(read_buff));
	#endif

	read_num = FT6336_POINT_INFO_NUM;
	
	LOGD("%s, read num is %d!\r\n", __func__, read_num);
	ft6336_read_point(read_buff, buff, read_num);

exit_:

	return ret;
}

const tp_sensor_config_t tp_sensor_ft6336 =
{
	.name = "ft6336",
	// default config
	.def_ppi = PPI_170X320,
	.def_int_type = TP_INT_TYPE_FALLING_EDGE,
	.def_refresh_rate = 10,
	.def_tp_num = 2,
	// capability config
	.id = TP_ID_FT6336,
	.address = (FT6336_WRITE_ADDRESS >> 1),
	.detect = ft6336_detect,
	.init = ft6336_init,
	.read_tp_info = ft6336_read_tp_info,
};
