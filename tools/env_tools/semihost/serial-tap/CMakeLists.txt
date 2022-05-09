cmake_minimum_required(VERSION 3.6)
project(serial_tun)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-Wall -Wextra -Wshadow -Wmissing-prototypes")

find_package(Threads)

set(SOURCE_FILES main.c tun-driver.c tun-driver.h /usr/include/libserialport.h slip.c slip.h)
add_executable(serial_tun ${SOURCE_FILES})
target_link_libraries(serial_tun libserialport.so ${CMAKE_THREAD_LIBS_INIT})
