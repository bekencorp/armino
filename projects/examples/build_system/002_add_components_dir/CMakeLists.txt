cmake_minimum_required(VERSION 3.5)

# The project variable should be set before include the project.cmake
# my_components_dir can also use the abosolute path
set(EXTRA_COMPONENTS_DIRS my_components_dir)

include($ENV{ARMINO_PATH}/tools/build_tools/cmake/project.cmake)
project(cmake_exam)
