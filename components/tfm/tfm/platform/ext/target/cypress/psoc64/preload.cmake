#-------------------------------------------------------------------------------
# Copyright (c) 2020, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

# preload.cmake is used to set things that related to the platform that are both
# immutable and global, which is to say they should apply to any kind of project
# that uses this platform. In practise this is normally compiler definitions and
# variables related to hardware.

# Set architecture and CPU. Cypress PSoC64 runs the S code on its Cortex-m0+
set(TFM_SYSTEM_PROCESSOR cortex-m0plus)
set(TFM_SYSTEM_ARCHITECTURE armv6-m)
