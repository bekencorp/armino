#include <common/bk_include.h>
#include "adc_hal.h"
#include "power_driver.h"
#include "clock_driver.h"
#include "icu_driver.h"
#include "bk_sys_ctrl.h"
#include "bk_drv_model.h"

//TODO the adc_hal should reset the ADC => by Frank!!!
bk_err_t adc_hal_init(adc_hal_t *hal)
{
	hal->hw = (adc_hw_t *)ADC_LL_REG_BASE();

	adc_ll_init(hal->hw);
	adc_ll_wait_4_cycle(hal->hw);
	adc_ll_disable_bypass_calib(hal->hw);
	adc_hal_set_saturate_mode(hal, ADC_INIT_SATURATE_MODE);
	return BK_OK;
}

bk_err_t adc_hal_deinit(adc_hal_t *hal)
{
	adc_ll_deinit(hal->hw);

	return BK_OK;
}

bk_err_t adc_hal_set_clk(adc_hal_t *hal, adc_src_clk_t src_clk, uint32_t adc_clk)
{
	uint32_t pre_div = 0;

	if (src_clk == ADC_SCLK_32M) {
		adc_ll_enable_32m_clk(hal->hw);
		pre_div = ADC_SRC_32M_CLK/2/adc_clk -1;
	} else {
		adc_ll_disable_32m_clk(hal->hw);
	}

	if (src_clk == ADC_SCLK_XTAL_26M) {
		clk_set_saradc_clk_26m();
		pre_div = ADC_SRC_26M_CLK/2/adc_clk -1;
	} else if (src_clk == ADC_SCLK_DCO) {
		clk_disable_saradc_audio_pll();
		clk_set_saradc_clk_dco();
		pre_div = ADC_SRC_DCO_CLK/2/adc_clk -1;
	} else if (src_clk == ADC_SCLK_DPLL) {
		clk_set_saradc_clk_dco();
		clk_enable_saradc_audio_pll();
		pre_div = ADC_SRC_DPLL_CLK/2/adc_clk -1;
	}

	adc_ll_set_pre_div(hal->hw, pre_div);

	return BK_OK;
}

#if SARADC_AUTOTEST
bk_err_t adc_hal_set_div(adc_hal_t *hal, uint32_t div)
{
    uint32_t pre_div = div;
    adc_ll_disable_32m_clk(hal->hw);
    clk_set_saradc_clk_26m();
    adc_ll_set_pre_div(hal->hw, pre_div);
    return BK_OK;
}
#endif

bk_err_t adc_hal_set_mode(adc_hal_t *hal, adc_mode_t adc_mode)
{
	if(adc_mode == ADC_SOFTWARE_CONTRL_MODE) {
		adc_ll_set_software_control_mode(hal->hw);
	} else if (adc_mode == ADC_CONTINUOUS_MODE) {
		adc_ll_set_continuous_mode(hal->hw);
	}else {
		adc_ll_set_single_step_mode(hal->hw);
	}

	return BK_OK;
}

bk_err_t adc_hal_set_saturate_mode(adc_hal_t *hal, adc_saturate_mode_t mode)
{
	uint32 saturate_mode = 0;

	if (mode == ADC_SATURATE_MODE_NONE) {
		adc_ll_disable_sat_process(hal->hw);
		return BK_OK;
	}

	if (mode == ADC_SATURATE_MODE_0) {
		saturate_mode = 0;
	} else if (mode == ADC_SATURATE_MODE_1) {
		saturate_mode = 1;
	}
#if ADC_ACURACY_12_BIT
	else if (mode == ADC_SATURATE_MODE_2) {
		saturate_mode = 2;
	}
#endif
	else {
		saturate_mode = 3;
	}

	adc_ll_set_sat_ctrl(hal->hw, saturate_mode);
	adc_ll_enable_sat_process(hal->hw);

	return BK_OK;
}

bk_err_t adc_hal_start_commom(adc_hal_t *hal)
{
	adc_ll_enable(hal->hw);

	return BK_OK;
}

bk_err_t adc_hal_stop_commom(adc_hal_t *hal)
{
	adc_ll_disable(hal->hw);

	return BK_OK;
}

uint16_t adc_hal_get_single_step_adc_data(adc_hal_t *hal)
{
	uint16_t value = 0;
	uint32_t mode = 0;
	mode  = adc_hal_get_mode(hal);

	BK_WHILE_DO(mode, mode = adc_hal_get_mode(hal));

	value = adc_hal_get_adc_data(hal);

	return value;
}

