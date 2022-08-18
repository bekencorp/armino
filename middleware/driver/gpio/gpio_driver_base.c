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
#include "icu_driver.h"
#include <os/mem.h>
#include "gpio_hal.h"
#include "gpio_driver_base.h"
#include "gpio_driver.h"
#include <driver/gpio_types.h>
#include <driver/int.h>
#include "amp_lock_api.h"
#if CONFIG_AON_PMU
#include "aon_pmu_driver.h"
#endif
#if (CONFIG_SYSTEM_CTRL)
#include "sys_driver.h"
#endif
#if CONFIG_GPIO_SIMULATE_UART_WRITE
#include "bk_misc.h"
#endif
gpio_driver_t s_gpio = {0};
static gpio_isr_t s_gpio_isr[SOC_GPIO_NUM] = {NULL};
static bool s_gpio_is_init = false;

#define GPIO_RETURN_ON_INVALID_ID(id) do {\
		if ((id) >= SOC_GPIO_NUM) {\
			return BK_ERR_GPIO_CHAN_ID;\
		}\
	} while(0)

#define GPIO_RETURN_ON_INVALID_INT_TYPE_MODE(mode) do {\
				if ((mode) >= GPIO_INT_TYPE_MAX) {\
					return BK_ERR_GPIO_INVALID_INT_TYPE;\
				}\
			} while(0)

#define GPIO_RETURN_ON_INVALID_IO_MODE(mode) do {\
				if (((mode)) >= GPIO_IO_INVALID) {\
					return BK_ERR_GPIO_INVALID_MODE;\
				}\
			} while(0)

#define GPIO_RETURN_ON_INVALID_PULL_MODE(mode) do {\
				if (((mode)) >= GPIO_PULL_INVALID) {\
					return BK_ERR_GPIO_INVALID_MODE;\
				}\
			} while(0)

#if CONFIG_GPIO_WAKEUP_SUPPORT
static uint16_t s_gpio_bak_regs[GPIO_NUM_MAX];
uint32_t s_gpio_bak_int_type_regs[3];
uint32_t s_gpio_bak_int_enable_regs[2];
uint64_t s_gpio_is_setted_wake_status;
static gpio_id_t s_gpio_wakeup_gpio_id = GPIO_NUM;

static void gpio_wakeup_default_isr(gpio_id_t gpio_id);

#if CONFIG_GPIO_DYNAMIC_WAKEUP_SUPPORT
#ifndef CONFIG_GPIO_DYNAMIC_WAKEUP_SOURCE_MAX_CNT
#define CONFIG_GPIO_DYNAMIC_WAKEUP_SOURCE_MAX_CNT (4)
#endif
#define GPIO_WAKE_SOURCE_IDLE_ID (GPIO_NUM)	//
typedef struct
{
	gpio_id_t id;
	gpio_int_type_t int_type;
	//gpio_isr_t isr;
}gpio_dynamic_wakeup_t;
static gpio_dynamic_wakeup_t s_gpio_dynamic_wakeup_source_map[CONFIG_GPIO_DYNAMIC_WAKEUP_SOURCE_MAX_CNT];
static void	gpio_dynamic_wakeup_source_init(void);
static void bk_gpio_set_wakeup_gpio_id(gpio_id_t gpio_id);
static bool bk_gpio_get_aon_pmu_deepsleep_flag();
#endif
#endif

static void gpio_isr(void);

bk_err_t bk_gpio_driver_init(void)
{
	//avoid re-init caused some info lost
	if(s_gpio_is_init)
	{
		GPIO_LOGI("%s:has inited \r\n", __func__);
		return BK_OK;
	}

	os_memset(&s_gpio, 0, sizeof(s_gpio));

	gpio_hal_disable_jtag_mode(&s_gpio.hal);

	gpio_hal_init(&s_gpio.hal);

#ifdef CONFIG_GPIO_DYNAMIC_WAKEUP_SUPPORT
	gpio_dynamic_wakeup_source_init();
#endif

	amp_res_init(AMP_RES_ID_GPIO);

	//Move ISR to last to avoid other resouce doesn't finish but isr has came.
	//F.E:GPIO wakeup deepsleep.
	bk_int_isr_register(INT_SRC_GPIO, gpio_isr, NULL);
	//interrupt to CPU enable
#if (CONFIG_SYSTEM_CTRL)
	sys_drv_int_group2_enable(GPIO_INTERRUPT_CTRL_BIT);
#else
	icu_enable_gpio_interrupt();
#endif

	s_gpio_is_init = true;
	return BK_OK;
}

bk_err_t bk_gpio_driver_deinit(void)
{
	if(!s_gpio_is_init)
	{
		GPIO_LOGI("%s:isn't init \r\n", __func__);
		return BK_OK;
	}

	//interrupt to CPU disable
#if (CONFIG_SYSTEM_CTRL)
	sys_drv_int_group2_disable(GPIO_INTERRUPT_CTRL_BIT);
#else
	icu_disable_gpio_interrupt();
#endif

	s_gpio_is_init = false;

	return BK_OK;
}


bk_err_t bk_gpio_enable_output(gpio_id_t gpio_id)
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);

	return gpio_hal_output_enable(&s_gpio.hal, gpio_id, 1);
}

bk_err_t bk_gpio_disable_output(gpio_id_t gpio_id)
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);

	return gpio_hal_output_enable(&s_gpio.hal, gpio_id, 0);
}

bk_err_t bk_gpio_enable_input(gpio_id_t gpio_id)
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);

	return gpio_hal_input_enable(&s_gpio.hal, gpio_id, 1);
}

bk_err_t bk_gpio_disable_input(gpio_id_t gpio_id)
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);

	return gpio_hal_input_enable(&s_gpio.hal, gpio_id, 0);
}

bk_err_t bk_gpio_enable_pull(gpio_id_t gpio_id )
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);

	return gpio_hal_pull_enable(&s_gpio.hal, gpio_id, 1);
}

bk_err_t bk_gpio_disable_pull(gpio_id_t gpio_id )
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);

	return gpio_hal_pull_enable(&s_gpio.hal, gpio_id, 0);
}

bk_err_t bk_gpio_pull_up(gpio_id_t gpio_id)
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);

	gpio_hal_pull_enable(&s_gpio.hal, gpio_id, 1);
	return gpio_hal_pull_up_enable(&s_gpio.hal, gpio_id, 1);
}

bk_err_t bk_gpio_pull_down(gpio_id_t gpio_id)
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);

	gpio_hal_pull_enable(&s_gpio.hal, gpio_id, 1);
	return gpio_hal_pull_up_enable(&s_gpio.hal, gpio_id, 0);
}



bk_err_t bk_gpio_set_output_high(gpio_id_t gpio_id)
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);

	return gpio_hal_set_output_value(&s_gpio.hal, gpio_id, 1);
}

bk_err_t bk_gpio_set_output_low(gpio_id_t gpio_id)
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);

	return gpio_hal_set_output_value(&s_gpio.hal, gpio_id, 0);
}

bool bk_gpio_get_input(gpio_id_t gpio_id)
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);

	return gpio_hal_get_input(&s_gpio.hal, gpio_id);
}

//MAX capactiy:3
bool bk_gpio_set_capacity(gpio_id_t gpio_id, uint32 capacity)
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);

	return gpio_hal_set_capacity(&s_gpio.hal, gpio_id, capacity);
}

bk_err_t bk_gpio_set_config(gpio_id_t gpio_id, const gpio_config_t *config)
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);
	GPIO_RETURN_ON_INVALID_IO_MODE(config->io_mode);
	GPIO_RETURN_ON_INVALID_PULL_MODE(config->pull_mode);

	return gpio_hal_set_config(&s_gpio.hal, gpio_id, config);
}


/* Enable GPIO  interrupt.
 */
bk_err_t bk_gpio_register_isr(gpio_id_t gpio_id, gpio_isr_t isr)
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);
	s_gpio_isr[gpio_id] = isr;

	return BK_OK;
}

//This function just enable the select GPIO can report IRQ to CPU
bk_err_t bk_gpio_enable_interrupt(gpio_id_t gpio_id)
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);

	return gpio_hal_enable_interrupt(&s_gpio.hal, gpio_id);
}

bk_err_t bk_gpio_disable_interrupt(gpio_id_t gpio_id)
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);

	//WARNING:We can't call icu_enable_gpio_interrupt/sys_drv_int_group2_disable in this function
	//If more then one GPIO_ID enable interrupt, here disable the IRQ to CPU, it caused other GPIO ID can't work

	gpio_hal_disable_interrupt(&s_gpio.hal, gpio_id);

	return BK_OK;
}

bk_err_t bk_gpio_clear_interrupt(gpio_id_t gpio_id)
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);

	//WARNING:We can't call icu_enable_gpio_interrupt/sys_drv_int_group2_disable in this function
	//If more then one GPIO_ID enable interrupt, here disable the IRQ to CPU, it caused other GPIO ID can't work

	gpio_hal_clear_chan_interrupt_status(&s_gpio.hal, gpio_id);

	return BK_OK;
}
bk_err_t bk_gpio_set_interrupt_type(gpio_id_t gpio_id, gpio_int_type_t type)
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);
	GPIO_RETURN_ON_INVALID_INT_TYPE_MODE(type);

	return gpio_hal_set_int_type(&s_gpio.hal, gpio_id, type);
}


static void gpio_isr(void)
{
	gpio_hal_t *hal = &s_gpio.hal;
	gpio_interrupt_status_t gpio_status;
	int gpio_id;

	gpio_hal_get_interrupt_status(hal, &gpio_status);

	for (gpio_id = 0; gpio_id < GPIO_NUM; gpio_id++) {
		if (gpio_hal_is_interrupt_triggered(hal, gpio_id, &gpio_status)) {
#if CONFIG_GPIO_WAKEUP_SUPPORT
			/* Get the ID of the first interrupt wake up,
			 * which is not necessarily the registered ID.
			 */
			if(bk_gpio_get_aon_pmu_deepsleep_flag())
				bk_gpio_set_wakeup_gpio_id(gpio_id);

			/* if many gpio wakeup at the same time, it will called many times */
			if(s_gpio_is_setted_wake_status & ((uint64_t)0x1 << gpio_id))
			{
				gpio_wakeup_default_isr(gpio_id);
			}
#endif
			if (s_gpio_isr[gpio_id]) {
				GPIO_LOGD("gpio int: index:%d \r\n",gpio_id);
				s_gpio_isr[gpio_id](gpio_id);
			}
		}
	}

	//move it after callback:if isr is caused by level type,
	//clear IRQ status and doesn't disable IRQ, then it causes a new ISR
	gpio_hal_clear_interrupt_status(hal, &gpio_status);

}

#if CONFIG_GPIO_WAKEUP_SUPPORT

static void gpio_dump_baked_regs(bool configs, bool int_type_status, bool int_en_status)
{
#if CONFIG_GPIO_WAKEUP_DEBUG
	gpio_id_t gpio_id = 0;

	GPIO_LOGD("%s[+]\r\n", __func__);

	GPIO_LOGD("is_setted_wake_status h= 0x%x, l=0x%x\r\n", (uint32_t)(s_gpio_is_setted_wake_status>>32), (uint32_t)s_gpio_is_setted_wake_status);

	if(configs)
	{
		for(gpio_id = 0; gpio_id < GPIO_NUM; gpio_id++)
		{
			GPIO_LOGD("s_gpio_bak_regs[%d]=0x%x\r\n", gpio_id, s_gpio_bak_regs[gpio_id]);
		}
	}

	if(int_type_status)
	{
		for(gpio_id = 0; gpio_id < sizeof(s_gpio_bak_int_type_regs)/sizeof(s_gpio_bak_int_type_regs[0]); gpio_id++)
		{
			GPIO_LOGD("int_type_regs[%d]=0x%x\r\n", gpio_id, s_gpio_bak_int_type_regs[gpio_id]);
		}
	}

	if(int_en_status)
	{
		for(gpio_id = 0; gpio_id < sizeof(s_gpio_bak_int_enable_regs)/sizeof(s_gpio_bak_int_enable_regs[0]); gpio_id++)
		{
			GPIO_LOGD("int_enable_regs[%d]=0x%x\r\n", gpio_id, s_gpio_bak_int_enable_regs[gpio_id]);
		}
	}

	GPIO_LOGD("%s[-]\r\n", __func__);
#endif
}

static void gpio_dump_regs(bool configs, bool int_status)
{
#if CONFIG_GPIO_WAKEUP_DEBUG
	gpio_id_t gpio_id = 0;

	GPIO_LOGD("%s[+]\r\n", __func__);

	if(configs)
	{
		for(gpio_id = 0; gpio_id < GPIO_NUM; gpio_id++)
		{
			///gpio_struct_dump(gpio_id);
			GPIO_LOGD("gpio[%d]=0x%x\r\n", gpio_id, *(volatile uint32_t*)(GPIO_LL_REG_BASE + 4*gpio_id));
		}
	}

	//WARNING:BK7256 has this 9 regs, maybe other project doesn't has this 9 REGs
	if(int_status)
	{
		for(gpio_id = 0; gpio_id < 9; gpio_id++)
		{
			///gpio_struct_dump(gpio_id);
			GPIO_LOGD("REG0x%x=0x%x\r\n", (GPIO_LL_REG_BASE + 4*(0x40+gpio_id)), *(volatile uint32_t*)(GPIO_LL_REG_BASE + 4*(0x40+gpio_id)));
		}
	}
	GPIO_LOGD("%s[-]\r\n", __func__);
#endif
}

void gpio_get_interrupt_status(uint32_t *h_status, uint32_t *l_status)
{
	gpio_hal_t *hal = &s_gpio.hal;
	gpio_interrupt_status_t gpio_status;

	gpio_hal_get_interrupt_status(hal, &gpio_status);
	if(h_status)
		*h_status = gpio_status.gpio_32_64_int_status;
	if(l_status)
		*l_status = gpio_status.gpio_0_31_int_status;
}

static void gpio_wakeup_default_isr(gpio_id_t gpio_id)
{
	GPIO_LOGD("gpio int: index:%d \r\n", gpio_id);
}

static void gpio_config_wakeup_function(void)
{
	uint32_t i = 0;
	gpio_wakeup_t gpio_wakeup_map[] = GPIO_STATIC_WAKEUP_SOURCE_MAP;

	GPIO_LOGD("%s[+]\r\n", __func__);

	s_gpio_is_setted_wake_status = 0;
	for(i = 0; i < sizeof(gpio_wakeup_map)/sizeof(gpio_wakeup_t); i++)
	{
		bk_gpio_set_interrupt_type(gpio_wakeup_map[i].id, gpio_wakeup_map[i].int_type);
		bk_gpio_enable_interrupt(gpio_wakeup_map[i].id);
		s_gpio_is_setted_wake_status |= ((uint64_t)1 << gpio_wakeup_map[i].id);
	}

#ifdef CONFIG_GPIO_DYNAMIC_WAKEUP_SUPPORT
	for(i = 0; i < CONFIG_GPIO_DYNAMIC_WAKEUP_SOURCE_MAX_CNT; i++)
	{
		if(s_gpio_dynamic_wakeup_source_map[i].id != GPIO_WAKE_SOURCE_IDLE_ID)
		{
			bk_gpio_set_interrupt_type(s_gpio_dynamic_wakeup_source_map[i].id, s_gpio_dynamic_wakeup_source_map[i].int_type);
			bk_gpio_enable_interrupt(s_gpio_dynamic_wakeup_source_map[i].id);
			s_gpio_is_setted_wake_status |= ((uint64_t)1 << s_gpio_dynamic_wakeup_source_map[i].id);
		}
	}
#endif

	GPIO_LOGD("%s[-]set wake src h=0x%0x, l=0x%0x\r\n", __func__, (uint32_t)(s_gpio_is_setted_wake_status>>32), (uint32_t)s_gpio_is_setted_wake_status) ;
}

static void gpio_config_low_power_wakeup_pin(void)
{
	uint32_t i = 0;

	gpio_wakeup_t gpio_wakeup_map[] = GPIO_STATIC_WAKEUP_SOURCE_MAP;
	for(i = 0; i < sizeof(gpio_wakeup_map)/sizeof(gpio_wakeup_t); i++)
	{
		bk_gpio_disable_output(gpio_wakeup_map[i].id);
		bk_gpio_enable_input(gpio_wakeup_map[i].id);
	}

#ifdef CONFIG_GPIO_DYNAMIC_WAKEUP_SUPPORT
	for(i = 0; i < CONFIG_GPIO_DYNAMIC_WAKEUP_SOURCE_MAX_CNT; i++)
	{
		if(s_gpio_dynamic_wakeup_source_map[i].id != GPIO_WAKE_SOURCE_IDLE_ID)
		{
			bk_gpio_disable_output(s_gpio_dynamic_wakeup_source_map[i].id);
			bk_gpio_enable_input(s_gpio_dynamic_wakeup_source_map[i].id);
		}
	}
#endif
}

static void gpio_clear_wakeup_function(void)
{
	s_gpio_is_setted_wake_status = 0;
}

#ifdef CONFIG_GPIO_DYNAMIC_WAKEUP_SUPPORT

gpio_id_t bk_gpio_get_wakeup_gpio_id()
{
	GPIO_LOGD("GET wakeup gpio_id: %d \r\n", s_gpio_wakeup_gpio_id);
	return s_gpio_wakeup_gpio_id;
}

static void bk_gpio_set_wakeup_gpio_id(gpio_id_t gpio_id)
{
	if (s_gpio_wakeup_gpio_id == GPIO_NUM)
		s_gpio_wakeup_gpio_id = gpio_id;
	GPIO_LOGD("SET wakeup gpio_id: %d \r\n", s_gpio_wakeup_gpio_id);
}

static bool bk_gpio_get_aon_pmu_deepsleep_flag()
{
#if CONFIG_AON_PMU
	return (aon_pmu_drv_reg_get(PMU_REG2) & BIT(BIT_SLEEP_FLAG_DEEP_SLEEP));
#endif
	return false;
}

static void gpio_dynamic_wakeup_source_init(void)
{
	uint32_t i = 0;

	GPIO_LOGD("%s[+]gpio wakecnt=%d\r\n", __func__, CONFIG_GPIO_DYNAMIC_WAKEUP_SOURCE_MAX_CNT);
	//search the same id and replace it.
	for(i = 0; i < CONFIG_GPIO_DYNAMIC_WAKEUP_SOURCE_MAX_CNT; i++)
	{
		s_gpio_dynamic_wakeup_source_map[i].id = GPIO_WAKE_SOURCE_IDLE_ID;
	}

	GPIO_LOGD("%s[-]\r\n", __func__);
}

bk_err_t bk_gpio_register_wakeup_source(gpio_id_t gpio_id,
                                                 gpio_int_type_t int_type)
{
	uint32_t i = 0;

	GPIO_RETURN_ON_INVALID_ID(gpio_id);
	GPIO_RETURN_ON_INVALID_INT_TYPE_MODE(int_type);

	//search the same id and replace it.
	for(i = 0; i < CONFIG_GPIO_DYNAMIC_WAKEUP_SOURCE_MAX_CNT; i++)
	{
		if(s_gpio_dynamic_wakeup_source_map[i].id == gpio_id)
		{
			s_gpio_dynamic_wakeup_source_map[i].int_type = int_type;
			//s_gpio_dynamic_wakeup_source_map[i].isr = isr;

			GPIO_LOGD("gpio=%d,int_type=%d replace previous wake src\r\n", gpio_id, int_type);
			return BK_OK;
		}
	}

	//serach the first idle id
	for(i = 0; i < CONFIG_GPIO_DYNAMIC_WAKEUP_SOURCE_MAX_CNT; i++)
	{
		if(s_gpio_dynamic_wakeup_source_map[i].id == GPIO_WAKE_SOURCE_IDLE_ID)
		{
			s_gpio_dynamic_wakeup_source_map[i].id = gpio_id;
			s_gpio_dynamic_wakeup_source_map[i].int_type = int_type;
			//s_gpio_dynamic_wakeup_source_map[i].isr = isr;
			s_gpio_is_setted_wake_status |= ((uint64_t)1 << s_gpio_dynamic_wakeup_source_map[i].id);

			GPIO_LOGD("gpio=%d,int_type=%d register wake src\r\n", gpio_id, int_type);

			return BK_OK;
		}
	}

	GPIO_LOGE("too much(%d) GPIO is setted wake src\r\n", CONFIG_GPIO_DYNAMIC_WAKEUP_SOURCE_MAX_CNT);
	for(i = 0; i < CONFIG_GPIO_DYNAMIC_WAKEUP_SOURCE_MAX_CNT; i++)
	{
		GPIO_LOGE("gpio id:%d is using \r\n", s_gpio_dynamic_wakeup_source_map[i].id);	
	}
	return BK_FAIL;
}

bk_err_t bk_gpio_unregister_wakeup_source(gpio_id_t gpio_id)
{
	uint32_t i = 0;

	GPIO_RETURN_ON_INVALID_ID(gpio_id);

	/* search the same id and replace it.*/
	for(i = 0; i < CONFIG_GPIO_DYNAMIC_WAKEUP_SOURCE_MAX_CNT; i++)
	{
		if(s_gpio_dynamic_wakeup_source_map[i].id == gpio_id)
		{
			s_gpio_dynamic_wakeup_source_map[i].id = GPIO_WAKE_SOURCE_IDLE_ID;
			s_gpio_dynamic_wakeup_source_map[i].int_type = GPIO_INT_TYPE_MAX;
			//s_gpio_dynamic_wakeup_source_map[i].isr = NULL;
			s_gpio_is_setted_wake_status &= ~(((uint64_t)1 << s_gpio_dynamic_wakeup_source_map[i].id));

			/* Clear the hardware status during deregister */
			bk_gpio_disable_input(gpio_id);
			bk_gpio_disable_interrupt(gpio_id);

			GPIO_LOGD("%s[-]gpioid=%d\r\n", __func__, gpio_id);

			return BK_OK;
		}
	}

	GPIO_LOGE("gpio id:%d is not using \r\n", gpio_id);
	return BK_FAIL;
}

#endif

bk_err_t gpio_enter_low_power(void *param)
{
	GPIO_LOGD("%s[+]\r\n", __func__);

	gpio_dump_regs(true, true);

	gpio_hal_bak_configs(&s_gpio_bak_regs[0], GPIO_NUM_MAX);
	gpio_dump_baked_regs(true, false, false);
	gpio_hal_bak_int_type_configs(&s_gpio_bak_int_type_regs[0], sizeof(s_gpio_bak_int_type_regs)/sizeof(s_gpio_bak_int_type_regs[0]));
	gpio_hal_bak_int_enable_configs(&s_gpio_bak_int_enable_regs[0], sizeof(s_gpio_bak_int_enable_regs)/sizeof(s_gpio_bak_int_enable_regs[0]));

	gpio_dump_baked_regs(false, true, true);

#if 1	//TODO:
	if((uint32_t)param != 0x534b4950)	//just debug:magic value == "SKIP"
	{
		GPIO_LOGD("switch to low power tatus\r\n");
		gpio_hal_switch_to_low_power_status();
		gpio_config_low_power_wakeup_pin();
		GPIO_LOGD("exit switch to low power tatus\r\n");
		gpio_dump_regs(true, false);
	}
#endif

	/*
	 * !!! NOTES NOTES !!!
	 * ASIC switch PIN function to GPIO Input mode,
	 * it takes 3 cycles(GPIO uses 32K clock) to be stable.
	 * If in the 3 cycles enable INPUT level/edge check, it will report an error status.
	 * so enable gpio input irq, should wait enough time.
	 */
	delay_us(125);	//125 == ((3+1)/32) * 1000 us

	gpio_config_wakeup_function();
	gpio_dump_regs(false, true);

	GPIO_LOGD("%s[-]\r\n", __func__);
	
	return BK_OK;
}

bk_err_t gpio_exit_low_power(void *param)
{
	gpio_hal_t *hal = &s_gpio.hal;
	gpio_interrupt_status_t gpio_status;

	GPIO_LOGD("%s[+]\r\n", __func__);

	gpio_dump_regs(true, true);
	gpio_dump_baked_regs(true, true, true);

	gpio_hal_get_interrupt_status(hal, &gpio_status);
	
	GPIO_LOGD("wakeup isn't by gpio\r\n");
	GPIO_LOGD("%s[+]\r\n", __func__);
	
	//wakeup not by gpio, should clear wakeup function,then GPIO wake ISR will not called.
	if((gpio_status.gpio_0_31_int_status == 0) && (gpio_status.gpio_32_64_int_status == 0))	
	{
		GPIO_LOGD("wakeup isn't by gpio\r\n");
		gpio_clear_wakeup_function();
	}

#if 1	//TODO:
	if((uint32_t)param != 0x534b4950)	//just debug:magic value == "SKIP"
	{
		gpio_hal_restore_configs(&s_gpio_bak_regs[0], GPIO_NUM_MAX);
		gpio_hal_restore_int_type_configs(&s_gpio_bak_int_type_regs[0], sizeof(s_gpio_bak_int_type_regs)/sizeof(s_gpio_bak_int_type_regs[0]));
		gpio_hal_restore_int_enable_configs(&s_gpio_bak_int_enable_regs[0], sizeof(s_gpio_bak_int_enable_regs)/sizeof(s_gpio_bak_int_enable_regs[0]));
	}
#endif

	gpio_dump_regs(true, true);
	gpio_dump_baked_regs(true, true, true);

	GPIO_LOGD("%s[-]\r\n", __func__);

	return BK_OK;
}

#else
bk_err_t bk_gpio_reg_save(uint32_t*  gpio_cfg)
{
    return gpio_hal_reg_save(gpio_cfg);
}

bk_err_t bk_gpio_reg_restore(uint32_t*  gpio_cfg)
{
    return gpio_hal_reg_restore(gpio_cfg);
}

bk_err_t bk_gpio_wakeup_enable(int64_t index, uint64_t type_l, uint64_t type_h)
{
    return gpio_hal_wakeup_enable( index, type_l, type_h);
}

bk_err_t bk_gpio_wakeup_interrupt_clear()
{
    return gpio_hal_wakeup_interrupt_clear();
}
#endif

#if CONFIG_GPIO_SIMULATE_UART_WRITE
/**
 * @brief	  Uses specifies GPIO to simulate UART write data
 *
 * This API Uses specifies GPIO to simulate UART write data:
 *	 - Uses CPU poll wait to do delay, so it blocks CPU.
 *	 - The caller should confirm the specifies GPIO is not used by other APP.
 *
 * @param *buff  Which buffers will be write with GPIO.
 * @param len    How many bytes data will be wrote.
 * @param gpio_id  Which GPIO will be simulated as UART write data.
 * @param div    Baud rate == 1Mbps/(1+div)
 *
 * @attention 1. As this function just simulate uart write, it blocks the CPU,
 *               so please don't write too much data.
 *
 * @return
 */
void gpio_simulate_uart_write(unsigned char *buff, uint32_t len, gpio_id_t gpio_id, uint32_t div)
{
	volatile unsigned char c, n;
	UINT32 param;
	uint32_t div_cnt = div+1;

	BK_LOG_ON_ERR(bk_gpio_disable_input(gpio_id));
	BK_LOG_ON_ERR(bk_gpio_enable_output(gpio_id));

	bk_gpio_set_output_high(gpio_id);
	delay_us(div_cnt);

	while (len--) {
		//in while loop, to avoid disable IRQ too much time, release it if finish one byte.
		GLOBAL_INT_DECLARATION();
		GLOBAL_INT_DISABLE();

		//UART start bit
		bk_gpio_set_output_low(gpio_id);
		delay_us(div_cnt);

		//char value
		c = *buff++;
		n = 8;
		while (n--) {
			param = c & 0x01;
			if (param) {
				bk_gpio_set_output_high(gpio_id);
			} else {
				bk_gpio_set_output_low(gpio_id);
			}

			delay_us(div_cnt);
			c >>= 1;
		}

		//UART stop bit
		bk_gpio_set_output_high(gpio_id);
		delay_us(div_cnt);

		GLOBAL_INT_RESTORE();
	}
}
#endif

