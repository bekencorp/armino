set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(toolchain_path "$ENV{TOOLCHAIN_DIR}")
set(CMAKE_AR ${toolchain_path}/arm-none-eabi-ar)
set(CMAKE_C_COMPILER ${toolchain_path}/arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER ${toolchain_path}/arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER ${toolchain_path}/arm-none-eabi-gcc)

set(armino_objcopy ${toolchain_path}/arm-none-eabi-objcopy)
set(armino_readelf ${toolchain_path}/arm-none-eabi-readelf)
set(armino_nm ${toolchain_path}/arm-none-eabi-nm)
set(armino_objdump ${toolchain_path}/arm-none-eabi-objdump)
set(armino_toolchain_size ${toolchain_path}/arm-none-eabi-size)

set(CMAKE_C_FLAGS "-DWIFI_BLE_COEXIST -DCONFIG_CMAKE=1 -DBK_MAC=1 -DCFG_HOSTED=0 -Wl,--gc-sections -nostdlib -Wl,-wrap,malloc -Wl,-wrap,free -Wl,-wrap,__free_r -Wl,-wrap,zalloc -Wl,-wrap,calloc -Wl,-wrap,realloc  -Wl,-wrap,__realloc_r -Wl,-wrap,printf -Wl,-wrap,vsnprintf -Wl,-wrap,snprintf -Wl,-wrap,sprintf -Wl,-wrap,puts")
set(CMAKE_CXX_FLAGS "-nostdlib")
