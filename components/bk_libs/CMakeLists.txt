armino_build_get_property(armino_soc ARMINO_SOC)
set(incs include)
armino_component_register(INCLUDE_DIRS "${incs}")

armino_build_get_property(toolchain_dir TOOLCHAIN_DIR)
set(gcc_compiler ${toolchain_dir}/riscv32-elf-gcc)
set(toolchain_version_file ${CMAKE_CURRENT_SOURCE_DIR}/${armino_soc}_$ENV{PROJECT_LIBS}/hash/toolchain_version.txt)
set(lib_hash $ENV{LIB_HASH})
set(app_version $ENV{APP_VERSION})
set(cmd_target $ENV{CMD_TARGET})
if("${lib_hash}" STREQUAL "NULL")
	execute_process(COMMAND
					"${gcc_compiler}" --version
					OUTPUT_VARIABLE sdk_toolchain_version
					RESULT_VARIABLE result)
	file(WRITE ${toolchain_version_file} "${sdk_toolchain_version}")
else()
	execute_process(COMMAND
					cat ${CMAKE_CURRENT_SOURCE_DIR}/${armino_soc}_$ENV{PROJECT_LIBS}/hash/toolchain_version.txt
					OUTPUT_VARIABLE sdk_toolchain_version
					RESULT_VARIABLE result)
	execute_process(COMMAND
					cat ${CMAKE_CURRENT_SOURCE_DIR}/${armino_soc}_$ENV{PROJECT_LIBS}/hash/libs_hash.txt
					OUTPUT_VARIABLE sdk_lib_hash
					RESULT_VARIABLE result)
	execute_process(COMMAND
					"${gcc_compiler}" --version
					OUTPUT_VARIABLE toolchain_version
					RESULT_VARIABLE result)
	STRING(STRIP "${sdk_toolchain_version}" sdk_toolchain_version)
	STRING(STRIP "${sdk_lib_hash}" sdk_lib_hash)
	STRING(STRIP "${toolchain_version}" toolchain_version)
	if(sdk_lib_hash STREQUAL lib_hash AND sdk_toolchain_version STREQUAL toolchain_version)
		message(STATUS "bk_libs and toolchain version verify success...")
	elseif(app_version STREQUAL "test")
		message(STATUS "bk_libs or toolchain version verify failed, skip for app test!!!")
	elseif(cmd_target STREQUAL "menuconfig")
		message(STATUS "bk_libs or toolchain version verify failed, skip for menuconfig!!!")
	else()
		message(SEND_ERROR "bk_libs or toolchain version verify failed, stop!!!")
	endif()
endif()

file(GLOB common_libs ${CMAKE_CURRENT_SOURCE_DIR}/common/*.a)
foreach(lib ${common_libs})
	get_filename_component(lib_name ${lib} NAME)
	add_prebuilt_library(${lib_name} "${CMAKE_CURRENT_SOURCE_DIR}/common/${lib_name}")
	target_link_libraries(${COMPONENT_LIB} INTERFACE ${lib_name})
endforeach()

file(GLOB libs ${CMAKE_CURRENT_SOURCE_DIR}/${armino_soc}_$ENV{PROJECT_LIBS}/libs/*.a)

foreach(lib ${libs})
 	get_filename_component(lib_name ${lib} NAME)
 	add_prebuilt_library(${lib_name} "${CMAKE_CURRENT_SOURCE_DIR}/${armino_soc}_$ENV{PROJECT_LIBS}/libs/${lib_name}")
 	target_link_libraries(${COMPONENT_LIB} INTERFACE ${lib_name})
endforeach()

