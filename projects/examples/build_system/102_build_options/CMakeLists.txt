# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

include($ENV{ARMINO_PATH}/tools/build_tools/cmake/project.cmake)
project(compile_options)

# The project compile options should be set after project command
armino_build_set_property(COMPILE_OPTIONS "-Wno-error" APPEND)
