# Copyright (c) 2021, NXP Semiconductors. All rights reserved.
# SPDX-License-Identifier: BSD-3-Clause

import os
import platform

# Move to the TFM build folder
os.chdir('../../../../../../build/bin')

# Parse flash base addresses of secure and non secure images stored in CMakeCache.txt
if platform.system() == 'Windows':
    BL2_S_IMAGE_START =  os.popen('findstr BL2_S_IMAGE_START:STRING= ..\CMakeCache.txt').read().split('=')[1].rstrip()
    BL2_NS_IMAGE_START = os.popen('findstr BL2_NS_IMAGE_START:STRING= ..\CMakeCache.txt').read().split('=')[1].rstrip()

else:
    BL2_S_IMAGE_START =  os.popen('grep "BL2_S_IMAGE_START:STRING=" ../CMakeCache.txt').read().split('=')[1].rstrip()
    BL2_NS_IMAGE_START = os.popen('grep "BL2_NS_IMAGE_START:STRING=" ../CMakeCache.txt').read().split('=')[1].rstrip()

target = 'LPC55S69'

# Flash with PyOCD
os.system('pyocd erase --mass -t ' + target)                                                       # erase the flash memory
os.system('pyocd flash bl2.hex -t ' + target)                                                      # flash the secure image into target
os.system('pyocd flash tfm_s_signed.bin --base-address ' + BL2_S_IMAGE_START + ' -t ' + target)    # flash the signed secure image into target
os.system('pyocd flash tfm_ns_signed.bin --base-address ' + BL2_NS_IMAGE_START + ' -t ' + target)  # flash the signed non-secure image into target
