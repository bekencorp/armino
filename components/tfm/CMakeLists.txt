armino_build_get_property(target ARMINO_SOC)

if(NOT "${target}" STREQUAL "bk7236")
    return()
endif()

set(incs)
set(srcs)

if (CONFIG_TFM)
	list(APPEND srcs
		tfm_ns_interface.c
		tfm/interface/src/tfm_psa_ns_api.c
	)
	list(APPEND incs
		tfm/interface/include
		tfm/secure_fw/include
		${CMAKE_BINARY_DIR}/armino/tfm/generated/interface/include
	)
endif()

if (CONFIG_TFM_SYSTEM_CLOCK)
	list(APPEND srcs
		tfm/interface/src/tfm_system_clock_ipc_api.c
	)
endif()

if (CONFIG_TFM_FWU)
	list(APPEND srcs
		tfm/interface/src/tfm_firmware_update_ipc_api.c
	)
endif()

armino_component_register(SRCS "${srcs}"
	INCLUDE_DIRS "${incs}"
	REQUIRES partitions)

if (CONFIG_TFM)
armino_build_get_property(partition_hdr_dir PARTITIONS_HDR_DIR)
set(armino_dir ${COMPONENT_DIR}/../..)
set(tfm_dir ${COMPONENT_DIR}/tfm)
set(tfm_platform beken/${target})
set(tfm_toolchain_path ${CONFIG_TOOLCHAIN_PATH})
set(tfm_build_dir ${CMAKE_BINARY_DIR}/armino/tfm)
set(tfm_install_dir ${CMAKE_BINARY_DIR}/install)

include(ExternalProject)
ExternalProject_Add(tfm_project
	SOURCE_DIR ${tfm_dir}
	CONFIGURE_COMMAND ""
	BUILD_IN_SOURCE 1
	CMAKE_ARGS
	BUILD_COMMAND
		cmake
		-G Ninja
		-B ${tfm_build_dir}
		-DCMAKE_PREFIX_PATH=${tfm_toolchain_path}
		-DCMAKE_INSTALL_PREFIX=${tfm_install_dir}
		-DARMINO_SDK_PATH=${armino_path}
		-DTFM_PLATFORM=${tfm_platform}
		-DTFM_TOOLCHAIN_FILE=toolchain_GNUARM.cmake
		-DCMAKE_BUILD_TYPE=${CONFIG_TFM_BUILD_TYPE}
		-DTEST_S=${CONFIG_TFM_TEST_S}
		-DTEST_NS=${CONFIG_TFM_TEST_NS}
		-DUSER=$ENV{USER}
		-DBL2=${CONFIG_TFM_BL2}
		-DTFM_PROFILE=profile_medium
		-DFPGA=${CONFIG_TFM_FPGA}
		-DOTP_NV_COUNTERS_RAM_EMULATION=${CONFIG_OTP_NV_COUNTERS_RAM_EMULATION}
		-DPLATFORM_DEFAULT_OTP_WRITEABLE=${CONFIG_PLATFORM_DEFAULT_OTP_WRITEABLE}
		-DMCUBOOT_LOG_LEVEL=${CONFIG_TFM_BL2_LOG_LEVEL}
		-DPARTITION_HDR_DIR=${partition_hdr_dir}
		&& cd ${tfm_build_dir}
		&& ninja -- install
	BUILD_BYPRODUCTS ${tfm_build_dir}/secure_fw/libtfm_s_veneers.a
	BUILD_ALWAYS 1
	INSTALL_COMMAND ""
	)

add_dependencies(${COMPONENT_LIB} tfm_project)
add_library(lib_tfm STATIC IMPORTED GLOBAL)
add_dependencies(lib_tfm tfm_project)
if (CONFIG_TFM_SYSTEM_CLOCK OR CONFIG_TFM_FWU)
set_target_properties(lib_tfm PROPERTIES IMPORTED_LOCATION ${tfm_build_dir}/secure_fw/libtfm_s_veneers.a)
target_link_libraries(${COMPONENT_LIB} PUBLIC lib_tfm)
target_compile_definitions(${COMPONENT_LIB}
	PRIVATE
		TFM_PSA_API
)
endif()
endif()
