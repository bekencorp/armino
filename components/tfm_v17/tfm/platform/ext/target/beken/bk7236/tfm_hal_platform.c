/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "cmsis.h"
#include "target_cfg.h"
#include "tfm_hal_platform.h"
#include "tfm_plat_defs.h"
#include "uart_stdout.h"
#include "core_star.h"
#include "tfm_sp_log.h"

extern const struct memory_region_limits memory_regions;

/*
	MCLK:26MHz, delay(1): about 25us
				delay(10):about 125us
				delay(100):about 850us
 */
void delay(uint32_t num)
{
	volatile uint32_t i, j;

	for (i = 0; i < num; i ++) {
		for (j = 0; j < 100; j ++)
			;
	}
}

#ifdef TFM_FIH_PROFILE_ON
fih_int tfm_hal_platform_init(void)
#else
enum tfm_hal_status_t tfm_hal_platform_init(void)
#endif
{
    enum tfm_plat_err_t plat_err = TFM_PLAT_ERR_SYSTEM_ERR;
#ifdef TFM_FIH_PROFILE_ON
    fih_int fih_rc = FIH_FAILURE;
#endif

    plat_err = enable_fault_handlers();
    if (plat_err != TFM_PLAT_ERR_SUCCESS) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }

    plat_err = system_reset_cfg();
    if (plat_err != TFM_PLAT_ERR_SUCCESS) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }

#ifdef TFM_FIH_PROFILE_ON
    FIH_CALL(init_debug, fih_rc);
    if (fih_not_eq(fih_rc, fih_int_encode(TFM_PLAT_ERR_SUCCESS))) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }
#else
    plat_err = init_debug();
    if (plat_err != TFM_PLAT_ERR_SUCCESS) {
        return TFM_HAL_ERROR_GENERIC;
    }
#endif

    __enable_irq();
    stdio_init();

    plat_err = nvic_interrupt_target_state_cfg();
    if (plat_err != TFM_PLAT_ERR_SUCCESS) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }

    plat_err = nvic_interrupt_enable();
    if (plat_err != TFM_PLAT_ERR_SUCCESS) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }

    FIH_RET(fih_int_encode(TFM_HAL_SUCCESS));
}

uint32_t tfm_hal_get_ns_VTOR(void)
{
    return memory_regions.non_secure_code_start;
}

uint32_t tfm_hal_get_ns_MSP(void)
{
    return *((uint32_t *)memory_regions.non_secure_code_start);
}

/*todo for debugging*/
void bfhfnmi_handler(void)
{
	uint32_t *psp_ns_pointer = (uint32_t  *)__TZ_get_PSP_NS();
	printf("\r\n[SPE]SFSR-0xe000ede4:0x%x\r\n", (*(volatile uint32_t *)(0xe000ede4)));
	printf("\r\n[SPE]SFAR-0xe000ede8:0x%x\r\n", (*(volatile uint32_t *)(0xe000ede8)));
	printf("\r\n[SPE]psp_ns_pointer:0x%x\r\n", psp_ns_pointer);

	printf_word_buf_hex(psp_ns_pointer, 128);
	while(1);
}

void tfm_s_2_ns_hook(void)
{
	// TODO wangzhilei
	/*dma secure attributes only are configured at the spe mode*/
	*((volatile uint32_t *)(0x45020000 + 2 * 4)) = 0; /*soft reset dma0 module*/
	*((volatile uint32_t *)(0x45020000 + 5 * 4)) = 0xFFF;
	*((volatile uint32_t *)(0x45020000 + 4 * 4)) = 0;

	*((volatile uint32_t *)(0x45030000 + 2 * 4)) = 0; /*soft reset dma1 module*/
	*((volatile uint32_t *)(0x45030000 + 5 * 4)) = 0xFFF;
	*((volatile uint32_t *)(0x45030000 + 4 * 4)) = 0;

	*((volatile uint32_t *)(0x41040000 + 2 * 4)) = 1; /*soft reset ppro module*/
	
	*((volatile uint32_t *)(0x41040000 + 4 * 4)) = 0; /* privilege setting*/
	*((volatile uint32_t *)(0x41040000 + 5 * 4)) = 0xFFFFFFFF;
	*((volatile uint32_t *)(0x41040000 + 8 * 4)) = 0xFFFFFFFB; /*[2]uart0 for spe*/
	*((volatile uint32_t *)(0x41040000 + 10 * 4)) = (0xF);     /*[3:0], bit3:wdt*/
	*((volatile uint32_t *)(0x41040000 + 11 * 4)) = 0xFFFFC3FF;/*gpio_nonsec0  gpio10/11:uart0*/
	*((volatile uint32_t *)(0x41040000 + 12 * 4)) = 0xFFFFFFFF;/*gpio_nonsec1*/

	*((volatile uint32_t *)(0x41040000 + 13 * 4)) = 0x01      ;/*jp enc*/
	*((volatile uint32_t *)(0x41040000 + 14 * 4)) = 0x01      ;/*jp dec*/
	*((volatile uint32_t *)(0x41040000 + 15 * 4)) = 0x01      ;/*dma d_m*/
	*((volatile uint32_t *)(0x41040000 + 16 * 4)) = 0x01      ;/*mac_m*/
	*((volatile uint32_t *)(0x41040000 + 17 * 4)) = 0x01      ;/*hsu_m*/
	*((volatile uint32_t *)(0x41040000 + 18 * 4)) = 0x01      ;/*btdm_m*/
	*((volatile uint32_t *)(0x41040000 + 19 * 4)) = 0x01      ;/*la_m*/
	*((volatile uint32_t *)(0x41040000 + 20 * 4)) = 0x01      ;/*disp_m*/
	*((volatile uint32_t *)(0x41040000 + 21 * 4)) = 0x00      ;/*enc shanghai*/

	SCB->AIRCR  = (uint32_t)((0x5FAUL << SCB_AIRCR_VECTKEY_Pos)
						|SCB_AIRCR_BFHFNMINS_Msk );

    /* Permit Non-secure access to the Floating-point Extension.
     * Note: It is still necessary to set CPACR_NS to enable the FP Extension
     * in the NSPE. This configuration is left to NS privileged software.
     */
    SCB->NSACR |= SCB_NSACR_CP10_Msk | SCB_NSACR_CP11_Msk;

	printf("SCB->AIRCR:0x%x\r\n", SCB->AIRCR);
}

uint32_t tfm_hal_get_ns_entry_point(void)
{
	tfm_s_2_ns_hook();

    return *((uint32_t *)(memory_regions.non_secure_code_start + 4));
}
