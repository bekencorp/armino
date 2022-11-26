set(incs)
set(srcs)

if (CONFIG_CMSIS)
	list(APPEND srcs
		port/start.c
	)

	list(APPEND incs
		CMSIS_5/CMSIS/Core/Include
	)

if (CONFIG_SOC_BK7236)
	list(APPEND srcs
		CMSIS_5/Device/Beken/bk7236xx/Source/startup_bk7236.c
		CMSIS_5/Device/Beken/bk7236xx/Source/system_bk7236xx.c
	)

	list(APPEND incs
		CMSIS_5/Device/Beken/bk7236xx/Include
		CMSIS_5/Device/Beken/bk7236xx/Include/Template
	)
endif()
endif()

armino_component_register(SRCS "${srcs}"
			INCLUDE_DIRS "${incs}"
			PRIV_REQUIRES bk_common coredump)

add_definitions(-D__ARM_ARCH_8M_MAIN__=1)
