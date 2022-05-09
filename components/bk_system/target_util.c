#include <common/bk_include.h>
#include "bk_arm_arch.h"

#include "bk_misc.h"
#include "bk_sys_ctrl.h"
#include "bk_fake_clock.h"
#include "bk_drv_model.h"
#include <components/system.h>
#include "sys_driver.h"


/*******************************************************************************
* Function Implemantation
*******************************************************************************/
/*
	MCLK:26MHz, delay(1): about 25us
				delay(10):about 125us
				delay(100):about 850us
 */
void delay(INT32 num)
{
	volatile INT32 i, j;

	for (i = 0; i < num; i ++) {
		for (j = 0; j < 100; j ++)
			;
	}
}



#if (CONFIG_SYSTEM_CTRL)
void delay_cycle(INT32 num)
{
	volatile INT32 i;

	for (i = 0; i < num; i ++) {
			;
	}
}

void delay_ms(UINT32 ms_count)
{
	UINT32 div;
	UINT32 clk = 0;
	UINT32 cell;

	UINT32 mux = sys_drv_mclk_mux_get();
	div = sys_drv_mclk_div_get();
	switch (mux) {
	case 0:  //XTAL
		clk = 26000000 / (1 + div);
		break;

	case 1: //DCO
		clk = 26000000 / (1 + div);
		break;

	case 2: //320M
		clk = 320000000 / (1 + div);
		break;

	case 3: //480M
		clk = 480000000 / (1 + div);
		break;

	default:
		break;
	}

	cell = clk / 18000;
	delay_cycle(ms_count * cell);
}

void delay_10us(UINT32 count)
{
	UINT32 div;
	UINT32 clk = 0;
	UINT32 cell;

	UINT32 mux = sys_drv_mclk_mux_get();
	div = sys_drv_mclk_div_get();
	switch (mux) {
	case 0:  //XTAL
		clk = 26000000 / (1 + div);
		break;

	case 1: //DCO
		clk = 26000000 / (1 + div);
		break;

	case 2: //320M
		clk = 320000000 / (1 + div);
		break;

	case 3: //480M
		clk = 480000000 / (1 + div);
		break;

	default:
		break;
	}

	cell = (clk + 900000) / 1800000;
	delay_cycle(count * cell);
}

#else
/*
	when parameter is 1, the return result is approximately 1 ms;
 */
void delay_ms(UINT32 ms_count)
{
	UINT32 ret;
	UINT32 div;
	UINT32 clk = 0;
	UINT32 cell;
	SYS_CTRL_U param;

	ret = sddev_control(DD_DEV_TYPE_SCTRL, CMD_GET_SCTRL_CONTROL, &param);
	BK_ASSERT(SCTRL_SUCCESS == ret);

	div = param.bits.mclk_div;
	switch (param.bits.mclk_mux) {
	case MCLK_MODE_DCO:
	case MCLK_MODE_26M_XTAL:
		clk = 26000000;
		break;

	case MCLK_MODE_DPLL:
		clk = 480000000 / (2 << div);
		break;

	case MCLK_MODE_LPO:
		clk = 32000;
		break;

	default:
		break;
	}

	BK_ASSERT(clk);

	cell = 100 * clk / 26000000;
	delay(ms_count * cell);
}
#endif


/*
	[delay offset]worst case: delay about 1 second;
 */
void delay_sec(UINT32 ms_count)
{
	UINT32 t0;
	UINT32 t1;

	t0 = fclk_get_second();
	while (1) {
		t1 = fclk_get_second();
		if (t1 - t0 >= 1)
			break;
	}
}

/*
	[delay offset]worst case: delay about 1 tick;
 */
void delay_tick(UINT32 tick_count)
{
	UINT32 t0;
	UINT32 t1;

	t0 = bk_get_tick();
	while (1) {
		t1 = bk_get_tick();
		if (t1 - t0 >= 1)
			break;
	}
}

// EOF

