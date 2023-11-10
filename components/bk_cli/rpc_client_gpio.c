// Copyright 2020-2022 Beken
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

#include <stdio.h>
#include <string.h>

#include <os/os.h>
#include "mb_ipc_cmd.h"
#include <driver/gpio.h>

#define MOD_TAG		"RPC"

#define GPIO_API_IDX(member)			FIELD_IDX(gpio_func_tbl_t, member)

typedef struct
{
	rpc_call_hdr_t	call_hdr;
	u32				gpio_id;
} rpc_gpio_call_t;

typedef struct
{
	rpc_call_hdr_t	call_hdr;
	u32		ret_val;			/* api_ret_data_t */
} rpc_gpio_ret_t;

typedef struct
{
	void *	gpio_enable_output;
	void *	gpio_disable_output;
	void *	gpio_enable_input;
	void *	gpio_disable_input;
	void *	gpio_enable_pull;
	void *	gpio_disable_pull;
	void *	gpio_pull_up;
	void *	gpio_pull_down;
	void *	gpio_set_output_high;
	void *	gpio_set_output_low;
	void *	gpio_get_input;
	void *	gpio_set_config;
	void *	gpio_enable_interrupt;
	void *	gpio_disable_interrupt;
	void *	gpio_set_interrupt_type;
} gpio_func_tbl_t;


#if CONFIG_SLAVE_CORE

/**    ============================      gpio client    ============================   **/

//bk_err_t bk_gpio_enable_output(gpio_id_t gpio_id)  // should used this prototype.
bk_err_t bk_gpio_enable_output_rpc(gpio_id_t gpio_id)
{
	rpc_gpio_call_t	gpio_rpc;

	gpio_rpc.call_hdr.mod_id = RPC_MOD_GPIO;
	gpio_rpc.call_hdr.api_id = GPIO_API_IDX(gpio_enable_output);
	gpio_rpc.call_hdr.ctrl = RPC_CTRL_NO_RETURN;
	gpio_rpc.call_hdr.data_len = sizeof(gpio_rpc) - FIELD_OFFSET(rpc_call_def_t, call_param[0]);
	gpio_rpc.gpio_id = gpio_id;

	return rpc_client_call((rpc_call_def_t *)&gpio_rpc, sizeof(gpio_rpc), NULL, 0);
}

//bk_err_t bk_gpio_disable_output(gpio_id_t gpio_id)
bk_err_t bk_gpio_disable_output_rpc(gpio_id_t gpio_id)
{
	rpc_gpio_call_t	gpio_rpc;

	gpio_rpc.call_hdr.mod_id = RPC_MOD_GPIO;
	gpio_rpc.call_hdr.api_id = GPIO_API_IDX(gpio_disable_output);
	gpio_rpc.call_hdr.ctrl = RPC_CTRL_NO_RETURN;
	gpio_rpc.call_hdr.data_len = sizeof(gpio_rpc) - FIELD_OFFSET(rpc_call_def_t, call_param[0]);
	gpio_rpc.gpio_id = gpio_id;

	return rpc_client_call((rpc_call_def_t *)&gpio_rpc, sizeof(gpio_rpc), NULL, 0);
}

//bk_err_t bk_gpio_set_output_high(gpio_id_t gpio_id)
bk_err_t bk_gpio_set_output_high_rpc(gpio_id_t gpio_id)
{
	rpc_gpio_call_t	gpio_rpc;

	gpio_rpc.call_hdr.mod_id = RPC_MOD_GPIO;
	gpio_rpc.call_hdr.api_id = GPIO_API_IDX(gpio_set_output_high);
	gpio_rpc.call_hdr.ctrl = RPC_CTRL_NO_RETURN;
	gpio_rpc.call_hdr.data_len = sizeof(gpio_rpc) - FIELD_OFFSET(rpc_call_def_t, call_param[0]);
	gpio_rpc.gpio_id = gpio_id;

	return rpc_client_call((rpc_call_def_t *)&gpio_rpc, sizeof(gpio_rpc), NULL, 0);
}

//bk_err_t bk_gpio_set_output_low(gpio_id_t gpio_id)
bk_err_t bk_gpio_set_output_low_rpc(gpio_id_t gpio_id)
{
	rpc_gpio_call_t	gpio_rpc;

	gpio_rpc.call_hdr.mod_id = RPC_MOD_GPIO;
	gpio_rpc.call_hdr.api_id = GPIO_API_IDX(gpio_set_output_low);
	gpio_rpc.call_hdr.ctrl = RPC_CTRL_NO_RETURN;
	gpio_rpc.call_hdr.data_len = sizeof(gpio_rpc) - FIELD_OFFSET(rpc_call_def_t, call_param[0]);
	gpio_rpc.gpio_id = gpio_id;

	return rpc_client_call((rpc_call_def_t *)&gpio_rpc, sizeof(gpio_rpc), NULL, 0);
}

/**    ============================    gpio client end  ============================   **/

#endif

#if CONFIG_MASTER_CORE

/**    ============================      gpio server    ============================   **/

static const gpio_func_tbl_t rpc_gpio_api_tbl = 
{
	.gpio_enable_output = bk_gpio_enable_output,
	.gpio_disable_output = bk_gpio_disable_output,
	.gpio_enable_input = bk_gpio_enable_input,
	.gpio_disable_input = bk_gpio_disable_input,
	.gpio_enable_pull = bk_gpio_enable_pull,
	.gpio_disable_pull = bk_gpio_disable_pull,
	.gpio_pull_up = bk_gpio_pull_up,
	.gpio_pull_down = bk_gpio_pull_down,
	.gpio_set_output_high = bk_gpio_set_output_high,
	.gpio_set_output_low = bk_gpio_set_output_low,
	.gpio_get_input = bk_gpio_get_input,
	.gpio_set_config = bk_gpio_set_config,
	.gpio_enable_interrupt = bk_gpio_enable_interrupt,
	.gpio_disable_interrupt = bk_gpio_disable_interrupt,
	.gpio_set_interrupt_type = bk_gpio_set_interrupt_type,
};

typedef int (*gpio_func_t)();

int bk_gpio_api_svr(rpc_call_def_t * call_buf)
{
	rpc_gpio_call_t * gpio_param = (rpc_gpio_call_t *)call_buf;
	gpio_func_t *     func_list = (gpio_func_t *)&rpc_gpio_api_tbl;

	if(call_buf->call_hdr.api_id >= sizeof(gpio_func_tbl_t) / sizeof(void *))
		return 0;

	switch(call_buf->call_hdr.api_id)
	{
		case GPIO_API_IDX(gpio_enable_output):
		case GPIO_API_IDX(gpio_disable_output):
		case GPIO_API_IDX(gpio_enable_input):
		case GPIO_API_IDX(gpio_disable_input):
		case GPIO_API_IDX(gpio_enable_pull):
		case GPIO_API_IDX(gpio_disable_pull):
		case GPIO_API_IDX(gpio_set_output_high):
		case GPIO_API_IDX(gpio_set_output_low):
			{
				rpc_gpio_ret_t	gpio_ret;

				gpio_ret.call_hdr.call_id = call_buf->call_hdr.call_id;
				gpio_ret.call_hdr.data_len = sizeof(gpio_ret) - sizeof(gpio_ret.call_hdr);
				gpio_ret.ret_val = func_list[call_buf->call_hdr.api_id](gpio_param->gpio_id);

				BK_LOGI(MOD_TAG, "gpio api: %d, id: %d\r\n", call_buf->call_hdr.api_id, gpio_param->gpio_id);

				rpc_server_rsp((rpc_ret_def_t *)&gpio_ret, sizeof(gpio_ret));
			}
			break;

		case GPIO_API_IDX(gpio_set_config):
			{
				//bk_gpio_set_config(gpio_param->gpio_id);
				return 0;
			}
			break;

		default:
			return 0;
			break;
	}

	return 1;
}

/**    ============================    gpio server end  ============================   **/

#endif

#include <os/os.h>
#include "mb_ipc_cmd.h"
#include <driver/dma.h>

//#define MOD_TAG		"RPC"

/**    ============================      dma client    ============================   **/

#define DMA_API_IDX(member)				FIELD_IDX(dma_func_tbl_t, member)

typedef struct
{
	void *	dma_init;
	void *	dma_deinit;
	void *	dma_start;
	void *	dma_stop;
	void *	dma_write;
	void *	dma_read;
	void *	dma_enable_finish_interrupt;
	void *	dma_disable_finish_interrupt;
	void *	dma_enable_half_finish_interrupt;
	void *	dma_disable_half_finish_interrupt;
	void *	dma_set_transfer_len;
	void *	dma_set_src_addr;
	void *	dma_set_dest_addr;
	void *	dma_get_remain_len;
	void *	dma_set_src_pause_addr;
	void *	dma_set_dst_pause_addr;
	void *	dma_get_src_read_addr;
	void *	dma_get_dest_write_addr;
	void *	dma_memcpy;
} dma_func_tbl_t;

/**    ============================    dma client end  ============================   **/

/**    ============================      dma server    ============================   **/

static const dma_func_tbl_t rpc_dma_api_tbl = 
{
	.dma_init = bk_dma_init,
	.dma_deinit = bk_dma_deinit,
	.dma_start = bk_dma_start,
	.dma_stop = bk_dma_stop,
	.dma_write = bk_dma_write,
	.dma_read = bk_dma_read,
	.dma_enable_finish_interrupt = bk_dma_enable_finish_interrupt,
	.dma_disable_finish_interrupt = bk_dma_disable_finish_interrupt,
	.dma_enable_half_finish_interrupt = bk_dma_enable_half_finish_interrupt,
	.dma_disable_half_finish_interrupt = bk_dma_disable_half_finish_interrupt,
	.dma_set_transfer_len = bk_dma_set_transfer_len,
	.dma_set_src_addr = bk_dma_set_src_addr,
	.dma_set_dest_addr = bk_dma_set_dest_addr,
	.dma_get_remain_len = bk_dma_get_remain_len,
/*	.dma_set_src_pause_addr = dma_set_src_pause_addr,
	.dma_set_dst_pause_addr = dma_set_dst_pause_addr,
	.dma_get_src_read_addr = dma_get_src_read_addr,
	.dma_get_dest_write_addr = dma_get_dest_write_addr,
	.dma_memcpy = dma_memcpy,*/
};

typedef int (*dma_func_t)();

int bk_dma_api_svr(rpc_call_def_t * call_buf)
{
	if(call_buf->call_hdr.api_id >= sizeof(rpc_dma_api_tbl) / sizeof(void *))
		return 0;

	return 0;
}

/**    ============================    dma server end  ============================   **/

