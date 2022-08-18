armino_build_get_property(target ARMINO_SOC)

if("${CONFIG_SOC_ARCH}" STREQUAL ${COMPONENT_NAME})
set(incs
	.
	include/bk_private
	# os/freertos
	)

set(srcs
	boot_handlers_base.S
	boot_vectors_base.S
	stack_base.c
	trap_base.c
	# os/freertos/boot_handlers.S
	# os/freertos/cpu.c
	arm.c
	arch_interrupt.c
	)

if(CONFIG_FREERTOS)
	list(APPEND incs os/freertos)
	list(APPEND srcs os/freertos/boot_handlers.S os/freertos/cpu.c)
elseif(CONFIG_LITEOS_M)
	list(APPEND incs os/liteos_m)
	list(APPEND srcs os/liteos_m/boot_handlers.S os/liteos_m/cpu.c)
elseif(CONFIG_NON_OS)
	list(APPEND incs os/non_os)
	list(APPEND srcs os/non_os/boot_handlers.S os/non_os/cpu.c)
endif()
endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES bk_common)

