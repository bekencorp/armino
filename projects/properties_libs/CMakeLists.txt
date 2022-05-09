# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)
set(COMPONENTS_DIRS ../../properties/modules ../../properties/soc)
set(COMPONENT_DIRS ../../include ./main)
set(COMMON_REQUIRES include)
set(SOC_PATH properties/soc)
set(BUILD_PROPERTIES_LIB 1)
include($ENV{ARMINO_PATH}/tools/build_tools/cmake/project.cmake)
project(app)
