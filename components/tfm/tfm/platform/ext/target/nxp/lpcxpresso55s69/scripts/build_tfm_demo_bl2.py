# Copyright (c) 2021, NXP Semiconductors. All rights reserved.
# Copyright (c) 2020, Linaro. All rights reserved.
# Copyright (c) 2020, Arm Limited. All rights reserved.
# SPDX-License-Identifier: BSD-3-Clause

import os
import platform

# Declaration of the default flash base addresses parsed from config.cmake
if platform.system() == 'Windows':
    BL2_S_IMAGE_START = os.popen('findstr BL2_S_IMAGE_START ..\config.cmake').read().split("set(BL2_S_IMAGE_START")[1].split("\"")[1]
    BL2_NS_IMAGE_START = os.popen('findstr BL2_NS_IMAGE_START ..\config.cmake').read().split("set(BL2_NS_IMAGE_START")[1].split("\"")[1]
else:
    BL2_S_IMAGE_START = os.popen('grep "BL2_S_IMAGE_START" ../config.cmake').read().split("set(BL2_S_IMAGE_START")[1].split("\"")[1]
    BL2_NS_IMAGE_START = os.popen('grep "BL2_NS_IMAGE_START" ../config.cmake').read().split("set(BL2_NS_IMAGE_START")[1].split("\"")[1]

# Update flash base addresses according to the flash_layout.h if pyclibrary is present
try:
  import pyclibrary

  # Parse the flash_layout.h header file with the BL2 configuration
  parser = pyclibrary.CParser('../partition/flash_layout.h',  macros={'BL2': 'ON'})

  # Update secure image flash base address according to FLASH_AREA_0_OFFSET
  BL2_S_IMAGE_START = parser.defs['values']['FLASH_AREA_0_OFFSET']
  # Update non secure image flash base address according to BL2_S_IMAGE_START + FLASH_S_PARTITION_SIZE
  BL2_NS_IMAGE_START = BL2_S_IMAGE_START + parser.defs['values']['FLASH_S_PARTITION_SIZE']

# Use the default values if the previous step was not successful
except:
  print("*** WARNING: pyclibrary is not installed ***")
  print("Please install pyclibrary for updating flash base addresses in case of flash_layout.h modification")
  print("Using default values:")
  print("BL2_S_IMAGE_START = " + BL2_S_IMAGE_START)
  print("BL2_NS_IMAGE_START = " + BL2_NS_IMAGE_START)
  print("")

# Move to the TF-M root directory
os.chdir('../../../../../../')

# Remove previous build folder
if os.path.isdir("build"):
    if platform.system() == 'Windows':
        os.system('rd /s /q build')
    else:
        os.system('rm -rf build')

# Generate the S and NS makefiles
os.system('cmake -S . -B build -DBL2_S_IMAGE_START=' + BL2_S_IMAGE_START + ' -DBL2_NS_IMAGE_START=' + BL2_NS_IMAGE_START + ' -DTFM_PLATFORM=nxp/lpcxpresso55s69 -DTFM_TOOLCHAIN_FILE=toolchain_GNUARM.cmake -DTFM_PROFILE=profile_medium -DCMAKE_BUILD_TYPE=Relwithdebinfo -DTFM_ISOLATION_LEVEL=2 -G"Unix Makefiles"')

# Build the binaries
os.chdir('build')
os.system('make install')