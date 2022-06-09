# Copyright (c) 2021, NXP Semiconductors. All rights reserved.
# SPDX-License-Identifier: BSD-3-Clause

import os

# Move to the TFM build folder
os.chdir('../../../../../../build/bin')

target = 'LPC55S69'

# Flash with PyOCD
os.system('pyocd erase --mass -t ' + target)        # erase the flash memory
os.system('pyocd flash tfm_s.hex -t ' + target)     # flash the secure image into target
os.system('pyocd flash tfm_ns.hex -t ' + target)    # flash the non-secure image into target
