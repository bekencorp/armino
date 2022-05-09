get_property(__armino_env_set GLOBAL PROPERTY __ARMINO_ENV_SET)
if(NOT __armino_env_set)
    # Infer an ARMINO_PATH relative to the tools/cmake directory
    get_filename_component(_armino_path "${CMAKE_CURRENT_LIST_DIR}/../../../" REALPATH)
    file(TO_CMAKE_PATH "${_armino_path}" _armino_path)

    # Get the path set in environment
    set(armino_path $ENV{ARMINO_PATH})

    # Environment ARMINO_PATH should match the inferred ARMINO_PATH. If not, warn the user.
    # (Note: REALPATH is needed in both above steps to account for case on case
    # insensitive filesystems, or relative paths)
    if(armino_path)
        get_filename_component(armino_path "${armino_path}" REALPATH)
        file(TO_CMAKE_PATH "${armino_path}" armino_path)

        if(NOT armino_path STREQUAL _armino_path)
            LOGW("ARMINO_PATH environment variable is different from inferred ARMINO_PATH.
                            Check if your project's top-level CMakeLists.txt includes the right
                            CMake files. Environment ARMINO_PATH will be used for the build:
                            ${armino_path}")
        endif()
    else()
        LOGW("ARMINO_PATH environment variable not found. Setting ARMINO_PATH to '${_armino_path}'.")
        set(armino_path ${_armino_path})
        set(ENV{ARMINO_PATH} ${_armino_path})
    endif()

    # Include other CMake modules required
    set(CMAKE_MODULE_PATH
        "${armino_path}/tools/build_tools/cmake"
        "${armino_path}/tools/build_tools/cmake/third_party"
        ${CMAKE_MODULE_PATH})
    include(build)

    set(ARMINO_PATH ${armino_path})
    if (SOC_PATH)
        set(SOC_PATH ${armino_path}/${SOC_PATH})
    else()
        set(SOC_PATH ${armino_path}/middleware/soc)
    endif()

    include(colors)
    define_colors()
    include(GetGitRevisionDescription)
    include(git_submodules)
    include(crosstool_version_check)
    include(kconfig)
    include(component)
    include(utilities)
    include(targets)
    include(ldgen)
    include(dfu)
    include(version)

    __build_init("${armino_path}")

    set_property(GLOBAL PROPERTY __ARMINO_ENV_SET 1)
endif()
