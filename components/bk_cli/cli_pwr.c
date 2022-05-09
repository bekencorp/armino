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
#if CONFIG_AON_RTC
static UINT32 s_cli_sleep_mode = 0;
static UINT32 s_pm_vote1 = 0;
static UINT32 s_pm_vote2 = 0;
static UINT32 s_pm_vote3 = 0;
static void cli_pm_rtc_callback()
{
	if(s_cli_sleep_mode == LOW_POWER_DEEP_SLEEP)//when wakeup from deep sleep, all thing initial
	{
		pm_sleep_mode_set(LOW_POWER_MODE_NONE);
		//pm_power_ctrl(s_pm_vote1,POWER_MODULE_STATE_ON);
		//pm_power_ctrl(s_pm_vote2,POWER_MODULE_STATE_ON);
		//pm_power_ctrl(s_pm_vote3,POWER_MODULE_STATE_ON);
	}
	else if(s_cli_sleep_mode == LOW_POWER_MODE_LOW_VOLTAGE)
	{
		pm_sleep_mode_set(LOW_POWER_MODE_NONE);
		pm_module_vote_sleep_ctrl(s_pm_vote1,0x0,0x0);
		pm_module_vote_sleep_ctrl(s_pm_vote2,0x0,0x0);
		pm_module_vote_sleep_ctrl(s_pm_vote3,0x0,0x0);
	}
	else
	{
		pm_sleep_mode_set(LOW_POWER_MODE_NONE);
		pm_module_vote_sleep_ctrl(s_pm_vote1,0x0,0x0);
		pm_module_vote_sleep_ctrl(s_pm_vote2,0x0,0x0);
		pm_module_vote_sleep_ctrl(s_pm_vote3,0x0,0x0);
	}
	
	os_printf("cli_pm_callback\r\n");
	
}
extern void stop_slave_core(void);
static void cli_pm_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	UINT32 pm_sleep_mode = 0;
	UINT32 pm_vote1 = 0,pm_vote2 = 0,pm_vote3=0;
	UINT32 pm_wake_source = 0;
	UINT32 pm_param1 = 0,pm_param2 = 0,pm_param3 = 0;
	rtc_wakeup_param_t      rtc_wakeup_param         = {0};
	system_wakeup_param_t   system_wakeup_param      = {0};
	gpio_wakeup_param_t     gpio_wakeup_param        = {0};
	touch_wakeup_param_t    touch_wakeup_param       = {0};
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
	pm_param1       = os_strtoul(argv[6], NULL, 10);
	pm_param2       = os_strtoul(argv[7], NULL, 10);
	pm_param3       = os_strtoul(argv[8], NULL, 10);
	
	os_printf("cli_pm_cmd %d %d %d %d %d %d %d!!! \r\n",
				pm_sleep_mode,
				pm_wake_source,
				pm_vote1,
				pm_vote2,
				pm_vote3,
				pm_param1,
				pm_param2);
	if((pm_sleep_mode > LOW_POWER_MODE_NONE)|| (pm_vote1 > POWER_MODULE_NAME_NONE) ||(pm_vote2 > POWER_MODULE_NAME_NONE) ||(pm_vote3 > POWER_MODULE_NAME_NONE)||(pm_wake_source > WAKEUP_SOURCE_INT_NONE))
	{
		os_printf("set low power  parameter value  invalid\r\n");
		return;
	}

	s_cli_sleep_mode  = pm_sleep_mode;
	s_pm_vote1 = pm_vote1;
	s_pm_vote2 = pm_vote2;
	s_pm_vote3 = pm_vote3;

	/*set sleep mode*/
	pm_sleep_mode_set(pm_sleep_mode);

	/*set wakeup source*/
	if(pm_wake_source == WAKEUP_SOURCE_INT_RTC)
	{
		rtc_wakeup_param.period = pm_param1;
		rtc_wakeup_param.isr_callback = cli_pm_rtc_callback;
		pm_wakeup_source_set(WAKEUP_SOURCE_INT_RTC, &rtc_wakeup_param);
	}
	else if(pm_wake_source == WAKEUP_SOURCE_INT_GPIO)
	{
		gpio_wakeup_param.gpio_id = pm_param1;
		gpio_wakeup_param.gpio_valid = PARAM_DATA_VALID;
		gpio_wakeup_param.gpio_trigger_interrupt_type = pm_param2;
		pm_wakeup_source_set(WAKEUP_SOURCE_INT_GPIO, &gpio_wakeup_param);
	}
	else if(pm_wake_source == WAKEUP_SOURCE_INT_SYSTEM_WAKE)
	{   
		if(pm_param1 == WIFI_WAKEUP)
		{
			system_wakeup_param.wifi_bt_wakeup = WIFI_WAKEUP;
		}
		else
		{
			system_wakeup_param.wifi_bt_wakeup = BT_WAKEUP;
		}

		pm_wakeup_source_set(WAKEUP_SOURCE_INT_SYSTEM_WAKE, &system_wakeup_param);
	}
	else if(pm_wake_source == WAKEUP_SOURCE_INT_TOUCHED)
	{
		touch_wakeup_param.touch_channel = pm_param1;
		pm_wakeup_source_set(WAKEUP_SOURCE_INT_TOUCHED, &touch_wakeup_param);
	}
	else if(pm_wake_source == WAKEUP_SOURCE_INT_USBPLUG)
	{
		pm_wakeup_source_set(WAKEUP_SOURCE_INT_USBPLUG, &usbplug_wakeup_param);
	}
	else
	{
		;
	}

	/*vote*/
	if(pm_sleep_mode == LOW_POWER_DEEP_SLEEP)
	{
		if((pm_vote1 == POWER_MODULE_NAME_BTSP)||(pm_vote1 == POWER_MODULE_NAME_WIFIP_MAC))
		{
			pm_module_vote_power_ctrl(pm_vote1,POWER_MODULE_STATE_OFF);
		}
		
		if((pm_vote2 == POWER_MODULE_NAME_BTSP)||(pm_vote2 == POWER_MODULE_NAME_WIFIP_MAC))
		{
			pm_module_vote_power_ctrl(pm_vote2,POWER_MODULE_STATE_OFF);
		}

		if(pm_vote3 == POWER_MODULE_NAME_CPU1)
		{
			#if (CONFIG_SLAVE_CORE_OFFSET && CONFIG_SLAVE_CORE_RESET_VALUE)
				stop_slave_core();
			#endif
		}

	}
	else if(pm_sleep_mode == LOW_POWER_MODE_LOW_VOLTAGE)
	{
		if(pm_vote1 == POWER_MODULE_NAME_BTSP)
		{
			pm_module_vote_sleep_ctrl(pm_vote1,0x1,pm_param3);
		}
		else
		{
			pm_module_vote_sleep_ctrl(pm_vote1,0x1,0x0);
		}

		if(pm_vote2 == POWER_MODULE_NAME_BTSP)
		{
			pm_module_vote_sleep_ctrl(pm_vote2,0x1,pm_param3);
		}
		else
		{
			pm_module_vote_sleep_ctrl(pm_vote2,0x1,0x0);
		}

		if(pm_vote3 == POWER_MODULE_NAME_BTSP)
		{
			pm_module_vote_sleep_ctrl(pm_vote3,0x1,pm_param3);
		}
		else
		{
			pm_module_vote_sleep_ctrl(pm_vote3,0x1,0x0);
		}
	}
	else
	{
		;//do something
	}

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
			pm_module_vote_power_ctrl(pm_vote,pm_vote_value);
		}
	}
	else if(pm_sleep_mode == LOW_POWER_MODE_LOW_VOLTAGE)
	{
		pm_module_vote_sleep_ctrl(pm_vote,pm_vote_value,pm_sleep_time);
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
		return;
	}

	if((ckdiv_core > CLI_DVFS_FREQUNCY_DIV_MAX) || (ckdiv_bus > CLI_DVFS_FREQUNCY_DIV_BUS_MAX)||(ckdiv_cpu0 > CLI_DVFS_FREQUNCY_DIV_MAX)||(ckdiv_cpu0 > CLI_DVFS_FREQUNCY_DIV_MAX))
	{
		os_printf("set dvfs ckdiv_core ckdiv_bus ckdiv_cpu0  ckdiv_cpu0  > 15 invalid\r\n");
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
}
#endif
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

#define PWR_CMD_CNT (sizeof(s_pwr_commands) / sizeof(struct cli_command))
static const struct cli_command s_pwr_commands[] = {
	{"ps", "ps {rfdtim|mcudtim|rf_timer} {1|0}", cli_ps_cmd},
	{"mac_ps", "mac_ps {func} [param1] [param2]", cli_mac_ps_cmd},
	{"deep_sleep", "deep_sleep [param]", cli_deep_sleep_cmd},
#if CONFIG_AON_RTC
	{"pm", "pm [sleep_mode] [wake_source] [vote1] [vote2] [vote3] [param1] [param2] [param3]", cli_pm_cmd},
	{"dvfs", "dvfs [cksel_core] [ckdiv_core] [ckdiv_bus] [ckdiv_cpu0] [ckdiv_cpu1]", cli_dvfs_cmd},
	{"pm_vote", "pm_vote [pm_sleep_mode] [pm_vote] [pm_vote_value] [pm_sleep_time]", cli_pm_vote_cmd},
	{"pm_debug", "pm_debug [debug_en_value]", cli_pm_debug},
#endif
#if CONFIG_TPC_PA_MAP
	{"pwr", "pwr {sta|ap} pwr", cli_pwr_cmd },
#endif
};

int cli_pwr_init(void)
{
	return cli_register_commands(s_pwr_commands, PWR_CMD_CNT);
}
