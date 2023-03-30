#-------------------------------------------------------------------------------
# Copyright (c) 2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

# A string calibrating routine to the specified length by
# appending to an input string a specified character.
#
# output - variable to return the calibrated string

function(format_string output input size filler)
    string(LENGTH ${input} length)
    foreach(i RANGE ${length} ${size})
        string(CONCAT input ${input} ${filler})
    endforeach()
    set(${output} ${input} PARENT_SCOPE)
endfunction()

# Prints formatted list of options with a title
#
# title - will be printed in a header
# options - list of CMake options to print (semicolon separated)
#
# Example:
# dump_options("Partitions" "TFM_PARTITION_CRYPTO; TFM_PARTITION_FIRMWARE_UPDATE ")
# will produce:
# -- -------- Partitions ---------------------
# -- TFM_PARTITION_CRYPTO                  ON
# -- TFM_PARTITION_FIRMWARE_UPDATE         OFF
# -- -----------------------------------------

function(dump_options title options)

    if (CONFIG_TFM_PARTITION_QUIET)
        return()
    endif()

    format_string(header "-------- ${title} " 50 "-")
    message(STATUS )
    message(STATUS "${header}")

    foreach (option ${options})
        string(STRIP ${option} option)
        # avoid errors on empty strings to tolerate ';' at the end of list
        if((DEFINED ${option}) AND NOT ${option} STREQUAL "")
            format_string(option_name ${option} 40 " ")
            message(STATUS "${option_name} ${${option}}")
        endif()
    endforeach()

    format_string(footer "-" 50 "-")
    message(STATUS "${footer}")
endfunction()

