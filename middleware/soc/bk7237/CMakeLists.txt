armino_build_get_property(target ARMINO_SOC)

if(NOT "${target}" STREQUAL ${COMPONENT_NAME})
    return()
endif()

set(incs
	soc
	hal
	)
set(srcs "stub.c")
list(APPEND srcs
	"hal/icu_hal_debug.c"
	"hal/icu_ll.c"
	"hal/pwm_hal_debug.c"
	"hal/pwm_ll.c"
	"hal/timer_hal_debug.c"
	"hal/timer_ll.c"
	"hal/gpio_hal_debug.c"
	"hal/gpio_ll.c"
	"hal/dma_hal_debug.c"
	"hal/dma_ll.c"
	"hal/uart_hal_debug.c"
	"hal/uart_ll.c"
	"hal/icu_hal_debug.c"
	"hal/icu_ll.c"
	"hal/wdt_hal_debug.c"
	"hal/wdt_ll.c"
	"hal/trng_hal_debug.c"
	"hal/trng_ll.c"
	"hal/efuse_hal_debug.c"
	"hal/efuse_ll.c"
	"hal/adc_hal_debug.c"
	"hal/adc_ll.c"
	"hal/spi_hal_debug.c"
	"hal/spi_ll.c"
	"hal/aon_rtc_hal_debug.c"
	"hal/aon_rtc_ll.c"
	"hal/sys_hal.c"
	"hal/aon_pmu_hal.c"
	"hal/i2c_hal_debug.c"
	"hal/i2c_ll.c"
	"hal/jpeg_hal_debug.c"
	"hal/jpeg_ll.c"
	"hal/fft_hal_debug.c"
	"hal/lcd_disp_hal_debug.c"
	"hal/lcd_disp_ll.c"
	"hal/lcd_disp_hal.c"
	"hal/i2s_hal_debug.c"
	"hal/jpeg_dec_hal.c"
	"hal/flash_hal_debug.c"
	"hal/flash_ll.c"
)

if(CONFIG_SECURITYIP)
	list(APPEND srcs
		"hal/vault_hal_debug.c"
		"hal/vault_ll.c"
	)
endif()



if(CONFIG_AUDIO)
	list(APPEND srcs
		"hal/aud_hal_debug.c"
		"hal/aud_ll.c"
	)
endif()

if(CONFIG_DMA2D)
	list(APPEND srcs
		"hal/dma2d_hal.c"
	)
endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}")

armino_build_get_property(sdkconfig_header SDKCONFIG_HEADER)
armino_build_get_property(config_dir CONFIG_DIR)

#TODO add support to generate ELF with/without bootloader
#
target_linker_script(${COMPONENT_LIB} INTERFACE "${target}_bsp.ld")

set(LD_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_custom_command(
	OUTPUT ${target}_out.ld
	COMMAND "${CMAKE_C_COMPILER}" -C -P -x c -E -o ${target}_out.ld -I ${config_dir} ${LD_DIR}/${target}.ld
	MAIN_DEPENDENCY ${LD_DIR}/${target}.ld ${sdkconfig_header}
	COMMENT "Generating linker script..."
	VERBATIM)

add_custom_target(${target}_linker_script DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${target}_out.ld)
add_dependencies(${COMPONENT_LIB} ${target}_linker_script)

