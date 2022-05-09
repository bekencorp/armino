cmake_minimum_required(VERSION 3.5)
include($ENV{ARMINO_PATH}/tools/build_tools/cmake/project.cmake)
project(cmake_exam)

# The project compile options should be set after project command
armino_build_set_property(COMPILE_OPTIONS "-Wno-error" APPEND)
