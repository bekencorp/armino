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
#include "pwm_hal.h"
#include "pwm_driver.h"
#include "clock_driver.h"
#include "power_driver.h"
#include <driver/int.h>
#include "sys_driver.h"
#include "gpio_driver.h"
#include <driver/gpio.h>

static void pwm_isr(void) __BK_SECTION(".itcm");

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

pwm_driver_t s_pwm = {0};
static pwm_isr_t s_pwm_isr[SOC_PWM_CHAN_NUM_PER_UNIT] = {NULL};
static bool s_pwm_driver_is_init = false;

#define PWM_RETURN_ON_INVALID_CHAN(chan) do {\
		if ((chan) >= SOC_PWM_CHAN_NUM_PER_UNIT) {\
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

#define PWM_RETURN_ON_CHAN_NOT_INIT(chan) do {\
		if (!(s_pwm.chan_init_bits & BIT((chan)))) {\
			return BK_ERR_PWM_CHAN_NOT_INIT;\
		}\
	} while(0)

#define PWM_RETURN_ON_INVALID_GROUP(chan) do {\
		if (chan >= PWM_GROUP_NUM) {\
			return BK_ERR_PWM_GROUP_ID;\
		}\
	} while(0)

#define PWM_RETURN_ON_INVALID_GPIO_MODE(chan) do {\
			if (chan >= PWM_GPIO_MODE_MAX) {\
				return BK_ERR_PWM_INVALID_GPIO_MODE;\
			}\
		} while(0)


#define PWM_RETURN_ON_CHAN_NOT_STOP(chan) do {\
	} while(0)

#define PWM_SET_PIN(id) do {\
	gpio_dev_unmap(PWM##id##_LL_PIN);\
	gpio_dev_map(PWM##id##_LL_PIN, GPIO_DEV_PWM##id);\
	bk_gpio_pull_up(PWM##id##_LL_PIN);\
} while(0)

static void pwm_init_gpio(pwm_id_t id);
static void pwm_init_gpio(pwm_id_t id)
{
#if (SOC_PWM_CHAN_NUM_PER_UNIT > 6)
	if(id == 10) {
		PWM_SET_PIN(10);
		return;
	}

	if(id == 11) {
		PWM_SET_PIN(11);
		return;
	}
#endif

	switch(id) {
	case PWM_ID_0:
		PWM_SET_PIN(0);
		break;

	case PWM_ID_1:
		PWM_SET_PIN(1);
		break;

	case PWM_ID_2:
		PWM_SET_PIN(2);
		break;

	case PWM_ID_3:
		PWM_SET_PIN(3);
		break;

	case PWM_ID_4:
		PWM_SET_PIN(4);
		break;

	case PWM_ID_5:
		PWM_SET_PIN(5);
		break;

	default:
		break;
	}
}

//TODO call it via gpio hal
static void pwm_set_gpio(pwm_chan_t chan)
{
	pwm_init_gpio(chan);
}

static void pwm_chan_enable_interrupt_common(pwm_chan_t chan)
{
	if (pwm_hal_is_pwm2_interrupt(&s_pwm.hal, chan))
		icu_enable_pwm2_interrupt();
	else {
#if (CONFIG_SYSTEM_CTRL)
	sys_drv_int_enable(PWM_INTERRUPT_CTRL_BIT);
#else
	icu_enable_pwm_interrupt();
#endif
	}
	pwm_hal_enable_interrupt(&s_pwm.hal, chan);
}

/* Once the global PWM interrupt is enabled, we never disable it.
 * It should be OK, because if all PWM are stop, there will be no
 * PWM interrupt even though the global PWM interrupt is enabled.
 *
 * We can choose to disable the global PWM interrupt if all PWM
 * channels are stopped, but it make the code complex!
 **/
static void pwm_chan_disable_interrupt_common(pwm_chan_t chan)
{
	pwm_hal_disable_interrupt(&s_pwm.hal, chan);
	pwm_hal_clear_chan_interrupt_status(&s_pwm.hal, chan);
}

/* Init common to PWM/Capture init
 * 1. Set channel init bit
 * 2. Power up channel
 * 3. Set channel specific clock
 */
static void pwm_chan_init_common(pwm_chan_t chan)
{
	s_pwm.chan_init_bits |= BIT(chan);


#if (CONFIG_SYSTEM_CTRL)
	sys_drv_dev_clk_pwr_up(CLK_PWR_ID_PWM_1, CLK_PWR_CTRL_PWR_UP);
	sys_drv_dev_clk_pwr_up(CLK_PWR_ID_PWM_2, CLK_PWR_CTRL_PWR_UP);
#else
	power_pwm_pwr_up(chan);

	/* The hardware support to enable LPO clock, however no customer
	 * currently use LPO clock, here we always disable LPO clock for
	 * simplicity.
	 **/
	clk_disable_pwm_clk_lpo(chan);
#endif

	//TODO call hal
	pwm_set_gpio(chan);
	pwm_hal_set_init_signal_high(&s_pwm.hal, chan);
}

static void pwm_chan_deinit_common(pwm_chan_t chan)
{
	s_pwm.chan_init_bits &= ~BIT(chan);
	pwm_hal_stop_common(&s_pwm.hal, chan);
	pwm_hal_reset_config_to_default(&s_pwm.hal, chan);

#if (CONFIG_SYSTEM_CTRL)
	sys_drv_dev_clk_pwr_up(CLK_PWR_ID_PWM_1, CLK_PWR_CTRL_PWR_DOWN);
	sys_drv_dev_clk_pwr_up(CLK_PWR_ID_PWM_2, CLK_PWR_CTRL_PWR_DOWN);
#else
	power_pwm_pwr_down(chan);
#endif
}

//TODO we should remove pwm_init finally
//Implement pwm_init/pwm_exit to make it compitable with existing driver model
bk_err_t bk_pwm_driver_init(void)
{
	if (s_pwm_driver_is_init)
		return BK_OK;

	os_memset(&s_pwm, 0, sizeof(s_pwm));

	/* Hardware support to configure DCO clock, considering no customers use
	 * DCO clock, we hard-coded to 26M clock for simplicity.
	 *
	 */
#if (CONFIG_SYSTEM_CTRL)
	sys_drv_pwm_select_clock(SYS_SEL_PWM0, PWM_SCLK_XTAL);
	sys_drv_pwm_select_clock(SYS_SEL_PWM1, PWM_SCLK_XTAL);
#else
	clk_set_pwms_clk_26m();
#endif

	//TODO optimize ISR module
	os_memset(&s_pwm_isr, 0, sizeof(s_pwm_isr));
	bk_int_isr_register(INT_SRC_PWM, pwm_isr, NULL);
#if (CONFIG_PWM_HAS_TWO_INT)
	if (icu_is_pwm_has_two_int())
		bk_int_isr_register(INT_SRC_PWM, pwm_isr, NULL);
#endif
	os_memset(&s_pwm, 0, sizeof(s_pwm));
	pwm_hal_init(&s_pwm.hal);

	s_pwm_driver_is_init = true;
	return BK_OK;
}

bk_err_t bk_pwm_driver_deinit(void)
{
	if (!s_pwm_driver_is_init)
		return BK_OK;

	for (int chan = 0; chan < SOC_PWM_CHAN_NUM_PER_UNIT; chan++)
		pwm_chan_deinit_common(chan);

	s_pwm_driver_is_init = false;
	return BK_OK;
}

static void pwm_adjust_init_signal_via_duty(pwm_chan_t chan, uint32_t period,
		uint32_t duty1, uint32_t duty2, uint32_t duty3)
{
	/** If duty ratio is 0% and initial signal is high,
	 * then the PWM signal will always be high.
	 * To avoid that, we manually configure init signal to low if
	 * duty ratio is 0%.
	 */
	if (duty1 == 0)
		pwm_hal_set_init_signal_low(&s_pwm.hal, chan);

	/** If duty ratio is 100% and initial signal is low,
	 * then the PWM signal will always be low.
	 * To avoid that, we manually configure init signal to high if
	 * duty ratio is 100%.
	 */
	if (duty1 == period)
		pwm_hal_set_init_signal_high(&s_pwm.hal, chan);
}

bk_err_t bk_pwm_init(pwm_chan_t chan, const pwm_init_config_t *config)
{
	PWM_RETURN_ON_NOT_INIT();
	BK_RETURN_ON_NULL(config);
	PWM_RETURN_ON_INVALID_CHAN(chan);

	if (!pwm_hal_is_duty_valid(config->period_cycle, config->duty_cycle,
		config->duty2_cycle, config->duty3_cycle))
		return BK_ERR_PWM_PERIOD_DUTY;

	pwm_chan_init_common(chan);
	pwm_adjust_init_signal_via_duty(chan, config->period_cycle, config->duty_cycle,
				config->duty2_cycle, config->duty3_cycle);

	pwm_hal_config_t hal_config = {0};
	hal_config.t4 = config->period_cycle;
	hal_config.t1 = config->duty_cycle;

	if (config->duty2_cycle > 0)
		hal_config.t2 = hal_config.t1 + config->duty2_cycle;

	if (config->duty3_cycle > 0)
		hal_config.t3 = hal_config.t2 + config->duty3_cycle;

	PWM_LOGI("init, chan(%d), t1=%x, t2=%x, t3=%x, t4=%x\n",
			 chan, hal_config.t1, hal_config.t2, hal_config.t3, hal_config.t4);

	return pwm_hal_init_pwm(&s_pwm.hal, chan, &hal_config);
}

bk_err_t bk_pwm_deinit(pwm_chan_t chan)
{
	pwm_chan_deinit_common(chan);
	return BK_OK;
}

bk_err_t bk_pwm_start(pwm_chan_t chan)
{
	PWM_RETURN_ON_CHAN_NOT_INIT(chan);
	pwm_hal_start_common(&s_pwm.hal, chan);
	return BK_OK;
}

bk_err_t bk_pwm_stop(pwm_chan_t chan)
{
	PWM_RETURN_ON_CHAN_NOT_INIT(chan);
	pwm_hal_stop_common(&s_pwm.hal, chan);
	return BK_OK;
}

bk_err_t bk_pwm_register_isr(pwm_chan_t chan, pwm_isr_t isr)
{
	PWM_RETURN_ON_NOT_INIT();
	PWM_RETURN_ON_INVALID_CHAN(chan);

	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();
	s_pwm_isr[chan] = isr;
	GLOBAL_INT_RESTORE();

	return BK_OK;
}

/* The PWM channel should be started before interrupt can be enabled.
 */
bk_err_t bk_pwm_enable_interrupt(pwm_chan_t chan)
{
	PWM_RETURN_ON_CHAN_NOT_INIT(chan);
	PWM_RETURN_ON_INVALID_CHAN(chan);

	if (pwm_hal_is_chan_started(&s_pwm.hal, chan)) {
		pwm_chan_enable_interrupt_common(chan);
		return BK_OK;
	} else
		return BK_ERR_PWM_CHAN_NOT_START;
}

bk_err_t bk_pwm_disable_interrupt(pwm_chan_t chan)
{
	PWM_RETURN_ON_CHAN_NOT_INIT(chan);
	PWM_RETURN_ON_INVALID_CHAN(chan);

	pwm_chan_disable_interrupt_common(chan);
	return BK_OK;
}

bk_err_t bk_pwm_set_period_duty(pwm_chan_t chan, pwm_period_duty_config_t *config)
{
	uint32_t t1 = 0;
	uint32_t t2 = 0;
	uint32_t t3 = 0;
	uint32_t t4 = 0;

	BK_RETURN_ON_NULL(config);
	PWM_RETURN_ON_INVALID_CHAN(chan);
	PWM_RETURN_ON_CHAN_NOT_INIT(chan);

	if (!pwm_hal_is_duty_valid(config->period_cycle, config->duty_cycle,
							   config->duty2_cycle, config->duty3_cycle))
		return BK_ERR_PWM_PERIOD_DUTY;

	pwm_adjust_init_signal_via_duty(chan, config->period_cycle, config->duty_cycle,
									config->duty2_cycle, config->duty3_cycle);

	pwm_hal_set_t1(&s_pwm.hal, chan, config->duty_cycle);

	t1 = config->duty_cycle;
	t2 = config->duty2_cycle;
	if (t2 > 0)
		t2 += t1;

	t3 = config->duty3_cycle;
	if (t3 > 0)
		t3 += t2;

	t4 = config->period_cycle;

	pwm_hal_set_t1(&s_pwm.hal, chan, t1);
	pwm_hal_set_t2(&s_pwm.hal, chan, t2);
	pwm_hal_set_t3(&s_pwm.hal, chan, t3);
	pwm_hal_set_t4(&s_pwm.hal, chan, t4);

	//The new configuration take effect only loading it
	pwm_hal_load_new_config(&s_pwm.hal, chan);
	return BK_OK;
}

static bool pwm_is_0_duty_ratio(pwm_chan_t chan)
{
	return (pwm_hal_get_t1(&s_pwm.hal, chan) == 0);
}

static bool pwm_is_100_duty_ratio(pwm_chan_t chan)
{
	return (pwm_hal_get_t1(&s_pwm.hal, chan) == pwm_hal_get_t4(&s_pwm.hal, chan));
}

bk_err_t bk_pwm_set_init_signal_low(pwm_chan_t chan)
{
	PWM_RETURN_ON_INVALID_CHAN(chan);
	PWM_RETURN_ON_CHAN_NOT_INIT(chan);

	if (pwm_is_100_duty_ratio(chan))
		pwm_hal_set_init_signal_high(&s_pwm.hal, chan);
	else
		pwm_hal_set_init_signal_low(&s_pwm.hal, chan);

	pwm_hal_load_new_config(&s_pwm.hal, chan);
	return BK_OK;
}

bk_err_t bk_pwm_set_init_signal_high(pwm_chan_t chan)
{
	PWM_RETURN_ON_INVALID_CHAN(chan);
	PWM_RETURN_ON_CHAN_NOT_INIT(chan);

	if (pwm_is_0_duty_ratio(chan))
		pwm_hal_set_init_signal_low(&s_pwm.hal, chan);
	else
		pwm_hal_set_init_signal_high(&s_pwm.hal, chan);
	return BK_OK;
}

//TODO finish it
bk_err_t bk_pwm_set_gpio(pwm_chan_t id, uint32 mode)
{
	PWM_RETURN_ON_INVALID_CHAN(id);
	PWM_RETURN_ON_CHAN_NOT_INIT(id);
	PWM_RETURN_ON_INVALID_GPIO_MODE(id);

#if (SOC_PWM_CHAN_NUM_PER_UNIT > 6)
	if( (id == 6) || (id == 7) || (id == 8) ||(id == 9)  )
		return gpio_pwms_sel(id, mode);
#endif
	pwm_set_gpio(id);

	return BK_OK;
}

bk_err_t bk_pwm_capture_init(pwm_chan_t chan, const pwm_capture_init_config_t *config)
{
	PWM_RETURN_ON_NOT_INIT();
	BK_RETURN_ON_NULL(config);
	PWM_RETURN_ON_INVALID_CHAN(chan);

	if (!pwm_hal_is_capture_edge_valid(config->edge))
		return BK_ERR_PWM_CAPTURE_EDGE;

	pwm_chan_init_common(chan);
	s_pwm_isr[chan] = config->isr;

	pwm_hal_capture_config_t hal_config = {
		.edge = config->edge,
	};

	return pwm_hal_init_capture(&s_pwm.hal, chan, &hal_config);
}

bk_err_t bk_pwm_capture_deinit(pwm_chan_t chan)
{
	PWM_RETURN_ON_NOT_INIT();
	PWM_RETURN_ON_INVALID_CHAN(chan);
	pwm_chan_deinit_common(chan);
	return BK_OK;
}

bk_err_t bk_pwm_capture_start(pwm_chan_t chan)
{
	PWM_RETURN_ON_CHAN_NOT_INIT(chan);
	PWM_RETURN_ON_INVALID_CHAN(chan);
	pwm_hal_start_common(&s_pwm.hal, chan);
	pwm_chan_enable_interrupt_common(chan); //interrupt should be enabled after capture start
	return BK_OK;
}

bk_err_t bk_pwm_capture_stop(pwm_chan_t chan)
{
	PWM_RETURN_ON_CHAN_NOT_INIT(chan);
	PWM_RETURN_ON_INVALID_CHAN(chan);
	pwm_hal_stop_common(&s_pwm.hal, chan);
	return BK_OK;
}

//It's up the caller to make sure the channel ID is correct
uint32_t bk_pwm_capture_get_value(pwm_chan_t chan)
{
	return pwm_hal_get_capture_value(&s_pwm.hal, chan);
}

static pwm_group_t pwm_group_find_channel(pwm_chan_t chan)
{
	for (int i = 0; i < PWM_GROUP_NUM; i++) {
		if (s_pwm.groups[i].is_valid && ((s_pwm.groups[i].chan1 == chan) ||
										 (s_pwm.groups[i].chan2 == chan)))
			return i;
	}

	return PWM_GROUP_ID_INVALID;
}

static pwm_group_t pwm_group_find(pwm_chan_t chan1, pwm_chan_t chan2)
{
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

	return PWM_GROUP_ID_INVALID;
}

static bool pwm_group_is_existed(pwm_group_t group)
{
	if (group >= PWM_GROUP_NUM)
		return false;

	return (s_pwm.groups[group].is_valid == true);
}

static pwm_group_t pwm_group_add(pwm_chan_t chan1, pwm_chan_t chan2)
{
	for (int i = 0; i < PWM_GROUP_NUM; i++) {
		if (!s_pwm.groups[i].is_valid) {
			s_pwm.groups[i].is_valid = true;
			s_pwm.groups[i].chan1 = chan1;
			s_pwm.groups[i].chan2 = chan2;
			return i;
		}
	}
	return PWM_GROUP_ID_INVALID;
}

static void pwm_group_delete(pwm_group_t group)
{
	if (group < PWM_GROUP_NUM)
		s_pwm.groups[group].is_valid = false;
}

static bk_err_t pwm_group_validate_param(pwm_chan_t chan1, pwm_chan_t chan2,
		uint32_t period_cycle, uint32_t chan1_duty_cycle, uint32_t chan2_duty_cycle)

{
	PWM_RETURN_ON_INVALID_CHAN(chan1);
	PWM_RETURN_ON_INVALID_CHAN(chan2);

	if (chan1 == chan2)
		return BK_ERR_PWM_GROUP_SAME_CHAN;

	if (PWM_GROUP_ID_INVALID != pwm_group_find(chan1, chan2))
		return BK_ERR_PWM_GROUP_EXIST;

	if (PWM_GROUP_ID_INVALID != pwm_group_find_channel(chan1))
		return BK_ERR_PWM_GROUP_CHAN_USED;

	if (PWM_GROUP_ID_INVALID != pwm_group_find_channel(chan2))
		return BK_ERR_PWM_GROUP_CHAN_USED;

	if (period_cycle < (chan1_duty_cycle + chan2_duty_cycle))
		return BK_ERR_PWM_GROUP_DUTY;

	//duty is 0 is meanless
	if ((chan1_duty_cycle == 0) || (chan2_duty_cycle == 0))
		return BK_ERR_PWM_GROUP_DUTY;

	return BK_OK;
}

bk_err_t bk_pwm_group_init(const pwm_group_init_config_t *config, pwm_group_t *group)
{
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
	pwm_hal_set_init_signal_high(&s_pwm.hal, config->chan1);

	dead_cycle = (config->period_cycle - config->chan1_duty_cycle - config->chan2_duty_cycle) >> 1;
	pwm_config.duty_cycle = config->chan1_duty_cycle + dead_cycle;
	pwm_config.duty2_cycle = config->chan2_duty_cycle;
	ret = bk_pwm_init(config->chan2, &pwm_config);
	if (ret != BK_OK)
		return ret;
	pwm_hal_set_init_signal_low(&s_pwm.hal, config->chan2);

	//TODO peter adjust the PWM initial signal according to DUTY value

	*group = pwm_group_add(config->chan1, config->chan2);

	PWM_LOGI("group period=%x chan1_duty=%x chan2_duty=%x\n",
			 config->period_cycle, config->chan1_duty_cycle, config->chan2_duty_cycle);
	PWM_LOGI("group T21=%x T22=%x\n", pwm_config.duty_cycle, pwm_config.duty2_cycle);

	return BK_OK;
}

bk_err_t bk_pwm_group_deinit(pwm_group_t group)
{
	if (pwm_group_is_existed(group))
		return BK_OK;

	bk_pwm_group_stop(group);
	pwm_chan_deinit_common(s_pwm.groups[group].chan1);
	pwm_chan_deinit_common(s_pwm.groups[group].chan2);
	pwm_group_delete(group);
	return BK_OK;
}

bk_err_t bk_pwm_group_start(pwm_group_t group)
{
	pwm_chan_t chan1;
	pwm_chan_t chan2;

	if (!pwm_group_is_existed(group))
		return BK_ERR_PWM_GROUP_NOT_EXIST;

	chan1 = s_pwm.groups[group].chan1;
	chan2 = s_pwm.groups[group].chan2;
	if (pwm_hal_is_hardware_group(&s_pwm.hal, chan1, chan2))
		pwm_hal_start_hardware_group(&s_pwm.hal, chan1, chan2);

	pwm_hal_start_common(&s_pwm.hal, chan1);
	pwm_hal_start_common(&s_pwm.hal, chan2);
	return BK_OK;
}

bk_err_t bk_pwm_group_stop(pwm_group_t group)
{
	pwm_chan_t chan1;
	pwm_chan_t chan2;

	if (!pwm_group_is_existed(group))
		return BK_OK;

	chan1 = s_pwm.groups[group].chan1;
	chan2 = s_pwm.groups[group].chan2;
	if (pwm_hal_is_hardware_group(&s_pwm.hal, chan1, chan2))
		pwm_hal_stop_hardware_group(&s_pwm.hal, chan1, chan2);

	pwm_hal_stop_common(&s_pwm.hal, chan1);
	pwm_hal_stop_common(&s_pwm.hal, chan2);
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

bk_err_t bk_pwm_set_mode_timer(pwm_chan_t chan)
{
	PWM_RETURN_ON_INVALID_CHAN(chan);

	pwm_hal_set_mode_timer(&s_pwm.hal, chan);
	return BK_OK;
}

static void pwm_isr(void)
{
	pwm_hal_t *hal = &s_pwm.hal;
	uint32_t int_status;
	int chan;

	int_status = pwm_hal_get_interrupt_status(hal);
	pwm_hal_clear_interrupt_status(hal, int_status);
	for (chan = 0; chan < SOC_PWM_CHAN_NUM_PER_UNIT; chan++) {
		if (pwm_hal_is_interrupt_triggered(hal, chan, int_status)) {
			if (s_pwm_isr[chan])
				s_pwm_isr[chan](chan);
		}
	}
}
