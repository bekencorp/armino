

#include "sys_hal.h"
#include "sys_driver.h"

#if CONFIG_DUAL_CORE
#include "amp_lock_api.h"
#define SYSTEM_REG_LOCK_WAIT_TIME_MS  10
#endif

#define SYS_DRV_DELAY_TIME_10US	              120
#define SYS_DRV_DELAY_TIME_200US	          3400

/**  Platform Start **/
//Platform

/** Platform Misc Start **/

static uint32_t sys_amp_res_acquire()
{
#if CONFIG_DUAL_CORE
	uint32_t ret_val = SYS_DRV_FAILURE;
	ret_val = amp_res_acquire(AMP_RES_ID_SYS_REG, SYSTEM_REG_LOCK_WAIT_TIME_MS);
	return ret_val;
#endif
	return SYS_DRV_SUCCESS;
}
static uint32_t sys_amp_res_release()
{
#if CONFIG_DUAL_CORE
	uint32_t ret_val = SYS_DRV_FAILURE;
	ret_val = amp_res_release(AMP_RES_ID_SYS_REG);
	return ret_val;
#endif
	return SYS_DRV_SUCCESS;
}

static uint32_t sys_amp_res_init()
{
#if CONFIG_DUAL_CORE
	uint32_t ret_val = SYS_DRV_FAILURE;
	ret_val = amp_res_init(AMP_RES_ID_SYS_REG);
	return ret_val;
#endif
	return SYS_DRV_SUCCESS;
}

void sys_drv_init()
{
	uint32_t int_level = rtos_disable_int();

	sys_amp_res_init();
	sys_hal_init();

	rtos_enable_int(int_level);
}

/** Platform Misc End **/


#if CONFIG_USB
//函数命名：主谓宾
//sys_drv_主语(模块名)_谓语(动作:set/get/enable等)_宾语(status/value)
//sys_ctrl中的CMD:CMD_SCTRL_USB_POWERDOWN
void sys_drv_usb_power_ctrl(bool power_up)
{
/*
	定义全局宏：SYS_DRIVER_SUPPORT
	隔离BK7256，BK7231N使用新接口，以及暂时没有替换为新接口的项目
*/
	//sys_hal_usb_power_ctrl(power_up);
}

void sys_drv_usb_clock_ctrl(bool ctrl, void *arg)
{
	uint32_t int_level = rtos_disable_int();
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	sys_hal_usb_enable_clk(ctrl);

	if(!ret)
		ret = sys_amp_res_release();

	rtos_enable_int(int_level);
}

uint32_t sys_drv_usb_analog_phy_en(bool ctrl, void *arg)
{
	uint32_t int_level = rtos_disable_int();
	uint32_t ret = SYS_DRV_FAILURE;

	ret = sys_amp_res_acquire();
	if(ret != BK_OK)
		return ret;

	sys_hal_usb_analog_phy_en(ctrl);

	if(!ret)
		ret = sys_amp_res_release();
	if(ret != BK_OK)
		return ret;

	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_usb_analog_speed_en(bool ctrl, void *arg)
{
	uint32_t int_level = rtos_disable_int();
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();
	if(ret != BK_OK)
		return ret;

	sys_hal_usb_analog_speed_en(ctrl);

	if(!ret)
		ret = sys_amp_res_release();
	if(ret != BK_OK)
		return ret;

	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_usb_analog_ckmcu_en(bool ctrl, void *arg)
{
	uint32_t int_level = rtos_disable_int();
	uint32_t ret = SYS_DRV_FAILURE;

	ret = sys_amp_res_acquire();
	if(ret != BK_OK)
		return ret;

	sys_hal_usb_analog_ckmcu_en(ctrl);

	if(!ret)
		ret = sys_amp_res_release();
	if(ret != BK_OK)
		return ret;

	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

void sys_drv_usb_charge_start()
{
/*
	bk7271: usb charge:
	SCTRL_BASE:0x00800000 
	Reg26 ana_reg4 bit[21] aud_pll_vsel
	Reg26 ana_reg4 bit[19]	hw :0  sw:1
	Reg25 ana_reg3 bit[27:28]  11
	Reg26 ana_reg4 bit[13:14] 11  dpll
	Reg25 ana_reg3 bit[18:16]  111

	please check bk7271 VS bk7256
*/
}

void sys_drv_usb_charge_stop(bool ctrl)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_usb_enable_charge(ctrl);

	rtos_enable_int(int_level);
}

void sys_drv_usb_charge_ctrl(bool ctrl, void *arg)
{
	uint32_t int_level = rtos_disable_int();

	if(ctrl)
		sys_drv_usb_charge_start();
	else
		sys_drv_usb_charge_stop(ctrl);

	rtos_enable_int(int_level);
}

/*
 * bk7271: usb charge calibration:
 * SCTRL_BASE:0x00800000 
 * nternal hardware calibration
 * lcf calibration
 * Added 5V voltage on Vusb
 * Added 4.2V voltage on vbatterySet pwd=0
 * 
 * Icp calibration
 * Added parallel 60ohm resistor and 100nF capacitor from vbattery to ground.(Removed the external 4.2V)
 * Set pwd=0
 * Porb=0

 * vcv calibration
 * Added 5V voltage on Vusb
 * Added 4.2V voltage on vbattery
 * Set pwd=0
 * vPorb=0
*/
void sys_drv_usb_charge_cal(sys_drv_charge_step_t step, void *arg)
{
	uint32_t int_level = rtos_disable_int();

	switch(step)
		{
			case CHARGE_STEP1:
				{
					sys_hal_usb_charge_vlcf_cal();
					break;
				}
			case CHARGE_STEP2:
				{
					sys_hal_usb_charge_icp_cal();
					break;
				}
			case CHARGE_STEP3:
				{
					sys_hal_usb_charge_vcv_cal();
					break;
				}
			case CHARGE_STEP4:
				{
					sys_hal_usb_charge_get_cal();
					break;
				}
			default:
				break;
		}

	rtos_enable_int(int_level);
}
#endif

/* Platform UART Start **/
void sys_drv_uart_select_clock(uart_id_t id, uart_src_clk_t mode)
{
	uint32_t int_level = rtos_disable_int();
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	sys_hal_uart_select_clock(id, mode);

	if(!ret)
		ret = sys_amp_res_release();

	rtos_enable_int(int_level);
}
/* Platform UART End **/

/* Platform PWM Start **/
void sys_drv_pwm_set_clock(uint32_t mode, uint32_t param)
{
	uint32_t int_level = rtos_disable_int();
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	sys_hal_pwm_set_clock(mode, param);

	if(!ret)
		ret = sys_amp_res_release();

	rtos_enable_int(int_level);
}

void sys_drv_pwm_select_clock(sys_sel_pwm_t num, pwm_src_clk_t mode)
{
	uint32_t int_level = rtos_disable_int();
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	sys_hal_pwm_select_clock(num, mode);

	if(!ret)
		ret = sys_amp_res_release();

	rtos_enable_int(int_level);
}
/* Platform PWM End **/

void sys_drv_timer_select_clock(sys_sel_timer_t num, timer_src_clk_t mode)
{
	uint32_t int_level = rtos_disable_int();
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	sys_hal_timer_select_clock(num, mode);

	if(!ret)
		ret = sys_amp_res_release();


	rtos_enable_int(int_level);
}

//sys_ctrl CMD: CMD_SCTRL_SET_FLASH_DCO
void sys_drv_flash_set_dco(void)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_flash_set_dco();

	rtos_enable_int(int_level);
}

//sys_ctrl CMD: CMD_SCTRL_SET_FLASH_DPLL
void sys_drv_flash_set_dpll(void)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_flash_set_dpll();

	rtos_enable_int(int_level);
}

void sys_drv_flash_cksel(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_flash_set_clk(value);

	rtos_enable_int(int_level);
}

void sys_drv_flash_set_clk_div(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_flash_set_clk_div(value);

	rtos_enable_int(int_level);
}

uint32_t sys_drv_flash_get_clk_sel(void)
{
	return sys_hal_flash_get_clk_sel();
}

uint32_t sys_drv_flash_get_clk_div(void)
{
	return sys_hal_flash_get_clk_div();
}

//sys_ctrl CMD: CMD_QSPI_VDDRAM_VOLTAGE
void sys_drv_set_qspi_vddram_voltage(uint32_t param)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_set_qspi_vddram_voltage(param);

	rtos_enable_int(int_level);
}

//sys_ctrl CMD: CMD_QSPI_IO_VOLTAGE
void sys_drv_set_qspi_io_voltage(uint32_t param)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_set_qspi_io_voltage(param);

	rtos_enable_int(int_level);
}

//sys_ctrl CMD: CMD_QSPI_CLK_SEL
void sys_drv_qspi_clk_sel(uint32_t param)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_qspi_clk_sel(param);

	rtos_enable_int(int_level);
}

//sys_ctrl CMD: CMD_EFUSE_READ_BYTE
void sys_drv_efuse_read_byte(uint32_t param)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_efuse_read_byte(param);

	rtos_enable_int(int_level);
}

//sys_ctrl CMD: CMD_EFUSE_WRITE_BYTE
void sys_drv_efuse_write_byte(uint32_t param)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_efuse_write_byte(param);

	rtos_enable_int(int_level);
}
void sys_drv_enter_deep_sleep(void * param)
{
	uint32_t int_level = rtos_disable_int();
	//uint32_t ret = SYS_DRV_FAILURE;
	//ret = sys_amp_res_acquire();

	sys_hal_enter_deep_sleep(param);

	//if(!ret)
		//ret = sys_amp_res_release();

	rtos_enable_int(int_level);
}

void sys_drv_enter_normal_sleep(UINT32 peri_clk)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_enter_normal_sleep(peri_clk);

	rtos_enable_int(int_level);
}

void sys_drv_enter_normal_wakeup()
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_enter_normal_wakeup();

	rtos_enable_int(int_level);
}
void sys_drv_enter_low_voltage()
{
	uint32_t int_level = rtos_disable_int();
	//uint32_t ret = SYS_DRV_FAILURE;
	//ret = sys_amp_res_acquire();

	sys_hal_enter_low_voltage();

	//if(!ret)
		//ret = sys_amp_res_release();

	rtos_enable_int(int_level);
}
/*for low power function start*/
void sys_drv_module_power_ctrl(power_module_name_t module,power_module_state_t power_state)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_module_power_ctrl(module,power_state);

	rtos_enable_int(int_level);
}

void sys_drv_module_RF_power_ctrl (module_name_t module,power_module_state_t power_state)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_module_RF_power_ctrl (module, power_state);

	rtos_enable_int(int_level);
}

void sys_drv_core_bus_clock_ctrl(high_clock_module_name_t core, uint32_t clksel,uint32_t clkdiv, high_clock_module_name_t bus,uint32_t bus_clksel,uint32_t bus_clkdiv)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_core_bus_clock_ctrl(core, clksel,clkdiv,bus,bus_clksel,bus_clkdiv);

	rtos_enable_int(int_level);
}
void sys_drv_cpu0_main_int_ctrl(dev_clk_pwr_ctrl_t clock_state)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_cpu0_main_int_ctrl(clock_state);

	rtos_enable_int(int_level);
}

void sys_drv_cpu1_main_int_ctrl(dev_clk_pwr_ctrl_t clock_state)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_cpu1_main_int_ctrl(clock_state);

	rtos_enable_int(int_level);
}

void sys_drv_set_cpu1_boot_address_offset(uint32_t address_offset)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_set_cpu1_boot_address_offset(address_offset);

	rtos_enable_int(int_level);
}

void sys_drv_set_cpu1_reset(uint32_t reset_value)
{
	uint32_t int_level = rtos_disable_int();

	/* 1:reset release, means cpu1 start to run; 0:reset means cpu1 at reset status */
	sys_hal_set_cpu1_reset(reset_value);

	rtos_enable_int(int_level);
}

void sys_drv_enable_mac_wakeup_source()
{
 	uint32_t int_level = rtos_disable_int();
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	sys_hal_enable_mac_wakeup_source();

	if(!ret)
		ret = sys_amp_res_release();

	rtos_enable_int(int_level);
}

void sys_drv_enable_bt_wakeup_source()
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_enable_bt_wakeup_source();

	rtos_enable_int(int_level);
}
void sys_drv_all_modules_clk_div_set(clk_div_reg_e reg, uint32_t value)
{
    sys_hal_all_modules_clk_div_set( reg, value);
}
uint32_t sys_drv_all_modules_clk_div_get(clk_div_reg_e reg)
{
	return sys_hal_all_modules_clk_div_get(reg);
}
void sys_drv_wakeup_interrupt_clear(wakeup_source_t interrupt_source)
{
    //sys_hal_wakeup_interrupt_clear(interrupt_source);
}
void sys_drv_wakeup_interrupt_set(wakeup_source_t interrupt_source)
{
    
}
void sys_drv_touch_wakeup_enable(uint8_t index)
{
    uint32_t int_level = rtos_disable_int();
    sys_hal_touch_wakeup_enable(index);
	rtos_enable_int(int_level);
}

void sys_drv_usb_wakeup_enable(uint8_t index)
{
    sys_hal_usb_wakeup_enable(index);
}
void sys_drv_cpu_clk_div_set(uint32_t core_index, uint32_t value)
{
	sys_hal_cpu_clk_div_set(core_index,value);
}
uint32_t sys_drv_cpu_clk_div_get(uint32_t core_index)
{
	return sys_hal_cpu_clk_div_get(core_index);
}
void sys_drv_low_power_hardware_init()
{
	sys_hal_low_power_hardware_init();
}
/*for low power function end*/
uint32 sys_drv_get_chip_id(void)
{
	uint32 reg = 0;
	uint32_t int_level = rtos_disable_int();

	reg = sys_hal_get_chip_id();

	rtos_enable_int(int_level);

	return reg;
}

// Replace sddev_control(DD_DEV_TYPE_SCTRL,CMD_GET_DEVICE_ID, NULL)
uint32 sys_drv_get_device_id(void)
{
	uint32 reg = 0;
	uint32_t int_level = rtos_disable_int();

	reg = sys_hal_get_device_id();

	rtos_enable_int(int_level);

	return reg;
}

int32 sys_drv_dsp_power_down(void)
{
	int32 ret = 0;
	uint32_t int_level = rtos_disable_int();

	ret = sys_hal_dsp_power_down();

	rtos_enable_int(int_level);

	return ret;
}

int32 sys_drv_dsp_power_up(void)
{
	int32 ret = 0;
	uint32_t int_level = rtos_disable_int();

	ret = sys_hal_dsp_power_up();

	rtos_enable_int(int_level);

	return ret;
}


int32 sys_drv_rtos_idle_sleep(void *param)
{
	int32 ret = 0;
	uint32_t int_level = rtos_disable_int();

	ret = sys_hal_rtos_idle_sleep(param);

	rtos_enable_int(int_level);

	return ret;
}

int32 sys_drv_rtos_idle_wakeup(void)
{
	int32 ret = 0;
	uint32_t int_level = rtos_disable_int();

	ret = sys_hal_rtos_idle_wakeup();

	rtos_enable_int(int_level);

	return ret;
}

int32 sys_drv_rtos_deep_sleep(void *param)
{
	int32 ret = 0;
	uint32_t int_level = rtos_disable_int();

	ret = sys_hal_rtos_deep_sleep(param);

	rtos_enable_int(int_level);

	return ret;
}



uint32 sys_drv_get_sctrl_retetion(void)
{
	int32 ret = 0;
	uint32_t int_level = rtos_disable_int();

	ret = sys_hal_get_sctrl_retetion();

	rtos_enable_int(int_level);

	return ret;
}

int32 sys_drv_set_sctrl_retetion(uint32 param)
{
	int32 ret = 0;
	uint32_t int_level = rtos_disable_int();

	ret = sys_hal_set_sctrl_retetion(param);

	rtos_enable_int(int_level);

	return ret;
}


int32 sys_drv_unconditional_mac_down(void)
{
	int32 ret = 0;
	uint32_t int_level = rtos_disable_int();

	ret = sys_hal_unconditional_mac_down();

	rtos_enable_int(int_level);

	return ret;
}

int32 sys_drv_unconditional_mac_up(void)
{
	int32 ret = 0;
	uint32_t int_level = rtos_disable_int();

	ret = sys_hal_unconditional_mac_up();

	rtos_enable_int(int_level);

	return ret;
}

int32 sys_drv_fix_dpll_div(void)
{
	int32 ret = 0;
	uint32_t int_level = rtos_disable_int();

	ret = sys_hal_fix_dpll_div();

	rtos_enable_int(int_level);

	return ret;
}

int32 sys_drv_set_low_pwr_clk(uint32 param)
{
	int32 ret = 0;
	uint32_t int_level = rtos_disable_int();

	ret = sys_hal_set_low_pwr_clk(param);

	rtos_enable_int(int_level);

	return ret;
}

int32 sys_drv_set_gadc_sel(uint32 param)
{
	int32 ret = 0;
	uint32_t int_level = rtos_disable_int();

	ret = sys_hal_set_gadc_sel(param);

	rtos_enable_int(int_level);

	return ret;
}

int32 sys_drv_clkgating_disable(uint32 param)
{
	int32 ret = 0;
	uint32_t int_level = rtos_disable_int();

	ret = sys_hal_clkgating_disable(param);

	rtos_enable_int(int_level);

	return ret;
}

int32 sys_drv_clkgating_enable(uint32 param)
{
	int32 ret = 0;
	uint32_t int_level = rtos_disable_int();

	ret = sys_hal_clkgating_enable(param);

	rtos_enable_int(int_level);

	return ret;
}

//NOTICE:This function is disable the INTERRUPT SOURCE, not the INTERRUPT TARGET.
int32 sys_drv_int_disable(uint32 param)
{
	int32 ret = 0;
	uint32_t int_level = rtos_disable_int();

	ret = sys_hal_int_disable(param);

	rtos_enable_int(int_level);

	return ret;
}

//NOTICE:This function is enable the INTERRUPT SOURCE, not the INTERRUPT TARGET.
int32 sys_drv_int_enable(uint32 param)
{
	int32 ret = 0;
	uint32_t int_level = rtos_disable_int();

	ret = sys_hal_int_enable(param);

	rtos_enable_int(int_level);

	return ret;
}

//NOTICE:Temp add for BK7256 product which has more then 32 Interrupt sources
int32 sys_drv_int_group2_disable(uint32 param)
{
	int32 ret = 0;
	uint32_t int_level = rtos_disable_int();

	ret = sys_hal_int_group2_disable(param);

	rtos_enable_int(int_level);

	return ret;
}

//NOTICE:Temp add for BK7256 product which has more then 32 Interrupt sources
int32 sys_drv_int_group2_enable(uint32 param)
{
	int32 ret = 0;
	uint32_t int_level = rtos_disable_int();

	ret = sys_hal_int_group2_enable(param);

	rtos_enable_int(int_level);

	return ret;
}

int32 sys_drv_fiq_disable(uint32 param)
{
	int32 ret = 0;
	uint32_t int_level = rtos_disable_int();

	ret = sys_hal_fiq_disable(param);

	rtos_enable_int(int_level);

	return ret;
}

int32 sys_drv_fiq_enable(uint32 param)
{
	int32 ret = 0;
	uint32_t int_level = rtos_disable_int();

	ret = sys_hal_fiq_enable(param);
	rtos_enable_int(int_level);

	return ret;
}

//	uint32_t int_level = rtos_disable_int();
int32 sys_drv_global_int_disable(uint32 param)
{
	int32 ret = 0;

	return ret;
}

// rtos_enable_int(int_level);
int32 sys_drv_global_int_enable(uint32 param)
{
	int32 ret = 0;
	uint32_t int_level = rtos_disable_int();

	ret = sys_hal_global_int_enable(param);
	rtos_enable_int(int_level);

	return ret;
}

//NOTICE:INT source status, not INT target status(IRQ,FIQ,NVIC,PLIC,INTC status)
uint32 sys_drv_get_int_source_status(void)
{
	uint32 ret = 0;
	uint32_t int_level = rtos_disable_int();

	ret = sys_hal_get_int_status();
	rtos_enable_int(int_level);

	return ret;
}

uint32_t sys_drv_get_cpu0_gpio_int_st(void)
{
    return sys_hal_get_cpu0_gpio_int_st();
}

//NOTICE:INT source status is read only and can't be set, we'll delete them.
int32 sys_drv_set_int_source_status(uint32 param)
{
	int32 ret = 0;
	uint32_t int_level = rtos_disable_int();

	ret = sys_hal_set_int_status(param);
	rtos_enable_int(int_level);

	return ret;
}

uint32 sys_drv_get_fiq_reg_status(void)
{
	int32 ret = 0;
	uint32_t int_level = rtos_disable_int();

	ret = sys_hal_get_fiq_reg_status();
	rtos_enable_int(int_level);

	return ret;
}

uint32 sys_drv_set_fiq_reg_status(uint32 param)
{
	int32 ret = 0;
	uint32_t int_level = rtos_disable_int();

	ret = sys_hal_set_fiq_reg_status(param);
	rtos_enable_int(int_level);

	return ret;
}

uint32 sys_drv_get_intr_raw_status(void)
{
	int32 ret = 0;
	uint32_t int_level = rtos_disable_int();

	ret = sys_hal_get_intr_raw_status();
	rtos_enable_int(int_level);

	return ret;
}

uint32 sys_drv_set_intr_raw_status(uint32 param)
{
	int32 ret = 0;
	uint32_t int_level = rtos_disable_int();


	ret = sys_hal_set_intr_raw_status(param);
	rtos_enable_int(int_level);

	return ret;
}

//sys_ctrl CMD: CMD_FUNC_CLK_PWR_DOWN
int32 sys_drv_func_clk_pwr_down(uint32 param)
{
	int32 ret = 0;
	uint32_t int_level = rtos_disable_int();

	ret = sys_hal_func_clk_pwr_down(param);
	rtos_enable_int(int_level);

	return ret;
}

//sys_ctrl CMD: CMD_FUNC_CLK_PWR_UP
int32 sys_drv_func_clk_pwr_up(uint32 param)
{
	int32 ret = 0;
	uint32_t int_level = rtos_disable_int();

	ret = sys_hal_func_clk_pwr_up(param);
	rtos_enable_int(int_level);

	return ret;
}

int32 sys_drv_conf_pwm_plck(uint32 param)
{
	int32 ret = 0;
	uint32_t int_level = rtos_disable_int();

	ret = sys_hal_conf_pwm_plck(param);
	rtos_enable_int(int_level);

	return ret;
}

int32 sys_drv_conf_pwm_lpoclk(uint32 param)
{
	int32 ret = 0;
	uint32_t int_level = rtos_disable_int();

	ret = sys_hal_conf_pwm_lpoclk(param);
	rtos_enable_int(int_level);

	return ret;
}

int32 sys_drv_conf_plck_26m(uint32 param)
{
	int32 ret = 0;
	uint32_t int_level = rtos_disable_int();

	ret = sys_hal_conf_plck_26m(param);
	rtos_enable_int(int_level);

	return ret;
}

int32 sys_drv_conf_plck_dco(uint32 param)
{
	int32 ret = 0;
	uint32_t int_level = rtos_disable_int();

	ret = sys_hal_conf_plck_dco(param);
	rtos_enable_int(int_level);

	return ret;
}

int32 sys_drv_set_jtag_mode(uint32 param)
{
	int32 ret = 0;
	uint32_t int_level = rtos_disable_int();

	ret = sys_hal_set_jtag_mode(param);
	rtos_enable_int(int_level);

	return ret;
}

uint32 sys_drv_get_jtag_mode(void)
{
	int32 ret = 0;
	uint32_t int_level = rtos_disable_int();

	ret = sys_hal_get_jtag_mode();
	rtos_enable_int(int_level);

	return ret;
}

/*clock power control start*/
void sys_drv_dev_clk_pwr_up(dev_clk_pwr_id_t dev, dev_clk_pwr_ctrl_t power_up)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_clk_pwr_ctrl(dev, power_up);

	rtos_enable_int(int_level);
}

void sys_drv_set_clk_select(dev_clk_select_id_t dev, dev_clk_select_t clk_sel)
{
	uint32_t int_level = rtos_disable_int();
	
	sys_hal_set_clk_select(dev, clk_sel);

	rtos_enable_int(int_level);
}

dev_clk_select_t sys_drv_get_clk_select(dev_clk_select_id_t dev)
{
	dev_clk_select_t clk_sel;	

	uint32_t int_level = rtos_disable_int();

	clk_sel = sys_hal_get_clk_select(dev);

	rtos_enable_int(int_level);

	return clk_sel;
}

//DCO divider is valid for all of the peri-devices.
void sys_drv_set_dco_div(dev_clk_dco_div_t div)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_set_dco_div(div);

	rtos_enable_int(int_level);
}

//DCO divider is valid for all of the peri-devices.
dev_clk_dco_div_t sys_drv_get_dco_div(void)
{
	dev_clk_dco_div_t dco_div;	

	uint32_t int_level = rtos_disable_int();

	dco_div = sys_hal_get_dco_div();

	rtos_enable_int(int_level);
	
	return dco_div;
}

/*clock power control end*/

/*wake up control start*/
//sys_ctrl CMD: CMD_ARM_WAKEUP_ENABLE
void sys_drv_arm_wakeup_enable(uint32_t param)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_arm_wakeup_enable(param);

	rtos_enable_int(int_level);
}

//sys_ctrl CMD: CMD_ARM_WAKEUP_DISABLE
void sys_drv_arm_wakeup_disable(uint32_t param)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_arm_wakeup_disable(param);

	rtos_enable_int(int_level);
}

//sys_ctrl CMD: CMD_GET_ARM_WAKEUP
uint32_t sys_drv_get_arm_wakeup(void)
{
	int32 ret = 0;
	uint32_t int_level = rtos_disable_int();


	ret = sys_hal_get_arm_wakeup();
	rtos_enable_int(int_level);

	return ret;
}
/*wake up control end*/

void sys_drv_sadc_int_enable(void)
{
    uint32_t int_level = rtos_disable_int();

    sys_hal_sadc_int_enable();
    rtos_enable_int(int_level);
}

void sys_drv_sadc_int_disable(void)
{
    uint32_t int_level = rtos_disable_int();

    sys_hal_sadc_int_disable();
    rtos_enable_int(int_level);
}

void sys_drv_sadc_pwr_up(void)
{
    uint32_t int_level = rtos_disable_int();

    sys_hal_sadc_pwr_up();
    rtos_enable_int(int_level);
}

void sys_drv_sadc_pwr_down(void)
{
    uint32_t int_level = rtos_disable_int();

    sys_hal_sadc_pwr_down();
    rtos_enable_int(int_level);
}

void sys_drv_en_tempdet(uint32_t value)
{
    uint32_t int_level = rtos_disable_int();

    sys_hal_en_tempdet(value);
    rtos_enable_int(int_level);
}
/**  Platform End **/



/**  BT Start **/
//BT

//CMD_SCTRL_MCLK_MUX_GET
uint32_t sys_drv_mclk_mux_get(void)
{
	uint32_t ret = 0;
	uint32_t int_level = rtos_disable_int();

	ret = sys_hal_mclk_mux_get();
	rtos_enable_int(int_level);

	return ret;
}

//CMD_SCTRL_MCLK_DIV_GET
uint32_t sys_drv_mclk_div_get(void)
{
	uint32_t ret = 0;
	uint32_t int_level = rtos_disable_int();

	ret = sys_hal_mclk_div_get();
	rtos_enable_int(int_level);

	return ret;
}

// CMD_SCTRL_MCLK_SELECT
void sys_drv_mclk_select(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_mclk_select(value);

	rtos_enable_int(int_level);
}

//CMD_SCTRL_MCLK_DIVISION
void sys_drv_mclk_div_set(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_mclk_div_set(value);

	rtos_enable_int(int_level);
}

//CMD_SCTRL_BLE_POWERUP/CMD_SCTRL_BLE_POWERDOWN/CMD_SCTRL_BT_POWERDOWN/CMD_SCTRL_BT_POWERUP
void sys_drv_bt_power_ctrl(bool power_up)
{
	uint32_t int_level = rtos_disable_int();
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	sys_hal_bt_power_ctrl(power_up);

	if(!ret)
		ret = sys_amp_res_release();

	rtos_enable_int(int_level);
}

//CMD_TL410_CLK_PWR_UP/CMD_TL410_CLK_PWR_DOWN
void sys_drv_bt_clock_ctrl(bool en)
{
	uint32_t int_level = rtos_disable_int();
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	sys_hal_bt_clock_ctrl(en);

	if(!ret)
		ret = sys_amp_res_release();

	rtos_enable_int(int_level);
}

//bk7256 add
void sys_drv_xvr_clock_ctrl(bool en)
{
 	uint32_t int_level = rtos_disable_int();
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	sys_hal_xvr_clock_ctrl(en);

	if(!ret)
		ret = sys_amp_res_release();

	rtos_enable_int(int_level);
}

// CMD_GET_INTR_STATUS
uint32_t sys_drv_interrupt_status_get(void)
{
	uint32_t ret = 0;
	uint32_t int_level = rtos_disable_int();

	ret = sys_hal_interrupt_status_get();
	rtos_enable_int(int_level);

	return ret;
}

// CMD_CLR_INTR_STATUS
void sys_drv_interrupt_status_set(uint32_t value)
{
 	uint32_t int_level = rtos_disable_int();

	sys_hal_interrupt_status_set(value);

	rtos_enable_int(int_level);
}

//从CMD_ICU_INT_ENABLE抽象出来，先模块单独实现，后面再调用平台统一接口
void sys_drv_btdm_interrupt_ctrl(bool en)
{
  	uint32_t int_level = rtos_disable_int();

	sys_hal_btdm_interrupt_ctrl(en);

	rtos_enable_int(int_level);
}

//从CMD_ICU_INT_ENABLE抽象出来，先模块单独实现，后面再调用平台统一接口
void sys_drv_ble_interrupt_ctrl(bool en)
{
  	uint32_t int_level = rtos_disable_int();

	sys_hal_ble_interrupt_ctrl(en);

	rtos_enable_int(int_level);
}

//从CMD_ICU_INT_ENABLE抽象出来，先模块单独实现，后面再调用平台统一接口
void sys_drv_bt_interrupt_ctrl(bool en)
{
   	uint32_t int_level = rtos_disable_int();

	sys_hal_bt_interrupt_ctrl(en);

	rtos_enable_int(int_level);
}

// CMD_BLE_RF_BIT_SET/CMD_BLE_RF_BIT_CLR
void sys_drv_bt_rf_ctrl(bool en)
{
  	uint32_t int_level = rtos_disable_int();

	sys_hal_bt_rf_ctrl(en);

	rtos_enable_int(int_level);
}

// CMD_BLE_RF_BIT_GET
uint32_t sys_drv_bt_rf_status_get(void)
{
	uint32_t ret = 0;
	uint32_t int_level = rtos_disable_int();

	ret = sys_hal_bt_rf_status_get();
	rtos_enable_int(int_level);

	return ret;
}

void sys_drv_bt_sleep_exit_ctrl(bool en)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_bt_sleep_exit_ctrl(en);

	rtos_enable_int(int_level);
}

/**  BT End **/




/**  Audio Start **/
//Audio
/**  Audio End **/




/**  Video Start **/
uint32_t sys_drv_lcd_set(uint8_t clk_src_sel, uint8_t clk_div_l, uint8_t clk_div_h, uint8_t int_en,uint8_t clk_always_on)
{
	uint32_t int_level = rtos_disable_int();
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	sys_hal_lcd_disp_clk_en(clk_src_sel, clk_div_l,clk_div_h, int_en, clk_always_on);

	if(!ret)
		ret = sys_amp_res_release();

	rtos_enable_int(int_level);

	return ret;
}

uint32_t sys_drv_lcd_close(void)
{
	uint32_t int_level = rtos_disable_int();
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	sys_hal_lcd_disp_close();

	if(!ret)
		ret = sys_amp_res_release();

	rtos_enable_int(int_level);

	return ret;
}

uint32_t sys_drv_dma2d_set(uint8_t clk_always_on, uint8_t sys_int_en)
{
	uint32_t int_level = rtos_disable_int();
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	sys_hal_dma2d_clk_en(clk_always_on, sys_int_en);
	if(!ret)
		ret = sys_amp_res_release();

	rtos_enable_int(int_level);

	return ret;
}

uint32_t sys_drv_jpeg_dec_set(uint8_t clk_always_on, uint8_t int_en)
{
	uint32_t int_level = rtos_disable_int();
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	sys_hal_jpeg_dec_ctrl(clk_always_on, int_en);
	if(!ret)
		ret = sys_amp_res_release();
	rtos_enable_int(int_level);

	return ret;
}




/**  Video End **/




/**  WIFI Start **/
//WIFI

void sys_drv_delay10us(void)
{
	volatile UINT32 i = 0;

	for (i = 0; i < SYS_DRV_DELAY_TIME_10US; i ++)
		;
}

void sys_drv_delay200us(void)
{
	volatile UINT32 i = 0;

	for (i = 0; i < SYS_DRV_DELAY_TIME_200US; i ++)
		;
}

void sys_drv_ps_dpll_delay(UINT32 time)
{
	volatile UINT32 i = 0;

	for (i = 0; i < time; i ++)
		;
}

uint32_t sys_drv_cali_dpll(uint32_t param)
{
	uint32_t int_level = rtos_disable_int();

	
    sys_hal_cali_dpll_spi_trig_disable();

    if (!param)
    {
        sys_drv_delay10us();
    }
    else
    {
        sys_drv_ps_dpll_delay(60);
    }
    
    sys_hal_cali_dpll_spi_trig_enable();
    sys_hal_cali_dpll_spi_detect_disable();

    if (!param)
    {
        sys_drv_delay200us();
    }
    else
    {
        sys_drv_ps_dpll_delay(340);
    }

	sys_hal_cali_dpll_spi_detect_enable();

	rtos_enable_int(int_level);

    return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_bias_reg_set(uint32_t param)
{
    uint32_t int_level = rtos_disable_int();

	sys_hal_bias_reg_set(param);
    rtos_enable_int(int_level);
    return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_bias_reg_clean(uint32_t param)
{
    uint32_t int_level = rtos_disable_int();

	sys_hal_bias_reg_clean(param);
    rtos_enable_int(int_level);
    return SYS_DRV_SUCCESS;
}
   
uint32_t sys_drv_bias_reg_read(void)
{
	uint32 ret;
	uint32_t int_level = rtos_disable_int();

	ret = sys_hal_bias_reg_read();
    rtos_enable_int(int_level);
    return ret;
}

uint32_t sys_drv_bias_reg_write(uint32_t param)
{
    uint32_t int_level = rtos_disable_int();

	sys_hal_bias_reg_write(param);
    rtos_enable_int(int_level);
    return SYS_DRV_SUCCESS;
}

#if 0
uint32_t sys_drv_bias_get_cali_out(void)
{
    //can't find PMU reg addr and bit filed definition
    //only used in BK7271
    return SYS_DRV_SUCCESS;
}
#endif

uint32_t sys_drv_analog_reg4_bits_or(uint32_t param)
{
    uint32_t int_level = rtos_disable_int();

    rtos_enable_int(int_level);
    //analog reg write only?
    return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_analog_ctrl9_real_set(uint32_t param)
{
    uint32_t int_level = rtos_disable_int();

    rtos_enable_int(int_level);
    //analog reg write only?
    return SYS_DRV_SUCCESS;
}


uint32_t sys_drv_analog_set_xtalh_ctune(uint32_t param)
{
    uint32_t int_level = rtos_disable_int();

    sys_hal_set_xtalh_ctune(param);
	rtos_enable_int(int_level);
    return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_analog_get_xtalh_ctune(void)
{
    uint32_t int_level = rtos_disable_int();
    uint32 reg_value;
    reg_value = sys_hal_get_xtalh_ctune();
	rtos_enable_int(int_level);
    return reg_value;
}
void sys_drv_analog_set(analog_reg_t reg, uint32_t value)
{
    sys_hal_analog_set(reg,value);
}
uint32_t sys_drv_analog_get(analog_reg_t reg)
{
    return sys_hal_analog_get(reg);
}
uint32_t sys_drv_analog_reg1_set(uint32_t param)
{
    uint32_t int_level = rtos_disable_int();

    sys_hal_set_ana_reg1_value(param);
	rtos_enable_int(int_level);
    return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_analog_reg2_set(uint32_t param)
{
    uint32_t int_level = rtos_disable_int();

    sys_hal_set_ana_reg2_value(param);
	rtos_enable_int(int_level);
    return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_analog_reg3_set(uint32_t param)
{
    uint32_t int_level = rtos_disable_int();

    sys_hal_set_ana_reg3_value(param);
	rtos_enable_int(int_level);
    return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_analog_reg4_set(uint32_t param)
{
    uint32_t int_level = rtos_disable_int();

    sys_hal_set_ana_reg4_value(param);
	rtos_enable_int(int_level);
    return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_analog_reg12_set(uint32_t param)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_set_ana_reg12_value(param);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_analog_reg13_set(uint32_t param)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_set_ana_reg13_value(param);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_analog_reg14_set(uint32_t param)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_set_ana_reg14_value(param);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_analog_reg15_set(uint32_t param)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_set_ana_reg15_value(param);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_analog_reg16_set(uint32_t param)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_set_ana_reg16_value(param);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_analog_reg17_set(uint32_t param)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_set_ana_reg17_value(param);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_analog_reg6_set(uint32_t param)
{
    //analog reg write only?
	uint32_t int_level = rtos_disable_int();

    sys_hal_set_ana_reg6_value(param);
    rtos_enable_int(int_level);
    return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_analog_reg7_set(uint32_t param)
{
    uint32_t int_level = rtos_disable_int();

	sys_hal_set_ana_reg7_value(param);
    rtos_enable_int(int_level);
    return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_analog_reg1_get(void)
{
	uint32 ret;
    uint32_t int_level = rtos_disable_int();

	ret = sys_hal_analog_reg1_get();
    rtos_enable_int(int_level);
    return ret;
}
uint32_t sys_drv_analog_reg2_get(void)
{
	uint32 ret;
    uint32_t int_level = rtos_disable_int();

	ret = sys_hal_analog_reg2_get();
    rtos_enable_int(int_level);
    return ret;
}
uint32_t sys_drv_analog_reg4_get(void)
{
	uint32 ret;
    uint32_t int_level = rtos_disable_int();

	ret = sys_hal_analog_reg4_get();
    rtos_enable_int(int_level);
	return ret;
}


uint32_t sys_drv_analog_reg6_get(void)
{
	uint32 ret;
    uint32_t int_level = rtos_disable_int();

	ret = sys_hal_analog_reg6_get();
    rtos_enable_int(int_level);
	return ret;
}


uint32_t sys_drv_analog_reg7_get(void)
{
	uint32 ret;
    uint32_t int_level = rtos_disable_int();

	ret = sys_hal_analog_reg7_get();
	rtos_enable_int(int_level);
	return ret;
}

uint32_t sys_drv_get_bgcalm(void)
{
    uint32_t ret;
	uint32_t int_level = rtos_disable_int();

	ret = sys_hal_get_bgcalm();
	rtos_enable_int(int_level);
    return ret;
}

uint32_t sys_drv_set_bgcalm(uint32_t param)
{
    uint32_t int_level = rtos_disable_int();

    sys_hal_set_bgcalm(param);
	rtos_enable_int(int_level);
    return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_set_dpll_for_i2s(void)
{
    uint32_t int_level = rtos_disable_int();

    sys_hal_set_audioen(1);
	sys_hal_set_dpll_div_cksel(1);
	sys_hal_set_dpll_reset(1);
	rtos_enable_int(int_level);
    return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_set_gadc_ten(uint32_t param)
{
    uint32_t int_level = rtos_disable_int();

    sys_hal_set_gadc_ten(param);
	rtos_enable_int(int_level);
    return SYS_DRV_SUCCESS;
}


//Yantao Add Start

//Unused
//CMD_SCTRL_RESET_SET, CMD_SCTRL_RESET_CLR
uint32_t sys_drv_reset_ctrl(uint32_t value)
{
	return SYS_DRV_SUCCESS;
}

//CMD_SCTRL_MODEM_CORE_RESET
uint32_t sys_drv_modem_core_reset(void)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_modem_core_reset();

	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

//CMD_SCTRL_MPIF_CLK_INVERT
uint32_t sys_drv_mpif_invert(void)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_mpif_invert();

	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

//CMD_SCTRL_MODEM_SUBCHIP_RESET
uint32_t sys_drv_modem_subsys_reset(void)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_modem_subsys_reset();

	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}
//CMD_SCTRL_MAC_SUBSYS_RESET
uint32_t sys_drv_mac_subsys_reset(void)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_mac_subsys_reset();

	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}
//CMD_SCTRL_USB_SUBSYS_RESET
uint32_t sys_drv_usb_subsys_reset(void)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_usb_subsys_reset();

	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}
//CMD_SCTRL_DSP_SUBSYS_RESET
uint32_t sys_drv_dsp_subsys_reset(void)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_dsp_subsys_reset();

	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

//CMD_SCTRL_MAC_POWERDOWN, CMD_SCTRL_MAC_POWERUP
uint32_t sys_drv_mac_power_ctrl(bool power_up)
{
	uint32_t int_level = rtos_disable_int();
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	sys_hal_mac_power_ctrl(power_up);

	if(!ret)
		ret = sys_amp_res_release();

	rtos_enable_int(int_level);
	return ret;
}

//CMD_SCTRL_MODEM_POWERDOWN, CMD_SCTRL_MODEM_POWERUP
uint32_t sys_drv_modem_power_ctrl(bool power_up)
{
	uint32_t int_level = rtos_disable_int();
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	sys_hal_modem_clk_ctrl(power_up);

	if(!ret)
		ret = sys_amp_res_release();

	rtos_enable_int(int_level);
	return ret;
}

//CMD_BLE_RF_PTA_EN, CMD_BLE_RF_PTA_DIS
uint32_t sys_drv_pta_ctrl(bool pta_en)
{
#if (CONFIG_SOC_BK7231N) || (CONFIG_SOC_BK7236) || (CONFIG_SOC_BK7256XX) || (CONFIG_SOC_BK7256_CP1)
	uint32_t int_level = rtos_disable_int();

	sys_hal_pta_ctrl(pta_en);

	rtos_enable_int(int_level);
#endif
	return SYS_DRV_SUCCESS;
}

//CMD_SCTRL_MODEM_AHB_CLOCK_DISABLE, CMD_SCTRL_MODEM_AHB_CLOCK_ENABLE
uint32_t sys_drv_modem_bus_clk_ctrl(bool clk_en)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_modem_bus_clk_ctrl(clk_en);

	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}
//CMD_SCTRL_MODEM_CLOCK480M_DISABLE, CMD_SCTRL_MODEM_CLOCK480M_ENABLE
uint32_t sys_drv_modem_clk_ctrl(bool clk_en)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_modem_clk_ctrl(clk_en);

	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

//CMD_SCTRL_MAC_AHB_CLOCK_DISABLE, CMD_SCTRL_MAC_AHB_CLOCK_ENABLE
uint32_t sys_drv_mac_bus_clk_ctrl(bool clk_en)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_mac_bus_clk_ctrl(clk_en);

	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}
//CMD_SCTRL_MAC_CLOCK480M_DISABLE,CMD_SCTRL_MAC_CLOCK480M_ENABLE
uint32_t sys_drv_mac_clk_ctrl(bool clk_en)
{
	uint32_t int_level = rtos_disable_int();
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	sys_hal_mac_clk_ctrl(clk_en);

	if(!ret)
		ret = sys_amp_res_release();

	rtos_enable_int(int_level);
	return ret;
}

//CMD_SCTRL_SET_VDD_VALUE
uint32_t sys_drv_set_vdd_value(uint32_t param)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_set_vdd_value(param);

	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}
//CMD_SCTRL_GET_VDD_VALUE
uint32_t sys_drv_get_vdd_value(void)
{
	uint32 ret;

	uint32_t int_level = rtos_disable_int();

	ret = sys_hal_get_vdd_value();

	rtos_enable_int(int_level);
	return ret;
}

//CMD_SCTRL_BLOCK_EN_MUX_SET
uint32_t sys_drv_block_en_mux_set(uint32_t param)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_block_en_mux_set(param);

	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}
uint32_t  sys_drv_enable_mac_gen_int(void)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_enable_mac_gen_int();

	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}
uint32_t  sys_drv_enable_mac_prot_int(void)
{
	uint32_t int_level = rtos_disable_int();


	sys_hal_enable_mac_prot_int();

	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}
uint32_t  sys_drv_enable_mac_tx_trigger_int(void)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_enable_mac_tx_trigger_int();

	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}
uint32_t  sys_drv_enable_mac_rx_trigger_int(void)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_enable_mac_rx_trigger_int();

	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}
uint32_t  sys_drv_enable_mac_txrx_misc_int(void)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_enable_mac_txrx_misc_int();

	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}
uint32_t  sys_drv_enable_mac_txrx_timer_int(void)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_enable_mac_txrx_timer_int();

	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t  sys_drv_enable_modem_int(void)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_enable_modem_int();

	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}
uint32_t  sys_drv_enable_modem_rc_int(void)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_enable_modem_rc_int();

	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}


//Yantao Add End
/**  WIFI End **/

/**  Audio Start  **/

uint32_t  sys_drv_aud_select_clock(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	sys_hal_aud_select_clock(value);

	if(!ret)
		ret = sys_amp_res_release();
	rtos_enable_int(int_level);

	return ret;
}

uint32_t  sys_drv_aud_clock_en(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	sys_hal_aud_clock_en(value);

	if(!ret)
		ret = sys_amp_res_release();

	rtos_enable_int(int_level);
	return ret;
}

uint32_t sys_drv_aud_vdd1v_en(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_aud_vdd1v_en(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_vdd1v5_en(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_aud_vdd1v5_en(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_mic1_en(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_aud_mic1_en(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_mic2_en(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_aud_mic2_en(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_audpll_en(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_aud_audpll_en(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_dacdrv_en(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_aud_dacdrv_en(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_bias_en(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_aud_bias_en(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_dacr_en(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_aud_dacr_en(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_dacl_en(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_aud_dacl_en(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_rvcmd_en(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_aud_rvcmd_en(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_lvcmd_en(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_aud_lvcmd_en(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_micbias1v_en(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_aud_micbias1v_en(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_micbias_trim_set(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_aud_micbias_trim_set(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_mic_rst_set(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_aud_mic_rst_set(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_mic1_gain_set(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_aud_mic1_gain_set(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_mic2_gain_set(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_aud_mic2_gain_set(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_int_en(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_aud_int_en(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_sbc_int_en(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_sbc_int_en(value);

	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_aud_power_en(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	sys_hal_aud_power_en(value);

	if(!ret)
		ret = sys_amp_res_release();

	rtos_enable_int(int_level);
	return ret;
}


/**  Audio End  **/


/**  FFT Start  **/
uint32_t sys_drv_fft_disckg_set(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_fft_disckg_set(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_cpu_fft_int_en(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_cpu_fft_int_en(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

/**  FFT End  **/

/**  I2S Start  **/
uint32_t  sys_drv_i2s_select_clock(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	sys_hal_i2s_select_clock(value);

	if(!ret)
		ret = sys_amp_res_release();

	rtos_enable_int(int_level);
	return ret;
}

uint32_t  sys_drv_i2s_clock_en(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_aud_clock_en(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_i2s_disckg_set(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_i2s_disckg_set(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_i2s_int_en(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_i2s_int_en(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_apll_en(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_apll_en(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_cb_manu_val_set(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_cb_manu_val_set(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_ana_reg11_vsel_set(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_ana_reg11_vsel_set(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_ana_reg10_sdm_val_set(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_ana_reg10_sdm_val_set(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_ana_reg11_spi_trigger_set(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_ana_reg11_spi_trigger_set(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}


/**  I2S End  **/

/**  Touch Start **/
uint32_t sys_drv_touch_power_down(uint32_t enable)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_touch_power_down(enable);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_touch_sensitivity_level_set(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_touch_sensitivity_level_set(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_touch_scan_mode_enable(uint32_t enable)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_touch_scan_mode_enable(enable);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_touch_detect_threshold_set(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_touch_detect_threshold_set(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_touch_detect_range_set(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_touch_detect_range_set(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_touch_calib_enable(uint32_t enable)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_touch_calib_enable(enable);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_touch_manul_mode_calib_value_set(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_touch_manul_mode_calib_value_set(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_touch_manul_mode_enable(uint32_t enable)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_touch_manul_mode_enable(enable);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_touch_scan_mode_chann_set(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_touch_scan_mode_chann_set(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_touch_int_enable(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_touch_int_enable(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

/**  Touch End **/

/**  Jpeg Start **/
uint32_t sys_drv_mclk_mux_set(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	sys_hal_mclk_mux_set(value);

	if(!ret)
		ret = sys_amp_res_release();

	rtos_enable_int(int_level);
	return ret;
}

uint32_t sys_drv_set_jpeg_clk_sel(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_set_jpeg_clk_sel(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_set_clk_div_mode1_clkdiv_jpeg(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	sys_hal_set_clk_div_mode1_clkdiv_jpeg(value);

	if(!ret)
		ret = sys_amp_res_release();

	rtos_enable_int(int_level);
	return ret;
}

uint32_t sys_drv_set_jpeg_disckg(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();
	sys_hal_set_jpeg_disckg(value);

	if(!ret)
		ret = sys_amp_res_release();
	rtos_enable_int(int_level);
	return ret;
}

uint32_t sys_drv_set_cpu_clk_div_mode1_clkdiv_bus(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	sys_hal_set_cpu_clk_div_mode1_clkdiv_bus(value);

	if(!ret)
		ret = sys_amp_res_release();

	rtos_enable_int(int_level);
	return ret;
}

uint32_t sys_drv_video_power_en(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	sys_hal_video_power_en(value);

	if(!ret)
		ret = sys_amp_res_release();


	rtos_enable_int(int_level);
	return ret;
}

/**  Jpeg End **/

/**  psram Start **/
uint32_t sys_drv_psram_volstage_sel(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_psram_volstage_sel(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_psram_xtall_osc_enable(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_psram_xtall_osc_enable(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_psram_dco_enable(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_psram_doc_enable(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_psram_dpll_enable(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_psram_dpll_enable(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_psram_ldo_enable(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_psram_ldo_enable(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}


uint32_t sys_drv_psram_clk_sel(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_psram_clk_sel(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_psram_set_clkdiv(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_psram_set_clkdiv(value);
	rtos_enable_int(int_level);
	return SYS_DRV_SUCCESS;
}

/**  psram End **/


uint32_t sys_drv_get_cpu_storage_connect_op_select_flash_sel(void)
{
return 	sys_hal_get_cpu_storage_connect_op_select_flash_sel();
}

void sys_drv_set_cpu_storage_connect_op_select_flash_sel(uint32_t value)
{
	sys_hal_set_cpu_storage_connect_op_select_flash_sel(value);
}


/**  Misc Start **/
//Misc
/**  Misc End **/


/* Temp reserved previous system_drive.c's codes */
#if 0
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
#include "system_hal.h"

static system_hal_t s_system;

extern bk_err_t system_hal_init(system_hal_t *hal);

void system_driver_init()
{
	system_hal_init(&s_system);
}

//0x0
uint32_t system_driver_get_device_id()
{
	return system_hal_get_device_id(&s_system);
}

//0x01
uint32_t system_driver_get_version_id()
{
	return system_hal_get_version_id(&s_system);
}

//0x02
uint32_t system_driver_get_core0_halted_st()
{
	return system_hal_get_core0_halted_st(&s_system);
}
uint32_t system_driver_get_core1_halted_st()
{
	return system_hal_get_core1_halted_st(&s_system);
}
uint32_t system_driver_get_cpu0_sw_reset_st()
{
	return system_hal_get_core1_halted_st(&s_system);
}
uint32_t systesm_driver_get_cpu1_sw_reset_st()
{
	return system_hal_get_cpu1_sw_reset_st(&s_system);
}
uint32_t system_driver_get_cpu0_pwr_dw_st()
{
	return system_hal_get_cpu0_pwr_dw_st(&s_system);
}
uint32_t system_driver_get_cpu1_pwr_dw_st()
{
	return system_hal_get_cpu1_pwr_dw_st(&s_system);
}

//0x03
void system_driver_set_boot_rom_mode()
{
	system_hal_set_boot_rom_mode(&s_system);

}
void system_driver_set_boot_flash_mode()
{
	system_hal_set_boot_flash_mode(&s_system);
}
uint32_t system_driver_get_boot_mode()
{
	return system_hal_get_boot_mode(&s_system);
}

void system_driver_set_jtag_connect_core0()
{
	system_hal_set_jtag_connect_core0(&s_system);
}
void system_driver_set_jtag_connect_core1()
{
	system_hal_set_jtag_connect_core1(&s_system);
}
uint32_t system_driver_get_jtag_connect_core()
{
	return system_hal_get_jtag_connect_core(&s_system);
}

void system_driver_set_flash_select_normal_op()
{
	system_hal_set_flash_select_normal_op(&s_system);
}
void system_driver_set_flash_select_spi_op()
{
	system_hal_set_flash_select_spi_op(&s_system);
}
uint32_t system_driver_get_flash_select_op()
{
	return system_hal_get_flash_select_op(&s_system);
}

//0x04
void system_driver_set_cpu0_int_mask(bool value)
{
	if(value)
		system_hal_set_cpu0_int_mask(&s_system, 1);
	else
		system_hal_set_cpu0_int_mask(&s_system, 0);

}
uint32_t system_driver_get_cpu0_int_mask()
{
	return system_hal_get_cpu0_int_mask(&s_system);
}

void system_driver_set_core0_halt_indicate(bool value)
{
	if(value)
		system_hal_set_core0_halt_indicate(&s_system, 1);
	else
		system_hal_set_core0_halt_indicate(&s_system, 0);
}
uint32_t system_driver_get_core0_halt_indicate()
{
	return system_hal_get_core0_halt_indicate(&s_system);
}

void system_driver_set_cpu0_clk_div(bool value)
{
	if(value)
		system_hal_set_cpu0_clk_div(&s_system, 1);
	else
		system_hal_set_cpu0_clk_div(&s_system, 0);

}
uint32_t system_driver_get_cpu0_clk_div()
{
	return system_hal_get_cpu0_clk_div(&s_system);
}


//0x05
void system_driver_set_cpu1_int_mask(bool value)
{
	if(value)
		system_hal_set_cpu1_int_mask(&s_system, 1);
	else
		system_hal_set_cpu1_int_mask(&s_system, 0);

}
uint32_t system_driver_get_cpu1_int_mask()
{
	return system_hal_get_cpu1_int_mask(&s_system);
}

void system_driver_set_core1_halt_indicate(bool value)
{
	if(value)
		system_hal_set_core1_halt_indicate(&s_system, 1);
	else
		system_hal_set_core1_halt_indicate(&s_system, 0);
}
uint32_t system_driver_get_core1_halt_indicate()
{
	return system_hal_get_cpu1_clk_div(&s_system);
}

void system_driver_set_cpu1_clk_div(uint32_t value)
{
	system_hal_set_core_clk_div(&s_system, value);
}
uint32_t system_driver_get_cpu1_clk_div()
{
	return system_hal_get_core_clk_div(&s_system);
}


//0x08
void system_driver_set_core_clk_div(bool value)
{
	system_hal_set_core_clk_sel(&s_system, value);
}
uint32_t system_driver_get_core_clk_div()
{
	return system_hal_get_core_clk_sel(&s_system);
}

void system_driver_set_core_clk_sel(bool value)
{
	if(value)
		system_hal_set_bus_clk_div(&s_system, 1);
	else
		system_hal_set_bus_clk_div(&s_system, 0);
}
uint32_t system_driver_get_core_clk_sel()
{
	return system_hal_get_core_clk_sel(&s_system);
}

void system_driver_set_bus_clk_div(bool value)
{
	if(value)
		system_hal_set_bus_clk_div(&s_system, 1);
	else
		system_hal_set_bus_clk_div(&s_system, 0);
}
uint32_t system_driver_get_bus_clk_div()
{
	return system_hal_get_bus_clk_div(&s_system);
}

void system_driver_set_uart0_clk_div(uint32_t value)
{
	if(value)
		system_hal_set_uart0_clk_div(&s_system, value);
}
uint32_t system_driver_get_uart0_clk_div()
{
	return system_hal_get_uart0_clk_div(&s_system);
}
void system_driver_set_uart0_clk_sel(bool value)
{
	if(value)
		system_hal_set_uart0_clk_sel(&s_system, 1);
	else
		system_hal_set_uart0_clk_sel(&s_system, 0);

}
uint32_t system_driver_get_uart0_clk_sel()
{
	return system_hal_get_uart0_clk_sel(&s_system);
}

void system_driver_set_uart1_clk_div(uint32_t value)
{
	if(value)
		system_hal_set_uart1_clk_div(&s_system, value);

}
uint32_t system_driver_get_uart1_clk_div()
{
	return system_hal_get_uart1_clk_div(&s_system);
}
void system_driver_set_uart1_clk_sel(bool value)
{
	if(value)
		system_hal_set_uart1_clk_sel(&s_system, 1);
	else
		system_hal_set_uart1_clk_sel(&s_system, 0);
}
uint32_t system_driver_get_uart1_clk_sel()
{
	return system_hal_get_uart1_clk_sel(&s_system);
}

void system_driver_set_uart2_clk_div(bool value)
{
	if(value)
		system_hal_set_uart2_clk_div(&s_system, value);
}
uint32_t system_driver_get_uart2_clk_div()
{
	return system_hal_get_uart2_clk_div(&s_system);
}

void system_driver_set_uart2_clk_sel(bool value)
{
	if(value)
		system_hal_set_uart2_clk_sel(&s_system, 1);
	else
		system_hal_set_uart2_clk_sel(&s_system, 0);
}
uint32_t system_driver_get_uart2_clk_sel()
{
	return system_hal_get_uart2_clk_sel(&s_system);
}

void system_driver_set_sadc_clk_sel(bool value)
{
	if(value)
		system_hal_set_sadc_clk_sel(&s_system, 1);
	else
		system_hal_set_sadc_clk_sel(&s_system, 0);
}
uint32_t system_driver_get_sadc_clk_sel()
{
	return system_hal_get_sadc_clk_sel(&s_system);
}

void system_driver_set_pwm0_clk_sel(bool value)
{
	if(value)
		system_hal_set_pwm0_clk_sel(&s_system, 1);
	else
		system_hal_set_pwm0_clk_sel(&s_system, 0);
}
uint32_t system_driver_get_pwm0_clk_sel()
{
	return system_hal_get_pwm0_clk_sel(&s_system);
}

void system_driver_set_pwm1_clk_sel(bool value)
{
	if(value)
		system_hal_set_pwm1_clk_sel(&s_system, 1);
	else
		system_hal_set_pwm1_clk_sel(&s_system, 0);
}
uint32_t system_driver_get_pwm1_clk_sel()
{
	return system_hal_get_pwm1_clk_sel(&s_system);
}

void system_driver_set_timer0_clk_sel(bool value)
{
	if(value)
		system_hal_set_timer0_clk_sel(&s_system, 1);
	else
		system_hal_set_timer0_clk_sel(&s_system, 0);
}
uint32_t system_driver_get_timer0_clk_sel()
{
	return system_hal_get_timer0_clk_sel(&s_system);
}

void system_driver_set_timer1_clk_sel(bool value)
{
	if(value)
		system_hal_set_timer1_clk_sel(&s_system, 1);
	else
		system_hal_set_timer1_clk_sel(&s_system, 0);
}
uint32_t system_driver_get_timer1_clk_sel()
{
	return system_hal_get_timer1_clk_sel(&s_system);
}
void system_driver_set_timer2_clk_sel(bool value)
{
	if(value)
		system_hal_set_timer2_clk_sel(&s_system, 1);
	else
		system_hal_set_timer2_clk_sel(&s_system, 0);
}
uint32_t system_driver_get_timer2_clk_sel()
{
	return system_hal_get_timer2_clk_sel(&s_system);
}

void system_driver_set_i2s_clk_sel(bool value)
{
	if(value)
		system_hal_set_i2s_clk_sel(&s_system, 1);
	else
		system_hal_set_i2s_clk_sel(&s_system, 0);
}
uint32_t system_driver_get_i2s_clk_sel()
{
	return system_hal_get_i2s_clk_sel(&s_system);
}

void system_driver_set_aud_clk_sel(bool value)
{
	if(value)
		system_hal_set_aud_clk_sel(&s_system, 1);
	else
		system_hal_set_aud_clk_sel(&s_system, 0);
}
uint32_t system_driver_get_aud_clk_sel()
{
	return system_hal_get_aud_clk_sel(&s_system);
}
void system_driver_set_jpeg_clk_div(uint32_t value)
{
	system_hal_set_jpeg_clk_div(&s_system, value);
}
uint32_t system_driver_get_jpeg_clk_div()
{
	return system_hal_get_jpeg_clk_div(&s_system);
}
void system_driver_set_jpeg_clk_sel(bool value)
{
	if(value)
		system_hal_set_jpeg_clk_sel(&s_system, 1);
	else
		system_hal_set_jpeg_clk_sel(&s_system, 0);
}
uint32_t system_driver_get_jpeg_clk_sel()
{
	return system_hal_get_jpeg_clk_sel(&s_system);
}

void system_driver_set_disp_l_clk_div(uint32_t value)
{
	system_hal_set_disp_l_clk_div(&s_system, value);
}
uint32_t system_driver_get_disp_l_clk_div()
{
	return system_hal_get_disp_l_clk_div(&s_system);
}

//0x09
void system_driver_set_disp_h_clk_div(bool value)
{
	system_hal_set_disp_h_clk_div(&s_system, value);
}
uint32_t system_driver_get_disp_h_clk_div()
{
	return system_hal_get_disp_h_clk_div(&s_system);
}
void system_driver_set_disp_clk_sel(bool value)
{
	if(value)
		system_hal_set_disp_clk_sel(&s_system, 1);
	else
		system_hal_set_disp_clk_sel(&s_system, 0);
}

uint32_t system_driver_get_disp_clk_sel()
{
	return system_hal_get_disp_clk_sel(&s_system);
}

void system_driver_set_psram_clk_div(uint32_t value)
{
	system_hal_set_psram_clk_div(&s_system, value);
}
uint32_t system_driver_get_psram_clk_div()
{
	return system_hal_get_psram_clk_div(&s_system);
}
void system_driver_set_psram_clk_sel(bool value)
{
	if(value)
		system_hal_set_psram_clk_sel(&s_system, 1);
	else
		system_hal_set_psram_clk_sel(&s_system, 0);
}
uint32_t system_driver_get_psram_clk_sel()
{
	return system_hal_get_psram_clk_sel(&s_system);
}

void system_driver_set_qspi0_clk_div(uint32_t value)
{
	system_hal_set_qspi0_clk_div(&s_system, value);
}
uint32_t system_driver_get_qspi0_clk_div()
{
	return system_hal_get_qspi0_clk_div(&s_system);
}
#endif

#if CONFIG_SDIO_V2P0
void sys_driver_set_sdio_clk_en(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	sys_hal_set_sdio_clk_en(value);

	if(!ret)
		ret = sys_amp_res_release();

	rtos_enable_int(int_level);
}

void sys_driver_set_cpu0_sdio_int_en(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_set_cpu0_sdio_int_en(value);

	rtos_enable_int(int_level);
}

void sys_driver_set_cpu1_sdio_int_en(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_set_cpu1_sdio_int_en(value);

	rtos_enable_int(int_level);
}

void sys_driver_set_sdio_clk_div(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_set_sdio_clk_div(value);

	rtos_enable_int(int_level);
}

uint32_t sys_driver_get_sdio_clk_div()
{
	uint32_t reg_v;
	uint32_t int_level = rtos_disable_int();

	reg_v = sys_hal_get_sdio_clk_div();
	rtos_enable_int(int_level);

	return reg_v;
}
void sys_driver_set_sdio_clk_sel(uint32_t value)
{
	uint32_t int_level = rtos_disable_int();

	sys_hal_set_sdio_clk_sel(value);

	rtos_enable_int(int_level);
}
uint32_t sys_driver_get_sdio_clk_sel()
{
	uint32_t reg_v;
	uint32_t int_level = rtos_disable_int();

	reg_v = sys_hal_get_sdio_clk_sel();
	rtos_enable_int(int_level);

	return reg_v;
}
#endif

#if 0
void system_driver_set_auxs_clk_div(uint32_t value)
{
	system_hal_set_auxs_clk_div(&s_system, value);
}
uint32_t system_driver_get_auxs_clk_div()
{
	return system_hal_get_auxs_clk_div(&s_system);
}
void system_driver_set_auxs_clk_sel(bool value)
{
	if(value)
		system_hal_set_auxs_clk_sel(&s_system, 1);
	else
		system_hal_set_auxs_clk_sel(&s_system, 0);
}
uint32_t system_driver_get_auxs_clk_sel()
{
	return system_hal_get_auxs_clk_sel(&s_system);
}

void system_driver_set_flash_clk_sel(bool value)
{
	if(value)
		system_hal_set_flash_clk_sel(&s_system, 1);
	else
		system_hal_set_flash_clk_sel(&s_system, 0);
}
uint32_t system_driver_get_flash_clk_sel()
{
	return system_hal_get_flash_clk_sel(&s_system);
}
void system_driver_set_flash_clk_div(uint32_t value)
{
	system_hal_set_flash_clk_div(&s_system, value);
}
uint32_t system_driver_get_flash_clk_div()
{
	return system_hal_get_flash_clk_div(&s_system);
}

void system_driver_set_i2s0_clk_div(uint32_t value)
{
	system_hal_set_i2s0_clk_div(&s_system, value);
}
uint32_t system_driver_get_i2s0_clk_div()
{
	return system_hal_get_i2s0_clk_div(&s_system);
}

//0x0a
void system_driver_set_26m_clk_div(uint32_t value)
{
	system_hal_set_26m_clk_div(&s_system, value);
}
uint32_t system_driver_get_26m_clk_div()
{
	return system_hal_get_26m_clk_div(&s_system);
}
void system_driver_set_wdt_clk_div(uint32_t value)
{
	system_hal_set_wdt_clk_div(&s_system, value);
}
uint32_t system_driver_get_wdt_clk_div()
{
	return system_hal_get_wdt_clk_div(&s_system);
}

//0x0b
void system_driver_set_anaspi_freq(uint32_t value)
{
	system_hal_set_anaspi_freq(&s_system, value);
}
uint32_t system_driver_get_anaspi_freq()
{
	return system_hal_get_anaspi_freq(&s_system);
}

//0x0c
void system_driver_set_i2c0_clk_en(bool value)
{
	if(value)
		system_hal_set_i2c0_clk_en(&s_system, 1);
	else
		system_hal_set_i2c0_clk_en(&s_system, 0);
}
uint32_t system_driver_get_i2c0_clk_en()
{
	return system_hal_get_i2c0_clk_en(&s_system);
}

void system_driver_set_spi0_clk_en(bool value)
{
	if(value)
		system_hal_set_spi0_clk_en(&s_system, 1);
	else
		system_hal_set_spi0_clk_en(&s_system, 0);
}
uint32_t system_driver_get_spi0_clk_en()
{
	return system_hal_get_spi0_clk_en(&s_system);
}

void system_driver_set_uart0_clk_en(bool value)
{
	if(value)
		system_hal_set_uart0_clk_en(&s_system, 1);
	else
		system_hal_set_uart0_clk_en(&s_system, 0);
}
uint32_t system_driver_get_uart0_clk_en()
{
	return system_hal_get_uart0_clk_en(&s_system);
}

void system_driver_set_pwm0_clk_en(bool value)
{
	if(value)
		system_hal_set_pwm0_clk_en(&s_system, 1);
	else
		system_hal_set_pwm0_clk_en(&s_system, 0);
}
uint32_t system_driver_get_pwm0_clk_en()
{
	return system_hal_get_pwm0_clk_en(&s_system);
}

void system_driver_set_timer0_clk_en(bool value)
{
	if(value)
		system_hal_set_timer0_clk_en(&s_system, 1);
	else
		system_hal_set_timer0_clk_en(&s_system, 0);
}
uint32_t system_driver_get_timer0_clk_en()
{
	return system_hal_get_timer0_clk_en(&s_system);
}

void system_driver_set_sadc_clk_en(bool value)
{
	if(value)
		system_hal_set_sadc_clk_en(&s_system, 1);
	else
		system_hal_set_sadc_clk_en(&s_system, 0);
}
uint32_t system_driver_get_sadc_clk_en()
{
	return system_hal_get_sadc_clk_en(&s_system);
}

void system_driver_set_irda_clk_en(bool value)
{
	if(value)
		system_hal_set_irda_clk_en(&s_system, 1);
	else
		system_hal_set_irda_clk_en(&s_system, 0);
}
uint32_t system_driver_get_irda_clk_en()
{
	return system_hal_get_irda_clk_en(&s_system);
}
void system_driver_set_efuse_clk_en(bool value)
{
	if(value)
		system_hal_set_efuse_clk_en(&s_system, 1);
	else
		system_hal_set_efuse_clk_en(&s_system, 0);
}
uint32_t system_driver_get_efuse_clk_en()
{
	return system_hal_get_efuse_clk_en(&s_system);
}

void system_driver_set_i2c1_clk_en(bool value)
{
	if(value)
		system_hal_set_i2c1_clk_en(&s_system, 1);
	else
		system_hal_set_i2c1_clk_en(&s_system, 0);
}
uint32_t system_driver_get_i2c1_clk_en()
{
	return system_hal_get_i2c1_clk_en(&s_system);
}

void system_driver_set_spi1_clk_en(bool value)
{
	if(value)
		system_hal_set_spi1_clk_en(&s_system, 1);
	else
		system_hal_set_spi1_clk_en(&s_system, 0);
}
uint32_t system_driver_get_spi1_clk_en()
{
	return system_hal_get_spi1_clk_en(&s_system);
}

void system_driver_set_uart1_clk_en(bool value)
{
	if(value)
		system_hal_set_uart1_clk_en(&s_system, 1);
	else
		system_hal_set_uart1_clk_en(&s_system, 0);
}
uint32_t system_driver_get_uart1_clk_en()
{
	return system_hal_get_uart1_clk_en(&s_system);
}

void system_driver_set_uart2_clk_en(bool value)
{
	if(value)
		system_hal_set_uart2_clk_en(&s_system, 1);
	else
		system_hal_set_uart2_clk_en(&s_system, 0);
}
uint32_t system_driver_get_uart2_clk_en()
{
	return system_hal_get_uart2_clk_en(&s_system);
}
void system_driver_set_pwm1_clk_en(bool value)
{
	if(value)
		system_hal_set_pwm1_clk_en(&s_system, 1);
	else
		system_hal_set_pwm1_clk_en(&s_system, 0);
}
uint32_t system_driver_get_pwm1_clk_en()
{
	return system_hal_get_pwm1_clk_en(&s_system);
}

void system_driver_set_timer1_clk_en(bool value)
{
	if(value)
		system_hal_set_timer1_clk_en(&s_system, 1);
	else
		system_hal_set_timer1_clk_en(&s_system, 0);
}
uint32_t system_driver_get_timer1_clk_en()
{
	return system_hal_get_timer1_clk_en(&s_system);
}

void system_driver_set_timer2_clk_en(bool value)
{
	if(value)
		system_hal_set_timer2_clk_en(&s_system, 1);
	else
		system_hal_set_timer2_clk_en(&s_system, 0);
}
uint32_t system_driver_get_timer2_clk_en()
{
	return system_hal_get_timer2_clk_en(&s_system);
}

void system_driver_set_otp_clk_en(bool value)
{
	if(value)
		system_hal_set_otp_clk_en(&s_system, 1);
	else
		system_hal_set_otp_clk_en(&s_system, 0);
}
uint32_t system_driver_get_otp_clk_en()
{
	return system_hal_get_otp_clk_en(&s_system);
}

void system_driver_set_i2s_clk_en(bool value)
{
	if(value)
		system_hal_set_i2s_clk_en(&s_system, 1);
	else
		system_hal_set_i2s_clk_en(&s_system, 0);
}
uint32_t system_driver_get_i2s_clk_en()
{
	return system_hal_get_i2s_clk_en(&s_system);
}
void system_driver_set_usb_clk_en(bool value)
{
	if(value)
		system_hal_set_usb_clk_en(&s_system, 1);
	else
		system_hal_set_usb_clk_en(&s_system, 0);
}
uint32_t system_driver_get_usb_clk_en()
{
	return system_hal_get_usb_clk_en(&s_system);
}

void system_driver_set_can_clk_en(bool value)
{
	if(value)
		system_hal_set_can_clk_en(&s_system, 1);
	else
		system_hal_set_can_clk_en(&s_system, 0);
}
uint32_t system_driver_get_can_clk_en()
{
	return system_hal_get_can_clk_en(&s_system);
}

void system_driver_set_psram_clk_en(bool value)
{
	if(value)
		system_hal_set_psram_clk_en(&s_system, 1);
	else
		system_hal_set_psram_clk_en(&s_system, 0);
}
uint32_t system_driver_get_psram_clk_en()
{
	return system_hal_get_psram_clk_en(&s_system);
}

void system_driver_set_qspi0_clk_en(bool value)
{
	if(value)
		system_hal_set_qspi0_clk_en(&s_system, 1);
	else
		system_hal_set_qspi0_clk_en(&s_system, 0);
}
uint32_t system_driver_get_qspi0_clk_en()
{
	return system_hal_get_qspi0_clk_en(&s_system);
}

void system_driver_set_qspi1_clk_en(bool value)
{
	if(value)
		system_hal_set_qspi1_clk_en(&s_system, 1);
	else
		system_hal_set_qspi1_clk_en(&s_system, 0);
}
uint32_t system_driver_get_qspi1_clk_en()
{
	return system_hal_get_qspi1_clk_en(&s_system);
}
void system_driver_set_sdio_clk_en(bool value)
{
	if(value)
		system_hal_set_sdio_clk_en(&s_system, 1);
	else
		system_hal_set_sdio_clk_en(&s_system, 0);
}
uint32_t system_driver_get_sdio_clk_en()
{
	return system_hal_get_sdio_clk_en(&s_system);
}

void system_driver_set_auxs_clk_en(bool value)
{
	if(value)
		system_hal_set_auxs_clk_en(&s_system, 1);
	else
		system_hal_set_auxs_clk_en(&s_system, 0);
}
uint32_t system_driver_get_auxs_clk_en()
{
	return system_hal_get_auxs_clk_en(&s_system);
}
#endif

#if 0
void system_driver_set_btdm_clk_en(bool value)
{
	if(value)
		sys_hal_set_btdm_clk_en(1);
	else
		sys_hal_set_btdm_clk_en(0);
}

uint32_t system_driver_get_btdm_clk_en()
{
	return sys_hal_get_btdm_clk_en();
}
#endif

#if 0
void system_driver_set_xvr_clk_en(bool value)
{
	if(value)
		sys_hal_set_xvr_clk_en(1);
	else
		sys_hal_set_xvr_clk_en(0);
}
uint32_t system_driver_get_xvr_clk_en()
{
	return sys_hal_get_xvr_clk_en();
}
#endif

#if 0
void system_driver_set_mac_clk_en(bool value)
{
	if(value)
		system_hal_set_mac_clk_en(&s_system, 1);
	else
		system_hal_set_mac_clk_en(&s_system, 0);
}
uint32_t system_driver_get_mac_clk_en()
{
	return system_hal_get_mac_clk_en(&s_system);
}

void system_driver_set_phy_clk_en(bool value)
{
	if(value)
		system_hal_set_phy_clk_en(&s_system, 1);
	else
		system_hal_set_phy_clk_en(&s_system, 0);
}
uint32_t system_driver_get_phy_clk_en()
{
	return system_hal_get_phy_clk_en(&s_system);
}

void system_driver_set_jpeg_clk_en(bool value)
{
	if(value)
		system_hal_set_jpeg_clk_en(&s_system, 1);
	else
		system_hal_set_jpeg_clk_en(&s_system, 0);
}
uint32_t system_driver_get_jpeg_clk_en()
{
	return system_hal_get_jpeg_clk_en(&s_system);
}

void system_driver_set_disp_clk_en(bool value)
{
	if(value)
		system_hal_set_disp_clk_en(&s_system, 1);
	else
		system_hal_set_disp_clk_en(&s_system, 0);
}
uint32_t system_driver_get_disp_clk_en()
{
	return system_hal_get_disp_clk_en(&s_system);
}

void system_driver_set_aud_clk_en(bool value)
{
	if(value)
		system_hal_set_aud_clk_en(&s_system, 1);
	else
		system_hal_set_aud_clk_en(&s_system, 0);
}
uint32_t system_driver_get_aud_clk_en()
{
	return system_hal_get_aud_clk_en(&s_system);
}

void system_driver_set_wdt_clk_en(bool value)
{
	if(value)
		system_hal_set_wdt_clk_en(&s_system, 1);
	else
		system_hal_set_wdt_clk_en(&s_system, 0);
}
uint32_t system_driver_get_wdt_clk_en()
{
	return system_hal_get_wdt_clk_en(&s_system);
}


//0x0e
void system_driver_set_aon_disckg(bool value)
{
	if(value)
		system_hal_set_aon_disckg(&s_system, 1);
	else
		system_hal_set_aon_disckg(&s_system, 0);
}
uint32_t system_driver_get_aon_disckg()
{
	return system_hal_get_aon_disckg(&s_system);
}

void system_driver_set_sys_disckg(bool value)
{
	if(value)
		system_hal_set_sys_disckg(&s_system, 1);
	else
		system_hal_set_sys_disckg(&s_system, 0);
}
uint32_t system_driver_get_sys_disckg()
{
	return system_hal_get_sys_disckg(&s_system);
}

void system_driver_set_dma_disckg(bool value)
{
	if(value)
		system_hal_set_dma_disckg(&s_system, 1);
	else
		system_hal_set_dma_disckg(&s_system, 0);
}
uint32_t system_driver_get_dma_disckg()
{
	return system_hal_get_dma_disckg(&s_system);
}

void system_driver_set_flash_disckg(bool value)
{
	if(value)
		system_hal_set_flash_disckg(&s_system, 1);
	else
		system_hal_set_flash_disckg(&s_system, 0);
}
uint32_t system_driver_get_flash_disckg()
{
	return system_hal_get_flash_disckg(&s_system);
}

void system_driver_set_wdt_disckg(bool value)
{
	if(value)
		system_hal_set_wdt_disckg(&s_system, 1);
	else
		system_hal_set_wdt_disckg(&s_system, 0);
}
uint32_t system_driver_get_wdt_disckg()
{
	return system_hal_get_wdt_disckg(&s_system);
}
void system_driver_set_timer0_disckg(bool value)
{
	if(value)
		system_hal_set_timer0_disckg(&s_system, 1);
	else
		system_hal_set_timer0_disckg(&s_system, 0);
}
uint32_t system_driver_get_timer0_disckg()
{
	return system_hal_get_timer0_disckg(&s_system);
}
void system_driver_set_uart0_disckg(bool value)
{
	if(value)
		system_hal_set_uart0_disckg(&s_system, 1);
	else
		system_hal_set_uart0_disckg(&s_system, 0);
}
uint32_t system_driver_get_uart0_disckg()
{
	return system_hal_get_uart0_disckg(&s_system);
}

void system_driver_set_pwm0_disckg(bool value)
{
	if(value)
		system_hal_set_pwm0_disckg(&s_system, 1);
	else
		system_hal_set_pwm0_disckg(&s_system, 0);
}
uint32_t system_driver_get_pwm0_disckg()
{
	return system_hal_get_pwm0_disckg(&s_system);
}

void system_driver_set_i2c0_disckg(bool value)
{
	if(value)
		system_hal_set_i2c0_disckg(&s_system, 1);
	else
		system_hal_set_i2c0_disckg(&s_system, 0);
}
uint32_t system_driver_get_i2c0_disckg()
{
	return system_hal_get_i2c0_disckg(&s_system);
}

void system_driver_set_spi0_disckg(bool value)
{
	if(value)
		system_hal_set_spi0_disckg(&s_system, 1);
	else
		system_hal_set_spi0_disckg(&s_system, 0);
}
uint32_t system_driver_get_spi0_disckg()
{
	return system_hal_get_spi0_disckg(&s_system);
}
void system_driver_set_sadc_disckg(bool value)
{
	if(value)
		system_hal_set_sadc_disckg(&s_system, 1);
	else
		system_hal_set_sadc_disckg(&s_system, 0);
}
uint32_t system_driver_get_sadc_disckg()
{
	return system_hal_get_sadc_disckg(&s_system);
}

void system_driver_set_efuse_disckg(bool value)
{
	if(value)
		system_hal_set_efuse_disckg(&s_system, 1);
	else
		system_hal_set_efuse_disckg(&s_system, 0);
}
uint32_t system_driver_get_efuse_disckg()
{
	return system_hal_get_efuse_disckg(&s_system);
}

void system_driver_set_irda_disckg(bool value)
{
	if(value)
		system_hal_set_irda_disckg(&s_system, 1);
	else
		system_hal_set_irda_disckg(&s_system, 0);
}
uint32_t system_driver_get_irda_disckg()
{
	return system_hal_get_irda_disckg(&s_system);
}

void system_driver_set_trng_disckg(bool value)
{
	if(value)
		system_hal_set_trng_disckg(&s_system, 1);
	else
		system_hal_set_trng_disckg(&s_system, 0);
}
uint32_t system_driver_get_trng_disckg()
{
	return system_hal_get_trng_disckg(&s_system);
}

void system_driver_set_sdio_disckg(bool value)
{
	if(value)
		system_hal_set_sdio_disckg(&s_system, 1);
	else
		system_hal_set_sdio_disckg(&s_system, 0);
}
uint32_t system_driver_get_sdio_disckg()
{
	return system_hal_get_sdio_disckg(&s_system);
}

void system_driver_set_la_disckg(bool value)
{
	if(value)
		system_hal_set_la_disckg(&s_system, 1);
	else
		system_hal_set_la_disckg(&s_system, 0);
}
uint32_t system_driver_get_la_disckg()
{
	return system_hal_get_la_disckg(&s_system);
}

void system_driver_set_timer1_disckg(bool value)
{
	if(value)
		system_hal_set_timer1_disckg(&s_system, 1);
	else
		system_hal_set_timer1_disckg(&s_system, 0);
}
uint32_t system_driver_get_timer1_disckg()
{
	return system_hal_get_timer1_disckg(&s_system);
}

void system_driver_set_uart1_disckg(bool value)
{
	if(value)
		system_hal_set_uart1_disckg(&s_system, 1);
	else
		system_hal_set_uart1_disckg(&s_system, 0);
}
uint32_t system_driver_get_uart1_disckg()
{
	return system_hal_get_uart1_disckg(&s_system);
}

void system_driver_set_uart2_disckg(bool value)
{
	if(value)
		system_hal_set_uart2_disckg(&s_system, 1);
	else
		system_hal_set_uart2_disckg(&s_system, 0);
}
uint32_t system_driver_get_uart2_disckg()
{
	return system_hal_get_uart2_disckg(&s_system);
}

void system_driver_set_pwm1_disckg(bool value)
{
	if(value)
		system_hal_set_pwm1_disckg(&s_system, 1);
	else
		system_hal_set_pwm1_disckg(&s_system, 0);
}
uint32_t system_driver_get_pwm1_disckg()
{
	return system_hal_get_pwm1_disckg(&s_system);
}

void system_driver_set_i2c1_disckg(bool value)
{
	if(value)
		system_hal_set_i2c1_disckg(&s_system, 1);
	else
		system_hal_set_i2c1_disckg(&s_system, 0);
}
uint32_t system_driver_get_i2c1_disckg()
{
	return system_hal_get_i2c1_disckg(&s_system);
}

void system_driver_set_spi1_disckg(bool value)
{
	if(value)
		system_hal_set_spi1_disckg(&s_system, 1);
	else
		system_hal_set_spi1_disckg(&s_system, 0);
}
uint32_t system_driver_get_spi1_disckg()
{
	return system_hal_get_spi1_disckg(&s_system);
}

void system_driver_set_usb_disckg(bool value)
{
	if(value)
		system_hal_set_usb_disckg(&s_system, 1);
	else
		system_hal_set_usb_disckg(&s_system, 0);
}
uint32_t system_driver_get_usb_disckg()
{
	return system_hal_get_usb_disckg(&s_system);
}

void system_driver_set_can_disckg(bool value)
{
	if(value)
		system_hal_set_can_disckg(&s_system, 1);
	else
		system_hal_set_can_disckg(&s_system, 0);
}
uint32_t system_driver_get_can_disckg()
{
	return system_hal_get_can_disckg(&s_system);
}

void system_driver_set_qspi0_disckg(bool value)
{
	if(value)
		system_hal_set_qspi0_disckg(&s_system, 1);
	else
		system_hal_set_qspi0_disckg(&s_system, 0);
}
uint32_t system_driver_get_qspi0_disckg()
{
	return system_hal_get_qspi0_disckg(&s_system);
}

void system_driver_set_psram_disckg(bool value)
{
	if(value)
		system_hal_set_psram_disckg(&s_system, 1);
	else
		system_hal_set_psram_disckg(&s_system, 0);
}
uint32_t system_driver_get_psram_disckg()
{
	return system_hal_get_psram_disckg(&s_system);
}

void system_driver_set_fft_disckg(bool value)
{
	if(value)
		system_hal_set_fft_disckg(&s_system, 1);
	else
		system_hal_set_fft_disckg(&s_system, 0);
}
uint32_t system_driver_get_fft_disckg()
{
	return system_hal_get_fft_disckg(&s_system);
}

void system_driver_set_sbc_disckg(bool value)
{
	if(value)
		system_hal_set_sbc_disckg(&s_system, 1);
	else
		system_hal_set_sbc_disckg(&s_system, 0);
}
uint32_t system_driver_get_sbc_disckg()
{
	return system_hal_get_sbc_disckg(&s_system);
}

void system_driver_set_aud_disckg(bool value)
{
	if(value)
		system_hal_set_aud_disckg(&s_system, 1);
	else
		system_hal_set_aud_disckg(&s_system, 0);
}
uint32_t system_driver_get_aud_disckg()
{
	return system_hal_get_aud_disckg(&s_system);
}

void system_driver_set_i2s_disckg(bool value)
{
	if(value)
		system_hal_set_i2s_disckg(&s_system, 1);
	else
		system_hal_set_i2s_disckg(&s_system, 0);
}
uint32_t system_driver_get_i2s_disckg()
{
	return system_hal_get_i2s_disckg(&s_system);
}

void system_driver_set_jpeg_disckg(bool value)
{
	if(value)
		system_hal_set_jpeg_disckg(&s_system, 1);
	else
		system_hal_set_jpeg_disckg(&s_system, 0);
}
uint32_t system_driver_get_jpeg_disckg()
{
	return system_hal_get_jpeg_disckg(&s_system);
}

void system_driver_set_jpeg_dec_disckg(bool value)
{
	if(value)
		system_hal_set_jpeg_dec_disckg(&s_system, 1);
	else
		system_hal_set_jpeg_dec_disckg(&s_system, 0);
}
uint32_t system_driver_get_jpeg_dec_disckg()
{
	return system_hal_get_jpeg_dec_disckg(&s_system);
}

//0xff
void system_driver_set_disp_disckg(bool value)
{
	if(value)
		system_hal_set_disp_disckg(&s_system, 1);
	else
		system_hal_set_disp_disckg(&s_system, 0);
}
uint32_t system_driver_get_disp_disckg()
{
	return system_hal_get_disp_disckg(&s_system);
}

void system_driver_set_dma2d_disckg(bool value)
{
	if(value)
		system_hal_set_dma2d_disckg(&s_system, 1);
	else
		system_hal_set_dma2d_disckg(&s_system, 0);
}
uint32_t system_driver_get_dma2d_disckg()
{
	return system_hal_get_dma2d_disckg(&s_system);
}

void system_driver_set_btdm_disckg(bool value)
{
	if(value)
		system_hal_set_btdm_disckg(&s_system, 1);
	else
		system_hal_set_btdm_disckg(&s_system, 0);
}
uint32_t system_driver_get_btdm_disckg()
{
	return system_hal_get_btdm_disckg(&s_system);
}

void system_driver_set_xver_disckg(bool value)
{
	if(value)
		system_hal_set_xver_disckg(&s_system, 1);
	else
		system_hal_set_xver_disckg(&s_system, 0);
}
uint32_t system_driver_get_xver_disckg()
{
	return system_hal_get_xver_disckg(&s_system);
}

void system_driver_set_btdm_bps_disckg(bool value)
{
	if(value)
		system_hal_set_btdm_bps_disckg(&s_system, 1);
	else
		system_hal_set_btdm_bps_disckg(&s_system, 0);
}
uint32_t system_driver_get_btdm_bps_disckg()
{
	return system_hal_get_btdm_bps_disckg(&s_system);
}


//0x10
void system_driver_set_power_on_mem1(bool value)
{
	if(value)
		system_hal_set_power_on_mem1(&s_system, 1);
	else
		system_hal_set_power_on_mem1(&s_system, 0);
}
uint32_t system_driver_get_power_on_mem1()
{
	return system_hal_get_power_on_mem1(&s_system);
}
void system_driver_set_power_on_mem2(bool value)
{
	if(value)
		system_hal_set_power_on_mem2(&s_system, 1);
	else
		system_hal_set_power_on_mem2(&s_system, 0);
}
uint32_t system_driver_get_power_on_mem2()
{
	return system_hal_get_power_on_mem2(&s_system);
}
void system_driver_set_power_on_mem3(bool value)
{
	if(value)
		system_hal_set_power_on_mem3(&s_system, 1);
	else
		system_hal_set_power_on_mem3(&s_system, 0);
}
uint32_t system_driver_get_power_on_mem3()
{
	return system_hal_get_power_on_mem3(&s_system);
}

void system_driver_set_power_on_encp(bool value)
{
	if(value)
		system_hal_set_power_on_encp(&s_system, 1);
	else
		system_hal_set_power_on_encp(&s_system, 0);
}
uint32_t system_driver_get_power_on_encp()
{
	return system_hal_get_power_on_encp(&s_system);
}
void system_driver_set_power_on_bakp(bool value)
{
	if(value)
		system_hal_set_power_on_bakp(&s_system, 1);
	else
		system_hal_set_power_on_bakp(&s_system, 0);
}
uint32_t system_driver_get_power_on_bakp()
{
	return system_hal_get_power_on_bakp(&s_system);
}
void system_driver_set_power_on_ahbp(bool value)
{
	if(value)
		system_hal_set_power_on_ahbp(&s_system, 1);
	else
		system_hal_set_power_on_ahbp(&s_system, 0);
}
uint32_t system_driver_get_power_on_ahbp()
{
	return system_hal_get_power_on_ahbp(&s_system);
}

void system_driver_set_power_on_audp(bool value)
{
	if(value)
		system_hal_set_power_on_audp(&s_system, 1);
	else
		system_hal_set_power_on_audp(&s_system, 0);
}
uint32_t system_driver_get_power_on_audp()
{
	return system_hal_get_power_on_audp(&s_system);
}

void system_driver_set_power_on_vidp(bool value)
{
	if(value)
		system_hal_set_power_on_vidp(&s_system, 1);
	else
		system_hal_set_power_on_vidp(&s_system, 0);
}
uint32_t system_driver_get_power_on_vidp()
{
	return system_hal_get_power_on_vidp(&s_system);
}
#endif

#if 0
void system_driver_set_power_on_btsp(bool value)
{
	if(value)
		sys_hal_set_power_on_btsp(1);
	else
		sys_hal_set_power_on_btsp(0);
}
uint32_t system_driver_get_power_on_btsp()
{
	return sys_hal_get_power_on_btsp();
}
#endif

#if 0
void system_driver_set_power_on_wifi_mac(bool value)
{
	if(value)
		system_hal_set_power_on_wifi_mac(&s_system, 1);
	else
		system_hal_set_power_on_wifi_mac(&s_system, 0);
}
uint32_t system_driver_get_power_on_wifi_mac()
{
	return system_hal_get_power_on_wifi_mac(&s_system);
}

void system_driver_set_power_on_wifi_phy(bool value)
{
	if(value)
		system_hal_set_power_on_wifi_phy(&s_system, 1);
	else
		system_hal_set_power_on_wifi_phy(&s_system, 0);
}
uint32_t system_driver_get_power_on_wifi_phy()
{
	return system_hal_get_power_on_wifi_phy(&s_system);
}

void system_driver_set_sleep_en_need_flash_idle(bool value)
{
	if(value)
		system_hal_set_sleep_en_need_flash_idle(&s_system, 1);
	else
		system_hal_set_sleep_en_need_flash_idle(&s_system, 0);
}
uint32_t system_driver_get_sleep_en_need_flash_idle()
{
	return system_hal_get_sleep_en_need_flash_idle(&s_system);
}

void system_driver_set_sleep_en_need_cpu1_wifi(bool value)
{
	if(value)
		system_hal_set_sleep_en_need_cpu1_wifi(&s_system, 1);
	else
		system_hal_set_sleep_en_need_cpu1_wifi(&s_system, 0);
}
uint32_t system_driver_get_sleep_en_need_cpu1_wifi()
{
	return system_hal_get_sleep_en_need_cpu1_wifi(&s_system);
}

void system_driver_set_sleep_en_need_cpu0_wifi(bool value)
{
	if(value)
		system_hal_set_sleep_en_need_cpu0_wifi(&s_system, 1);
	else
		system_hal_set_sleep_en_need_cpu0_wifi(&s_system, 0);
}
uint32_t system_driver_get_sleep_en_need_cpu0_wifi()
{
	return system_hal_get_sleep_en_need_cpu0_wifi(&s_system);
}

void system_driver_set_sleep_en_need_global(bool value)
{
	if(value)
		system_hal_set_sleep_en_need_global(&s_system, 1);
	else
		system_hal_set_sleep_en_need_global(&s_system, 0);
}
uint32_t system_driver_get_sleep_en_need_global()
{
	return system_hal_get_sleep_en_need_global(&s_system);
}

void system_driver_set_wifi_wakeup_platform_en(bool value)
{
	if(value)
		system_hal_set_wifi_wakeup_platform_en(&s_system, 1);
	else
		system_hal_set_wifi_wakeup_platform_en(&s_system, 0);
}
uint32_t system_driver_get_wifi_wakeup_platform_en()
{
	return system_hal_get_wifi_wakeup_platform_en(&s_system);
}
#endif

#if 1
void system_driver_set_bts_wakeup_platform_en(bool value)
{
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();

	if(value)
		sys_hal_set_bts_wakeup_platform_en(1);
	else
		sys_hal_set_bts_wakeup_platform_en(0);

	if(!ret)
		ret = sys_amp_res_release();

}
uint32_t system_driver_get_bts_wakeup_platform_en()
{
	return sys_hal_get_bts_wakeup_platform_en();
}

void system_driver_set_bts_sleep_exit_req(bool value)
{
	if(value)
		sys_hal_set_bts_sleep_exit_req(1);
	else
		sys_hal_set_bts_sleep_exit_req(0);
}
uint32_t system_driver_get_bts_sleep_exit_req()
{
	return sys_hal_get_bts_sleep_exit_req();
}
#endif

#if 0

//0x20
void system_driver_set_cpu0_bmc32_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_bmc32_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_bmc32_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_bmc32_int_en()
{
	return system_hal_get_cpu0_bmc32_int_en(&s_system);
}

void system_driver_set_cpu0_host_0_irq_en(bool value)
{
	if(value)
		system_hal_set_cpu0_host_0_irq_en(&s_system, 1);
	else
		system_hal_set_cpu0_host_0_irq_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_host_0_irq_en()
{
	return system_hal_get_cpu0_host_0_irq_en(&s_system);
}

void system_driver_set_cpu0_host_0_sec_irq_en(bool value)
{
	if(value)
		system_hal_set_cpu0_host_0_sec_irq_en(&s_system, 1);
	else
		system_hal_set_cpu0_host_0_sec_irq_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_host_0_sec_irq_en()
{
	return system_hal_get_cpu0_host_0_sec_irq_en(&s_system);
}

void system_driver_set_cpu0_timer_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_timer_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_timer_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_timer_int_en()
{
	return system_hal_get_cpu0_timer_int_en(&s_system);
}

void system_driver_set_cpu0_uart_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_uart_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_uart_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_uart_int_en()
{
	return system_hal_get_cpu0_uart_int_en(&s_system);
}

void system_driver_set_cpu0_pwm_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_pwm_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_pwm_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_pwm_int_en()
{
	return system_hal_get_cpu0_pwm_int_en(&s_system);
}

void system_driver_set_cpu0_i2c_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_i2c_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_i2c_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_i2c_int_en()
{
	return system_hal_get_cpu0_i2c_int_en(&s_system);
}

void system_driver_set_cpu0_spi_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_spi_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_spi_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_spi_int_en()
{
	return system_hal_get_cpu0_spi_int_en(&s_system);
}

void system_driver_set_cpu0_sadc_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_sadc_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_sadc_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_sadc_int_en()
{
	return system_hal_get_cpu0_sadc_int_en(&s_system);
}

void system_driver_set_cpu0_irda_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_irda_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_irda_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_irda_int_en()
{
	return system_hal_get_cpu0_irda_int_en(&s_system);
}

void system_driver_set_cpu0_sdio_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_sdio_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_sdio_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_sdio_int_en()
{
	return system_hal_get_cpu0_sdio_int_en(&s_system);
}

void system_driver_set_cpu0_gdma_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_gdma_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_gdma_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_gdma_int_en()
{
	return system_hal_get_cpu0_gdma_int_en(&s_system);
}

void system_driver_set_cpu0_la_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_la_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_la_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_la_int_en()
{
	return system_hal_get_cpu0_la_int_en(&s_system);
}

void system_driver_set_cpu0_timer1_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_timer1_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_timer1_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_timer1_int_en()
{
	return system_hal_get_cpu0_timer1_int_en(&s_system);
}

void system_driver_set_cpu0_i2c1_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_i2c1_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_i2c1_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_i2c1_int_en()
{
	return system_hal_get_cpu0_i2c1_int_en(&s_system);
}

void system_driver_set_cpu0_uart1_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_uart1_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_uart1_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_uart1_int_en()
{
	return system_hal_get_cpu0_uart1_int_en(&s_system);
}


void system_driver_set_cpu0_uart2_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_uart2_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_uart2_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_uart2_int_en()
{
	return system_hal_get_cpu0_uart2_int_en(&s_system);
}

void system_driver_set_cpu0_spi1_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_spi1_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_spi1_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_spi1_int_en()
{
	return system_hal_get_cpu0_spi1_int_en(&s_system);
}

void system_driver_set_cpu0_can_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_can_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_can_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_can_int_en()
{
	return system_hal_get_cpu0_can_int_en(&s_system);
}

void system_driver_set_cpu0_usb_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_usb_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_usb_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_usb_int_en()
{
	return system_hal_get_cpu0_usb_int_en(&s_system);
}

void system_driver_set_cpu0_qspi_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_qspi_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_qspi_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_qspi_int_en()
{
	return system_hal_get_cpu0_qspi_int_en(&s_system);
}

void system_driver_set_cpu0_fft_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_fft_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_fft_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_fft_int_en()
{
	return system_hal_get_cpu0_fft_int_en(&s_system);
}

void system_driver_set_cpu0_sbc_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_sbc_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_sbc_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_sbc_int_en()
{
	return system_hal_get_cpu0_sbc_int_en(&s_system);
}

void system_driver_set_cpu0_aud_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_aud_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_aud_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_aud_int_en()
{
	return system_hal_get_cpu0_aud_int_en(&s_system);
}

void system_driver_set_cpu0_i2s_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_i2s_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_i2s_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_i2s_int_en()
{
	return system_hal_get_cpu0_i2s_int_en(&s_system);
}

void system_driver_set_cpu0_jpegenc_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_jpegenc_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_jpegenc_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_jpegenc_int_en()
{
	return system_hal_get_cpu0_jpegenc_int_en(&s_system);
}

void system_driver_set_cpu0_jpegdec_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_jpegdec_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_jpegdec_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_jpegdec_int_en()
{
	return system_hal_get_cpu0_jpegdec_int_en(&s_system);
}

void system_driver_set_cpu0_lcd_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_lcd_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_lcd_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_lcd_int_en()
{
	return system_hal_get_cpu0_lcd_int_en(&s_system);
}


void system_driver_set_cpu0_wifi_int_phy_en(bool value)
{
	if(value)
		system_hal_set_cpu0_wifi_int_phy_en(&s_system, 1);
	else
		system_hal_set_cpu0_wifi_int_phy_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_wifi_int_phy_en()
{
	return system_hal_get_cpu0_wifi_int_phy_en(&s_system);
}


void system_driver_set_cpu0_wifi_mac_int_timer_en(bool value)
{
	if(value)
		system_hal_set_cpu0_wifi_mac_int_timer_en(&s_system, 1);
	else
		system_hal_set_cpu0_wifi_mac_int_timer_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_wifi_mac_int_timer_en()
{
	return system_hal_get_cpu0_wifi_mac_int_timer_en(&s_system);
}

void system_driver_set_cpu0_wifi_mac_int_misc_en(bool value)
{
	if(value)
		system_hal_set_cpu0_wifi_mac_int_misc_en(&s_system, 1);
	else
		system_hal_set_cpu0_wifi_mac_int_misc_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_wifi_mac_int_misc_en()
{
	return system_hal_get_cpu0_wifi_mac_int_misc_en(&s_system);
}


//0x21
void system_driver_set_cpu0_wifi_mac_int_rx_trigger_en(bool value)
{
	if(value)
		system_hal_set_cpu0_wifi_mac_int_rx_trigger_en(&s_system, 1);
	else
		system_hal_set_cpu0_wifi_mac_int_rx_trigger_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_wifi_mac_int_rx_trigger_en()
{
	return system_hal_get_cpu0_wifi_mac_int_rx_trigger_en(&s_system);
}
void system_driver_set_cpu0_wifi_mac_int_tx_trigger_en(bool value)
{
	if(value)
		system_hal_set_cpu0_wifi_mac_int_tx_trigger_en(&s_system, 1);
	else
		system_hal_set_cpu0_wifi_mac_int_tx_trigger_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_wifi_mac_int_tx_trigger_en()
{
	return system_hal_get_cpu0_wifi_mac_int_tx_trigger_en(&s_system);
}

void system_driver_set_cpu0_wifi_mac_port_trigger_en(bool value)
{
	if(value)
		system_hal_set_cpu0_wifi_mac_port_trigger_en(&s_system, 1);
	else
		system_hal_set_cpu0_wifi_mac_port_trigger_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_wifi_mac_port_trigger_en()
{
	return system_hal_get_cpu0_wifi_mac_port_trigger_en(&s_system);
}

void system_driver_set_cpu0_wifi_mac_int_gen_en(bool value)
{
	if(value)
		system_hal_set_cpu0_wifi_mac_int_gen_en(&s_system, 1);
	else
		system_hal_set_cpu0_wifi_mac_int_gen_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_wifi_mac_int_gen_en()
{
	return system_hal_get_cpu0_wifi_mac_int_gen_en(&s_system);
}

void system_driver_set_cpu0_wifi_hsu_irq_en(bool value)
{
	if(value)
		system_hal_set_cpu0_wifi_hsu_irq_en(&s_system, 1);
	else
		system_hal_set_cpu0_wifi_hsu_irq_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_wifi_hsu_irq_en()
{
	return system_hal_get_cpu0_wifi_hsu_irq_en(&s_system);
}
void system_driver_set_cpu0_wifi_int_mac_wakeup_en(bool value)
{
	if(value)
		system_hal_set_cpu0_wifi_int_mac_wakeup_en(&s_system, 1);
	else
		system_hal_set_cpu0_wifi_int_mac_wakeup_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_wifi_int_mac_wakeup_en()
{
	return system_hal_get_cpu0_wifi_int_mac_wakeup_en(&s_system);
}
#endif

#if 1
void system_driver_set_cpu0_dm_irq_en(bool value)
{
	if(value)
		sys_hal_set_cpu0_dm_irq_en(1);
	else
		sys_hal_set_cpu0_dm_irq_en(0);
}
uint32_t system_driver_get_cpu0_dm_irq_en()
{
	return sys_hal_get_cpu0_dm_irq_en();
}
#endif

void system_driver_set_cpu0_ble_irq_en(bool value)
{
	if(value)
		sys_hal_set_cpu0_ble_irq_en(1);
	else
		sys_hal_set_cpu0_ble_irq_en(0);
}
uint32_t system_driver_get_cpu0_ble_irq_en()
{
	return sys_hal_get_cpu0_ble_irq_en();
}

void system_driver_set_cpu0_bt_irq_en(bool value)
{
	//TODO
}
uint32_t system_driver_get_cpu0_bt_irq_en()
{
	//TODO
	return 0;
}

#if 0
void system_driver_set_cpu0_mbox0_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_mbox0_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_mbox0_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_mbox0_int_en()
{
	return system_hal_get_cpu0_mbox0_int_en(&s_system);
}

void system_driver_set_cpu0_mbox1_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_mbox1_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_mbox1_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_mbox1_int_en()
{
	return system_hal_get_cpu0_mbox1_int_en(&s_system);
}
void system_driver_set_cpu0_bmc64_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_bmc64_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_bmc64_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_bmc64_int_en()
{
	return system_hal_get_cpu0_bmc64_int_en(&s_system);
}
void system_driver_set_cpu0_touched_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_touched_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_touched_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_touched_int_en()
{
	return system_hal_get_cpu0_touched_int_en(&s_system);
}

void system_driver_set_cpu0_usbplug_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_usbplug_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_usbplug_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_usbplug_int_en()
{
	return system_hal_get_cpu0_usbplug_int_en(&s_system);
}

void system_driver_set_cpu0_rtc_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_rtc_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_rtc_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_rtc_int_en()
{
	return system_hal_get_cpu0_rtc_int_en(&s_system);
}

void system_driver_set_cpu0_gpio_int_en(bool value)
{
	if(value)
		system_hal_set_cpu0_gpio_int_en(&s_system, 1);
	else
		system_hal_set_cpu0_gpio_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu0_gpio_int_en()
{
	return system_hal_get_cpu0_gpio_int_en(&s_system);
}


//0x22
void system_driver_set_cpu1_bmc32_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_bmc32_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_bmc32_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_bmc32_int_en()
{
	return system_hal_get_cpu1_bmc32_int_en(&s_system);
}

void system_driver_set_cpu1_host_0_irq_en(bool value)
{
	if(value)
		system_hal_set_cpu1_host_0_irq_en(&s_system, 1);
	else
		system_hal_set_cpu1_host_0_irq_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_host_0_irq_en()
{
	return system_hal_get_cpu1_host_0_irq_en(&s_system);
}

void system_driver_set_cpu1_host_0_sec_irq_en(bool value)
{
	if(value)
		system_hal_set_cpu1_host_0_sec_irq_en(&s_system, 1);
	else
		system_hal_set_cpu1_host_0_sec_irq_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_host_0_sec_irq_en()
{
	return system_hal_get_cpu1_host_0_sec_irq_en(&s_system);
}

void system_driver_set_cpu1_timer_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_timer_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_timer_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_timer_int_en()
{
	return system_hal_get_cpu1_timer_int_en(&s_system);
}

void system_driver_set_cpu1_uart_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_uart_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_uart_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_uart_int_en()
{
	return system_hal_get_cpu1_uart_int_en(&s_system);
}

void system_driver_set_cpu1_pwm_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_pwm_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_pwm_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_pwm_int_en()
{
	return system_hal_get_cpu1_pwm_int_en(&s_system);
}

void system_driver_set_cpu1_i2c_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_i2c_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_i2c_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_i2c_int_en()
{
	return system_hal_get_cpu1_i2c_int_en(&s_system);
}

void system_driver_set_cpu1_spi_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_spi_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_spi_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_spi_int_en()
{
	return system_hal_get_cpu1_spi_int_en(&s_system);
}

void system_driver_set_cpu1_sadc_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_sadc_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_sadc_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_sadc_int_en()
{
	return system_hal_get_cpu1_sadc_int_en(&s_system);
}

void system_driver_set_cpu1_irda_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_irda_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_irda_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_irda_int_en()
{
	return system_hal_get_cpu1_irda_int_en(&s_system);
}

void system_driver_set_cpu1_sdio_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_sdio_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_sdio_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_sdio_int_en()
{
	return system_hal_get_cpu1_sdio_int_en(&s_system);
}

void system_driver_set_cpu1_gdma_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_gdma_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_gdma_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_gdma_int_en()
{
	return system_hal_get_cpu1_gdma_int_en(&s_system);
}

void system_driver_set_cpu1_la_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_la_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_la_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_la_int_en()
{
	return system_hal_get_cpu1_la_int_en(&s_system);
}

void system_driver_set_cpu1_timer1_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_timer1_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_timer1_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_timer1_int_en()
{
	return system_hal_get_cpu1_timer1_int_en(&s_system);
}

void system_driver_set_cpu1_i2c1_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_i2c1_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_i2c1_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_i2c1_int_en()
{
	return system_hal_get_cpu1_i2c1_int_en(&s_system);
}

void system_driver_set_cpu1_uart1_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_uart1_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_uart1_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_uart1_int_en()
{
	return system_hal_get_cpu1_uart1_int_en(&s_system);
}


void system_driver_set_cpu1_uart2_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_uart2_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_uart2_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_uart2_int_en()
{
	return system_hal_get_cpu1_uart2_int_en(&s_system);
}

void system_driver_set_cpu1_spi1_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_spi1_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_spi1_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_spi1_int_en()
{
	return system_hal_get_cpu1_spi1_int_en(&s_system);
}

void system_driver_set_cpu1_can_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_can_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_can_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_can_int_en()
{
	return system_hal_get_cpu1_can_int_en(&s_system);
}

void system_driver_set_cpu1_usb_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_usb_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_usb_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_usb_int_en()
{
	return system_hal_get_cpu1_usb_int_en(&s_system);
}

void system_driver_set_cpu1_qspi_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_qspi_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_qspi_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_qspi_int_en()
{
	return system_hal_get_cpu1_qspi_int_en(&s_system);
}

void system_driver_set_cpu1_fft_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_fft_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_fft_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_fft_int_en()
{
	return system_hal_get_cpu1_fft_int_en(&s_system);
}

void system_driver_set_cpu1_sbc_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_sbc_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_sbc_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_sbc_int_en()
{
	return system_hal_get_cpu1_sbc_int_en(&s_system);
}

void system_driver_set_cpu1_aud_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_aud_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_aud_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_aud_int_en()
{
	return system_hal_get_cpu1_aud_int_en(&s_system);
}

void system_driver_set_cpu1_i2s_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_i2s_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_i2s_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_i2s_int_en()
{
	return system_hal_get_cpu1_i2s_int_en(&s_system);
}

void system_driver_set_cpu1_jpegenc_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_jpegenc_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_jpegenc_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_jpegenc_int_en()
{
	return system_hal_get_cpu1_jpegenc_int_en(&s_system);
}

void system_driver_set_cpu1_jpegdec_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_jpegdec_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_jpegdec_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_jpegdec_int_en()
{
	return system_hal_get_cpu1_jpegdec_int_en(&s_system);
}

void system_driver_set_cpu1_lcd_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_lcd_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_lcd_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_lcd_int_en()
{
	return system_hal_get_cpu1_lcd_int_en(&s_system);
}


void system_driver_set_cpu1_wifi_int_phy_en(bool value)
{
	if(value)
		system_hal_set_cpu1_wifi_int_phy_en(&s_system, 1);
	else
		system_hal_set_cpu1_wifi_int_phy_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_wifi_int_phy_en()
{
	return system_hal_get_cpu1_wifi_int_phy_en(&s_system);
}


void system_driver_set_cpu1_wifi_mac_int_timer_en(bool value)
{
	if(value)
		system_hal_set_cpu1_wifi_mac_int_timer_en(&s_system, 1);
	else
		system_hal_set_cpu1_wifi_mac_int_timer_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_wifi_mac_int_timer_en()
{
	return system_hal_get_cpu1_wifi_mac_int_timer_en(&s_system);
}

void system_driver_set_cpu1_wifi_mac_int_misc_en(bool value)
{
	if(value)
		system_hal_set_cpu1_wifi_mac_int_misc_en(&s_system, 1);
	else
		system_hal_set_cpu1_wifi_mac_int_misc_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_wifi_mac_int_misc_en()
{
	return system_hal_get_cpu1_wifi_mac_int_misc_en(&s_system);
}


//0x23
void system_driver_set_cpu1_wifi_mac_int_rx_trigger_en(bool value)
{
	if(value)
		system_hal_set_cpu1_wifi_mac_int_rx_trigger_en(&s_system, 1);
	else
		system_hal_set_cpu1_wifi_mac_int_rx_trigger_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_wifi_mac_int_rx_trigger_en()
{
	return system_hal_get_cpu1_wifi_mac_int_rx_trigger_en(&s_system);
}
void system_driver_set_cpu1_wifi_mac_int_tx_trigger_en(bool value)
{
	if(value)
		system_hal_set_cpu1_wifi_mac_int_tx_trigger_en(&s_system, 1);
	else
		system_hal_set_cpu1_wifi_mac_int_tx_trigger_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_wifi_mac_int_tx_trigger_en()
{
	return system_hal_get_cpu1_wifi_mac_int_tx_trigger_en(&s_system);
}

void system_driver_set_cpu1_wifi_mac_port_trigger_en(bool value)
{
	if(value)
		system_hal_set_cpu1_wifi_mac_port_trigger_en(&s_system, 1);
	else
		system_hal_set_cpu1_wifi_mac_port_trigger_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_wifi_mac_port_trigger_en()
{
	return system_hal_get_cpu1_wifi_mac_port_trigger_en(&s_system);
}

void system_driver_set_cpu1_wifi_mac_int_gen_en(bool value)
{
	if(value)
		system_hal_set_cpu1_wifi_mac_int_gen_en(&s_system, 1);
	else
		system_hal_set_cpu1_wifi_mac_int_gen_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_wifi_mac_int_gen_en()
{
	return system_hal_get_cpu1_wifi_mac_int_gen_en(&s_system);
}

void system_driver_set_cpu1_wifi_hsu_irq_en(bool value)
{
	if(value)
		system_hal_set_cpu1_wifi_hsu_irq_en(&s_system, 1);
	else
		system_hal_set_cpu1_wifi_hsu_irq_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_wifi_hsu_irq_en()
{
	return system_hal_get_cpu1_wifi_hsu_irq_en(&s_system);
}
void system_driver_set_cpu1_wifi_int_mac_wakeup_en(bool value)
{
	if(value)
		system_hal_set_cpu1_wifi_int_mac_wakeup_en(&s_system, 1);
	else
		system_hal_set_cpu1_wifi_int_mac_wakeup_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_wifi_int_mac_wakeup_en()
{
	return system_hal_get_cpu1_wifi_int_mac_wakeup_en(&s_system);
}

void system_driver_set_cpu1_dm_irq_en(bool value)
{
	if(value)
		system_hal_set_cpu1_dm_irq_en(&s_system, 1);
	else
		system_hal_set_cpu1_dm_irq_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_dm_irq_en()
{
	return system_hal_get_cpu1_dm_irq_en(&s_system);
}


void system_driver_set_cpu1_ble_irq_en(bool value)
{
	if(value)
		system_hal_set_cpu1_ble_irq_en(&s_system, 1);
	else
		system_hal_set_cpu1_ble_irq_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_ble_irq_en()
{
	return system_hal_get_cpu1_ble_irq_en(&s_system);
}

void system_driver_set_cpu1_bt_irq_en(bool value)
{
	if(value)
		system_hal_set_cpu1_bt_irq_en(&s_system, 1);
	else
		system_hal_set_cpu1_bt_irq_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_bt_irq_en()
{
	return system_hal_get_cpu1_bt_irq_en(&s_system);
}
void system_driver_set_cpu1_mbox0_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_mbox0_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_mbox0_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_mbox0_int_en()
{
	return system_hal_get_cpu1_mbox0_int_en(&s_system);
}

void system_driver_set_cpu1_mbox1_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_mbox1_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_mbox1_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_mbox1_int_en()
{
	return system_hal_get_cpu1_mbox1_int_en(&s_system);
}
void system_driver_set_cpu1_bmc64_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_bmc64_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_bmc64_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_bmc64_int_en()
{
	return system_hal_get_cpu1_bmc64_int_en(&s_system);
}
void system_driver_set_cpu1_touched_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_touched_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_touched_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_touched_int_en()
{
	return system_hal_get_cpu1_touched_int_en(&s_system);
}

void system_driver_set_cpu1_usbplug_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_usbplug_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_usbplug_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_usbplug_int_en()
{
	return system_hal_get_cpu1_usbplug_int_en(&s_system);
}

void system_driver_set_cpu1_rtc_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_rtc_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_rtc_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_rtc_int_en()
{
	return system_hal_get_cpu1_rtc_int_en(&s_system);
}

void system_driver_set_cpu1_gpio_int_en(bool value)
{
	if(value)
		system_hal_set_cpu1_gpio_int_en(&s_system, 1);
	else
		system_hal_set_cpu1_gpio_int_en(&s_system, 0);
}
uint32_t system_driver_get_cpu1_gpio_int_en()
{
	return system_hal_get_cpu1_gpio_int_en(&s_system);
}


//0x28
uint32_t system_driver_get_cpu0_bmc32_int_st()
{
	return system_hal_get_cpu0_bmc32_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_host_0_irq_st()
{
	return system_hal_get_cpu0_host_0_irq_st(&s_system);
}
uint32_t system_driver_get_cpu0_host_0_sec_irq_st()
{
	return system_hal_get_cpu0_host_0_sec_irq_st(&s_system);
}
uint32_t system_driver_get_cpu0_timer_int_st()
{
	return system_hal_get_cpu0_host_0_sec_irq_st(&s_system);
}
uint32_t system_driver_get_cpu0_uart_int_st()
{
	return system_hal_get_cpu0_uart_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_pwm_int_st()
{
	return system_hal_get_cpu0_pwm_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_i2c_int_st()
{
	return system_hal_get_cpu0_i2c_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_spi_int_st()
{
	return system_hal_get_cpu0_spi_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_sadc_int_st()
{
	return system_hal_get_cpu0_sadc_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_irda_int_st()
{
	return system_hal_get_cpu0_irda_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_sdio_int_st()
{
	return system_hal_get_cpu0_sdio_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_gdma_int_st()
{
	return system_hal_get_cpu0_gdma_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_la_int_st()
{
	return system_hal_get_cpu0_la_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_timer1_int_st()
{
	return system_hal_get_cpu0_timer1_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_i2c1_int_st()
{
	return system_hal_get_cpu0_i2c1_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_uart1_int_st()
{
	return system_hal_get_cpu0_uart1_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_uart2_int_st()
{
	return system_hal_get_cpu0_uart2_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_spi1_int_st()
{
	return system_hal_get_cpu0_spi1_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_can_int_st()
{
	return system_hal_get_cpu0_can_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_usb_int_st()
{
	return system_hal_get_cpu0_usb_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_qspi_int_st()
{
	return system_hal_get_cpu0_qspi_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_fft_int_st()
{
	return system_hal_get_cpu0_fft_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_sbc_int_st()
{
	return system_hal_get_cpu0_sbc_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_aud_int_st()
{
	return system_hal_get_cpu0_aud_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_i2s_int_st()
{
	return system_hal_get_cpu0_i2s_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_jpegenc_int_st()
{
	return system_hal_get_cpu0_jpegenc_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_jpegdec_int_st()
{
	return system_hal_get_cpu0_jpegdec_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_lcd_int_st()
{
	return system_hal_get_cpu0_lcd_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_wifi_int_phy_st()
{
	return system_hal_get_cpu0_wifi_int_phy_st(&s_system);
}
uint32_t system_driver_get_cpu0_wifi_mac_int_timer_st()
{
	return system_hal_get_cpu0_wifi_mac_int_timer_st(&s_system);
}
uint32_t system_driver_get_cpu0_wifi_mac_int_misc_st()
{
	return system_hal_get_cpu0_wifi_mac_int_misc_st(&s_system);
}

//0x29
uint32_t system_driver_get_cpu0_wifi_mac_int_rx_trigger_st()
{
	return system_hal_get_cpu0_wifi_mac_int_rx_trigger_st(&s_system);
}
uint32_t system_driver_get_cpu0_wifi_mac_int_tx_trigger_st()
{
	return system_hal_get_cpu0_wifi_mac_int_tx_trigger_st(&s_system);
}
uint32_t system_driver_get_cpu0_wifi_mac_port_trigger_st()
{
	return system_hal_get_cpu0_wifi_mac_port_trigger_st(&s_system);
}
uint32_t system_driver_get_cpu0_wifi_mac_int_gen_st()
{
	return system_hal_get_cpu0_wifi_mac_int_gen_st(&s_system);
}
uint32_t system_driver_get_cpu0_wifi_hsu_irq_st()
{
	return system_hal_get_cpu0_wifi_hsu_irq_st(&s_system);
}
uint32_t system_driver_get_cpu0_wifi_int_mac_wakeup_st()
{
	return system_hal_get_cpu0_wifi_int_mac_wakeup_st(&s_system);
}
uint32_t system_driver_get_cpu0_dm_irq_st()
{
	return system_hal_get_cpu0_dm_irq_st(&s_system);
}
uint32_t system_driver_get_cpu0_ble_irq_st()
{
	return system_hal_get_cpu0_ble_irq_st(&s_system);
}
uint32_t system_driver_get_cpu0_bt_irq_st()
{
	return system_hal_get_cpu0_bt_irq_st(&s_system);
}
uint32_t system_driver_get_cpu0_mbox0_int_st()
{
	return system_hal_get_cpu0_mbox0_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_mbox1_int_st()
{
	return system_hal_get_cpu0_mbox1_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_bmc64_int_st()
{
	return system_hal_get_cpu0_bmc64_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_touched_int_st()
{
	return system_hal_get_cpu0_touched_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_usbplug_int_st()
{
	return system_hal_get_cpu0_usbplug_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_rtc_int_st()
{
	return system_hal_get_cpu0_rtc_int_st(&s_system);
}
uint32_t system_driver_get_cpu0_gpio_int_st()
{
	return system_hal_get_cpu0_gpio_int_st(&s_system);
}

//0x2a
uint32_t system_driver_get_cpu1_bmc32_int_st()
{
	return system_hal_get_cpu1_bmc32_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_host_0_irq_st()
{
	return system_hal_get_cpu1_host_0_irq_st(&s_system);
}
uint32_t system_driver_get_cpu1_host_0_sec_irq_st()
{
	return system_hal_get_cpu1_host_0_sec_irq_st(&s_system);
}
uint32_t system_driver_get_cpu1_timer_int_st()
{
	return system_hal_get_cpu1_host_0_sec_irq_st(&s_system);
}
uint32_t system_driver_get_cpu1_uart_int_st()
{
	return system_hal_get_cpu1_uart_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_pwm_int_st()
{
	return system_hal_get_cpu1_pwm_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_i2c_int_st()
{
	return system_hal_get_cpu1_i2c_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_spi_int_st()
{
	return system_hal_get_cpu1_spi_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_sadc_int_st()
{
	return system_hal_get_cpu1_sadc_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_irda_int_st()
{
	return system_hal_get_cpu1_irda_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_sdio_int_st()
{
	return system_hal_get_cpu1_sdio_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_gdma_int_st()
{
	return system_hal_get_cpu1_gdma_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_la_int_st()
{
	return system_hal_get_cpu1_la_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_timer1_int_st()
{
	return system_hal_get_cpu1_timer1_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_i2c1_int_st()
{
	return system_hal_get_cpu1_i2c1_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_uart1_int_st()
{
	return system_hal_get_cpu1_uart1_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_uart2_int_st()
{
	return system_hal_get_cpu1_uart2_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_spi1_int_st()
{
	return system_hal_get_cpu1_spi1_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_can_int_st()
{
	return system_hal_get_cpu1_can_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_usb_int_st()
{
	return system_hal_get_cpu1_usb_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_qspi_int_st()
{
	return system_hal_get_cpu1_qspi_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_fft_int_st()
{
	return system_hal_get_cpu1_fft_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_sbc_int_st()
{
	return system_hal_get_cpu1_sbc_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_aud_int_st()
{
	return system_hal_get_cpu1_aud_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_i2s_int_st()
{
	return system_hal_get_cpu1_i2s_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_jpegenc_int_st()
{
	return system_hal_get_cpu1_jpegenc_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_jpegdec_int_st()
{
	return system_hal_get_cpu1_jpegdec_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_lcd_int_st()
{
	return system_hal_get_cpu1_lcd_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_wifi_int_phy_st()
{
	return system_hal_get_cpu1_wifi_int_phy_st(&s_system);
}
uint32_t system_driver_get_cpu1_wifi_mac_int_timer_st()
{
	return system_hal_get_cpu1_wifi_mac_int_timer_st(&s_system);
}
uint32_t system_driver_get_cpu1_wifi_mac_int_misc_st()
{
	return system_hal_get_cpu1_wifi_mac_int_misc_st(&s_system);
}

//0x29
uint32_t system_driver_get_cpu1_wifi_mac_int_rx_trigger_st()
{
	return system_hal_get_cpu1_wifi_mac_int_rx_trigger_st(&s_system);
}
uint32_t system_driver_get_cpu1_wifi_mac_int_tx_trigger_st()
{
	return system_hal_get_cpu1_wifi_mac_int_tx_trigger_st(&s_system);
}
uint32_t system_driver_get_cpu1_wifi_mac_port_trigger_st()
{
	return system_hal_get_cpu1_wifi_mac_port_trigger_st(&s_system);
}
uint32_t system_driver_get_cpu1_wifi_mac_int_gen_st()
{
	return system_hal_get_cpu1_wifi_mac_int_gen_st(&s_system);
}
uint32_t system_driver_get_cpu1_wifi_hsu_irq_st()
{
	return system_hal_get_cpu1_wifi_hsu_irq_st(&s_system);
}
uint32_t system_driver_get_cpu1_wifi_int_mac_wakeup_st()
{
	return system_hal_get_cpu1_wifi_int_mac_wakeup_st(&s_system);
}
uint32_t system_driver_get_cpu1_dm_irq_st()
{
	return system_hal_get_cpu1_dm_irq_st(&s_system);
}
uint32_t system_driver_get_cpu1_ble_irq_st()
{
	return system_hal_get_cpu1_ble_irq_st(&s_system);
}
uint32_t system_driver_get_cpu1_bt_irq_st()
{
	return system_hal_get_cpu1_bt_irq_st(&s_system);
}
uint32_t system_driver_get_cpu1_mbox0_int_st()
{
	return system_hal_get_cpu1_mbox0_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_mbox1_int_st()
{
	return system_hal_get_cpu1_mbox1_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_bmc64_int_st()
{
	return system_hal_get_cpu1_bmc64_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_touched_int_st()
{
	return system_hal_get_cpu1_touched_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_usbplug_int_st()
{
	return system_hal_get_cpu1_usbplug_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_rtc_int_st()
{
	return system_hal_get_cpu1_rtc_int_st(&s_system);
}
uint32_t system_driver_get_cpu1_gpio_int_st()
{
	return system_hal_get_cpu1_gpio_int_st(&s_system);
}


//REG_0x30 0x31 0x32 0x33 0x34 0x35
void system_driver_set_gpio0_perial_mode(uint32_t group, uint32_t value)
{
	system_hal_set_gpio0_perial_mode(&s_system, group, value);
}
uint32_t system_driver_get_gpio0_perial_mode(uint32_t group)
{
	return system_hal_get_gpio0_perial_mode(&s_system, group);
}
void system_driver_set_gpio1_perial_mode(uint32_t group, uint32_t value)
{
	system_hal_set_gpio1_perial_mode(&s_system, group, value);
}
uint32_t system_driver_get_gpio1_perial_mode(uint32_t group)
{
	return system_hal_get_gpio1_perial_mode(&s_system, group);
}

void system_driver_set_gpio2_perial_mode(uint32_t group, uint32_t value)
{
	system_hal_set_gpio2_perial_mode(&s_system, group, value);
}
uint32_t system_driver_get_gpio2_perial_mode(uint32_t group)
{
	return system_hal_get_gpio2_perial_mode(&s_system, group);
}
void system_driver_set_gpio3_perial_mode(uint32_t group, uint32_t value)
{
	system_hal_set_gpio3_perial_mode(&s_system, group, value);
}
uint32_t system_driver_get_gpio3_perial_mode(uint32_t group)
{
	return system_hal_get_gpio3_perial_mode(&s_system, group);
}
void system_driver_set_gpio4_perial_mode(uint32_t group, uint32_t value)
{
	system_hal_set_gpio4_perial_mode(&s_system, group, value);
}
uint32_t system_driver_get_gpio4_perial_mode(uint32_t group)
{
	return system_hal_get_gpio4_perial_mode(&s_system, group);
}
void system_driver_set_gpio5_perial_mode(uint32_t group, uint32_t value)
{
	system_hal_set_gpio5_perial_mode(&s_system, group, value);
}
uint32_t system_driver_get_gpio5_perial_mode(uint32_t group)
{
	return system_hal_get_gpio5_perial_mode(&s_system, group);
}
void system_driver_set_gpio6_perial_mode(uint32_t group, uint32_t value)
{
	system_hal_set_gpio6_perial_mode(&s_system, group, value);
}
uint32_t system_driver_get_gpio6_perial_mode(uint32_t group)
{
	return system_hal_get_gpio6_perial_mode(&s_system, group);
}
void system_driver_set_gpio7_perial_mode(uint32_t group, uint32_t value)
{
	system_hal_set_gpio7_perial_mode(&s_system, group, value);
}
uint32_t system_driver_get_gpio7_perial_mode(uint32_t group)
{
	return system_hal_get_gpio7_perial_mode(&s_system, group);
}

//0x38
void system_driver_set_dbug_cfg0(uint32_t group, bool value)
{
	system_hal_set_dbug_cfg0(&s_system, group, value);
}
uint32_t system_driver_get_dbug_cfg0(uint32_t group)
{
	return system_hal_get_dbug_cfg0(&s_system, group);
}

//0x39
void system_driver_set_dbug_cfg1(uint32_t group, bool value)
{
	system_hal_set_dbug_cfg1(&s_system, group, value);
}
uint32_t system_driver_get_dbug_cfg1(uint32_t group)
{
	return system_hal_get_dbug_cfg1(&s_system, group);
}


#endif

void sys_drv_set_ana_trxt_tst_enable(uint32_t value)
{
    uint32_t int_level = rtos_disable_int();

    sys_hal_set_ana_trxt_tst_enable(value);
    rtos_enable_int(int_level);
}
void sys_drv_set_ana_scal_en(uint32_t value)
{
    uint32_t int_level = rtos_disable_int();

    sys_hal_set_ana_scal_en(value);
    rtos_enable_int(int_level);
}
void sys_drv_set_ana_gadc_buf_ictrl(uint32_t value)
{
   uint32_t int_level = rtos_disable_int();

    sys_hal_set_ana_gadc_buf_ictrl(value);
    rtos_enable_int(int_level);
}
void sys_drv_set_ana_gadc_cmp_ictrl(uint32_t value)
{
    uint32_t int_level = rtos_disable_int();

    sys_hal_set_ana_gadc_cmp_ictrl(value);
    rtos_enable_int(int_level);
}

void sys_drv_set_ana_pwd_gadc_buf(uint32_t value)
{
    uint32_t int_level = rtos_disable_int();

    sys_hal_set_ana_pwd_gadc_buf(value);
    rtos_enable_int(int_level);
}
void sys_drv_set_ana_vref_sel(uint32_t value)
{
    uint32_t int_level = rtos_disable_int();

    sys_hal_set_ana_vref_sel(value);
    rtos_enable_int(int_level);
}
void sys_drv_set_ana_cb_cal_manu(uint32_t value)
{
    uint32_t int_level = rtos_disable_int();
    
    sys_hal_set_ana_cb_cal_manu(value);
    rtos_enable_int(int_level);
}
void sys_drv_set_ana_cb_cal_trig(uint32_t value)
{
    uint32_t int_level = rtos_disable_int();
    
    sys_hal_set_ana_cb_cal_trig(value);
    rtos_enable_int(int_level);
}

void sys_drv_set_ana_vlsel_ldodig(uint32_t value)
{
    uint32_t int_level = rtos_disable_int();
    sys_hal_set_ana_vlsel_ldodig(value);
    rtos_enable_int(int_level);
}
void sys_drv_set_ana_vhsel_ldodig(uint32_t value)
{
    uint32_t int_level = rtos_disable_int();
    sys_hal_set_ana_vhsel_ldodig(value);
    rtos_enable_int(int_level);
}

void sys_drv_set_ana_vctrl_sysldo(uint32_t value)
{
    uint32_t int_level = rtos_disable_int();
    sys_hal_set_ana_vctrl_sysldo(value);
    rtos_enable_int(int_level);
}
