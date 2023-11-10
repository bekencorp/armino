#include "cli.h"
#include <components/system.h>
#include <os/str.h>
#include "bk_sys_ctrl.h"
#include "bk_drv_model.h"
#include "release.h"
#include <driver/efuse.h>
#include <components/system.h>
#include <modules/wifi.h>
#include "sys_driver.h"
#include <driver/wdt.h>
#include <bk_wdt.h>
#include "gpio_driver.h"
#include <driver/gpio.h>
#include "bk_ps.h"
#include "bk_pm_internal_api.h"
#include "reset_reason.h"
#include <modules/pm.h>
#if CONFIG_CACHE_ENABLE
#include "cache.h"
#endif
#include <components/ate.h>


#if (CONFIG_EFUSE)
static void efuse_cmd_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
static void efuse_mac_cmd_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
#endif //#if (CONFIG_EFUSE)


static int hex2num(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	return -1;
}


static int hex2byte(const char *hex)
{
	int a, b;
	a = hex2num(*hex++);
	if (a < 0)
		return -1;
	b = hex2num(*hex++);
	if (b < 0)
		return -1;
	return (a << 4) | b;
}

int hexstr2bin_cli(const char *hex, u8 *buf, size_t len)
{
	size_t i;
	int a;
	const char *ipos = hex;
	u8 *opos = buf;

	for (i = 0; i < len; i++) {
		a = hex2byte(ipos);
		if (a < 0)
			return -1;
		*opos++ = a;
		ipos += 2;
	}
	return 0;
}

__maybe_unused static void cli_misc_help(void)
{
	CLI_LOGI("pwm_driver init {26M|DCO}\n");
#if (CONFIG_WIFI_ENABLE)
	CLI_LOGI("mac <mac>, get/set mac. e.g. mac c89346000001\r\n");
#endif

#if (!CONFIG_SOC_BK7231)
#if (CONFIG_EFUSE)
	CLI_LOGI("efuse [-r addr] [-w addr data]\r\n");
	CLI_LOGI("efusemac [-r] [-w] [mac]\r\n");
#endif
#endif
#if (CONFIG_MASTER_CORE)
	CLI_LOGI("bootcore1 boot slave core,1:start,0:stop,others:start and stop many times\r\n");
#endif
	CLI_LOGI("setjtagmode set jtag mode [cpu0|cpu1] [group1|group2]\r\n");
	CLI_LOGI("setcpufreq [cksel] [ckdiv_core] [ckdiv_bus] [ckdiv_cpu]\r\n");
#if CONFIG_COMMON_IO
	CLI_LOGI("testcommonio test common io\r\n");
#endif

}

extern volatile const uint8_t build_version[];

void get_version(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	CLI_LOGI("get_version\r\n");
	//os_printf("firmware version : %s", BEKEN_SDK_REV);
	CLI_LOGI("firmware version : %s", build_version);
}

void cli_show_reset_reason(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	show_reset_reason();
}


void get_id(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	CLI_LOGI("get_id\r\n");
	//os_printf("id : %x_%x",sddev_control(DD_DEV_TYPE_SCTRL,CMD_GET_DEVICE_ID, NULL), sddev_control(DD_DEV_TYPE_SCTRL,CMD_GET_CHIP_ID, NULL));
	CLI_LOGI("id : %x_%x",sys_drv_get_device_id(), sys_drv_get_chip_id());
}

static void uptime_Command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	CLI_LOGI("UP time %ldms\r\n", rtos_get_time());
}

void reboot(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	bk_reboot();
}

#if (!CONFIG_SOC_BK7231)
#if (CONFIG_EFUSE)
static void efuse_cmd_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint8_t addr, data;

	if (argc == 3) {
		if (os_strncmp(argv[1], "-r", 2) == 0) {
			hexstr2bin_cli(argv[2], &addr, 1);
			bk_efuse_read_byte(addr, &data);
			CLI_LOGI("efuse read: addr-0x%02x, data-0x%02x\r\n",
					  addr, data);
		}
	} else if (argc == 4) {
		if (os_strncmp(argv[1], "-w", 2) == 0)  {
			hexstr2bin_cli(argv[2], &addr, 1);
			hexstr2bin_cli(argv[3], &data, 6);
			CLI_LOGI("efuse write: addr-0x%02x, data-0x%02x, ret:%d\r\n",
					  addr, data, bk_efuse_write_byte(addr, data));
		}
	} else
		cli_misc_help();
}

static void efuse_mac_cmd_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
#if CONFIG_BASE_MAC_FROM_EFUSE
	uint8_t mac[6];

	if (argc == 1) {
		if (bk_get_mac(mac, MAC_TYPE_BASE) == BK_OK)
			CLI_LOGI("MAC address: %02x-%02x-%02x-%02x-%02x-%02x\r\n",
					  mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	} else if (argc == 2) {
		if (os_strncmp(argv[1], "-r", 2) == 0) {
			if (bk_get_mac(mac, MAC_TYPE_BASE) == BK_OK)
				CLI_LOGI("MAC address: %02x-%02x-%02x-%02x-%02x-%02x\r\n",
						  mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
		}
	} else if (argc == 3) {
		if (os_strncmp(argv[1], "-w", 2) == 0)  {
			hexstr2bin_cli(argv[2], mac, 6);
			CLI_LOGI("Set MAC address: %02x-%02x-%02x-%02x-%02x-%02x\r\n",
					  mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
		}
	} else
		CLI_LOGI("efusemac [-r] [-w] [mac]\r\n");
#else
	CLI_LOGI("base mac is not from efuse\n");
#endif
}
#endif //#if (CONFIG_EFUSE)
#endif //(!CONFIG_SOC_BK7231)

#if (CONFIG_WIFI_ENABLE)
static void mac_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint8_t base_mac[BK_MAC_ADDR_LEN] = {0};
	uint8_t sta_mac[BK_MAC_ADDR_LEN] = {0};
	uint8_t ap_mac[BK_MAC_ADDR_LEN] = {0};

	if (argc == 1) {
		BK_LOG_ON_ERR(bk_get_mac(base_mac, MAC_TYPE_BASE));
		BK_LOG_ON_ERR(bk_wifi_sta_get_mac(sta_mac));
		BK_LOG_ON_ERR(bk_wifi_ap_get_mac(ap_mac));
		if (ate_is_enabled())
			os_printf("MAC address: %02x-%02x-%02x-%02x-%02x-%02x\r\n",
						base_mac[0],base_mac[1],base_mac[2],base_mac[3],base_mac[4],base_mac[5]);
		else {
			CLI_LOGI("base mac: "BK_MAC_FORMAT"\n", BK_MAC_STR(base_mac));
			CLI_LOGI("sta mac: "BK_MAC_FORMAT"\n", BK_MAC_STR(sta_mac));
			CLI_LOGI("ap mac: "BK_MAC_FORMAT"\n", BK_MAC_STR(ap_mac));
		}
	} else if (argc == 2) {
		hexstr2bin_cli(argv[1], base_mac, BK_MAC_ADDR_LEN);
		bk_set_base_mac(base_mac);
		if (ate_is_enabled())
			os_printf("Set MAC address: %02x-%02x-%02x-%02x-%02x-%02x\r\n",
						base_mac[0],base_mac[1],base_mac[2],base_mac[3],base_mac[4],base_mac[5]);
		else
			CLI_LOGI("set base mac: "BK_MAC_FORMAT"\n", BK_MAC_STR(base_mac));
	} else
		cli_misc_help();

}
#endif //#if (CONFIG_WIFI_ENABLE)

#if (CONFIG_MASTER_CORE) 
//extern void reset_slave_core(uint32 offset, uint32_t reset_value);
extern void start_slave_core(void);
extern void stop_slave_core(void);
static void boot_slave_core(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
#if (CONFIG_SLAVE_CORE_OFFSET && CONFIG_SLAVE_CORE_RESET_VALUE) 
	//uint32 offset = CONFIG_SLAVE_CORE_OFFSET;
	uint32_t reset_value = CONFIG_SLAVE_CORE_RESET_VALUE;

	if (argc == 2) {
		reset_value = os_strtoul(argv[1], NULL, 10);
	}
	
	//reset_slave_core(offset, reset_value);
	if(reset_value == 1)
	{
		bk_pm_module_vote_power_ctrl(PM_POWER_MODULE_NAME_CPU1, PM_POWER_MODULE_STATE_ON);
		start_slave_core();
	}
	else if(reset_value == 0)
	{
		stop_slave_core();
		bk_pm_module_vote_power_ctrl(PM_POWER_MODULE_NAME_CPU1, PM_POWER_MODULE_STATE_OFF);
	}
	else	//test start/stop many times.
	{
		uint32_t i = 0;

		//start first:odd will be at start status, even will be at stop status
		while(1)
		{
			bk_pm_module_vote_power_ctrl(PM_POWER_MODULE_NAME_CPU1, PM_POWER_MODULE_STATE_ON);
			start_slave_core();
			i++;
			if(i == reset_value)
				break;
			stop_slave_core();
			bk_pm_module_vote_power_ctrl(PM_POWER_MODULE_NAME_CPU1, PM_POWER_MODULE_STATE_OFF);
			i++;
			if(i == reset_value)
				break;
		}
		CLI_LOGI("boot on/off %d times.\r\n", reset_value);
	}

#endif
	CLI_LOGI("boot_slave_core end.\r\n");
}
#endif


void bk_set_jtag_mode(uint32_t cpu_id, uint32_t group_id) {
	uint32_t cksel_core = 3;
	uint32_t ckdiv_core = 3;
	uint32_t ckdiv_bus  = 0;
	uint32_t ckdiv_cpu0 = 0;
	uint32_t ckdiv_cpu1 = 0;

	if (cpu_id == 0) {
		(void)sys_drv_set_jtag_mode(0);
	} else if (cpu_id == 1) {
		(void)sys_drv_set_jtag_mode(1);
	} else {
		CLI_LOGI("Unsupported cpu id(%d).\r\n", cpu_id);
		return;
	}

	CLI_LOGI("[cksel:%d] [ckdiv_core:%d] [ckdiv_bus:%d] [ckdiv_cpu0:%d] [ckdiv_cpu1:%d]\r\n",
			cksel_core, ckdiv_core, ckdiv_bus, ckdiv_cpu0, ckdiv_cpu1);
	pm_core_bus_clock_ctrl(cksel_core, ckdiv_core, ckdiv_bus, ckdiv_cpu0, ckdiv_cpu1);

	/*close watchdog*/
#if CONFIG_INT_WDT
	bk_wdt_stop();
#endif
#if CONFIG_TASK_WDT
	bk_task_wdt_stop();
#endif

	if (group_id == 0) {
		gpio_jtag_sel(0);
	} else if (group_id == 1) {
		gpio_jtag_sel(1);
	} else {
		CLI_LOGI("Unsupported group id(%d).\r\n", group_id);
		return;
	}

}

static void set_jtag_mode(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 3) {
		cli_misc_help();
		return;
	}

	uint32_t cpu_id = 0;
	uint32_t group_id = 0;

	if (os_strcmp(argv[1], "cpu0") == 0) {
		cpu_id = 0;
		CLI_LOGI("gpio Jtag CPU0\r\n");
	} else if (os_strcmp(argv[1], "cpu1") == 0) {
		cpu_id = 1;
		CLI_LOGI("gpio Jtag CPU1\r\n");
	} else {
		cli_misc_help();
	}

	if (os_strcmp(argv[2], "group1") == 0) {
		group_id = 0;
		CLI_LOGI("gpio Jtag group1\r\n");
	} else if (os_strcmp(argv[2], "group2") == 0) {
		group_id = 1;
		CLI_LOGI("gpio Jtag group2\r\n");
	} else
		cli_misc_help();

	bk_set_jtag_mode(cpu_id, group_id);

	CLI_LOGI("set_jtag_mode end.\r\n");
}


/*
	//For BK7256 ckdiv_bus is 0x8[6]
	//For BK7236 ckdiv_bus is the same as ckdiv_cpu1 0x5[4]

	//============ BK7236 Sample begin============

		//cpu0:160m;cpu1:160m;bus:160m
		setcpufreq 3 2 0 0 0

		//cpu0:160m;cpu1:320m;bus:160m
		setcpufreq 2 0 0 0 1

		//cpu0:120m;cpu1:120m;bus:120m
		setcpufreq 3 3 0 0 0

		//cpu0:120m;cpu1:240m;bus:120m
		setcpufreq 3 1 0 0 1

	//============ BK7236 Sample end============
*/
static void set_cpu_clock_freq(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 6) {
		cli_misc_help();
		return;
	}

	uint32_t cksel_core = 3;
	uint32_t ckdiv_core = 3;
	uint32_t ckdiv_bus  = 0;
	uint32_t ckdiv_cpu0 = 0;
	uint32_t ckdiv_cpu1 = 0;


	cksel_core = os_strtoul(argv[1], NULL, 10);
	ckdiv_core = os_strtoul(argv[2], NULL, 10);
#if CONFIG_SOC_BK7256XX
	ckdiv_bus  = os_strtoul(argv[3], NULL, 10);
	ckdiv_cpu0  = os_strtoul(argv[4], NULL, 10);
#endif
	ckdiv_cpu1  = os_strtoul(argv[5], NULL, 10);

	CLI_LOGI("set_cpu_clock_freq: [cksel_core:%d] [ckdiv_core:%d] [ckdiv_bus:%d] [ckdiv_cpu0:%d] [ckdiv_cpu1:%d]\r\n",
			cksel_core, ckdiv_core, ckdiv_bus, ckdiv_cpu0, ckdiv_cpu1);
	pm_core_bus_clock_ctrl(cksel_core, ckdiv_core, ckdiv_bus, ckdiv_cpu0, ckdiv_cpu1);


	CLI_LOGI("set_cpu_clock_freq end.\r\n");
}


#if CONFIG_COMMON_IO
extern int common_io_test_main(int argc, const char * argv[]);
void test_common_io(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	CLI_LOGI("common io test begin.===================.\r\n");
	common_io_test_main(0, NULL);
	CLI_LOGI("common io test end.====================.\r\n");
}
#endif

#if (CONFIG_ARCH_RISCV && !CONFIG_SLAVE_CORE)
extern void show_pmp_config(void);
static void pmp_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc == 2) {
		if (os_strncmp(argv[1], "test1", 5) == 0) {
			int *test_ptr = NULL;
			*test_ptr = 0x12345678;
			return;
		}
		if (os_strncmp(argv[1], "test2", 5) == 0) {
			int *test_ptr = (int *)0x10000000; //itcm
			*test_ptr = 0x12345678;
			return;
		}
	}

	os_printf("Show pmp config info.\r\n");
	show_pmp_config();
}
#endif

void set_printf_uart_port(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	unsigned char uart_port = 0;

	if (argc != 2) {
		os_printf("set log/shell uart port 0/1/2");
		return;
	}

	uart_port = os_strtoul(argv[1], NULL, 10);
	os_printf("set_printf_uart_port: %d.\r\n", uart_port);

	if (uart_port < UART_ID_MAX) {
#if CONFIG_SHELL_ASYNCLOG
		shell_set_uart_port(uart_port);
#else
		bk_set_printf_port(uart_port);
#endif
	} else {
		os_printf("uart_port must be 0/1/2.\r\n");
	}

	os_printf("uart_port end.\r\n");
}

#if CONFIG_CACHE_ENABLE
static void prvBUS(void) {
	union {
		char a[10];
		int i;
	} u;

	int *p = (int *) &(u.a[1]);
	os_printf("prvBUS() enter(%x).\n", &(u.a[1]));
	os_printf("prvBUS() p(%x).\n", p);
	*p = 17;
	os_printf("prvBUS() left().\n");
}


void cli_cache_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		show_cache_config_info();
	}

	uint32_t mode = os_strtoul(argv[1], NULL, 10);
	os_printf("cache mode(%d).\n", mode);

	if (mode == 0) {
		enable_dcache(0);
	} else if (mode == 1) {
		enable_dcache(1);
	} else {
		prvBUS();
	}

}

#endif

#if CONFIG_ADC_KEY
static void adc_key_next_cb()
{
	os_printf("adc_key_next_cb NEXT\r\n");
}
static void adc_key_prev_cb()
{
	os_printf("adc_key_prev_cb PREV\r\n");
}
static void adc_key_play_pause_cb()
{
	os_printf("adc_key_play_pause_cb PLAY PAUSE\r\n");
}
static void adc_key_menu_cb()
{
	os_printf("adc_key_menu_cb MENU\r\n");
}
static void cli_adc_key_op(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		os_printf("cli_adc_key_op please init/deinit\n");
		return;
	}
	if (os_strcmp(argv[1], "init") == 0) {
		adc_key_init();
		adc_key_register_next_cb(adc_key_next_cb);
		adc_key_register_prev_cb(adc_key_prev_cb);
		adc_key_register_play_pause_cb(adc_key_play_pause_cb);
		adc_key_register_menu_cb(adc_key_menu_cb);
		CLI_LOGI("adc_key init\n");
	} else if(os_strcmp(argv[1], "deinit") == 0) {
		adc_key_deinit();
		CLI_LOGI("adc_key deinit\n");
	} else if(os_strcmp(argv[1], "set_int") == 0) {
		int type = os_strtoul(argv[2], NULL, 10);
		adc_key_set_gpio_int_type(type);
		CLI_LOGI("adc_key SET_GPIO_INT_TYPE\n");
	} else {
		return;
	}
}
#endif


__attribute__ ((__optimize__ ("-fno-tree-loop-distribute-patterns"))) \
int32_t cpu_test(uint32_t count) {
#if CONFIG_SOC_BK7236XX
	#define portNVIC_SYSTICK_CURRENT_VALUE_REG    ( *( ( volatile uint32_t * ) 0xe000e018 ) )

    uint32_t i;
    volatile uint32_t time_begin = 0;
    volatile uint32_t time_end = 0;

    time_begin = portNVIC_SYSTICK_CURRENT_VALUE_REG;
    for(i = 0; i < count; i++)
    {
		__asm volatile("nop \n");
    }

    time_end = portNVIC_SYSTICK_CURRENT_VALUE_REG;
    CLI_LOGI("cpu_test: count[%d], begin[%d], end[%d], duration[%d].\r\n", count, time_begin, time_end, time_end - time_begin);
#endif

    return 0;
}

static void cli_cpu_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t count = 0;
	if (argc < 2) {
		CLI_LOGI("cputest [count]\r\n");
		return;
	}
	count = os_strtoul(argv[1], NULL, 10);
	cpu_test(count);
	CLI_LOGI("cputest end.\r\n");
}

#define MISC_CMD_CNT (sizeof(s_misc_commands) / sizeof(struct cli_command))
static const struct cli_command s_misc_commands[] = {
	{"version", NULL, get_version},
	{"starttype", "show start reason type",cli_show_reset_reason},
	{"id", NULL, get_id},
	{"reboot", "reboot system", reboot},
	{"time",   "system time",   uptime_Command},
#if (CONFIG_WIFI_ENABLE)
	{"mac", "mac <mac>, get/set mac. e.g. mac c89346000001", mac_command},
#endif

#if (!CONFIG_SOC_BK7231)
#if (CONFIG_EFUSE)
	{"efuse",       "efuse [-r addr] [-w addr data]", efuse_cmd_test},
	{"efusemac",    "efusemac [-r] [-w] [mac]",       efuse_mac_cmd_test},
#endif //#if (CONFIG_EFUSE)
#endif
#if (CONFIG_MASTER_CORE) 
	{"bootcore1", "boot slave core,1:start,0:stop,others:start and stop many times", boot_slave_core},
#endif

#if (!CONFIG_SLAVE_CORE)
	{"setjtagmode", "set jtag mode {cpu0|cpu1} {group1|group2}", set_jtag_mode},
#if CONFIG_COMMON_IO
	{"testcommonio", "test common io", test_common_io},
#endif
#if CONFIG_ARCH_RISCV
	{"pmp", "show pmp config info", pmp_command},
#endif
	{"setprintport", "set log/shell uart port 0/1/2", set_printf_uart_port},
#if CONFIG_CACHE_ENABLE
	{"cache", "show cache config info", cli_cache_cmd},
#endif
#if CONFIG_ADC_KEY
	{"adc_key", "adc_key {init|deinit}", cli_adc_key_op},
#endif
	{"setcpufreq", "setcpufreq [ckdiv_core] [ckdiv_bus] [ckdiv_cpu0] [ckdiv_cpu1]", set_cpu_clock_freq},
#endif //#if (!CONFIG_SLAVE_CORE)
	{"cputest", "cputest [count]", cli_cpu_test},
};

int cli_misc_init(void)
{
	return cli_register_commands(s_misc_commands, MISC_CMD_CNT);
}
