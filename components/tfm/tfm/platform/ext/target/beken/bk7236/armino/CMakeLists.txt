string(REGEX REPLACE "beken/" "" SOC "${TFM_PLATFORM}")

set(incs
        config/
        port/

        ${ARMINO_INCLUDE_REPO_PATH}/
        ${ARMINO_INCLUDE_REPO_PATH}/arch/compiler
        ${ARMINO_COMMON_REPO_PATH}/include

        ${ARMINO_MIDDLEWARE_REPO_PATH}/arch/cm33/include

        ${ARMINO_MIDDLEWARE_REPO_PATH}/driver/include/bk_private
        ${ARMINO_MIDDLEWARE_REPO_PATH}/driver/common
        ${ARMINO_MIDDLEWARE_REPO_PATH}/driver/${SOC}
        ${ARMINO_MIDDLEWARE_REPO_PATH}/driver/uart
        ${ARMINO_MIDDLEWARE_REPO_PATH}/driver/gpio
        ${ARMINO_MIDDLEWARE_REPO_PATH}/driver/sys_ctrl
        ${ARMINO_MIDDLEWARE_REPO_PATH}/driver/flash
        ${ARMINO_MIDDLEWARE_REPO_PATH}/driver/spinlock
        ${ARMINO_MIDDLEWARE_REPO_PATH}/driver/mailbox
        ${ARMINO_MIDDLEWARE_REPO_PATH}/driver/timer

        ${ARMINO_MIDDLEWARE_REPO_PATH}/soc/common/soc/include
        ${ARMINO_MIDDLEWARE_REPO_PATH}/soc/common/hal/include
        ${ARMINO_MIDDLEWARE_REPO_PATH}/soc/${SOC}/soc
        ${ARMINO_MIDDLEWARE_REPO_PATH}/soc/${SOC}/hal
        ${ARMINO_MIDDLEWARE_REPO_PATH}/boards/${SOC}/partitions
	${PARTITION_HDR_DIR}
)

set(srcs
        ${ARMINO_MIDDLEWARE_REPO_PATH}/driver/port/os_port.c
        ${ARMINO_MIDDLEWARE_REPO_PATH}/driver/port/mem_port.c

        ${ARMINO_MIDDLEWARE_REPO_PATH}/driver/sys_ctrl/sys_driver_common.c
        ${ARMINO_MIDDLEWARE_REPO_PATH}/driver/sys_ctrl/sys_clock_driver.c
        ${ARMINO_MIDDLEWARE_REPO_PATH}/driver/uart/uart_driver.c
        ${ARMINO_MIDDLEWARE_REPO_PATH}/driver/uart/uart_statis.c
        ${ARMINO_MIDDLEWARE_REPO_PATH}/driver/gpio/gpio_driver_base.c
        ${ARMINO_MIDDLEWARE_REPO_PATH}/driver/${SOC}/gpio_driver.c
        ${ARMINO_MIDDLEWARE_REPO_PATH}/driver/spinlock/amp_res_lock.c
        ${ARMINO_MIDDLEWARE_REPO_PATH}/driver/flash/flash_driver.c
        ${ARMINO_MIDDLEWARE_REPO_PATH}/driver/sys_ctrl/sys_driver.c
        ${ARMINO_MIDDLEWARE_REPO_PATH}/driver/sys_ctrl/sys_driver_common.c
        ${ARMINO_MIDDLEWARE_REPO_PATH}/driver/sys_ctrl/sys_clock_driver.c
        ${ARMINO_MIDDLEWARE_REPO_PATH}/driver/timer/timer_driver.c
        ${ARMINO_MIDDLEWARE_REPO_PATH}/driver/sys_ctrl/sys_int_driver.c

        ${ARMINO_MIDDLEWARE_REPO_PATH}/soc/common/hal/uart_hal.c
        ${ARMINO_MIDDLEWARE_REPO_PATH}/soc/common/hal/timer_hal.c
        ${ARMINO_MIDDLEWARE_REPO_PATH}/soc/${SOC}/hal/uart_hal_debug.c
        ${ARMINO_MIDDLEWARE_REPO_PATH}/soc/${SOC}/hal/uart_ll.c
        ${ARMINO_MIDDLEWARE_REPO_PATH}/soc/${SOC}/hal/timer_ll.c

        ${ARMINO_MIDDLEWARE_REPO_PATH}/soc/${SOC}/hal/sys_hal.c
        ${ARMINO_MIDDLEWARE_REPO_PATH}/soc/common/hal/gpio_hal.c
        ${ARMINO_MIDDLEWARE_REPO_PATH}/soc/${SOC}/hal/gpio_hal_debug.c
        ${ARMINO_MIDDLEWARE_REPO_PATH}/soc/${SOC}/hal/gpio_ll.c

        ${ARMINO_MIDDLEWARE_REPO_PATH}/soc/common/hal/flash_hal.c
        ${ARMINO_MIDDLEWARE_REPO_PATH}/soc/${SOC}/hal/flash_hal_debug.c
        ${ARMINO_MIDDLEWARE_REPO_PATH}/soc/${SOC}/hal/flash_ll.c

        ${ARMINO_MIDDLEWARE_REPO_PATH}/soc/${SOC}/hal/sys_hal.c
)

set(s_srcs
        driver/bk_tfm_mpc.c
        driver/bk_tfm_log.c

        ${ARMINO_MIDDLEWARE_REPO_PATH}/driver/mpc/mpc_driver.c
        ${ARMINO_MIDDLEWARE_REPO_PATH}/soc/common/hal/mpc_hal.c
        ${ARMINO_MIDDLEWARE_REPO_PATH}/soc/${SOC}/hal/mpc_hal_debug.c
)

set(s_incs
        driver
        ${ARMINO_MIDDLEWARE_REPO_PATH}/driver/mpc
)

target_include_directories(platform_s PUBLIC ${incs} ${s_incs})
target_sources(platform_s PRIVATE ${srcs} ${s_srcs})

target_include_directories(platform_ns PUBLIC ${incs})
target_sources(platform_ns PRIVATE ${srcs})

if(BL2)
	set(bl2_srcs driver/bk_tfm_flash.c)
	set(bl2_incs driver)
	target_include_directories(platform_bl2 PUBLIC ${incs} ${bl2_incs})
	target_sources(platform_bl2 PRIVATE ${srcs} ${bl2_srcs})
endif()

if(NS)
    #TODO wangzhilei
    target_compile_definitions(platform_ns
    	PUBLIC
    	CONFIG_TFM_NS
    	DOMAIN_NS
    )
endif()

