#pragma once
#include "driver/dvp_camera_types.h"


#ifdef __cplusplus
extern "C" {
#endif


void lcd_spi_write_cmd(uint8_t data);
void lcd_spi_init_gpio(void);
void lcd_spi_write_data(uint8_t data);



#ifdef __cplusplus
}
#endif

