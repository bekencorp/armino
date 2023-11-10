// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <common/bk_include.h>
#include "dd_pub.h"
#include "bk_drv_model.h"
#include "bk_sys_ctrl.h"
#include "sys_driver.h"
#include <driver/int.h>
#include <driver/pwm.h>
#include <driver/timer.h>
#include <driver/gpio.h>
#include <driver/dma.h>
#include <driver/uart.h>
#include <driver/wdt.h>
#include <driver/aon_wdt.h>
#include <driver/trng.h>
#include <driver/efuse.h>
#include <os/mem.h>
#include <driver/adc.h>
#include <driver/spi.h>
#include <driver/i2c.h>
#include <driver/aon_rtc.h>
#include <modules/pm.h>
#include <driver/mpc.h>
#include <driver/prro.h>
#include "bk_driver.h"
#include "interrupt_base.h"

#if CONFIG_SECURITY
#include "bk_security.h"
#endif

#if CONFIG_AON_PMU
#include "aon_pmu_driver.h"
#endif

#if CONFIG_FLASH
#include <driver/flash.h>
#endif

#if CONFIG_EASY_FLASH
#include "bk_ef.h"
#endif

#if ((CONFIG_SDIO_HOST) || (CONFIG_SDCARD))
#include "driver/sdio_host.h"
#endif

#if CONFIG_SDCARD
#include "sdcard.h"
#endif

#if (CONFIG_SDIO_V2P0 && CONFIG_SDIO_SLAVE)
#include "sdio_slave_driver.h"
#if CONFIG_SDIO_TEST_EN
#include "sdio_test.h"
#endif
#endif

#if CONFIG_QSPI
#include <driver/qspi.h>
#endif

#if CONFIG_JPEGENC_HW
#include <driver/jpeg_enc.h>
#endif

#if CONFIG_CALENDAR
#include <driver/calendar.h>
#endif

#if CONFIG_ATE
#include <components/ate.h>
#endif

#if CONFIG_USB
#include "bk_fusb.h"
#include "bk_usb.h"
#endif

#if CONFIG_TOUCH_PM_SUPPORT
#include <driver/touch.h>
#endif

#if CONFIG_CHIP_SUPPORT
#include "modules/chip_support.h"
#endif

#if CONFIG_YUV_BUF
#include <driver/yuv_buf.h>
#endif

#if CONFIG_SDMADC
#include <driver/sdmadc.h>
#endif

#if CONFIG_HW_ROTATE_PFC
#include <driver/rott_driver.h>
#endif

//TODO only init driver model and necessary drivers
#if CONFIG_POWER_CLOCK_RF
#define   MODULES_POWER_OFF_ENABLE (1)
#define   ROSC_DEBUG_EN            (0)
#define   MODULES_CLK_ENABLE       (0)
extern void clock_dco_cali(UINT32 speed);
void power_clk_rf_init()
{
    uint32_t param =0;
	/*power on all the modules for bringup test*/

	module_name_t use_module = MODULE_NAME_WIFI;
    /*1. power on all the modules*/
	#if MODULES_POWER_OFF_ENABLE
	    sys_drv_module_power_ctrl(POWER_MODULE_NAME_ENCP,POWER_MODULE_STATE_OFF);
		//sys_drv_module_power_ctrl(POWER_MODULE_NAME_BAKP,POWER_MODULE_STATE_OFF);
		sys_drv_module_power_ctrl(POWER_MODULE_NAME_AUDP,POWER_MODULE_STATE_OFF);
		sys_drv_module_power_ctrl(POWER_MODULE_NAME_VIDP,POWER_MODULE_STATE_OFF);
		sys_drv_module_power_ctrl(POWER_MODULE_NAME_BTSP,POWER_MODULE_STATE_OFF);
		sys_drv_module_power_ctrl(POWER_MODULE_NAME_WIFIP_MAC,POWER_MODULE_STATE_OFF);
		sys_drv_module_power_ctrl(POWER_MODULE_NAME_WIFI_PHY,POWER_MODULE_STATE_OFF);
		sys_drv_module_power_ctrl(POWER_MODULE_NAME_CPU1,POWER_MODULE_STATE_OFF);
	#else
	    power_module_name_t module = POWER_MODULE_NAME_MEM1;
        for(module = POWER_MODULE_NAME_MEM1 ; module < POWER_MODULE_NAME_NONE ; module++)
        {
            sys_drv_module_power_ctrl(module,POWER_MODULE_STATE_ON);
        }
    #endif
    /*2. enable the analog clock*/
    sys_drv_module_RF_power_ctrl(use_module ,POWER_MODULE_STATE_ON);

	/*3.enable all the modules clock*/
	#if MODULES_CLK_ENABLE
	dev_clk_pwr_id_t devid = 0;
	for(devid = 0; devid < 32; devid++)
	{
	    sys_drv_dev_clk_pwr_up(devid, CLK_PWR_CTRL_PWR_UP);
    }
	#endif
    /*4.set the cpu0 and matrix clock*/
   /*cpu0:26m ,matrix:26m*/
   //sys_drv_core_bus_clock_ctrl(HIGH_FREQUECY_CLOCK_MODULE_CPU0, 1,0, HIGH_FREQUECY_CLOCK_MODULE_CPU0_MATRIX,0,0);
   /*cpu0:120m ,matrix:120m*/
   //sys_drv_core_bus_clock_ctrl(HIGH_FREQUECY_CLOCK_MODULE_CPU0, 3,0, HIGH_FREQUECY_CLOCK_MODULE_CPU0_MATRIX,0,0);
   /*cpu0:240m ,matrix:120m*/
	//sys_drv_core_bus_clock_ctrl(HIGH_FREQUECY_CLOCK_MODULE_CPU0, 0,0, HIGH_FREQUECY_CLOCK_MODULE_CPU0_MATRIX,0,0);
	#if !CONFIG_SLAVE_CORE
	//bk_pm_module_vote_cpu_freq(PM_DEV_ID_DEFAULT,PM_CPU_FRQ_120M);
	#if CONFIG_ATE_TEST
		//bk_pm_module_vote_cpu_freq(PM_DEV_ID_DEFAULT,PM_CPU_FRQ_320M);//improve the cpu frequency for save boot time at ate test
	#endif
	#endif
   /*5.config the analog*/
   //sys_drv_analog_set(ANALOG_REG0, param);
   //sys_drv_analog_set(ANALOG_REG0, param);
   //sys_drv_analog_set(ANALOG_REG0, param);

	//config apll
	//param = 0;
	//param = sys_drv_analog_get(ANALOG_REG4);
	//param &= ~(0x1f << 5);
	//param |= (0x14 << 5);
	//sys_drv_analog_set(ANALOG_REG4, param);

	/*set low power low voltage value */
#if 0
	param = 0;
	param = sys_drv_analog_get(ANALOG_REG3);
	param &= ~(0x7 << 29);
	param |= (0x4 << 29);
	sys_drv_analog_set(ANALOG_REG3, param);

	param = sys_drv_analog_get(ANALOG_REG2);
	param |= (0x1 << 25);
	sys_drv_analog_set(ANALOG_REG2, param);
#endif
	/*tempreture det enable for VIO*/
	param = 0;
	param = sys_drv_analog_get(ANALOG_REG6);
	param |= (0x1 << SYS_ANA_REG6_EN_TEMPDET_POS)|(0x7 << SYS_ANA_REG6_RXTAL_LP_POS)|(0x7 << SYS_ANA_REG6_RXTAL_HP_POS);
	param &= ~(0x1 << SYS_ANA_REG6_EN_SLEEP_POS);
	sys_drv_analog_set(ANALOG_REG6, param);

#if 0
	param = 0;
	param = aon_pmu_drv_reg_get(PMU_REG3);
	param = 0x1 << 0; //security boot  bypass
	aon_pmu_drv_reg_set(PMU_REG3,param);
#endif
	/*let rosc to bt/wifi ip*/
	param = 0;
	param = aon_pmu_drv_reg_get(PMU_REG0x41);
	param |= 0x1 << 24;
	aon_pmu_drv_reg_set(PMU_REG0x41,param);
	/*wake delay of Xtal*/
#if 0
	param = 0;
	param = aon_pmu_drv_reg_get(PMU_REG0x40);
	param |= 0xF << 0;
	aon_pmu_drv_reg_set(PMU_REG0x40,param);
#endif
	/* rosc calibration start*/
	/*a.open rosc debug*/
#if ROSC_DEBUG_EN
	param = 0;
	param = sys_drv_analog_get(ANALOG_REG5);
	param |= 0x1 << SYS_ANA_REG5_CK_TST_ENBALE_POS;
	sys_drv_analog_set(ANALOG_REG5,param);

	param = 0;
	param = sys_drv_analog_get(ANALOG_REG11);
	param |= 0x1 << SYS_ANA_REG11_TEST_EN_POS;
	sys_drv_analog_set(ANALOG_REG11,param);

	param = 0;
	param = sys_drv_analog_get(ANALOG_REG4);
	param |= (0x1 << SYS_ANA_REG4_ROSC_TSTEN_POS);//Rosc test enable
	sys_drv_analog_set(ANALOG_REG4,param);

#endif
	/*b.config calibration*/
	param = 0;
	param = sys_drv_analog_get(ANALOG_REG4);
	param &= ~(SYS_ANA_REG4_ROSC_CAL_INTVAL_MASK << SYS_ANA_REG4_ROSC_CAL_INTVAL_POS);//clear the data
	param |= 0x4 << SYS_ANA_REG4_ROSC_CAL_INTVAL_POS;//Rosc Calibration Interlval 0.25s~2s (4:1s)
	param |= 0x1 << SYS_ANA_REG4_ROSC_CAL_MODE_POS;//0x1: 32K ;0x0: 31.25K
	param |= 0x1 << SYS_ANA_REG4_ROSC_CAL_EN_POS;//Rosc Calibration Enable
	param &= ~(0x1 << SYS_ANA_REG4_ROSC_MANU_EN_POS);//0:close Rosc Calibration Manual Mode
	sys_drv_analog_set(ANALOG_REG4,param);

	/*c.trigger calibration*/
	param = 0;
	param = sys_drv_analog_get(ANALOG_REG4);
	param &= ~(0x1 << SYS_ANA_REG4_ROSC_CAL_TRIG_POS);//trigger clear
	sys_drv_analog_set(ANALOG_REG4,param);

	param = 0;
	param = sys_drv_analog_get(ANALOG_REG4);
	param |= (0x1 << SYS_ANA_REG4_ROSC_CAL_TRIG_POS);//trigger enable
	sys_drv_analog_set(ANALOG_REG4,param);
	/* rosc calibration end*/
	/*7.dpll calibration */
	//sys_drv_cali_dpll(0);

	/*8.dco calibration*/
	//clock_dco_cali(0x4);
}
#endif

int driver_init(void)
{
	sys_drv_init();

#if CONFIG_SPE
#if CONFIG_MPC
	bk_mpc_driver_init();
#endif

#if CONFIG_PRRO
	bk_prro_driver_init();
#endif
#endif

#if CONFIG_AON_PMU
	aon_pmu_drv_init();
#endif

#if CONFIG_POWER_CLOCK_RF
	power_clk_rf_init();
#endif

	interrupt_init();

	bk_gpio_driver_init();

	//Important notice!!!!!
	//ATE uses UART TX PIN as the detect ATE mode pin,
	//so it should be called after GPIO init and before UART init.
	//or caused ATE can't work or UART can't work
#if CONFIG_ATE
	bk_ate_init();
#endif

	//Important notice!
	//Before UART is initialized, any call of BK_LOG_RAW/os_print/BK_LOGx may
	//cause problems, such as crash etc!
	bk_uart_driver_init();

#if CONFIG_CHIP_SUPPORT
	if(!bk_is_chip_supported()) {
		return BK_FAIL;
	}
#endif

	os_show_memory_config_info(); //TODO - remove it after bk_early_printf() is supported.
	drv_model_init();

	g_dd_init();

#if CONFIG_FLASH
	bk_flash_driver_init();
#if CONFIG_FLASH_ORIGIN_API
	extern int hal_flash_init();
	hal_flash_init();
#endif
#endif

#if CONFIG_EASY_FLASH
	easyflash_init();
#endif

#if CONFIG_SECURITY
	bk_secrity_init();
#endif

#if CONFIG_PWM
	bk_pwm_driver_init();
#endif

#if CONFIG_TIMER
	bk_timer_driver_init();
#endif

#if CONFIG_GENERAL_DMA
	bk_dma_driver_init();
#endif

	bk_wdt_driver_init();

#if CONFIG_AON_WDT && !CONFIG_INT_AON_WDT
	bk_aon_wdt_stop();
#endif

#if CONFIG_TRNG_SUPPORT
	bk_trng_driver_init();
#endif

#if CONFIG_EFUSE
	bk_efuse_driver_init();
#endif

#if CONFIG_SARADC
	bk_adc_driver_init();
#endif

	bk_spi_driver_init();
#if CONFIG_I2C
	bk_i2c_driver_init();
#endif

#if CONFIG_QSPI
	bk_qspi_driver_init();
#endif

#if CONFIG_YUV_BUF
	bk_yuv_buf_driver_init();
#endif

#if !CONFIG_SYSTEM_CTRL
#if CONFIG_JPEGENC_HW
	bk_jpeg_enc_driver_init();
#endif
#endif

#if CONFIG_AON_RTC
	bk_aon_rtc_driver_init();
#endif

#if ((CONFIG_SDIO_HOST) || (CONFIG_SDCARD))
	bk_sdio_host_driver_init();
#endif

#if (CONFIG_SDIO_V2P0 && CONFIG_SDIO_SLAVE)
	bk_sdio_slave_driver_init();
#if CONFIG_SDIO_TEST_EN
	bk_sdio_test_init();
#endif
#endif

#if CONFIG_CALENDAR
	bk_calendar_driver_init();
#endif

//call it after LOG is valid.
#if CONFIG_ATE
	os_printf("ate enabled is %d\r\n", ate_is_enabled());
#endif

#if CONFIG_USB
	bk_usb_init();
#endif

#if CONFIG_TOUCH_PM_SUPPORT
	bk_touch_pm_init();
#endif

#if CONFIG_SDMADC
	//bk_sdmadc_driver_init();
#endif

#if CONFIG_HW_ROTATE_PFC
	bk_rott_driver_init();
#endif

	os_printf("driver_init end\r\n");

	return 0;
}

