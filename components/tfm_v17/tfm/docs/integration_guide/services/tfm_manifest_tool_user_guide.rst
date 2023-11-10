#############################
TF-M Manifest Tool User Guide
#############################
This document describes the TF-M manifest tool and its usage.
The target audiences are mainly platform integrators and Secure Partition
developers.

This document assumes that audiences have knowledge about the terminologies
defined in PSA Firmware Framework (FF-M) v1.0 [1]_ and the FF-M v1.1 extensions
[2]_, such as Secure Partition and manifests.

************
Introduction
************
The TF-M manifest tool is a python script which is used to parse Secure
Partition manifests and generate source header files defined by FF-M [1]_ such
as ``psa_manifest/pid.h`` and ``psa_manifest/sid.h``.
It also generates TF-M specific files for building.

In the TF-M build system, the manifest tool is invoked during building
automatically. Arguments are passed through CMake variables and they are
customizable.
See `Usage in TF-M Build System`_ for more details.

It can be also used as a standalone tool.

*********
Arguments
*********
The tool takes 5 arguments at most.

.. code-block:: bash

    tfm_parse_manifest_list.py [-h] -o out_dir
                               -m manifest list [manifest list ...]
                               -f file-list [file-list ...]
                               -c config-files [config-files ...]
                               [-q]

**-o/--outdir**

Required.

The directory to hold the output files.

**-m/--manifest-lists**

Required. At least one item must be provided.

A list of TF-M Secure Partition manifest lists which contain one or more
Secure Partition manifests in each.
See `Manifest List`_ for details.

**-f/--file-list**

Required. At least one item must be provided.

A list of files that describe what files the tool should generate.
See `Generated File List`_ for details.

**-c/--config-files**

Required. At least one item must be provided.

A list of header files which contain build configurations such as isolation
level and Secure Partition enabled status.
See `Configuration Header File`_ for details.

**-q/--quiet**

Optional.

Reduces log messages, only warnings and errors will be printed.

.. _tfm_manifest_list:

Manifest List
=============
A manifest list is a YAML [3]_ file that describes a list of Secure Partition
manifests.
Refer to the TF-M default manifest list [4]_ as an example.

Each manifest list must contain a ``manifest_list`` attribute which collects the
descriptions of Secure Partition manifests.
Following are the supported attributes of in the manifest lists.

- ``description``

  Required.

  Descriptive information for the tool to refer to the Secure Partition, for
  example in logs.

- ``manifest``

  Required.

  The manifest file of the Secure Partition.
  Both absolute path and relative path to the manifest list are supported.
  Environment variables are supported.

- ``output_path``

  Optional.

  The directory to hold the Secure Partition specific output files.
  See `Generated File List`_ for more details.

  Both absolute path and relative path to the directory specified by
  ``-o/--outdir`` are supported.
  Environment variables are supported.

  It is set to the directory specified by ``-o/--outdir`` if omitted.

- ``conditional``

  Optional.

  The configuration to enable or disable this Secure Partition.
  The value must be defined in one of the `Configuration Header File`_.
  If it is omitted, the Secure Partition is always enabled.

- ``pid``

  Optional.

  The Secure Partition ID (PID).

  **For non-test purpose Secure Partitions, it is required.**
  See :doc:`Adding Secure Partition <tfm_secure_partition_addition>`
  for the PID allocations.

  For test purpose Secure Partitions, this attribute is optional.
  The manifest tool assigns one for the Secure Partition.
  The value is not guaranteed to the same for different builds.

- ``linker_pattern``

  Optional.

  The information for linker to place the symbols of the Secure Partition.
  It is only required if you are using the linker scripts provided by TF-M.
  Each Secure Partition is expected to be built as a library.
  The name of the library must follow the format of
  ``tfm_<type>_partition_<name>``.
  The valid value for ``<type>`` is ``[psa_rot, app_rot]`` corresponding to the
  type of the Secure Partitions.
  The ``<name>`` is any string to distinguish the Secure Partition from others.

  Supported patterns are:

  - ``library_list``, must be ``*tfm_*partition_<name>.*``.
  - ``object_list``

    Any object files containing symbols belonging to the Secure Partition that
    are not included in the Secure Partitions library.

Generated File List
===================
A generated file list is a YAML file that describes the files to be generated
by the manifest tool.
Refer to TF-M default generated file list [5]_ as an example.

Each one must contain a ``file_list`` attribute which collects the files to
generate.
Each item in the ``file_list`` must contain the following attributes.

- ``template``

  This attribute is the file path of a Jinja2 [6]_ template.
  The TF-M manifest tool uses Jinja2 template engine for file generations.
  It can be a relative path to TF-M root directory or an absolute path.
  Environment variables are supported.

- ``output``

  The output file of the ``template``.
  Both absolute path and relative path to the directory specified by
  ``-o/--outdir`` are supported.
  Environment variables are supported.

**The tfm_generated_file_list.yaml is essential to build TF-M.**

There are several files that are required for each Secure Partition,
so they are not in any generated file lists since one template generates
multiple files.

- ``psa_manifest/<manifestfilename>.h``

  ``manifestfilename`` is the file name of the manifest.
  This file contains internal definitions for the Secure Partition
  implementation, such as RoT Service signals and Secure Functions.
  Refer to FF-M [1]_ for more details.
  The corresponding template is ``manifestfilename.template``

- ``intermedia_<manifestfilename>.c``

  TF-M specific, which holds the stacks of Secure Partitions.
  This file must be built with the Secure Partition libraries.
  The corresponding template is ``partition_intermedia.template``.

- ``load_info_<manifestfilename>.c``

  TF-M specific, which contains the load information of Secure Partitions.
  This file must be built with the TF-M SPM library.
  The corresponding template is ``partition_load_info.template``.

These files are generated to ``output_path`` specified by each Secure Partition
in the manifest lists.

Configuration Header File
=========================
The format of each configuration item must be

.. code-block::

    #define CONFIG_NAME   VALUE

The following format is also supported for boolean type configurations.

.. code-block::

    #define CONFIG_NAME

The configurations can be divided into two categories.

- Generic configurations:

  - ``PSA_FRAMEWORK_ISOLATION_LEVEL``

    The isolation level, required. Valid values are ``[1, 2, 3]``.

  - ``CONFIG_TFM_SPM_BACKEND``

    The backend of SPM, required. Valid values are ``[SFN, IPC]``.
    See :doc:`SPM backends </integration_guide/spm_backends>`
    for details of backends.

- Secure Partition enablement configurations

  Configurations used to enable or disable Secure Partitions.
  The configuration names must match the values of ``conditional`` attributes in
  `Manifest List`_. Valid values are ``[0, 1]``.
  It's optional for a Secure Partition which does not have the ``conditional``
  attribute.

The configurations can be split to multiple files corresponding to the multiple
manifest lists.

**************************
Usage in TF-M Build System
**************************
In the TF-M build system, the manifest tool is invoked during building
automatically.
The arguments can be customized by altering the CMake configurations.

The manifest lists are passed to the manifest tool via the ``TFM_MANIFEST_LIST``
CMake configuration.
The default value is the ``tfm_manifest_list.yaml``.
It can be overridden or appended with other manifest lists.

Corresponding manifest lists of test Secure Partitions are appended if either
TF-M regression or PSA compliance tests are enabled.

The generated file lists are passed via ``GENERATED_FILE_LISTS``.
It can be also overridden or appended with other file lists.

The ``-q`` argument is appended if ``PARSE_MANIFEST_QUIET_FLAG`` is enabled.

Paths in manifest lists and generated file lists can have CMake variables as
long as the are absolute paths.
The lists then must be processed by the CMake command ``configure_file()`` [7]_
before passing to the manifest tool.

The configuration header file is generated by the build system automatically.

**********
References
**********

.. [1] `FF-M v1.0 Specification <https://developer.arm.com/-/media/Files/pdf/PlatformSecurityArchitecture/Architect/DEN0063-PSA_Firmware_Framework-1.0.0-2.pdf?revision=2d1429fa-4b5b-461a-a60e-4ef3d8f7f4b4>`__

.. [2] `FF-M v1.1 Extension <https://documentation-service.arm.com/static/600067c09b9c2d1bb22cd1c5?token=>`__

.. [3] `YAML <https://yaml.org/>`__

.. [4] `TF-M manifest list <https://git.trustedfirmware.org/TF-M/trusted-firmware-m.git/tree/tools/tfm_manifest_list.yaml>`__

.. [5] `TF-M generated file list <https://git.trustedfirmware.org/TF-M/trusted-firmware-m.git/tree/tools/tfm_generated_file_list.yaml>`__

.. [6] `Jinja2 <https://jinja.palletsprojects.com/en/3.1.x/>`__

.. [7] `CMake configure_file() <https://cmake.org/cmake/help/latest/command/configure_file.html>`__

--------------

*Copyright (c) 2022, Arm Limited. All rights reserved.*
*Copyright (c) 2022 Cypress Semiconductor Corporation (an Infineon company)
or an affiliate of Cypress Semiconductor Corporation. All rights reserved.*
