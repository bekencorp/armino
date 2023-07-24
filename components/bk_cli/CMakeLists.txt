set(incs include include/bk_private)
set(srcs)

list(APPEND incs
	../../properties/modules/aec/
	.
	)

if (CONFIG_CLI_USER_CONFIG)
list(APPEND incs ${CONFIG_CLI_USER_CONFIG_DIR})
endif()

if (CONFIG_CLI)
list(APPEND srcs
	cli_adc.c
	cli_ble.c
	cli_event.c
	cli_keyValue.c
	cli_gpio.c
	cli_main.c
	cli_misc.c
	cli_mico.c
	cli_mem.c
	cli_os.c
	cli_rpc.c
	cli_reg.c
	cli_timer.c
	cli_temp_detect.c
	cli_uart.c
	cli_security.c
	cli_spi.c
	cli_exception.c
	)

if (CONFIG_BUTTON OR CONFIG_ADC_KEY)
	list(APPEND srcs
		cli_key.c
	)
endif()

if (CONFIG_INT_WDT)
	list(APPEND srcs
		cli_wdt.c
	)
endif()

if (CONFIG_STA_PS)
	list(APPEND srcs
		cli_pwr.c
	)
endif()

if (CONFIG_PM_TEST)
	list(APPEND srcs cli_pm.c)
endif()

if (CONFIG_OTA_TFTP OR CONFIG_OTA_HTTP)
	list(APPEND srcs
		cli_ota.c
	)
endif()

if (CONFIG_WIFI_ENABLE)
	list(APPEND srcs
		cli_wifi.c
		cli_phy.c
	)
endif()

if (CONFIG_AIRKISS_TEST)
	list(APPEND srcs
		cli_airkiss.c
	)
endif()

if (CONFIG_IPERF_TEST)
	list(APPEND srcs
		cli_iperf.c
	)
endif()

if (CONFIG_GENERAL_DMA)
	list(APPEND srcs
		cli_dma.c
	)
endif()

if (CONFIG_SDCARD)
	list(APPEND srcs
		cli_sd.c
	)
endif()

if (CONFIG_SDIO_SLAVE)
	list(APPEND srcs
		cli_sdio_slave.c
	)
endif()

if (CONFIG_PWM)
	list(APPEND srcs
		cli_pwm.c
	)
endif()

if (CONFIG_I2C)
	list(APPEND srcs
		cli_i2c.c
	)
endif()

if (CONFIG_SUPPORT_MATTER)
	list(APPEND srcs
		cli_matter.c
	)
endif()

if (CONFIG_FLASH)
	list(APPEND srcs
		cli_flash.c
	)
endif()

if (CONFIG_FLASH)
	list(APPEND srcs
		cli_flash_test.c
	)
endif()

if (CONFIG_TRNG_SUPPORT)
	list(APPEND srcs
		cli_trng.c
	)
endif()

if (CONFIG_FATFS)
	list(APPEND srcs "cli_fatfs.c")
endif()

if (CONFIG_VFS)
	list(APPEND srcs
		cli_vfs.c
	)
endif()

if (CONFIG_EFUSE)
	list(APPEND srcs
		cli_efuse.c
	)
endif()

if (CONFIG_SDIO_HOST)
	list(APPEND srcs
		cli_sdio_host.c
	)
endif()

if (CONFIG_LWIP)
	list(APPEND srcs
		cli_lwip.c
	)
endif()

if (CONFIG_BK_NETIF)
	list(APPEND srcs
		cli_netif.c
	)
endif()

if (CONFIG_DUAL_CORE)
	list(APPEND srcs
		rpc_client_gpio.c
	)
endif()

if (CONFIG_SOC_BK7271)
	list(APPEND srcs "cli_dsp_bk7271.c")
endif()

if (CONFIG_USB)
	list(APPEND srcs "cli_usb.c")
endif()

if (CONFIG_VAULT_SUPPORT)
	if (CONFIG_SECURITYIP)
		list(APPEND srcs "cli_securityip.c")
	endif()

	if (CONFIG_OTP)
		list(APPEND srcs "cli_otp.c")
	endif()
ENDIF()

if (CONFIG_QSPI)
	list(APPEND srcs
		"cli_qspi.c"
	)
endif()

if (CONFIG_AON_RTC_TEST)
	list(APPEND srcs
		"cli_aon_rtc.c"
	)
endif()

if (CONFIG_JPEGENC_HW)
	list(APPEND srcs
		"cli_jpegenc.c"
	)
endif()


if (CONFIG_AT_CMD)
	list(APPEND srcs "cli_at.c")
endif()

if (CONFIG_LCD_TEST)
	list(APPEND srcs
		"cli_lcd.c")
endif() #CONFIG_LCD_TEST
if (CONFIG_DMA2D_TEST)
	list(APPEND srcs
		"cli_dma2d.c"
	)
endif() #CONFIG_DMA2D_TEST

if (CONFIG_HW_ROTT_TEST)
	list(APPEND srcs
		"cli_rott.c"
	)
endif() #CONFIG_ROTT_TEST

if (CONFIG_LCD_QSPI_TEST)
	list(APPEND srcs
		"cli_lcd_qspi.c"
	)
endif()

if (CONFIG_EASY_FLASH)
	list(APPEND srcs
		"cli_easyflash.c"
	)
endif()

if (CONFIG_AUDIO_TEST AND CONFIG_ARCH_CM33)
	list(APPEND srcs "cli_aud_v1_1.c")
endif()

if (CONFIG_AUDIO_TEST AND CONFIG_AUDIO_RISCV_IP_V1_0)
	list(APPEND srcs "cli_aud.c")
endif() #if(CONFIG_AUDIO)

if (CONFIG_AUDIO_TEST)
if (CONFIG_AUDIO_RISCV_IP_V1_0 AND CONFIG_DUAL_CORE)
if (NOT CONFIG_SLAVE_CORE)
	list(APPEND srcs cli_aud_cp0.c)
endif()
endif()
endif()

if (CONFIG_AUDIO_AEC_TEST)
	list(APPEND srcs "cli_aec.c")
endif()

if (CONFIG_AUDIO_G711_TEST)
	list(APPEND srcs "cli_g711.c")
endif()

if (CONFIG_AUDIO_OPUS_TEST)
	list(APPEND srcs "cli_opus.c")
endif()

if (CONFIG_AUDIO_ADPCM_TEST)
	list(APPEND srcs "cli_adpcm.c")
endif()

if (CONFIG_AUDIO_MP3_TEST)
	list(APPEND srcs "cli_mp3.c")
endif()

if (CONFIG_FFT_TEST)
	list(APPEND srcs "cli_fft.c")
endif()

if (CONFIG_SBC_TEST)
	list(APPEND srcs "cli_sbc.c")
endif()

if (CONFIG_I2S_TEST)
	list(APPEND srcs "cli_i2s.c")
endif()

if (CONFIG_TOUCH_TEST)
	list(APPEND srcs "cli_touch.c")
endif()

if (CONFIG_CALENDAR)
	list(APPEND srcs "cli_calendar.c")
endif()

elseif (CONFIG_SHELL_ASYNCLOG)
	list(APPEND srcs "cli_main.c")
endif() #if(CONFIG_CLI)

if (CONFIG_SHELL_ASYNCLOG)
	list(APPEND srcs
			shell_task.c
			shell_uart.c
	)
	if (CONFIG_MASTER_CORE)
		list(APPEND srcs
			shell_mailbox_ipc.c
		)
	endif()
	if (CONFIG_SLAVE_CORE)
		list(APPEND srcs
			shell_mailbox_cp1.c
		)
	endif()
endif()

if (CONFIG_JPEGDEC_SW)
	list(APPEND srcs cli_jpegdec.c)
endif()

if (CONFIG_DVP_CAMERA AND CONFIG_DVP_CAMERA_TEST)
	list(APPEND srcs "cli_dvp.c")
endif()

if (CONFIG_DOORBELL)
	list(APPEND srcs cli_doorbell.c)
endif()

if (CONFIG_PSRAM AND CONFIG_PSRAM_TEST)
	list(APPEND srcs cli_psram.c)
endif()

if (CONFIG_AUD_INTF_TEST)
	list(APPEND srcs cli_aud_intf.c)
endif()

if (CONFIG_AUDIO_RSP_TEST)
	list(APPEND srcs cli_aud_rsp.c)
endif()

if (CONFIG_AUDIO_VAD_TEST)
	list(APPEND srcs cli_aud_vad.c)
endif()

if (CONFIG_AUDIO_NS_TEST)
	list(APPEND srcs cli_aud_ns.c)
endif()

if (CONFIG_AUDIO_FLAC_TEST)
	list(APPEND srcs cli_aud_flac.c)
endif()

if (CONFIG_QRCODEGEN_TEST)
	list(APPEND srcs cli_qrcodegen.c)
endif()

if (CONFIG_SPE_TEST)
	list(APPEND srcs cli_spe.c)
endif()

if (CONFIG_MPC_TEST)
	list(APPEND srcs cli_mpc.c)
endif()

if (CONFIG_PRRO_TEST)
	list(APPEND srcs cli_prro.c)
endif()

if (CONFIG_INTERRUPT_TEST)
	list(APPEND srcs cli_interrupt.c)
endif()

if (CONFIG_VIDEO_TRANSFER_DEMO)
	list(APPEND srcs cli_video_transfer.c)
endif()

if (CONFIG_SDMADC_TEST)
	list(APPEND srcs cli_sdmadc.c)
endif()

if (CONFIG_AGORA_IOT_SDK_TEST)
	list(APPEND srcs cli_agora.c)
endif()

if (CONFIG_ES8311_TEST)
	list(APPEND srcs cli_es8311.c)
endif()

if (CONFIG_ASR_TEST)
	list(APPEND srcs cli_asr.c)
endif()

if (CONFIG_FLASHDB_DEMO)
	list(APPEND srcs cli_flashdb.c)
endif()

if (CONFIG_PUF_TEST)
	list(APPEND srcs cli_puf.c)
endif()

armino_component_register(SRCS "${srcs}"
	INCLUDE_DIRS "${incs}"
	REQUIRES utf8 temp_detect app driver user_driver bk_system iperf fatfs user_driver http adc_key key
	#PRIV_REQUIRES airkiss bk_common bk_netif bk_ble bk_ps
	PRIV_REQUIRES bk_common mbedtls bk_netif bk_ble bk_ps fatfs bk_vfs bk_init at bk_wifi compal media_common media multimedia bk_bt ota cmsis jrtplib flashdb
	)

