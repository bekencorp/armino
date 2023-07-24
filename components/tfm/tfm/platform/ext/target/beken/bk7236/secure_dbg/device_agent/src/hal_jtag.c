#include <stdint.h>
#include "hal_jtag.h"

void hal_jtag_enable(void)
{
	/*DAUTHCTRL register, jtag/swd debug enable*/
    *((volatile uint32_t*)CORTEX_M33_REG_DAUTHCTRL) =
        CORTEX_M33_CPU_DEBUG_ENABLE_VALUE;
}

void hal_jtag_disable(void)
{
	/*DAUTHCTRL register, jtag/swd disable*/
    *((volatile uint32_t*)CORTEX_M33_REG_DAUTHCTRL) =
        CORTEX_M33_CPU_DEBUG_DISABLE_VALUE;
}
// eof

