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
#include <driver/int.h>
#include <driver/int_types.h>
#include "sys_driver.h"

#include "bk_uart.h"
#include "bk_gpio.h"
#include <driver/gpio.h>

#include "gpio_driver.h"
#if CONFIG_SDIO_GDMA_EN
#include "driver/dma.h"
#include <driver/hal/hal_dma_types.h>
#endif

#include "driver/sdio_types.h"
#include "sdio_hal.h"
#include "sdio_utils.h"
#include "sdio_slave_driver.h"


#if CONFIG_SDIO_V2P0

static sdio_driver_t s_sdio_driver;
static bool s_sdio_driver_is_init = false;
static beken_queue_t s_sdio_msg_que = NULL;
static beken_thread_t s_sdio_thread = NULL;


#define SDIO_RETURN_ON_NOT_INIT() do {\
		if (!s_sdio_driver_is_init) {\
			SDIO_LOG_ERR("driver not init\r\n");\
			return BK_ERR_SDIO_NOT_INIT;\
		}\
	} while(0)

#define SDIO_RETURN_CHAN_ID(chan_id) do {\
		if (chan_id >= SDIO_CHAN_MAX_CNT) {\
			SDIO_LOG_ERR("chan id %d\r\n", chan_id);\
			return BK_ERR_SDIO_CHAN_ID_INVALID;\
		}\
	} while(0)

#define SDIO_RETURN_CHAN_DIRECT(direct) do {\
		if (direct > SDIO_CHAN_RX) {\
			SDIO_LOG_ERR("direct %d\r\n", direct);\
			return BK_ERR_SDIO_DIRECT_INVALID;\
		}\
	} while(0)


#if CONFIG_SDIO_GDMA_EN
void sdio_slave_tx_finish(dma_id_t dma_id)
{

}

static bk_err_t sdio_tx_dma_set_src_buf(uint8_t *psrc, uint32_t count)
{
	return bk_dma_set_src_addr(DMA_ID_5, (uint32_t)psrc, (uint32_t)(psrc+count));
	//BK_LOG_ON_ERR(bk_dma_set_transfer_len(DMA_ID_5, count));
}

static bk_err_t sdio_dma_tx_init(void)
{
	dma_config_t dma_config = {0};

	SDIO_LOG_DEBUG_FUNCTION_ENTRY();	

	dma_config.mode = DMA_WORK_MODE_SINGLE;
	dma_config.chan_prio = 0;

	dma_config.src.dev = DMA_DEV_DTCM;
	dma_config.src.width = DMA_DATA_WIDTH_32BITS;
	//dma_config.src.start_addr = (uint32) psrc;
	//dma_config.src.end_addr =  (uint32) psrc + count;

	dma_config.dst.dev = DMA_DEV_SDIO;
	dma_config.dst.width = DMA_DATA_WIDTH_32BITS;
	dma_config.dst.start_addr = (uint32_t) SDIO_REG0XB_ADDR;
	dma_config.dst.end_addr = (uint32_t) SDIO_REG0XB_ADDR;

	BK_LOG_ON_ERR(bk_dma_init(DMA_ID_5, &dma_config));
	BK_LOG_ON_ERR(bk_dma_register_isr(DMA_ID_5, NULL, sdio_slave_tx_finish));
	BK_LOG_ON_ERR(bk_dma_enable_finish_interrupt(DMA_ID_5));

	SDIO_LOG_DEBUG_FUNCTION_EXIT();	

	return BK_OK;
}

void sdio_slave_rx_finish(dma_id_t dma_id)
{

}

static bk_err_t sdio_rx_dma_set_dst_buf(uint8_t *pdst, uint32_t count)
{
	return bk_dma_set_dest_addr(DMA_ID_5, (uint32_t)pdst, (uint32_t)(pdst+count));
}

static bk_err_t sdio_dma_rx_init()
{
	dma_config_t dma_config = {0};

	SDIO_LOG_DEBUG_FUNCTION_ENTRY();	

	dma_config.mode = DMA_WORK_MODE_SINGLE;
	dma_config.chan_prio = 0;

	dma_config.src.dev = DMA_DEV_SDIO;
	dma_config.src.width = DMA_DATA_WIDTH_32BITS;
	dma_config.src.start_addr = (uint32) SDIO_REG0XC_ADDR;
	dma_config.src.end_addr =  (uint32) SDIO_REG0XC_ADDR;

	dma_config.dst.dev = DMA_DEV_DTCM;
	dma_config.dst.width = DMA_DATA_WIDTH_32BITS;
	//dma_config.dst.start_addr = (uint32) pbuf;
	//dma_config.dst.end_addr = (uint32) pbuf + count;

	BK_LOG_ON_ERR(bk_dma_init(DMA_ID_5, &dma_config));
	//BK_LOG_ON_ERR(bk_dma_set_transfer_len(DMA_ID_5, count));
	BK_LOG_ON_ERR(bk_dma_register_isr(DMA_ID_5, NULL, sdio_slave_rx_finish));
	BK_LOG_ON_ERR(bk_dma_enable_finish_interrupt(DMA_ID_5));

	SDIO_LOG_DEBUG_FUNCTION_EXIT();	

	return BK_OK;
}

/* It doesn't config the SRC/DST address in APP's buffer */
static bk_err_t sdio_dma_init()
{
	bk_err_t ret = BK_OK;

	//SDIO DMA TX init
	sdio_dma_tx_init();
	
	//SDIO DMA RX init
	sdio_dma_rx_init();

	return ret;
}

static bk_err_t sdio_dma_deinit()
{
	//TODO:
	bk_err_t ret = BK_OK;
	return ret;
}

#endif

#if CONFIG_GPIO_NOTIFY_TRANSACTION_EN
static uint8_t gpio_to_host_num = GPIO_SDIO_NOTIFY_HOST, gpio_from_host_num = GPIO_SDIO_HOST_WAKEUP_SLAVE;
static volatile uint32_t allow_sleep = 1;	//default:host whether allow ctrl sleep
static uint64_t last_rx_data_time = 0;
static wk_slp_fail_callback wk_host_fail_cb;
static beken_semaphore_t s_sdio_host_ack;

void register_wake_host_fail_func(wk_slp_fail_callback cb)
{
	SDIO_LOG_DEBUG_FUNCTION_ENTRY();
	wk_host_fail_cb = cb;
	SDIO_LOG_DEBUG_FUNCTION_EXIT();
}

static void sdio_wake_host_fail(void)		//do nothing until now
{
	SDIO_LOG_DEBUG_FUNCTION_ENTRY();
	if(wk_host_fail_cb)
		wk_host_fail_cb();
	SDIO_LOG_DEBUG_FUNCTION_EXIT();
}

static bk_err_t sdio_slave_tx_recv_host_ack_notification(void)
{
	bk_err_t ret = BK_OK;

	ret = rtos_set_semaphore(&s_sdio_host_ack);
	if(ret != BK_OK)
	{
		SDIO_LOG_ERR("set sema");
		return ret;
	}

	return ret;
}

/**
 * @brief	Pullup GPIO to notify host, slave will send data.
 *
 * This API Pullup GPIO to notify host, slave will send data.
 *	 - Pullup GPIO to notify host
 *	 - Slave wait "host send CMD52 to get slave will transaction data length".
 *     If 100ms doesn't get CMD52 mean's host timeout.
 *
 * @attention 1. This API should be called after SDIO init.
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t sdio_slave_notify_host(void)
{
	bk_err_t ret = BK_OK;

	SDIO_LOG_DEBUG_FUNCTION_ENTRY();

	if(WAKEUP_LEVEL)
		bk_gpio_set_output_high(gpio_to_host_num);
	else
		bk_gpio_set_output_low(gpio_to_host_num);

	//wait here
	ret = rtos_get_semaphore(&s_sdio_host_ack, WAIT_HOST_ACK_TIMEOUT);
	if(ret != BK_OK)
	{
		SDIO_LOG_ERR("get sema");
		sdio_wake_host_fail();

		//TODO: Assert?
		return ret;
	}

	//clear wake host
	if(WAKEUP_LEVEL)
		bk_gpio_set_output_low(gpio_to_host_num);
	else
		bk_gpio_set_output_high(gpio_to_host_num);

	SDIO_LOG_DEBUG_FUNCTION_EXIT();

	return ret;
}

static void sdio_gpio_notify_to_host_init(uint8_t gpio_num)
{
	SDIO_LOG_DEBUG_FUNCTION_ENTRY();
	gpio_to_host_num = gpio_num;
	bk_gpio_enable_output(gpio_to_host_num);	//output
	bk_gpio_set_output_low(gpio_to_host_num);	//to low, means allow peer side enter sleep.
	SDIO_LOG_DEBUG_FUNCTION_EXIT();
}

static void sdio_gpio_notify_to_host_deinit(uint8_t gpio_num)
{
	SDIO_LOG_DEBUG_FUNCTION_ENTRY();

	gpio_dev_unmap(gpio_num);

	SDIO_LOG_DEBUG_FUNCTION_EXIT();
}


static void sdio_gpio_notify_from_host_init(uint8_t gpio_num)
{
	SDIO_LOG_DEBUG_FUNCTION_ENTRY();
	gpio_from_host_num = gpio_num;
	bk_gpio_enable_input(gpio_from_host_num);
	bk_gpio_pull_down(gpio_from_host_num);

	//maybe host pull up the gpio before controller finish bootup.
	if(get_controller_wk_slp_status())
	{
		bk_gpio_enable_interrupt(gpio_from_host_num);	//default check high level
	}
	else
	{
		bk_gpio_disable_interrupt(gpio_from_host_num);	//maybe host pull up the gpio before controller finish bootup.
	}

	SDIO_LOG_DEBUG_FUNCTION_EXIT();
}

static void sdio_gpio_notify_from_host_deinit(uint8_t gpio_num)
{
	SDIO_LOG_DEBUG_FUNCTION_ENTRY();

	gpio_dev_unmap(gpio_num);
	bk_gpio_disable_interrupt(gpio_num);	//default check high level
	bk_gpio_register_isr(gpio_num, (gpio_isr_t)NULL);

	SDIO_LOG_DEBUG_FUNCTION_EXIT();
}

/**
 * @brief	Just update software allow sleep status, used to check gpio irq is 
 *          whether sync with software status.
 *
 * This API ....
 *	 - 1:means allow sleep, 0 means host vote bk7256 don't go to sleep.
 *
 * @attention 1. This API should be called after SDIO init.
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
static void set_controller_wk_slp_status(uint32_t allow_slp)
{
	SDIO_LOG_DEBUG("v=%d\r\n", allow_slp);
	allow_sleep = allow_slp;	
}

/**
 * @brief	Get host whether allow slave goto sleep
 *
 * This API ....
 *
 * @attention 1. This API should be called after SDIO init.
 *
 * @return
 *	  - 1:means allow sleep, 0 means host vote bk7256 don't go to sleep.
 *	  - 
 */
uint32_t get_controller_wk_slp_status(void)
{
	uint32_t cur_sts = bk_gpio_get_input(gpio_from_host_num);

	SDIO_LOG_DEBUG_FUNCTION_ENTRY("cur_sts=%d,allow_sleep=%d", cur_sts, allow_sleep);

	if(cur_sts)		// !bk_gpio_get_input: it's optimized by compiler? the value maybe equal=0x100
		cur_sts = 0;
	else
		cur_sts = 1;
	if(allow_sleep != cur_sts)
	{
		SDIO_LOG_DEBUG("cur_sts=%d,allow_sleep=%d", cur_sts, allow_sleep);
		allow_sleep = cur_sts;
	}

	return (allow_sleep);
}

static void sdio_slave_response_host_wake_slave_ack(void)
{
	//do nothing now, no need to ack host
	SDIO_LOG_DEBUG_FUNCTION_ENTRY();
}

static void sdio_slave_response_host_sleep_slave_ack(void)
{
	//do nothing now, no need to ack host
	SDIO_LOG_DEBUG_FUNCTION_ENTRY();
}

/**
 * @brief	Host pull up/down GPIO to wakeup/clear wakeup slave.
 *          slave get the GPIO interrupt and run this handler.
 *
 * This API ....
 *
 * @attention 1. This API should be called after SDIO init.
 *
 * @return
 *	  - 
 *	  - 
 */
void wk_slp_controller_handler(void)
{
	uint32_t v27=bk_gpio_get_input(gpio_from_host_num);

	SDIO_LOG_DEBUG_FUNCTION_ENTRY();

	{
		if(v27)	//now is high:check to low
		{
			SDIO_LOG_DEBUG("wakeup:low to high");
			set_controller_wk_slp_status(0);
			set_last_receive_data_time();	//update wakeup time.

			sdio_slave_response_host_wake_slave_ack();

			bk_gpio_enable_input(gpio_from_host_num);
			bk_gpio_pull_up(gpio_from_host_num);
			bk_gpio_disable_interrupt(gpio_from_host_num);
		}
		else
		{
			SDIO_LOG_DEBUG("sleep:high to low");
			set_controller_wk_slp_status(1);
			//set_last_receive_data_time();	//update wakeup time.

			sdio_slave_response_host_sleep_slave_ack();
			bk_gpio_enable_input(gpio_from_host_num);
			bk_gpio_pull_down(gpio_from_host_num);
			bk_gpio_enable_interrupt(gpio_from_host_num);
		}
	}

	SDIO_LOG_DEBUG_FUNCTION_EXIT();
}

void set_last_receive_data_time(void)
{
	SDIO_LOG_DEBUG_FUNCTION_ENTRY();
	last_rx_data_time = bk_get_tick();
}

uint64_t get_last_receive_data_time(void)
{
	SDIO_LOG_DEBUG("last_time=%d", last_rx_data_time);
	return last_rx_data_time;
}

uint64_t check_wakeup_contorller_validity_timeout(void)
{
	uint8_t timeout;

	//LOG_D(GWS, "%s[+],last_time=%d\r\n", __func__, last_rx_data_time);
	if(get_controller_wk_slp_status() == 0)
	{
		timeout = (bk_get_tick() > (last_rx_data_time + FORBID_SLEEP_TIMEOUT) ? 1 : 0);
		if(timeout)
		{
			#define PRINT_INTERAL_TIME 10
			static uint64_t last_print_time = 0;

			if(bk_get_tick() - last_print_time > PRINT_INTERAL_TIME)
				SDIO_LOG_ERR("cur_time=%d,last_time=%d", __func__, bk_get_tick(), last_rx_data_time);
			last_print_time = bk_get_tick();
		}
	}

	return (bk_get_tick() - last_rx_data_time);
}

/**
 * @brief	wake,allow sleep by gpio feature resources init
 *
 * This API ....
 *
 * @param to_host: the gpio num which is used to wake up host or allow host sleep.
 * @param from_host: the gpio num which is used to wake up controller or allow controller sleep from host.
 * 
 * 
 * @attention 1. This API should be called after SDIO init.
 *
 * @return
 *	  - 
 *	  - 
 */
void sdio_gpio_notify_transaction_init(uint8_t to_host, uint8_t from_host)
{
	SDIO_LOG_DEBUG_FUNCTION_ENTRY();

	sdio_gpio_notify_to_host_init(to_host);
	sdio_gpio_notify_from_host_init(from_host);
	bk_gpio_register_isr(from_host, (gpio_isr_t)wk_slp_controller_handler);

	SDIO_LOG_DEBUG_FUNCTION_EXIT();
}

void sdio_gpio_notify_transaction_deinit(uint8_t to_host, uint8_t from_host)
{
	SDIO_LOG_DEBUG_FUNCTION_ENTRY();

	sdio_gpio_notify_to_host_deinit(to_host);
	sdio_gpio_notify_from_host_deinit(from_host);

	SDIO_LOG_DEBUG_FUNCTION_EXIT();
}

#endif	//CONFIG_GPIO_NOTIFY_TRANSACTION_EN

static void sdio_send_msg(sdio_msg_t *msg_p)
{
	bk_err_t ret;

	if (s_sdio_msg_que) {
		ret = rtos_push_to_queue(&s_sdio_msg_que, msg_p, BEKEN_NO_WAIT);
		if (kNoErr != ret)
			SDIO_LOG_ERR("sdio send msg failed");
	}
}

static bk_err_t sdio_gpio_init(void)
{
#if CONFIG_SDIO_4LINES_EN
	return gpio_sdio_sel(GPIO_SDIO_MAP_MODE0);
#else
	return gpio_sdio_one_line_sel(GPIO_SDIO_MAP_MODE0);
#endif
}

static bk_err_t sdio_gpio_deinit(void)
{
#if CONFIG_SDIO_4LINES_EN
	return gpio_sdio_sel(GPIO_SDIO_MAP_MODE0);
#else
	return gpio_sdio_one_line_sel(GPIO_SDIO_MAP_MODE0);
#endif
}

/**
 * @brief	sdio set controller clock.
 *
 * This API set the sdio clock as slave device.
 *	 -  1.The sdio slave default clock is setted as 26M.
 *      2.The transfer data clock on the lines is decided by host side,
 *        not this clock which is used for sdio-controller.
 *
 * @attention 
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t bk_sdio_set_clk(SDIO_CLK_SEL_T sel, SDIO_CLK_DIV_T div)
{
	uint32_t int_level = 0;

	int_level = rtos_disable_int();

	sys_driver_set_sdio_clk_sel((uint32_t)sel);
	sys_driver_set_sdio_clk_div((uint32_t)div);

	rtos_enable_int(int_level);
	return BK_OK;
}

/**
 * @brief	  Log out the SDIO REGs value without the "Data Read Reg(REG0xC)".
 *
 * This API log out SDIO REGs value without the "Data Read Reg(REG0xC)".
 *
 * @attention 1. This API doesn't log out the value of "Data Read Reg(REG0xC)",
 *               because if read REG0xC will cause FIFO data out.
 *
 * @return
 *	  - None.
 */
void bk_sdio_driver_dump_sdio_regs(void)
{
	uint32_t reg_addr = 0;

	SDIO_LOG_DEBUG_FUNCTION_ENTRY();

	for(reg_addr = SDIO_LL_REG_BASE; reg_addr < SDIO_LL_REG_END; reg_addr += 4)
	{
		if(reg_addr != SDIO_REG0XC_ADDR)	//skip read data out.
		{
			SDIO_LOG_DUMP("Reg0x%x=0x%x\r\n", (reg_addr - SDIO_LL_REG_BASE)/4, *(volatile uint32_t *)reg_addr);
		}
	}

	SDIO_LOG_DEBUG_FUNCTION_EXIT();
}

#if CONFIG_SDIO_CHANNEL_EN

/**
 * @brief	  Push the data use finish link list to free list.
 *
 * This API Push the data use finish link list to free list.
 *	 - Link the link list to free list.
 *   - 
 *
 * @param chan_id	The selected chan id.
 * @param direct	The channel used for TX data or RX data
 * @param head_p    The list head of which node will be to free list
 * @param tail_p    The list tail of which data will be to free list
 * @param count     How many nodes will be to free list
 *
 * @attention
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t bk_sdio_chan_push_free_list(
										sdio_chan_id_t chan_id, 
										chan_direct_t direct,
										sdio_node_ptr_t head_p,
										sdio_node_ptr_t tail_p,
										uint32_t count
										)
{
	bk_err_t ret = BK_OK;
	uint32_t int_level = 0;
#if SDIO_BIDIRECT_CHANNEL_EN
	uint32_t buf_index = direct;
#else
	uint32_t buf_index = 0;
#endif
	sdio_chan_buf_t *chan_buf_p = NULL;

	SDIO_RETURN_CHAN_ID(chan_id);
	SDIO_RETURN_CHAN_DIRECT(direct);

	int_level = rtos_disable_int();

	chan_buf_p = &s_sdio_driver.chan[chan_id].chan_buf[buf_index];
	sdio_list_push_list(&chan_buf_p->free_list, head_p, tail_p, count);
	chan_buf_p->free_list.count += count;

	rtos_enable_int(int_level);

	return ret;
}

/**
 * @brief	  Pop a free node from the selected channel and direct.
 *
 * This API Pop a free node from the selected channel and direct.
 *	 - Get a free node from the selected channel and direct.
 *   - Update the free node list to next node.
 *
 * @param chan_id	The selected chan id.
 * @param direct	The channel used for TX data or RX data
 * @param node_p    Saved the getted free buffer node.
 * @param size_p Saved the getted free buffer node capacity(How many bytes data can be saved).
 *
 * @attention
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t bk_sdio_chan_pop_free_node(sdio_chan_id_t chan_id, chan_direct_t direct, sdio_node_ptr_t *node_p, uint32_t *size_p)
{
	bk_err_t ret = BK_OK;
	uint32_t int_level = 0;
#if SDIO_BIDIRECT_CHANNEL_EN
	uint32_t buf_index = direct;
#else
	uint32_t buf_index = 0;
#endif
	sdio_chan_buf_t *chan_buf_p = NULL;

	SDIO_RETURN_CHAN_ID(chan_id);
	SDIO_RETURN_CHAN_DIRECT(direct);
	if(node_p == NULL)
	{
		SDIO_LOG_ERR("null pointer");
		return BK_ERR_SDIO_NULL_POINTER_PARAM;
	}

	int_level = rtos_disable_int();
	chan_buf_p = &s_sdio_driver.chan[chan_id].chan_buf[buf_index];

	if(chan_buf_p->free_list.count)
	{
		sdio_node_ptr_t tmp_node_p;

		ret = sdio_list_pop_node(&chan_buf_p->free_list, &tmp_node_p);
		if(ret != BK_OK)
		{
			SDIO_LOG_ERR("no free buf");
			return BK_ERR_SDIO_NO_BUFFER;
		}
		*node_p = tmp_node_p;
		*size_p = chan_buf_p->buf_size;
	}
	else
	{
		*size_p = 0;
		*node_p = NULL;
		SDIO_LOG_ERR("no free node");
		return BK_ERR_SDIO_NO_BUFFER;
	}

	rtos_enable_int(int_level);
	return BK_OK;
}

void sdio_chan_push_ongoing_node(
										sdio_chan_id_t chan_id, 
										chan_direct_t direct,
										sdio_node_ptr_t head_p,
										sdio_node_ptr_t tail_p,
										uint32_t count
										)
{
	uint32_t int_level = 0;
#if SDIO_BIDIRECT_CHANNEL_EN
	uint32_t buf_index = direct;
#else
	uint32_t buf_index = 0;
#endif
	sdio_chan_buf_t *chan_buf_p = NULL;

	//TODO: check chan_id and direct is whether valid

	int_level = rtos_disable_int();
	chan_buf_p = &s_sdio_driver.chan[chan_id].chan_buf[buf_index];

	sdio_list_push_list(&chan_buf_p->ongoing_list, head_p, tail_p, count);
	chan_buf_p->ongoing_list.count += count;

	rtos_enable_int(int_level);
}

bk_err_t sdio_chan_pop_ongoing_node(sdio_chan_id_t chan_id, chan_direct_t direct, sdio_node_ptr_t *node_p)
{
	bk_err_t ret = BK_OK;
	uint32_t int_level = 0;
#if SDIO_BIDIRECT_CHANNEL_EN
	uint32_t buf_index = direct;
#else
	uint32_t buf_index = 0;
#endif
	sdio_chan_buf_t *chan_buf_p = NULL;

	SDIO_RETURN_CHAN_ID(chan_id);
	SDIO_RETURN_CHAN_DIRECT(direct);

	int_level = rtos_disable_int();
	chan_buf_p = &s_sdio_driver.chan[chan_id].chan_buf[buf_index];
	if(chan_buf_p->ongoing_list.count)
	{
		sdio_node_ptr_t tmp_node_p;
		ret = sdio_list_pop_node(&chan_buf_p->ongoing_list, &tmp_node_p);
		if(ret != BK_OK)
		{
			SDIO_LOG_ERR("no transaction finish node");
			return BK_ERR_SDIO_NO_TRANSACTION_FINISH_NODE;
		}

		*node_p = tmp_node_p;
	}
	else
	{
		rtos_enable_int(int_level);
		return BK_FAIL;
	}

	rtos_enable_int(int_level);

	return BK_OK;
}

bk_err_t sdio_chan_push_finish_list(
										sdio_chan_id_t chan_id, 
										chan_direct_t direct,
										sdio_node_ptr_t head_p,
										sdio_node_ptr_t tail_p,
										uint32_t count
										)
{
	uint32_t int_level = 0;
#if SDIO_BIDIRECT_CHANNEL_EN
	uint32_t buf_index = direct;
#else
	uint32_t buf_index = 0;
#endif
	sdio_chan_buf_t *chan_buf_p = NULL;

	SDIO_RETURN_CHAN_ID(chan_id);
	SDIO_RETURN_CHAN_DIRECT(direct);

	int_level = rtos_disable_int();
	chan_buf_p = &s_sdio_driver.chan[chan_id].chan_buf[buf_index];

	sdio_list_push_list(&chan_buf_p->finish_list, head_p, tail_p, count);
	chan_buf_p->finish_list.count += count;

	rtos_enable_int(int_level);

	return BK_OK;
}

bk_err_t sdio_chan_pop_finish_node(sdio_chan_id_t chan_id, chan_direct_t direct, sdio_node_ptr_t *node_p)
{
	bk_err_t ret = BK_OK;
	uint32_t int_level = 0;
#if SDIO_BIDIRECT_CHANNEL_EN
	uint32_t buf_index = direct;
#else
	uint32_t buf_index = 0;
#endif
	sdio_chan_buf_t *chan_buf_p = NULL;

	SDIO_RETURN_CHAN_ID(chan_id);
	SDIO_RETURN_CHAN_DIRECT(direct);

	int_level = rtos_disable_int();
	chan_buf_p = &s_sdio_driver.chan[chan_id].chan_buf[buf_index];

	if(chan_buf_p->finish_list.count)
	{
		sdio_node_ptr_t tmp_node_p;
		ret = sdio_list_pop_node(&chan_buf_p->ongoing_list, &tmp_node_p);
		if(ret != BK_OK)
		{
			SDIO_LOG_ERR("no free buf");
			return BK_ERR_SDIO_NO_BUFFER;
		}

		*node_p = tmp_node_p;
	}

	rtos_enable_int(int_level);

	return BK_OK;
}

/**
 * @brief	  Register the clalback function for sdio selected channel and direct.
 *
 * This API register the clalback function for sdio selected channel and direct.
 *	 - Save the callback in channel.
 *   - When tx/rx data finish, it will call this callback function.
 *
 * @param chan_id	The selected chan id.
 * @param direct	The channel used for TX data or RX data
 * @param cb        Registered callback function.
 *
 * @attention
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t bk_sdio_register_chan_cb(sdio_chan_id_t chan_id, chan_direct_t direct, sdio_chan_cb_t cb)
{
	uint32_t int_level = 0;
#if SDIO_BIDIRECT_CHANNEL_EN
	uint32_t buf_index = direct;
#else
	uint32_t buf_index = 0;
#endif
	sdio_chan_buf_t *chan_buf_p = NULL;

	SDIO_RETURN_CHAN_ID(chan_id);
	SDIO_RETURN_CHAN_DIRECT(direct);

	int_level = rtos_disable_int();
	chan_buf_p = &s_sdio_driver.chan[chan_id].chan_buf[buf_index];
	chan_buf_p->cb = cb;

	rtos_enable_int(int_level);

	return BK_OK;
}

//should call sdio_chan_push_free_node after finish use
static bk_err_t sdio_chan_notify_cb(sdio_chan_id_t chan_id, chan_direct_t direct)
{
	bk_err_t ret = BK_OK;
#if SDIO_BIDIRECT_CHANNEL_EN
	uint32_t buf_index = direct;
#else
	uint32_t buf_index = 0;
#endif
	sdio_chan_buf_t *chan_buf_p = &s_sdio_driver.chan[chan_id].chan_buf[buf_index];

	if(chan_buf_p->cb)
	{
		chan_buf_p->cb(&chan_buf_p->finish_list.head, &chan_buf_p->finish_list.tail, chan_buf_p->finish_list.count);
	}

	ret = rtos_set_semaphore(&chan_buf_p->semaphore);
	if(ret != BK_OK)
	{
		SDIO_LOG_ERR("set sema");
		return ret;
	}

	return ret;
}

static bool sdio_chan_direct_is_inited(sdio_chan_id_t chan_id, chan_direct_t direct)
{
#if SDIO_BIDIRECT_CHANNEL_EN
	uint32_t buf_index = direct;
#else
	uint32_t buf_index = 0;
#endif

	if((s_sdio_driver.chan[chan_id].tx_direct == (1<<buf_index)) ||
		(s_sdio_driver.chan[chan_id].rx_direct == (1<<buf_index)))
		return true;
	else
		return false;
}

/**
 * @brief	  Init a sdio channel.
 *
 * This API init a sdio channel.
 *	 - Init a single list for buffers if count != 0
 *   - Create a semaphore to do notification for read/write data
 *   - Create a mutex lock to protect concurrence operate link list
 *
 * @param chan_id	The selected chan id.
 * @param direct	The channel used for TX data or RX data
 * @param count How many buffers will be malloced by channel.
 *              If count == 0 means the buffer malloced by APP.      
 * @param size  Each buffer size for data which doesn't include SDIO header,it should be 4 bytes align
 *
 * @attention The count value is re-used as APPLICATION supports buffer or DRIVER supports buffer.
 *            Though if count == 0, the APPLICATION(caller) should set the size value as driver knows
 *            how many bytes can be saved in the buffer which APPLICATION supported.
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t bk_sdio_init_channel(sdio_chan_id_t chan_id, chan_direct_t direct, uint32_t count, uint32_t size)
{
	bk_err_t ret = BK_OK;
	uint32_t int_level = 0;
	uint32_t sema_count = SDIO_CHAN_SEMAPHORE_COUNT;
#if SDIO_BIDIRECT_CHANNEL_EN
	uint32_t buf_index = direct;
#else
	uint32_t buf_index = 0;
#endif
	sdio_chan_buf_t *chan_buf_p = NULL;

	SDIO_RETURN_CHAN_ID(chan_id);
	SDIO_RETURN_CHAN_DIRECT(direct);

	int_level = rtos_disable_int();

	chan_buf_p = &s_sdio_driver.chan[chan_id].chan_buf[buf_index];
	if(sdio_chan_direct_is_inited(chan_id, direct))
	{
		SDIO_LOG_INFO("chan%d,direct %d inited,buf cnt=%d,buf size=%d",
			          chan_id, direct, chan_buf_p->buf_cnt, chan_buf_p->buf_size);
		goto err_exit;
	}

#if 0	//TODO:check whether needs it.
	if(chan_buf_p->lock_p == NULL)
	{
		ret = rtos_init_mutex(&chan_buf_p->lock_p);
		if (kNoErr != ret) 
		{
			SDIO_LOG_ERR("mutex init");
			goto err_exit;
		}
	}
#endif

	if(count)
		sema_count = count;
	ret = rtos_init_semaphore(&chan_buf_p->semaphore, sema_count);
	if (kNoErr != ret) 
	{
		SDIO_LOG_ERR("semaphore init");
		goto err_exit;
	}

	if(count)	//driver support buffer
	{
		ret = sdio_list_init(count, size, &chan_buf_p->free_list.head, &chan_buf_p->free_list.tail);
		if (BK_OK != ret) 
		{
			SDIO_LOG_ERR("list init");
			goto err_exit;
		}
	}

	//save channel info
	s_sdio_driver.chan[chan_id].chan_id = chan_id;
	if(direct == SDIO_CHAN_TX)
		s_sdio_driver.chan[chan_id].tx_direct = 1;
	else
		s_sdio_driver.chan[chan_id].rx_direct = 1;

	//save buf info
	chan_buf_p->buf_cnt = count;
	chan_buf_p->buf_size = size;
	chan_buf_p->free_list.count = count;
	chan_buf_p->transaction_len = 0;

	//add this to avoid modify err_exit involve issue.
	rtos_enable_int(int_level);
	return ret;

err_exit:
	rtos_enable_int(int_level);
	return ret;
}

/**
 * @brief	  Deinit a sdio channel.
 *
 * This API deinit a sdio channel.
 *	 - Init a single list for buffers
 *   - Create a mutex lock to protect concurrence operate link list
 *
 * @param chan_id	The selected chan id.
 * @param direct	The channel used for TX data or RX data
 *
 * @attention
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t bk_sdio_deinit_channel(sdio_chan_id_t chan_id, chan_direct_t direct)
{
	bk_err_t ret = BK_OK;
	uint32_t int_level = 0;
#if SDIO_BIDIRECT_CHANNEL_EN
	uint32_t buf_index = direct;
#else
	uint32_t buf_index = 0;
#endif
	sdio_chan_buf_t *chan_buf_p = NULL;

	SDIO_RETURN_CHAN_ID(chan_id);
	SDIO_RETURN_CHAN_DIRECT(direct);

	int_level = rtos_disable_int();

	chan_buf_p = &s_sdio_driver.chan[chan_id].chan_buf[buf_index];
	if(sdio_chan_direct_is_inited(chan_id, direct) == false)
	{
		SDIO_LOG_INFO("chan%d,direct %d isn't inited", chan_id, direct);
		goto err_exit;
	}

	if(chan_buf_p->buf_cnt)	//driver support buffer
	{
		ret = sdio_list_deinit(&chan_buf_p->free_list);
		if (BK_OK != ret) 
		{
			SDIO_LOG_ERR("list deinit");
			goto err_exit;
		}
		chan_buf_p->free_list.head = NULL;

		ret = sdio_list_deinit(&chan_buf_p->ongoing_list);
		if (BK_OK != ret) 
		{
			SDIO_LOG_ERR("list deinit");
			goto err_exit;
		}
		chan_buf_p->ongoing_list.head = NULL;

		ret = sdio_list_deinit(&chan_buf_p->finish_list);
		if (BK_OK != ret) 
		{
			SDIO_LOG_ERR("list deinit");
			goto err_exit;
		}
		chan_buf_p->finish_list.head = NULL;
	}

	if(chan_buf_p->semaphore)
	{
		ret = rtos_deinit_semaphore(&chan_buf_p->semaphore);
		if (kNoErr != ret) 
		{
			SDIO_LOG_ERR("semaphore deinit");
			goto err_exit;
		}

		chan_buf_p->semaphore = NULL;
	}

#if 0	//TODO:check whether needs it.
	if(chan_buf_p->lock_p == NULL)
	{
		ret = rtos_deinit_mutex(&chan_buf_p->lock_p);
		if (kNoErr != ret) 
		{
			SDIO_LOG_ERR("mutex deinit");
			goto err_exit;
		}
	}
#endif

	//save channel info
	s_sdio_driver.chan[chan_id].chan_id = SDIO_CHAN_MAX_CNT;
	if(direct == SDIO_CHAN_TX)
		s_sdio_driver.chan[chan_id].tx_direct = 0;
	else
		s_sdio_driver.chan[chan_id].rx_direct = 0;

	//save buf info
	chan_buf_p->buf_cnt = 0;
	chan_buf_p->buf_size = 0;
	chan_buf_p->free_list.count = 0;
	chan_buf_p->transaction_len = 0;

	//add this to avoid modify err_exit involve issue.
	rtos_enable_int(int_level);
	return ret;

err_exit:
	rtos_enable_int(int_level);
	return ret;
}

static bk_err_t sdio_slave_get_valid_ongoing_rx_buf_ptr(sdio_chan_id_t chan_id, uint32_t count, uint8_t **buf_p)
{
	bk_err_t ret = BK_OK;
	uint32_t int_level = 0;
	uint32_t left_size = 0;
#if SDIO_BIDIRECT_CHANNEL_EN
	uint32_t buf_index = SDIO_CHAN_RX;
#else
	uint32_t buf_index = 0;
#endif
	sdio_chan_buf_t *chan_buf_p = NULL;

	SDIO_RETURN_CHAN_ID(chan_id);

	int_level = rtos_disable_int();
	chan_buf_p = &s_sdio_driver.chan[chan_id].chan_buf[buf_index];

	if(count > chan_buf_p->buf_size)
	{
		//TODO:Assert
		return BK_FAIL;
	}
		
	//*trans_len_p = s_sdio_driver.chan[chan_id].chan_buf[buf_index].transaction_len;
	left_size = chan_buf_p->buf_size - s_sdio_driver.chan[chan_id].chan_buf[buf_index].transaction_len;
	if(left_size >= count)
	{
		*buf_p = (uint8_t *)chan_buf_p->ongoing_list.head + sizeof(sdio_node_t) + s_sdio_driver.chan[chan_id].chan_buf[buf_index].transaction_len;
	}
	else	//current node left size isn't enough, save data to next node
	{
		sdio_node_ptr_t head_p = NULL;

		//pop out the buffer node from ongoing list
		ret = sdio_chan_pop_ongoing_node(chan_id, SDIO_CHAN_RX, &head_p);

		//push current node to rx finish list
		//here can't confirm this is one packet as maybe host packet size is large then slave node capacity.
		sdio_chan_push_finish_list(chan_id, SDIO_CHAN_RX, head_p, NULL, 1);

		//get next valid node
		s_sdio_driver.chan[chan_id].chan_buf[buf_index].transaction_len = 0;
		*buf_p = (uint8_t *)chan_buf_p->ongoing_list.head + sizeof(sdio_node_t);
	}

	rtos_enable_int(int_level);

	return ret;
}

static bk_err_t sdio_slave_get_valid_ongoing_tx_buf_ptr(sdio_chan_id_t chan_id, uint32_t len, uint8_t **buf_p)
{
	bk_err_t ret = BK_OK;
	uint32_t int_level = 0;
	uint32_t left_size = 0;
#if SDIO_BIDIRECT_CHANNEL_EN
	uint32_t buf_index = SDIO_CHAN_TX;
#else
	uint32_t buf_index = 0;
#endif
	sdio_chan_buf_t *chan_buf_p = NULL;

	SDIO_LOG_DEBUG_FUNCTION_ENTRY();

	SDIO_RETURN_CHAN_ID(chan_id);

	int_level = rtos_disable_int();
	chan_buf_p = &s_sdio_driver.chan[chan_id].chan_buf[buf_index];
	left_size = chan_buf_p->ongoing_list.head->len - s_sdio_driver.chan[chan_id].chan_buf[buf_index].transaction_len;
	if(left_size > 0)	//current head node tx isn't finish 
	{
		*buf_p = (uint8_t *)chan_buf_p->ongoing_list.head + sizeof(sdio_node_t) + s_sdio_driver.chan[chan_id].chan_buf[buf_index].transaction_len;
	}
	else	//this packet finish, next packet
	{
		sdio_node_ptr_t head_p = NULL;
		sdio_msg_t msg;

		//pop out the buffer node from ongoing list
		ret = sdio_chan_pop_ongoing_node(chan_id, SDIO_CHAN_TX, &head_p);
		if(ret != BK_OK)
		{
			SDIO_LOG_ERR("pop node");
			rtos_enable_int(int_level);
			return ret;
		}

		//push to tx finish list
		ret = sdio_chan_push_finish_list(chan_id, SDIO_CHAN_TX, head_p, NULL, 1);
		if(ret != BK_OK)
		{
			SDIO_LOG_ERR("push lish");
			rtos_enable_int(int_level);
			return ret;
		}

		//notify application can release buffer
		msg.id = SDIO_WRITE_NODE_FINISH;
		sdio_send_msg(&msg);

		//get next tx node
		s_sdio_driver.chan[chan_id].chan_buf[buf_index].transaction_len = 0;
		*buf_p = (uint8_t *)chan_buf_p->ongoing_list.head + sizeof(sdio_node_t);
	}	

	rtos_enable_int(int_level);

	SDIO_LOG_DEBUG_FUNCTION_EXIT();

	return ret;
}

static bk_err_t sdio_slave_add_ongoing_buf_trans_len(sdio_chan_id_t chan_id, chan_direct_t direct, uint32_t trans_len)
{
	bk_err_t ret = BK_OK;
	uint32_t int_level = 0;
#if SDIO_BIDIRECT_CHANNEL_EN
	uint32_t buf_index = direct;
#else
	uint32_t buf_index = 0;
#endif
	sdio_chan_buf_t *chan_buf_p = NULL;

	SDIO_RETURN_CHAN_ID(chan_id);
	SDIO_RETURN_CHAN_DIRECT(direct);

	int_level = rtos_disable_int();

	chan_buf_p = &s_sdio_driver.chan[chan_id].chan_buf[buf_index];
	chan_buf_p->transaction_len += trans_len;
	rtos_enable_int(int_level);

	SDIO_LOG_DEBUG("chan_id=%d,tx/rx=%d,tarns_len=%d,total_len=%d", chan_id, direct, trans_len, chan_buf_p->transaction_len);

	return ret;
}

#endif

//WARNING:Becard 4bytes align issue
static bk_err_t sdio_slave_rx_fifo_to_buf(uint8_t *tar_addr_p, uint32_t count)
{
	//TODO:DMA not support until now.
#ifdef CONFIG_SDIO_GDMA_EN
	sdio_rx_dma_set_dst_buf(tar_addr_p, count);
	return bk_dma_start(DMA_ID_5);
#else	//if not use GDMA, here will block IRQ too much time
	uint32_t i, byte_cnt = count % 4, word_cnt = count/4;
	uint8_t *cur_p = tar_addr_p;

	//read data from SDIO REG to buffer
	for(i = 0; i < word_cnt; i = i + 4)
	{
		while(sdio_hal_get_read_ready() == 0)
		{
	
		}
	
		*(uint32_t *)cur_p = sdio_hal_slave_read_data();
		cur_p += 4;
	}

	if(byte_cnt)
	{
		while(sdio_hal_get_read_ready() == 0)
		{
		
		}

		//though here write 4 bytes, but the buffer should not use this length if not align 4 bytes
		*(uint32_t *)cur_p = sdio_hal_slave_read_data();
	}

#endif

	return BK_OK;
}

static bk_err_t sdio_slave_rx(uint32_t count)
{
	uint8_t *tar_addr_p = NULL;
	//TODO: As SW doesn't check header chan_id, so here use default value.
	sdio_chan_id_t chan_id = SDIO_CHAN_PLATFORM;

	//search current channel
	//TODO:uses CHAN_ID_PLATFORM

	//get current channel rx ptr
	sdio_slave_get_valid_ongoing_rx_buf_ptr(chan_id, count, &tar_addr_p);

	//save data to rx buffer
	if(tar_addr_p)
	{
		sdio_slave_rx_fifo_to_buf(tar_addr_p, count);
	}
	else
	{
		return BK_FAIL;
	}

	//update rx transaction len
	sdio_slave_add_ongoing_buf_trans_len(chan_id, SDIO_CHAN_RX, count);

	return BK_OK;
}

/**
 * @brief	 SDIO sync read data from peer side(From Host) 
 *
 * This API SDIO sync read data from peer side(From Host).
 *	 - Push the read link list to rxing list, after finish read, it will run out, or blocked by sema.
 *	 - 
 * @param chan_id	The selected chan id.
 * @param head_p    The list head of which data will be save read data from peer side
 * @param tail_p    The list tail of which data will be save read data from peer side
 * @param count     How many nodes will be used to save data.
 *
 * @attention 1. 
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t bk_sdio_slave_sync_read(sdio_chan_id_t chan_id, sdio_node_ptr_t head_p, sdio_node_ptr_t tail_p, uint32_t count)
{
	bk_err_t ret = BK_OK;
	uint32_t int_level = 0;
#if SDIO_BIDIRECT_CHANNEL_EN
	uint32_t buf_index = SDIO_CHAN_RX;
#else
	uint32_t buf_index = 0;
#endif
	sdio_chan_buf_t *chan_buf_p = NULL;

	SDIO_RETURN_CHAN_ID(chan_id);

	int_level = rtos_disable_int();
	chan_buf_p = &s_sdio_driver.chan[chan_id].chan_buf[buf_index];

	ret = sdio_list_push_list(&chan_buf_p->ongoing_list, head_p, tail_p, count);
	{
		SDIO_LOG_ERR("push list");
		goto err_exit;
	}

	chan_buf_p->ongoing_list.count += count;

	//CMD start
	sdio_hal_slave_cmd_start(1);

	//TODO:wait read finish
	ret = rtos_get_semaphore(&chan_buf_p->semaphore, BEKEN_WAIT_FOREVER);
	if(ret != BK_OK)
	{
		SDIO_LOG_ERR("get sema");
		goto err_exit;
	}

	//add this to avoid modify err_exit involve issue.
	rtos_enable_int(int_level);
	return ret;

err_exit:
	rtos_enable_int(int_level);
	return ret;
}

/**
 * @brief	  
 *
 * This API .
 *	 - 
 *	 - 
 *
 * @attention 1. 
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t sdio_slave_async_read(sdio_chan_id_t chan_id, sdio_node_ptr_t head_p, sdio_node_ptr_t tail_p, uint32_t count)
{
	bk_err_t ret = BK_OK;

	return ret;
}

//WARNING:Becare 4bytes align issue
static bk_err_t sdio_slave_tx_buf_to_fifo(uint8_t *src_addr_p, uint32_t count)
{
	//TODO:DMA not support until now.
#ifdef CONFIG_SDIO_GDMA_EN
	sdio_tx_dma_set_src_buf(src_addr_p, count);
	return bk_dma_start(DMA_ID_5);
#else	//if not use GDMA, here will block IRQ too much time
	uint32_t i, byte_cnt = count % 4, word_cnt = count/4;
	uint8_t *cur_p = src_addr_p;

	//read data from buffer to SDIO REG(SDIO FIFO)
	for(i = 0; i < word_cnt; i = i + 4)
	{
		while(sdio_hal_get_write_ready() == 0)
		{
	
		}
	
		sdio_hal_slave_write_data(*(uint32_t *)cur_p);
		cur_p += 4;
	}

	if(byte_cnt)
	{
		while(sdio_hal_get_write_ready() == 0)
		{
		
		}
		//though here write 4 bytes, but the buffer should not use this length if not align 4 bytes
		sdio_hal_slave_write_data(*(uint32_t *)cur_p);
	}

#endif

	return BK_OK;
}

static bk_err_t sdio_slave_tx(sdio_chan_id_t chan_id, uint32_t len)
{
	uint8_t *src_addr_p = NULL;

	//get current channel tx ptr
	sdio_slave_get_valid_ongoing_tx_buf_ptr(chan_id, len, &src_addr_p);

	//save data to tx fifo
	if(src_addr_p)
	{
		sdio_slave_tx_buf_to_fifo(src_addr_p, len);
	}
	else
	{
		return BK_FAIL;
	}

	//update tx transaction len
	sdio_slave_add_ongoing_buf_trans_len(chan_id, SDIO_CHAN_TX, len);

	return BK_OK;
}

/**
 * @brief	 SDIO sync write data to peer side(To Host) 
 *
 * This API SDIO sync write data to peer side(To Host) .
 *	 - Push the write link list to write list, after finish write, it will run out, or blocked by sema.
 *	 - 
 * @param chan_id	The selected chan id.
 * @param head_p    The list head of which data will be write
 * @param tail_p    The list tail of which data will be write
 * @param count     How many nodes will be write
 *
 * @attention 1. 
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t bk_sdio_slave_sync_write(sdio_chan_id_t chan_id, sdio_node_ptr_t head_p, sdio_node_ptr_t tail_p, uint32_t count)
{
	bk_err_t ret = BK_OK;
	uint32_t int_level = 0;
#if SDIO_BIDIRECT_CHANNEL_EN
	uint32_t buf_index = SDIO_CHAN_TX;
#else
	uint32_t buf_index = 0;
#endif
	sdio_chan_buf_t *chan_buf_p = NULL;
	
	SDIO_RETURN_CHAN_ID(chan_id);

	int_level = rtos_disable_int();

	chan_buf_p = &s_sdio_driver.chan[chan_id].chan_buf[buf_index];

	ret = sdio_list_push_list(&chan_buf_p->ongoing_list, head_p, tail_p, count);
	if(ret != BK_OK)
	{
		SDIO_LOG_ERR("push list fail");
		goto err_exit;
	}

	//Slave set TX length, CMD start
	//be care:maybe the sdio is transaction head node now.
	sdio_hal_slave_set_tx_length(chan_buf_p->ongoing_list.head->len);
	sdio_hal_slave_cmd_start(1);

	//Notify host:slave will send data
#if CONFIG_GPIO_NOTIFY_TRANSACTION_EN
	sdio_slave_notify_host();
#endif

	//TODO:wait write finish
	ret = rtos_get_semaphore(&chan_buf_p->semaphore, BEKEN_WAIT_FOREVER);
	if(ret != BK_OK)
	{
		SDIO_LOG_ERR("get sema");
		goto err_exit;
	}

	//add this to avoid modify err_exit involve issue.
	rtos_enable_int(int_level);
	return ret;

err_exit:
	rtos_enable_int(int_level);
	return ret;
}


/**
 * @brief	  
 *
 * This API .
 *	 - 
 *	 - 
 *
 * @attention 1. 
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t sdio_slave_async_write(sdio_chan_id_t chan_id, sdio_node_ptr_t head_p, sdio_node_ptr_t tail_p, uint32_t count)
{
	bk_err_t ret = BK_OK;

	return ret;
}

static void sdio_thread(void *arg)
{
	bk_err_t ret;

	while(1)
	{
		sdio_msg_t msg;
		ret = rtos_pop_from_queue(&s_sdio_msg_que, &msg, BEKEN_WAIT_FOREVER);
		if (kNoErr == ret)
		{
			switch(msg.id)
			{
				case SDIO_READ_NODE_FINISH:
				{
					sdio_chan_notify_cb(SDIO_CHAN_PLATFORM, SDIO_CHAN_RX);
					break;
				}

				case SDIO_WRITE_NODE_FINISH:
				{
					sdio_chan_notify_cb(SDIO_CHAN_PLATFORM, SDIO_CHAN_TX);
					break;
				}

				//DMA INT notify

				default:
					break;
			}
		}
	}
}

static bk_err_t sdio_sw_init(void)
{
	bk_err_t ret = BK_OK;

	ret = rtos_init_queue(
							&s_sdio_msg_que,
							SDIO_MSG_QUEUE_NAME,
							sizeof(sdio_msg_t),
							SDIO_MSG_QUEUE_COUNT
						);
	if (kNoErr != ret)
	{
		SDIO_LOG_ERR("init queue ret=%d", ret);
		goto err_exit;
	}

	ret = rtos_create_thread(
								&s_sdio_thread,
								SDIO_THREAD_PRIORITY,
								SDIO_THREAD_NAME,
								sdio_thread,
								SDIO_THREAD_STACK_SIZE,
								NULL
							);
	if (kNoErr != ret)
	{
		SDIO_LOG_ERR("init thread ret=%d", ret);
		goto err_exit;
	}

	//confirm to 0 as maybe crash value by other app.
	memset(&s_sdio_driver, 0, sizeof(s_sdio_driver));
	SDIO_LOG_DEBUG("sizeof(s_sdio_driver)=%d", sizeof(s_sdio_driver));

	//return, or err_exit release resources caused bug.
	return ret;

err_exit:
	if(s_sdio_msg_que)
	{
		rtos_deinit_queue(&s_sdio_msg_que);
	}

	if(s_sdio_thread)
	{
		rtos_delete_thread(&s_sdio_thread);
	}

	return ret;
}

static void sdio_slave_isr(void)
{
	uint32_t reg0x9_v = sdio_hal_get_int_status();
	uint8_t cmd_index = (reg0x9_v >> 14) & (0x3f);
	//uint32_t cmd_arg0 = 0;
	//uint32_t cmd_arg1 = 0;

	os_printf("SDIO enter interrupt Reg0x9=0x%x \r\n", reg0x9_v);

	//Slave response end int
	if(sdio_hal_slave_get_cmd_response_end_int())
	{
		switch (cmd_index)
		{
			case SDIO_CMD_INDEX_52:
			{
				uint32_t cmd52_arg0 = sdio_hal_slave_get_func_reg_value();

				//host write func reg0 to 1, notify slave stop, which means one packet send finish.
				if(((sdio_cmd52_func_arg0_t) cmd52_arg0).stop == 1)
				{
					sdio_msg_t  msg;
					sdio_node_ptr_t head_p = NULL;
					//pop out the buffer node from ongoing list
					//TODO:channel value is fixed.
					sdio_chan_pop_ongoing_node(SDIO_CHAN_PLATFORM, SDIO_CHAN_RX, &head_p);

					//push to rx finish list
					//TODO:channel value is fixed.
					sdio_chan_push_finish_list(SDIO_CHAN_PLATFORM, SDIO_CHAN_RX, head_p, NULL, 1);

					//notify data finish
					msg.id = SDIO_READ_NODE_FINISH;
					sdio_send_msg(&msg);

					//clear stop status
					sdio_hal_slave_clear_stop();
				}
				//host read:get slave tx length
				{
					//do nothing, just wait next cmd:cmd53

#if CONFIG_GPIO_NOTIFY_TRANSACTION_EN
					//notify Slave that Host has response TX.
					sdio_slave_tx_recv_host_ack_notification();
#endif
				}
				break;
			}
			
			case SDIO_CMD_INDEX_53:
			{
				uint32_t cmd53_arg = sdio_hal_slave_get_cmd_arg0();
				
				if(((sdio_cmd53_arg_t)cmd53_arg).rw)	//host read, slave write
				{
					//TODO:CHAN ID is fixed. 
					//save data to sdio fifo
					sdio_slave_tx(SDIO_CHAN_PLATFORM, (uint32_t)((sdio_cmd53_arg_t)cmd53_arg).count);
					//start transaction en, wait host read
					sdio_hal_slave_tx_transaction_en();
				}
				else	//host write, slave read
				{
					//donothing, just wait next cmd
				}
				break;
			}

			default:
				break;
		}

		sdio_hal_slave_clear_cmd_response_end_int();	//CMD_S_RES_END_INT
		os_printf("CMD%d Respond end \r\n", cmd_index);

		sdio_hal_slave_cmd_start(1);	//wait to receive next CMD.

		//TODO:need to check whether return.
		//return;
	}

	//slave rx full or finish data int
	if(sdio_hal_slave_get_read_int_status())
	{
		uint32_t cmd53_arg = sdio_hal_slave_get_cmd_arg0();
		uint32_t rx_cnt = ((sdio_cmd53_arg_t)cmd53_arg).count;

		//clear self int status
		sdio_hal_slave_clear_read_int_status();

		//save data from sdio to buffer
		rx_cnt = sdio_hal_slave_get_rx_count();
		sdio_slave_rx(rx_cnt);

		//slave can receive data again
		sdio_hal_slave_rx_clear_host_wait_write_data();

		//allow host send next block
		sdio_hal_slave_notify_host_next_block();

		//TODO:need to check whether return.
		//return;
	}

//TODO:
#if 0
	if(SDIO_REG0X9_ADDR & (1<<4)){
		SDIO_REG0X9_ADDR  = (1<<4);	//SD_DATA_WR_END_INT
		os_printf("write end \r\n");
		s_write_end = 1;
	}

	if(SDIO_REG0X9_ADDR & (1<<3)){
		SDIO_REG0X9_ADDR  = (1<<3);	//TX_FIFO_NEED_WRITE
	}

	if(SDIO_REG0X9_ADDR & (1<<7)){
		SDIO_REG0X9_ADDR  = (1<<7);	//TX_FIFO_NEED_WRITE
	}	

	if(SDIO_REG0X9_ADDR & (1<<9)){	//TX_FIFO_EMPT
		SDIO_REG0XA_ADDR  &= ~(1<<9);	//TX_FIFO_EMPT_MASK
	}	
#endif

}

static bk_err_t sdio_slave_hw_init(void)
{
	bk_err_t ret = BK_OK;

	sdio_hal_set_host_slave_mode(SDIO_SLAVE_MODE);

	//clock
	sys_hal_set_sdio_clk_en(true);
	sys_hal_set_sdio_clk_sel((uint32_t)SDIO_CLK_XTL);
	sys_hal_set_sdio_clk_div((uint32_t)SDIO_CLK_DIV_1);

	//pin
	ret = sdio_gpio_init();
	if(ret != BK_OK)
	{
		SDIO_LOG_ERR("gpio init");
		return ret;
	}
#if CONFIG_GPIO_NOTIFY_TRANSACTION_EN
	sdio_gpio_notify_transaction_init(GPIO_SDIO_NOTIFY_HOST, GPIO_SDIO_HOST_WAKEUP_SLAVE);
#endif

	//dma
#if CONFIG_SDIO_GDMA_EN
	ret = sdio_dma_init();
	if(ret != BK_OK)
	{
		SDIO_LOG_ERR("dma init");
		return ret;
	}
#endif

	//interrupt
	bk_int_isr_register(INT_SRC_SDIO, sdio_slave_isr, NULL);
#if	(defined CONFIG_DUAL_CORE && defined CONFIG_SLAVE_CORE)
	sys_driver_set_cpu1_sdio_int_en(true);
#else
	sys_driver_set_cpu0_sdio_int_en(true);
#endif

	return ret;
}

/**
 * @brief	  Init the sdio driver as slave device.
 *
 * This API init the sdio driver as slave device.
 *	 - Init SDIO slave SW: Thread, Semaphore, Event.
 *	 - Init SDIO slave HW: GPIO,CLOCK,DMA,IRQ ...
 *
 * @attention 1. This API should be called before any other SDIO SLAVE APIs.
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t bk_sdio_slave_driver_init(void)
{
	bk_err_t ret = BK_OK;
	uint32_t int_level = 0;

	SDIO_LOG_DEBUG_FUNCTION_ENTRY();

	int_level = rtos_disable_int();

	if(s_sdio_driver_is_init) 
	{
		rtos_enable_int(int_level);
		SDIO_LOG_INFO("has inited");
		return BK_OK;
	}

	ret = sdio_sw_init();
	if(ret != BK_OK)
	{
		SDIO_LOG_ERR("sw init fail");
		goto err_exit;
	}

	ret = sdio_slave_hw_init();
	if(ret != BK_OK)
	{
		SDIO_LOG_ERR("sw init fail");
		goto err_exit;
	}

	s_sdio_driver_is_init = true;

	bk_sdio_driver_dump_sdio_regs();

	SDIO_LOG_DEBUG_FUNCTION_EXIT();

	//add this to avoid modify err_exit involve issue.
	rtos_enable_int(int_level);
	return ret;

err_exit:
	rtos_enable_int(int_level);
	return ret;
}

static bk_err_t sdio_sw_deinit(void)
{
	bk_err_t ret = BK_OK;

	if(s_sdio_msg_que)
	{
		rtos_deinit_queue(&s_sdio_msg_que);
		if (BK_OK != ret)
		{
			SDIO_LOG_ERR("deinit queue ret=%d", ret);
			return ret;
		}

		s_sdio_msg_que = NULL;
	}

	if(s_sdio_thread)
	{
		ret = rtos_delete_thread(&s_sdio_thread);
		if (BK_OK != ret)
		{
			SDIO_LOG_ERR("delete thread ret=%d", ret);
			return ret;
		}

		s_sdio_msg_que = NULL;
	}

	return BK_OK;
}

static bk_err_t sdio_hw_deinit(void)
{
	bk_err_t ret = BK_OK;

	sdio_hal_set_host_slave_mode(SDIO_HOST_MODE);

	//clock
	sys_hal_set_sdio_clk_en(false);
	sys_hal_set_sdio_clk_sel((uint32_t)SDIO_CLK_XTL);
	sys_hal_set_sdio_clk_div((uint32_t)SDIO_CLK_DIV_1);

	//pin
	ret = sdio_gpio_deinit();
	if(ret != BK_OK)
	{
		SDIO_LOG_ERR("gpio deinit");
		return ret;
	}

	//TODO:
#if CONFIG_GPIO_NOTIFY_TRANSACTION_EN
	sdio_gpio_notify_transaction_deinit(GPIO_SDIO_NOTIFY_HOST, GPIO_SDIO_HOST_WAKEUP_SLAVE);
#endif

	//dma
#if CONFIG_SDIO_GDMA_EN
	ret = sdio_dma_deinit();
	if(ret != BK_OK)
	{
		SDIO_LOG_ERR("dma deinit");
		return ret;
	}
#endif

	//interrupt
	bk_int_isr_unregister(INT_SRC_SDIO);
#if	(defined CONFIG_DUAL_CORE && defined CONFIG_SLAVE_CORE)
	sys_driver_set_cpu1_sdio_int_en(false);
#else
	sys_driver_set_cpu0_sdio_int_en(false);
#endif

	return ret;
}


/**
 * @brief	  Deinit the sdio driver as slave device.
 *
 * This API deinit the sdio driver as slave device.
 *	 - Deinit SDIO slave HW: GPIO,DMA,IRQ ...
 *	 - Deinit SDIO slave SW: Thread, Queue...
 *
 * @attention 1. This API should be called after any other SDIO SLAVE APIs.
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t sdio_slave_driver_deinit(void)
{
	bk_err_t ret = BK_OK;
	uint32_t int_level = 0;

	SDIO_LOG_DEBUG_FUNCTION_ENTRY();

	int_level = rtos_disable_int();

	ret = sdio_sw_deinit();
	if (BK_OK != ret)
	{
		SDIO_LOG_ERR("sw deinit ret=%d", ret);
		rtos_enable_int(int_level);
		return ret;
	}

	ret = sdio_hw_deinit();
	if (BK_OK != ret)
	{
		SDIO_LOG_ERR("hw deinit ret=%d", ret);
		rtos_enable_int(int_level);
		return ret;
	}

	bk_sdio_driver_dump_sdio_regs();

	SDIO_LOG_DEBUG_FUNCTION_EXIT();

	rtos_enable_int(int_level);
	return ret;
}

#endif

