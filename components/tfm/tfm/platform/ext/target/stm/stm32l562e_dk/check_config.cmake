#-------------------------------------------------------------------------------
# Copyright (c) 2020-2021, Arm Limited. All rights reserved.
# Copyright (c) 2021 STMicroelectronics. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

## test incompatible config due to memory constrains
tfm_invalid_config(TEST_S_CRYPTO AND (TEST_S_PLATFORM OR TEST_S_ATTESTATION OR TEST_S_QCBOR OR TEST_S_T_COSE OR TEST_S_PS OR TEST_S_ITS))
tfm_invalid_config(TEST_NS_CRYPTO AND (TEST_NS_PLATFORM OR TEST_NS_ATTESTATION OR TEST_NS_QCBOR OR TEST_NS_T_COSE OR TEST_NS_PS OR TEST_NS_ITS))
