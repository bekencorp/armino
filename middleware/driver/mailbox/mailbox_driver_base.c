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
#include "mailbox_driver_base.h"
#include "mailbox_driver.h"
#include "icu_driver.h"


static const mailbox_config_t mailbox_cfg_map_table[] = MAILBAOX_CONFIG_INFO_TABLE;

static mailbox_info_t s_mailbox[ARRAY_SIZE(mailbox_cfg_map_table)] = {NULL};
static bool s_mailbox_is_init = false;

#define MAILBOX_RETURN_ON_NOT_INIT() do {\
		if (!s_mailbox_is_init) {\
			return BK_ERR_MAILBOX_NOT_INIT;\
		}\
	} while(0)

#define MAILBOX_RETURN_ON_REPEAT_INIT() do {\
		if (s_mailbox_is_init) {\
			return BK_ERR_MAILBOX_REPEAT_INIT;\
		}\
	} while(0)

static void mailbox_config(void);
static mailbox_id_t mailbox_check_src_dst(mailbox_endpoint_t src, mailbox_endpoint_t dst);
static void mailbox_write_data(mailbox_data_t *data, mailbox_id_t id, mailbox_box_num_t box);
static void mailbox_receive_data(mailbox_id_t id, mailbox_data_t *data, mailbox_box_num_t box);
static void mailbox_receive_isr(mailbox_id_t id);
static void mailbox_service0_isr(void);
static void mailbox_service1_isr(void);
static void mailbox_interrupt_service(mailbox_info_t *mailbox);


bk_err_t bk_mailbox_init(void)
{
	MAILBOX_LOGD("Begin %s\n", __func__);
	MAILBOX_RETURN_ON_REPEAT_INIT();
	s_mailbox_is_init = true;

	mailbox_config();

	for(int index = 0; index < ARRAY_SIZE(mailbox_cfg_map_table); index++)
	{
		if (INT_SRC_NONE == s_mailbox[index].dir.icu_src)
			continue;
		bk_int_isr_register((s_mailbox[index].dir.icu_src),
							(int_group_isr_t)(s_mailbox[index].imp.mailbox_isr), NULL);

		mailbox_interrupt_enable(s_mailbox[index].dir.src);

	}


	MAILBOX_LOGD("End %s\n", __func__);
	return BK_OK;
}

bk_err_t bk_mailbox_deinit(void)
{
	MAILBOX_LOGD("Begin %s\n", __func__);
	MAILBOX_RETURN_ON_NOT_INIT();

	for(int index = 0; index < ARRAY_SIZE(mailbox_cfg_map_table); index++) {
		mailbox_hal_box_init(&(s_mailbox[index].hal));

		if (!(int_group_isr_t)(s_mailbox[index].imp.mailbox_isr))
			continue;
		bk_int_isr_unregister(s_mailbox[index].dir.icu_src);

		mailbox_interrupt_disable(s_mailbox[index].dir.src);
	}

	s_mailbox_is_init = false;
	MAILBOX_LOGD("End %s\n", __func__);
	return BK_OK;
}

static void mailbox_config(void)
{
	uint32_t index;
	uint32_t m_id;

	MAILBOX_LOGI("%s\n", __func__);

	for (index = 0; index < ARRAY_SIZE(mailbox_cfg_map_table); index++) {
		m_id = mailbox_cfg_map_table[index].id;
		s_mailbox[m_id].hal.id = m_id;
		s_mailbox[m_id].dir = mailbox_cfg_map_table[index];
		mailbox_interrupt_service(&s_mailbox[m_id]);

		if (mailbox_hal_addr_init(&(s_mailbox[m_id].hal)))
			MAILBOX_LOGI("%s addr is error!\n", __func__);

		MAILBOX_LOGD("%s addr is %x!\n", __func__, s_mailbox[m_id].hal);
		mailbox_hal_set_identity(&(s_mailbox[m_id].hal));
		mailbox_hal_box_init(&(s_mailbox[m_id].hal));
	}
}

bk_err_t bk_mailbox_set_param(mailbox_data_t *data, uint32_t p0, uint32_t p1, uint32_t p2, uint32_t p3)
{
	MAILBOX_RETURN_ON_NOT_INIT();

    data->param0 = p0;
    data->param1 = p1;
    data->param2 = p2;
    data->param3 = p3;

	return BK_OK;
}

static mailbox_id_t mailbox_check_src_dst(mailbox_endpoint_t src, mailbox_endpoint_t dst)
{
	for(int index = 0; index < ARRAY_SIZE(mailbox_cfg_map_table); index++)
	{
		if (s_mailbox[index].dir.src != src)
			continue;
		if (s_mailbox[index].dir.dst != dst)
			continue;
		return s_mailbox[index].hal.id;
	}

	MAILBOX_LOGD("%s is ERROR!\n", __func__);
	return BK_ERR_MAILBOX_SRC_DST;
}

static void mailbox_write_data(mailbox_data_t *data, mailbox_id_t id, mailbox_box_num_t box)
{
	mailbox_hal_write_param0(&(s_mailbox[id].hal), data->param0, box);
	mailbox_hal_write_param1(&(s_mailbox[id].hal), data->param1, box);
	mailbox_hal_write_param2(&(s_mailbox[id].hal), data->param2, box);
	mailbox_hal_write_param3(&(s_mailbox[id].hal), data->param3, box);
	MAILBOX_LOGD("[MAILBOX_SEND_PARAM]: (%08x, 0x%08x, %d, %d)\r\n", data->param0,
				data->param1, data->param2, data->param3);
}

bk_err_t bk_mailbox_send(mailbox_data_t *data, mailbox_endpoint_t src, mailbox_endpoint_t dst, void *arg)
{
	MAILBOX_RETURN_ON_NOT_INIT();

	mailbox_id_t id;
	mailbox_box_num_t box;

	MAILBOX_LOGD("%s\n", __func__);

	id = mailbox_check_src_dst(src, dst);
	if (id >= ARRAY_SIZE(mailbox_cfg_map_table))
		return BK_ERR_MAILBOX_SRC_DST;

	MAILBOX_LOGD("mailbox send to %d\n", dst);
	MAILBOX_LOGD("%s addr is %x! id: %d\n", __func__, (s_mailbox[id].hal), id);

	if(arg == NULL)
	{
		int wait_count = 0;

		while(wait_count < MAILBOX_SEND_WAIT_COUNT)
		{
			box = mailbox_hal_check_which_box_ready(&(s_mailbox[id].hal));

			if (box != MAILBOX_NONE)
			{
				break;
			}
			wait_count++;
		}

		if (box == MAILBOX_NONE) // mailbox busy
			return BK_ERR_MAILBOX_TIMEOUT;
	}
	else
	{
		box = (*(uint16_t *)arg) & 0xFF;

		if ((box != MAILBOX_BOX0) && (box != MAILBOX_BOX1))
		{
			return BK_ERR_MAILBOX_ID;
		}

		if(mailbox_hal_read_box_ready(&(s_mailbox[id].hal), box) != 0)
		{
			return BK_ERR_MAILBOX_TIMEOUT;  // mailbox busy
		}
	}

	MAILBOX_LOGD("BOX: %x param0: %x, param1: %x, param2: %x, param3: %x\r\n",
			      box, data->param0, data->param1, data->param2, data->param3);

	mailbox_hal_box_clear_ready(&(s_mailbox[id].hal), box);
	mailbox_write_data(data, id, box);
	mailbox_hal_box_trigger(&(s_mailbox[id].hal), box);

	return BK_OK;

}

static void mailbox_receive_data(mailbox_id_t id, mailbox_data_t *data, mailbox_box_num_t box)
{
	data->param0 = mailbox_hal_read_param0(&(s_mailbox[id].hal), box);
	data->param1 = mailbox_hal_read_param1(&(s_mailbox[id].hal), box);
	data->param2 = mailbox_hal_read_param2(&(s_mailbox[id].hal), box);
	data->param3 = mailbox_hal_read_param3(&(s_mailbox[id].hal), box);
	MAILBOX_LOGD("[MAILBOX_RECEIVE_PARAM]: (%08x, 0x%08x, %d, %d)\r\n", data->param0,
				data->param1, data->param2, data->param3);

}

static void mailbox_receive_isr(mailbox_id_t id)
{
	mailbox_data_t data;
	mailbox_box_num_t box;

	MAILBOX_LOGD("%s addr is %x! id: %d\n", __func__, (s_mailbox[id].hal), id);

	box = mailbox_hal_check_which_box_trigger(&(s_mailbox[id].hal));
	if (box == MAILBOX_NONE) {
		MAILBOX_LOGD("%s BOX ERROR\n", __func__);
		return;
	}

	mailbox_receive_data(id, &data, box);

	mailbox_hal_box_clear(&(s_mailbox[id].hal), box);

	for (int num = 0; num < MAILBOX_CALLBACK_NUMBER; num++)
	{	if (s_mailbox[id].imp.rx[num] != NULL)
			(s_mailbox[id].imp.rx[num])(&data);
	}

	MAILBOX_LOGD("[RECIVE_R]: (%08x, 0x%08x, %d, %d)\r\n", data.param0,
				data.param1, data.param2, data.param3);
}

 bk_err_t bk_mailbox_recv_callback_register(mailbox_endpoint_t src, mailbox_endpoint_t dst, mailbox_callback_t callback)
{
	MAILBOX_RETURN_ON_NOT_INIT();

	mailbox_id_t id;

	MAILBOX_LOGI("%s\n", __func__);

	id = mailbox_check_src_dst(src, dst);
	if (id >= ARRAY_SIZE(mailbox_cfg_map_table))
		return BK_ERR_MAILBOX_SRC_DST;

	for (int num = 0; num < MAILBOX_CALLBACK_NUMBER; num++)
	{
		if ((NULL == s_mailbox[id].imp.rx[num]) || (callback == s_mailbox[id].imp.rx[num])) {
			s_mailbox[id].imp.rx[num] = callback;
			return BK_OK;
		}
	}

	MAILBOX_LOGI("%s mailbox_cb is full\n", __func__);
	return BK_ERR_MAILBOX_CALLBACK;
}

bk_err_t bk_mailbox_recv_callback_unregister(mailbox_endpoint_t src, mailbox_endpoint_t dst)
{
	MAILBOX_RETURN_ON_NOT_INIT();

	mailbox_id_t id;

	MAILBOX_LOGI("%s\n", __func__);

	id = mailbox_check_src_dst(src, dst);
	if (id >= ARRAY_SIZE(mailbox_cfg_map_table))
		return BK_ERR_MAILBOX_SRC_DST;

	for (int num = 0; num < MAILBOX_CALLBACK_NUMBER; num++)
	{	if (s_mailbox[id].imp.rx[num] != NULL) {
			s_mailbox[id].imp.rx[num] = NULL;
			return BK_OK;
		}
	}

	MAILBOX_LOGI("%s 0x%x not found\n", __func__);

	return BK_ERR_MAILBOX_CALLBACK;
}

static void mailbox_service0_isr(void)
{
	MAILBOX_LOGD("%s INTO\n", __func__);

	mailbox_id_t id = 0;

	for(int index = 0; index < ARRAY_SIZE(mailbox_cfg_map_table); index++)
	{
		if (s_mailbox[index].dir.icu_src != INT_SRC_MAILBOX0)
			continue;
		id = s_mailbox[index].dir.id;
	}

	mailbox_receive_isr(id);
	MAILBOX_LOGD("%s OUT\n", __func__);
}

static void mailbox_service1_isr(void)
{
	MAILBOX_LOGD("%s INTO\n", __func__);

	mailbox_id_t id = 0;

	for(int index = 0; index < ARRAY_SIZE(mailbox_cfg_map_table); index++)
	{
		if (s_mailbox[index].dir.icu_src != INT_SRC_MAILBOX1)
			continue;
		id = s_mailbox[index].dir.id;
	}
	mailbox_receive_isr(id);
	MAILBOX_LOGD("%s OUT\n", __func__);
}

static void mailbox_interrupt_service(mailbox_info_t *mailbox)
{
	switch (mailbox->dir.icu_src){
		case INT_SRC_MAILBOX0:
			mailbox->imp.mailbox_isr = mailbox_service0_isr;
			break;
		case INT_SRC_MAILBOX1:
			mailbox->imp.mailbox_isr = mailbox_service1_isr;
			break;
		default:
			break;
	}
}

//TWO function for other projects
void mailbox_driver_init(void)
{
	MAILBOX_LOGD("%s mailbox driver INIT!!!\n", __func__);
	bk_mailbox_init();

}
void mailbox_driver_deinit(void)
{
	bk_mailbox_deinit();
}

