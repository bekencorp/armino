#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
# Copyright (c) 2022 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

############################ Partitions ########################################

set(CONFIG_TFM_SPM_BACKEND      "IPC"       CACHE STRING    "The SPM backend [IPC, SFN]")
set(CONFIG_TFM_SPM_BACKEND_IPC  ON)
set(CONFIG_TFM_SPM_BACKEND_SFN  OFF)
set(CONFIG_TFM_PARTITION_META   ON)
# Derive ns_agent partitions
if (TFM_MULTI_CORE_TOPOLOGY)
    set(TFM_PARTITION_NS_AGENT_MAILBOX ON  CACHE BOOL "Enable Non-Secure Mailbox Agent partition")
else()
    set(TFM_PARTITION_NS_AGENT_MAILBOX OFF  CACHE BOOL "Enable Non-Secure Mailbox Agent partition")
endif()
if (CONFIG_TFM_USE_TRUSTZONE)
    set(TFM_PARTITION_NS_AGENT_TZ ON  CACHE BOOL "Enable Non-Secure TrustZone Agent partition")
else()
    set(TFM_PARTITION_NS_AGENT_TZ OFF  CACHE BOOL "Enable Non-Secure TrustZone Agent partition")
endif()
