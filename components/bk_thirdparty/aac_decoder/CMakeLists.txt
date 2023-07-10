if (CONFIG_AAC_DECODER)

armino_build_get_property(armino_soc ARMINO_SOC)

set(incs include)

endif()

armino_component_register(INCLUDE_DIRS "${incs}")

if (CONFIG_AAC_DECODER)
	if (${CONFIG_SOC_ARCH} STREQUAL "riscv")
		string(FIND ${CONFIG_TOOLCHAIN_PATH} "nds32le-elf-mculib-v5f" res)
		if (${res} STREQUAL "-1")
			get_filename_component(lib_name ${CMAKE_CURRENT_SOURCE_DIR}/lib/nds32le-elf-mculib-v5/libaac_decoder.a NAME)
			add_prebuilt_library(${lib_name} "${CMAKE_CURRENT_SOURCE_DIR}/lib/nds32le-elf-mculib-v5/libaac_decoder.a")
			#
		else()
			get_filename_component(lib_name ${CMAKE_CURRENT_SOURCE_DIR}/lib/nds32le-elf-mculib-v5f/libaac_decoder.a NAME)
			add_prebuilt_library(${lib_name} "${CMAKE_CURRENT_SOURCE_DIR}/lib/nds32le-elf-mculib-v5f/libaac_decoder.a")
		endif()
	endif()
	target_link_libraries(${COMPONENT_LIB} INTERFACE ${lib_name})
endif()
