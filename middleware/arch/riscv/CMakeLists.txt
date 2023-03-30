armino_build_get_property(target ARMINO_SOC)

if("${CONFIG_SOC_ARCH}" STREQUAL ${COMPONENT_NAME})
set(incs 
   . 
   include
   include/bk_private)

set(srcs
	boot_vectors_base.S
	riscv_utility.S
	stack_base.c
	trap_base.c
	arch_interrupt.c
	reset.c
	pmp.c
	initfini.c
	riscv.c
	mon_call.c
	)

#if(CONFIG_CACHE_ENABLE)
	list(APPEND srcs
		cache.c
	)
#endif()

endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES bk_common)

