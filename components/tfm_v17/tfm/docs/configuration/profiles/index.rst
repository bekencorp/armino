.. _tf-m_profiles:

#############
TF-M Profiles
#############

The capabilities and resources may dramatically vary on different IoT devices.
Some IoT devices may have very limited memory resource. The program on those devices should keep
small memory footprint and basic functionalities.
On the other hand, some devices may consist of more memory and extended storage, to support stronger
software capabilities.

Diverse IoT use cases also require different levels of security and requirements on device resource.
For example, use cases require different cipher capabilities. Selecting cipher suites can be
sensitive to memory footprint on devices with constrained resource.

Trusted Firmware-M (TF-M) defines several general profiles, such as Profile Small, Profile Medium,
Profile Medium ARoT-less and Profile Large, to provide different levels of security to fit diverse
device capabilities and use cases applied on the top of the base configuration.

Each profile specifies a predefined list of features, targeting typical use cases with specific
hardware constraints. Profiles can serve as reference designs, based on which developers can
continue further development and configurations, according to use case.

TF-M Profiles align with Platform Security Architecture specifications and certification
guidelines. It can help vendors to simplify security configuring for PSA certification.

Please check the table below to compare differences while details are discussed
in the links below.

.. toctree::
    :maxdepth: 1
    :glob:

     Small <tfm_profile_small>
     ARoT-less <tfm_profile_medium_arot-less>
     Medium <tfm_profile_medium>
     Large <tfm_profile_large>

+----------------------------------------+--------+--------+---------+--------+--------+
| Option                                 | Base   | Small  |ARoT-less| Medium | Large  |
+========================================+========+========+=========+========+========+
| TFM_ISOLATION_LEVEL                    | 1      | 1      | 1       | 2      | 3      |
+----------------------------------------+--------+--------+---------+--------+--------+
| CONFIG_TFM_SPM_BACKEND                 | SFN    | SFN    | SFN     | IPC    | IPC    |
+----------------------------------------+--------+--------+---------+--------+--------+
| TFM_PARTITION_CRYPTO                   | OFF    | ON     | ON      | ON     | ON     |
+----------------------------------------+--------+--------+---------+--------+--------+
| TFM_PARTITION_INTERNAL_TRUSTED_STORAGE | OFF    | ON     | ON      | ON     | ON     |
+----------------------------------------+--------+--------+---------+--------+--------+
| TFM_PARTITION_PLATFORM                 | OFF    | OFF    | ON      | ON     | ON     |
+----------------------------------------+--------+--------+---------+--------+--------+
| TFM_PARTITION_PROTECTED_STORAGE        | OFF    | OFF    | OFF     | ON     | ON     |
+----------------------------------------+--------+--------+---------+--------+--------+
| TFM_PARTITION_INITIAL_ATTESTATION      | OFF    | ON     | ON      | ON     | ON     |
+----------------------------------------+--------+--------+---------+--------+--------+
| SYMMETRIC_INITIAL_ATTESTATION          | OFF    | ON     | OFF     | OFF    | OFF    |
+----------------------------------------+--------+--------+---------+--------+--------+
| TFM_PARTITION_FIRMWARE_UPDATE          | OFF    | OFF    | ON      | OFF    | OFF    |
+----------------------------------------+--------+--------+---------+--------+--------+
| PS_CRYPTO_AEAD_ALG                     | GCM    | `-`    | `-`     | CCM    | CCM    |
+----------------------------------------+--------+--------+---------+--------+--------+
| PSA_FRAMEWORK_HAS_MM_IOVEC             | OFF    | ON     | OFF     | OFF    | OFF    |
+----------------------------------------+--------+--------+---------+--------+--------+
| MCUBOOT_IMAGE_NUMBER :sup:`1`          | 2      | 1      | 2       | 2      | 2      |
+----------------------------------------+--------+--------+---------+--------+--------+
| *Advanced options, defined in the corresponded header (.h) file*                     |
+----------------------------------------+--------+--------+---------+--------+--------+
| CRYPTO_ENGINE_BUF_SIZE                 | 0x2080 | 0x400  | 0x2080  | 0x2080 | 0x2080 |
+----------------------------------------+--------+--------+---------+--------+--------+
| CRYPTO_ASYM_SIGN_MODULE_ENABLED        | ON     | OFF    | ON      | ON     | ON     |
+----------------------------------------+--------+--------+---------+--------+--------+
| CRYPTO_ASYM_ENCRYPT_MODULE_ENABLED     | ON     | OFF    | OFF     | OFF    | ON     |
+----------------------------------------+--------+--------+---------+--------+--------+
| CRYPTO_SINGLE_PART_FUNCS_DISABLED      | OFF    | ON     | OFF     | OFF    | OFF    |
+----------------------------------------+--------+--------+---------+--------+--------+
| CRYPTO_CONC_OPER_NUM                   | 8      | 4      | 8       | 8      | 8      |
+----------------------------------------+--------+--------+---------+--------+--------+
| CONFIG_TFM_CONN_HANDLE_MAX_NUM         | 8      | 3      | 8       | 8      | 8      |
+----------------------------------------+--------+--------+---------+--------+--------+
| ITS_BUF_SIZE :sup:`2`                  | 512    | 32     | 32      | 32     | 512    |
+----------------------------------------+--------+--------+---------+--------+--------+

1. `MCUBOOT_IMAGGE_NUMBER` value is taken from MCUBoot default configuration,
   execept profile Small.
2. Many platforms redefine `ITS_BUF_SIZE` value.

Each profile has predefined configuration for cryptographic library, located in
``/lib/ext/mbedcrypto/mbedcrypto_config/``

--------------

*Copyright (c) 2020, Arm Limited. All rights reserved.*
