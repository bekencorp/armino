armino_build_get_property(target ARMINO_SOC)

if("${CONFIG_SOC_ARCH}" STREQUAL ${COMPONENT_NAME})
set(incs 
   . 
   include
   include/bk_private)

set(srcs
	boot_handlers_base.S
	riscv_utility.S
	stack_base.c
	trap_base.c
	arch_interrupt.c
	reset.c
	pmp.c
	initfini.c
	riscv.c
	)

#if(CONFIG_CACHE_ENABLE)
	list(APPEND srcs
		cache.c
	)
#endif()

if(CONFIG_FREERTOS)
	list(APPEND incs os/freertos)
	list(APPEND srcs
		os/freertos/cpu.c
		boot_vectors_base.S
	)
endif()

if(CONFIG_LITEOS_M_V3)
	list(APPEND srcs
		boot_vectors_liteos_m.S
	)
endif()
endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES bk_common)
if(CONFIG_ARCH_RISCV)
target_compile_options(${COMPONENT_LIB} PRIVATE -mno-relax)
endif()