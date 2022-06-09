#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
# Copyright (c) 2021, NXP Semiconductors. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

#========================= Pull MCUxpresso NXP SDK drivers from https://github.com/NXPmicro/mcux-sdk =========================#
file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/drivers/casper/fsl_casper.c  ${NXP_HAL_FILE_PATH}/common/Native_Driver/drivers/fsl_casper.c)
file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/drivers/casper/fsl_casper.h  ${NXP_HAL_FILE_PATH}/common/Native_Driver/drivers/fsl_casper.h)

file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/drivers/common/fsl_common.c  ${NXP_HAL_FILE_PATH}/common/Native_Driver/drivers/fsl_common.c)
file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/drivers/common/fsl_common.h  ${NXP_HAL_FILE_PATH}/common/Native_Driver/drivers/fsl_common.h)

file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/drivers/common/fsl_common_arm.c  ${NXP_HAL_FILE_PATH}/common/Native_Driver/drivers/fsl_common_arm.c)
file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/drivers/common/fsl_common_arm.h  ${NXP_HAL_FILE_PATH}/common/Native_Driver/drivers/fsl_common_arm.h)

file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/drivers/ctimer/fsl_ctimer.c  ${NXP_HAL_FILE_PATH}/common/Native_Driver/drivers/fsl_ctimer.c)
file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/drivers/ctimer/fsl_ctimer.h  ${NXP_HAL_FILE_PATH}/common/Native_Driver/drivers/fsl_ctimer.h)

file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/drivers/flexcomm/fsl_flexcomm.c  ${NXP_HAL_FILE_PATH}/common/Native_Driver/drivers/fsl_flexcomm.c)
file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/drivers/flexcomm/fsl_flexcomm.h  ${NXP_HAL_FILE_PATH}/common/Native_Driver/drivers/fsl_flexcomm.h)

file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/drivers/lpc_gpio/fsl_gpio.c  ${NXP_HAL_FILE_PATH}/common/Native_Driver/drivers/fsl_gpio.c)
file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/drivers/lpc_gpio/fsl_gpio.h  ${NXP_HAL_FILE_PATH}/common/Native_Driver/drivers/fsl_gpio.h)

file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/drivers/hashcrypt/fsl_hashcrypt.c  ${NXP_HAL_FILE_PATH}/common/Native_Driver/drivers/fsl_hashcrypt.c)
file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/drivers/hashcrypt/fsl_hashcrypt.h  ${NXP_HAL_FILE_PATH}/common/Native_Driver/drivers/fsl_hashcrypt.h)

file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/drivers/iap1/fsl_iap.c  ${NXP_HAL_FILE_PATH}/common/Native_Driver/drivers/fsl_iap.c)
file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/drivers/iap1/fsl_iap.h  ${NXP_HAL_FILE_PATH}/common/Native_Driver/drivers/fsl_iap.h)
file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/drivers/iap1/fsl_iap_kbp.h  ${NXP_HAL_FILE_PATH}/common/Native_Driver/drivers/fsl_iap_kbp.h)
file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/drivers/iap1/fsl_iap_skboot_authenticate.h  ${NXP_HAL_FILE_PATH}/common/Native_Driver/drivers/fsl_iap_skboot_authenticate.h)

file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/drivers/iap1/fsl_iap_ffr.h  ${NXP_HAL_FILE_PATH}/common/Native_Driver/drivers/fsl_iap_ffr.h)

file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/drivers/lpc_iocon/fsl_iocon.h ${NXP_HAL_FILE_PATH}/common/Native_Driver/drivers/fsl_iocon.h)

file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/drivers/rng_1/fsl_rng.c  ${NXP_HAL_FILE_PATH}/common/Native_Driver/drivers/fsl_rng.c)
file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/drivers/rng_1/fsl_rng.h ${NXP_HAL_FILE_PATH}/common/Native_Driver/drivers/fsl_rng.h)

file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/drivers/flexcomm/fsl_usart.c ${NXP_HAL_FILE_PATH}/common/Native_Driver/drivers/fsl_usart.c)
file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/drivers/flexcomm/fsl_usart.h ${NXP_HAL_FILE_PATH}/common/Native_Driver/drivers/fsl_usart.h)

#========================= Pull MCUxpresso NXP SDK components from https://github.com/NXPmicro/mcux-sdk =========================#
file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/components/lists/fsl_component_generic_list.c ${NXP_HAL_FILE_PATH}/common/Native_Driver/components/lists/fsl_component_generic_list.c)
file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/components/lists/fsl_component_generic_list.h ${NXP_HAL_FILE_PATH}/common/Native_Driver/components/lists/fsl_component_generic_list.h)

file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/components/serial_manager/fsl_component_serial_manager.c ${NXP_HAL_FILE_PATH}/common/Native_Driver/components/serial_manager/fsl_component_serial_manager.c)
file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/components/serial_manager/fsl_component_serial_manager.h ${NXP_HAL_FILE_PATH}/common/Native_Driver/components/serial_manager/fsl_component_serial_manager.h)

file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/components/serial_manager/fsl_component_serial_port_internal.h ${NXP_HAL_FILE_PATH}/common/Native_Driver/components/serial_manager/fsl_component_serial_port_internal.h)

file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/components/serial_manager/fsl_component_serial_port_uart.c ${NXP_HAL_FILE_PATH}/common/Native_Driver/components/serial_manager/fsl_component_serial_port_uart.c)
file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/components/serial_manager/fsl_component_serial_port_uart.h ${NXP_HAL_FILE_PATH}/common/Native_Driver/components/serial_manager/fsl_component_serial_port_uart.h)

file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/components/uart/fsl_adapter_uart.h ${NXP_HAL_FILE_PATH}/common/Native_Driver/components/uart/fsl_adapter_uart.h)

file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/components/uart/fsl_adapter_usart.c ${NXP_HAL_FILE_PATH}/common/Native_Driver/components/uart/fsl_adapter_usart.c)

#========================= Pull MCUxpresso NXP SDK utilities from https://github.com/NXPmicro/mcux-sdk =========================#
file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/utilities/debug_console/debug_console/fsl_debug_console.c ${NXP_HAL_FILE_PATH}/common/Native_Driver/utilities/debug_console/fsl_debug_console.c)
file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/utilities/debug_console/debug_console/fsl_debug_console.h ${NXP_HAL_FILE_PATH}/common/Native_Driver/utilities/debug_console/fsl_debug_console.h)
file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/utilities/debug_console/debug_console/fsl_debug_console_conf.h ${NXP_HAL_FILE_PATH}/common/Native_Driver/utilities/debug_console/fsl_debug_console_conf.h)

file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/utilities/debug_console/str/fsl_str.c ${NXP_HAL_FILE_PATH}/common/Native_Driver/utilities/str/fsl_str.c)
file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/utilities/debug_console/str/fsl_str.h ${NXP_HAL_FILE_PATH}/common/Native_Driver/utilities/str/fsl_str.h)

file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/utilities/assert/fsl_assert.c ${NXP_HAL_FILE_PATH}/common/Native_Driver/utilities/fsl_assert.c)

#========================= Pull MCUxpresso NXP SDK devices from https://github.com/NXPmicro/mcux-sdk =========================#
file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/devices/LPC55S69/fsl_device_registers.h ${NXP_HAL_FILE_PATH}/lpcxpresso55s69/Native_Driver/fsl_device_registers.h)

file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/devices/LPC55S69/LPC55S69_cm33_core0.h ${NXP_HAL_FILE_PATH}/lpcxpresso55s69/Native_Driver/LPC55S69_cm33_core0.h)
file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/devices/LPC55S69/LPC55S69_cm33_core0_features.h ${NXP_HAL_FILE_PATH}/lpcxpresso55s69/Native_Driver/LPC55S69_cm33_core0_features.h)

file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/devices/LPC55S69/LPC55S69_cm33_core1.h ${NXP_HAL_FILE_PATH}/lpcxpresso55s69/Native_Driver/LPC55S69_cm33_core1.h)
file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/devices/LPC55S69/LPC55S69_cm33_core1_features.h ${NXP_HAL_FILE_PATH}/lpcxpresso55s69/Native_Driver/LPC55S69_cm33_core1_features.h)

file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/devices/LPC55S69/system_LPC55S69_cm33_core0.c ${NXP_HAL_FILE_PATH}/lpcxpresso55s69/Native_Driver/system_LPC55S69_cm33_core0.c)
file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/devices/LPC55S69/system_LPC55S69_cm33_core0.h ${NXP_HAL_FILE_PATH}/lpcxpresso55s69/Native_Driver/system_LPC55S69_cm33_core0.h)

file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/devices/LPC55S69/utilities/fsl_notifier.c ${NXP_HAL_FILE_PATH}/lpcxpresso55s69/Native_Driver/utilities/fsl_notifier.c)
file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/devices/LPC55S69/utilities/fsl_notifier.h ${NXP_HAL_FILE_PATH}/lpcxpresso55s69/Native_Driver/utilities/fsl_notifier.h)

file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/devices/LPC55S69/utilities/misc_utilities/fsl_sbrk.c ${NXP_HAL_FILE_PATH}/common/Native_Driver/utilities/fsl_sbrk.c)

file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/devices/LPC55S69/utilities/fsl_shell.c ${NXP_HAL_FILE_PATH}/lpcxpresso55s69/Native_Driver/utilities/fsl_shell.c)
file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/devices/LPC55S69/utilities/fsl_shell.h ${NXP_HAL_FILE_PATH}/lpcxpresso55s69/Native_Driver/utilities/fsl_shell.h)

file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/devices/LPC55S69/drivers/fsl_power.c  ${NXP_HAL_FILE_PATH}/lpcxpresso55s69/Native_Driver/drivers/fsl_power.c)
file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/devices/LPC55S69/drivers/fsl_power.h ${NXP_HAL_FILE_PATH}/lpcxpresso55s69/Native_Driver/drivers/fsl_power.h)

file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/devices/LPC55S69/drivers/fsl_reset.c  ${NXP_HAL_FILE_PATH}/lpcxpresso55s69/Native_Driver/drivers/fsl_reset.c)
file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/devices/LPC55S69/drivers/fsl_reset.h ${NXP_HAL_FILE_PATH}/lpcxpresso55s69/Native_Driver/drivers/fsl_reset.h)

file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/devices/LPC55S69/drivers/fsl_clock.c  ${NXP_HAL_FILE_PATH}/lpcxpresso55s69/Native_Driver/drivers/fsl_clock.c)
file(DOWNLOAD https://raw.githubusercontent.com/NXPmicro/mcux-sdk/${NXP_SDK_GIT_TAG}/devices/LPC55S69/drivers/fsl_clock.h ${NXP_HAL_FILE_PATH}/lpcxpresso55s69/Native_Driver/drivers/fsl_clock.h)