#-------------------------------------------------------------------------------
# Copyright (c) 2022, Arm Limited. All rights reserved.
# Copyright (c) 2022 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

# If tf-m-extras partitions are required, please set TFM_EXTRAS_REPO_PATH to
# DOWNLOAD and TFM_EXTRAS_REPO_VERSION to appropriate commit-id in the platform
# specific config. By default, they are not fetched.
set(TFM_EXTRAS_REPO_PATH                ""         CACHE PATH    "Path to tf-m-extras repo (or DOWNLOAD to fetch automatically")
set(TFM_EXTRAS_REPO_VERSION             ""         CACHE STRING  "The version of tf-m-extras to use")

fetch_remote_library(
    LIB_NAME                tf-m-extras
    LIB_SOURCE_PATH_VAR     TFM_EXTRAS_REPO_PATH
    FETCH_CONTENT_ARGS
        GIT_REPOSITORY      https://git.trustedfirmware.org/TF-M/tf-m-extras.git
        GIT_TAG             ${TFM_EXTRAS_REPO_VERSION}
        GIT_SHALLOW         FALSE
        GIT_PROGRESS        TRUE
)
