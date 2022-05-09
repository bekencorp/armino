# A CMake script to run dfu-util from within ninja or make
# or another cmake-based build runner
#
# It is recommended to NOT USE this CMake script directly

cmake_minimum_required(VERSION 3.5)

set(TOOL "dfu-util")
set(CMD "${TOOL}")

if(${BEKEN_DFU_LIST})
    list(APPEND CMD "--list")
else()
    # The following works even when BEKEN_DFU_PID is not defined.
    list(APPEND CMD "-d" "303a:${BEKEN_DFU_PID}")

    if(NOT $ENV{BEKEN_DFU_PATH} STREQUAL "")
        list(APPEND CMD "--path" $ENV{BEKEN_DFU_PATH})
    endif()
    list(APPEND CMD "-D" ${BEKEN_DFU_BIN})
endif()

LOGI("Command list: ${CMD}")
execute_process(COMMAND ${CMD} RESULT_VARIABLE result)

if(${result})
    LOGE("${TOOL} failed")
endif()
