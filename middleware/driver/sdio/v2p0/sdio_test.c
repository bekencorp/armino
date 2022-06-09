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


#include <os/mem.h>
#include "string.h"

#include <common/bk_include.h>

#include "bk_uart.h"

#include "sdio_utils.h"
#include "sdio_slave_driver.h"
#include "sdio_test.h"

#if CONFIG_SDIO_TEST_EN

static bool s_sdio_test_inited = false;
static beken_queue_t s_sdio_test_msg_que = NULL;
static beken_thread_t s_test_sdio_thread = NULL;

static volatile uint8_t s_sdio_test_main_case;
static volatile uint8_t s_sdio_test_sub_case;

static bk_err_t sdio_test_tx_cb(sdio_node_ptr_t head_p, sdio_node_ptr_t tail_p, uint32_t count)
{
	bk_err_t ret = BK_OK;
	sdio_node_ptr_t cur_p = head_p;
	uint32_t i = count, j = 0;

	SDIO_LOG_DEBUG_FUNCTION_ENTRY();

	//just check data
	while(cur_p && i > 0)
	{
		for(j = 0; i < cur_p->len; j++)
		{
			os_printf("%x ", (uint8_t *)cur_p + (sizeof(sdio_node_t)) + i);
			if(i % 16 == 0)
				os_printf("\r\n");
		}

		cur_p = cur_p->next;
		i--;
	}

	//TODO:release buffer
	//WARNING: Application should should use self CHAN_ID and CHAN_DIRECT.
	bk_sdio_chan_push_free_list(SDIO_CHAN_PLATFORM, SDIO_CHAN_TX, head_p, tail_p, count);

	//notify app task finish read

	SDIO_LOG_DEBUG_FUNCTION_EXIT();
	return ret;
}

static bk_err_t sdio_test_tx_init(uint32_t chan_id, uint32_t buf_cnt, uint32_t buf_size)
{
	bk_err_t ret = BK_OK;

	SDIO_LOG_DEBUG_FUNCTION_ENTRY();

	ret = bk_sdio_init_channel(chan_id, SDIO_CHAN_TX, buf_cnt, buf_size);
	if(ret != BK_OK)
	{
		SDIO_LOG_ERR("init channel");
		return ret;
	}
	bk_sdio_register_chan_cb(chan_id, SDIO_CHAN_TX, (sdio_chan_cb_t)sdio_test_tx_cb);

	SDIO_LOG_DEBUG_FUNCTION_EXIT();
	return ret;
}

static bk_err_t sdio_test_tx_sub_case(SDIO_TEST_MSG_T *msg_p)
{
	bk_err_t ret = BK_OK;

	SDIO_LOG_DEBUG_FUNCTION_ENTRY();

	switch(msg_p->sub_case)
	{
		//chan id, buf_cnt, buf_size
		case SDIO_TEST_TX_INIT:
		{
			sdio_chan_id_t id  = msg_p->param1;
			uint32_t buf_cnt = msg_p->param2;
			uint32_t buf_size = msg_p->param3;
		
			ret = sdio_test_tx_init(id, buf_cnt, buf_size);
			break;
		}

		//len, value
		case SDIO_TEST_TX_SINGLE_PACKET:
		{
			sdio_node_ptr_t buf_p = NULL;
			uint32_t len = msg_p->param1;
			uint8_t value = (uint8_t)msg_p->param2;
			uint32_t size;
			
			ret = bk_sdio_chan_pop_free_node(SDIO_CHAN_PLATFORM, SDIO_CHAN_TX, &buf_p, &size);
			if(buf_p)
			{
				//fill data
				if(len > size)
				{
					len = size;
					SDIO_LOG_ERR("max packet size=%d, len=%d", size, len);
				}

				memset((uint8_t *)buf_p + sizeof(sdio_node_t), value, len);
				buf_p->len = len;
				
				//request write
				ret = bk_sdio_slave_sync_write(SDIO_CHAN_PLATFORM, buf_p, NULL, 1);
			}
			else
			{
				SDIO_LOG_ERR("no free buff");
				ret = BK_FAIL;
			}

			break;
		}
	
		case SDIO_TEST_TX_MANY_SINGLE_PACKETS:
		{
			break;
		}
	
		case SDIO_TEST_TX_MULTI_PACKETS:
		{
			break;
		}
	
		default:
			break;
	}

	SDIO_LOG_DEBUG_FUNCTION_EXIT();

	return ret;
}

static bk_err_t sdio_test_rx_cb(sdio_node_ptr_t head_p, sdio_node_ptr_t tail_p, uint32_t count)
{
	bk_err_t ret = BK_OK;
	sdio_node_ptr_t cur_p = head_p;
	uint32_t i = count, j = 0;

	SDIO_LOG_DEBUG_FUNCTION_ENTRY();

	//deal data
	while(cur_p && i > 0)
	{
		for(j = 0; i < cur_p->len; j++)
		{
			os_printf("%x ", (uint8_t *)cur_p + (sizeof(sdio_node_t)) + i);
			if(i % 16 == 0)
				os_printf("\r\n");
		}

		cur_p = cur_p->next;
		i--;
	}

	//TODO:release buffer to free list mem-pool
	//WARNING: Application should should use self CHAN_ID and CHAN_DIRECT.
	ret = bk_sdio_chan_push_free_list(SDIO_CHAN_PLATFORM, SDIO_CHAN_RX, head_p, tail_p, count);

	SDIO_LOG_DEBUG_FUNCTION_EXIT();
	return ret;
}

static bk_err_t sdio_test_rx_init(uint32_t chan_id, uint32_t buf_cnt, uint32_t buf_size)
{
	bk_err_t ret = BK_OK;

	SDIO_LOG_DEBUG_FUNCTION_ENTRY();

	ret = bk_sdio_init_channel(chan_id, SDIO_CHAN_RX, buf_cnt, buf_size);
	if(ret != BK_OK)
	{
		SDIO_LOG_ERR("init channel");
		return ret;
	}

	ret = bk_sdio_register_chan_cb(chan_id, SDIO_CHAN_RX, (sdio_chan_cb_t)sdio_test_rx_cb);

	SDIO_LOG_DEBUG_FUNCTION_EXIT();
	return ret;
}

static bk_err_t sdio_test_rx_sub_case(SDIO_TEST_MSG_T *msg_p)
{
	bk_err_t ret = BK_OK;

	SDIO_LOG_DEBUG_FUNCTION_ENTRY();

	switch(msg_p->sub_case)
	{
		//chan id, buf_cnt, buf_size
		case SDIO_TEST_RX_INIT:
		{
			sdio_chan_id_t id  = msg_p->param1;
			uint32_t buf_cnt = msg_p->param2;
			uint32_t buf_size = msg_p->param3;
		
			ret = sdio_test_rx_init(id, buf_cnt, buf_size);
			break;
		}
		
		//len, value
		case SDIO_TEST_RX_SINGLE_PACKET:
		{
			sdio_node_ptr_t buf_p;
			uint32_t len = msg_p->param1;
			uint8_t value = (uint8_t)msg_p->param2;
			uint32_t size = 0;

			ret = bk_sdio_chan_pop_free_node(SDIO_CHAN_PLATFORM, SDIO_CHAN_RX, &buf_p, &size);
			if(buf_p)
			{
				//fill dirty data
				memset((uint8_t *)buf_p + sizeof(sdio_node_t), value, len);
				buf_p->len = len;
				
				//request read
				ret = bk_sdio_slave_sync_read(SDIO_CHAN_PLATFORM, buf_p, NULL, 1);
			}
			else
			{
				SDIO_LOG_ERR("no free buff");
				ret = BK_FAIL;
			}

			break;
		}
	
		case SDIO_TEST_RX_MANY_SINGLE_PACKETS:
		{
			break;
		}
	
		case SDIO_TEST_RX_MULTI_PACKETS:
		{
			break;
		}
	
		default:
			break;
	}

	SDIO_LOG_DEBUG_FUNCTION_EXIT();
	return ret;
}

static bk_err_t sdio_test_send_msg(SDIO_TEST_MSG_T *msg_p)
{
	bk_err_t ret = BK_FAIL;

	SDIO_LOG_DEBUG_FUNCTION_ENTRY();

	if (s_sdio_test_msg_que)
	{
		ret = rtos_push_to_queue(&s_sdio_test_msg_que, msg_p, BEKEN_NO_WAIT);
		if (kNoErr != ret)
			SDIO_LOG_ERR("sdio test send msg failed\r\n");
	}

	SDIO_LOG_DEBUG_FUNCTION_EXIT();
	return ret;
}

/**
 * @brief	Set the sdio test case.
 *
 * This API set the sdio driver test case.
 *	 - It will tested with the selected case.
 *	 - Every case param has itself mean.
 *
 * @attention 1. This API should be called after sdio test inited.
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t bk_sdio_set_test_case(
							uint8_t main_case,
							uint8_t sub_case,
							uint32_t param1,
							uint32_t param2,
							uint32_t param3,
							uint32_t param4
							)
{
	SDIO_TEST_MSG_T msg;

	if(main_case >= SDIO_TEST_MAX_MAIN_CASE)
	{
		SDIO_LOG_ERR("main case id %d is big then %d", main_case, SDIO_TEST_MAX_MAIN_CASE);
		return BK_FAIL;
	}

	msg.main_case = main_case;
	msg.sub_case = sub_case;
	msg.param1 = param1;
	msg.param2 = param2;
	msg.param3 = param3;
	msg.param4 = param4;

	return sdio_test_send_msg(&msg);
}

static void sdio_test_thread(void *arg)
{
	bk_err_t ret = BK_OK;

	SDIO_LOG_DEBUG_FUNCTION_ENTRY();

	while(1)
	{
		SDIO_TEST_MSG_T msg;

		ret = rtos_pop_from_queue(&s_sdio_test_msg_que, &msg, BEKEN_WAIT_FOREVER);
		if (kNoErr == ret)
		{
			//
			switch(msg.main_case)
			{
				case SDIO_TEST_TX:
				{
					if(msg.sub_case >= SDIO_TEST_TX_MAX_SUB_CASE)
					{
						SDIO_LOG_ERR("sub case id %d is big then %d", msg.sub_case, SDIO_TEST_TX_MAX_SUB_CASE);
						break;
					}
					
					sdio_test_tx_sub_case(&msg);
					break;
				}

				case SDIO_TEST_RX:
				{
					if(msg.sub_case >= SDIO_TEST_RX_MAX_SUB_CASE)
					{
						SDIO_LOG_ERR("sub case id %d is big then %d", msg.sub_case, SDIO_TEST_RX_MAX_SUB_CASE);
						break;
					}
					
					sdio_test_rx_sub_case(&msg);
					break;
				}

				default:
					break;
			}
		}
	}

	SDIO_LOG_DEBUG_FUNCTION_EXIT();

	//TODO: Assert
}

/**
 * @brief	Init the sdio driver test program.
 *
 * This API init the sdio driver test program.
 *	 - It will create a task do selected test case.
 *	 - It can select test case by command line.
 *
 * @attention 1. This API should be called after sdio inited finish.
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t bk_sdio_test_init(void)
{
	bk_err_t ret = BK_OK;

	SDIO_LOG_DEBUG_FUNCTION_ENTRY();

	if(s_sdio_test_inited)
	{
		SDIO_LOG_INFO("has inited");
		return ret;
	}

	ret = rtos_init_queue(
							&s_sdio_test_msg_que,
							SDIO_TEST_MSG_QUEUE_NAME,
							sizeof(SDIO_TEST_MSG_T),
							SDIO_TEST_MSG_QUEUE_COUNT
						);
	if (kNoErr != ret)
	{
		SDIO_LOG_ERR("init queue ret=%d", ret);
		goto err_exit;
	}

	ret = rtos_create_thread(
			&s_test_sdio_thread,
			SDIO_TEST_PRIORITY,
			SDIO_TEST_THREAD_NAME,
			sdio_test_thread,
			SDIO_TEST_THREAD_STACK_SIZE,
			NULL
		);
	if (kNoErr != ret)
	{
		SDIO_LOG_ERR("init thread ret=%d", ret);
		goto err_exit;
	}

	s_sdio_test_inited = true;

	SDIO_LOG_DEBUG_FUNCTION_EXIT();

	//return, or err_exit release resources caused bug.
	return ret;

err_exit:
	if(s_sdio_test_msg_que)
	{
		rtos_deinit_queue(&s_sdio_test_msg_que);
		s_sdio_test_msg_que = NULL;
	}

	if(s_test_sdio_thread)
	{
		rtos_delete_thread(&s_test_sdio_thread);
		s_test_sdio_thread = NULL;
	}

	return ret;
}

/**
 * @brief	Deinit the sdio driver test program.
 *
 * This API init the sdio driver test program.
 *	 - It will delete the test thread and queue.
 *
 * @attention 1. This API should be called after sdio inited finish.
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t bk_sdio_test_deinit(void)
{
	bk_err_t ret = BK_OK;

	SDIO_LOG_DEBUG_FUNCTION_ENTRY();

	if(s_sdio_test_msg_que)
	{
		ret = rtos_deinit_queue(&s_sdio_test_msg_que);
		if (BK_OK != ret)
		{
			SDIO_LOG_ERR("deinit queue ret=%d", ret);
			return ret;
		}

		s_sdio_test_msg_que = NULL;
	}

	if(s_test_sdio_thread)
	{
		ret = rtos_delete_thread(&s_test_sdio_thread);
		if (BK_OK != ret)
		{
			SDIO_LOG_ERR("delete thread ret=%d", ret);
			return ret;
		}

		s_test_sdio_thread = NULL;
	}

	SDIO_LOG_DEBUG_FUNCTION_EXIT();

	return BK_OK;
}

#endif //CONFIG_SDIO_TEST_EN
