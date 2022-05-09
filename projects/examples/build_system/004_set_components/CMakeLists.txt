cmake_minimum_required(VERSION 3.5)

# The project variable should be set before include the project.cmake
set(COMPONENTS c3_contain_main)

include($ENV{ARMINO_PATH}/tools/build_tools/cmake/project.cmake)
project(cmake_exam)
