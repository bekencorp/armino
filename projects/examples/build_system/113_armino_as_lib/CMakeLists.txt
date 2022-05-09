cmake_minimum_required(VERSION 3.5)

project(build_lib C)

if("${TARGET}" STREQUAL "bk7231n")
    include($ENV{ARMINO_PATH}/tools/build_tools/cmake/armino.cmake)
    armino_build_process(bk7231n
                    COMPONENTS bk7231n bk_log
                    SDKCONFIG ${CMAKE_CURRENT_LIST_DIR}/sdkconfig
                    BUILD_DIR ${CMAKE_BINARY_DIR})
else()
    # Create stubs for log and freertos
    add_subdirectory(stubs/log)
endif()

set(elf_file ${CMAKE_PROJECT_NAME}.elf)
add_executable(${elf_file} main.c)

# Link the static libraries to the executable
if("${TARGET}" STREQUAL "bk7231n")
    target_link_libraries(${elf_file} armino::bk_log)
    # Attach additional targets to the executable file for flashing,
    armino_build_executable(${elf_file})
else()
    target_link_libraries(${elf_file} stub::log)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
