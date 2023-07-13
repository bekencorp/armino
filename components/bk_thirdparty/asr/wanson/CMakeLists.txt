if (CONFIG_ASR)

armino_build_get_property(armino_soc ARMINO_SOC)

set(incs include)

endif()

armino_component_register(INCLUDE_DIRS "${incs}")

if (CONFIG_ASR)
	get_filename_component(lib_name ${CMAKE_CURRENT_SOURCE_DIR}/lib/libasr.a NAME)
	add_prebuilt_library(${lib_name} "${CMAKE_CURRENT_SOURCE_DIR}/lib/libasr.a")
	target_link_libraries(${COMPONENT_LIB} INTERFACE ${lib_name})
endif()
