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
#include <common/bk_compiler.h>
#include "icu_driver.h"
#include <os/mem.h>
#include <driver/pwm.h>
#include "pwm_hal_v1px.h"
#include "pwm_driver.h"
#include "clock_driver.h"
#include "power_driver.h"
#include <driver/int.h>
#include "sys_driver.h"
#include "gpio_driver.h"
#include <driver/gpio.h>

static void pwm_isr(pwm_id_t id) __BK_SECTION(".itcm");
static void pwm0_isr(void) __BK_SECTION(".itcm");
static void pwm1_isr(void) __BK_SECTION(".itcm");

//TODO list
// 1. Check if a PWM channel already in using
// 2. Support PWM timer/counter API

typedef struct {
	pwm_chan_t chan1;
	pwm_chan_t chan2;
	bool is_valid;
} pwm_group_info_t;

#define PWM_GROUP_NUM (SOC_PWM_CHAN_NUM_PER_UNIT >> 1)
#define PWM_GPIO_MODE_MAX	4

typedef struct {
	pwm_hal_t hal;
	//Important notes: currently no lock for bits
	//Concurrently operation is NOT allowed!!!
	uint32_t chan_init_bits;
	pwm_group_info_t groups[PWM_GROUP_NUM];
} pwm_driver_t;

static pwm_driver_t s_pwm[SOC_PWM_GROUP_NUM] = {0};
static pwm_isr_t s_pwm_isr[SOC_PWM_GROUP_NUM][SOC_PWM_CHAN_NUM_PER_UNIT] = {NULL};
static bool s_pwm_driver_is_init = false;

#define PWM_RETURN_ON_INVALID_CHAN(hw_ch) do {\
		if ((hw_ch) >= SOC_PWM_CHAN_NUM_PER_UNIT) {\
			return BK_ERR_PWM_CHAN_ID;\
		}\
	} while(0)

#define PWM_RETURN_ON_INVALID_CHAN_CLOCK(clock) do {\
		if ((clock) >= PWM_CHAN_CLOCK_MAX) {\
			return BK_ERR_PWM_CHAN_CLOCK;\
		}\
	} while(0)

#define PWM_RETURN_ON_NOT_INIT() do {\
		if (!s_pwm_driver_is_init) {\
			return BK_ERR_PWM_NOT_INIT;\
		}\
	} while(0)

#define PWM_RETURN_ON_CHAN_NOT_INIT(id, hw_ch) do {\
		if (!(s_pwm[id].chan_init_bits & BIT((hw_ch)))) {\
			return BK_ERR_PWM_CHAN_NOT_INIT;\
		}\
	} while(0)

#define PWM_RETURN_ON_INVALID_GROUP(hw_ch) do {\
		if (hw_ch >= PWM_GROUP_NUM) {\
			return BK_ERR_PWM_GROUP_ID;\
		}\
	} while(0)

#define PWM_RETURN_ON_INVALID_GPIO_MODE(hw_ch) do {\
			if (hw_ch >= PWM_GPIO_MODE_MAX) {\
				return BK_ERR_PWM_INVALID_GPIO_MODE;\
			}\
		} while(0)


#define PWM_RETURN_ON_CHAN_NOT_STOP(hw_ch) do {\
	} while(0)


typedef struct
{
	gpio_id_t gpio_id;
	gpio_dev_t gpio_dev;
}pwm_gpio_map;

static const pwm_gpio_map g_pwm_pin_id_map[SOC_PWM_GROUP_NUM][SOC_PWM_CHAN_NUM_PER_UNIT] = GPIO_PWM_MAP_TABLE;

static void pwm_sw_ch_to_hw_id_ch(pwm_ch_t sw_ch, pwm_id_t *id, pwm_ch_t *hw_ch)
{
#if (CONFIG_PWM_DEVICE0_SUPPORT && CONFIG_PWM_DEVICE1_SUPPORT)
	if(id)
		*id = sw_ch / SOC_PWM_CHAN_NUM_PER_UNIT;
	if(hw_ch)
		*hw_ch = sw_ch % SOC_PWM_CHAN_NUM_PER_UNIT;
#elif (CONFIG_PWM_DEVICE0_SUPPORT || CONFIG_PWM_DEVICE1_SUPPORT)
	PWM_LOGE("TODO:PWM allocs to different CPU", __func__);
	if(id)
		*id = 0;
	if(hw_ch)
		*hw_ch = sw_ch % SOC_PWM_CHAN_NUM_PER_UNIT;
#endif
}

static void pwm_init_gpio(pwm_ch_t sw_ch)
{
	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);

	gpio_dev_unmap(g_pwm_pin_id_map[id][hw_ch].gpio_id);
	gpio_dev_map(g_pwm_pin_id_map[id][hw_ch].gpio_id, g_pwm_pin_id_map[id][hw_ch].gpio_dev);
	bk_gpio_pull_up(g_pwm_pin_id_map[id][hw_ch].gpio_id);
}

//TODO call it via gpio hal
static void pwm_set_gpio(pwm_chan_t sw_ch)
{
	pwm_init_gpio(sw_ch);
}

static void pwm_chan_enable_interrupt_common(pwm_chan_t sw_ch)
{
	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);
	if(id == PWM_ID_0)
		sys_drv_int_enable(PWM_INTERRUPT_CTRL_BIT);
	else
		sys_drv_int_enable(PWM1_INTERRUPT_CTRL_BIT);

	PWM_LOGE("TODO:enable interrupt", __func__);
#if 0
	pwm_hal_enable_interrupt(&s_pwm[id].hal, hw_ch);
#endif
}

/* Once the global PWM interrupt is enabled, we never disable it.
 * It should be OK, because if all PWM are stop, there will be no
 * PWM interrupt even though the global PWM interrupt is enabled.
 *
 * We can choose to disable the global PWM interrupt if all PWM
 * channels are stopped, but it make the code complex!
 **/
static void pwm_chan_disable_interrupt_common(pwm_chan_t sw_ch)
{
	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);

	PWM_LOGE("TODO:enable interrupt", __func__);

#if 0
	pwm_hal_disable_interrupt(&s_pwm[id].hal, hw_ch);
	pwm_hal_clear_chan_interrupt_status(&s_pwm[id].hal, hw_ch);
#endif
}

/* Init common to PWM/Capture init
 * 1. Set channel init bit
 * 2. Power up channel
 * 3. Set channel specific clock
 */
static void pwm_chan_init_common(pwm_chan_t sw_ch)
{
	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);

	s_pwm[id].chan_init_bits |= BIT(hw_ch);

#if CONFIG_PWM_DEVICE0_SUPPORT
	if(id == PWM_ID_0)
		sys_drv_dev_clk_pwr_up(CLK_PWR_ID_PWM_1, CLK_PWR_CTRL_PWR_UP);
#endif

#if CONFIG_PWM_DEVICE1_SUPPORT
	if(id == PWM_ID_1)
		sys_drv_dev_clk_pwr_up(CLK_PWR_ID_PWM_2, CLK_PWR_CTRL_PWR_UP);
#endif
	//TODO call hal
	pwm_set_gpio(sw_ch);

	PWM_LOGE("TODO:%s:init signal high?", __func__);
#if 0
	pwm_hal_set_init_signal_high(&s_pwm[id].hal, hw_ch);
#endif
}

static void pwm_chan_deinit_common(pwm_chan_t sw_ch)
{
	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);

	s_pwm[id].chan_init_bits &= ~BIT(hw_ch);
	PWM_LOGE("TODO:%s:pwm_hal_stop_common", __func__);
#if 0
	pwm_hal_stop_common(&s_pwm[id].hal, hw_ch);
	pwm_hal_reset_config_to_default(&s_pwm[id].hal, hw_ch);
#endif

#if (CONFIG_SYSTEM_CTRL)
	sys_drv_dev_clk_pwr_up(CLK_PWR_ID_PWM_1, CLK_PWR_CTRL_PWR_DOWN);
	sys_drv_dev_clk_pwr_up(CLK_PWR_ID_PWM_2, CLK_PWR_CTRL_PWR_DOWN);
#else
	power_pwm_pwr_down(hw_ch);
#endif
}

bk_err_t bk_pwm_driver_init(void)
{
	uint32_t index = 0;

	if (s_pwm_driver_is_init)
		return BK_OK;

	//clock source
#if (CONFIG_SYSTEM_CTRL)
	sys_drv_pwm_select_clock(SYS_SEL_PWM0, PWM_SCLK_XTAL);
	sys_drv_pwm_select_clock(SYS_SEL_PWM1, PWM_SCLK_XTAL);
#else
	clk_set_pwms_clk_26m();
#endif

	//isr
	os_memset(&s_pwm_isr, 0, sizeof(s_pwm_isr));
#if (CONFIG_PWM_DEVICE0_SUPPORT)
	bk_int_isr_register(INT_SRC_PWM, pwm0_isr, NULL);
	sys_drv_int_enable(PWM_INTERRUPT_CTRL_BIT);
#endif
#if (CONFIG_PWM_DEVICE1_SUPPORT)
	bk_int_isr_register(INT_SRC_PWM1, pwm1_isr, NULL);
	sys_drv_int_enable(PWM_INTERRUPT_CTRL_BIT);
#endif

	//hal init
	os_memset(&s_pwm, 0, sizeof(s_pwm));
	index = 0;
#if (CONFIG_PWM_DEVICE0_SUPPORT)
	s_pwm[index].hal.id = PWM_ID_0;
	pwm_hal_init(&s_pwm[index].hal);
	index++;
#endif
#if (CONFIG_PWM_DEVICE1_SUPPORT)
	s_pwm[index].hal.id = PWM_ID_1;
	pwm_hal_init(&s_pwm[index].hal);
	index++;
#endif

	s_pwm_driver_is_init = true;
	return BK_OK;
}

bk_err_t bk_pwm_driver_deinit(void)
{
	if (!s_pwm_driver_is_init)
		return BK_OK;

	PWM_LOGE("TODO:sw_ch max value\n");

	for (int sw_ch = 0; sw_ch < SOC_PWM_CHAN_NUM_PER_UNIT; sw_ch++)
		pwm_chan_deinit_common(sw_ch);

	s_pwm_driver_is_init = false;
	return BK_OK;
}

static void pwm_adjust_init_signal_via_duty(pwm_chan_t sw_ch, uint32_t period,
		uint32_t duty1, uint32_t duty2, uint32_t duty3)
{
	PWM_LOGE("TODO:%s:init signal high?", __func__);
#if 1
	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);

	/** If duty ratio is 0% and initial signal is high,
	 * then the PWM signal will always be high.
	 * To avoid that, we manually configure init signal to low if
	 * duty ratio is 0%.
	 */
	if (duty1 == 0)
		bk_gpio_pull_down(g_pwm_pin_id_map[id][hw_ch].gpio_id);

	/** If duty ratio is 100% and initial signal is low,
	 * then the PWM signal will always be low.
	 * To avoid that, we manually configure init signal to high if
	 * duty ratio is 100%.
	 */
	if (duty1 == period)
		bk_gpio_pull_up(g_pwm_pin_id_map[id][hw_ch].gpio_id);
#endif
}

static inline bool pwm_driver_duty_is_valid(uint32_t period, uint32_t duty1, uint32_t duty2,
										uint32_t duty3)
{
	if (period == 0)
		return false;

	//Not consider (duty1 + duty2 + duty3) overflow
	if ((duty1 + duty2 + duty3) > period)
		return false;

	//Not consier (duty2 + duty3) overflow
	if ((duty1 == 0) && ((duty2 + duty3) != 0))
		return false;

	if ((duty2 == 0) && (duty3 != 0))
		return false;

	return true;
}

static void pwm_single_chan_output_config(pwm_chan_t sw_ch, int end_val, int psc, int cc)
{
	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);

	pwm_hal_get_pwm_cr1_arpe1(&s_pwm[id].hal); 			//enable arr1 preload
	pwm_hal_set_tim1_arr_tim1_arr(&s_pwm[id].hal, end_val); 			 //end value arr1 0x60
	pwm_hal_set_prescaler_psc1(&s_pwm[id].hal, psc) ;		   //prescale is 5 
	pwm_hal_set_ccrx(&s_pwm[id].hal, hw_ch, cc);
	pwm_hal_set_pwm_ccmr_oc1m(&s_pwm[id].hal, 0x3) ;			 //toggle at ccr1 and arr1
	pwm_hal_set_pwm_dier_cc1ie(&s_pwm[id].hal, true) ;			 //enable ccr1 int
	pwm_hal_set_pwm_ccmr_ch1e(&s_pwm[id].hal, true);			  //enable pwm_o[0]
	pwm_hal_set_pwm_ccmr_ch1p(&s_pwm[id].hal, 0x01) ;			 //set pwm_o[1] as complementary of pwm_o[0] 
	pwm_hal_set_pwm_edtr_ug1(&s_pwm[id].hal, 0x01); 			 //update ccr1 and arr1
}

static void pwm_output_mode_config(pwm_chan_t sw_ch, int end_val, int psc, int cc, int ddt)
{
	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);

	pwm_hal_set_ccrx(&s_pwm[id].hal, hw_ch, cc);			 //ccr1 is 0x20

    switch(hw_ch)
    {
        case 0:	//PWM_CHANNEL0:
			pwm_hal_set_pwm_cr1_arpe1(&s_pwm[id].hal, 1) ;             //enable arr1 preload
			pwm_hal_set_tim1_arr_tim1_arr(&s_pwm[id].hal, end_val);				 //end value arr1 0x60
			pwm_hal_set_prescaler_psc1(&s_pwm[id].hal, psc) ;          //prescale is 5 
			//pwm_hal_set_ccrx(&s_pwm[id].hal, hw_ch, cc);             //ccr1 is 0x20
			pwm_hal_set_pwm_ccmr_oc1m(&s_pwm[id].hal, 0x3) ;		     //toggle at ccr1 and arr1
			pwm_hal_set_pwm_dier_cc1ie(&s_pwm[id].hal, true) ;			 //enable ccr1 int
			pwm_hal_set_pwm_ccmr_ch1e(&s_pwm[id].hal, true);              //enable pwm_o[0]
			pwm_hal_set_pwm_ccmr_ch2e(&s_pwm[id].hal, true);              //enable pwm_o[1]
			pwm_hal_set_dt_dt1(&s_pwm[id].hal, ddt) ;				 //set deadtime as 5 cycle
			pwm_hal_set_pwm_ccmr_ch1p(&s_pwm[id].hal, 0x01) ;			 //set pwm_o[1] as complementary of pwm_o[0] 
			pwm_hal_set_pwm_edtr_ug1(&s_pwm[id].hal, 0x01);				 //update ccr1 and arr1
            break;
        case 2:	//PWM_CHANNEL2:
#if 0
			setf_PWM_0_Reg0x4_ARPE2 ;             //enable arr2 preload
			addPWM_0_Reg0x10 = end_val ;				 //end value arr2 0x60
			set_PWM_0_Reg0xe_PSC2(psc) ;          //prescale is 5 
			addPWM_0_Reg0x18 = cc ;             //ccr4 is 0x20
			set_PWM_0_Reg0xa_OC2M(0x3) ;		     //toggle at ccr1 and arr1
            //setf_PWM_0_Reg0x7_CC4IE ;			 //enable ccr4 int
			setf_PWM_0_Reg0xa_CH3E ;              //enable pwm_o[2]
			setf_PWM_0_Reg0xa_CH4E ;              //enable pwm_o[3]
			set_PWM_0_Reg0x1e_DT2(ddt) ;				 //set deadtime as 5 cycle
			set_PWM_0_Reg0xa_CH3P(0x01) ;			 //set pwm_o[3] as complementary of pwm_o[4] 
			setf_PWM_0_Reg0x9_UG2 ;				 //update ccr4 and arr2				
#endif
			pwm_hal_set_pwm_cr1_arpe2(&s_pwm[id].hal, 1) ;             //enable arr1 preload
			pwm_hal_set_tim2_arr_tim2_arr(&s_pwm[id].hal, end_val);				 //end value arr1 0x60
			pwm_hal_set_prescaler_psc2(&s_pwm[id].hal, psc) ;          //prescale is 5 
			//pwm_hal_set_ccr4_value(&s_pwm[id].hal, cc);             //ccr1 is 0x20
			pwm_hal_set_pwm_ccmr_oc2m(&s_pwm[id].hal, 0x3) ;		     //toggle at ccr1 and arr1
			pwm_hal_set_pwm_dier_cc3ie(&s_pwm[id].hal, true) ;			 //enable ccr1 int
			pwm_hal_set_pwm_ccmr_ch3e(&s_pwm[id].hal, true);              //enable pwm_o[0]
			pwm_hal_set_pwm_ccmr_ch4e(&s_pwm[id].hal, true);              //enable pwm_o[1]
			pwm_hal_set_dt_dt2(&s_pwm[id].hal, ddt) ;				 //set deadtime as 5 cycle
			pwm_hal_set_pwm_ccmr_ch3p(&s_pwm[id].hal, 0x01) ;			 //set pwm_o[1] as complementary of pwm_o[0] 
			pwm_hal_set_pwm_edtr_ug2(&s_pwm[id].hal, 0x01);				 //update ccr1 and arr1
            break;
        case 4:	//PWM_CHANNEL4:
#if 0
			setf_PWM_0_Reg0x4_ARPE3 ;             //enable arr3 preload
			addPWM_0_Reg0x11 = end_val ;			 //end value arr3 0x60
			set_PWM_0_Reg0xe_PSC3(psc) ;          //prescale is 5 
			addPWM_0_Reg0x1b = cc ;               //ccr4 is 0x20
			set_PWM_0_Reg0xa_OC3M(0x3) ;		     //toggle at ccr4 and arr2
            //setf_PWM_0_Reg0x7_CC7IE ;			 //enable ccr1 int
			setf_PWM_0_Reg0xa_CH5E ;              //enable pwm_o[4]
			setf_PWM_0_Reg0xa_CH6E ;              //enable pwm_o[5]
			set_PWM_0_Reg0x1e_DT3(ddt) ;			 //set deadtime as 5 cycle
			set_PWM_0_Reg0xa_CH5P(0x01) ;			 //set pwm_o[5] as complementary of pwm_o[4] 
			setf_PWM_0_Reg0x9_UG3 ;				 //update ccr7 and arr3
#endif
			pwm_hal_set_pwm_cr1_arpe3(&s_pwm[id].hal, 1) ;             //enable arr1 preload
			pwm_hal_set_tim3_arr_tim3_arr(&s_pwm[id].hal, end_val);				 //end value arr1 0x60
			pwm_hal_set_prescaler_psc3(&s_pwm[id].hal, psc) ;          //prescale is 5 
			//pwm_hal_set_ccr7_value(&s_pwm[id].hal, cc);             //ccr1 is 0x20
			pwm_hal_set_pwm_ccmr_oc3m(&s_pwm[id].hal, 0x3) ;		     //toggle at ccr1 and arr1
			pwm_hal_set_pwm_dier_cc7ie(&s_pwm[id].hal, true) ;			 //enable ccr1 int
			pwm_hal_set_pwm_ccmr_ch5e(&s_pwm[id].hal, true);              //enable pwm_o[0]
			pwm_hal_set_pwm_ccmr_ch6e(&s_pwm[id].hal, true);              //enable pwm_o[1]
			pwm_hal_set_dt_dt3(&s_pwm[id].hal, ddt) ;				 //set deadtime as 5 cycle
			pwm_hal_set_pwm_ccmr_ch6p(&s_pwm[id].hal, 0x01) ;			 //set pwm_o[1] as complementary of pwm_o[0] 
			pwm_hal_set_pwm_edtr_ug3(&s_pwm[id].hal, 0x01);				 //update ccr1 and arr1

			break;
        default:
            break;
    }
}

void pwm_output_mode_init(uint32_t channel, int end_val, int psc, int cc, int ddt )
{
	pwm_output_mode_config(channel, end_val, psc, cc, ddt);
}

bk_err_t bk_pwm_init(pwm_chan_t sw_ch, const pwm_init_config_t *config)
{
#if 1
	PWM_RETURN_ON_NOT_INIT();
	BK_RETURN_ON_NULL(config);

	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);
	PWM_RETURN_ON_INVALID_CHAN(hw_ch);

	if (!pwm_driver_duty_is_valid(config->period_cycle, config->duty_cycle,
		config->duty2_cycle, config->duty3_cycle))
		return BK_ERR_PWM_PERIOD_DUTY;

	pwm_chan_init_common(sw_ch);

	PWM_LOGI("TODO:precision should be considered\n");
	pwm_adjust_init_signal_via_duty(hw_ch, config->period_cycle, config->duty_cycle,
				config->duty2_cycle, config->duty3_cycle);

	pwm_hal_config_t hal_config = {0};
	hal_config.t1 = config->duty_cycle;

	PWM_LOGI("init, hw_ch(%d), t1=%x, t2=%x, t3=%x, t4=%x\n",
			 hw_ch, hal_config.t1, hal_config.t2, hal_config.t3, hal_config.t4);
	PWM_LOGE("TODO:pwm param needs to dynamic control\r\n");
	PWM_LOGE("TODO:%s,fixed psc", __func__);
	pwm_output_mode_init(sw_ch, (config->period_cycle), 25, config->duty_cycle, 0x5);

	//return pwm_hal_init_pwm(&s_pwm[id].hal, hw_ch, &hal_config);
#endif
	return BK_OK;
}

bk_err_t bk_pwm_deinit(pwm_chan_t sw_ch)
{
	pwm_chan_deinit_common(sw_ch);
	return BK_OK;
}

bk_err_t bk_pwm_start(pwm_chan_t sw_ch)
{
	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);
	PWM_RETURN_ON_CHAN_NOT_INIT(id, hw_ch);

	pwm_hal_set_pwm_cenx(&s_pwm[id].hal, hw_ch, 1);
	return BK_OK;
}

bk_err_t bk_pwm_stop(pwm_chan_t sw_ch)
{
	PWM_LOGE("TODO:%s", __func__);
#if 0
	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);
	PWM_RETURN_ON_CHAN_NOT_INIT(id, hw_ch);
	pwm_hal_stop_common(&s_pwm[id].hal, hw_ch);
#endif
	return BK_OK;
}

bk_err_t bk_pwm_register_isr(pwm_chan_t sw_ch, pwm_isr_t isr)
{
	PWM_RETURN_ON_NOT_INIT();

	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);
	PWM_RETURN_ON_INVALID_CHAN(hw_ch);

	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();
	s_pwm_isr[id][hw_ch] = isr;
	GLOBAL_INT_RESTORE();

	return BK_OK;
}

/* The PWM channel should be started before interrupt can be enabled.
 */
bk_err_t bk_pwm_enable_interrupt(pwm_chan_t sw_ch)
{
	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);
	PWM_RETURN_ON_CHAN_NOT_INIT(id, hw_ch);
	PWM_RETURN_ON_INVALID_CHAN(hw_ch);

	pwm_chan_enable_interrupt_common(sw_ch);
	return BK_OK;
}

bk_err_t bk_pwm_disable_interrupt(pwm_chan_t sw_ch)
{
#if 0
	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);
	PWM_RETURN_ON_INVALID_CHAN(hw_ch);

	pwm_chan_disable_interrupt_common(hw_ch);
#endif
	return BK_OK;
}

bk_err_t bk_pwm_set_period_duty(pwm_chan_t sw_ch, pwm_period_duty_config_t *config)
{
#if 0
	uint32_t t1 = 0;
	uint32_t t2 = 0;
	uint32_t t3 = 0;
	uint32_t t4 = 0;

	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);
	PWM_RETURN_ON_INVALID_CHAN(hw_ch);

	BK_RETURN_ON_NULL(config);
	PWM_RETURN_ON_INVALID_CHAN(hw_ch);
	PWM_RETURN_ON_CHAN_NOT_INIT(id, hw_ch);

	if (!pwm_hal_is_duty_valid(config->period_cycle, config->duty_cycle,
							   config->duty2_cycle, config->duty3_cycle))
		return BK_ERR_PWM_PERIOD_DUTY;

	pwm_adjust_init_signal_via_duty(hw_ch, config->period_cycle, config->duty_cycle,
									config->duty2_cycle, config->duty3_cycle);

	pwm_hal_set_t1(&s_pwm[id].hal, hw_ch, config->duty_cycle);

	t1 = config->duty_cycle;
	t2 = config->duty2_cycle;
	if (t2 > 0)
		t2 += t1;

	t3 = config->duty3_cycle;
	if (t3 > 0)
		t3 += t2;

	t4 = config->period_cycle;

	pwm_hal_set_t1(&s_pwm[id].hal, hw_ch, t1);
	pwm_hal_set_t2(&s_pwm[id].hal, hw_ch, t2);
	pwm_hal_set_t3(&s_pwm[id].hal, hw_ch, t3);
	pwm_hal_set_t4(&s_pwm[id].hal, hw_ch, t4);

	//The new configuration take effect only loading it
	pwm_hal_load_new_config(&s_pwm[id].hal, hw_ch);
#endif
	return BK_OK;
}

static bool pwm_is_0_duty_ratio(pwm_chan_t sw_ch)
{
#if 0
	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);
	PWM_RETURN_ON_INVALID_CHAN(hw_ch);

	return (pwm_hal_get_t1(&s_pwm[id].hal, hw_ch) == 0);
#endif
	return 0;
}

static bool pwm_is_100_duty_ratio(pwm_chan_t sw_ch)
{
#if 0
	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);
	PWM_RETURN_ON_INVALID_CHAN(hw_ch);

	return (pwm_hal_get_t1(&s_pwm[id].hal, hw_ch) == pwm_hal_get_t4(&s_pwm[id].hal, hw_ch));
#endif
	return 0;
}

bk_err_t bk_pwm_set_init_signal_low(pwm_chan_t sw_ch)
{
#if 0
	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);
	PWM_RETURN_ON_INVALID_CHAN(hw_ch);
	PWM_RETURN_ON_CHAN_NOT_INIT(id, hw_ch);

	if (pwm_is_100_duty_ratio(sw_ch))
		pwm_hal_set_init_signal_high(&s_pwm[id].hal, hw_ch);
	else
		pwm_hal_set_init_signal_low(&s_pwm[id].hal, hw_ch);

	pwm_hal_load_new_config(&s_pwm[id].hal, hw_ch);
#endif
	return BK_OK;
}

bk_err_t bk_pwm_set_init_signal_high(pwm_chan_t sw_ch)
{
#if 0
	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);
	PWM_RETURN_ON_INVALID_CHAN(hw_ch);
	PWM_RETURN_ON_CHAN_NOT_INIT(id, hw_ch);

	if (pwm_is_0_duty_ratio(hw_ch))
		pwm_hal_set_init_signal_low(&s_pwm[id].hal, hw_ch);
	else
		pwm_hal_set_init_signal_high(&s_pwm[id].hal, hw_ch);
#endif
	return BK_OK;
}

//TODO finish it
bk_err_t bk_pwm_set_gpio(pwm_chan_t sw_ch, uint32 mode)
{
	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);

	PWM_RETURN_ON_INVALID_CHAN(hw_ch);
	PWM_RETURN_ON_CHAN_NOT_INIT(id, hw_ch);
	PWM_RETURN_ON_INVALID_GPIO_MODE(id);

	pwm_set_gpio(id);

	return BK_OK;
}

bk_err_t bk_pwm_capture_init(pwm_chan_t sw_ch, const pwm_capture_init_config_t *config)
{
	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);
	pwm_chan_init_common(sw_ch);

	pwm_hal_set_pwm_ursx(&s_pwm[id].hal, hw_ch/2, 1) ;
	pwm_hal_set_timx_arr(&s_pwm[id].hal, hw_ch/2, 0xffffffff);
	//
	PWM_LOGE("TODO:%s,fixed psc", __func__);
	pwm_hal_set_prescaler_pscx(&s_pwm[id].hal, hw_ch/2, 25);
	pwm_hal_set_pwm_dier_ccxie(&s_pwm[id].hal, hw_ch, 1);	//ccr2,5,8

	pwm_hal_set_pwm_ccmr_chxe(&s_pwm[id].hal, hw_ch, 1);	//channel 2

	pwm_hal_set_timx_ccm(&s_pwm[id].hal, (hw_ch/2), 1);			 //CAPTURE mode
	pwm_hal_set_smsx(&s_pwm[id].hal, (hw_ch/2), 5)	;			 //clear timer every capture

	PWM_LOGE("TODO:%s,detect input type should change from hw_ch to xxx", __func__);
	pwm_hal_set_tsx(&s_pwm[id].hal, (hw_ch/2), hw_ch);

#if 0
	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);
	PWM_RETURN_ON_INVALID_CHAN(hw_ch);
	PWM_RETURN_ON_CHAN_NOT_INIT(id, hw_ch);

	PWM_RETURN_ON_NOT_INIT();
	BK_RETURN_ON_NULL(config);
	PWM_RETURN_ON_INVALID_CHAN(hw_ch);

	if (!pwm_hal_is_capture_edge_valid(config->edge))
		return BK_ERR_PWM_CAPTURE_EDGE;

	pwm_chan_init_common(hw_ch);
	s_pwm_isr[id][hw_ch] = config->isr;

	pwm_hal_capture_config_t hal_config = {
		.edge = config->edge,
	};

	return pwm_hal_init_capture(&s_pwm[id].hal, hw_ch, &hal_config);
#endif
	return BK_OK;
}

bk_err_t bk_pwm_capture_deinit(pwm_chan_t sw_ch)
{
#if 0
	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);

	PWM_RETURN_ON_NOT_INIT();
	PWM_RETURN_ON_INVALID_CHAN(hw_ch);
	pwm_chan_deinit_common(hw_ch);
#endif
	return BK_OK;
}

bk_err_t bk_pwm_capture_start(pwm_chan_t sw_ch)
{
#if 1
	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);

	PWM_RETURN_ON_CHAN_NOT_INIT(id, hw_ch);
	PWM_RETURN_ON_INVALID_CHAN(hw_ch);
	pwm_hal_set_pwm_cenx(&s_pwm[id].hal, hw_ch, 1);
	pwm_chan_enable_interrupt_common(sw_ch); //interrupt should be enabled after capture start
#endif
	return BK_OK;
}

bk_err_t bk_pwm_capture_stop(pwm_chan_t sw_ch)
{
	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);

	PWM_RETURN_ON_CHAN_NOT_INIT(id, hw_ch);
	PWM_RETURN_ON_INVALID_CHAN(hw_ch);

	PWM_LOGE("TODO:%s:pwm_hal_stop_common", __func__);
#if 0
	pwm_hal_stop_common(&s_pwm[id].hal, hw_ch);
#endif
	return BK_OK;
}

//It's up the caller to make sure the channel ID is correct
uint32_t bk_pwm_capture_get_value(pwm_chan_t sw_ch)
{
#if 0
	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);
	return pwm_hal_get_capture_value(&s_pwm[id].hal, hw_ch);
#endif
	return 0;
}

static pwm_group_t pwm_group_find_channel(pwm_chan_t sw_ch)
{
	PWM_LOGI("TODO:%s\n", __func__);

#if 0
	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);

	PWM_RETURN_ON_CHAN_NOT_INIT(id, hw_ch);
	PWM_RETURN_ON_INVALID_CHAN(hw_ch);

	for (int i = 0; i < PWM_GROUP_NUM; i++) {
		if (s_pwm[id].groups[i].is_valid && ((s_pwm[id].groups[i].chan1 == hw_ch) ||
										 (s_pwm[id].groups[i].chan2 == hw_ch)))
			return i;
	}
#endif
	return PWM_GROUP_ID_INVALID;
}

static pwm_group_t pwm_group_find(pwm_chan_t chan1, pwm_chan_t chan2)
{
#if 0
	for (int i = 0; i < PWM_GROUP_NUM; i++) {
		if (s_pwm.groups[i].is_valid) {

			if ((s_pwm.groups[i].chan1 == chan1) &&
				(s_pwm.groups[i].chan2 == chan2))
				return i;

			if ((s_pwm.groups[i].chan1 == chan2) &&
				(s_pwm.groups[i].chan2 == chan1))
				return i;
		}
	}
#endif
	return PWM_GROUP_ID_INVALID;
}

static bool pwm_group_is_existed(pwm_group_t group)
{
#if 0
	if (group >= PWM_GROUP_NUM)
		return false;

	return (s_pwm.groups[group].is_valid == true);
#endif
	return false;
}

static pwm_group_t pwm_group_add(pwm_chan_t chan1, pwm_chan_t chan2)
{
#if 0
	for (int i = 0; i < PWM_GROUP_NUM; i++) {
		if (!s_pwm.groups[i].is_valid) {
			s_pwm.groups[i].is_valid = true;
			s_pwm.groups[i].chan1 = chan1;
			s_pwm.groups[i].chan2 = chan2;
			return i;
		}
	}
#endif
	return PWM_GROUP_ID_INVALID;
}

static void pwm_group_delete(pwm_group_t group)
{
#if 0
	if (group < PWM_GROUP_NUM)
		s_pwm.groups[group].is_valid = false;
#endif
}

static bk_err_t pwm_group_validate_param(pwm_chan_t sw_ch1, pwm_chan_t sw_ch2,
		uint32_t period_cycle, uint32_t chan1_duty_cycle, uint32_t chan2_duty_cycle)

{
#if 0
	pwm_id_t id1, id2;
	pwm_ch_t hw_ch1, hw_ch2;

	pwm_sw_ch_to_hw_id_ch(sw_ch1, &id1, &hw_ch1);
	pwm_sw_ch_to_hw_id_ch(sw_ch2, &id2, &hw_ch2);

	PWM_RETURN_ON_INVALID_CHAN(hw_ch1);
	PWM_RETURN_ON_INVALID_CHAN(hw_ch2);

	if (hw_ch1 == hw_ch2)
		return BK_ERR_PWM_GROUP_SAME_CHAN;

	if (PWM_GROUP_ID_INVALID != pwm_group_find(hw_ch1, hw_ch2))
		return BK_ERR_PWM_GROUP_EXIST;

	if (PWM_GROUP_ID_INVALID != pwm_group_find_channel(hw_ch1))
		return BK_ERR_PWM_GROUP_CHAN_USED;

	if (PWM_GROUP_ID_INVALID != pwm_group_find_channel(hw_ch2))
		return BK_ERR_PWM_GROUP_CHAN_USED;

	if (period_cycle < (chan1_duty_cycle + chan2_duty_cycle))
		return BK_ERR_PWM_GROUP_DUTY;

	//duty is 0 is meanless
	if ((chan1_duty_cycle == 0) || (chan2_duty_cycle == 0))
		return BK_ERR_PWM_GROUP_DUTY;
#endif
	return BK_OK;
}

bk_err_t bk_pwm_group_init(const pwm_group_init_config_t *config, pwm_group_t *group)
{
#if 0
	pwm_init_config_t pwm_config;
	uint32_t dead_cycle = 0;
	int ret;

	PWM_RETURN_ON_NOT_INIT();
	BK_RETURN_ON_NULL(group);

	*group = PWM_GROUP_ID_INVALID;
	ret = pwm_group_validate_param(config->chan1, config->chan2, config->period_cycle,
								   config->chan1_duty_cycle, config->chan2_duty_cycle);
	if (ret != BK_OK)
		return ret;

	/* Relationship between period/duty
	 *
	 * T14/T24 - Period cycle P, P = period_cycle
	 * T11 - PWM channel 1 duty cycle, T11 = chan1_duty_cycle
	 * T21 - PWM channel 2 signal reverse time1
	 * T22 - PWM cahnnel 2 signal reverse time2
	 * D2 = (T22 - T21) = chan2_duty_cycle, it's channe 2's high signal duty cycle
	 * D = (P - chan1_duty_cycle - chan2_duty_cycle)/2, the dead cycle
	 *
	 * T21 = T11 + D
	 * T22 = T21 + chan2_duty_cycle
	 *
	 *                            |<--D----->|<--D2-----|<--D----->|
	 *              ______________
	 *  channe1    |              |________________________________|
	 *             0              T11                              T14
	 *                                        __________
	 *  channel2   |_________________________|          |__________|
	 *             0                         T21        T22        T24
	 *
	 */
	pwm_config.period_cycle = config->period_cycle;
	pwm_config.duty_cycle = config->chan1_duty_cycle;
	pwm_config.duty2_cycle = 0;
	pwm_config.duty3_cycle = 0;
	ret = bk_pwm_init(config->chan1, &pwm_config);
	if (ret != BK_OK)
		return ret;
	pwm_hal_set_init_signal_high(&s_pwm[id].hal, config->chan1);

	dead_cycle = (config->period_cycle - config->chan1_duty_cycle - config->chan2_duty_cycle) >> 1;
	pwm_config.duty_cycle = config->chan1_duty_cycle + dead_cycle;
	pwm_config.duty2_cycle = config->chan2_duty_cycle;
	ret = bk_pwm_init(config->chan2, &pwm_config);
	if (ret != BK_OK)
		return ret;
	pwm_hal_set_init_signal_low(&s_pwm[id].hal, config->chan2);

	//TODO peter adjust the PWM initial signal according to DUTY value

	*group = pwm_group_add(config->chan1, config->chan2);

	PWM_LOGI("group period=%x chan1_duty=%x chan2_duty=%x\n",
			 config->period_cycle, config->chan1_duty_cycle, config->chan2_duty_cycle);
	PWM_LOGI("group T21=%x T22=%x\n", pwm_config.duty_cycle, pwm_config.duty2_cycle);
#endif
	return BK_OK;
}

bk_err_t bk_pwm_group_deinit(pwm_group_t group)
{
#if 0
	if (pwm_group_is_existed(group))
		return BK_OK;

	bk_pwm_group_stop(group);
	pwm_chan_deinit_common(s_pwm.groups[group].chan1);
	pwm_chan_deinit_common(s_pwm.groups[group].chan2);
	pwm_group_delete(group);
#endif
	return BK_OK;
}

bk_err_t bk_pwm_group_start(pwm_group_t group)
{
	PWM_LOGI("TODO:%s\n", __func__);
#if 0
	pwm_chan_t chan1;
	pwm_chan_t chan2;

	if (!pwm_group_is_existed(group))
		return BK_ERR_PWM_GROUP_NOT_EXIST;

	chan1 = s_pwm.groups[group].chan1;
	chan2 = s_pwm.groups[group].chan2;
	if (pwm_hal_is_hardware_group(&s_pwm[id].hal, chan1, chan2))
		pwm_hal_start_hardware_group(&s_pwm[id].hal, chan1, chan2);

	pwm_hal_start_common(&s_pwm[id].hal, chan1);
	pwm_hal_start_common(&s_pwm[id].hal, chan2);
#endif
	return BK_OK;
}

bk_err_t bk_pwm_group_stop(pwm_group_t group)
{
	PWM_LOGI("TODO:%s\n", __func__);

#if 0
	pwm_chan_t chan1;
	pwm_chan_t chan2;

	if (!pwm_group_is_existed(group))
		return BK_OK;

	chan1 = s_pwm.groups[group].chan1;
	chan2 = s_pwm.groups[group].chan2;
	if (pwm_hal_is_hardware_group(&s_pwm[id].hal, chan1, chan2))
		pwm_hal_stop_hardware_group(&s_pwm[id].hal, chan1, chan2);

	pwm_hal_stop_common(&s_pwm[id].hal, chan1);
	pwm_hal_stop_common(&s_pwm[id].hal, chan2);
#endif
	return BK_OK;
}

bk_err_t bk_pwm_group_set_config(pwm_group_t group, const pwm_group_config_t *config)
{
	return BK_OK;
}

bk_err_t bk_pwm_group_set_init_signal(pwm_group_t group, const pwm_group_config_t *config)
{
	return BK_OK;
}

bk_err_t bk_pwm_set_mode_timer(pwm_chan_t sw_ch)
{
#if 0
	pwm_id_t id;
	pwm_ch_t hw_ch;

	pwm_sw_ch_to_hw_id_ch(sw_ch, &id, &hw_ch);

	PWM_RETURN_ON_INVALID_CHAN(hw_ch);

	pwm_hal_set_mode_timer(&s_pwm[id].hal, hw_ch);
#endif
	return BK_OK;
}

static void pwm_isr(pwm_id_t id)
{
#if 1
	pwm_hal_t *hal = &s_pwm[id].hal;
	uint32_t int_status;
	int hw_ch;

static uint32_t s_entry_cnt = 0;

	int_status = pwm_hal_get_pwm_interrupt_status(hal);
	pwm_hal_clr_interrupt_status(hal, int_status);
	s_entry_cnt++;
	if(s_entry_cnt % 10 == 0)
	{
		//pwm_hal_set_tim1_rcr_value(hal, 0x100 *(1 << (s_entry_cnt/10)) * s_entry_cnt);
	}

PWM_LOGI("TODO:%s s_entry_cnt=%d, int_status=%d\n", __func__, s_entry_cnt, int_status);
//TODO:
	for (hw_ch = 0; hw_ch < SOC_PWM_CHAN_NUM_PER_UNIT; hw_ch++) {
		if (pwm_hal_get_pwm_sr_ccxif(hal, hw_ch)) {
			if (s_pwm_isr[id][hw_ch])
				s_pwm_isr[id][hw_ch](hw_ch);
		}
	}

#endif
}

#if (CONFIG_PWM_DEVICE0_SUPPORT)
static void pwm0_isr(void)
{
	PWM_LOGI("TODO:%s\n", __func__);
	pwm_isr(0);
}
#endif

#if (CONFIG_PWM_DEVICE1_SUPPORT)
static void pwm1_isr(void)
{
	PWM_LOGI("TODO:%s\n", __func__);
	pwm_isr(1);
}
#endif

