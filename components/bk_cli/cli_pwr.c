#include "cli.h"
#include "wlan_ui_pub.h"
#include "bk_manual_ps.h"
#include "bk_mac_ps.h"
#include "bk_mcu_ps.h"
#include "bk_ps.h"
#include "bk_wifi.h"
#include "modules/pm.h"
#include "sys_driver.h"
#include "bk_pm_internal_api.h"
#include <driver/mailbox_channel.h>
#include <driver/gpio.h>
#include <driver/touch.h>
#include <driver/touch_types.h>
#include <driver/hal/hal_aon_rtc_types.h>
#include <driver/aon_rtc_types.h>
#include <driver/aon_rtc.h>
#include <modules/ble_types.h>
#include <driver/timer.h>


#if CONFIG_MCU_PS
static void cli_ps_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
#if PS_SUPPORT_MANUAL_SLEEP
	UINT32 standby_time = 0;
	UINT32 dtim_wait_time = 0;
#endif

	if (argc != 3)
		goto _invalid_ps_arg;

#if CONFIG_MCU_PS
	if (0 == os_strcmp(argv[1], "mcudtim")) {
		UINT32 dtim = os_strtoul(argv[2], NULL, 10);
		if (dtim == 1)
			bk_wlan_mcu_ps_mode_enable();
		else if (dtim == 0)
			bk_wlan_mcu_ps_mode_disable();
		else
			goto _invalid_ps_arg;
	}
#endif
#if CONFIG_STA_PS
	else if (0 == os_strcmp(argv[1], "rfdtim")) {
		UINT32 dtim = os_strtoul(argv[2], NULL, 10);
		if (dtim == 1) {
			if (bk_wlan_ps_enable())
				os_printf("dtim enable failed\r\n");
		} else if (dtim == 0) {
			if (bk_wlan_ps_disable())
				os_printf("dtim disable failed\r\n");
		} else
			goto _invalid_ps_arg;
	}
#if PS_USE_KEEP_TIMER
	else if (0 == os_strcmp(argv[1], "rf_timer")) {
		UINT32 dtim = os_strtoul(argv[2], NULL, 10);

		if (dtim == 1) {
			extern int bk_wlan_ps_timer_start(void);
			bk_wlan_ps_timer_start();
		} else  if (dtim == 0) {
			extern int bk_wlan_ps_timer_pause(void);
			bk_wlan_ps_timer_pause();
		} else
			goto _invalid_ps_arg;
	}
#endif
#endif
	else
		goto _invalid_ps_arg;


	return;

_invalid_ps_arg:
	os_printf("Usage:ps {rfdtim|mcudtim|rf_timer} {1/0}\r\n");
}
#endif
#if CONFIG_SYSTEM_CTRL
#define PM_MANUAL_LOW_VOL_VOTE_ENABLE    (0)
#define PM_DEEP_SLEEP_REGISTER_CALLBACK_ENABLE (0x1)

static UINT32 s_cli_sleep_mode = 0;
static UINT32 s_pm_vote1       = 0;
static UINT32 s_pm_vote2       = 0;
static UINT32 s_pm_vote3       = 0;

extern void stop_slave_core(void);
#if CONFIG_AON_RTC
static void cli_pm_rtc_callback(aon_rtc_id_t id, uint8_t *name_p, void *param)
{
	if(s_cli_sleep_mode == PM_MODE_DEEP_SLEEP)//when wakeup from deep sleep, all thing initial
	{
		bk_pm_sleep_mode_set(PM_MODE_DEFAULT);
	}
	else if(s_cli_sleep_mode == PM_MODE_LOW_VOLTAGE)
	{
		bk_pm_sleep_mode_set(PM_MODE_DEFAULT);
		bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_APP,0x0,0x0);
	}
	else
	{
		bk_pm_sleep_mode_set(PM_MODE_DEFAULT);
		bk_pm_module_vote_sleep_ctrl(s_pm_vote1,0x0,0x0);
		bk_pm_module_vote_sleep_ctrl(s_pm_vote2,0x0,0x0);
		bk_pm_module_vote_sleep_ctrl(s_pm_vote3,0x0,0x0);
	}
	os_printf("cli_pm_rtc_callback\r\n");
}
#endif
#if CONFIG_TOUCH
void cli_pm_touch_callback(void *param)
{
	if(s_cli_sleep_mode == PM_MODE_DEEP_SLEEP)//when wakeup from deep sleep, all thing initial
	{
		bk_pm_sleep_mode_set(PM_MODE_DEFAULT);
	}
	else if(s_cli_sleep_mode == PM_MODE_LOW_VOLTAGE)
	{
		bk_pm_sleep_mode_set(PM_MODE_DEFAULT);
		bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_APP,0x0,0x0);
	}
	else
	{
		bk_pm_sleep_mode_set(PM_MODE_DEFAULT);
		bk_pm_module_vote_sleep_ctrl(s_pm_vote1,0x0,0x0);
		bk_pm_module_vote_sleep_ctrl(s_pm_vote2,0x0,0x0);
		bk_pm_module_vote_sleep_ctrl(s_pm_vote3,0x0,0x0);
	}
	os_printf("cli_pm_touch_callback\r\n");
}
#endif
void cli_pm_gpio_callback(gpio_id_t gpio_id)
{
	if(s_cli_sleep_mode == PM_MODE_DEEP_SLEEP)//when wakeup from deep sleep, all thing initial
	{
		bk_pm_sleep_mode_set(PM_MODE_DEFAULT);
	}
	else if(s_cli_sleep_mode == PM_MODE_LOW_VOLTAGE)
	{
		bk_pm_sleep_mode_set(PM_MODE_DEFAULT);
		bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_APP,0x0,0x0);
	}
	else
	{
		bk_pm_sleep_mode_set(PM_MODE_DEFAULT);
		bk_pm_module_vote_sleep_ctrl(s_pm_vote1,0x0,0x0);
		bk_pm_module_vote_sleep_ctrl(s_pm_vote2,0x0,0x0);
		bk_pm_module_vote_sleep_ctrl(s_pm_vote3,0x0,0x0);
	}
	os_printf("cli_pm_gpio_callback\r\n");
}

#define PM_MANUAL_LOW_VOL_VOTE_ENABLE    (0)
extern void stop_slave_core(void);

static void cli_pm_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	UINT32 pm_sleep_mode = 0;
	UINT32 pm_vote1 = 0,pm_vote2 = 0,pm_vote3=0;
	UINT32 pm_wake_source = 0;
	UINT32 pm_param1 = 0,pm_param2 = 0,pm_param3 = 0;
	//rtc_wakeup_param_t      rtc_wakeup_param         = {0};
	system_wakeup_param_t   system_wakeup_param      = {0};
	#if (!CONFIG_GPIO_WAKEUP_SUPPORT) && (!CONFIG_SLAVE_CORE)
	gpio_wakeup_param_t     gpio_wakeup_param        = {0};
	#endif
	#if CONFIG_TOUCH
	touch_wakeup_param_t    touch_wakeup_param       = {0};
	#endif
	usbplug_wakeup_param_t  usbplug_wakeup_param     = {0};

	if (argc != 9) 
	{
		os_printf("set low power parameter invalid %d\r\n",argc);
		return;
	}
	pm_sleep_mode  = os_strtoul(argv[1], NULL, 10);
	pm_wake_source = os_strtoul(argv[2], NULL, 10);
	pm_vote1       = os_strtoul(argv[3], NULL, 10);
	pm_vote2       = os_strtoul(argv[4], NULL, 10);
	pm_vote3       = os_strtoul(argv[5], NULL, 10);
	pm_param1      = os_strtoul(argv[6], NULL, 10);
	pm_param2      = os_strtoul(argv[7], NULL, 10);
	pm_param3      = os_strtoul(argv[8], NULL, 10);
	
	os_printf("cli_pm_cmd %d %d %d %d %d %d %d!!! \r\n",
				pm_sleep_mode,
				pm_wake_source,
				pm_vote1,
				pm_vote2,
				pm_vote3,
				pm_param1,
				pm_param2);
	if((pm_sleep_mode > PM_MODE_DEFAULT)||(pm_wake_source > PM_WAKEUP_SOURCE_INT_NONE))
	{
		os_printf("set low power  parameter value  invalid\r\n");
		return;
	}

	if(pm_sleep_mode == PM_MODE_DEEP_SLEEP)
	{
		if((pm_vote1 > PM_POWER_MODULE_NAME_NONE) ||(pm_vote2 > PM_POWER_MODULE_NAME_NONE) ||(pm_vote3 > PM_POWER_MODULE_NAME_NONE))
		{
			os_printf("set pm vote deepsleep parameter value invalid\r\n");
			return;
		}
	}

	if(pm_sleep_mode == PM_MODE_LOW_VOLTAGE)
	{
		if((pm_vote1 > PM_SLEEP_MODULE_NAME_NONE) ||(pm_vote2 > PM_SLEEP_MODULE_NAME_NONE) ||(pm_vote3 > PM_SLEEP_MODULE_NAME_NONE))
		{
			os_printf("set pm vote low vol parameter value invalid\r\n");
			return;
		}
	}

	s_cli_sleep_mode  = pm_sleep_mode;
	s_pm_vote1 = pm_vote1;
	s_pm_vote2 = pm_vote2;
	s_pm_vote3 = pm_vote3;




	/*set sleep mode*/
	//bk_pm_sleep_mode_set(pm_sleep_mode);

	/*set wakeup source*/
	if(pm_wake_source == PM_WAKEUP_SOURCE_INT_RTC)
	{
		//rtc_wakeup_param.period = pm_param1;
		//rtc_wakeup_param.isr_callback = cli_pm_rtc_callback;
		#if CONFIG_AON_RTC
		alarm_info_t low_valtage_alarm = {
										"low_vol",
										pm_param1*RTC_TICKS_PER_1MS,
										1,
										cli_pm_rtc_callback,
										NULL
										};

		//force unregister previous if doesn't finish.
		bk_alarm_unregister(AON_RTC_ID_1, low_valtage_alarm.name);
		bk_alarm_register(AON_RTC_ID_1, &low_valtage_alarm);
		#endif
		bk_pm_wakeup_source_set(PM_WAKEUP_SOURCE_INT_RTC, NULL);

	}
	else if(pm_wake_source == PM_WAKEUP_SOURCE_INT_GPIO)
	{
	#if CONFIG_GPIO_WAKEUP_SUPPORT
		bk_gpio_register_isr(pm_param1, cli_pm_gpio_callback);
		bk_gpio_register_wakeup_source(pm_param1,pm_param2);
		bk_pm_wakeup_source_set(PM_WAKEUP_SOURCE_INT_GPIO, NULL);
	#else
		#if !CONFIG_SLAVE_CORE
		gpio_wakeup_param.gpio_id = pm_param1;
		gpio_wakeup_param.gpio_valid = PARAM_DATA_VALID;
		gpio_wakeup_param.gpio_trigger_interrupt_type = pm_param2;

		bk_pm_wakeup_source_set(PM_WAKEUP_SOURCE_INT_GPIO, &gpio_wakeup_param);
		#endif
	#endif
	}
	else if(pm_wake_source == PM_WAKEUP_SOURCE_INT_SYSTEM_WAKE)
	{   
		if(pm_param1 == WIFI_WAKEUP)
		{
			system_wakeup_param.wifi_bt_wakeup = WIFI_WAKEUP;
		}
		else
		{
			system_wakeup_param.wifi_bt_wakeup = BT_WAKEUP;
		}

		bk_pm_wakeup_source_set(PM_WAKEUP_SOURCE_INT_SYSTEM_WAKE, &system_wakeup_param);
	}
	else if(pm_wake_source == PM_WAKEUP_SOURCE_INT_TOUCHED)
	{
		#if CONFIG_TOUCH
		touch_wakeup_param.touch_channel = pm_param1;
		bk_touch_register_touch_isr((1<< touch_wakeup_param.touch_channel), cli_pm_touch_callback, NULL);
		bk_pm_wakeup_source_set(PM_WAKEUP_SOURCE_INT_TOUCHED, &touch_wakeup_param);
		#endif
	}
	else if(pm_wake_source == PM_WAKEUP_SOURCE_INT_USBPLUG)
	{
		bk_pm_wakeup_source_set(PM_WAKEUP_SOURCE_INT_USBPLUG, &usbplug_wakeup_param);
	}
	else
	{
		;
	}

	/*vote*/
	if(pm_sleep_mode == PM_MODE_DEEP_SLEEP)
	{
		if(pm_vote3 == PM_POWER_MODULE_NAME_CPU1)
		{
			#if (CONFIG_SLAVE_CORE_OFFSET && CONFIG_SLAVE_CORE_RESET_VALUE)
				stop_slave_core();
			#endif
		}

	}
	else if(pm_sleep_mode == PM_MODE_LOW_VOLTAGE)
	{
		#if PM_MANUAL_LOW_VOL_VOTE_ENABLE
		if(pm_vote1 == PM_SLEEP_MODULE_NAME_APP)
		{
			bk_pm_module_vote_sleep_ctrl(pm_vote1,0x1,pm_param3);
		}
		else
		{
			bk_pm_module_vote_sleep_ctrl(pm_vote1,0x1,0x0);
		}

		if(pm_vote2 == PM_SLEEP_MODULE_NAME_APP)
		{
			bk_pm_module_vote_sleep_ctrl(pm_vote2,0x1,pm_param3);
		}
		else
		{
			bk_pm_module_vote_sleep_ctrl(pm_vote2,0x1,0x0);
		}

		if(pm_vote3 == PM_SLEEP_MODULE_NAME_APP)
		{
			bk_pm_module_vote_sleep_ctrl(pm_vote3,0x1,pm_param3);
		}
		else
		{
			bk_pm_module_vote_sleep_ctrl(pm_vote3,0x1,0x0);
		}
		#endif

		if((pm_vote1 == PM_SLEEP_MODULE_NAME_APP)||(pm_vote2 == PM_SLEEP_MODULE_NAME_APP)||(pm_vote3 == PM_SLEEP_MODULE_NAME_APP))
		{
			bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_APP,0x1,pm_param3);
		}
	}
	else
	{
		;//do something
	}

	bk_pm_sleep_mode_set(pm_sleep_mode);

}
static void cli_pm_debug(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	UINT32 pm_debug  = 0;
	if (argc != 2)
	{
		os_printf("set low power debug parameter invalid %d\r\n",argc);
		return;
	}

	pm_debug = os_strtoul(argv[1], NULL, 10);

	pm_debug_ctrl(pm_debug);

}
static void cli_pm_vol(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	UINT32 pm_vol  = 0;
	if (argc != 2)
	{
		os_printf("set pm voltage parameter invalid %d\r\n",argc);
		return;
	}

	pm_vol = os_strtoul(argv[1], NULL, 10);
	if ((pm_vol < 0) || (pm_vol > 7))
	{
		os_printf("set pm voltage value invalid %d\r\n",pm_vol);
		return;
	}

	bk_pm_lp_vol_set(pm_vol);

}
static void cli_pm_clk(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	UINT32 pm_clk_state  = 0;
	UINT32 pm_module_id  = 0;
	if (argc != 3)
	{
		os_printf("set pm clk parameter invalid %d\r\n",argc);
		return;
	}

	pm_module_id = os_strtoul(argv[1], NULL, 10);
	pm_clk_state = os_strtoul(argv[2], NULL, 10);
	if ((pm_clk_state < 0) || (pm_clk_state > 1) || (pm_module_id < 0) || (pm_module_id > 31))
	{
		os_printf("set pm clk value invalid %d %d\r\n",pm_clk_state,pm_module_id);
		return;
	}
	bk_pm_clock_ctrl(pm_module_id,pm_clk_state);

}
static void cli_pm_power(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	UINT32 pm_power_state  = 0;
	UINT32 pm_module_id  = 0;
	if (argc != 3)
	{
		os_printf("set pm power parameter invalid %d\r\n",argc);
		return;
	}

	pm_module_id = os_strtoul(argv[1], NULL, 10);
	pm_power_state = os_strtoul(argv[2], NULL, 10);
	if ((pm_power_state < 0) || (pm_power_state > 1) || (pm_module_id < 0) || (pm_module_id > 31))
	{
		os_printf("set pm power value invalid %d %d \r\n",pm_power_state,pm_module_id);
		return;
	}

	bk_pm_module_vote_power_ctrl(pm_module_id,pm_power_state);

}
static void cli_pm_freq(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	UINT32 pm_freq  = 0;
	UINT32 pm_module_id  = 0;
	pm_cpu_freq_e module_freq = 0;
	pm_cpu_freq_e current_max_freq = 0;
	if (argc != 3)
	{
		os_printf("set pm freq parameter invalid %d\r\n",argc);
		return;
	}

	pm_module_id = os_strtoul(argv[1], NULL, 10);
	pm_freq = os_strtoul(argv[2], NULL, 10);
	if ((pm_freq < 0) || (pm_freq > 3) || (pm_module_id < 0) || (pm_module_id > PM_DEV_ID_MAX))
	{
		os_printf("set pm freq value invalid %d %d \r\n",pm_freq,pm_module_id);
		return;
	}

	bk_pm_module_vote_cpu_freq(pm_module_id,pm_freq);

	module_freq =  bk_pm_module_current_cpu_freq_get(pm_module_id);

	current_max_freq = bk_pm_current_max_cpu_freq_get();

	os_printf("pm cpu freq test id: %d; freq: %d; current max cpu freq: %d;\r\n",pm_module_id,module_freq,current_max_freq);

}
static void cli_pm_lpo(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	UINT32 pm_lpo  = 0;
	if (argc != 2)
	{
		os_printf("set pm lpo parameter invalid %d\r\n",argc);
		return;
	}

	pm_lpo = os_strtoul(argv[1], NULL, 10);
	if ((pm_lpo < 0) || (pm_lpo > 3))
	{
		os_printf("set  pm lpo value invalid %d\r\n",pm_lpo);
		return;
	}

	bk_pm_lpo_src_set(pm_lpo);

}
static void cli_pm_ctrl(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	UINT32 pm_ctrl  = 0;
	if (argc != 2)
	{
		os_printf("set pm ctrl parameter invalid %d\r\n",argc);
		return;
	}

	pm_ctrl = os_strtoul(argv[1], NULL, 10);
	if ((pm_ctrl < 0) || (pm_ctrl > 1))
	{
		os_printf("set pm ctrl value invalid %d\r\n",pm_ctrl);
		return;
	}

	bk_pm_mcu_pm_ctrl(pm_ctrl);

}
static void cli_pm_pwr_state(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	UINT32 pm_pwr_module        = 0;
	UINT32 pm_pwr_module_state  = 0;
	if (argc != 2)
	{
		os_printf("set pm pwr state parameter invalid %d\r\n",argc);
		return;
	}

	pm_pwr_module = os_strtoul(argv[1], NULL, 10);
	if ((pm_pwr_module < 0) || (pm_pwr_module >= PM_POWER_MODULE_NAME_NONE))
	{
		os_printf("pm module[%d] not support ,get power state fail\r\n",pm_pwr_module);
		return;
	}

	pm_pwr_module_state = bk_pm_module_power_state_get(pm_pwr_module);
	os_printf("Get module[%d] power state[%d] \r\n",pm_pwr_module,pm_pwr_module_state);

}
static void cli_pm_auto_vote(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	UINT32 pm_ctrl  = 0;
	if (argc != 2)
	{
		os_printf("set pm auto_vote parameter invalid %d\r\n",argc);
		return;
	}

	pm_ctrl = os_strtoul(argv[1], NULL, 10);
	if ((pm_ctrl < 0) || (pm_ctrl > 1))
	{
		os_printf("set pm auto vote value invalid %d\r\n",pm_ctrl);
		return;
	}

	bk_pm_app_auto_vote_state_set(pm_ctrl);

}
static void cli_pm_vote_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	UINT32 pm_sleep_mode   = 0;
	UINT32 pm_vote         = 0;
	UINT32 pm_vote_value   = 0;
	UINT32 pm_sleep_time   = 0;
	if (argc != 5) 
	{
		os_printf("set low power vote parameter invalid %d\r\n",argc);
		return;
	}
	pm_sleep_mode        = os_strtoul(argv[1], NULL, 10);
	pm_vote              = os_strtoul(argv[2], NULL, 10);
	pm_vote_value        = os_strtoul(argv[3], NULL, 10);
	pm_sleep_time        = os_strtoul(argv[4], NULL, 10);
	if((pm_sleep_mode > LOW_POWER_MODE_NONE)|| (pm_vote > POWER_MODULE_NAME_NONE)||(pm_vote_value > 1))
	{
		os_printf("set low power vote parameter value  invalid\r\n");
		return;
	}
	/*vote*/
	if(pm_sleep_mode == LOW_POWER_DEEP_SLEEP)
	{
		if((pm_vote == POWER_MODULE_NAME_BTSP)||(pm_vote == POWER_MODULE_NAME_WIFIP_MAC))
		{
			bk_pm_module_vote_power_ctrl(pm_vote,pm_vote_value);
		}
	}
	else if(pm_sleep_mode == LOW_POWER_MODE_LOW_VOLTAGE)
	{
		bk_pm_module_vote_sleep_ctrl(pm_vote,pm_vote_value,pm_sleep_time);
	}
	else
	{
		;//do something
	}

	
}
#define CLI_DVFS_FREQUNCY_DIV_MAX      (15)
#define CLI_DVFS_FREQUNCY_DIV_BUS_MAX  (1)
static void cli_dvfs_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	UINT32 cksel_core = 0;
	UINT32 ckdiv_core = 0;
	UINT32 ckdiv_bus  = 0;
	UINT32 ckdiv_cpu0 = 0;
	UINT32 ckdiv_cpu1 = 0;
	UINT32 clk_param  = 0;

	if (argc != 6) 
	{
		os_printf("set dvfs parameter invalid %d\r\n",argc);
		return;
	}

	GLOBAL_INT_DECLARATION();
	cksel_core   = os_strtoul(argv[1], NULL, 10);
	ckdiv_core   = os_strtoul(argv[2], NULL, 10);
	ckdiv_bus    = os_strtoul(argv[3], NULL, 10);
	ckdiv_cpu0   = os_strtoul(argv[4], NULL, 10);
	ckdiv_cpu1   = os_strtoul(argv[5], NULL, 10);

	os_printf("cli_dvfs_cmd %d %d %d %d %d !!! \r\n",
				cksel_core,
				ckdiv_core,
				ckdiv_bus,
				ckdiv_cpu0,
				ckdiv_cpu1);
	GLOBAL_INT_DISABLE();
	if(cksel_core > 3)
	{
		os_printf("set dvfs cksel core > 3 invalid %d\r\n",cksel_core);
		GLOBAL_INT_RESTORE();
		return;
	}

	if((ckdiv_core > CLI_DVFS_FREQUNCY_DIV_MAX) || (ckdiv_bus > CLI_DVFS_FREQUNCY_DIV_BUS_MAX)||(ckdiv_cpu0 > CLI_DVFS_FREQUNCY_DIV_MAX)||(ckdiv_cpu0 > CLI_DVFS_FREQUNCY_DIV_MAX))
	{
		os_printf("set dvfs ckdiv_core ckdiv_bus ckdiv_cpu0  ckdiv_cpu0  > 15 invalid\r\n");
		GLOBAL_INT_RESTORE();
		return;
	}

	clk_param = 0;
	clk_param = sys_drv_all_modules_clk_div_get(CLK_DIV_REG0);
	if(((clk_param >> 0x4)&0x3) > cksel_core)//when it from the higher frequecy to lower frqquecy
	{
		/*1.core clk select*/
		clk_param = 0;
		clk_param = sys_drv_all_modules_clk_div_get(CLK_DIV_REG0);
		clk_param &=  ~(0x3 << 4);
		clk_param |=  cksel_core << 4;
		sys_drv_all_modules_clk_div_set(CLK_DIV_REG0,clk_param);

		/*2.config bus and core clk div*/
		clk_param = 0;
		clk_param = sys_drv_all_modules_clk_div_get(CLK_DIV_REG0);
		clk_param &=  ~((0x1 << 6)|(0xF << 0));
		clk_param |=  ckdiv_core << 0;
		clk_param |=  (ckdiv_bus&0x1) << 6;
		sys_drv_all_modules_clk_div_set(CLK_DIV_REG0,clk_param);

		/*3.config cpu clk div*/
		sys_drv_cpu_clk_div_set(0,ckdiv_cpu0);

		sys_drv_cpu_clk_div_set(1,ckdiv_cpu1);

	}
	else//when it from the lower frequecy to higher frqquecy
	{
		/*1.config bus and core clk div*/
		clk_param = 0;
		clk_param = sys_drv_all_modules_clk_div_get(CLK_DIV_REG0);
		clk_param &=  ~(0xF << 0);
		clk_param |=  ckdiv_core << 0;
		sys_drv_all_modules_clk_div_set(CLK_DIV_REG0,clk_param);
	
		clk_param = 0;
		clk_param = sys_drv_all_modules_clk_div_get(CLK_DIV_REG0);
		clk_param &=  ~(0x1 << 6);
		clk_param |=  (ckdiv_bus&0x1) << 6;
		sys_drv_all_modules_clk_div_set(CLK_DIV_REG0,clk_param);

		/*2.config cpu clk div*/
		sys_drv_cpu_clk_div_set(0,ckdiv_cpu0);

		sys_drv_cpu_clk_div_set(1,ckdiv_cpu1);

		/*3.core clk select*/

		clk_param = 0;
		clk_param = sys_drv_all_modules_clk_div_get(CLK_DIV_REG0);
		clk_param &=  ~(0x3 << 4);
		clk_param |=  cksel_core << 4;
		sys_drv_all_modules_clk_div_set(CLK_DIV_REG0,clk_param);
	}
	GLOBAL_INT_RESTORE();
	os_printf("switch cpu frequency ok 0x%x 0x%x 0x%x\r\n",sys_drv_all_modules_clk_div_get(CLK_DIV_REG0),sys_drv_cpu_clk_div_get(0),sys_drv_cpu_clk_div_get(1));
}
#if CONFIG_AON_RTC
static UINT32 s_pre_tick;
static void cli_pm_timer_isr(timer_id_t chan)
{
	UINT32 current_tick = 0;
	current_tick = bk_aon_rtc_get_current_tick(AON_RTC_ID_1);

    os_printf("rosc accuracy count %d %d %d \r\n",current_tick - s_pre_tick,current_tick,s_pre_tick);
	s_pre_tick = current_tick;
}
#endif
static void cli_pm_rosc_accuracy(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
#if CONFIG_AON_RTC
	UINT32 timer_count_interval = 0;

	if (argc != 2)
	{
		os_printf("set osc_accurac parameter invalid %d\r\n",argc);
		return;
	}

	timer_count_interval   = os_strtoul(argv[1], NULL, 10);
	bk_timer_start(0, timer_count_interval, cli_pm_timer_isr);
#endif
}

#endif
#if CONFIG_MCU_PS
static void cli_deep_sleep_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	PS_DEEP_CTRL_PARAM deep_sleep_param;

	deep_sleep_param.wake_up_way            = 0;

	deep_sleep_param.gpio_index_map         = os_strtoul(argv[1], NULL, 16);
	deep_sleep_param.gpio_edge_map          = os_strtoul(argv[2], NULL, 16);
	deep_sleep_param.gpio_last_index_map    = os_strtoul(argv[3], NULL, 16);
	deep_sleep_param.gpio_last_edge_map     = os_strtoul(argv[4], NULL, 16);
	deep_sleep_param.sleep_time             = os_strtoul(argv[5], NULL, 16);
	deep_sleep_param.wake_up_way            = os_strtoul(argv[6], NULL, 16);
	deep_sleep_param.gpio_stay_lo_map       = os_strtoul(argv[7], NULL, 16);
	deep_sleep_param.gpio_stay_hi_map       = os_strtoul(argv[8], NULL, 16);

	if (argc == 9) {
		os_printf("---deep sleep test param : 0x%0X 0x%0X 0x%0X 0x%0X %d %d\r\n",
				  deep_sleep_param.gpio_index_map,
				  deep_sleep_param.gpio_edge_map,
				  deep_sleep_param.gpio_last_index_map,
				  deep_sleep_param.gpio_last_edge_map,
				  deep_sleep_param.sleep_time,
				  deep_sleep_param.wake_up_way);

#if (!CONFIG_SOC_BK7271) && (CONFIG_DEEP_PS)
		bk_enter_deep_sleep_mode(&deep_sleep_param);
#endif
	} else
		os_printf("---argc error!!! \r\n");
}

void cli_mac_ps_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
#if (CONFIG_MCU_PS | CONFIG_STA_PS)
	UINT32 dtim = 0;
#endif

#if PS_SUPPORT_MANUAL_SLEEP
	UINT32 standby_time = 0;
	UINT32 dtim_wait_time = 0;
#endif

	if (argc < 2)
		goto _invalid_mac_ps_arg;
#if CONFIG_FAKE_RTC_PS
	if (0 == os_strcmp(argv[1], "idleps")) {
		GLOBAL_INT_DECLARATION();
		int count = 0;
		bk_printf("[ARF]rwnxl_reset_evt\r\n");
		HAL_FATAL_ERROR_RECOVER(0); // rwnxl_reset_evt(0);

		rtos_delay_milliseconds(10);

		while (1) {
			GLOBAL_INT_DISABLE();
			evt_field_t field = ke_evt_get();
			GLOBAL_INT_RESTORE();

			if (!(field & KE_EVT_RESET_BIT))
				break;

			rtos_delay_milliseconds(10);
			if (++count > 10000) {
				bk_printf("%s: failed\r\n", __func__);
				break;
			}
		}

		//bk_enable_unconditional_sleep();
		count = 100;
		while ((1 == bk_unconditional_normal_sleep(0xFFFFFFFF, 1))) {
			rtos_delay_milliseconds(2);
			count--;
			if (count == 0) {
				bk_printf("IDLE_SLEEP timeout\r\n");
				break;
			}
		}

		bk_printf("idle Sleep out\r\n");
	}
#endif

#if CONFIG_STA_PS
	else if (0 == os_strcmp(argv[1], "rfwkup")) {
		mac_ps_wakeup_immediately();
		os_printf("done.\r\n");
	} else if (0 == os_strcmp(argv[1], "bcmc")) {
		if (argc != 3)
			goto _invalid_mac_ps_arg;

		dtim = os_strtoul(argv[2], NULL, 10);

		if (dtim == 0 || dtim == 1)
			power_save_sm_set_all_bcmc(dtim);
		else
			goto _invalid_mac_ps_arg;
	} else if (0 == os_strcmp(argv[1], "listen")) {
		if (argc != 4)
			goto _invalid_mac_ps_arg;

		if (0 == os_strcmp(argv[2], "dtim")) {
			dtim = os_strtoul(argv[3], NULL, 10);
			power_save_set_dtim_multi(dtim);

		} else
			goto _invalid_mac_ps_arg;

	} else if (0 == os_strcmp(argv[1], "dump")) {
#if CONFIG_MCU_PS
		mcu_ps_dump();
#endif
		power_save_dump();
	}
#endif
	else
		goto _invalid_mac_ps_arg;

	return;
_invalid_mac_ps_arg:
	os_printf("Usage:mac_ps {func} [param1] [param2]\r\n");
#if CONFIG_FAKE_RTC_PS
	os_printf("mac_ps {idleps}\r\n");
#endif
#if CONFIG_STA_PS
	os_printf("mac_ps {rfwkup}\r\n");
	os_printf("mac_ps {bcmc} {1|0}\r\n");
	os_printf("mac_ps {listen} {dtim} {dtim_val}\r\n");
#endif
	os_printf("mac_ps dump\r\n");
}

void cli_pwr_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int pwr = 0;

	if (argc != 3) {
		os_printf("Usage: pwr [hex:5~15].");
		return;
	}

	pwr = os_strtoul(argv[2], NULL, 16);
	if (0 == os_strcmp(argv[1], "sta")) {
		bk_wifi_sta_set_power(pwr);
	} else if (0 == os_strcmp(argv[1], "ap")) {
		bk_wifi_ap_set_power(pwr);
	}
}
#endif
#define PWR_CMD_CNT (sizeof(s_pwr_commands) / sizeof(struct cli_command))
static const struct cli_command s_pwr_commands[] = {
#if CONFIG_MCU_PS
	{"ps", "ps {rfdtim|mcudtim|rf_timer} {1|0}", cli_ps_cmd},
	{"mac_ps", "mac_ps {func} [param1] [param2]", cli_mac_ps_cmd},
	{"deep_sleep", "deep_sleep [param]", cli_deep_sleep_cmd},
#endif
#if CONFIG_SYSTEM_CTRL
	{"pm", "pm [sleep_mode] [wake_source] [vote1] [vote2] [vote3] [param1] [param2] [param3]", cli_pm_cmd},
	{"dvfs", "dvfs [cksel_core] [ckdiv_core] [ckdiv_bus] [ckdiv_cpu0] [ckdiv_cpu1]", cli_dvfs_cmd},
	{"pm_vote", "pm_vote [pm_sleep_mode] [pm_vote] [pm_vote_value] [pm_sleep_time]", cli_pm_vote_cmd},
	{"pm_debug", "pm_debug [debug_en_value]", cli_pm_debug},
	{"pm_lpo", "pm_lpo [lpo_type]", cli_pm_lpo},
	{"pm_vol", "pm_vol [vol_value]", cli_pm_vol},
	{"pm_clk", "pm_clk [module_name][clk_state]", cli_pm_clk},
	{"pm_power", "pm_power [module_name][ power state]", cli_pm_power},
	{"pm_freq", "pm_freq [module_name][ frequency]", cli_pm_freq},
	{"pm_ctrl", "pm_ctrl [ctrl_value]", cli_pm_ctrl},
	{"pm_pwr_state", "pm_pwr_state [pwr_state]", cli_pm_pwr_state},
	{"pm_auto_vote", "pm_auto_vote [auto_vote_value]", cli_pm_auto_vote},
	{"pm_rosc", "pm_rosc [rosc_accuracy_count_interval]", cli_pm_rosc_accuracy},
#endif
#if CONFIG_TPC_PA_MAP
	{"pwr", "pwr {sta|ap} pwr", cli_pwr_cmd },
#endif
};

int cli_pwr_init(void)
{
	return cli_register_commands(s_pwr_commands, PWR_CMD_CNT);
}
