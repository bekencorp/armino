# Designed to be included from an ARMINO app's CMakeLists.txt file
cmake_minimum_required(VERSION 3.5)

include(${CMAKE_CURRENT_LIST_DIR}/colors.cmake)
define_colors()
# The mere inclusion of this CMake file sets up some interal build properties.
# These properties can be modified in between this inclusion the the armino_build_process
# call.
include(${CMAKE_CURRENT_LIST_DIR}/armino.cmake)

set(ARMINOTOOL ${PYTHON} "${ARMINO_PATH}/tools/build_tools/armino.py")
# Internally, the Python interpreter is already set to 'python'. Re-set here
# to be absolutely sure.
set_default(PYTHON "python")
file(TO_CMAKE_PATH ${PYTHON} PYTHON)
armino_build_set_property(PYTHON ${PYTHON})

# On processing, checking Python required modules can be turned off if it was
# already checked externally.
if(PYTHON_DEPS_CHECKED)
    armino_build_set_property(__CHECK_PYTHON 0)
endif()

# Store CMake arguments that need to be passed into all CMake sub-projects as well
# (bootloader, ULP, etc)
#
# It's not possible to tell if CMake was called with --warn-uninitialized, so to also
# have these warnings in sub-projects we set a cache variable as well and then check that.
if(WARN_UNINITIALIZED)
    armino_build_set_property(EXTRA_CMAKE_ARGS --warn-uninitialized)
else()
    armino_build_set_property(EXTRA_CMAKE_ARGS "")
endif()

# Initialize build target for this build using the environment variable or
# value passed externally.
__target_init()

#
# Get the project version from either a version file or the Git revision. This is passed
# to the armino_build_process call. Dependencies are also set here for when the version file
# changes (if it is used).
#
function(__project_get_revision var)
    set(_project_path "${CMAKE_CURRENT_LIST_DIR}")
    if(NOT DEFINED PROJECT_VER)
        if(EXISTS "${_project_path}/version.txt")
            file(STRINGS "${_project_path}/version.txt" PROJECT_VER)
            set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${_project_path}/version.txt")
        else()
            git_describe(PROJECT_VER_GIT "${_project_path}")
            if(PROJECT_VER_GIT)
                set(PROJECT_VER ${PROJECT_VER_GIT})
            else()
                LOGI("Project is not inside a git repository, or git repository has no commits;"
                        " will not use 'git describe' to determine PROJECT_VER.")
                set(PROJECT_VER 1)
            endif()
        endif()
    endif()
    set(${var} "${PROJECT_VER}" PARENT_SCOPE)
endfunction()

#
# Output the built components to the user. Generates files for invoking armino_monitor.py
# that doubles as an overview of some of the more important build properties.
#
function(__project_info test_components)
    armino_build_get_property(prefix __PREFIX)
    armino_build_get_property(inc_prefix __INCLUDE_PREFIX)
    armino_build_get_property(_build_components BUILD_COMPONENTS)
    armino_build_get_property(build_dir BUILD_DIR)
    armino_build_get_property(armino_path ARMINO_PATH)

    list(SORT _build_components)

    unset(build_components)
    unset(build_component_paths)

    foreach(build_component ${_build_components})
        __component_get_target(component_target "${build_component}")
        __component_get_property(_name ${component_target} COMPONENT_NAME)
        __component_get_property(_prefix ${component_target} __PREFIX)
        __component_get_property(_alias ${component_target} COMPONENT_ALIAS)
        __component_get_property(_dir ${component_target} COMPONENT_DIR)

        if(_alias IN_LIST test_components)
            list(APPEND test_component_paths ${_dir})
        else()
            if(_prefix STREQUAL prefix OR _prefix STREQUAL inc_prefix)
                set(component ${_name})
            else()
                set(component ${_alias})
            endif()
            list(APPEND build_components ${component})
            list(APPEND build_component_paths ${_dir})
        endif()
    endforeach()

    set(PROJECT_NAME ${CMAKE_PROJECT_NAME})
    armino_build_get_property(PROJECT_PATH PROJECT_DIR)
    armino_build_get_property(BUILD_DIR BUILD_DIR)
    armino_build_get_property(SDKCONFIG SDKCONFIG)
    armino_build_get_property(SDKCONFIG_DEFAULTS SDKCONFIG_DEFAULTS)
    armino_build_get_property(PROJECT_EXECUTABLE EXECUTABLE)
    set(PROJECT_BIN ${CMAKE_PROJECT_NAME}.bin)
    armino_build_get_property(ARMINO_VER ARMINO_VER)

    armino_build_get_property(sdkconfig_cmake SDKCONFIG_CMAKE)
    #include(${sdkconfig_cmake})
    armino_build_get_property(COMPONENT_KCONFIGS KCONFIGS)
    armino_build_get_property(COMPONENT_KCONFIGS_PROJBUILD KCONFIG_PROJBUILDS)

    # Write project description JSON file
    armino_build_get_property(build_dir BUILD_DIR)
    make_json_list("${build_components};${test_components}" build_components_json)
    make_json_list("${build_component_paths};${test_component_paths}" build_component_paths_json)
    configure_file("${armino_path}/tools/build_tools/cmake/project_description.json.in"
        "${build_dir}/project_description.json")

    # We now have the following component-related variables:
    #
    # build_components is the list of components to include in the build.
    # build_component_paths is the paths to all of these components, obtained from the component dependencies file.
    #
    # Print the list of found components and test components
    string(REPLACE ";" " " build_components "${build_components}")
    string(REPLACE ";" " " build_component_paths "${build_component_paths}")
    LOGI("Components: ${build_components}")

    if(test_components)
        string(REPLACE ";" " " test_components "${test_components}")
        string(REPLACE ";" " " test_component_paths "${test_component_paths}")
        LOGI("Test components: ${test_components}")
        LOGI("Test component paths: ${test_component_paths}")
    endif()
endfunction()

function(__project_reload)
    if(COMPONENTS_DIRS)
        armino_build_set_property(__COMPONENT_TARGETS "")

        spaces2list(COMPONENTS_DIRS)
        foreach(component_dir ${COMPONENTS_DIRS})
            __build_add_components(${component_dir})
        endforeach()
        spaces2list(COMPONENT_DIRS)
        armino_build_get_property(prefix __PREFIX)
        foreach(component_dir ${COMPONENT_DIRS})
            get_filename_component(component_dir ${component_dir} ABSOLUTE)
            __component_add(${component_dir} ${prefix})
        endforeach()
    endif()
endfunction()

function(__project_init components_var test_components_var)
    # Use EXTRA_CFLAGS, EXTRA_CXXFLAGS and EXTRA_CPPFLAGS to add more priority options to the compiler
    # EXTRA_CPPFLAGS is used for both C and C++
    # Unlike environments' CFLAGS/CXXFLAGS/CPPFLAGS which work for both host and target build,
    # these works only for target build
    set(extra_cflags "$ENV{EXTRA_CFLAGS}")
    set(extra_cxxflags "$ENV{EXTRA_CXXFLAGS}")
    set(extra_cppflags "$ENV{EXTRA_CPPFLAGS}")

    spaces2list(extra_cflags)
    spaces2list(extra_cxxflags)
    spaces2list(extra_cppflags)

    armino_build_set_property(C_COMPILE_OPTIONS "${extra_cflags}" APPEND)
    armino_build_set_property(CXX_COMPILE_OPTIONS "${extra_cxxflags}" APPEND)
    armino_build_set_property(COMPILE_OPTIONS "${extra_cppflags}" APPEND)

    function(__project_component_dir component_dir)
        get_filename_component(component_dir "${component_dir}" ABSOLUTE)
        if(EXISTS ${component_dir}/CMakeLists.txt)
            armino_build_component(${component_dir})
        else()
            file(GLOB component_dirs ${component_dir}/*)
            foreach(component_dir ${component_dirs})
                if(EXISTS ${component_dir}/CMakeLists.txt)
                    get_filename_component(base_dir ${component_dir} NAME)
                    __component_dir_quick_check(is_component ${component_dir})
                    if(is_component)
                        armino_build_component(${component_dir})
                    endif()
                endif()
            endforeach()
        endif()
    endfunction()

    armino_build_set_property(ARMINO_COMPONENT_MANAGER "$ENV{ARMINO_COMPONENT_MANAGER}")

    if(COMMON_REQUIRES)
        armino_build_set_property(__COMPONENT_REQUIRES_COMMON "${COMMON_REQUIRES}")
    endif()
    # Add component directories to the build, given the component filters, exclusions
    # extra directories, etc. passed from the root CMakeLists.txt.
    if(COMPONENTS_DIRS)
        # User wants to fully override where components are pulled from.
        armino_build_set_property(__COMPONENT_TARGETS "")

        spaces2list(COMPONENTS_DIRS)
        foreach(component_dir ${COMPONENTS_DIRS})
            __build_add_components(${component_dir})
        endforeach()

        spaces2list(COMPONENT_DIRS)
        armino_build_get_property(prefix __PREFIX)
        foreach(component_dir ${COMPONENT_DIRS})
            get_filename_component(component_dir ${component_dir} ABSOLUTE)
            __component_add(${component_dir} ${prefix})
        endforeach()
    else()
        # Add project manifest and lock file to the list of dependencies
        set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/armino_project.yml")
        set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/dependencies.lock")

        armino_build_get_property(armino_component_manager ARMINO_COMPONENT_MANAGER)
        if(armino_component_manager)
            if(armino_component_manager EQUAL "0")
                LOGV("ARMINO Component manager was explicitly disabled by setting ARMINO_COMPONENT_MANAGER=0")
            elseif(armino_component_manager EQUAL "1")
                set(managed_components_list_file ${CMAKE_BINARY_DIR}/managed_components_list.temp.cmake)

                # Call for package manager to prepare remote dependencies
                execute_process(COMMAND ${PYTHON}
                    "-m"
                    "armino_component_manager.prepare_components"
                    "--project_dir=${CMAKE_CURRENT_LIST_DIR}"
                    "prepare_dependencies"
                    "--managed_components_list_file=${managed_components_list_file}"
                    RESULT_VARIABLE result
                    ERROR_VARIABLE error)

                if(NOT result EQUAL 0)
                    LOGE("${error}")
                endif()

                # Include managed components
                include(${managed_components_list_file})
                file(REMOVE ${managed_components_list_file})
            else()
                LOGW("ARMINO_COMPONENT_MANAGER environment variable is set to unknown value "
                        "\"${armino_component_manager}\". If you want to use component manager set it to 1.")
            endif()
        elseif(EXISTS "${CMAKE_CURRENT_LIST_DIR}/armino_project.yml")
            LOGW("\"armino_project.yml\" file is found in project directory, "
                    "but component manager is not enabled. Please set ARMINO_COMPONENT_MANAGER environment variable.")
        endif()

        spaces2list(EXTRA_COMPONENTS_DIRS)
        foreach(component_dir ${EXTRA_COMPONENTS_DIRS})
            __project_component_dir("${component_dir}")
        endforeach()

        __project_component_dir("${CMAKE_CURRENT_LIST_DIR}/components")

        # Look for components in the usual places: CMAKE_CURRENT_LIST_DIR/main,
        # CMAKE_CURRENT_LIST_DIR/components, and the extra component dirs
        if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/main")
            __project_component_dir("${CMAKE_CURRENT_LIST_DIR}/main")
        endif()
    endif()

    spaces2list(COMPONENTS)
    spaces2list(EXCLUDE_COMPONENTS)
    armino_build_get_property(component_targets __COMPONENT_TARGETS)
    foreach(component_target ${component_targets})
        __component_get_property(component_name ${component_target} COMPONENT_NAME)
        set(include 1)
        if(COMPONENTS AND NOT component_name IN_LIST COMPONENTS)
            set(include 0)
        endif()
        if(EXCLUDE_COMPONENTS AND component_name IN_LIST EXCLUDE_COMPONENTS)
            set(include 0)
        endif()
        if(include)
            list(APPEND components ${component_name})
        endif()
    endforeach()

    if(TESTS_ALL OR BUILD_TESTS OR TEST_COMPONENTS OR TEST_EXCLUDE_COMPONENTS)
        spaces2list(TEST_COMPONENTS)
        spaces2list(TEST_EXCLUDE_COMPONENTS)
        armino_build_get_property(component_targets __COMPONENT_TARGETS)
        foreach(component_target ${component_targets})
            __component_get_property(component_dir ${component_target} COMPONENT_DIR)
            __component_get_property(component_name ${component_target} COMPONENT_NAME)
            if(component_name IN_LIST components)
                set(include 1)
                if(TEST_COMPONENTS AND NOT component_name IN_LIST TEST_COMPONENTS)
                    set(include 0)
                endif()
                if(TEST_EXCLUDE_COMPONENTS AND component_name IN_LIST TEST_EXCLUDE_COMPONENTS)
                    set(include 0)
                endif()
                if(include AND EXISTS ${component_dir}/test)
                    __component_add(${component_dir}/test ${component_name})
                    list(APPEND test_components ${component_name}::test)
                endif()
            endif()
        endforeach()
    endif()

    set(${components_var} "${components}" PARENT_SCOPE)
    set(${test_components_var} "${test_components}" PARENT_SCOPE)
endfunction()

# Trick to temporarily redefine project(). When functions are overriden in CMake, the originals can still be accessed
# using an underscore prefixed function of the same name. The following lines make sure that __project  calls
# the original project(). See https://cmake.org/pipermail/cmake/2015-October/061751.html.
function(project)
endfunction()

function(_project)
endfunction()

macro(project project_name)
    # Initialize project, preparing COMPONENTS argument for armino_build_process()
    # call later using external COMPONENT_DIRS, COMPONENTS_DIRS, EXTRA_COMPONENTS_DIR,
    # EXTRA_COMPONENTS_DIRS, COMPONENTS, EXLUDE_COMPONENTS, TEST_COMPONENTS,
    # TEST_EXLUDE_COMPONENTS, TESTS_ALL, BUILD_TESTS
    __project_init(components test_components)

    armino_build_parse_config_file_list()
    armino_build_parse_toolchain_dir()
    __target_set_toolchain()

    if(CCACHE_ENABLE)
        find_program(CCACHE_FOUND ccache)
        if(CCACHE_FOUND)
            LOGI("ccache will be used for faster recompilation")
            set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
        else()
            LOGW("enabled ccache in build but ccache program not found")
        endif()
    endif()

    # The actual call to project()
    __project("app" C CXX ASM)

    # Generate compile_commands.json (needs to come after project call).
    set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

    # Since components can import third-party libraries, the original definition of project() should be restored
    # before the call to add components to the build.
    function(project)
        set(project_ARGV ARGV)
        __project(${${project_ARGV}})

        # Set the variables that project() normally sets, documented in the
        # command's docs.
        #
        # https://cmake.org/cmake/help/v3.5/command/project.html
        #
        # There is some nuance when it comes to setting version variables in terms of whether
        # CMP0048 is set to OLD or NEW. However, the proper behavior should have bee already handled by the original
        # project call, and we're just echoing the values those variables were set to.
        set(PROJECT_NAME "${PROJECT_NAME}" PARENT_SCOPE)
        set(PROJECT_BINARY_DIR "${PROJECT_BINARY_DIR}" PARENT_SCOPE)
        set(PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}" PARENT_SCOPE)
        set(PROJECT_VERSION "${PROJECT_VERSION}" PARENT_SCOPE)
        set(PROJECT_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}" PARENT_SCOPE)
        set(PROJECT_VERSION_MINOR "${PROJECT_VERSION_MINOR}" PARENT_SCOPE)
        set(PROJECT_VERSION_PATCH "${PROJECT_VERSION_PATCH}" PARENT_SCOPE)
        set(PROJECT_VERSION_TWEAK "${PROJECT_VERSION_TWEAK}" PARENT_SCOPE)

        set(${PROJECT_NAME}_BINARY_DIR "${${PROJECT_NAME}_BINARY_DIR}" PARENT_SCOPE)
        set(${PROJECT_NAME}_SOURCE_DIR "${${PROJECT_NAME}_SOURCE_DIR}" PARENT_SCOPE)
        set(${PROJECT_NAME}_VERSION "${${PROJECT_NAME}_VERSION}" PARENT_SCOPE)
        set(${PROJECT_NAME}_VERSION_MAJOR "${${PROJECT_NAME}_VERSION_MAJOR}" PARENT_SCOPE)
        set(${PROJECT_NAME}_VERSION_MINOR "${${PROJECT_NAME}_VERSION_MINOR}" PARENT_SCOPE)
        set(${PROJECT_NAME}_VERSION_PATCH "${${PROJECT_NAME}_VERSION_PATCH}" PARENT_SCOPE)
        set(${PROJECT_NAME}_VERSION_TWEAK "${${PROJECT_NAME}_VERSION_TWEAK}" PARENT_SCOPE)
    endfunction()

    # Prepare the following arguments for the armino_build_process() call using external
    # user values:
    #
    # BUILD_DIR is set to project binary dir
    #
    # PROJECT_NAME is taken from the passed name from project() call
    # PROJECT_DIR is set to the current directory
    # PROJECT_VER is from the version text or git revision of the current repo

    if(BUILD_DIR)
        get_filename_component(build_dir "${BUILD_DIR}" ABSOLUTE)
        if(NOT EXISTS "${build_dir}")
            LOGE("BUILD_DIR '${build_dir}' does not exist.")
        endif()
    else()
        set(build_dir ${CMAKE_BINARY_DIR})
    endif()

    __project_get_revision(project_ver)

    LOGI("Building Beken ARMINO components for target ${ARMINO_SOC}")

    armino_build_get_property(sdkconfig_defaults SDKCONFIG_DEFAULTS)
    armino_build_get_property(sdkconfig_default_soc SDKCONFIG_DEFAULT_SOC)
    armino_build_get_property(sdkconfig SDKCONFIG)
    armino_build_process(${ARMINO_SOC}
                    SDKCONFIG_DEFAULTS "${sdkconfig_defaults}"
                    SDKCONFIG_DEFAULT_SOC ${sdkconfig_default_soc}
                    SDKCONFIG ${sdkconfig}
                    BUILD_DIR ${build_dir}
                    PROJECT_NAME ${CMAKE_PROJECT_NAME}
                    PROJECT_DIR ${CMAKE_CURRENT_LIST_DIR}
                    PROJECT_VER "${project_ver}"
                    COMPONENTS "${components};${test_components}")

    # Special treatment for 'main' component for standard projects (not part of core build system).
    # Have it depend on every other component in the build. This is
    # a convenience behavior for the standard project; thus is done outside of the core build system
    # so that it treats components equally.
    #
    # This behavior should only be when user did not set REQUIRES/PRIV_REQUIRES manually.
    armino_build_get_property(build_components BUILD_COMPONENT_ALIASES)
    if(armino::main IN_LIST build_components)
        __component_get_target(main_target armino::main)
        __component_get_property(reqs ${main_target} REQUIRES)
        __component_get_property(priv_reqs ${main_target} PRIV_REQUIRES)
        armino_build_get_property(common_reqs __COMPONENT_REQUIRES_COMMON)
        if(reqs STREQUAL common_reqs AND NOT priv_reqs) #if user has not set any requirements
            if(test_components)
                list(REMOVE_ITEM build_components ${test_components})
            endif()
            list(REMOVE_ITEM build_components armino::main)
            __component_get_property(lib ${main_target} COMPONENT_LIB)
            set_property(TARGET ${lib} APPEND PROPERTY INTERFACE_LINK_LIBRARIES "${build_components}")
            get_property(type TARGET ${lib} PROPERTY TYPE)
            if(type STREQUAL STATIC_LIBRARY)
                set_property(TARGET ${lib} APPEND PROPERTY LINK_LIBRARIES "${build_components}")
            endif()
        endif()
    endif()

    set(project_elf ${CMAKE_PROJECT_NAME}.elf)

    # Create a dummy file to work around CMake requirement of having a source
    # file while adding an executable
    set(project_elf_src ${CMAKE_BINARY_DIR}/project_elf_src.c)
    add_custom_command(OUTPUT ${project_elf_src}
        COMMAND ${CMAKE_COMMAND} -E touch ${project_elf_src}
        VERBATIM)
    add_custom_target(_project_elf_src DEPENDS "${project_elf_src}")
    add_executable(${project_elf} "${project_elf_src}")
    add_dependencies(${project_elf} _project_elf_src)

    target_link_libraries(${project_elf} "-Wl,--start-group")
    target_link_libraries(${project_elf} "-lstdc++")

    if(__PROJECT_GROUP_LINK_COMPONENTS)
        target_link_libraries(${project_elf} "-Wl,--start-group")
    endif()

    if(test_components)
        target_link_libraries(${project_elf} "-Wl,--whole-archive")
        foreach(test_component ${test_components})
            if(TARGET ${test_component})
                target_link_libraries(${project_elf} ${test_component})
            endif()
        endforeach()
        target_link_libraries(${project_elf} "-Wl,--no-whole-archive")
    endif()

    armino_build_get_property(build_components BUILD_COMPONENT_ALIASES)

    armino_build_get_property(armino_path ARMINO_PATH)

    if(test_components)
        list(REMOVE_ITEM build_components ${test_components})
    endif()

    target_link_libraries(${project_elf} ${build_components})

    set(mapfile "${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.map")
    target_link_libraries(${project_elf} "-Wl,--cref -Wl,--Map=${mapfile}")

    set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" APPEND PROPERTY
        ADDITIONAL_MAKE_CLEAN_FILES
        "${mapfile}" "${project_elf_src}")

    armino_build_get_property(armino_path ARMINO_PATH)
    armino_build_get_property(python PYTHON)

    set(armino_size ${python} ${armino_path}/tools/build_tools/armino_size.py)

    if(DEFINED OUTPUT_JSON AND OUTPUT_JSON)
        list(APPEND armino_size "--json")
    endif()

    # Add size targets, depend on map file, run armino_size.py
    add_custom_target(size
        DEPENDS ${project_elf}
        COMMAND ${armino_size} --summary --map_file ${mapfile}
        )

    add_custom_target(size-files
        DEPENDS ${project_elf}
        COMMAND ${armino_size} --files --map_file ${mapfile}
        )

    add_custom_target(size-components
        DEPENDS ${project_elf}
        COMMAND ${armino_size} --components --map_file ${mapfile}
        )

    add_custom_target(size-component
        DEPENDS ${project_elf}
        COMMAND ${armino_size} --component ${SIZE_COMPONENT} --map_file ${mapfile}
        )

    add_custom_target(size-diff
        DEPENDS ${project_elf}
        COMMAND ${armino_size} --diff ${SIZE_ANOTHER_MAP_FILE} --map_file ${mapfile}
        )

    unset(armino_size)
    # Add DFU build and flash targets
    __add_dfu_targets()

    if (NOT BUILD_PROPERTIES_LIB)
        armino_build_executable(${project_elf})
    endif()

    __project_info("${test_components}")
endmacro()
