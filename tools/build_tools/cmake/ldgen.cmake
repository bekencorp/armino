# Utilities for supporting linker script generation in the build system

# __ldgen_add_fragment_files
#
# Add one or more linker fragment files, and append it to the list of fragment
# files found so far.
function(__ldgen_add_fragment_files fragment_files)
    spaces2list(fragment_files)

    foreach(fragment_file ${fragment_files})
        get_filename_component(abs_path ${fragment_file} ABSOLUTE)
        list(APPEND _fragment_files ${abs_path})
    endforeach()

    armino_build_set_property(__LDGEN_FRAGMENT_FILES "${_fragment_files}" APPEND)
endfunction()

# __ldgen_add_component
#
# Generate sections info for specified target to be used in linker script generation
function(__ldgen_add_component component_lib)
    armino_build_set_property(__LDGEN_LIBRARIES "$<TARGET_FILE:${component_lib}>" APPEND)
    armino_build_set_property(__LDGEN_DEPENDS ${component_lib} APPEND)
endfunction()

# __ldgen_process_template
#
# Passes a linker script template to the linker script generation tool for
# processing
function(__ldgen_process_template template output)
    armino_build_get_property(armino_target ARMINO_SOC)
    armino_build_get_property(armino_path ARMINO_PATH)

    armino_build_get_property(build_dir BUILD_DIR)
    armino_build_get_property(ldgen_libraries __LDGEN_LIBRARIES GENERATOR_EXPRESSION)
    file(GENERATE OUTPUT ${build_dir}/ldgen_libraries.in CONTENT $<JOIN:${ldgen_libraries},\n>)
    file(GENERATE OUTPUT ${build_dir}/ldgen_libraries INPUT ${build_dir}/ldgen_libraries.in)

    set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES
        "${build_dir}/ldgen_libraries.in"
        "${build_dir}/ldgen_libraries")

    armino_build_get_property(ldgen_fragment_files __LDGEN_FRAGMENT_FILES GENERATOR_EXPRESSION)
    armino_build_get_property(ldgen_depends __LDGEN_DEPENDS GENERATOR_EXPRESSION)
    # Create command to invoke the linker script generator tool.
    armino_build_get_property(sdkconfig SDKCONFIG)
    armino_build_get_property(root_kconfig __ROOT_KCONFIG)
    armino_build_get_property(kconfigs KCONFIGS)
    armino_build_get_property(kconfig_projbuilds KCONFIG_PROJBUILDS)

    armino_build_get_property(python PYTHON)

    string(REPLACE ";" " " kconfigs "${kconfigs}")
    string(REPLACE ";" " " kconfig_projbuilds "${kconfig_projbuilds}")

    armino_build_get_property(config_env_path CONFIG_ENV_PATH)

    add_custom_command(
        OUTPUT ${output}
        COMMAND ${python} ${armino_path}/tools/ldgen/ldgen.py
        --config    ${sdkconfig}
        --fragments "$<JOIN:${ldgen_fragment_files},\t>"
        --input     ${template}
        --output    ${output}
        --kconfig   ${root_kconfig}
        --env-file  "${config_env_path}"
        --libraries-file ${build_dir}/ldgen_libraries
        --objdump   ${CMAKE_OBJDUMP}
        DEPENDS     ${template} ${ldgen_fragment_files} ${ldgen_depends} ${SDKCONFIG}
    )

    get_filename_component(_name ${output} NAME)
    add_custom_target(__ldgen_output_${_name} DEPENDS ${output})
    add_dependencies(__armino_build_target __ldgen_output_${_name})
    armino_build_set_property(__LINK_DEPENDS ${output} APPEND)
endfunction()
