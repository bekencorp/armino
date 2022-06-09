/*
 * Copyright (c) 2021 Laird Connectivity. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef BL5340_BOARD_H__
#define BL5340_BOARD_H__

#include <hal/nrf_gpio.h>

#define TWI_INSTANCE_NUMBER                   2 /* TWI instance number, cannot be 0 or 1 due to UART usage */
#define TWI_FREQUENCY                         NRF_TWIM_FREQ_400K /* Speed to operate I2C (TWI) at */

#define PORT_EXPANDER_I2C_ADDRESS             (0x70)
#define PORT_EXPANDER_IO_CONFIG               (0x0f) /* IOs 0-3 are input (buttons) and IOs 4-7 are output (LEDs) */

#define PORT_EXPANDER_INTERRUPT_PIN           (38UL)
#define PORT_EXPANDER_INTERRUPT_ACTIVE_LEVEL  (0UL)
#define PORT_EXPANDER_INTERRUPT_PULL          (NRF_GPIO_PIN_NOPULL)

#define PORT_EXPANDER_BUTTON1_IO              (0x1)
#define PORT_EXPANDER_BUTTON1_ACTIVE_LEVEL    (0x0) /* Corrisponding comparison of above value if button is pressed */

#define PORT_EXPANDER_LED1_IO                 (0x10)
#define PORT_EXPANDER_LED1_ACTIVE_LEVEL       (0x0) /* Corrisponding comparison of above value if LED is active */

#endif // BL5340_BOARD_H__
