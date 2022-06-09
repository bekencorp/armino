#!/usr/bin/env bash -
#******************************************************************************
#  * @attention
#  *
#  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
#  * All rights reserved.</center></h2>
#  * <h2><center>&copy; Copyright (c) 2021  Nordic Semiconductor ASA.
#  * </center></h2>
#  *
#  * This software component is licensed by ST under BSD 3-Clause license,
#  * the "License"; You may not use this file except in compliance with the
#  * License. You may obtain a copy of the License at:
#  *                        opensource.org/licenses/BSD-3-Clause
#  *
#  ******************************************************************************
# arg1 is the build directory
# arg2 is the file to preprocess
# arg3 is output file beeing preprocessed
# arg4 is optional, it fixes the GCC compiler full path if present

function preprocess
(
local compiler=arm-none-eabi-gcc
if [ $# -eq 4 ]; then
    compiler=$4
fi
$compiler -E -P -xc -I$1 -o$3 $2
)
