#include <common/bk_include.h>
#include "bk_arm_arch.h"
#include <common/sys_config.h>
#include "flash.h"
#include "sys_ctrl.h"
#include "bk_flash.h"
#include "bk_drv_model.h"
#include <string.h>
#include "bk_uart.h"
#include "bk_sys_ctrl.h"
#include "bk_mcu_ps.h"
#include <os/mem.h>
#include <components/log.h>
#include "bk_private/bk_ate.h"
#include "bk_pm_model.h"
#include "sys_driver.h"

#if CONFIG_FLASH_QUAD_ENABLE
#include "flash_bypass.h"
#endif

#define TAG "flash"

static const flash_config_t flash_config[] = {
	{0x1C7016, 1, 0x400000, 2,  0, 2, 0x1F, 0x1F, 0x00, 0x16, 0x01B, 0, 0, 0xA5, 0x01}, //en_25qh32b
	{0x1C7015, 1, 0x200000, 2,  0, 2, 0x1F, 0x1F, 0x00, 0x0d, 0x0d,  0, 0, 0xA5, 0x01}, //en_25qh16b
	{0x0B4014, 2, 0x100000, 2, 14, 2, 0x1F, 0x1F, 0x00, 0x0C, 0x101, 9, 1, 0xA0, 0x01}, //xtx_25f08b
	{0x0B4015, 2, 0x200000, 2, 14, 2, 0x1F, 0x1F, 0x00, 0x0D, 0x101, 9, 1, 0xA0, 0x01}, //xtx_25f16b
#if CONFIG_FLASH_QUAD_ENABLE
	{0x0B4016, 2, 0x400000, 4, 14, 2, 0x1F, 0x1F, 0x00, 0x0E, 0x101, 9, 1, 0xA0, 0x02}, //xtx_25f32b
#else
	{0x0B4016, 2, 0x400000, 2, 14, 2, 0x1F, 0x1F, 0x00, 0x0E, 0x101, 9, 1, 0xA0, 0x01}, //xtx_25f32b
#endif
	{0x0B4017, 2, 0x800000, 2, 14, 2, 0x1F, 0x05, 0x00, 0x0E, 0x109, 9, 1, 0xA0, 0x01}, //xtx_25f64b
	{0x0E4016, 2, 0x400000, 2, 14, 2, 0x1F, 0x1F, 0x00, 0x0E, 0x101, 9, 1, 0xA0, 0x01}, //xtx_FT25H32
	{0xC84015, 2, 0x200000, 2, 14, 2, 0x1F, 0x1F, 0x00, 0x0D, 0x101, 9, 1, 0xA0, 0x01}, //gd_25q16c
#if CONFIG_FLASH_QUAD_ENABLE
	{0xC84016, 2, 0x400000, 4, 14, 2, 0x1F, 0x1F, 0x00, 0x0E, 0x00E, 9, 1, 0xA0, 0x02}, //gd_25q32c
#else
	{0xC84016, 1, 0x400000, 2,  0, 2, 0x1F, 0x1F, 0x00, 0x0E, 0x00E, 0, 0, 0xA0, 0x01}, //gd_25q32c
#endif
	{0xEF4016, 2, 0x400000, 2, 14, 2, 0x1F, 0x1F, 0x00, 0x00, 0x101, 9, 1, 0xA0, 0x01}, //w_25q32(bfj)
	{0x204016, 2, 0x400000, 2, 14, 2, 0x1F, 0x1F, 0x00, 0x0E, 0x101, 9, 1, 0xA0, 0x01}, //xmc_25qh32b
	{0xC22315, 1, 0x200000, 2,  0, 2, 0x0F, 0x0F, 0x00, 0x0A, 0x00E, 6, 1, 0xA5, 0x01}, //mx_25v16b
	{0xEB6015, 2, 0x200000, 2, 14, 2, 0x1F, 0x1F, 0x00, 0x0D, 0x101, 9, 1, 0xA0, 0x01}, //zg_th25q16b
	{0x000000, 2, 0x400000, 2,  0, 2, 0x1F, 0x00, 0x00, 0x00, 0x000, 0, 0, 0x00, 0x01}, //default
};

__maybe_unused static void flash_enable_cpu_data_wr(void);
static const flash_config_t *flash_current_config = NULL;
static UINT32 flash_id;
static const DD_OPERATIONS flash_op = {
	NULL,
	NULL,
	flash_read,
	flash_write,
	flash_ctrl
};

static void flash_get_current_flash_config(void)
{
	int i;

	for (i = 0; i < (sizeof(flash_config) / sizeof(flash_config_t) - 1); i++) {
		if (flash_id == flash_config[i].flash_id) {
			flash_current_config = &flash_config[i];
			break;
		}
	}

	if (i == (sizeof(flash_config) / sizeof(flash_config_t) - 1)) {
		flash_current_config = &flash_config[i];
		BK_LOGI(TAG, "don't config this flash, choose default config\r\n");
	}
}

static void flash_set_clk(UINT8 clk_conf)
{
	UINT32 value;

#if CONFIG_FLASH_SRC_CLK_60M
	sys_drv_flash_set_clk_div(FLASH_DIV_VALUE_TWO);
	sys_drv_flash_set_dco();
#endif

	value = REG_READ(REG_FLASH_CONF);
	value &= ~(FLASH_CLK_CONF_MASK << FLASH_CLK_CONF_POSI);
	value |= (clk_conf << FLASH_CLK_CONF_POSI);

#if CONFIG_JTAG
	value &= ~(CRC_EN);
#endif

	REG_WRITE(REG_FLASH_CONF, value);
}

static void flash_enable_cpu_data_wr(void)
{
	UINT32 value;

	value = REG_READ(REG_FLASH_CONF);
	value |= (CPU_DATA_WR_MASK << CPU_DATA_WR_POSI);
	REG_WRITE(REG_FLASH_CONF, value);
}

#if (0 == CONFIG_JTAG)
static void flash_disable_cpu_data_wr(void)
{
	UINT32 value;

	value = REG_READ(REG_FLASH_CONF);
	value &= (~(CPU_DATA_WR_MASK << CPU_DATA_WR_POSI));

	REG_WRITE(REG_FLASH_CONF, value);
}
#endif

static void flash_write_enable(void)
{
	UINT32 value;

	value = (FLASH_OPCODE_WREN << OP_TYPE_SW_POSI) | OP_SW | WP_VALUE;
	REG_WRITE(REG_FLASH_OPERATE_SW, value);

	while (REG_READ(REG_FLASH_OPERATE_SW) & BUSY_SW);
}

static void flash_write_disable(void)
{
	UINT32 value;

	value = (FLASH_OPCODE_WRDI << OP_TYPE_SW_POSI) | OP_SW | WP_VALUE;
	REG_WRITE(REG_FLASH_OPERATE_SW, value);
	while (REG_READ(REG_FLASH_OPERATE_SW) & BUSY_SW);
}

static UINT16 flash_read_sr(UINT8 sr_width)
{
	UINT16 sr;
	UINT32 value;

	while (REG_READ(REG_FLASH_OPERATE_SW) & BUSY_SW);

	value = (FLASH_OPCODE_RDSR << OP_TYPE_SW_POSI) | OP_SW | WP_VALUE;
	REG_WRITE(REG_FLASH_OPERATE_SW, value);
	while (REG_READ(REG_FLASH_OPERATE_SW) & BUSY_SW);

	value = REG_READ(REG_FLASH_SR_DATA_CRC_CNT);
	sr = value & 0x00FF;

	if (sr_width == 2) {
		value = (FLASH_OPCODE_RDSR2 << OP_TYPE_SW_POSI) | OP_SW | WP_VALUE;
		REG_WRITE(REG_FLASH_OPERATE_SW, value);
		while (REG_READ(REG_FLASH_OPERATE_SW) & BUSY_SW);

		value = REG_READ(REG_FLASH_SR_DATA_CRC_CNT);
		sr |= (value & 0x00FF) << 8;
	}

	BK_LOGI(TAG, "--read sr:%x--\r\n",sr);

	return sr;
}

static void flash_write_sr(UINT8 sr_width,  UINT16 val)
{
	UINT32 value;

#if (CONFIG_SOC_BK7231N) || (CONFIG_SOC_BK7236) || (CONFIG_SOC_BK7256XX)
	GLOBAL_INT_DECLARATION();

	GLOBAL_INT_DISABLE();
#endif
	while (REG_READ(REG_FLASH_OPERATE_SW) & BUSY_SW);

	value = REG_READ(REG_FLASH_CONF);
	value &= ~(WRSR_DATA_MASK << WRSR_DATA_POSI);

	value |= (val << WRSR_DATA_POSI);

	REG_WRITE(REG_FLASH_CONF, value);
	while (REG_READ(REG_FLASH_OPERATE_SW) & BUSY_SW);

#if CONFIG_FLASH_QUAD_ENABLE
	if (FLASH_ID_GD25Q32C == flash_id)
		sr_width = 1;
#endif
	if (sr_width == 1) {
		value = (FLASH_OPCODE_WRSR << OP_TYPE_SW_POSI) | OP_SW | WP_VALUE;
		REG_WRITE(REG_FLASH_OPERATE_SW, value);
	} else if (sr_width == 2) {
		value = (FLASH_OPCODE_WRSR2 << OP_TYPE_SW_POSI) | OP_SW | WP_VALUE;
		REG_WRITE(REG_FLASH_OPERATE_SW, value);
	}

	while (REG_READ(REG_FLASH_OPERATE_SW) & BUSY_SW);
#if (CONFIG_SOC_BK7231N) || (CONFIG_SOC_BK7236) || (CONFIG_SOC_BK7256XX)
	GLOBAL_INT_RESTORE();
#endif
}

static UINT8 flash_read_qe(void)
{
	UINT8 temp;
	UINT32 value;

	if (1 == flash_current_config->sr_size) {
		value = (FLASH_OPCODE_RDSR << OP_TYPE_SW_POSI) | OP_SW | WP_VALUE;
		REG_WRITE(REG_FLASH_OPERATE_SW, value);
	} else {
		value = (FLASH_OPCODE_RDSR2 << OP_TYPE_SW_POSI) | OP_SW | WP_VALUE;
		REG_WRITE(REG_FLASH_OPERATE_SW, value);
	}

	while (REG_READ(REG_FLASH_OPERATE_SW) & BUSY_SW);

	value = REG_READ(REG_FLASH_SR_DATA_CRC_CNT);
	temp = (value & 0xFF);
	return temp;
}

static void flash_set_qe(void)
{
	UINT32 value, param;

	while (REG_READ(REG_FLASH_OPERATE_SW) & BUSY_SW);
	param = flash_read_sr(flash_current_config->sr_size);
	if ((param & (flash_current_config->qe_bit << flash_current_config->qe_bit_post)))
		return ;

#if CONFIG_FLASH_QUAD_ENABLE
	if (FLASH_ID_GD25Q32C == flash_id) {
		flash_bypass_quad_enable();
	}
#endif
	value = REG_READ(REG_FLASH_CONF);
	value &= ~(WRSR_DATA_MASK << WRSR_DATA_POSI);
	value |= (((flash_current_config->qe_bit << flash_current_config->qe_bit_post)
			   | param) << WRSR_DATA_POSI);
	REG_WRITE(REG_FLASH_CONF, value);

	value = REG_READ(REG_FLASH_OPERATE_SW);

	if (1 == flash_current_config->sr_size) {
		value = (value & (ADDR_SW_REG_MASK << ADDR_SW_REG_POSI))
				| (FLASH_OPCODE_WRSR << OP_TYPE_SW_POSI) | OP_SW | WP_VALUE;
	} else {
		value = (value & (ADDR_SW_REG_MASK << ADDR_SW_REG_POSI))
				| (FLASH_OPCODE_WRSR2 << OP_TYPE_SW_POSI) | OP_SW | WP_VALUE;
	}

	REG_WRITE(REG_FLASH_OPERATE_SW, value);

	while (REG_READ(REG_FLASH_OPERATE_SW) & BUSY_SW);
}

static void flash_set_qwfr(void)
{
	UINT32 value;

	value = REG_READ(REG_FLASH_CONF);
	value &= ~(MODEL_SEL_MASK << MODEL_SEL_POSI);
	value |= (flash_current_config->mode_sel << MODEL_SEL_POSI);
	REG_WRITE(REG_FLASH_CONF, value);
}

static void flash_clr_qwfr(void)
{
	UINT32 value;

	value = REG_READ(REG_FLASH_CONF);
	value &= ~(MODEL_SEL_MASK << MODEL_SEL_POSI);
	REG_WRITE(REG_FLASH_CONF, value);

	value = REG_READ(REG_FLASH_OPERATE_SW);
	value = ((0 << ADDR_SW_REG_POSI)
			 | (FLASH_OPCODE_CRMR << OP_TYPE_SW_POSI)
			 | OP_SW
			 | (value & WP_VALUE));
	REG_WRITE(REG_FLASH_OPERATE_SW, value);

	while (REG_READ(REG_FLASH_OPERATE_SW) & BUSY_SW);
}

static void flash_set_wsr(UINT16 data)
{
	UINT32 value;

	while (REG_READ(REG_FLASH_OPERATE_SW) & BUSY_SW);

	value = REG_READ(REG_FLASH_CONF);
	value &= ~(WRSR_DATA_MASK << WRSR_DATA_POSI);
	value |= (data << WRSR_DATA_POSI);
	REG_WRITE(REG_FLASH_CONF, value);

	value = REG_READ(REG_FLASH_OPERATE_SW);
	value = (value & (ADDR_SW_REG_MASK << ADDR_SW_REG_POSI))
			| (FLASH_OPCODE_WRSR2 << OP_TYPE_SW_POSI) | OP_SW | WP_VALUE;
	REG_WRITE(REG_FLASH_OPERATE_SW, value);

	while (REG_READ(REG_FLASH_OPERATE_SW) & BUSY_SW);
}

UINT8 flash_get_line_mode(void)
{
	return flash_current_config->line_mode;
}

void flash_set_line_mode(UINT8 mode)
{
	UINT32 value;

	if (1 == mode)
		flash_clr_qwfr();
	if (2 == mode) {
		flash_clr_qwfr();
		value = REG_READ(REG_FLASH_CONF);
		value &= ~(MODEL_SEL_MASK << MODEL_SEL_POSI);
		value |= ((MODE_DUAL & MODEL_SEL_MASK) << MODEL_SEL_POSI);
		REG_WRITE(REG_FLASH_CONF, value);
	} else if (4 == mode) {
		flash_clr_qwfr();
		value = REG_READ(REG_FLASH_SR_DATA_CRC_CNT);
		value &= ~(M_VALUE_MASK << M_VALUE_POST);
		value |= (flash_current_config->m_value << M_VALUE_POST);

		REG_WRITE(REG_FLASH_SR_DATA_CRC_CNT, value);

		value = REG_READ(REG_FLASH_SR_DATA_CRC_CNT);

		if (1 == flash_current_config->qe_bit)
			flash_set_qe();

		flash_set_qwfr();
	}
}

static UINT32 flash_get_id(void)
{
	UINT32 value;

	value = (FLASH_OPCODE_RDID << OP_TYPE_SW_POSI) | OP_SW | WP_VALUE;
	REG_WRITE(REG_FLASH_OPERATE_SW, value);

	while (REG_READ(REG_FLASH_OPERATE_SW) & BUSY_SW);
	flash_id = REG_READ(REG_FLASH_RDID_DATA_FLASH);
	return flash_id;
}

static UINT32 flash_read_mid(void)
{
	UINT32 value;
	UINT32 flash_id;

	while (REG_READ(REG_FLASH_OPERATE_SW) & BUSY_SW);

	value = REG_READ(REG_FLASH_OPERATE_SW);
	value = ((value & (ADDR_SW_REG_MASK << ADDR_SW_REG_POSI))
			 | (FLASH_OPCODE_RDID << OP_TYPE_SW_POSI)
			 | OP_SW
			 | (value & WP_VALUE));
	REG_WRITE(REG_FLASH_OPERATE_SW, value);
	while (REG_READ(REG_FLASH_OPERATE_SW) & BUSY_SW);

	flash_id = REG_READ(REG_FLASH_RDID_DATA_FLASH);

	return flash_id;
}

PROTECT_TYPE get_flash_protect(void)
{
	UINT16 sr_value, cmp, param, value, type;

	sr_value = flash_read_sr(flash_current_config->sr_size);
	param = (sr_value >> flash_current_config->protect_post) & flash_current_config->protect_mask;
	cmp = (sr_value >> flash_current_config->cmp_post) & 0x01;
	value = (cmp << 8) | param;

	if (value == flash_current_config->protect_all)
		type = FLASH_PROTECT_ALL;
	else if (value == flash_current_config->protect_none)
		type = FLASH_PROTECT_NONE;
	else if (value == flash_current_config->protect_half)
		type = FLASH_PROTECT_HALF;
	else if (value == flash_current_config->unprotect_last_block)
		type = FLASH_UNPROTECT_LAST_BLOCK;
	else
		type = -1;

	return type;
}

static void set_flash_protect(PROTECT_TYPE type)
{
	UINT32 param, value, cmp;

	switch (type) {
	case FLASH_PROTECT_NONE:
		param = flash_current_config->protect_none & 0xff;
		cmp = (flash_current_config->protect_none >> 8) & 0xff;
		break;

	case FLASH_PROTECT_ALL:
		param = flash_current_config->protect_all & 0xff;
		cmp = (flash_current_config->protect_all >> 8) & 0xff;
		break;

	case FLASH_PROTECT_HALF:
		param = flash_current_config->protect_half & 0xff;
		cmp = (flash_current_config->protect_half >> 8) & 0xff;
		break;

	case FLASH_UNPROTECT_LAST_BLOCK:
		param = flash_current_config->unprotect_last_block & 0xff;
		cmp = (flash_current_config->unprotect_last_block >> 8) & 0xff;
		break;

	default:
		param = flash_current_config->protect_all & 0xff;
		cmp = (flash_current_config->protect_all >> 8) & 0xff;
		break;
	}

	value = flash_read_sr(flash_current_config->sr_size);

	if (((param << flash_current_config->protect_post) !=
		 (value & (flash_current_config->protect_mask << flash_current_config->protect_post)))
		|| ((cmp << flash_current_config->cmp_post) !=
			(value & (0x01 << flash_current_config->cmp_post)))) {
		value = (value & (~(flash_current_config->protect_mask
							<< flash_current_config->protect_post)))
				| (param << flash_current_config->protect_post);
		value &= ~(1 << flash_current_config->cmp_post);
		value |= ((cmp & 0x01) << flash_current_config->cmp_post);

		BK_LOGI(TAG, "--write status reg:%x,%x--\r\n", value, flash_current_config->sr_size);
		flash_write_sr(flash_current_config->sr_size, value);
	}
}

static void flash_erase_sector(UINT32 address)
{
	UINT32 value;
	UINT32 erase_addr = address & 0xFFF000;
#if (CONFIG_SOC_BK7231N) || (CONFIG_SOC_BK7236) || (CONFIG_SOC_BK7256XX)
	GLOBAL_INT_DECLARATION();
#endif

	if (erase_addr >= flash_current_config->flash_size) {
		bk_printf("Erase error:invalid address0x%x\r\n", erase_addr);
		return;
	}

#if (CONFIG_SOC_BK7231N) || (CONFIG_SOC_BK7236) || (CONFIG_SOC_BK7256XX)
	GLOBAL_INT_DISABLE();
#endif
	while (REG_READ(REG_FLASH_OPERATE_SW) & BUSY_SW);
	value = REG_READ(REG_FLASH_OPERATE_SW);
	value = ((erase_addr << ADDR_SW_REG_POSI)
			 | (FLASH_OPCODE_SE << OP_TYPE_SW_POSI)
			 | OP_SW
			 | (value & WP_VALUE));
	REG_WRITE(REG_FLASH_OPERATE_SW, value);
	while (REG_READ(REG_FLASH_OPERATE_SW) & BUSY_SW);
#if (CONFIG_SOC_BK7231N) || (CONFIG_SOC_BK7236) || (CONFIG_SOC_BK7256XX)
	GLOBAL_INT_RESTORE();
#endif
}

static void flash_set_hpm(void)
{
	UINT32 value;

	while (REG_READ(REG_FLASH_OPERATE_SW) & BUSY_SW);
	value = REG_READ(REG_FLASH_OPERATE_SW);
	value = ((0x0 << ADDR_SW_REG_POSI)
			 | (FLASH_OPCODE_HPM << OP_TYPE_SW_POSI)
			 | (OP_SW)
			 | (value & WP_VALUE));
	REG_WRITE(REG_FLASH_OPERATE_SW, value);
	while (REG_READ(REG_FLASH_OPERATE_SW) & BUSY_SW);
}

static void flash_read_data(UINT8 *buffer, UINT32 address, UINT32 len)
{
	UINT32 i, reg_value;
	UINT32 addr = address & (~0x1F);
	UINT32 buf[8];
	UINT8 *pb = (UINT8 *)&buf[0];
#if (CONFIG_SOC_BK7231N) || (CONFIG_SOC_BK7236) || (CONFIG_SOC_BK7256XX)
	GLOBAL_INT_DECLARATION();
#endif

	if (len == 0)
		return;

#if (CONFIG_SOC_BK7231N) || (CONFIG_SOC_BK7236) || (CONFIG_SOC_BK7256XX)
	GLOBAL_INT_DISABLE();
#endif
	while (REG_READ(REG_FLASH_OPERATE_SW) & BUSY_SW);
	while (len) {
		reg_value = REG_READ(REG_FLASH_OPERATE_SW);
		reg_value = ((addr << ADDR_SW_REG_POSI)
					 | (FLASH_OPCODE_READ << OP_TYPE_SW_POSI)
					 | OP_SW
					 | (reg_value & WP_VALUE));
		REG_WRITE(REG_FLASH_OPERATE_SW, reg_value);
		while (REG_READ(REG_FLASH_OPERATE_SW) & BUSY_SW);
		addr += 32;

		for (i = 0; i < 8; i++)
			buf[i] = REG_READ(REG_FLASH_DATA_FLASH_SW);

		for (i = address % 32; i < 32; i++) {
			*buffer++ = pb[i];
			address++;
			len--;
			if (len == 0)
				break;
		}
	}
#if (CONFIG_SOC_BK7231N) || (CONFIG_SOC_BK7236) || (CONFIG_SOC_BK7256XX)
	GLOBAL_INT_RESTORE();
#endif
}

static void flash_write_data(UINT8 *buffer, UINT32 address, UINT32 len)
{
	UINT32 i, reg_value;
	UINT32 addr = address & (~0x1F);
	UINT32 buf[8];
	UINT8 *pb = (UINT8 *)&buf[0];
#if (CONFIG_SOC_BK7231N) || (CONFIG_SOC_BK7236) || (CONFIG_SOC_BK7256XX)
	GLOBAL_INT_DECLARATION();
#endif

	if ((addr >= flash_current_config->flash_size)
		|| (len > flash_current_config->flash_size)
		|| ((addr + len) > flash_current_config->flash_size)) {
		bk_printf("Write error[addr:0x%x len:0x%x]\r\n", addr, len);
		return;
	}

	if (address % 32)
		flash_read_data(pb, addr, 32);
	else
		os_memset(pb, 0xFF, 32);

	while (REG_READ(REG_FLASH_OPERATE_SW) & BUSY_SW);
	while (len) {
		if (len < 32) {
			flash_read_data(pb, addr, 32);
			while (REG_READ(REG_FLASH_OPERATE_SW) & BUSY_SW);
		}
		for (i = address % 32; i < 32; i++) {
			pb[i] = *buffer++;
			address++;
			len--;
			if (len == 0)
				break;
		}

#if (CONFIG_SOC_BK7231N) || (CONFIG_SOC_BK7236) || (CONFIG_SOC_BK7256XX)
		GLOBAL_INT_DISABLE();
#endif
		for (i = 0; i < 8; i++)
			REG_WRITE(REG_FLASH_DATA_SW_FLASH, buf[i]);

		reg_value = REG_READ(REG_FLASH_OPERATE_SW);
		reg_value = ((addr << ADDR_SW_REG_POSI)
					 | (FLASH_OPCODE_PP << OP_TYPE_SW_POSI)
					 | OP_SW
					 | (reg_value & WP_VALUE));
		REG_WRITE(REG_FLASH_OPERATE_SW, reg_value);
		while (REG_READ(REG_FLASH_OPERATE_SW) & BUSY_SW);
#if (CONFIG_SOC_BK7231N) || (CONFIG_SOC_BK7236) || (CONFIG_SOC_BK7256XX)
		GLOBAL_INT_RESTORE();
#endif
		addr += 32;
		os_memset(pb, 0xFF, 32);
	}
}

void flash_protection_op(UINT8 mode, PROTECT_TYPE type)
{
	set_flash_protect(type);
}

void flash_init(void)
{
	UINT32 id;

	while (REG_READ(REG_FLASH_OPERATE_SW) & BUSY_SW);

	id = flash_get_id();
	BK_LOGI(TAG, "id=0x%x\r\n", id);
	flash_get_current_flash_config();

	set_flash_protect(FLASH_UNPROTECT_LAST_BLOCK);

#if (0 == CONFIG_JTAG)
	flash_disable_cpu_data_wr();
#endif

	flash_set_line_mode(flash_current_config->line_mode);

	flash_set_clk(5);  // 5:clk_flash/2;8:clk_flash/1

	ddev_register_dev(DD_DEV_TYPE_FLASH, (DD_OPERATIONS *)&flash_op);

	BK_LOGD(TAG, "init over\r\n");
}

void flash_exit(void)
{
	ddev_unregister_dev(DD_DEV_TYPE_FLASH);
}

UINT32 flash_read(char *user_buf, UINT32 count, UINT32 address)
{
	peri_busy_count_add();

	flash_read_data((UINT8 *)user_buf, address, count);

	peri_busy_count_dec();

	return FLASH_SUCCESS;
}

UINT32 flash_write(char *user_buf, UINT32 count, UINT32 address)
{
	peri_busy_count_add();

	if (4 == flash_current_config->line_mode)
		flash_set_line_mode(LINE_MODE_TWO);

	flash_write_data((UINT8 *)user_buf, address, count);

	if (4 == flash_current_config->line_mode)
		flash_set_line_mode(LINE_MODE_FOUR);
	peri_busy_count_dec();

	return FLASH_SUCCESS;
}


UINT32 flash_ctrl(UINT32 cmd, void *parm)
{
	UINT8 clk;
	UINT16 wsr;
	UINT32 address;
	UINT32 reg;
	UINT32 ret = FLASH_SUCCESS;
	peri_busy_count_add();

	if (4 == flash_current_config->line_mode)
		flash_set_line_mode(LINE_MODE_TWO);

	switch (cmd) {
	case CMD_FLASH_SET_CLK:
		clk = (*(UINT8 *)parm);
		flash_set_clk(clk);
		break;

	case CMD_FLASH_SET_DPLL:
		//sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_SET_FLASH_DPLL, 0);
		sys_drv_flash_set_dpll();

		reg = REG_READ(REG_FLASH_CONF);
		reg &= ~(FLASH_CLK_CONF_MASK << FLASH_CLK_CONF_POSI);
		reg = reg | (5 << FLASH_CLK_CONF_POSI);
		REG_WRITE(REG_FLASH_CONF, reg);
		break;

	case CMD_FLASH_SET_DCO:
		//sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_SET_FLASH_DCO, 0);
		sys_drv_flash_set_dco();

		reg = REG_READ(REG_FLASH_CONF);
		reg &= ~(FLASH_CLK_CONF_MASK << FLASH_CLK_CONF_POSI);
		if (ate_is_enabled())
			reg = reg | (0xB << FLASH_CLK_CONF_POSI);
		else
			reg = reg | (9 << FLASH_CLK_CONF_POSI);
		REG_WRITE(REG_FLASH_CONF, reg);
		break;

	case CMD_FLASH_WRITE_ENABLE:
		flash_write_enable();
		break;

	case CMD_FLASH_WRITE_DISABLE:
		flash_write_disable();
		break;

	case CMD_FLASH_READ_SR:
		(*(UINT16 *)parm) = flash_read_sr(2);
		break;

	case CMD_FLASH_WRITE_SR:
		flash_write_sr(*(unsigned long *)parm & 0x00FF, ((*(unsigned long *)parm) >> 8) & 0x00FFFF);
		break;

	case CMD_FLASH_READ_QE:
		(*(UINT8 *)parm) = flash_read_qe();
		break;

	case CMD_FLASH_SET_QE:
		if (flash_current_config->qe_bit)
			flash_set_qe();
		break;

	case CMD_FLASH_SET_QWFR:
		flash_set_qwfr();
		break;

	case CMD_FLASH_CLR_QWFR:
		flash_clr_qwfr();
		break;

	case CMD_FLASH_SET_WSR:
		wsr = (*(UINT16 *)parm);
		flash_set_wsr(wsr);
		break;

	case CMD_FLASH_GET_ID:
		(*(UINT32 *)parm) = flash_get_id();
		break;

	case CMD_FLASH_READ_MID:
		(*(UINT32 *)parm) = flash_read_mid();
		break;

	case CMD_FLASH_GET_PROTECT:
		(*(UINT32 *)parm) = get_flash_protect();
		break;

	case CMD_FLASH_ERASE_SECTOR:
		address = (*(UINT32 *)parm);
		flash_erase_sector(address);
		break;

	case CMD_FLASH_SET_HPM:
		flash_set_hpm();
		break;

	case CMD_FLASH_SET_PROTECT:
		reg = (*(UINT32 *)parm);
		flash_protection_op(FLASH_XTX_16M_SR_WRITE_DISABLE, reg);
		break;

	default:
		ret = FLASH_FAILURE;
		break;
	}

	if (4 == flash_current_config->line_mode) {
		flash_set_line_mode(LINE_MODE_FOUR);
		//BK_LOGI(TAG, "change line mode 4\r\n");
	}

	peri_busy_count_dec();
	return ret;
}


void sctrl_flash_select_dco(void)
{
	DD_HANDLE flash_hdl;
	UINT32 status;

	/* Flash 26MHz clock select dco clock*/
	flash_hdl = ddev_open(DD_DEV_TYPE_FLASH, &status, 0);
	ddev_control(flash_hdl, CMD_FLASH_SET_DCO, 0);

	//flash get id  shouldn't remove
	ddev_control(flash_hdl, CMD_FLASH_GET_ID, &status);
}




// eof

