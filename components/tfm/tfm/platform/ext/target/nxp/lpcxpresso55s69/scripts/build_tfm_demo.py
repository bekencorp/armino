# Copyright (c) 2021, NXP Semiconductors. All rights reserved.
# Copyright (c) 2020, Linaro. All rights reserved.
# Copyright (c) 2020, Arm Limited. All rights reserved.
# SPDX-License-Identifier: BSD-3-Clause

import os
import platform

# Move to the TF-M root directory
os.chdir('../../../../../../')

# Remove previous build folder
if os.path.isdir("build"):
    if platform.system() == 'Windows':
        os.system('rd /s /q build')
    else:
        os.system('rm -rf build')

# Generate the S and NS makefiles
os.system('cmake -S . -B build -DTFM_PLATFORM=nxp/lpcxpresso55s69 -DTFM_TOOLCHAIN_FILE=toolchain_GNUARM.cmake -DTFM_PROFILE=profile_medium -DCMAKE_BUILD_TYPE=Relwithdebinfo -DBL2=OFF -DTFM_ISOLATION_LEVEL=2 -G"Unix Makefiles"')

# Build the binaries
os.chdir('build')
os.system('make install')