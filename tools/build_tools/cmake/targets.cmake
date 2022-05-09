#
# Set the target used for the standard project build.
#
macro(__target_init)
    # Input is ARMINO_SOC environement variable
    set(env_armino_target $ENV{ARMINO_SOC})

    if(NOT env_armino_target)
        # ARMINO_SOC not set in environment, see if it is set in cache
        if(ARMINO_SOC)
            set(env_armino_target ${ARMINO_SOC})
        else()
            LOGE("ARMINO_SOC not set, using default target: ${env_armino_target}")
        endif()
    else()
        # ARMINO_SOC set both in environment and in cache, must be the same
        if(NOT ${ARMINO_SOC} STREQUAL ${env_armino_target})
            LOGE("ARMINO_SOC in CMake cache does not match "
                            "ARMINO_SOC environment variable. To change the target, clear "
                            "the build directory and sdkconfig file, and build the project again")
        endif()
    endif()

    # ARMINO_SOC will be used by Kconfig, make sure it is set
    set(ENV{ARMINO_SOC} ${env_armino_target})

    # Finally, set ARMINO_SOC in cache
    set(ARMINO_SOC ${env_armino_target} CACHE STRING "ARMINO Build Target")

    # Check if ARMINO_ENV_FPGA environment is set
    set(env_armino_env_fpga $ENV{ARMINO_ENV_FPGA})
    if(${env_armino_env_fpga})
        armino_build_set_property(__ARMINO_ENV_FPGA "y")
        LOGI("ARMINO_ENV_FPGA is set, building for FPGA environment")
    endif()
endmacro()

#
# Check that the set build target and the config target matches.
#
function(__target_check)
    # Should be called after sdkconfig CMake file has been included.
    armino_build_get_property(armino_target ARMINO_SOC)
    armino_build_get_property(soc_path SOC_PATH)

    if (EXISTS ${soc_path}/${ARMINO_SOC}/${ARMINO_SOC}.defconfig)
        set(config_armino_target "${ARMINO_SOC}")
    else()
        message(SEND_ERROR "${ARMINO_SOC}" ": Miss")
    endif()

    if(NOT ${armino_target} STREQUAL ${config_armino_target})
        LOGE("CONFIG_SOC_BKxxx(${config_armino_target}) in sdkconfig does not match "
            "ARMINO_SOC(${armino_target}) environment variable. To change the target, delete "
            "sdkconfig file and build the project again.")
    endif()
endfunction()

#
# Used by the project CMake file to set the toolchain before project() call.
#
macro(__target_set_toolchain)
    armino_build_get_property(armino_path ARMINO_PATH)
    armino_build_get_property(soc_path SOC_PATH)
    # First try to load the toolchain file from the tools/cmake/directory of ARMINO
    set(toolchain_file_global ${soc_path}/${ARMINO_SOC}/toolchain-${ARMINO_SOC}.cmake)
    if(EXISTS ${toolchain_file_global})
	LOGI("global toolchain file: ${toolchain_file_global}")
        set(CMAKE_TOOLCHAIN_FILE ${toolchain_file_global})
    else()
        __component_get_target(component_target ${ARMINO_SOC})
        if(NOT component_target)
            LOGE("Unable to resolve '${ARMINO_SOC}' for setting toolchain file.")
        endif()
        get_property(component_dir TARGET ${component_target} PROPERTY COMPONENT_DIR)
        # Try to load the toolchain file from the directory of ARMINO_SOC component
        set(toolchain_file_component ${component_dir}/toolchain-${ARMINO_SOC}.cmake)
        if(EXISTS ${toolchain_file_component})
            set(CMAKE_TOOLCHAIN_FILE ${toolchain_file_component})
        else()
            LOGE("Toolchain file toolchain-${ARMINO_SOC}.cmake not found,"
                    "checked ${toolchain_file_global} and ${toolchain_file_component}")
        endif()
    endif()
endmacro()
