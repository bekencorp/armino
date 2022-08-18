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

#include "sys_hal.h"
#include "sys_ctrl.h"
#include "bk_icu.h"
#include "icu_hal.h"
#include "power_ll.h"
#include "bk_misc.h"

static icu_hal_t s_icu_hal;
//static sys_hal_t s_sys_hal;	//TODO:

/**  Platform Start **/
//Platform

//sys_hal_主语(模块名)_谓语(动作:set/get/enable等)_宾语(status/value)
//该函数在每个芯片目录中都有一份

/** Platform PWM Start **/


/** Platform PWM End **/

/**  Platform Start **/
//Platform
bk_err_t sys_hal_init()
{
	s_icu_hal.hw = (icu_hw_t *)SOC_ICU_REG_BASE;

	//TODO:
	//s_sys_hal.hw = SOC_SYS_CTRL_REG_BASE;
	return BK_OK;
}

void sys_hal_flash_set_dco(void)
{
	uint32_t reg = 0;

	reg = REG_READ(SCTRL_CONTROL);
	reg &= ~FLASH_26M_MUX_BIT;
	REG_WRITE(SCTRL_CONTROL, reg);

}

void sys_hal_flash_set_dpll(void)
{
	uint32_t reg = 0;

	reg = REG_READ(SCTRL_CONTROL);
	reg |= FLASH_26M_MUX_BIT;
	REG_WRITE(SCTRL_CONTROL, reg);
}

void sys_hal_set_qspi_vddram_voltage(uint32_t param)
{

#if 0
	uint32_t reg = 0;

	reg = REG_READ(SCTRL_CONTROL);
	reg &= ~(PSRAM_VDDPAD_VOLT_MASK << PSRAM_VDDPAD_VOLT_POSI);
	reg |= ((param & PSRAM_VDDPAD_VOLT_MASK) << PSRAM_VDDPAD_VOLT_POSI);
	REG_WRITE(SCTRL_CONTROL, reg);
#endif
}

void sys_hal_set_qspi_io_voltage(uint32_t param)
{
#if 0
	uint32_t reg = 0;

	reg = REG_READ(SCTRL_CONTROL);
	reg &= ~(QSPI_IO_VOLT_MASK << QSPI_IO_VOLT_POSI);
	reg |= ((param & QSPI_IO_VOLT_MASK) << QSPI_IO_VOLT_POSI);
	REG_WRITE(SCTRL_CONTROL, reg);
#endif
}

void sys_hal_qspi_clk_sel(uint32_t param)
{
#if 0
	uint32_t reg = 0;

	reg = REG_READ(ICU_R_CLK_MUX);
	reg &= (~(3 << 16));
	reg |= (param);
	REG_WRITE(ICU_R_CLK_MUX, reg);
#endif
}

void sys_hal_clk_pwr_up(uint32_t param)
{
#if 0
	uint32_t reg = 0;
	reg = REG_READ(ICU_PERI_CLK_PWD);
	reg &= ~(param);
	REG_WRITE(ICU_PERI_CLK_PWD, reg);
#endif
}

void sys_hal_clk_pwr_down(uint32_t param)
{
#if 0
	uint32_t reg = 0;
	reg = REG_READ(ICU_PERI_CLK_PWD);
	reg |= (param);
	REG_WRITE(ICU_PERI_CLK_PWD, reg);
#endif
}
/*low power feature start*/
void sys_hal_enter_deep_sleep(void * param)
{

}
void sys_hal_enter_normal_sleep(UINT32 peri_clk)
{

}

void sys_hal_enter_normal_wakeup()
{

}
void sys_hal_module_power_ctrl(power_module_name_t module,power_module_state_t power_state)
{

}
int32 sys_hal_module_power_state_get(power_module_name_t module)
{
	return 0;
}
void sys_hal_enter_low_voltage(void)
{

}
void sys_hal_touch_wakeup_enable(uint8_t index)
{

}
void sys_hal_usb_wakeup_enable(uint8_t index)
{

}
void sys_hal_rtc_wakeup_enable(uint32_t value)
{

}

void sys_hal_enable_bt_wakeup_source()
{

}

void sys_hal_cpu_clk_div_set(uint32_t core_index, uint32_t value)
{

}
uint32_t sys_hal_cpu_clk_div_get(uint32_t core_index)
{
	return 0;
}
void sys_hal_all_modules_clk_div_set(clk_div_reg_e reg, uint32_t value)
{

}
uint32_t sys_hal_all_modules_clk_div_get(clk_div_reg_e reg)
{
	return 0;
}
void sys_hal_low_power_hardware_init()
{
}
int32 sys_hal_lp_vol_set(uint32_t value)
{
	return 0;
}
uint32_t sys_hal_lp_vol_get()
{
	return 0;
}
/*low power feature end*/
void sys_hal_enable_mac_wakeup_source()
{
    UINT32 reg = 0;
	reg = REG_READ(ICU_R_ARM_WAKEUP_EN);
	reg |= MAC_ARM_WAKEUP_EN_BIT;
	REG_WRITE(ICU_R_ARM_WAKEUP_EN, reg);
}

uint32 sys_hal_get_chip_id(void)
{
	uint32 ret = 0;
	ret = REG_READ(SCTRL_CHIP_ID);
	return ret;
}

uint32 sys_hal_get_device_id(void)
{
	uint32 ret = 0;
	ret = REG_READ(SCTRL_DEVICE_ID);
	return ret;
}

int32 sys_hal_int_disable(uint32 param) //CMD_ICU_INT_DISABLE
{
	int32 ret = 0;
	uint32 reg = 0;

	reg = REG_READ(ICU_R_INT_EN);
	reg &= ~(param);
	REG_WRITE(ICU_R_INT_EN, reg);

	return ret;
}

int32 sys_hal_int_enable(uint32 param) //CMD_ICU_INT_ENABLE
{
	int32 ret = 0;
	uint32 reg = 0;

	reg = REG_READ(ICU_R_INT_EN);
	reg |= (param);
	REG_WRITE(ICU_R_INT_EN, reg);

	return ret;
}

//NOTICE:Temp add for BK7256 product which has more then 32 Interrupt sources
int32 sys_hal_int_group2_disable(uint32 param) //CMD_ICU_INT_DISABLE
{
	return 0;
}

//NOTICE:Temp add for BK7256 product which has more then 32 Interrupt sources
int32 sys_hal_int_group2_enable(uint32 param) //CMD_ICU_INT_ENABLE
{
	return 0;
}

int32 sys_hal_fiq_disable(uint32 param)
{
	int32 ret = 0;

#if (CONFIG_SOC_BK7271)
	uint32 reg = 0;

	reg = REG_READ(ICU_R_FIQ_ENABLE);
	reg &= ~(param);
	REG_WRITE(ICU_R_FIQ_ENABLE, reg);
#endif

	return ret;
}

int32 sys_hal_fiq_enable(uint32 param)
{
	int32 ret = 0;

#if (CONFIG_SOC_BK7271)
	uint32 reg = 0;

	reg = REG_READ(ICU_R_FIQ_ENABLE);
	reg |= (param);
	REG_WRITE(ICU_R_FIQ_ENABLE, reg);
#endif

	return ret;
}

int32 sys_hal_global_int_disable(uint32 param)
{
	int32 ret = 0;
	uint32 reg = 0;

	reg = REG_READ(ICU_R_GLOBAL_INT_EN);
	reg &= ~(param);
	REG_WRITE(ICU_R_GLOBAL_INT_EN, reg);

	return ret;
}

int32 sys_hal_global_int_enable(uint32 param)
{
	int32 ret = 0;
	uint32 reg = 0;

	reg = REG_READ(ICU_R_GLOBAL_INT_EN);
	reg |= (param);
	REG_WRITE(ICU_R_GLOBAL_INT_EN, reg);

	return ret;
}

uint32 sys_hal_get_int_status(void) // CMD_GET_INTR_STATUS
{
	uint32 reg = 0;
	reg = REG_READ(ICU_R_INT_STATUS);
	return reg;
}

int32 sys_hal_set_int_status(uint32 param) // CMD_CLR_INTR_STATUS
{
	uint32 reg = 0;
	reg = REG_READ(ICU_R_INT_STATUS);
	REG_WRITE(ICU_R_INT_STATUS, (reg | param));
	return 0;
}

uint32 sys_hal_get_fiq_reg_status(void)
{
	uint32 reg = 0;
#if (CONFIG_SOC_BK7271)
	reg = REG_READ(ICU_R_FIQ_STATUS);
#endif
	return reg;
}

uint32 sys_hal_set_fiq_reg_status(uint32 param)
{
	int32 ret = 0;
#if (CONFIG_SOC_BK7271)
	uint32 reg = 0;
	reg = REG_READ(ICU_R_FIQ_STATUS);
	REG_WRITE(ICU_R_FIQ_STATUS, (reg | param));
#endif

	return ret;
}

uint32 sys_hal_get_intr_raw_status(void)
{
	uint32 reg = 0;
	reg = REG_READ(ICU_R_INT_RAW_STATUS);
	return reg;
}

uint32 sys_hal_set_intr_raw_status(uint32 param) //CMD_CLR_INTR_RAW_STATUS
{
	int32 ret = 0;
	uint32 reg = 0;

	reg = REG_READ(ICU_R_INT_RAW_STATUS);
	REG_WRITE(ICU_R_INT_RAW_STATUS, (reg | param));

	return ret;
}

int32 sys_hal_set_jtag_mode(uint32 param)
{
	int32 ret = 0;

	if (JTAG_ARM_MODE == (param))
	{
		REG_WRITE(ICU_R_JTAG_SELECT, ICU_V_JTAG_SEL_WR_ARM);
	}
	else if (JTAG_TL410_MODE == (param))
	{
		REG_WRITE(ICU_R_JTAG_SELECT, ICU_V_JTAG_SEL_WR_TL4);
	}

#if (CONFIG_SOC_BK7271)
	else if (JTAG_BT_MODE == (param))
	{
		REG_WRITE(ICU_R_JTAG_SELECT, ICU_V_JTAG_SEL_WR_BT);
	}
#endif

	return ret;
}

uint32 sys_hal_get_jtag_mode(void)
{
	uint32 reg = 0;
	reg = REG_READ(ICU_R_JTAG_SELECT);
	return reg;
}

/*clock power control start*/
void sys_hal_clk_pwr_ctrl(dev_clk_pwr_id_t dev, dev_clk_pwr_ctrl_t power_up)
{
	uint32 v = icu_ll_get_peri_clk_pwr_status(s_icu_hal.hw);

	if(CLK_PWR_CTRL_PWR_UP == power_up)
		v &= ~(1 << dev);
	else
		v |= (1 << dev);

	icu_ll_set_peri_clk_pwr(s_icu_hal.hw, v);
}

//special dev:QSPI(2Bits valid),CLK_SEL_ID_SARADC_AUDIO_PLL(depends on CLK_SEL_ID_SARADC)
void sys_hal_set_clk_select(dev_clk_select_id_t dev, dev_clk_select_t clk_sel)
{
	uint32 v = icu_ll_get_clk_sel(s_icu_hal.hw);

	if(CLK_SEL_XTL_26M == clk_sel)
		v |= (1 << dev);
	else
		v &= ~(1 << dev);

	icu_ll_set_clk_sel(s_icu_hal.hw, v);
}

dev_clk_select_t sys_hal_get_clk_select(dev_clk_select_id_t dev)
{
	uint32 v = icu_ll_get_clk_sel(s_icu_hal.hw);

	if(v & (1 << dev))
		return CLK_SEL_XTL_26M;
	else
		return CLK_SEL_DCO;
}

//DCO divider is valid for all of the peri-devices.
void sys_hal_set_dco_div(dev_clk_dco_div_t div)
{
	icu_ll_set_dco_div(s_icu_hal.hw, div);
}

//DCO divider is valid for all of the peri-devices.
dev_clk_dco_div_t sys_hal_get_dco_div(void)
{
	return icu_ll_get_dco_div(s_icu_hal.hw);
}

/*clock power control end*/

/*wake up control start*/
void sys_hal_arm_wakeup_enable(uint32_t param)
{
	uint32_t reg = 0;

	reg = REG_READ(ICU_R_ARM_WAKEUP_EN);
	reg |= param;
	REG_WRITE(ICU_R_ARM_WAKEUP_EN, reg);
}

void sys_hal_arm_wakeup_disable(uint32_t param)
{
	uint32_t reg = 0;

	reg = REG_READ(ICU_R_ARM_WAKEUP_EN);
	reg &= ~(param);
	REG_WRITE(ICU_R_ARM_WAKEUP_EN, reg);
}

uint32_t sys_hal_get_arm_wakeup(void)
{
	uint32_t ret = 0;

	ret = REG_READ(ICU_R_ARM_WAKEUP_EN);
	return ret;
}
/*wake up control end*/

void sys_hal_sadc_int_enable(void)
{
    icu_ll_enable_saradc_interrupt(s_icu_hal.hw);
}

void sys_hal_sadc_int_disable(void)
{
   icu_ll_disable_saradc_interrupt(s_icu_hal.hw);
}

void sys_hal_sadc_pwr_up(void)
{
    power_ll_pwr_up_saradc(s_icu_hal.hw);
}

void sys_hal_sadc_pwr_down(void)
{
    power_ll_pwr_down_saradc(s_icu_hal.hw);
}
/**  Platform End **/




/**  BT Start **/
//BT
/**  BT End **/




/**  Audio Start **/
//Audio
/**  Audio End **/




/**  Video Start **/
//Video
/**  Video End **/




/**  WIFI Start **/
//WIFI
//Yantao Add Start
void sys_hal_modem_core_reset(void)
{
	UINT32 ret;
	UINT32 reg;

	ret = REG_READ(SCTRL_MODEM_CORE_RESET_PHY_HCLK);
	ret = ret & (~((MODEM_CORE_RESET_MASK) << MODEM_CORE_RESET_POSI));
	reg = ret | ((MODEM_CORE_RESET_WORD & MODEM_CORE_RESET_MASK)
				 << MODEM_CORE_RESET_POSI);
	REG_WRITE(SCTRL_MODEM_CORE_RESET_PHY_HCLK, reg);

	delay(1);
	reg = ret;
	REG_WRITE(SCTRL_MODEM_CORE_RESET_PHY_HCLK, reg);

	/*resetting, and waiting for done*/
	reg = REG_READ(SCTRL_RESET);
	while (reg & MODEM_CORE_RESET_BIT) {
		delay(10);
		reg = REG_READ(SCTRL_RESET);
	}
}

void sys_hal_mpif_invert(void)
{
	UINT32 reg;

	reg = REG_READ(SCTRL_CONTROL);
	reg |= MPIF_CLK_INVERT_BIT;
	REG_WRITE(SCTRL_CONTROL, reg);
}

void sys_hal_modem_subsys_reset(void)
{
 	UINT32 reg = 0;
	UINT32 reset_word = 0;

	reg = SCTRL_MODEM_SUBCHIP_RESET_REQ;
	reset_word = MODEM_SUBCHIP_RESET_WORD;

	REG_WRITE(reg, reset_word);
	delay(10);
	REG_WRITE(reg, 0);
}

void sys_hal_mac_subsys_reset(void)
{
	UINT32 reg = 0;
	UINT32 reset_word = 0;

	reg = SCTRL_MAC_SUBSYS_RESET_REQ;
	reset_word = MAC_SUBSYS_RESET_WORD;

	REG_WRITE(reg, reset_word);
	delay(10);
	REG_WRITE(reg, 0);
}

void sys_hal_usb_subsys_reset(void)
{
	UINT32 reg = 0;
	UINT32 reset_word = 0;

	reg = SCTRL_USB_SUBSYS_RESET_REQ;
	reset_word = USB_SUBSYS_RESET_WORD;

	REG_WRITE(reg, reset_word);
	delay(10);
	REG_WRITE(reg, 0);
}

void sys_hal_dsp_subsys_reset(void)
{
	UINT32 reg = 0;
	UINT32 reset_word = 0;

	reg = SCTRL_DSP_SUBSYS_RESET_REQ;
	reset_word = DSP_SUBSYS_RESET_WORD;

	REG_WRITE(reg, reset_word);
	delay(10);
	REG_WRITE(reg, 0);
}

void sys_hal_mac_power_ctrl(bool power_up)
{
 	UINT32 reg_val;
 	UINT32 reg_word = 0;

	reg_val = REG_READ(SCTRL_PWR_MAC_MODEM);
	reg_val &= ~(MAC_PWD_MASK << MAC_PWD_POSI);

	if (power_up)
 	{
		reg_val |= MAC_PWU << MAC_PWD_POSI;
	}
	else
	{
		reg_val |= MAC_PWD << MAC_PWD_POSI;
	}
	reg_word = reg_val;
	REG_WRITE(SCTRL_PWR_MAC_MODEM, reg_word);
}

void sys_hal_modem_power_ctrl(bool power_up)
{
	UINT32 reg_val;
	UINT32 reg_word = 0;

	reg_val = REG_READ(SCTRL_PWR_MAC_MODEM);
	reg_val &= ~(MODEM_PWD_MASK << MODEM_PWD_POSI);

	if (power_up)
	{
		reg_val |= MODEM_PWU << MODEM_PWD_POSI;
	}
	else
	{
		reg_val |= MODEM_PWD << MODEM_PWD_POSI;
	}
	reg_word = reg_val;
	REG_WRITE(SCTRL_PWR_MAC_MODEM, reg_word);
}

void sys_hal_pta_ctrl(bool pta_en)
{
	//no PTA
}

void sys_hal_modem_bus_clk_ctrl(bool clk_en)
{
	UINT32 reg;

#if (CONFIG_SOC_BK7271)
	reg = REG_READ(SCTRL_CONTROL);

	if (clk_en)
	{
		reg |= PHY_HCLK_EN_BIT;
	}
	else
	{
		reg &= ~PHY_HCLK_EN_BIT;
	}

	REG_WRITE(SCTRL_CONTROL, reg);
#else
	reg = REG_READ(SCTRL_MODEM_CORE_RESET_PHY_HCLK);

	if (clk_en)
	{
		reg |= PHY_HCLK_EN_BIT;
	}
	else
	{
		reg &= ~PHY_HCLK_EN_BIT;
	}

	REG_WRITE(SCTRL_MODEM_CORE_RESET_PHY_HCLK, reg);
#endif
}

void sys_hal_modem_clk_ctrl(bool clk_en)
{
	UINT32 reg;

	reg = REG_READ(SCTRL_CONTROL);

	if (clk_en)
	{
		reg &= ~MODEM_CLK480M_PWD_BIT;
	}
	else
	{
		reg |= MODEM_CLK480M_PWD_BIT;
	}

	REG_WRITE(SCTRL_CONTROL, reg);
}

void sys_hal_mac_bus_clk_ctrl(bool clk_en)
{
	UINT32 reg;

#if (CONFIG_SOC_BK7271)
	reg = REG_READ(SCTRL_CONTROL);

	if (clk_en)
	{
		reg |= MAC_HCLK_EN_BIT;
	}
	else
	{
		reg &= ~MAC_HCLK_EN_BIT;
	}

	REG_WRITE(SCTRL_CONTROL, reg);
#else
	reg = REG_READ(SCTRL_MODEM_CORE_RESET_PHY_HCLK);

	if (clk_en)
	{
		reg |= MAC_HCLK_EN_BIT;
	}
	else
	{
		reg &= ~MAC_HCLK_EN_BIT;
	}

	REG_WRITE(SCTRL_MODEM_CORE_RESET_PHY_HCLK, reg);
#endif

}

void sys_hal_mac_clk_ctrl(bool clk_en)
{
	UINT32 reg;

	reg = REG_READ(SCTRL_CONTROL);

	if (clk_en)
	{
		reg &= ~MAC_CLK480M_PWD_BIT;
	}
	else
	{
		reg |= MAC_CLK480M_PWD_BIT;
	}

	REG_WRITE(SCTRL_CONTROL, reg);
}


void sys_hal_set_vdd_value(uint32_t param)
{
	UINT32 reg;

	reg = REG_READ(SCTRL_DIGTAL_VDD);
	reg &= (~(DIG_VDD_ACTIVE_MASK << DIG_VDD_ACTIVE_POSI));
	reg |= param << DIG_VDD_ACTIVE_POSI;
	REG_WRITE(SCTRL_DIGTAL_VDD, reg);
}

uint32_t sys_hal_get_vdd_value(void)
{
	UINT32 reg;

	reg = REG_READ(SCTRL_DIGTAL_VDD);
	return (reg >> DIG_VDD_ACTIVE_POSI) & DIG_VDD_ACTIVE_MASK;
}

//CMD_SCTRL_BLOCK_EN_MUX_SET
void sys_hal_block_en_mux_set(uint32_t param)
{
	UINT32 reg;

	reg = REG_READ(SCTRL_BLOCK_EN_MUX);
	reg &= ~(0x1FF);
	reg |= param;
	REG_WRITE(SCTRL_BLOCK_EN_MUX, reg);
}
void sys_hal_enable_mac_gen_int(void)
{
	icu_ll_enable_mac_general_interrupt(s_icu_hal.hw);
}
void sys_hal_enable_mac_prot_int(void)
{
	icu_ll_enable_mac_prot_trigger_interrupt(s_icu_hal.hw);
}
void sys_hal_enable_mac_tx_trigger_int(void)
{
	icu_ll_enable_mac_tx_trigger_interrupt(s_icu_hal.hw);
}
void sys_hal_enable_mac_rx_trigger_int(void)
{
	icu_ll_enable_mac_rx_trigger_interrupt(s_icu_hal.hw);
}
void sys_hal_enable_mac_txrx_misc_int(void)
{
	icu_ll_enable_mac_txrx_misc_interrupt(s_icu_hal.hw);
}
void sys_hal_enable_mac_txrx_timer_int(void)
{
	icu_ll_enable_mac_txrx_timer_interrupt(s_icu_hal.hw);
}

void sys_hal_enable_modem_int(void)
{
	icu_ll_enable_modem_interrupt(s_icu_hal.hw);
}
void sys_hal_enable_modem_rc_int(void)
{
	//only 7236 has modem rc interrupt
}


//Yantao Add End


/**  WIFI End **/




/**  Misc Start **/
//Misc
/**  Misc End **/


