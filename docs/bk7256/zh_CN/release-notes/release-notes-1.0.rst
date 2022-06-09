:orphan:

.. _armino_1.0:

Armino 1.2.0
========================

We are pleased to announce the release of Armino version 1.2.0.

.. note::

 Armino is not designed from scratch, the baseline of Armino 1.2.0 is Beken Wi-Fi SDK v3.0,
 which has been used by many customers around the world. A lot of enhancements and changes
 have been made in Armino 1.2.0 compares to it's baseline, that's what we annouced in this
 release notes.
 
Major enhancements with this release include:

* Support new software architecture, such as components and middleware etc
* Support boot for RISC-V
* Support new APIs for following drivers:

  - ADC
  - GPIO
  - DMA
  - UART
  - SPI
  - QSPI
  - Timer
  - TRNG
  - MailBox
  - Interrupt
  - Watchdog
  - Efusea
  - PWM
  - Vault
* Support Wi-Fi 6 stack
* Support new APIs for Wi-Fi
* Support new APIs for BLE
* Support BLE 5.x stack
* Support POSIX for FreeRTOS
* Upgrade FreeRTOS to v10.0
* Upgrade LWIP to v2.1.2
* Support CMake build
* Support Kconfig and per-soc and per-project configuration
* Support new documentation

The following sections provide detailed lists of changes by component.

Software Architecture
---------------------------

The software architecture has been redesigned:

 - A dedicated layer ``components`` has been added, it's used to contain the reusable or service related modules.
 - A dedicated layer ``middleware`` has been added, it works as the adapter between hardware and other components.
 - A dedicated layer ``projects`` has been added to contains all applications, projects or examples.
 - The FreeRTOS have been moved to Armino, Armino supports FreeRTOS by default.

FreeRTOS
-------------------

The FreeRTOS has been upgrade from v9.0 to v10.0.

Posix APIs
-------------------

The POSIX APIs has been supported.

Wi-Fi
-------------------

 - Support Wi-Fi 6 stack
 - Support new Wi-Fi APIs

  - :cpp:func:`bk_wifi_init` - init WiFi
  - :cpp:func:`bk_wifi_deinit` - deinit WiFi
  - :cpp:func:`bk_wifi_set_country` - set country code
  - :cpp:func:`bk_wifi_get_country` - get country code
  - :cpp:func:`bk_wifi_send_raw` - send raw 802.11 frame
  - :cpp:func:`bk_wifi_sta_start` - start BK STA
  - :cpp:func:`bk_wifi_sta_stop` - stop BK STA
  - :cpp:func:`bk_wifi_sta_set_config` - set **basic** configurations BK STA, such as SSID etc
  - :cpp:func:`bk_wifi_sta_get_config` - get basic configuration of BK STA
  - :cpp:func:`bk_wifi_sta_get_link_status` - get link status of BK STA
  - :cpp:func:`bk_wifi_sta_get_mac` - get STA MAC address
  - :cpp:func:`bk_wifi_ap_start` - start BK AP
  - :cpp:func:`bk_wifi_ap_stop` - stop BK AP
  - :cpp:func:`bk_wifi_ap_set_config` - set the **basic** configuration of BK AP, such as SSID etc
  - :cpp:func:`bk_wifi_ap_get_config` - get the basic configuration of BK AP
  - :cpp:func:`bk_wifi_ap_get_mac` - get AP MAC address
  - :cpp:func:`bk_wifi_monitor_start` - start the monitor
  - :cpp:func:`bk_wifi_monitor_stop` - stop the monitor
  - :cpp:func:`bk_wifi_monitor_set_config` - set **basic** configuration of the monitor
  - :cpp:func:`bk_wifi_monitor_get_config` - get basic configuration of the monitor
  - :cpp:func:`bk_wifi_monitor_register_cb` - register monitor callback function
  - :cpp:func:`bk_wifi_monitor_set_channel` - set the channel of monitor
  - :cpp:func:`bk_wifi_scan_register_cb` - register scan callback function
  - :cpp:func:`bk_wifi_scan_start` - start the scan
  - :cpp:func:`bk_wifi_scan_stop` - stop the scan
  - :cpp:func:`bk_wifi_scan_get_result` - get the scan result
  - :cpp:func:`bk_wifi_scan_dump_result` - dump the scan result
  - :cpp:func:`bk_wifi_scan_free_result` - dump the scan result
  - :cpp:func:`bk_wifi_filter_set_config` - set filter configuration
  - :cpp:func:`bk_wifi_filter_register_cb` - register the filter callback function

BLE
-------------------

Driver
-------------------

The driver architecture has been redesigned:

 - The driver has been divided to 3 layers, the Driver layer, HAL layer and SoC Layer.
 - Some driver of the peripherals are re-implemented based on the new driver architecture, such as ADC etc.

ADC
*********

ADC are re-designed in this release, here are the new ADC APIs:

 - :cpp:func:`bk_adc_driver_init`
 - :cpp:func:`bk_adc_aquire`
 - :cpp:func:`bk_adc_release`
 - :cpp:func:`bk_adc_driver_deinit`
 - :cpp:func:`bk_adc_init`
 - :cpp:func:`bk_adc_deinit`
 - :cpp:func:`bk_adc_start`
 - :cpp:func:`bk_adc_stop`
 - :cpp:func:`bk_adc_read_raw`
 - :cpp:func:`bk_adc_read`
 - :cpp:func:`bk_adc_register_isr`
 - :cpp:func:`bk_adc_set_clk`
 - :cpp:func:`bk_adc_set_channel`
 - :cpp:func:`bk_adc_set_sample_rate`
 - :cpp:func:`bk_adc_set_filter`
 - :cpp:func:`bk_adc_set_steady_time`
 - :cpp:func:`bk_adc_set_sample_cnt`
 - :cpp:func:`bk_adc_enable_saturate`
 - :cpp:func:`bk_adc_disable_saturate`
 - :cpp:func:`bk_adc_set_saturate`
 - :cpp:func:`bk_adc_get_saturate`
 - :cpp:func:`bk_adc_set_config`
 - :cpp:func:`bk_adc_set_mode`
 - :cpp:func:`bk_adc_get_mode`

GPIO
*********

GPIO are re-designed in this release, here are the new GPIO APIs:

 - :cpp:func:`bk_gpio_driver_init`
 - :cpp:func:`bk_gpio_driver_deinit`
 - :cpp:func:`bk_gpio_enable_output`
 - :cpp:func:`bk_gpio_disable_output`
 - :cpp:func:`bk_gpio_enable_input`
 - :cpp:func:`bk_gpio_disable_input`
 - :cpp:func:`bk_gpio_enable_pull`
 - :cpp:func:`bk_gpio_disable_pull`
 - :cpp:func:`bk_gpio_pull_up`
 - :cpp:func:`bk_gpio_pull_down`
 - :cpp:func:`bk_gpio_set_output_high`
 - :cpp:func:`bk_gpio_set_output_low`
 - :cpp:func:`bk_gpio_get_input`
 - :cpp:func:`bk_gpio_set_config`
 - :cpp:func:`bk_gpio_register_isr`
 - :cpp:func:`bk_gpio_enable_interrupt`
 - :cpp:func:`bk_gpio_disable_interrupt`
 - :cpp:func:`bk_gpio_set_interrupt_type`

PWM
************

PWM are re-designed in this release, here are the new PWM APIs:

 - :cpp:func:`bk_pwm_driver_init`
 - :cpp:func:`bk_pwm_driver_deinit`
 - :cpp:func:`bk_pwm_init`
 - :cpp:func:`bk_pwm_deinit`
 - :cpp:func:`bk_pwm_set_duty_period`
 - :cpp:func:`bk_pwm_start`
 - :cpp:func:`bk_pwm_stop`
 - :cpp:func:`bk_pwm_set_init_signal_low`
 - :cpp:func:`bk_pwm_set_init_signal_high`
 - :cpp:func:`bk_pwm_register_isr`
 - :cpp:func:`bk_pwm_enable_interrupt`
 - :cpp:func:`bk_pwm_disable_interrupt`
 - :cpp:func:`bk_pwm_group_init`
 - :cpp:func:`bk_pwm_group_deinit`
 - :cpp:func:`bk_pwm_group_start`
 - :cpp:func:`bk_pwm_group_stop`
 - :cpp:func:`bk_pwm_capture_init`
 - :cpp:func:`bk_pwm_capture_deinit`
 - :cpp:func:`bk_pwm_capture_start`
 - :cpp:func:`bk_pwm_capture_stop`
 - :cpp:func:`bk_pwm_capture_get_value`


DMA
********

DMA are re-designed in this release, here are the new DMA APIs:

 - :cpp:func:`bk_dma_driver_init`
 - :cpp:func:`bk_dma_driver_deinit`
 - :cpp:func:`bk_dma_init`
 - :cpp:func:`bk_dma_deinit`
 - :cpp:func:`bk_dma_start`
 - :cpp:func:`bk_dma_stop`
 - :cpp:func:`bk_dma_write`
 - :cpp:func:`bk_dma_read`
 - :cpp:func:`bk_dma_enable_finish_interrupt`
 - :cpp:func:`bk_dma_disable_finish_interrupt`
 - :cpp:func:`bk_dma_enable_half_finish_interrupt`
 - :cpp:func:`bk_dma_disable_half_finish_interrupt`
 - :cpp:func:`bk_dma_register_isr`
 - :cpp:func:`bk_dma_set_transfer_len`
 - :cpp:func:`bk_dma_set_src_addr`
 - :cpp:func:`bk_dma_set_dest_addr`
 - :cpp:func:`bk_dma_get_remain_len`


UART
*******

UART are re-designed in this release, here are the new UART APIs:

 - :cpp:func:`bk_uart_driver_init`
 - :cpp:func:`bk_uart_driver_deinit`
 - :cpp:func:`bk_uart_init`
 - :cpp:func:`bk_uart_deinit`
 - :cpp:func:`bk_uart_set_baud_rate`
 - :cpp:func:`bk_uart_set_data_bits`
 - :cpp:func:`bk_uart_set_stop_bits`
 - :cpp:func:`bk_uart_set_parity`
 - :cpp:func:`bk_uart_set_hw_flow_ctrl`
 - :cpp:func:`bk_uart_set_rx_full_threshold`
 - :cpp:func:`bk_uart_set_tx_empty_threshold`
 - :cpp:func:`bk_uart_set_rx_timeout`
 - :cpp:func:`bk_uart_disable_hw_flow_ctrl`
 - :cpp:func:`bk_uart_enable_tx_interrupt`
 - :cpp:func:`bk_uart_disable_tx_interrupt`
 - :cpp:func:`bk_uart_enable_rx_interrupt`
 - :cpp:func:`bk_uart_disable_rx_interrupt`
 - :cpp:func:`bk_uart_register_rx_isr`
 - :cpp:func:`bk_uart_register_tx_isr`
 - :cpp:func:`bk_uart_write_bytes`
 - :cpp:func:`bk_uart_read_bytes`
 - :cpp:func:`bk_uart_disable_rx`
 - :cpp:func:`bk_uart_disable_tx`
 - :cpp:func:`bk_uart_enable_sw_fifo`
 - :cpp:func:`bk_uart_disable_sw_fifo`

SPI
********

SPI are re-designed in this release, here are the new SPI APIs:

 - :cpp:func:`bk_spi_driver_init`
 - :cpp:func:`bk_spi_driver_deinit`
 - :cpp:func:`bk_spi_init`
 - :cpp:func:`bk_spi_deinit`
 - :cpp:func:`bk_spi_set_mode`
 - :cpp:func:`bk_spi_set_bit_width`
 - :cpp:func:`bk_spi_set_wire_mode`
 - :cpp:func:`bk_spi_set_baud_rate`
 - :cpp:func:`bk_spi_set_bit_order`
 - :cpp:func:`bk_spi_register_rx_isr`
 - :cpp:func:`bk_spi_register_tx_finish_isr`
 - :cpp:func:`bk_spi_write_bytes`
 - :cpp:func:`bk_spi_read_bytes`
 - :cpp:func:`bk_spi_transmit`
 - :cpp:func:`bk_spi_dma_write_bytes`
 - :cpp:func:`bk_spi_dma_read_bytes`
 - :cpp:func:`bk_spi_dma_transmit`

QSPI
*********

QSPI driver are re-designed in this release, here are the new QSPI APIs:

 - :cpp:func:`bk_qspi_driver_init`
 - :cpp:func:`bk_qspi_driver_deinit`
 - :cpp:func:`bk_qspi_init`
 - :cpp:func:`bk_qspi_deinit`
 - :cpp:func:`bk_qspi_command`
 - :cpp:func:`bk_qspi_write`
 - :cpp:func:`bk_qspi_read`
 - :cpp:func:`bk_qspi_register_tx_isr`
 - :cpp:func:`bk_qspi_register_rx_isr`
 - :cpp:func:`bk_qspi_psram_init`
 - :cpp:func:`bk_qspi_psram_deinit`
 - :cpp:func:`bk_qspi_psram_enter_quad_mode`
 - :cpp:func:`bk_qspi_psram_exit_quad_mode`
 - :cpp:func:`bk_qspi_psram_quad_write`
 - :cpp:func:`bk_qspi_psram_single_write`
 - :cpp:func:`bk_qspi_psram_quad_read`
 - :cpp:func:`bk_qspi_psram_single_read`
 - :cpp:func:`bk_qspi_psram_write`
 - :cpp:func:`bk_qspi_psram_read`

Timer
*********

Timer driver are re-designed in this release, here are the new Timer APIs:

 - :cpp:func:`bk_timer_driver_init`
 - :cpp:func:`bk_timer_driver_deinit`
 - :cpp:func:`bk_timer_start`
 - :cpp:func:`bk_timer_stop`
 - :cpp:func:`bk_timer_get_cnt`
 - :cpp:func:`bk_timer_enable`
 - :cpp:func:`bk_timer_disable`
 - :cpp:func:`bk_timer_get_period`

TRNG
*********

TRNG driver are re-designed in this release, here are the new TRNG APIs:

 - :cpp:func:`bk_trng_driver_init`
 - :cpp:func:`bk_trng_driver_deinit`
 - :cpp:func:`bk_trng_start`
 - :cpp:func:`bk_trng_stop`
 - :cpp:func:`bk_rand`

EFUSE
**********

Efuse driver are re-designed in this release, here are the new Efuse APIs:

 - :cpp:func:`bk_efuse_driver_init`
 - :cpp:func:`bk_efuse_driver_deinit`
 - :cpp:func:`bk_efuse_write`
 - :cpp:func:`bk_efuse_read`

MailBox
**********

Mailbox driver are re-designed in this release, here are the new Mailbox APIs:

 - :cpp:func:`mailbox_init`
 - :cpp:func:`mailbox_deinit`
 - :cpp:func:`mailbox_set_param`
 - :cpp:func:`mailbox_recv_callback_register`
 - :cpp:func:`mailbox_recv_callback_unregister`
 - :cpp:func:`mailbox_send`

Interrupt
**************

Interrupt control are re-designed in this release, here are the new Interrupt APIs:

 - :cpp:func:`bk_int_isr_register`
 - :cpp:func:`bk_int_isr_unregister`
 - :cpp:func:`bk_int_set_priority`
 - :cpp:func:`bk_int_set_group`
 - :cpp:func:`bk_get_int_statis`
 - :cpp:func:`bk_dump_int_statis`


Watchdog
**************

Watchdog are re-designed in this release, here are the new Watchdog APIs:

 - :cpp:func:`bk_wdt_driver_init`
 - :cpp:func:`bk_wdt_driver_deinit`
 - :cpp:func:`bk_wdt_start`
 - :cpp:func:`bk_wdt_stop`
 - :cpp:func:`bk_wdt_feed`

Build System
-------------------

The legacy GNU Makefile build system has been removed and the CMake build system has been supported.

Log
-------------------

Following log APIs have been added:

 - BK_LOGI
 - BK_LOGD
 - BK_LOGV

Event
-------------------

Following Event APIs have been added:

 - :cpp:func:`bk_event_register_cb`
 - :cpp:func:`bk_event_unregister_cb`
 - :cpp:func:`bk_event_post`

Netif
-------------------

Following Netif APIs have been added:

 - :cpp:func:`bk_netif_init`
 - :cpp:func:`bk_netif_set_ip4_config`
 - :cpp:func:`bk_netif_get_ip4_config`

Documentation
-------------------

New documentation framework has been added to this release.

Examples
-------------------

Following examples has been added:

 - :example:`hello world <get-started/hello_world>`
 - :example:`ADC <peripherals/adc>`
 - :example:`DMA <peripherals/dma>`
 - :example:`GPIO <peripherals/gpio>`
 - :example:`Interrupt <peripherals/icu>`
 - :example:`PWM <peripherals/pwm>`
 - :example:`SPI <peripherals/spi>`
 - :example:`Timer <peripherals/timer>`
 - :example:`Uart <peripherals/uart>`
 - :example:`Soft-AP <wifi/ap>`
 - :example:`Wi-Fi Scan <wifi/scan>`
 - :example:`Station  <wifi/sta>`
 - :example:`Build System <build_system>`
