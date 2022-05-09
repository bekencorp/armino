#include "rw_platf_pub.h"
#include <common/sys_config.h>
#include "bk_mcu_ps.h"
#include "bk_wifi_rw.h"
#include <soc/soc.h>

extern void rwnxl_violence_reset_patch(void);
UINT32 mcu_ps_machw_reset(void);

void rwxl_reset_patch(void)
{
	rwnxl_violence_reset_patch();
}

void hal_machw_init_diagnostic_ports(void)
{
	// Initialize diagnostic ports
#if (CONFIG_SOC_BK7231) || (CONFIG_SOC_BK7236)
	bk_wifi_debug_port_sel_pack(0x1C, 0x25);
#else
	bk_wifi_debug_port_sel_pack(0x01, 0x07);

	//bypass buf fix
	REG_WRITE(bk_wifi_get_mac_base_address() + 0x00000700, 0x08);
#endif // (CONFIG_SOC_BK7231)
}

void hal_machw_before_reset_patch(void)
{
#if CONFIG_MCU_PS
	mcu_ps_machw_cal();
	mcu_ps_machw_reset();
#endif
}

void hal_machw_after_reset_patch(void)
{
#if CONFIG_MCU_PS
	mcu_ps_machw_init();
#endif
}

