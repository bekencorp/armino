############
SPM Backends
############
This document briefly introduces the backends of Secure Partition Manager (SPM)
in TF-M and how to select one for building.

***********
IPC and SFN
***********
The Firmware Framework M (FF-M) [1]_, [2]_ provides two different programming
models for Secure Partitions.

- IPC Model

  The Secure Partition processes signals in any order, and can defer
  responding to a message while continuing to process other signals.

- SFN Model

  The Secure Partition is made up of a collection of callback functions which
  implement secure services.

Although the programming model is different, they share the same APIs to
interact with the SPM.
The behaviours of the APIs share the same implementation with slight differences
for the two programming models.
This is regarded as the ``frontend``.

The TF-M runtime implementations behind ``frontend`` are different.
TF-M provides two ``backends`` correspondingly.

 - IPC backend

  In this backend, the SPM and each Secure Partition have their own execution
  contexts, which is required to support the IPC model Secure Partitions.
  This also enables the SPM to provide higher isolation levels.
  This SPM backend acts like a multiple-process system.
  It can also adopt SFN model Secure Partitions.

 - SFN backend

  The SFN backend provides more efficient executions because it shares a
  single-thread execution context with all the Secure Partitions.
  This SPM backend acts like a single library.
  Therefore, it can only adopt SFN model Secure Partitions.
  And it does not support higher isolation levels.
  On the other hand, it consumes less memory compared to the IPC backend.

The following table summaries the relationships between SPM backends, Secure
Partition models and isolation levels.

.. table::
    :widths: auto

    +--------------+---------------------------+---------------------------+
    | SPM backend  | Supported Partition model | Supported Isolation Level |
    +==============+===========================+===========================+
    | SFN          | SFN Partition             | 1                         |
    +--------------+---------------------------+---------------------------+
    | IPC          | IPC and SFN Partition     | 1, 2 and 3                |
    +--------------+---------------------------+---------------------------+

******************************
Implementation Recommendations
******************************
If an implementation doesn't contain any IPC model Secure Partition and only
requires isolation level 1, then it is recommended to select the SFN backend
to optimize memory consumption and execution performance.

If an implementation contains any IPC model Secure Partition or requires
isolation level 2 or 3, then the IPC backend is required.

***************************
TF-M Configuration Switches
***************************
In the TF-M build system, the ``CONFIG_TFM_SPM_BACKEND`` configuration is used
to select the backend of SPM.
The valid values are ``SFN`` and ``IPC``.

.. code-block:: bash

  -DCONFIG_TFM_SPM_BACKEND=SFN

If ``CONFIG_TFM_SPM_BACKEND`` is not set, then ``IPC`` is the default value.

**********
References
**********

.. [1] `FF-M v1.0 Specification <https://developer.arm.com/-/media/Files/pdf/PlatformSecurityArchitecture/Architect/DEN0063-PSA_Firmware_Framework-1.0.0-2.pdf?revision=2d1429fa-4b5b-461a-a60e-4ef3d8f7f4b4>`__

.. [2] `FF-M v1.1 Extention <https://documentation-service.arm.com/static/600067c09b9c2d1bb22cd1c5?token=>`__

--------------

*Copyright (c) 2022, Arm Limited. All rights reserved.*
