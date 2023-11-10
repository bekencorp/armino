string(REGEX REPLACE "beken/" "" SOC "${TFM_PLATFORM}")

set(incs
        config/
        port/
        driver/include/bk_private
        driver/common
        driver/include
        driver/uart
        driver/gpio
        driver/sys_ctrl
        driver/flash
        driver/spinlock
        driver/mailbox
        driver/timer
        driver/hal
        include/
)

set(srcs
	./driver/port/os_port.c
	./driver/port/mem_port.c
	./driver/sys_ctrl/sys_driver.c
	./driver/sys_ctrl/sys_driver_common.c
	./driver/sys_ctrl/sys_clock_driver.c
	./driver/sys_ctrl/sys_int_driver.c
	./driver/uart/uart_driver.c
	./driver/uart/uart_statis.c
	./driver/gpio/gpio_driver_base.c
	./driver/gpio/gpio_driver.c
	./driver/spinlock/amp_res_lock.c
	./driver/flash/flash_driver.c
	./driver/timer/timer_driver.c
	./driver/hal/uart_hal.c
	./driver/hal/timer_hal.c
	./driver/hal/sys_hal.c
	./driver/hal/gpio_hal.c
	./driver/hal/uart_hal_debug.c
	./driver/hal/gpio_hal_debug.c
	./driver/hal/flash_hal_debug.c
	./driver/hal/flash_hal.c
	./driver/hal/uart_ll.c
	./driver/hal/timer_ll.c
	./driver/hal/gpio_ll.c
	./driver/hal/flash_ll.c
)

set(s_srcs
        driver/bk_tfm_mpc.c
        driver/bk_tfm_log.c
        driver/mpc/mpc_driver.c
        driver/hal/mpc_hal.c
        driver/hal/mpc_hal_debug.c
        driver/prro/prro_driver.c
        driver/hal/prro_hal.c
        driver/hal/prro_hal_debug.c
)

set(s_incs
        driver
        driver/mpc
        driver/prro
)

target_include_directories(platform_s PUBLIC ${incs} ${s_incs})
target_sources(platform_s PRIVATE ${srcs} ${s_srcs})
target_compile_options(platform_s
    PUBLIC
        ${COMPILER_CP_FLAG}
)


target_include_directories(platform_ns PUBLIC ${incs})
target_sources(platform_ns PRIVATE ${srcs})
target_compile_options(platform_ns
    PUBLIC
        ${COMPILER_CP_FLAG}
)

if(BL2)
	set(bl2_srcs driver/bk_tfm_flash.c)
	set(bl2_incs driver)
	target_include_directories(platform_bl2 PUBLIC ${incs} ${bl2_incs})
	target_sources(platform_bl2 PRIVATE ${srcs} ${bl2_srcs})
	target_compile_options(platform_bl2 PUBLIC ${BL2_COMPILER_CP_FLAG})
endif()

if(NS)
    #TODO wangzhilei
    target_compile_definitions(platform_ns
    	PUBLIC
    	CONFIG_TFM_NS
    	DOMAIN_NS
    )
endif()

