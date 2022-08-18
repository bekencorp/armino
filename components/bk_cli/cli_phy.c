#include "cli.h"
#include "bk_phy.h"

#if CONFIG_POWER_TABLE
extern void pwr_tbl_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
#endif

#if (CLI_CFG_PHY == 1)

static void phy_cca_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc != 2) {
		os_printf("cca open\r\n");
		os_printf("cca close\r\n");
		os_printf("cca show\r\n");
		return;
	}

	if (os_strncmp(argv[1], "open", 4) == 0) {
		phy_open_cca();
		os_printf("cca opened\r\n");
	} else if (os_strncmp(argv[1], "close", 4) == 0) {
		phy_close_cca();
		os_printf("cca closed\r\n");
	} else if (os_strncmp(argv[1], "show", 4) == 0)
		phy_show_cca();
	else {
		os_printf("cca open\r\n");
		os_printf("cca close\r\n");
		os_printf("cca show\r\n");
	}
}

#if (CONFIG_SOC_BK7256XX)
extern void cmd_saradc_auto_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void cmd_sigtest(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

#endif
#define PHY_CMD_CNT (sizeof(s_phy_commands) / sizeof(struct cli_command))
static const struct cli_command s_phy_commands[] = {
	{"cca", "cca open\\close\\show", phy_cca_test},

	{"txevm", "txevm [-m] [-c] [-l] [-r] [-w]", tx_evm_cmd_test},
	{"rxsens", "rxsens [-m] [-d] [-c] [-l]", rx_sens_cmd_test},


#if ((!CONFIG_SOC_BK7231) && (!CONFIG_SOC_BK7271))
	{"rfcali_cfg_mode",      "1:manual, 0:auto",      cmd_rfcali_cfg_mode},
	{"rfcali_cfg_tssi_g",    "0-255",                 cmd_rfcali_cfg_tssi_g},
	{"rfcali_cfg_tssi_b",    "0-255",                 cmd_rfcali_cfg_tssi_b},
	{"rfcali_show_data",     "",                      cmd_rfcali_show_data},
	{"rfcali_cfg_rate_dist", "b g n40 ble (0-31)",    cmd_rfcali_cfg_rate_dist},
#if (CONFIG_SOC_BK7256XX)
	{"cali", "cali auto_test",    cmd_cali},
#if CONFIG_PSRAM
	{"psram", "psram enable clk_div",    cmd_psram},
#endif
	{"saradc", "start close", cmd_saradc_auto_test},
	{"sigtest", "sigtest enable format_mod mcs_index", cmd_sigtest}, 
	
	
#endif
#endif
#if CONFIG_POWER_TABLE
	{"pwrtbl", "pwrtbl cal/set/get <value>", pwr_tbl_command},
#endif
#if ((CONFIG_SOC_BK7256XX) || (CONFIG_SOC_BK7236A))
    {"la", "bk7256:la rf_adc/fe_adc/rf_dac/fe_dac;bk7236a:la rx_adc/rx_dac/tx_dac", cmd_la_sample_test},
#endif
};

int cli_phy_init(void)
{
	return cli_register_commands(s_phy_commands, PHY_CMD_CNT);
}

#endif //#if (CLI_CFG_PHY == 1)
