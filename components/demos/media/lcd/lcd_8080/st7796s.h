#ifndef _ST7796S_H_
#define _ST7796S_H_

#ifdef __cplusplus
extern "C" {
#endif

/* @brief Overview about this API header
 *
 */

/**
 * @brief LCD API
 * @defgroup bk_api_lcd LCD API group
 * @{
 */

//cmd list
#define SLEEP_OUT          0x11
#define COMMAND_1          0xf0
#define COMMAND_2          0xf0
#define PLOAR_CONVERT      0xb4
#define DISP_OUT_CTRL      0xe8
#define POWER_CTRL1         0xc1
#define POWER_CTRL2         0xc2
#define VCOM_CTRL          0xc5
#define CATHODE_CTRL       0xe0
#define ANODE_CTRL         0xe1
#define COLOR_MODE         0x3a
#define DISPLAY_ON         0x29
#define ROW_SET            0x2b
#define COLUMN_SET         0x2a
#define RAM_WRITE          0x2c
#define CONTINUE_WRITE     0x3c
#define MEM_ACCESS_CTRL    0x36

/**
 * @brief  lcd_st7796s_init
 * @param  none
 * @return none.
 */
bk_err_t st7796s_init(void);
void i8080_display_off(void);

/**
 * @brief  set display mem area
 * @param  column
 * @param  row
 * @return none
 */
void st7796s_set_display_mem_area(uint16 xs, uint16 xe, uint16 ys, uint16 ye);


#endif

