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
	pwm
	rc_beken
	flash
	rw_pub
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
	../compal/common_io/include
	)

set(srcs
	common/driver.c
	sys_ctrl/sys_driver.c
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
	)


	list(APPEND srcs
		flash/flash.c
		macphy_bypass/mac_phy_bypass.c
		gpio/gpio_driver_base.c
		uart/uart_driver.c
		uart/uart_statis.c
		uart/printf.c
		rw_pub/rw_platf_pub.c
		saradc/adc_driver.c
		saradc/adc_statis.c
		security/security.c
		security/hal_aes.c
		security/hal_sha.c
		spinlock/amp_res_lock.c
	)

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
	list(APPEND srcs
		i2c/i2c_driver.c
		i2c/i2c_statis.c
		)
endif()  #if (CONFIG_I2C)

if (CONFIG_SUPPORT_IRDA)
	list(APPEND srcs
		irda/irda.c
		)
endif()  #if (CONFIG_SUPPORT_IRDA)

if (CONFIG_PWM)
	list(APPEND srcs
		pwm/mcu_ps_timer.c
		pwm/pwm_driver.c
		)
endif()  #if (CONFIG_PWM)

if ((NOT CONFIG_SOC_BK7256XX) AND (NOT CONFIG_SOC_BK7256_CP1))
if (CONFIG_I2S)
	list(APPEND srcs
		i2s/i2s.c
		)
endif()  #if (CONFIG_I2S)
endif()

if ((NOT CONFIG_SOC_BK7256XX) AND (NOT CONFIG_SOC_BK7256_CP1))
if (CONFIG_FFT)
	list(APPEND srcs
		fft/fft.c
		)
endif()
endif()

if (CONFIG_SOC_BK7256XX OR CONFIG_SOC_BK7256_CP1)
	list(APPEND srcs
		"flash/flash_bypass.c"
		)
endif()

if (CONFIG_SOC_BK7256XX OR CONFIG_SOC_BK7256_CP1)
if (CONFIG_FFT)
	list(APPEND incs
		"fft"
	)

	list(APPEND srcs
		fft/fft_driver.c
		)
endif()
endif()

if (CONFIG_SOC_BK7256XX OR CONFIG_SOC_BK7256_CP1)
if (CONFIG_I2S)
	list(APPEND incs
		"i2s"
	)

	list(APPEND srcs
		i2s/i2s_driver.c
	)
endif()

	if (CONFIG_LCD)
	list(APPEND srcs
		"lcd_display/lcd_disp_driver.c"
		)
	endif()
	
	if(CONFIG_DMA2D)
	list(APPEND srcs
		dma2d/dma2d_driver.c
	)
	endif()
	
	if (CONFIG_JPEG_DEC)
	list(APPEND srcs
		"jpeg_dec/jpeg_dec_driver.c"
		)
	endif()
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
		)
endif() #if (CONFIG_QSPI)

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

if (CONFIG_SDCARD_HOST)
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

if (CONFIG_SDIO_V2P0)
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
		)
endif()

if (CONFIG_SBC)
if (CONFIG_SOC_BK7256XX OR CONFIG_SOC_BK7256_CP1)
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
if (CONFIG_SOC_BK7256XX OR CONFIG_SOC_BK7256_CP1)
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

if (CONFIG_SOC_BK7256XX OR CONFIG_SOC_BK7256_CP1)
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

if( "${target}" STREQUAL "bk7236")
	set(target "bk7231n")
endif()



if( "${target}" STREQUAL "bk7231n"
	OR  "${target}" STREQUAL "bk7251"
	OR "${target}" STREQUAL "bk7271"
	OR "${target}" STREQUAL "bk7256_cp1"
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


armino_component_register(SRCS "${srcs}"
	INCLUDE_DIRS "${incs}"
	REQUIRES app arm9 bk_wifi bk_usb riscv
	PRIV_REQUIRES bk_common bk_log bk_ble bk_ps bk_ate bk_init
)
