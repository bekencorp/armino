cmake_minimum_required(VERSION 3.5)

# The project variable should be set before include the project.cmake
set(EXCLUDE_COMPONENTS c1_not_build)

include($ENV{ARMINO_PATH}/tools/build_tools/cmake/project.cmake)
project(cmake_exam)
