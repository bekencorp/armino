#include "bk_cli.h"
#include "wlan_defs_pub.h"
#include "bk_private/bk_wifi_wrapper.h"
#if CONFIG_LWIP
#include "lwip/ping.h"
#endif
#include <components/netif.h>
#include "cli.h"

extern void make_tcp_server_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

#define CLI_DUMP_IP(_prompt, _ifx, _cfg) do {\
	CLI_LOGI("%s netif(%s) ip4=%s mask=%s gate=%s dns=%s\n", (_prompt),\
			(_ifx) == NETIF_IF_STA ? "sta" : "ap",\
			(_cfg)->ip, (_cfg)->mask, (_cfg)->gateway, (_cfg)->dns);\
} while(0)

#if (CLI_CFG_NETIF == 1)

static void ip_cmd_show_ip(int ifx)
{
	netif_ip4_config_t config;

	if (ifx == NETIF_IF_STA || ifx == NETIF_IF_AP) {
		BK_LOG_ON_ERR(bk_netif_get_ip4_config(ifx, &config));
		CLI_DUMP_IP(" ", ifx, &config);
	} else {
		BK_LOG_ON_ERR(bk_netif_get_ip4_config(NETIF_IF_STA, &config));
		CLI_DUMP_IP(" ", NETIF_IF_STA, &config);
		BK_LOG_ON_ERR(bk_netif_get_ip4_config(NETIF_IF_AP, &config));
		CLI_DUMP_IP(" ", NETIF_IF_AP, &config);
	}
}

void cli_ip_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	netif_ip4_config_t config = {0};
	int ifx = NETIF_IF_COUNT;

	if (argc > 1) {
		if (os_strcmp("sta", argv[1]) == 0) {
			ifx = NETIF_IF_STA;
		} else if (os_strcmp("ap", argv[1]) == 0) {
			ifx = NETIF_IF_AP;
		} else {
			CLI_LOGE("invalid netif name\n");
			return;
		}
	}

	if (argc == 1) {
		ip_cmd_show_ip(NETIF_IF_COUNT);
	} else if (argc == 2) {
		ip_cmd_show_ip(ifx);
	} else if (argc == 6) {
		os_strncpy(config.ip, argv[2], NETIF_IP4_STR_LEN);
		os_strncpy(config.mask, argv[3], NETIF_IP4_STR_LEN);
		os_strncpy(config.gateway, argv[4], NETIF_IP4_STR_LEN);
		os_strncpy(config.dns, argv[5], NETIF_IP4_STR_LEN);
		BK_LOG_ON_ERR(bk_netif_set_ip4_config(ifx, &config));
		CLI_DUMP_IP("set static ip, ", ifx, &config);
	} else {
	        CLI_LOGE("usage: ip [sta|ap][{ip}{mask}{gate}{dns}]\n");
	}
}

void cli_dhcpc_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	BK_LOG_ON_ERR(bk_netif_dhcpc_start(NETIF_IF_STA));
	CLI_LOGI("STA start dhcp client\n");
}

void arp_Command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	os_printf("arp_Command\r\n");
}

void cli_ping_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
#if CONFIG_LWIP
	uint32_t cnt = 4;
	if (argc == 1) {
		os_printf("Please input: ping <host address>\n");
		return;
	}

	if (argc > 2)
		cnt = os_strtoul(argv[2], NULL, 10);

	os_printf("ping IP address:%s\n", argv[1]);
	ping(argv[1], cnt, 0);
#endif
}

#if CONFIG_ALI_MQTT
extern void test_mqtt_start(void);
void cli_ali_mqtt_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	os_printf("start test mqtt...\n");
	test_mqtt_start();
}
#endif
#define NETIF_CMD_CNT (sizeof(s_netif_commands) / sizeof(struct cli_command))
static const struct cli_command s_netif_commands[] = {
	{"ip", "ip [sta|ap][{ip}{mask}{gate}{dns}]", cli_ip_cmd},
	{"dhcpc", "dhcpc", cli_dhcpc_cmd},
	{"ping", "ping <ip>", cli_ping_cmd},
#ifdef CONFIG_IPV6
	{"ping6",       "ping6 xxx",             cli_ping_cmd},
#endif
#ifdef TCP_CLIENT_DEMO
	{"tcp_cont", "tcp_cont [ip] [port]", tcp_make_connect_server_command},
#endif

#if CONFIG_TCP_SERVER_TEST
	{"tcp_server", "tcp_server [ip] [port]", make_tcp_server_command },
#endif
#if CONFIG_ALI_MQTT
	{"mqttali", "paho mqtt test", cli_ali_mqtt_cmd},
#endif
};

int cli_netif_init(void)
{
	return cli_register_commands(s_netif_commands, NETIF_CMD_CNT);
}

#endif //#if (CLI_CFG_NETIF == 1)
