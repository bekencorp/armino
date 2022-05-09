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

#if (CONFIG_EFUSE)
static void efuse_cmd_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
static void efuse_mac_cmd_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
#endif //#if (CONFIG_EFUSE)

extern volatile const uint8_t build_version[];

void get_version(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	os_printf("get_version\r\n");
//	os_printf("firmware version : %s", BEKEN_SDK_REV);
    os_printf("firmware version : %s", build_version);
}

void get_id(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	os_printf("get_id\r\n");
	// os_printf("id : %x_%x",sddev_control(DD_DEV_TYPE_SCTRL,CMD_GET_DEVICE_ID, NULL), sddev_control(DD_DEV_TYPE_SCTRL,CMD_GET_CHIP_ID, NULL));
	os_printf("id : %x_%x",sys_drv_get_device_id(), sys_drv_get_chip_id());
}

static void uptime_Command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	os_printf("UP time %ldms\r\n", rtos_get_time());
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
			hexstr2bin(argv[2], &addr, 1);
			bk_efuse_read_byte(addr, &data);
			os_printf("efuse read: addr-0x%02x, data-0x%02x\r\n",
					  addr, data);
		}
	} else if (argc == 4) {
		if (os_strncmp(argv[1], "-w", 2) == 0)  {
			hexstr2bin(argv[2], &addr, 1);
			hexstr2bin(argv[3], &data, 6);
			os_printf("efuse write: addr-0x%02x, data-0x%02x, ret:%d\r\n",
					  addr, data, bk_efuse_write_byte(addr, data));
		}
	} else
		os_printf("efuse [-r addr] [-w addr data]\r\n");
}

static void efuse_mac_cmd_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
#if CONFIG_BASE_MAC_FROM_EFUSE
	uint8_t mac[6];

	if (argc == 1) {
		if (bk_get_mac(mac, MAC_TYPE_BASE) == BK_OK)
			os_printf("MAC address: %02x-%02x-%02x-%02x-%02x-%02x\r\n",
					  mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	} else if (argc == 2) {
		if (os_strncmp(argv[1], "-r", 2) == 0) {
			if (bk_get_mac(mac, MAC_TYPE_BASE) == BK_OK)
				os_printf("MAC address: %02x-%02x-%02x-%02x-%02x-%02x\r\n",
						  mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
		}
	} else if (argc == 3) {
		if (os_strncmp(argv[1], "-w", 2) == 0)  {
			hexstr2bin(argv[2], mac, 6);
			os_printf("Set MAC address: %02x-%02x-%02x-%02x-%02x-%02x\r\n",
					  mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
		}
	} else
		os_printf("efusemac [-r] [-w] [mac]\r\n");
#else
	os_printf("base mac is not from efuse\n");
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
		os_printf("base mac: "BK_MAC_FORMAT"\n", BK_MAC_STR(base_mac));
		os_printf("sta mac: "BK_MAC_FORMAT"\n", BK_MAC_STR(sta_mac));
		os_printf("ap mac: "BK_MAC_FORMAT"\n", BK_MAC_STR(ap_mac));
	} else if (argc == 2) {
		hexstr2bin(argv[1], base_mac, BK_MAC_ADDR_LEN);
		bk_set_base_mac(base_mac);
		os_printf("set base mac: "BK_MAC_FORMAT"\n", BK_MAC_STR(base_mac));
	} else
		os_printf("invalid cmd\r\n");

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
		start_slave_core();
	else if(reset_value == 0)
		stop_slave_core();
	else	//test start/stop many times.
	{
		uint32_t i = 0;

		//start first:odd will be at start status, even will be at stop status
		while(1)
		{
			start_slave_core();
			i++;
			if(i == reset_value)
				break;
			stop_slave_core();
			i++;
			if(i == reset_value)
				break;
		}
		os_printf("boot on/off %d times.\r\n", reset_value);
	}

#endif
	os_printf("boot_slave_core end.\r\n");
}
#endif

void get_jtag_mode(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv) 
{
	uint32 jtag_mode = sys_drv_get_jtag_mode();

	os_printf("get_jtag_mode: %d.\r\n", jtag_mode);
}

void set_jtag_mode(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv) 
{
	uint32 jtag_mode = sys_drv_get_jtag_mode();

	if (argc != 2) {
		os_printf("set_jtag_mode parameter invalid\r\n");
		return;
	}

	jtag_mode = os_strtoul(argv[1], NULL, 10);
	os_printf("set_jtag_mode: %d.\r\n", jtag_mode);
	
	if (0 == jtag_mode || 1 == jtag_mode) {
		(void)sys_drv_set_jtag_mode(jtag_mode);
	} else {
		os_printf("jtag_mode must be 0 or 1.\r\n");
	}

	os_printf("set_jtag_mode end.\r\n");
}

#if CONFIG_COMMON_IO
extern int common_io_test_main(int argc, const char * argv[]);
void test_common_io(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	os_printf("common io test begin.===================.\r\n");
	common_io_test_main(0, NULL);
	os_printf("common io test end.====================.\r\n");
}
#endif

#define MISC_CMD_CNT (sizeof(s_misc_commands) / sizeof(struct cli_command))
static const struct cli_command s_misc_commands[] = {
	{"version", NULL, get_version},
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
	{"bootcore1", "boot slave core,0:start,1:stop,others:start and stop many times", boot_slave_core},
#endif
	{"jtagmode", "get jtag mode", get_jtag_mode},
	{"setjtagmode", "reboot system", set_jtag_mode},
#if CONFIG_COMMON_IO
	{"testcommonio", "test common io", test_common_io},
#endif
};

int cli_misc_init(void)
{
	return cli_register_commands(s_misc_commands, MISC_CMD_CNT);
}
