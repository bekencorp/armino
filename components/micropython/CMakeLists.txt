# Set location of base MicroPython directory.
if(NOT MICROPY_DIR)
    get_filename_component(MICROPY_DIR ${COMPONENT_DIR} ABSOLUTE)
    set(PROJECT_DIR ${MICROPY_DIR}/ports/beken)
endif()

# Include core source components.
include(${MICROPY_DIR}/py/py.cmake)

if(NOT CMAKE_BUILD_EARLY_EXPANSION)
    # Enable extmod components that will be configured by extmod.cmake.
    # A board may also have enabled additional components.
    set(MICROPY_PY_BTREE OFF)

    include(${MICROPY_DIR}/py/usermod.cmake)
    include(${MICROPY_DIR}/extmod/extmod.cmake)
endif()

set(MICROPY_QSTRDEFS_PORT
    ${PROJECT_DIR}/qstrdefsport.h
)

set(MICROPY_SOURCE_SHARED
    ${MICROPY_DIR}/shared/readline/readline.c
    ${MICROPY_DIR}/shared/netutils/netutils.c
    ${MICROPY_DIR}/shared/timeutils/timeutils.c
    ${MICROPY_DIR}/shared/runtime/gchelper_generic.c
    ${MICROPY_DIR}/shared/runtime/gchelper_native.c
    ${MICROPY_DIR}/shared/runtime/interrupt_char.c
    ${MICROPY_DIR}/shared/runtime/stdout_helpers.c
    ${MICROPY_DIR}/shared/runtime/sys_stdio_mphal.c
    ${MICROPY_DIR}/shared/runtime/pyexec.c
)

set(MICROPY_SOURCE_LIB
    ${MICROPY_DIR}/lib/littlefs/lfs1.c
    ${MICROPY_DIR}/lib/littlefs/lfs1_util.c
    ${MICROPY_DIR}/lib/littlefs/lfs2.c
    ${MICROPY_DIR}/lib/littlefs/lfs2_util.c
    ${MICROPY_DIR}/lib/oofatfs/ff.c
    ${MICROPY_DIR}/lib/oofatfs/ffunicode.c
)

set(MICROPY_SOURCE_DRIVERS
    ${MICROPY_DIR}/drivers/bus/softspi.c
    ${MICROPY_DIR}/drivers/dht/dht.c
)

set(MICROPY_SOURCE_PORT
    ${PROJECT_DIR}/mpmain.c
    ${PROJECT_DIR}/hal/mphalport.c
    ${PROJECT_DIR}/hal/mphalport_gc.c
    ${PROJECT_DIR}/hal/mphalport_io.c
    ${PROJECT_DIR}/mods/modmachine.c
    ${PROJECT_DIR}/mods/modnetwork.c
    ${PROJECT_DIR}/mods/modsocket.c
    ${PROJECT_DIR}/mods/modutime.c
    ${PROJECT_DIR}/mods/machine_adc.c
    ${PROJECT_DIR}/mods/machine_bitstream.c
    ${PROJECT_DIR}/mods/machine_dac.c
    ${PROJECT_DIR}/mods/machine_hw_spi.c
    ${PROJECT_DIR}/mods/machine_i2c.c
    ${PROJECT_DIR}/mods/machine_i2s.c
    ${PROJECT_DIR}/mods/machine_pin.c
    ${PROJECT_DIR}/mods/machine_rtc.c
    ${PROJECT_DIR}/mods/machine_sdcard.c
    ${PROJECT_DIR}/mods/machine_uart.c
    ${PROJECT_DIR}/mods/machine_wdt.c
    ${PROJECT_DIR}/mods/network_wlan.c
    ${PROJECT_DIR}/thread/mpthreadport.c
)

set(MICROPY_SOURCE_QSTR
    ${MICROPY_SOURCE_PY}
    ${MICROPY_SOURCE_EXTMOD}
    ${MICROPY_SOURCE_USERMOD}
    ${MICROPY_SOURCE_SHARED}
    ${MICROPY_SOURCE_LIB}
    ${MICROPY_SOURCE_DRIVERS}
    ${MICROPY_SOURCE_PORT}
    ${MICROPY_SOURCE_BOARD}
)

if(CONFIG_MICROPYTHON)
set(MICROPY_INCS
    ${MICROPY_DIR}
    ${PROJECT_DIR}
    ${PROJECT_DIR}/boards
    ${PROJECT_DIR}/genhdr
    ${PROJECT_DIR}/hal
    ${PROJECT_DIR}/mods
    ${PROJECT_DIR}/thread
)

armino_component_register(SRCS "${MICROPY_SOURCE_QSTR}"
    INCLUDE_DIRS "${MICROPY_INCS}"
    PRIV_REQUIRES bk_cli bk_ps lwip_intf_v2_1
)

set(MICROPY_TARGET ${COMPONENT_LIB})
set(CMAKE_BINARY_DIR ${PROJECT_DIR})
target_compile_definitions(${MICROPY_TARGET} PRIVATE
    ${MICROPY_DEF_CORE}
    MICROPY_PY_BUILTINS_STR_OP_MODULO=1
    mp_init=mp_initialize
    f_open=oof_open
    f_close=oof_close
    f_read=oof_read
    f_write=oof_write
    f_lseek=oof_lseek
    f_truncate=oof_truncate
    f_sync=oof_sync
    f_opendir=oof_opendir
    f_closedir=oof_closedir
    f_readdir=oof_readdir
    f_findfirst=oof_findfirst
    f_findnext=oof_findnext
    f_mkdir=oof_mkdir
    f_unlink=oof_unlink
    f_rename=oof_rename
    f_stat=oof_stat
    f_chmod=oof_chmod
    f_utime=oof_utime
    f_chdir=oof_chdir
    f_getcwd=oof_getcwd
    f_getfree=oof_getfree
    f_getlabel=oof_getlabel
    f_setlabel=oof_setlabel
    f_forward=oof_forward
    f_expand=oof_expand
    f_mount=oof_mount
    f_umount=oof_umount
    f_mkfs=oof_mkfs
    f_fdisk=oof_fdisk
    f_setcp=oof_setcp
    disk_read=oof_disk_read
    disk_write=oof_disk_write
    disk_ioctl=oof_disk_ioctl
    FFCONF_H=\"${MICROPY_OOFATFS_DIR}/ffconf.h\"
    LFS1_NO_MALLOC LFS1_NO_DEBUG LFS1_NO_WARN LFS1_NO_ERROR LFS1_NO_ASSERT
    LFS2_NO_MALLOC LFS2_NO_DEBUG LFS2_NO_WARN LFS2_NO_ERROR LFS2_NO_ASSERT
)

list(APPEND MICROPY_CPP_INC_EXTRA ${COMPONENT_DIR}/../../include)
list(APPEND MICROPY_CPP_INC_EXTRA ${COMPONENT_DIR}/../../components/bk_cli)
list(APPEND MICROPY_CPP_INC_EXTRA ${COMPONENT_DIR}/../../components/bk_ps/include)
list(APPEND MICROPY_CPP_INC_EXTRA ${COMPONENT_DIR}/../../components/bk_common/include)
list(APPEND MICROPY_CPP_INC_EXTRA ${COMPONENT_DIR}/../../components/bk_cli/include/bk_private)
list(APPEND MICROPY_CPP_INC_EXTRA ${COMPONENT_DIR}/../../components/bk_wifi/include/bk_private)
list(APPEND MICROPY_CPP_INC_EXTRA ${COMPONENT_DIR}/../../components/lwip_intf_v2_1/lwip-2.1.2/port)
list(APPEND MICROPY_CPP_INC_EXTRA ${COMPONENT_DIR}/../../components/lwip_intf_v2_1/lwip-2.1.2/src/include)
list(APPEND MICROPY_CPP_INC_EXTRA ${COMPONENT_DIR}/../../components/lwip_intf_v2_1/lwip-2.1.2/src/include/lwip)
list(APPEND MICROPY_CPP_INC_EXTRA ${COMPONENT_DIR}/../../components/os_source/freertos_v10/include)
list(APPEND MICROPY_CPP_INC_EXTRA ${COMPONENT_DIR}/../../components/bk_rtos/include)
list(APPEND MICROPY_CPP_INC_EXTRA ${COMPONENT_DIR}/../../components/bk_rtos/freertos)
list(APPEND MICROPY_CPP_INC_EXTRA ${COMPONENT_DIR}/../../middleware/driver/include/bk_private)

# Include the main MicroPython cmake rules.
include(${MICROPY_DIR}/py/mkrules.cmake)
else()
armino_component_register(SRCS ""
    INCLUDE_DIRS ""
    PRIV_REQUIRES bk_cli bk_ps lwip_intf_v2_1
)
endif()

