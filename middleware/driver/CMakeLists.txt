armino_build_get_property(target ARMINO_SOC)

# TODO
# 0. Too big components and makefile, need optimize it
# 2. optimize it when do driver optimization

set(incs
	.
	include
	include/bk_private
	include/bk_private/legacy
	common
	reset_reason
	pwm
	flash
	uart
	sys_ctrl
	gpio
	general_dma
	icu
	i2c
	sdcard
	saradc
	pmu
	mailbox
	spinlock
	touch
	sbc
	rtc
	i2s
	fft
	chip_support
	../compal/common_io/include
	)

set(srcs
	common/driver.c
	reset_reason/reset_reason.c
	sys_ctrl/sys_driver.c
	sys_ctrl/sys_driver_common.c
	sys_ctrl/sys_ps_driver.c
	sys_ctrl/sys_int_driver.c
	sys_ctrl/sys_clock_driver.c
	sys_ctrl/sys_bt_driver.c
	sys_ctrl/sys_wifi_driver.c
	sys_ctrl/sys_audio_driver.c
	sys_ctrl/sys_video_driver.c
	sys_ctrl/sys_psram_driver.c
	sys_ctrl/sys_touch_driver.c
	common/dd.c
	common/drv_model.c
	icu/icu_driver.c
	icu/interrupt_base.c
	timer/timer_driver.c
	wdt/wdt_driver.c
	spi/spi_driver.c
	spi/spi_statis.c
	sys_ctrl/sys_ctrl.c
	pmu/aon_pmu_driver.c
	gpio/gpio_driver_base.c
	uart/uart_driver.c
	uart/uart_statis.c
	uart/printf.c
	saradc/adc_driver.c
	saradc/adc_statis.c
	security/security.c
	security/hal_aes.c
	security/hal_sha.c
	spinlock/amp_res_lock.c
	)
if (CONFIG_AON_WDT)
	list(APPEND incs
		aon_wdt
	)

	list(APPEND srcs
		aon_wdt/aon_wdt_driver.c
	)
endif()

if (CONFIG_MPC)
	list(APPEND incs
		mpc
	)

	list(APPEND srcs
		mpc/mpc_driver.c
	)
endif()

if (CONFIG_FLASH_ORIGIN_API)
	list(APPEND srcs
		flash/flash.c
	)
endif()

if (CONFIG_PRRO)
	list(APPEND incs
		prro
	)

	list(APPEND srcs
		prro/prro_driver.c
	)
endif()

if (CONFIG_CHIP_SUPPORT)
	list(APPEND srcs
		chip_support/chip_support.c
	)
endif()

if (CONFIG_USB)
	list(APPEND srcs
		sys_ctrl/sys_usb_driver.c
	)
endif()

if (CONFIG_EFUSE)
	list(APPEND srcs
		efuse/efuse_driver.c
	)
endif()

if (CONFIG_GENERAL_DMA)
	list(APPEND srcs
		general_dma/dma_driver.c
	)
endif()

if (CONFIG_I2C)
if (NOT CONFIG_SIM_I2C)
	list(APPEND srcs
		i2c/i2c_driver.c
		i2c/i2c_statis.c
	)
endif()
if (CONFIG_SIM_I2C)
	list(APPEND srcs
		i2c/sim_i2c_driver.c
	)
endif() 
endif()  #if (CONFIG_I2C)

if (CONFIG_SUPPORT_IRDA)
	list(APPEND srcs
		irda/irda.c
		)
endif()  #if (CONFIG_SUPPORT_IRDA)

if (CONFIG_PWM)
	list(APPEND srcs
		pwm/pwm_driver.c
		)
endif()  #if (CONFIG_PWM)

if (NOT CONFIG_SOC_BK7256XX)
if (CONFIG_I2S)
	list(APPEND srcs
		i2s/i2s.c
		)
endif()  #if (CONFIG_I2S)
endif()

if (NOT CONFIG_SOC_BK7256XX)
if (CONFIG_FFT)
	list(APPEND srcs
		fft/fft.c
		)
endif()
endif()

if (CONFIG_SOC_BK7256XX)
	list(APPEND srcs
		"flash/flash_bypass.c"
		)
endif()

if (CONFIG_SOC_BK7256XX)
if (CONFIG_FFT)
	list(APPEND incs
		"fft"
	)

	list(APPEND srcs
		fft/fft_driver.c
		)
endif()
endif()

if (CONFIG_SOC_BK7256XX)
if (CONFIG_I2S)
	list(APPEND incs
		"i2s"
	)

	list(APPEND srcs
		i2s/i2s_driver.c
	)
endif()
endif()
	
if (CONFIG_LCD)
	list(APPEND srcs
		"lcd/lcd_driver.c"
		"lcd/lcd_spi.c"
		)

if (CONFIG_LCD_GC9503V)
	list(APPEND srcs
		"lcd/lcd_gc9503v.c"
		)
endif()
if (CONFIG_LCD_H050IWV)
	list(APPEND srcs
		"lcd/lcd_h050iwv.c"
		)
endif()
if (CONFIG_LCD_HX8282)
	list(APPEND srcs
		"lcd/lcd_hx8282.c"
		)
endif()
if (CONFIG_LCD_MD0430R)
	list(APPEND srcs
		"lcd/lcd_md0430r.c"
		)
endif()
if (CONFIG_LCD_MD0700R)
	list(APPEND srcs
		"lcd/lcd_md0700r.c"
		)
endif()
if (CONFIG_LCD_NT35512)
	list(APPEND srcs
		"lcd/lcd_nt35512.c"
		)
endif()
if (CONFIG_LCD_NT35510)
	list(APPEND srcs
		"lcd/lcd_nt35510.c"
		)
endif()
if (CONFIG_LCD_NT35510_MCU)
	list(APPEND srcs
		"lcd/lcd_nt35510_mcu.c"
		)
endif()
if (CONFIG_LCD_ST7282)
	list(APPEND srcs
		"lcd/lcd_st7282.c"
		)
endif()
if (CONFIG_LCD_ST7796S)
	list(APPEND srcs
		"lcd/lcd_st7796s.c"
		)
endif()

if (CONFIG_LCD_ST7710S)
	list(APPEND srcs
		"lcd/lcd_st7710s.c"
		)
endif()

if (CONFIG_LCD_ST7701S)
	list(APPEND srcs
		"lcd/lcd_st7701s.c"
		)
endif()

endif()

if (CONFIG_LCD)
if (CONFIG_TP)
	list(APPEND srcs
		"tp/tp_driver.c"
		"tp/tp_gt911.c"
		"tp/tp_gt1151.c"
		"tp/bk_queue.c"
		"tp/drv_tp.c"
		)
endif()
endif()

if (CONFIG_JPEG_DEC)
	list(APPEND srcs
		"jpeg_dec/jpeg_dec_driver.c"
		)
endif()

if (CONFIG_YUV_BUF)
	list(APPEND incs 
		yuv_buf
	)
	list(APPEND srcs
		"yuv_buf/yuv_buf_driver.c"
	)
endif()

if(CONFIG_DMA2D)
	list(APPEND srcs
		dma2d/dma2d_driver.c
	)
endif()

if (CONFIG_TRNG_SUPPORT)
	list(APPEND srcs
		trng/trng_driver.c
		)
endif() #if (CONFIG_TRNG_SUPPORT)

if (CONFIG_FLASH)
	list(APPEND srcs
		"flash/flash_driver.c"
		"flash/flash_partition.c"
		)
endif() #if (CONFIG_FLASH)

if (CONFIG_QSPI)
	list(APPEND srcs
		"qspi/qspi_driver.c"
		"qspi/qspi_statis.c"
		"qspi/qspi_psram.c"
		"qspi/qspi_flash.c"
		)
endif() #if (CONFIG_QSPI)

if (CONFIG_QSPI AND CONFIG_LCD_QSPI)
	list(APPEND srcs
		"lcd/lcd_qspi_driver.c"
		)

if (CONFIG_LCD_QSPI_SH8601A)
	list(APPEND srcs
		"lcd/lcd_qspi_sh8601a.c"
		)
endif()

if (CONFIG_LCD_QSPI_ST77903)
	list(APPEND srcs
		"lcd/lcd_qspi_st77903.c"
		)
endif()
endif()

if (CONFIG_JPEG_ENCODE)
	list(APPEND srcs
		"jpeg_enc/jpeg_driver.c"
		"jpeg_enc/jpeg_statis.c"
		)
endif() #if (CONFIG_JPEG_ENCODE)

if (CONFIG_PSRAM)
	list(APPEND srcs
		"psram/psram_driver.c"
		)
endif() #if (CONFIG_PSRAM)

if (CONFIG_CALENDAR)
	list(APPEND srcs
		"calendar/calendar_driver.c"
		)
endif() #if (CONFIG_CALENDAR)

if (CONFIG_AON_RTC)
	list(APPEND srcs
		"rtc/aon_rtc_driver.c"
		)
endif() #if (CONFIG_AON_RTC)

if (CONFIG_SDIO_HOST)
	list(APPEND srcs
		"sdio_host/sdio_host_driver.c"
		"sd_card/sd_card_driver.c"
		"sd_card/sdcard_test.c"
		)
endif() #if (CONFIG_SDIO_HOST)

if (CONFIG_SDCARD_V1P0)
	list(APPEND srcs
		"sdcard/sdio_driver.c"
		"sdcard/sdcard.c"
		"sdcard/sdcard_test.c"
		)
endif() #if (CONFIG_SDCARD_HOST)

if (CONFIG_SDIO_V1P0)
	list(APPEND incs
		"sdio/v1p0"
	)

	list(APPEND srcs
		"sdio/v1p0/sdio.c"
		"sdio/v1p0/sdma.c"
		"sdio/v1p0/sutil.c"
		)
endif() #if (CONFIG_SDIO_V1P0)

if (CONFIG_SDIO_V2P0 AND CONFIG_SDIO_SLAVE)
	list(APPEND incs
		"sdio/v2p0"
	)

	list(APPEND srcs
		"sdio/v2p0/sdio_slave_driver.c"
		"sdio/v2p0/sdio_utils.c"
		"sdio/v2p0/sdio_test.c"
		)
endif() #if (CONFIG_SDIO_V2P0)

if (CONFIG_AUDIO)
if (CONFIG_AUDIO_DSP_IP_V1_0)
	list(APPEND incs
		"audio"
		)

	list(APPEND srcs
		"audio/audio.c"
		"audio/ring_buffer.c"
		)
endif()

if (CONFIG_AUDIO_RISCV_IP_V1_0)
	list(APPEND incs
		"audio"
		)
	list(APPEND srcs
		"audio/aud_driver.c"
		"audio/uac_driver.c"
		)
endif()

if (CONFIG_SBC)
if (CONFIG_SOC_BK7256XX)
	list(APPEND srcs
		"sbc/sbc_driver.c"
		)
endif()
endif()

if (CONFIG_SOC_BK7251)
	list(APPEND srcs
		"audio/audio_adc.c"
		"audio/audio_dac.c"
		"audio/ring_buffer_dma_read.c"
		"audio/ring_buffer_dma_write.c"
		)
endif()
endif() #if (CONFIG_AUDIO)

if (CONFIG_TOUCH)
if (CONFIG_SOC_BK7256XX)
	list(APPEND srcs
		"touch/touch_driver.c"
		)
endif()
endif()

if (NOT CONFIG_FULLY_HOSTED)

	list(APPEND incs
		"spi"
		"jpeg_enc"
		"sdcard"
		)

	list(APPEND srcs
		"dsp/dsp.c"
		)

if (CONFIG_SOC_BK7271)
	list(APPEND srcs
		"bk7271/mailbox_driver.c"
		)
endif()
endif() #if (NOT CONFIG_FULLY_HOSTED)

if (CONFIG_SOC_BK7256XX)
	list(APPEND srcs
		"bk7256/mailbox_driver.c"
		)

endif()

if (CONFIG_MAILBOX)

	list(APPEND srcs
		"mailbox/mailbox_driver_base.c"
		)
	if (CONFIG_DUAL_CORE)
		list(APPEND srcs
			mailbox/mailbox_channel.c
			mailbox/mb_ipc_cmd.c
		)
	endif()

endif()

if (CONFIG_SOC_BK7251)
	list(APPEND incs
		"spi"
		)
endif()

if( "${target}" STREQUAL "bk7231u")
	set(target "bk7251")
endif()

if( "${target}" STREQUAL "bk7236a")
	set(target "bk7231n")
endif()

if( "${target}" STREQUAL "bk7231n"
	OR  "${target}" STREQUAL "bk7251"
	OR "${target}" STREQUAL "bk7271"
	OR "${target}" STREQUAL "bk7256_cp1"
	OR "${target}" STREQUAL "bk7236"
	OR CONFIG_SOC_BK7256XX
	)

	list(APPEND incs
		"${target}"
	)

	list(APPEND srcs
		"${target}/gpio_driver.c"
		"${target}/interrupt.c"
		"${target}/interrupt_statis.c"
		)
endif()

if (CONFIG_CAMERA)
    list(APPEND srcs
		"camera/dvp_camera.c"
		"camera/dvp_gc0328c.c"
		"camera/dvp_gc2145.c"
		"camera/dvp_hm1055.c"
		"camera/dvp_ov2640.c"
		"camera/dvp_sensor_devices.c"
		)
endif()

if (CONFIG_USB_UVC)
	list(APPEND srcs
		"camera/uvc_camera.c"
		"camera/uvc_sensor_devices.c"
		)
endif()

if (CONFIG_VAULT_SUPPORT)
	list(APPEND incs
		"vault"
	)

	list(APPEND srcs
		"vault/vault_driver.c"
	)
	
	if (CONFIG_OTP)
		list(APPEND incs
			"otp"
		)

		list(APPEND srcs
			"otp/otp_driver.c"
		)
	endif()
endif()



armino_component_register(SRCS "${srcs}"
	INCLUDE_DIRS "${incs}"
	REQUIRES arm9 riscv cm33
	PRIV_REQUIRES bk_log bk_ate easy_flash
)
