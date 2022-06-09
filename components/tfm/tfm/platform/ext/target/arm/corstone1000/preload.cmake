#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

# preload.cmake is used to set things that related to the platform that are both
# immutable and global, which is to say they should apply to any kind of project
# that uses this plaform. In practise this is normally compiler definitions and
# variables related to hardware.

# Set architecture and CPU
set(TFM_SYSTEM_PROCESSOR cortex-m0plus)
set(TFM_SYSTEM_ARCHITECTURE armv6-m)


# Secure Enclave has a CryptoCell-312 as an accelerator.
set(CRYPTO_HW_ACCELERATOR_TYPE cc312)
