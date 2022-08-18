# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)
set(COMPONENTS main bk_init bk_startup bk_rtos bk_system driver bk_adapter)
include($ENV{ARMINO_PATH}/tools/build_tools/cmake/project.cmake)
project(app)
