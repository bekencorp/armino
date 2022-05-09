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

#pragma once

#include <soc/soc.h>
#include "hal_port.h"
#include <driver/hal/hal_gpio_types.h>
#include "i2c_hw.h"
#include "icu_hw.h"
#include "icu_ll.h"
#include "power_ll.h"

#ifdef __cplusplus
extern "C" {
#endif

#define I2C_LL_REG_BASE(_i2c_unit_id)    (SOC_I2C0_REG_BASE + _i2c_unit_id * 0x300)

#define I2C0_LL_SCL_PIN    GPIO_20
#define I2C0_LL_SDA_PIN    GPIO_21

#define I2C1_LL_SCL_PIN    GPIO_0
#define I2C1_LL_SDA_PIN    GPIO_1

static inline void i2c_ll_init(i2c_hw_t *hw)
{
	hw->i2c0_hw = (i2c0_hw_t *)I2C_LL_REG_BASE(0);
	hw->i2c1_hw = (i2c1_hw_t *)I2C_LL_REG_BASE(1);
	hw->i2c0_hw->sm_bus_cfg.v = 0;
	hw->i2c1_hw->sm_bus_cfg.v = 0;
	hw->i2c1_hw->sm_bus_status.v = 0;
}

static inline void i2c_ll_enable(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_0) {
		hw->i2c0_hw->sm_bus_cfg.en = 1;
	} else {
		hw->i2c1_hw->sm_bus_cfg.en = 1;
	}
}

static inline void i2c_ll_disable(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_0) {
		hw->i2c0_hw->sm_bus_cfg.en = 0;
	} else {
		hw->i2c1_hw->sm_bus_cfg.en = 0;
	}
}

static inline void i2c_ll_set_pin(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_1) {
		icu_hw_t *icu_hw = (icu_hw_t *)ICU_LL_REG_BASE();
		power_ll_pwr_down_uart(icu_hw, 1);
	}
}

static inline void i2c_ll_set_idle_detect_threshold(i2c_hw_t *hw, i2c_id_t id, uint32_t threshold)
{
	if (id == I2C_ID_1) {
		hw->i2c1_hw->sm_bus_cfg.idle_cr = threshold;
	}
}

static inline void i2c_ll_set_scl_timeout_threshold(i2c_hw_t *hw, i2c_id_t id, uint32_t threshold)
{
	if (id == I2C_ID_1) {
		hw->i2c1_hw->sm_bus_cfg.scl_cr = threshold;
	}
}

static inline void i2c_ll_set_clk_src(i2c_hw_t *hw, i2c_id_t id, uint32_t clk_src)
{
	if (id == I2C_ID_1) {
		hw->i2c1_hw->sm_bus_cfg.clk_src = clk_src;
	}
}

static inline void i2c_ll_enable_scl_timeout(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_1) {
		hw->i2c1_hw->sm_bus_cfg.timeout_en = 1;
	}
}

static inline void i2c_ll_disable_scl_timeout(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_1) {
		hw->i2c1_hw->sm_bus_cfg.timeout_en = 0;
	}
}

static inline void i2c_ll_enable_idle_det(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_1) {
		hw->i2c1_hw->sm_bus_cfg.idle_det_en = 1;
	}
}

static inline void i2c_ll_disable_idle_det(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_1) {
		hw->i2c1_hw->sm_bus_cfg.idle_det_en = 0;
	}
}

static inline void i2c_ll_enable_slave(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_1) {
		hw->i2c1_hw->sm_bus_cfg.inh = 0;
	}
}

static inline void i2c_ll_disable_slave(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_1) {
		hw->i2c1_hw->sm_bus_cfg.inh = 1;
	}
}

static inline void i2c_ll_set_freq_div(i2c_hw_t *hw, i2c_id_t id, uint32_t freq_div)
{
	if (id == I2C_ID_0) {
		hw->i2c0_hw->sm_bus_cfg.freq_div = freq_div & I2C_F_FREQ_DIV_M;
	} else {
		hw->i2c1_hw->sm_bus_cfg.freq_div = freq_div & I2C_F_FREQ_DIV_M;
	}
}

static inline void i2c_ll_set_slave_addr(i2c_hw_t *hw, i2c_id_t id, uint32_t slave_addr)
{
	if (id == I2C_ID_1) {
		hw->i2c1_hw->sm_bus_cfg.slave_addr = slave_addr & I2C_F_SLAVE_ADDR_M;
	}
}

static inline void i2c_ll_reset_config_to_default(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_0) {
		hw->i2c0_hw->sm_bus_cfg.v = 0;
	} else {
		hw->i2c1_hw->sm_bus_cfg.v = 0;
	}
}

static inline void i2c_ll_enable_start(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_0) {
		hw->i2c0_hw->sm_bus_cfg.start = 1;
	} else {
		hw->i2c1_hw->sm_bus_status.start = 1;
	}
}

static inline void i2c_ll_disable_start(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_0) {
		hw->i2c0_hw->sm_bus_cfg.start = 0;
	} else {
		hw->i2c1_hw->sm_bus_status.start = 0;
	}
}

static inline bool i2c_ll_is_start(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_0) {
		return !!(hw->i2c0_hw->sm_bus_cfg.start);
	} else {
		return !!(hw->i2c1_hw->sm_bus_status.start);
	}
}

static inline bool i2c_ll_is_start_triggered(i2c_hw_t *hw, i2c_id_t id, uint32_t int_status)
{
	if (id == I2C_ID_0) {
		return int_status & BIT(1);
	} else {
		return int_status & BIT(10);
	}
}

static inline void i2c_ll_enable_stop(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_0) {
		hw->i2c0_hw->sm_bus_cfg.stop = 1;
	} else {
		/* sm_int and stop (bit[0] and bit[9]) must set together,
		 * otherwize i2c stop will not work.
		 * e.g.
		 * case 1: i2c stop not work
		 * hw->i2c1_hw->int_status.stop = 1;
		 * or REG_SET_BIT(I2C1_R_INT_STAUS, BIT(9));
		 *
		 * case 2: i2c stop not work
		 * hw->i2c1_hw->int_status.stop = 1;
		 * hw->i2c1_hw->int_status.sm_int = 0;
		 *
		 * case 3: i2c stop not work
		 * hw->i2c1_hw->int_status.sm_int = 0;
		 * hw->i2c1_hw->int_status.stop = 1;
 		 */
	}
}

static inline void i2c_ll_disable_stop(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_0) {
		hw->i2c0_hw->sm_bus_cfg.stop = 0;
	} else {
		hw->i2c1_hw->sm_bus_status.stop = 0;
	}
}

static inline bool i2c_ll_is_stop_triggered(i2c_hw_t *hw, i2c_id_t id, uint32_t int_status)
{
	if (id == I2C_ID_0) {
		return int_status & BIT(2);
	} else {
		return int_status & BIT(9);
	}
}

static inline void i2c_ll_set_write_int_mode(i2c_hw_t *hw, i2c_id_t id, i2c_fifo_int_level_t int_mode)
{
	if (id == I2C_ID_0) {
		return;
	}

	switch (int_mode) {
	case I2C_FIFO_INT_LEVEL_1:
		hw->i2c1_hw->sm_bus_status.int_mode = 0;
		break;
	case I2C_FIFO_INT_LEVEL_4:
		hw->i2c1_hw->sm_bus_status.int_mode = 1;
		break;
	case I2C_FIFO_INT_LEVEL_8:
		hw->i2c1_hw->sm_bus_status.int_mode = 2;
		break;
	case I2C_FIFO_INT_LEVEL_12:
		hw->i2c1_hw->sm_bus_status.int_mode = 3;
		break;
	default:
		break;
	}
}

static inline void i2c_ll_set_read_int_mode(i2c_hw_t *hw, i2c_id_t id, i2c_fifo_int_level_t int_mode)
{
	if (id == I2C_ID_0) {
		return;
	}

	switch (int_mode) {
	case I2C_FIFO_INT_LEVEL_1:
		hw->i2c1_hw->sm_bus_status.int_mode = 3;
		break;
	case I2C_FIFO_INT_LEVEL_4:
		hw->i2c1_hw->sm_bus_status.int_mode = 2;
		break;
	case I2C_FIFO_INT_LEVEL_8:
		hw->i2c1_hw->sm_bus_status.int_mode = 1;
		break;
	case I2C_FIFO_INT_LEVEL_12:
		hw->i2c1_hw->sm_bus_status.int_mode = 0;
		break;
	default:
		break;
	}
}

static inline uint32_t i2c_ll_get_write_empty_fifo_num(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_0) {
		return 0;
	}

	switch (hw->i2c1_hw->sm_bus_status.int_mode) {
	case 0x00:
		return 16;
	case 0x01:
		return 12;
	case 0x02:
		return 8;
	case 0x03:
		return 4;
	default:
		return 0;
	}
}

static inline uint32_t i2c_ll_get_read_fifo_num(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_0) {
		return 0;
	}

	switch (hw->i2c1_hw->sm_bus_status.int_mode)
	{
	case 0x00:
		return 12;
	case 0x01:
		return 8;
	case 0x02:
		return 4;
	case 0x03:
		return 1;
	default:
		return 0;
	}
}

static inline bool i2c_ll_is_busy(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_0) {
		return !!(hw->i2c0_hw->sm_bus_cfg.busy);
	} else {
		return !!(hw->i2c1_hw->sm_bus_status.busy);
	}
}

static inline bool i2c_ll_is_addr_matched(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_1) {
		return !!(hw->i2c1_hw->sm_bus_status.addr_match);
	} else {
		return true;
	}
}

static inline bool i2c_ll_is_tx_mode(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_1) {
		return !!(hw->i2c1_hw->sm_bus_status.tx_mode);
	} else {
		return true;
	}
}

static inline bool i2c_ll_is_rx_mode(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_1) {
		return !(hw->i2c1_hw->sm_bus_status.tx_mode);
	} else {
		return true;
	}
}

static inline uint32_t i2c_ll_get_interrupt_status(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_0) {
		return hw->i2c0_hw->sm_bus_cfg.v;
	} else {
		return hw->i2c1_hw->sm_bus_status.v;
	}
}

static inline void i2c_ll_clear_interrupt_status(i2c_hw_t *hw, i2c_id_t id, uint32_t int_status)
{
	if (id == I2C_ID_0) {
		if (int_status & BIT(1)) {
			hw->i2c0_hw->sm_bus_cfg.start = 0;
		}
		if (int_status & BIT(16)) {
			hw->i2c0_hw->sm_bus_cfg.sm_int = 0;
		}
	} else {
		if (int_status & I2C1_F_SM_INT) {
			int_status &= ~I2C1_F_SM_INT; /* clear sm_bus_int */
		}
		REG_WRITE(I2C1_R_INT_STAUS, int_status);
	}
}

static inline bool i2c_ll_is_sm_int_triggered(i2c_hw_t *hw, i2c_id_t id, uint32_t int_status)
{
	if (id == I2C_ID_0) {
		return !!(int_status & BIT(16));
	} else {
		return !!(int_status & BIT(0));
	}
}

static inline bool i2c_ll_is_scl_timeout_triggered(i2c_hw_t *hw, i2c_id_t id, uint32_t int_status)
{
	if (id == I2C_ID_1) {
		return !!(int_status & BIT(1));
	} else {
		return false;
	}
}

static inline bool i2c_ll_is_arb_lost_triggered(i2c_hw_t *hw, i2c_id_t id, uint32_t int_status)
{
	if (id == I2C_ID_1) {
		return !!(int_status & BIT(3));
	} else {
		return false;
	}
}

static inline bool i2c_ll_is_rx_ack_triggered(i2c_hw_t *hw, i2c_id_t id, uint32_t int_status)
{
	if (id == I2C_ID_0) {
		return !!(int_status & BIT(17));
	} else {
		return !!(int_status & BIT(8));
	}
}

static inline bool i2c_ll_is_tx_ack_triggered(i2c_hw_t *hw, i2c_id_t id, uint32_t int_status)
{
	if (id == I2C_ID_0) {
		return !!(int_status & BIT(3));
	} else {
		return !!(int_status & BIT(8));
	}
}

static inline void i2c_ll_tx_ack(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_0) {
		hw->i2c0_hw->sm_bus_cfg.tx_ack = 1;
	}
}

static inline void i2c_ll_tx_non_ack(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_0) {
		hw->i2c0_hw->sm_bus_cfg.tx_ack = 0;
	} else {
		hw->i2c1_hw->sm_bus_status.ack = 0;
	}
}

static inline void i2c_ll_set_tx_mode(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_0) {
		hw->i2c0_hw->sm_bus_cfg.op_mode = 1;
	}
}

static inline void i2c_ll_set_rx_mode(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_0) {
		hw->i2c0_hw->sm_bus_cfg.op_mode = 0;
	}
}

static inline void i2c_ll_write_byte(i2c_hw_t *hw, i2c_id_t id, uint32_t data)
{
	if (id == I2C_ID_0) {
		hw->i2c0_hw->sm_bus_data.data = data & I2C_F_DATA_M;
	} else {
		hw->i2c1_hw->sm_bus_data.data = data & I2C_F_DATA_M;
	}
}

static inline uint8_t i2c_ll_read_byte(i2c_hw_t *hw, i2c_id_t id)
{
	if (id == I2C_ID_0) {
		return hw->i2c0_hw->sm_bus_data.data;
	} else {
		return hw->i2c1_hw->sm_bus_data.data;
	}
}

#ifdef __cplusplus
}
#endif

