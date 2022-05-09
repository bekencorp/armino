#include <common/bk_include.h>
#include "bk_arm_arch.h"
#include <common/sys_config.h>
#include "flash_bypass.h"
#include "sys_driver.h"
#include "gpio_driver.h"
#include <driver/spi.h>
#include "spi_hal.h"

void flash_bypass_init(void) {
	char *text_ptr, temp_buf = 0;
	uint32_t reg;
	
	/*step 2, resident cache*/
	REG_WRITE(SPI_R_CTRL(0), 0);
	do {
		text_ptr = (char *)flash_bypass_quad_enable;
		for (uint32_t i = 0; i < CURRENT_ROUTINE_TEXT_SIZE; i ++)
			temp_buf += text_ptr[i];

		REG_WRITE(SPI_R_INT_STATUS(0), temp_buf);
	} while (0);

	/*step 3, config spi master*/
	/*  clear spi status*/
	REG_WRITE(SPI_R_CTRL(0), 0);
	reg = REG_READ(SPI_R_INT_STATUS(0));
	REG_WRITE(SPI_R_INT_STATUS(0), reg);
	REG_WRITE(SPI_R_CFG(0), 0);

	spi_config_t config = {0};
	config.role = SPI_ROLE_MASTER;
	config.bit_width = SPI_BIT_WIDTH_8BITS;
	config.polarity = 1;
	config.phase = 1;
	config.wire_mode = SPI_4WIRE_MODE;
	config.baud_rate = 1000000;
	config.bit_order = SPI_MSB_FIRST;
	bk_spi_driver_init();
	bk_spi_init(0, &config);

	/*step 4, gpioi of SPI0 are set as high-impedance state or input state ,
	          for spi mux with them*/
	gpio_dev_unmap(SPI0_LL_CSN_PIN);
	gpio_dev_unmap(SPI0_LL_SCK_PIN);
	gpio_dev_unmap(SPI0_LL_MOSI_PIN);
	gpio_dev_unmap(SPI0_LL_MISO_PIN);
}

__attribute__((section(".itcm_sec_code"))) void flash_bypass_quad_enable(void)
{
	uint32_t reg;
	uint32_t reg_ctrl, reg_dat;
	uint32_t reg_stat, reg_cfg;
	uint32_t int_status = 0;
	uint32_t spi_status = 0;
	volatile uint32_t i, j, delay_count;

	int_status =  rtos_disable_int();

	/*step 1, save spi register configuration*/
	reg_ctrl = REG_READ(SPI_R_CTRL(0));
	reg_stat = REG_READ(SPI_R_INT_STATUS(0));
	reg_dat  = REG_READ(SPI_R_DATA(0));
	reg_cfg  = REG_READ(SPI_R_CFG(0));

	flash_bypass_init();

	/*step 5, switch flash interface to spi
	 *        Pay attention to prefetch instruction destination, the text can not
	 *        fetch from flash space after this timepoint.
	 */
	sys_drv_set_cpu_storage_connect_op_select_flash_sel(1);

	/*step 6, write enable for status register: 06H*/
	reg = REG_READ(SPI_R_INT_STATUS(0));
	REG_WRITE(SPI_R_INT_STATUS(0), reg);
	REG_WRITE(SPI_R_CFG(0), SPI_CFG_TX_EN_ONE_BYTE);
	REG_WRITE(SPI_R_DATA(0), FLASH_CMD_WR_EN_SR);

	/*step 7, write cmd 31H, data 0x02*/
	for(i = 0; i < 50; i++) {
		spi_status = REG_READ(SPI_R_INT_STATUS(0));
		if(0 != (spi_status & SPI_STATUS_TX_FINISH_INT)) {
			break;
		}
	}
	for(delay_count = 0; delay_count < 50; delay_count ++)
	{
		for(j = 0; j < 8; j ++)
		;
	}

	REG_WRITE(SPI_R_CFG(0), 0);
	reg = REG_READ(SPI_R_INT_STATUS(0));
	REG_WRITE(SPI_R_INT_STATUS(0), reg);

	REG_WRITE(SPI_R_CFG(0), SPI_CFG_TX_EN_TWO_BYTE);
	REG_WRITE(SPI_R_DATA(0), FLASH_CMD_WR_SR);
	REG_WRITE(SPI_R_DATA(0), FLASH_GD25Q32C_SR_QUAD_EN);

	for(i = 0; i < 50; i++) {
		spi_status = REG_READ(SPI_R_INT_STATUS(0));
		if(0 != (spi_status & SPI_STATUS_TX_FINISH_INT)) {
			break;
		}
	}
	for(delay_count = 0; delay_count < 50; delay_count ++)
	{
		for(j = 0; j < 8; j ++)
		;
	}
	REG_WRITE(SPI_R_CFG(0), 0);
	reg = REG_READ(SPI_R_INT_STATUS(0));
	REG_WRITE(SPI_R_INT_STATUS(0), reg);

	/*step 8, switch flash interface to flash controller */
	sys_drv_set_cpu_storage_connect_op_select_flash_sel(0);

	/*step 9, restore spi register configuration*/
	REG_WRITE(SPI_R_CTRL(0), reg_ctrl);
	REG_WRITE(SPI_R_INT_STATUS(0), reg_stat);
	REG_WRITE(SPI_R_DATA(0), reg_dat);
	REG_WRITE(SPI_R_CFG(0), reg_cfg);
	rtos_enable_int(int_status);
}
