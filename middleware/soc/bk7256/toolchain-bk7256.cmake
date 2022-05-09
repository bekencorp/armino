include(build)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR riscv)

set(toolchain_path "$ENV{TOOLCHAIN_DIR}")
set(CMAKE_AR ${toolchain_path}/riscv32-elf-ar)
set(CMAKE_C_COMPILER ${toolchain_path}/riscv32-elf-gcc)
set(CMAKE_CXX_COMPILER ${toolchain_path}/riscv32-elf-g++)
set(CMAKE_ASM_COMPILER ${toolchain_path}/riscv32-elf-gcc)

set(armino_objcopy ${toolchain_path}/riscv32-elf-objcopy)
set(armino_readelf ${toolchain_path}/riscv32-elf-readelf)
set(armino_nm ${toolchain_path}/riscv32-elf-nm)
set(armino_objdump ${toolchain_path}/riscv32-elf-objdump)
set(armino_toolchain_size ${toolchain_path}/riscv32-elf-size)

#add the libs in toolchain
link_libraries(libm.a) #contian sin() cos() ...
link_libraries(libgcc.a) #contian __riscv_restore_2  __riscv_save_3 ...
link_libraries(libc.a)#contain memset() memcopy() ....
link_libraries(libgloss.a)#contain _write() ...

set(CMAKE_C_FLAGS "-D__freertos_irq_stack_top=_stack -DWIFI_BLE_COEXIST -DCONFIG_CMAKE=1 -DBK_MAC=1 -Wl,--gc-sections -nostdlib -Wl,-wrap,strlen -Wl,-wrap,malloc -Wl,-wrap,_malloc_r -Wl,-wrap,free -Wl,-wrap,_free_r -Wl,-wrap,zalloc -Wl,-wrap,calloc -Wl,-wrap,realloc  -Wl,-wrap,_realloc_r -Wl,-wrap,printf -Wl,-wrap,vsnprintf -Wl,-wrap,snprintf -Wl,-wrap,sprintf -Wl,-wrap,puts")
set(CMAKE_CXX_FLAGS "-nostdlib")
