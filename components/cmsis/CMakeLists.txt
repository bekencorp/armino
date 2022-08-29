set(incs)
set(srcs)

if (CONFIG_CMSIS)
	list(APPEND srcs
		CMSIS_5/Device/ArmChina/STAR/Source/startup_STAR.c
		CMSIS_5/Device/ArmChina/STAR/Source/system_STAR.c
		)

	list(APPEND incs
		CMSIS_5/CMSIS/Core/Include
		CMSIS_5/Device/ArmChina/STAR/Include
		CMSIS_5/Device/ArmChina/STAR/Include/Template
		)
endif()

armino_component_register(SRCS "${srcs}"
			INCLUDE_DIRS "${incs}"
			PRIV_REQUIRES bk_common)

add_definitions(-DSTAR_SE -D__ARM_ARCH_8M_MAIN__=1)
