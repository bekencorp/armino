# armino_build_get_property
#
# @brief Retrieve the value of the specified property related to BEKEN-ARMINO build.
#
# @param[out] var the variable to store the value in
# @param[in] property the property to get the value of
#
# @param[in, optional] GENERATOR_EXPRESSION (option) retrieve the generator expression for the property
#                   instead of actual value
function(armino_build_get_property var property)
    cmake_parse_arguments(_ "GENERATOR_EXPRESSION" "" "" ${ARGN})
    if(__GENERATOR_EXPRESSION)
        set(val "$<TARGET_PROPERTY:__armino_build_target,${property}>")
    else()
        get_property(val TARGET __armino_build_target PROPERTY ${property})
    endif()
    set(${var} ${val} PARENT_SCOPE)
endfunction()

# armino_build_set_property
#
# @brief Set the value of the specified property related to BEKEN-ARMINO build. The property is
#        also added to the internal list of build properties if it isn't there already.
#
# @param[in] property the property to set the value of
# @param[out] value value of the property
#
# @param[in, optional] APPEND (option) append the value to the current value of the
#                     property instead of replacing it
function(armino_build_set_property property value)
    cmake_parse_arguments(_ "APPEND" "" "" ${ARGN})

    #unset(__armino_build_target)
    if(__APPEND)
        set_property(TARGET __armino_build_target APPEND PROPERTY ${property} ${value})
    else()
        set_property(TARGET __armino_build_target PROPERTY ${property} ${value})

        armino_build_get_property(target BKD_TARGET)
    endif()

    # Keep track of set build properties so that they can be exported to a file that
    # will be included in early expansion script.
    armino_build_get_property(build_properties __BUILD_PROPERTIES)
    if(NOT property IN_LIST build_properties)
        armino_build_set_property(__BUILD_PROPERTIES "${property}" APPEND)
    endif()
endfunction()

# armino_build_unset_property
#
# @brief Unset the value of the specified property related to BEKEN-ARMINO build. Equivalent
#        to setting the property to an empty string; though it also removes the property
#        from the internal list of build properties.
#
# @param[in] property the property to unset the value of
function(armino_build_unset_property property)
    armino_build_set_property(${property} "") # set to an empty value
    armino_build_get_property(build_properties __BUILD_PROPERTIES) # remove from tracked properties
    list(REMOVE_ITEM build_properties ${property})
    armino_build_set_property(__BUILD_PROPERTIES "${build_properties}")
endfunction()

#
# Retrieve the ARMINO_PATH repository's version, either using a version
# file or Git revision. Sets the ARMINO_VER build property.
#
function(__build_get_armino_git_revision)
    armino_build_get_property(armino_path ARMINO_PATH)
    git_describe(armino_ver_git "${armino_path}")
    if(EXISTS "${armino_path}/version.txt")
        file(STRINGS "${armino_path}/version.txt" armino_ver_t)
        set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${armino_path}/version.txt")
    else()
        set(armino_ver_t ${armino_ver_git})
    endif()
    # cut ARMINO_VER to required 32 characters.
    string(SUBSTRING "${armino_ver_t}" 0 31 armino_ver)
    armino_build_set_property(COMPILE_DEFINITIONS "-DARMINO_VER=\"${armino_ver}\"" APPEND)
    armino_build_set_property(COMPILE_DEFINITIONS "-DARMINO_INTERNAL_LIB_VER=\"${armino_ver}\"" APPEND)
    git_submodule_check("${armino_path}")
    armino_build_set_property(ARMINO_VER ${armino_ver})
endfunction()

#
# Sets initial list of build specifications (compile options, definitions, etc.) common across
# all library targets built under the BEKEN-ARMINO build system. These build specifications are added
# privately using the directory-level CMake commands (add_compile_options, include_directories, etc.)
# during component registration.
#
function(__build_set_default_build_specifications)
    unset(compile_definitions)
    unset(compile_options)
    unset(c_compile_options)
    unset(cxx_compile_options)

    set(armino_target $ENV{ARMINO_SOC})
    if(NOT armino_target)
        if(ARMINO_SOC)
            set(armino_target ${ARMINO_SOC})
        else()
            set(armino_target bk7231n)
            LOGI("ARMINO_SOC not set, using default target: ${armino_target}")
        endif()
    endif()

    list(APPEND compile_definitions "-D_GNU_SOURCE")
	list(APPEND compile_options     "-g"
                                    "-Os"
                                    "-std=c99"
                                    "-nostdlib"

                                    "-Wall"
                                    "-Werror"
                                    "-Wno-address"
                                    "-Wno-array-bounds"
                                    "-Wno-format"
                                    "-Wno-unknown-pragmas"
                                    "-Wno-address-of-packed-member"  # gcc 5 don't recognize no-address-of-packed-member
                                    "-Wno-deprecated-declarations"   # prefer to use -Wno-error=deprecated-declarations

                                    "-ffunction-sections"
                                    "-fsigned-char"
                                    "-fdata-sections"
                                    "-fno-strict-aliasing"

                                    #"-x assembler-with-cpp" #TODO fix it

                                    "-ggdb")

    list(APPEND c_compile_options   "-std=gnu99"
                                    "-Wno-old-style-declaration"
                                    "-nostdlib"
                                    "-ffunction-sections"
                                    "-Wl, --gc-sections"
                                    )

    list(APPEND cxx_compile_options "-std=gnu++11"
                                    "-nostdlib"
                                     )

    armino_build_get_property(soc_path SOC_PATH)
    if (EXISTS ${soc_path}/${armino_target}/compile-options.cmake)
        include (${soc_path}/${armino_target}/compile-options.cmake)

        if (DEFINED OVERRIDE_COMPILE_OPTIONS)
            list(APPEND compile_options ${OVERRIDE_COMPILE_OPTIONS})
        endif()

    endif()

    armino_build_set_property(COMPILE_DEFINITIONS "${compile_definitions}" APPEND)
    armino_build_set_property(COMPILE_OPTIONS "${compile_options}" APPEND)
    armino_build_set_property(C_COMPILE_OPTIONS "${c_compile_options}" APPEND)
    armino_build_set_property(CXX_COMPILE_OPTIONS "${cxx_compile_options}" APPEND)
endfunction()

function(__build_add_components components_path)
    armino_build_get_property(prefix __PREFIX)
    file(GLOB property_dirs ${components_path}/*)
    foreach(property_dir ${property_dirs})
        get_filename_component(property_dir ${property_dir} ABSOLUTE)
        __component_dir_quick_check(is_component ${property_dir})
        if(is_component)
            __component_add(${property_dir} ${prefix})
        endif()

        __component_loop_sub_dirs(is_loop_sub_dirs ${property_dir})

        if(is_loop_sub_dirs)
            execute_process(COMMAND
                find ${property_dir} -maxdepth 8 -name "CMakeLists.txt" -exec  dirname {} \;
                OUTPUT_VARIABLE sub_property_dirs
                RESULT_VARIABLE result
            )

            string(REPLACE "\n" ";" sub_dirs_list ${sub_property_dirs})

            foreach(sub_dir ${sub_dirs_list})
                if(EXISTS ${sub_dir}/CMakeLists.txt)
                    get_filename_component(base_dir ${sub_dir} NAME)
                    __component_dir_quick_check(is_component ${sub_dir})
                    if(is_component)
                        armino_build_component(${sub_dir})
                    endif()
                endif()
            endforeach()
        endif()
    endforeach()
endfunction()

#
# Initialize the build. This gets called upon inclusion of armino.cmake to set internal
# properties used for the processing phase of the build.
#
function(__build_init armino_path)
    # Create the build target, to which the BEKEN-ARMINO build properties, dependencies are attached to.
    # Must be global so as to be accessible from any subdirectory in custom projects.
    add_library(__armino_build_target STATIC IMPORTED GLOBAL)

    set_default(python "python3")

    armino_build_set_property(PYTHON ${python})
    armino_build_set_property(ARMINO_PATH ${armino_path})

    armino_build_set_property(__PREFIX armino)
    armino_build_set_property(__INCLUDE_PREFIX inc)
    armino_build_set_property(__CHECK_PYTHON 1)
    armino_build_set_property(SOC_PATH ${SOC_PATH})

    __build_set_default_build_specifications()

    # Add internal components to the build
    armino_build_get_property(armino_path ARMINO_PATH)
    armino_build_get_property(prefix __PREFIX)
    __build_add_components(${armino_path}/components)
    __build_add_components(${armino_path}/middleware)
    __component_add(${armino_path}/include ${prefix})

    # Set components required by all other components in the build
    set(requires_common include bk_common bk_log bk_event driver bk_rtos common)
    armino_build_set_property(__COMPONENT_REQUIRES_COMMON "${requires_common}")

    armino_build_set_property(COMPILE_DEFINITIONS "-DAPP_VERSION=\"$ENV{APP_VERSION}\"" APPEND)

    #__build_get_armino_git_revision()
    __kconfig_init()

endfunction()

function(__build_check_config)
    if (NOT BUILD_PROPERTIES_LIB)
        set(config_check_tool ${armino_path}/components/bk_libs/config_checking.py)
        set(lib_sdkconfig ${armino_path}/components/bk_libs/${ARMINO_SOC}/config/sdkconfig)
        set(armino_sdkconfig ${CMAKE_BINARY_DIR}/sdkconfig)
        execute_process(
            COMMAND ${config_check_tool} --lib_sdkconfig ${lib_sdkconfig} --armino_sdkconfig ${armino_sdkconfig}
           RESULT_VARIABLE result 
        )

        if ("${result}" EQUAL "0")
            LOGI("sdkconfig checking passed")
        else()
            #TODO fix it after optimze bk_private!
            LOGI("!!!sdkconfig checking failed")
            #LOGE("sdkconfig checking failed")
        endif()
    endif()
endfunction()

# armino_build_component
#
# @brief Present a directory that contains a component to the build system.
#        Relative paths are converted to absolute paths with respect to current directory.
#        All calls to this command must be performed before armino_build_process.
#
# @note  This command does not guarantee that the component will be processed
#        during build (see the COMPONENTS argument description for command armino_build_process)
#
# @param[in] component_dir directory of the component
function(armino_build_component component_dir)
    armino_build_get_property(prefix __PREFIX)
    __component_add(${component_dir} ${prefix} 0)
endfunction()

#
# Resolve the requirement component to the component target created for that component.
#
function(__build_resolve_and_add_req var component_target req type)
    __component_get_target(_component_target ${req})
    __component_get_property(_component_registered ${component_target} __COMPONENT_REGISTERED)

    if(NOT _component_target OR NOT _component_registered)
        __component_get_property(_component_name ${component_target} COMPONENT_NAME)
        __component_get_property(_component_dir ${component_target} COMPONENT_DIR)
        LOGE("Component '${_component_name}' depends on component '${req}', but '${req}' is not registered by armino_component_register()! See ${_component_dir}/CMakeLists.txt")
    endif()
    __component_set_property(${component_target} ${type} ${_component_target} APPEND)
    set(${var} ${_component_target} PARENT_SCOPE)
endfunction()

#
# Build a list of components (in the form of component targets) to be added to the build
# based on public and private requirements. This list is saved in an internal property,
# __BUILD_COMPONENT_TARGETS.
#
function(__build_expand_requirements component_target)
    # Since there are circular dependencies, make sure that we do not infinitely
    # expand requirements for each component.
    armino_build_get_property(component_targets_seen __COMPONENT_TARGETS_SEEN)
    __component_get_property(component_registered ${component_target} __COMPONENT_REGISTERED)
    if(component_target IN_LIST component_targets_seen OR NOT component_registered)
        return()
    endif()

    armino_build_set_property(__COMPONENT_TARGETS_SEEN ${component_target} APPEND)

    get_property(reqs TARGET ${component_target} PROPERTY REQUIRES)
    get_property(priv_reqs TARGET ${component_target} PROPERTY PRIV_REQUIRES)

    foreach(req ${reqs})
        __build_resolve_and_add_req(_component_target ${component_target} ${req} __REQUIRES)
        __build_expand_requirements(${_component_target})
    endforeach()

    foreach(req ${priv_reqs})
        __build_resolve_and_add_req(_component_target ${component_target} ${req} __PRIV_REQUIRES)
        __build_expand_requirements(${_component_target})
    endforeach()

    armino_build_get_property(build_component_targets __BUILD_COMPONENT_TARGETS)
    if(NOT component_target IN_LIST build_component_targets)
        armino_build_set_property(__BUILD_COMPONENT_TARGETS ${component_target} APPEND)

        __component_get_property(component_lib ${component_target} COMPONENT_LIB)
        armino_build_set_property(__BUILD_COMPONENTS ${component_lib} APPEND)

        armino_build_get_property(prefix __PREFIX)
        armino_build_get_property(inc_prefix __INCLUDE_PREFIX)
        __component_get_property(component_prefix ${component_target} __PREFIX)

        __component_get_property(component_alias ${component_target} COMPONENT_ALIAS)

        armino_build_set_property(BUILD_COMPONENT_ALIASES ${component_alias} APPEND)

        # Only put in the prefix in the name if it is not the default one
        if(component_prefix STREQUAL prefix OR component_prefix STREQUAL inc_prefix)
            __component_get_property(component_name ${component_target} COMPONENT_NAME)
            armino_build_set_property(BUILD_COMPONENTS ${component_name} APPEND)
        else()
            armino_build_set_property(BUILD_COMPONENTS ${component_alias} APPEND)
        endif()
    endif()
endfunction()

#
# Write a CMake file containing set build properties, owing to the fact that an internal
# list of properties is maintained in armino_build_set_property() call. This is used to convert
# those set properties to variables in the scope the output file is included in.
#
function(__build_write_properties output_file)
    armino_build_get_property(build_properties __BUILD_PROPERTIES)
    foreach(property ${build_properties})
        armino_build_get_property(val ${property})
        set(build_properties_text "${build_properties_text}\nset(${property} ${val})")
    endforeach()
    file(WRITE ${output_file} "${build_properties_text}")
endfunction()

#
# Check if the Python interpreter used for the build has all the required modules.
#
function(__build_check_python)
    armino_build_get_property(check __CHECK_PYTHON)
    if(check)
        armino_build_get_property(python PYTHON)
        armino_build_get_property(armino_path ARMINO_PATH)
        LOGI("Checking Python dependencies...")
        execute_process(COMMAND "${python}" "${armino_path}/tools/build_tools/check_python_dependencies.py"
            RESULT_VARIABLE result)
        if(NOT result EQUAL 0)
            LOGE("Some Python dependencies must be installed. Check above message for details.")
        endif()
    endif()
endfunction()

#
# Prepare for component processing expanding each component's project include
#
macro(__build_process_project_includes)
    # Include the sdkconfig cmake file, since the following operations require
    # knowledge of config values.
    armino_build_get_property(sdkconfig_cmake SDKCONFIG_CMAKE)
    include(${sdkconfig_cmake})

    # Make each build property available as a read-only variable
    armino_build_get_property(build_properties __BUILD_PROPERTIES)
    foreach(build_property ${build_properties})
        armino_build_get_property(val ${build_property})
        set(${build_property} "${val}")
    endforeach()

    # Check that the CMake target value matches the Kconfig target value.
    __target_check()

    armino_build_get_property(build_component_targets __BUILD_COMPONENT_TARGETS)

    # Include each component's project_include.cmake
    foreach(component_target ${build_component_targets})
        __component_get_property(dir ${component_target} COMPONENT_DIR)
        __component_get_property(_name ${component_target} COMPONENT_NAME)
        set(COMPONENT_NAME ${_name})
        set(COMPONENT_DIR ${dir})
        set(COMPONENT_PATH ${dir})  # this is deprecated, users are encouraged to use COMPONENT_DIR;
                                    # retained for compatibility
        if(EXISTS ${COMPONENT_DIR}/project_include.cmake)
            include(${COMPONENT_DIR}/project_include.cmake)
        endif()
    endforeach()
endmacro()

#
# Utility macro for setting default property value if argument is not specified
# for armino_build_process().
#
macro(__build_set_default var default)
    set(_var __${var})
    if(NOT "${${_var}}" STREQUAL "")
        armino_build_set_property(${var} "${${_var}}")
    else()
        armino_build_set_property(${var} "${default}")
    endif()
    unset(_var)
endmacro()

#
# Import configs as build instance properties so that they are accessible
# using armino_build_get_config(). Config has to have been generated before calling
# this command.
#
function(__build_import_configs)
    # Include the sdkconfig cmake file, since the following operations require
    # knowledge of config values.
    armino_build_get_property(sdkconfig_cmake SDKCONFIG_CMAKE)
    include(${sdkconfig_cmake})

    armino_build_set_property(__CONFIG_VARIABLES "${CONFIGS_LIST}")
    foreach(config ${CONFIGS_LIST})
        set_property(TARGET __armino_build_target PROPERTY ${config} "${${config}}")
    endforeach()
endfunction()

# armino_build_process
#
# @brief Main processing step for BEKEN-ARMINO build: config generation, adding components to the build,
#        dependency resolution, etc.
#
# @param[in] target BEKEN-ARMINO target
#
# @param[in, optional] PROJECT_DIR (single value) directory of the main project the buildsystem
#                      is processed for; defaults to CMAKE_SOURCE_DIR
# @param[in, optional] PROJECT_VER (single value) version string of the main project; defaults
#                      to 1
# @param[in, optional] PROJECT_NAME (single value) main project name, defaults to CMAKE_PROJECT_NAME
# @param[in, optional] SDKCONFIG (single value) sdkconfig output path, defaults to PROJECT_DIR/sdkconfig
#                       if PROJECT_DIR is set and CMAKE_SOURCE_DIR/sdkconfig if not
# @param[in, optional] SDKCONFIG_DEFAULTS (single value) config defaults file to use for the build; defaults
#                       to none (Kconfig defaults or previously generated config are used)
# @param[in, optional] BUILD_DIR (single value) directory for build artifacts; defautls to CMAKE_BINARY_DIR
# @param[in, optional] COMPONENTS (multivalue) select components to process among the components
#                       known by the build system
#                       (added via `armino_build_component`). This argument is used to trim the build.
#                       Other components are automatically added if they are required
#                       in the dependency chain, i.e.
#                       the public and private requirements of the components in this list
#                       are automatically added, and in
#                       turn the public and private requirements of those requirements,
#                       so on and so forth. If not specified, all components known to the build system
#                       are processed.
macro(armino_build_process target)
    set(options)
    set(single_value PROJECT_DIR PROJECT_VER PROJECT_NAME BUILD_DIR SDKCONFIG SDKCONFIG_DEFAULT_SOC)
    set(multi_value COMPONENTS SDKCONFIG_DEFAULTS)
    cmake_parse_arguments(_ "${options}" "${single_value}" "${multi_value}" ${ARGN})

    armino_build_set_property(BOOTLOADER_BUILD "${BOOTLOADER_BUILD}")

    # Check build target is specified. Since this target corresponds to a component
    # name, the target component is automatically added to the list of common component
    # requirements.
    if(target STREQUAL "")
        LOGE("Build target not specified.")
    endif()

    armino_build_set_property(ARMINO_SOC ${target})

    __build_set_default(PROJECT_DIR ${CMAKE_SOURCE_DIR})
    __build_set_default(PROJECT_NAME ${CMAKE_PROJECT_NAME})
    __build_set_default(PROJECT_VER 1)
    __build_set_default(BUILD_DIR ${CMAKE_BINARY_DIR})

    armino_build_get_property(project_dir PROJECT_DIR)
    __build_set_default(SDKCONFIG "${project_dir}/sdkconfig")

    __build_set_default(SDKCONFIG_DEFAULTS "")
    __build_set_default(SDKCONFIG_DEFAULT_SOC "")
    # Check for required Python modules
    __build_check_python()

    armino_build_set_property(__COMPONENT_REQUIRES_COMMON ${target} APPEND)

    # Perform early expansion of component CMakeLists.txt in CMake scripting mode.
    # It is here we retrieve the public and private requirements of each component.
    # It is also here we add the common component requirements to each component's
    # own requirements.
    __component_get_requirements()

    armino_build_get_property(component_targets __COMPONENT_TARGETS)

    # Finally, do component expansion. In this case it simply means getting a final list
    # of build component targets given the requirements set by each component.

    # Check if we need to trim the components first, and build initial components list
    # from that.
    if(__COMPONENTS)
        unset(component_targets)
        foreach(component ${__COMPONENTS})
            __component_get_target(component_target ${component})
            if(NOT component_target)
                LOGE("Failed to resolve component '${component}'.")
            endif()
            list(APPEND component_targets ${component_target})
        endforeach()
    endif()

    foreach(component_target ${component_targets})
        __build_expand_requirements(${component_target})
    endforeach()
    armino_build_unset_property(__COMPONENT_TARGETS_SEEN)

    # Get a list of common component requirements in component targets form (previously
    # we just have a list of component names)
    armino_build_get_property(common_reqs __COMPONENT_REQUIRES_COMMON)
    foreach(common_req ${common_reqs})
        __component_get_target(component_target ${common_req})
        __component_get_property(lib ${component_target} COMPONENT_LIB)
        armino_build_set_property(___COMPONENT_REQUIRES_COMMON ${lib} APPEND)
    endforeach()

    # Generate config values in different formats
    armino_build_get_property(sdkconfig SDKCONFIG)
    armino_build_get_property(sdkconfig_defaults SDKCONFIG_DEFAULTS)
    armino_build_get_property(sdkconfig_default_soc SDKCONFIG_DEFAULT_SOC)
    __kconfig_generate_config("${sdkconfig}" "${sdkconfig_defaults}" "${sdkconfig_default_soc}")
    __build_import_configs()
    __build_check_config()

    # All targets built under this scope is with the BEKEN-ARMINO build system
    set(BEKEN_PLATFORM 1)
    armino_build_set_property(COMPILE_DEFINITIONS "-DBEKEN_PLATFORM" APPEND)

    # Perform component processing (inclusion of project_include.cmake, adding component
    # subdirectories, creating library targets, linking libraries, etc.)
    __build_process_project_includes()

    armino_build_get_property(armino_path ARMINO_PATH)
    armino_build_get_property(build_dir BUILD_DIR)
    add_subdirectory(${armino_path} ${build_dir}/armino)

    unset(BEKEN_PLATFORM)
endmacro()

# armino_build_executable
#
# @brief Specify the executable the build system can attach dependencies to (for generating
# files used for linking, targets which should execute before creating the specified executable,
# generating additional binary files, generating files related to flashing, etc.)
function(armino_build_executable bin)
    # Set additional link flags for the executable
    armino_build_get_property(link_options LINK_OPTIONS)
    armino_build_get_property(link_libs LINK_LIBRARIES)
    # Using LINK_LIBRARIES here instead of LINK_OPTIONS, as the latter is not in CMake 3.5.
    set_property(TARGET ${bin} APPEND PROPERTY LINK_LIBRARIES "${link_options}")

    # Propagate link dependencies from component library targets to the executable
    armino_build_get_property(link_depends __LINK_DEPENDS)
    set_property(TARGET ${bin} APPEND PROPERTY LINK_DEPENDS "${link_depends}")

    # Set the EXECUTABLE_NAME and EXECUTABLE properties since there are generator expression
    # from components that depend on it
    get_filename_component(bin_name ${bin} NAME_WE)
    get_target_property(bin_dir ${bin} BINARY_DIR)

    armino_build_set_property(EXECUTABLE_NAME ${bin_name})
    armino_build_set_property(EXECUTABLE ${bin})
    armino_build_set_property(EXECUTABLE_DIR "${bin_dir}")

    # Add dependency of the build target to the executable
    add_dependencies(${bin} __armino_build_target)
 
    if (EXISTS "$ENV{ARMINO_PATH}/middleware/boards/${ARMINO_SOC}/${ARMINO_SOC}.wrapper")
        set(armino_pack "$ENV{ARMINO_PATH}/middleware/boards/${ARMINO_SOC}/${ARMINO_SOC}.wrapper")
    else()
        set(armino_pack "$ENV{ARMINO_PATH}/tools/env_tools/beken_packager/cmake_packager_wrapper")
    endif()

    set(target  ${ARMINO_SOC})
    add_custom_command(OUTPUT "${bin_dir}/bin_tmp"
        COMMAND "${armino_objcopy}" -O binary "${bin_dir}/${bin}" "${bin_dir}/${bin_name}.bin"
        COMMAND "${armino_readelf}" -a -h -l -S -g -s "${bin_dir}/${bin}" > "${bin_dir}/${bin_name}.txt"
        COMMAND "${armino_nm}" -n -l -C -a -A -g "${bin_dir}/${bin}" > "${bin_dir}/${bin_name}.nm"
        COMMAND "${armino_objdump}" -d "${bin_dir}/${bin}" > "${bin_dir}/${bin_name}.lst"
        COMMAND python "${armino_pack}" -n "all-${bin_name}.bin" -f "${bin_name}.bin" -c "${ARMINO_SOC}"
        DEPENDS ${bin}
        VERBATIM
        WORKING_DIRECTORY ${bin_dir}
        COMMENT "Generating binary image from built executable"
    )

    set(armino_size_statistic ${python} ${armino_path}/tools/build_tools/armino_size_statistic.py)
    add_custom_target(gen_project_binary DEPENDS "${bin_dir}/bin_tmp")
    add_custom_target(app ALL DEPENDS gen_project_binary)

    armino_build_get_property(armino_target ARMINO_SOC)

    add_custom_target(size-statistic ALL DEPENDS gen_project_binary
        COMMAND ${armino_size_statistic} --size "${armino_toolchain_size}" --dirs "${armino_path}/components/bk_libs/${armino_target}" --outfile "${build_dir}/size_map.txt"
    )


endfunction()

# armino_build_get_config
#
# @brief Get value of specified config variable
function(armino_build_get_config var config)
    cmake_parse_arguments(_ "GENERATOR_EXPRESSION" "" "" ${ARGN})
    if(__GENERATOR_EXPRESSION)
        set(val "$<TARGET_PROPERTY:__armino_build_target,${config}>")
    else()
        get_property(val TARGET __armino_build_target PROPERTY ${config})
    endif()
    set(${var} ${val} PARENT_SCOPE)
endfunction()

function(armino_build_parse_config_file_list)
    set(_sdkconfig_defaults "$ENV{SDKCONFIG_DEFAULTS}")

    armino_build_get_property(armino_path ARMINO_PATH)
    armino_build_get_property(soc_path SOC_PATH)

    if(NOT _sdkconfig_defaults)
        set(_sdkconfig_defaults "")

        if(EXISTS "${CMAKE_SOURCE_DIR}/config/common.config")
            list(APPEND _sdkconfig_defaults "${CMAKE_SOURCE_DIR}/config/common.config")
        endif()

        if(EXISTS "${CMAKE_SOURCE_DIR}/config/${ARMINO_SOC}.config")
            list(APPEND _sdkconfig_defaults "${CMAKE_SOURCE_DIR}/config/${ARMINO_SOC}.config")
        endif()

        if(EXISTS "${soc_path}/${ARMINO_SOC}/${ARMINO_SOC}.defconfig")
            set(_sdkconfig_default_soc "${soc_path}/${ARMINO_SOC}/${ARMINO_SOC}.defconfig")
            get_filename_component(sdkconfig_default_soc "${_sdkconfig_default_soc}" ABSOLUTE)
        else()
            set(sdkconfig_default_soc "")
        endif()
    endif()

    if(SDKCONFIG_DEFAULTS)
        set(_sdkconfig_defaults "${SDKCONFIG_DEFAULTS}")
        set(_sdkconfig_defaults_soc "")
    endif()

    foreach(sdkconfig_default ${_sdkconfig_defaults})
        get_filename_component(sdkconfig_default "${sdkconfig_default}" ABSOLUTE)
        if(NOT EXISTS "${sdkconfig_default}")
            LOGE("SDKCONFIG_DEFAULTS '${sdkconfig_default}' does not exist.")
        endif()
        list(APPEND sdkconfig_defaults ${sdkconfig_default})
    endforeach()

    if(SDKCONFIG)
        get_filename_component(sdkconfig "${SDKCONFIG}" ABSOLUTE)
    else()
        set(sdkconfig "${CMAKE_BINARY_DIR}/sdkconfig")
    endif()

    armino_build_set_property(SDKCONFIG_DEFAULTS "${sdkconfig_defaults}")
    armino_build_set_property(SDKCONFIG_DEFAULT_SOC "${sdkconfig_default_soc}")
    armino_build_set_property(SDKCONFIG "${sdkconfig}")
endfunction()

function(armino_build_parse_toolchain_dir)
    set(default_toolchain_dir "/opt/gcc-arm-none-eabi-5_4-2016q3/bin")
    armino_build_get_property(sdkconfig_defaults SDKCONFIG_DEFAULTS)
    armino_build_get_property(sdkconfig_default_soc SDKCONFIG_DEFAULT_SOC)
    set(user_configed_toolchain_dir "")
    foreach(config_file ${sdkconfig_default_soc})
        file(STRINGS ${config_file} toolchain_dir_line REGEX ^CONFIG_TOOLCHAIN_PATH=)
        if(NOT "${toolchain_dir_line}" STREQUAL "")
            set(user_configed_toolchain_dir ${toolchain_dir_line})
        endif()
    endforeach()

    foreach(config_file ${sdkconfig_defaults})
        file(STRINGS ${config_file} toolchain_dir_line REGEX ^CONFIG_TOOLCHAIN_PATH=)
        if(NOT "${toolchain_dir_line}" STREQUAL "")
            set(user_configed_toolchain_dir ${toolchain_dir_line})
        endif()
    endforeach()

    if("${user_configed_toolchain_dir}" STREQUAL "")
        set(toolchain_dir ${default_toolchain_dir})
        LOGI("use default toolchain path: ${toolchain_dir}")
    else()
        string(REPLACE "CONFIG_TOOLCHAIN_PATH=" "" toolchain_dir_temp ${user_configed_toolchain_dir})
        string(REPLACE "\"" "" toolchain_dir ${toolchain_dir_temp})
        LOGI("use configured toolchain path: ${toolchain_dir}")
    endif()

    if(NOT EXISTS ${toolchain_dir})
        LOGE("toolchain PATH ${toolchain_dir} doesn't exist!")
    endif()

    armino_build_set_property(TOOLCHAIN_DIR ${toolchain_dir})
    set(ENV{TOOLCHAIN_DIR} ${toolchain_dir})
endfunction()
