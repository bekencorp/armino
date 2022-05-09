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
#include "spi_hw.h"
#include <driver/hal/hal_spi_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SPI_LL_REG_BASE(_spi_unit_id)    (SOC_SPI_REG_BASE)
#define GPIO_R_18_CONFIG    (SOC_GPIO_REG_BASE + 18 * 0x4)

#define SPI0_LL_CSN_PIN     GPIO_15
#define SPI0_LL_SCK_PIN     GPIO_14
#define SPI0_LL_MOSI_PIN    GPIO_16
#define SPI0_LL_MISO_PIN    GPIO_17

//TODO init more
static inline void spi_ll_init(spi_hw_t *hw)
{
	hw->ctrl.tx_fifo_int_level = 3;
	hw->ctrl.rx_fifo_int_level = 3;
	hw->ctrl.nssmd = 0x3;
}

static inline void spi_ll_enable(spi_hw_t *hw)
{
	hw->ctrl.enable = 1;
}

static inline void spi_ll_disable(spi_hw_t *hw)
{
	hw->ctrl.enable = 0;
}

static inline void spi_ll_set_tx_fifo_int_level(spi_hw_t *hw, spi_fifo_int_level level)
{
	hw->ctrl.tx_fifo_int_level = level;
}

static inline void spi_ll_set_rx_fifo_int_level(spi_hw_t *hw, spi_fifo_int_level level)
{
	hw->ctrl.rx_fifo_int_level = level;
}

static inline uint32_t spi_ll_get_tx_fifo_int_level(spi_hw_t *hw)
{
	switch (hw->ctrl.tx_fifo_int_level) {
	case 0:
		return 1;
	case 1:
		return 4;
	case 2:
		return 8;
	case 3:
	default:
		return 12;
	}
}

static inline uint32_t spi_ll_get_rx_fifo_int_level(spi_hw_t *hw)
{
	switch (hw->ctrl.rx_fifo_int_level) {
	case 0:
		return 12;
	case 1:
		return 8;
	case 2:
		return 4;
	case 3:
	default:
		return 1;
	}
}

static inline void spi_ll_enable_tx_underflow_int(spi_hw_t *hw)
{
	hw->ctrl.tx_udf_int_en = 1;
}

static inline void spi_ll_disable_tx_underflow_int(spi_hw_t *hw)
{
	hw->ctrl.tx_udf_int_en = 0;
}

static inline void spi_ll_enable_rx_overflow_int(spi_hw_t *hw)
{
	hw->ctrl.rx_ovf_int_en = 1;
}

static inline void spi_ll_disable_rx_overflow_int(spi_hw_t *hw)
{
	hw->ctrl.rx_ovf_int_en = 0;
}

static inline void spi_ll_enable_tx_fifo_int(spi_hw_t *hw)
{
	hw->ctrl.tx_fifo_int_en = 1;
}

static inline void spi_ll_disable_tx_fifo_int(spi_hw_t *hw)
{
	hw->ctrl.tx_fifo_int_en = 0;
}

static inline void spi_ll_enable_rx_fifo_int(spi_hw_t *hw)
{
	hw->ctrl.rx_fifo_int_en = 1;
}

static inline void spi_ll_disable_rx_fifo_int(spi_hw_t *hw)
{
	hw->ctrl.rx_fifo_int_en = 0;
}

static inline void spi_ll_set_clk_div(spi_hw_t *hw, uint32_t clk_div)
{
	hw->ctrl.clk_rate = clk_div & SPI_F_CLK_DIV_M;
}

static inline void spi_ll_take_cs(spi_hw_t *hw)
{
	hw->ctrl.nssmd = 0x2;
}

static inline void spi_ll_release_cs(spi_hw_t *hw)
{
	hw->ctrl.nssmd = 0x3;
}

static inline void spi_ll_enable_slave_release_int(spi_hw_t *hw)
{
	hw->slave_ctrl.slave_release_int_en = 1;
}

static inline void spi_ll_disable_slave_release_int(spi_hw_t *hw)
{
	hw->slave_ctrl.slave_release_int_en = 0;
}

static inline void spi_ll_set_wire_mode(spi_hw_t *hw, spi_wire_mode_t wire_mode)
{
	if (wire_mode == SPI_3WIRE_MODE) {
		hw->ctrl.nssmd = 0;
	} else {
		hw->ctrl.nssmd = 1;
	}
}

#define spi_ll_set_first_bit(hw, first_bit)

static inline void spi_ll_set_bit_width(spi_hw_t *hw, spi_bit_width_t bit_width)
{
	hw->ctrl.bit_width = bit_width;
}

static inline void spi_ll_set_cpol(spi_hw_t *hw, spi_polarity_t cpol)
{
	hw->ctrl.cpol = cpol;
}

static inline void spi_ll_set_cpha(spi_hw_t *hw, spi_phase_t cpha)
{
	hw->ctrl.cpha = cpha;
}

static inline void spi_ll_set_role(spi_hw_t *hw, spi_role_t role)
{
	hw->ctrl.master_en = role;
}

static inline void spi_ll_set_role_master(spi_hw_t *hw)
{
	hw->ctrl.master_en = 1;
	hw->ctrl.nssmd = 3;
}

static inline void spi_ll_set_role_slave(spi_hw_t *hw)
{
	hw->ctrl.master_en = 0;
	hw->ctrl.nssmd = 1;
}

static inline bool spi_ll_is_role_master(spi_hw_t *hw)
{
	return !!(hw->ctrl.master_en);
}

static inline bool spi_ll_is_role_slave(spi_hw_t *hw)
{
	return !(hw->ctrl.master_en);
}

static inline void spi_ll_enable_tx(spi_hw_t *hw)
{
	if (hw->ctrl.master_en) {
		hw->ctrl.nssmd = 0x2;
	} else {
		hw->ctrl.nssmd = 0x1; /* need set nssmd=1 when as spi slave */
	}

	/* spi MOSI line have no data when write data in spi master mode
	 * tmp solution (not know real reason):
	 * set gpio_18/gpio_19 as normal gpio and output high level then spi work ok
	 */
	REG_WRITE(GPIO_R_18_CONFIG, 0x2);
}

static inline void spi_ll_disable_tx(spi_hw_t *hw)
{
	if (hw->ctrl.master_en) {
		hw->ctrl.nssmd = 0x3;
	}
}

static inline void spi_ll_enable_rx(spi_hw_t *hw)
{
	if (hw->ctrl.master_en) {
		hw->ctrl.nssmd = 0x2;
	} else {
		hw->ctrl.nssmd = 0x1; /* need set nssmd=1 when as spi slave */
	}

	/* spi MOSI line have no data when write data in spi master mode
	 * tmp solution (not know real reason):
	 * set gpio_18/gpio_19 as normal gpio and output high level then spi work ok
	 */
	REG_WRITE(GPIO_R_18_CONFIG, 0x2);
}

static inline void spi_ll_disable_rx(spi_hw_t *hw)
{
	if (hw->ctrl.master_en) {
		hw->ctrl.nssmd = 0x3;
	}
}

#define spi_ll_enable_tx_finish_int(hw)
#define spi_ll_disable_tx_finish_int(hw)
#define spi_ll_enable_rx_finish_int(hw)
#define spi_ll_disable_rx_finish_int(hw)
#define spi_ll_set_tx_trans_len(hw, len)
#define spi_ll_set_rx_trans_len(hw, len)

static inline void spi_ll_clear_int_status(spi_hw_t *hw)
{
	uint32_t int_status = REG_READ(SPI_R_STATUS(0));
	REG_WRITE(SPI_R_STATUS(0), int_status);
	int_status = REG_READ(SPI_R_SLAVE_CTRL(0));
	REG_WRITE(SPI_R_SLAVE_CTRL(0), int_status);
}

static inline void spi_ll_clear_slave_release_int_status(spi_hw_t *hw)
{
	hw->slave_ctrl.slave_release_int = 1;
}

static inline void spi_ll_reset_slave_release_int_status_to_default(spi_hw_t *hw)
{
	hw->slave_ctrl.slave_release_int = 0;
}

static inline void spi_ll_clear_tx_underflow_int_status(spi_hw_t *hw)
{
	hw->status.tx_underflow_int = 1;
}

static inline void spi_ll_clear_rx_overflow_int_status(spi_hw_t *hw)
{
	hw->status.rx_overflow_int = 1;
}

static inline uint32_t spi_ll_get_interrupt_status(spi_hw_t *hw)
{
	/* bit[0:15]
	 * bit[16] slave_release_int
	 */
	return (REG_READ(SPI_R_STATUS(0)) & 0xffff) | (hw->slave_ctrl.slave_release_int << 16);
}

static inline void spi_ll_clear_interrupt_status(spi_hw_t *hw, uint32_t status)
{
	REG_WRITE(SPI_R_STATUS(0), status & 0xffff);
	if (status & BIT(16)) {
		hw->slave_ctrl.slave_release_int = 1;
	}
}

static inline bool spi_ll_is_tx_fifo_wr_ready(spi_hw_t *hw)
{
	return ((!(hw->status.tx_fifo_full)) && (!(hw->status.spi_busy)));
}

static inline bool spi_ll_is_rx_fifo_rd_ready(spi_hw_t *hw)
{
	return !(hw->status.rx_fifo_empty);
}

static inline bool spi_ll_is_rx_fifo_empty(spi_hw_t *hw)
{
	return hw->status.rx_fifo_empty;
}

static inline bool spi_ll_is_tx_fifo_int_triggered(spi_hw_t *hw)
{
	return !!(hw->status.tx_fifo_int);
}

static inline bool spi_ll_is_rx_fifo_int_triggered(spi_hw_t *hw)
{
	return !!(hw->status.rx_fifo_int);
}

static inline bool spi_ll_is_tx_fifo_int_triggered_with_status(spi_hw_t *hw, uint32_t status)
{
	return !!(status & BIT(8));
}

static inline bool spi_ll_is_rx_fifo_int_triggered_with_status(spi_hw_t *hw, uint32_t status)
{
	return !!(status & BIT(9));
}

static inline bool spi_ll_is_slave_release_int_triggered(spi_hw_t *hw, uint32_t status)
{
	return !!(status & BIT(16));
}

static inline bool spi_ll_is_tx_finish_int_triggered(spi_hw_t *hw, uint32_t status)
{
	/* tx fifo empty */
	return !!(status & BIT(0));
}

static inline bool spi_ll_is_rx_finish_int_triggered(spi_hw_t *hw, uint32_t status)
{
	/* slave_release_int not triggered
	 * int_status (tx_fifo=0, rx_fifo=1, rx_empty=1)
	 */
	return ((status & BIT(9)) && (!(status & BIT(8))) && (status & BIT(2)));
}

static inline bool spi_ll_is_rx_overflow_int_triggered(spi_hw_t *hw, uint32_t status)
{
	return !!(status & BIT(12));
}

static inline bool spi_ll_is_tx_underflow_int_triggered(spi_hw_t *hw, uint32_t status)
{
	return !!(status & BIT(11));
}

#define spi_ll_clear_tx_finish_int_status(hw)
#define spi_ll_clear_rx_finish_int_status(hw)

static inline void spi_ll_clear_tx_fifo_int_status(spi_hw_t *hw)
{
	hw->status.tx_fifo_int = 1;
}

static inline void spi_ll_clear_rx_fifo_int_status(spi_hw_t *hw)
{
	hw->status.rx_fifo_int = 1;
}

#define spi_ll_clear_tx_fifo(hw)
#define spi_ll_clear_rx_fifo(hw)

static inline bk_err_t spi_ll_write_byte(spi_hw_t *hw, uint32_t data)
{
	if (spi_ll_is_tx_fifo_wr_ready(hw)) {
		hw->data.fifo_data = data;
		return BK_OK;
	}
	return BK_ERR_SPI_FIFO_WR_NOT_READY;
}

static inline uint32_t spi_ll_read_byte(spi_hw_t *hw)
{
	return hw->data.fifo_data;
}

#ifdef __cplusplus
}
#endif

