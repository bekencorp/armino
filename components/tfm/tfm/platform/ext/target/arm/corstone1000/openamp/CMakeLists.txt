#-------------------------------------------------------------------------------
# Copyright (c) 2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

add_subdirectory(ext/libmetal)
add_subdirectory(ext/libopenamp)

set(CMAKE_SYSTEM_PROCESSOR  "arm")
set(MACHINE  "template")
set(LIBMETAL_INCLUDE_DIR "${LIBMETAL_BIN_PATH}/lib/include")
set(LIBMETAL_LIB "${LIBMETAL_BIN_PATH}/lib")

add_subdirectory(${LIBMETAL_SRC_PATH} ${LIBMETAL_BIN_PATH})
add_subdirectory(${LIBOPENAMP_SRC_PATH} ${LIBOPENAMP_BIN_PATH})

target_include_directories(platform_s
    PRIVATE
        ${LIBMETAL_BIN_PATH}/lib/include
        ${LIBOPENAMP_SRC_PATH}/lib/include
)

target_include_directories(platform_s
    PUBLIC
        .
)

target_sources(platform_s
    PRIVATE
        tfm_spe_openamp_platform_interconnect.c
        tfm_spe_dual_core_psa_client_secure_lib.c
        tfm_spe_openamp_interface_impl.c
        platform_spe_dual_core_hal.c
        tfm_spe_psa_client_lib_unordered_map.c
)

target_link_libraries(open_amp-static
    PRIVATE
        metal-static
)

target_compile_definitions(open_amp-static
    PRIVATE
        RPMSG_BUFFER_SIZE=8192
)

target_link_libraries(platform_s
    PRIVATE
        open_amp-static
)

# Export header file shared with non-secure side
install(FILES       tfm_openamp_lib.h
        DESTINATION ${TFM_INSTALL_PATH}/interface/include
)
