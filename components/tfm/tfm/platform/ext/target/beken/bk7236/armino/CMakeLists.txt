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

        ${ARMINO_MIDDLEWARE_REPO_PATH}/soc/common/soc/include
        ${ARMINO_MIDDLEWARE_REPO_PATH}/soc/common/hal/include
        ${ARMINO_MIDDLEWARE_REPO_PATH}/soc/${SOC}/soc
        ${ARMINO_MIDDLEWARE_REPO_PATH}/soc/${SOC}/hal
)

set(srcs
        ${ARMINO_MIDDLEWARE_REPO_PATH}/driver/port/os_port.c
        ${ARMINO_MIDDLEWARE_REPO_PATH}/driver/port/mem_port.c

        ${ARMINO_MIDDLEWARE_REPO_PATH}/driver/uart/uart_driver.c
        ${ARMINO_MIDDLEWARE_REPO_PATH}/driver/uart/uart_statis.c
        ${ARMINO_MIDDLEWARE_REPO_PATH}/driver/gpio/gpio_driver_base.c
        ${ARMINO_MIDDLEWARE_REPO_PATH}/driver/${SOC}/gpio_driver.c
        ${ARMINO_MIDDLEWARE_REPO_PATH}/driver/spinlock/amp_res_lock.c
        ${ARMINO_MIDDLEWARE_REPO_PATH}/driver/icu/icu_driver.c
        ${ARMINO_MIDDLEWARE_REPO_PATH}/driver/flash/flash_driver.c

        ${ARMINO_MIDDLEWARE_REPO_PATH}/soc/common/hal/uart_hal.c
        ${ARMINO_MIDDLEWARE_REPO_PATH}/soc/${SOC}/hal/uart_hal_debug.c
        ${ARMINO_MIDDLEWARE_REPO_PATH}/soc/${SOC}/hal/uart_ll.c

        ${ARMINO_MIDDLEWARE_REPO_PATH}/soc/common/hal/gpio_hal.c
        ${ARMINO_MIDDLEWARE_REPO_PATH}/soc/${SOC}/hal/gpio_hal_debug.c
        ${ARMINO_MIDDLEWARE_REPO_PATH}/soc/${SOC}/hal/gpio_ll.c

        ${ARMINO_MIDDLEWARE_REPO_PATH}/soc/common/hal/flash_hal.c
        ${ARMINO_MIDDLEWARE_REPO_PATH}/soc/${SOC}/hal/flash_hal_debug.c
        ${ARMINO_MIDDLEWARE_REPO_PATH}/soc/${SOC}/hal/flash_ll.c
)

set(s_srcs
        driver/bk_mpc.c

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
	target_include_directories(platform_bl2 PUBLIC ${incs})
	target_sources(platform_bl2 PRIVATE ${srcs})
endif()


