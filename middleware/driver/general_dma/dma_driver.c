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
#include <os/mem.h>
#include "icu_driver.h"
#include "dma_driver.h"
#include "dma_hal.h"
#include <driver/dma.h>
#include "bk_general_dma.h"
#include <driver/int.h>
#include "sys_driver.h"
#if CONFIG_SLAVE_CORE
#include "mb_ipc_cmd.h"
#endif

#define DMA_CPU_MASTER		0
#define DMA_CPU_SLAVE1		1

static void dma_isr(void) __SECTION(".itcm");

typedef struct {
    dma_hal_t hal;
    uint32_t id_init_bits;
} dma_driver_t;

static dma_driver_t s_dma = {0};
static dma_isr_t s_dma_finish_isr[SOC_DMA_CHAN_NUM_PER_UNIT] = {NULL};
static dma_isr_t s_dma_half_finish_isr[SOC_DMA_CHAN_NUM_PER_UNIT] = {NULL};
static bool s_dma_driver_is_init = false;
static dma_chnl_pool_t s_dma_chnl_pool = {0};

#define DMA_MEMCPY_CHANNEL    DMA_ID_0

#define DMA_RETURN_ON_NOT_INIT() do {\
        if (!s_dma_driver_is_init) {\
            return BK_ERR_DMA_NOT_INIT;\
        }\
    } while(0)

#define DMA_RETURN_ON_INVALID_ID(id) do {\
        if ((id) >= SOC_DMA_CHAN_NUM_PER_UNIT) {\
            return BK_ERR_DMA_ID;\
        }\
    } while(0)

#define DMA_RETURN_ON_ID_NOT_INIT(id) do {\
        if (!(s_dma.id_init_bits & BIT((id)))) {\
            return BK_ERR_DMA_ID_NOT_INIT;\
        }\
    } while(0)

#define DMA_LOG_ON_ID_IS_STARTED(id) do {\
        if (dma_hal_is_id_started(&s_dma.hal, (id))) {\
            DMA_LOGW("%s dma%d is working.\n", __func__, id);\
        }\
    } while(0)

#define DMA_RETURN_ON_INVALID_ADDR(start_addr, end_addr) do {\
        if ((0 < (end_addr)) && ((end_addr) < (start_addr))) {\
            return BK_ERR_DMA_INVALID_ADDR;\
        }\
    } while(0)

static void dma_id_init_common(dma_id_t id)
{
    s_dma.id_init_bits |= BIT(id);
}

static void dma_id_deinit_common(dma_id_t id)
{
    s_dma.id_init_bits &= ~BIT(id);
    dma_hal_stop_common(&s_dma.hal, id);
    dma_hal_reset_config_to_default(&s_dma.hal, id);
}

static void dma_id_enable_interrupt_common(dma_id_t id)
{
#if (CONFIG_SYSTEM_CTRL)
	sys_drv_int_enable(GDMA_INTERRUPT_CTRL_BIT);
#else
    icu_enable_dma_interrupt();
#endif
}

/* used internally, called in context of interrupt disabled. */
u8 dma_chnl_alloc(u32 user_id)
{
	u8		chnl_id;

	for(chnl_id = 0; chnl_id < DMA_ID_MAX; chnl_id++)
	{
		if((s_dma_chnl_pool.chnl_bitmap & (0x01 << chnl_id)) == 0)
		{
			s_dma_chnl_pool.chnl_bitmap |= (0x01 << chnl_id);
			s_dma_chnl_pool.chnl_user[chnl_id] = user_id;
			break;
		}
	}

	return chnl_id;
}

/* used internally, called in context of interrupt disabled. */
bk_err_t dma_chnl_free(u32 user_id, dma_id_t chnl_id)
{
	if( chnl_id >= DMA_ID_MAX )
		return BK_ERR_DMA_ID;
	
	if( s_dma_chnl_pool.chnl_user[chnl_id] != user_id )
		return BK_ERR_PARAM;
	
	s_dma_chnl_pool.chnl_bitmap &= ~(0x01 << chnl_id);
	s_dma_chnl_pool.chnl_user[chnl_id] = -1;

	return BK_OK;
}

/* used internally. */
u32 dma_chnl_user(dma_id_t chnl_id)
{
	if( chnl_id >= DMA_ID_MAX )
		return -1;
	
	return s_dma_chnl_pool.chnl_user[chnl_id];
}

bk_err_t bk_dma_driver_init(void)
{
    if (s_dma_driver_is_init) {
        return BK_OK;
    }

	s_dma_chnl_pool.chnl_bitmap = 0;
	for(u8 i = 0; i < DMA_ID_MAX; i++)
	{
		s_dma_chnl_pool.chnl_user[i] = -1;
	}

    /* 1)intc_service_register
     * 2)init dma_finish_int handler, dma_half_finish_int handler
     * 3)disable dma_en (0~6), clear int status
     * 4)init dma_config
     */
    os_memset(&s_dma, 0, sizeof(s_dma));
    os_memset(&s_dma_finish_isr, 0, sizeof(s_dma_finish_isr));
    os_memset(&s_dma_half_finish_isr, 0, sizeof(s_dma_half_finish_isr));
    bk_int_isr_register(INT_SRC_GDMA, dma_isr, NULL);
    dma_hal_init(&s_dma.hal);

    s_dma_driver_is_init = true;

    return BK_OK;
}

bk_err_t bk_dma_driver_deinit(void)
{
    if (!s_dma_driver_is_init) {
        return BK_OK;
    }

    for (int id = 0; id < SOC_DMA_CHAN_NUM_PER_UNIT; id++) {
        dma_id_deinit_common(id);
    }

#if (CONFIG_SYSTEM_CTRL)
	sys_drv_int_disable(GDMA_INTERRUPT_CTRL_BIT);
#else
    icu_disable_dma_interrupt();
#endif

    s_dma_driver_is_init = false;

    return BK_OK;
}

dma_id_t bk_dma_alloc(u16 user_id)
{
    if (!s_dma_driver_is_init)
	{
        return DMA_ID_MAX;
    }

#if CONFIG_SLAVE_CORE

#define DMA_USER_CPU		DMA_CPU_SLAVE1

	u32		dma_user = ((DMA_USER_CPU) << 16) + (u32)user_id;

	return ipc_send_alloc_dma_chnl(dma_user);

#else

#define DMA_USER_CPU		DMA_CPU_MASTER

	u32  int_mask = rtos_disable_int();

	u32		dma_user = ((DMA_USER_CPU) << 16) + (u32)user_id;
	
	u8		chnl_id = dma_chnl_alloc(dma_user);

	rtos_enable_int(int_mask);

	return chnl_id;

#endif
}

bk_err_t bk_dma_free(u16 user_id, dma_id_t chnl_id)
{
    if (!s_dma_driver_is_init)
	{
        return BK_ERR_DMA_NOT_INIT;
    }

#if CONFIG_SLAVE_CORE

#define DMA_USER_CPU		DMA_CPU_SLAVE1

	u32		dma_user = ((DMA_USER_CPU) << 16) + (u32)user_id;

	return ipc_send_free_dma_chnl(dma_user, chnl_id);

#else

#define DMA_USER_CPU		DMA_CPU_MASTER

	u32  int_mask = rtos_disable_int();

	u32			dma_user = ((DMA_USER_CPU) << 16) + (u32)user_id;
	bk_err_t	ret_val = dma_chnl_free(dma_user, chnl_id);

	rtos_enable_int(int_mask);

	return ret_val;

#endif
}

u32 bk_dma_user(dma_id_t chnl_id)
{
    if (!s_dma_driver_is_init)
	{
        return -1;
    }

#if CONFIG_SLAVE_CORE
	return ipc_send_dma_chnl_user( (u8)chnl_id );
#else
	return dma_chnl_user(chnl_id);
#endif

}

bk_err_t bk_dma_init(dma_id_t id, const dma_config_t *config)
{
    DMA_RETURN_ON_NOT_INIT();
    BK_RETURN_ON_NULL(config);
    DMA_RETURN_ON_INVALID_ID(id);
    DMA_RETURN_ON_INVALID_ADDR(config->src.start_addr, config->src.end_addr);
    DMA_RETURN_ON_INVALID_ADDR(config->dst.start_addr, config->dst.end_addr);
    DMA_LOG_ON_ID_IS_STARTED(id);

    dma_id_init_common(id);
    return dma_hal_init_dma(&s_dma.hal, id, config);
}

bk_err_t bk_dma_deinit(dma_id_t id)
{
    DMA_RETURN_ON_INVALID_ID(id);
    dma_id_deinit_common(id);
    return BK_OK;
}

bk_err_t bk_dma_start(dma_id_t id)
{
    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_ID_NOT_INIT(id);
    dma_hal_start_common(&s_dma.hal, id);
    return BK_OK;
}

bk_err_t bk_dma_stop(dma_id_t id)
{
    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_ID_NOT_INIT(id);
    dma_hal_stop_common(&s_dma.hal, id);
    return BK_OK;
}

/* DTCM->peripheral
 */
bk_err_t bk_dma_write(dma_id_t id, const uint8_t *data, uint32_t size)
{
    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_ID_NOT_INIT(id);

    dma_hal_set_src_start_addr(&s_dma.hal, id, (uint32_t)data);
    dma_hal_set_src_loop_addr(&s_dma.hal, id, (uint32_t)data, (uint32_t)(data + size));
    dma_hal_set_transfer_len(&s_dma.hal, id, size);
    dma_hal_start_common(&s_dma.hal, id);

    return BK_OK;
}

/* peripheral->DTCM
 */
bk_err_t bk_dma_read(dma_id_t id, uint8_t *data, uint32_t size)
{
    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_ID_NOT_INIT(id);

    dma_hal_set_dest_start_addr(&s_dma.hal, id, (uint32_t)data);
    dma_hal_set_dest_loop_addr(&s_dma.hal, id, (uint32_t)data, (uint32_t)(data + size));
    dma_hal_set_transfer_len(&s_dma.hal, id, size);
    dma_hal_start_common(&s_dma.hal, id);

    return BK_OK;
}

bk_err_t bk_dma_enable_finish_interrupt(dma_id_t id)
{
    DMA_RETURN_ON_ID_NOT_INIT(id);
    DMA_RETURN_ON_INVALID_ID(id);
    dma_id_enable_interrupt_common(id);
    dma_hal_enable_finish_interrupt(&s_dma.hal, id);
    return BK_OK;
}

bk_err_t bk_dma_disable_finish_interrupt(dma_id_t id)
{
    DMA_RETURN_ON_ID_NOT_INIT(id);
    DMA_RETURN_ON_INVALID_ID(id);

    dma_hal_disable_finish_interrupt(&s_dma.hal, id);
    dma_hal_clear_finish_interrupt_status(&s_dma.hal, id);
    return BK_OK;
}

bk_err_t bk_dma_enable_half_finish_interrupt(dma_id_t id)
{
    DMA_RETURN_ON_ID_NOT_INIT(id);
    DMA_RETURN_ON_INVALID_ID(id);
    dma_id_enable_interrupt_common(id);
    dma_hal_enable_half_finish_interrupt(&s_dma.hal, id);
    return BK_OK;
}

bk_err_t bk_dma_disable_half_finish_interrupt(dma_id_t id)
{
    DMA_RETURN_ON_ID_NOT_INIT(id);
    DMA_RETURN_ON_INVALID_ID(id);

    dma_hal_disable_half_finish_interrupt(&s_dma.hal, id);
    dma_hal_clear_half_finish_interrupt_status(&s_dma.hal, id);
    return BK_OK;
}

bk_err_t bk_dma_register_isr(dma_id_t id, dma_isr_t half_finish_isr, dma_isr_t finish_isr)
{
    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);

    GLOBAL_INT_DECLARATION();
    GLOBAL_INT_DISABLE();
    s_dma_half_finish_isr[id] = half_finish_isr;
    s_dma_finish_isr[id] = finish_isr;
    GLOBAL_INT_RESTORE();

    return BK_OK;
}

bk_err_t bk_dma_set_transfer_len(dma_id_t id, uint32_t tran_len)
{
    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);
    dma_hal_set_transfer_len(&s_dma.hal, id, tran_len);
    return BK_OK;
}

bk_err_t bk_dma_set_src_addr(dma_id_t id, uint32_t start_addr, uint32_t end_addr)
{
    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);
    dma_hal_set_src_start_addr(&s_dma.hal, id, start_addr);
    dma_hal_set_src_loop_addr(&s_dma.hal, id, start_addr, end_addr);
    return BK_OK;
}

bk_err_t bk_dma_set_src_start_addr(dma_id_t id, uint32_t start_addr)
{
    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);
    dma_hal_set_src_start_addr(&s_dma.hal, id, start_addr);
    return BK_OK;
}

bk_err_t bk_dma_set_dest_addr(dma_id_t id, uint32_t start_addr, uint32_t end_addr)
{
    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);
    dma_hal_set_dest_start_addr(&s_dma.hal, id, start_addr);
    dma_hal_set_dest_loop_addr(&s_dma.hal, id, start_addr, end_addr);
    return BK_OK;
}

bk_err_t bk_dma_set_dest_start_addr(dma_id_t id, uint32_t start_addr)
{
    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);
    dma_hal_set_dest_start_addr(&s_dma.hal, id, start_addr);
    return BK_OK;
}

bk_err_t bk_dma_enable_src_addr_increase(dma_id_t id)
{
    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);
    dma_hal_enable_src_addr_inc(&s_dma.hal, id);
    return BK_OK;
}

bk_err_t bk_dma_disable_src_addr_increase(dma_id_t id)
{
    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);
    dma_hal_disable_src_addr_inc(&s_dma.hal, id);
    return BK_OK;
}

bk_err_t bk_dma_enable_src_addr_loop(dma_id_t id)
{
    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);
    dma_hal_enable_src_addr_loop(&s_dma.hal, id);
    return BK_OK;
}

bk_err_t bk_dma_disable_src_addr_loop(dma_id_t id)
{
    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);
    dma_hal_disable_src_addr_loop(&s_dma.hal, id);
    return BK_OK;
}

bk_err_t bk_dma_enable_dest_addr_increase(dma_id_t id)
{
    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);
    dma_hal_enable_dest_addr_inc(&s_dma.hal, id);
    return BK_OK;
}

bk_err_t bk_dma_disable_dest_addr_increase(dma_id_t id)
{
    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);
    dma_hal_disable_dest_addr_inc(&s_dma.hal, id);
    return BK_OK;
}

bk_err_t bk_dma_enable_dest_addr_loop(dma_id_t id)
{
    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);
    dma_hal_enable_dest_addr_loop(&s_dma.hal, id);
    return BK_OK;
}

bk_err_t bk_dma_disable_dest_addr_loop(dma_id_t id)
{
    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);
    dma_hal_disable_dest_addr_loop(&s_dma.hal, id);
    return BK_OK;
}

uint32_t bk_dma_get_remain_len(dma_id_t id)
{
    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_ID_NOT_INIT(id);
    return dma_hal_get_remain_len(&s_dma.hal, id);
}

bk_err_t dma_set_src_pause_addr(dma_id_t id, uint32_t addr)
{
    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);
    dma_hal_set_src_pause_addr(&s_dma.hal, id, addr);

    return BK_OK;
}

bk_err_t dma_set_dst_pause_addr(dma_id_t id, uint32_t addr)
{
    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);
    dma_hal_set_dest_pause_addr(&s_dma.hal, id, addr);

    return BK_OK;
}

uint32_t dma_get_src_read_addr(dma_id_t id)
{
    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);

    return dma_hal_get_src_read_addr(&s_dma.hal, id);
}

uint32_t dma_get_dest_write_addr(dma_id_t id)
{
    DMA_RETURN_ON_NOT_INIT();
    DMA_RETURN_ON_INVALID_ID(id);

    return dma_hal_get_dest_write_addr(&s_dma.hal, id);
}

bk_err_t dma_memcpy(void *out, const void *in, uint32_t len)
{
    GLOBAL_INT_DECLARATION();
    GLOBAL_INT_DISABLE();
    dma_config_t dma_config;
    os_memset(&dma_config, 0, sizeof(dma_config_t));
    dma_config.mode = DMA_WORK_MODE_SINGLE;
    dma_config.chan_prio = 0;
    dma_config.src.dev = DMA_DEV_DTCM;
    dma_config.src.width = DMA_DATA_WIDTH_32BITS;
    dma_config.src.addr_inc_en = DMA_ADDR_INC_ENABLE;
    dma_config.src.start_addr = (uint32_t)in;
    dma_config.src.end_addr = (uint32_t)(in + len);
    dma_config.dst.dev = DMA_DEV_DTCM;
    dma_config.dst.width = DMA_DATA_WIDTH_32BITS;
    dma_config.dst.addr_inc_en = DMA_ADDR_INC_ENABLE;
    dma_config.dst.start_addr = (uint32_t)out;
    dma_config.dst.end_addr = (uint32_t)(out + len);
    bk_dma_init(DMA_MEMCPY_CHANNEL, &dma_config);
    dma_hal_set_transfer_len(&s_dma.hal, DMA_MEMCPY_CHANNEL, len);
    dma_hal_start_common(&s_dma.hal, DMA_MEMCPY_CHANNEL);
    BK_WHILE (dma_hal_get_enable_status(&s_dma.hal, DMA_MEMCPY_CHANNEL));
    GLOBAL_INT_RESTORE();
    return BK_OK;
}

static void dma_isr(void)
{
    dma_hal_t *hal = &s_dma.hal;
    for (int id = 0; id < SOC_DMA_CHAN_NUM_PER_UNIT; id++) {
        if (dma_hal_is_half_finish_interrupt_triggered(hal, id)) {
            dma_hal_clear_half_finish_interrupt_status(hal, id);
            if (s_dma_half_finish_isr[id]) {
                s_dma_half_finish_isr[id](id);
            }
        }
        if (dma_hal_is_finish_interrupt_triggered(hal, id)) {
            dma_hal_clear_finish_interrupt_status(hal, id);
            if (s_dma_finish_isr[id]) {
                s_dma_finish_isr[id](id);
            }
        }
   }
}

