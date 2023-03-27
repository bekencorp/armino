# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)


include($ENV{ARMINO_PATH}/tools/build_tools/cmake/project.cmake)
project(app)

# The project compile options should be set after project command
armino_build_set_property(C_COMPILE_OPTIONS "-DAPP_BIN_NAME=\"$ENV{APP_NAME}\"" APPEND)
armino_build_set_property(C_COMPILE_OPTIONS "-DUSER_SW_VER=\"$ENV{APP_VERSION}\"" APPEND)
