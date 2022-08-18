armino_build_get_property(target ARMINO_SOC)

set(incs
	soc/include
	hal/include
	)

set(srcs
	hal/pwm_hal.c
	hal/timer_hal.c
	hal/dma_hal.c
	hal/icu_hal.c
	hal/wdt_hal.c
	hal/spi_hal.c
	hal/i2c_hal.c
	hal/gpio_hal.c
	hal/uart_hal.c
	hal/adc_hal.c
	)

if (CONFIG_FLASH)
	list(APPEND srcs
		hal/flash_hal.c
	)
endif()

if (CONFIG_MPC)
	list(APPEND srcs
		hal/mpc_hal.c
	)
endif()

if (CONFIG_PRRO)
	list(APPEND srcs 
		hal/prro_hal.c
	)
endif()

if (CONFIG_FFT)
	list(APPEND srcs
		hal/fft_hal.c
		)
endif()

if (CONFIG_I2S)
	list(APPEND srcs
		hal/i2s_hal.c
		)
endif()

if(CONFIG_EFUSE)
	list(APPEND srcs
		hal/efuse_hal.c
	)
endif()

if(CONFIG_TRNG_SUPPORT)
	list(APPEND srcs
		"hal/trng_hal.c"
	)
endif()

if(CONFIG_MAILBOX)
	list(APPEND srcs
		"hal/mailbox_hal.c"
	)
endif()

if(CONFIG_QSPI)
	list(APPEND srcs
		"hal/qspi_hal.c"
	)
endif()

if(CONFIG_JPEG_ENCODE)
	list(APPEND srcs
		"hal/jpeg_hal.c"
	)
endif()

if(CONFIG_PSRAM)
	list(APPEND srcs
		"hal/psram_hal.c"
	)
endif()

if(CONFIG_AON_RTC)
	list(APPEND srcs
		"hal/aon_rtc_hal.c"
	)
endif()

if (CONFIG_SDIO_V2P0)
	list(APPEND srcs
		"hal/sdio_hal.c"
	)
endif()

if(CONFIG_SECURITYIP)
	list(APPEND srcs
		"hal/vault_hal.c"
	)
endif()

if(CONFIG_AUDIO)
if(CONFIG_SOC_BK7256XX)
	list(APPEND srcs
		"hal/aud_hal.c"
	)
endif()
endif()

if(CONFIG_SBC)
	list(APPEND srcs
		"hal/sbc_hal.c"
	)
endif()

if(CONFIG_CALENDAR)
	list(APPEND srcs
		"hal/calendar_hal.c"
	)
endif()

if(CONFIG_SDIO_HOST)
	list(APPEND srcs
		"hal/sdio_host_hal.c"
	)
endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" REQUIRES ${target})
