#################
TF-M builtin keys
#################

:Author: Raef Coles
:Organization: Arm Limited
:Contact: raef.coles@arm.com

************
Introduction
************

TF-M has several keys that are bound to the device itself instead of a secure
partition. These keys must be accessed through a HAL function, either loading
them from OTP or another platform-specific location. These keys are henceforth
referred to as "builtin keys", and include (but are not limited to):

1) The Hardware Unique Key (HUK)
2) The Initial Attestation Key (IAK)

Currently, the IAK is loaded by the attestation partition as a transient key,
which requires some key-loading logic to be implemented by that partition. The
HUK is not loaded in the crypto service, and is instead used by an
implementation of a TF-M specific KDF algorithm which then loads the key and
invokes Mbed TLS directly.

****************
PSA builtin keys
****************

The PSA Cryptographic API provides a mechanism for accessing keys that are
stored in platform-specific locations (often hardware accelerators or OTP). One
of the properties of builtin keys is that they are accessed via a predefined
handle, which can be leveraged to allow TF-M to define a set of handles for the
builtin keys that it provides.

Defining these constant handles allows these keys to be used by secure partition
and non-secure callers (subject to access policy), via the standard PSA crypto
interfaces.

Ideally, it would be possible to just have PSA builtin keys that are stored in
crypto service RAM, in the same way that volatile keys are. Mbed TLS does not
support this and only supports builtin keys as part of the code flow that
interfaces with hardware accelerators.

*********************
PSA crypto driver API
*********************

The PSA crypto driver API allows most PSA Crypto APIs to defer their operation
to an accelerator driver in preference of the software implementation. It also
adds the concept of storage locations for keys, which is used to access keys
stored on hardware accelerators.

The TF-M builtin keys code leverages the PSA crypto driver API by creating a new
driver that provides no acceleration, only a key storage location. This storage
location is not backed by hardware, but is instead inside the RAM of the crypto
partition.

This is done by hooking two functions into the
``library/psa_crypto_driver_wrappers.c`` file. These functions are:

1) ``tfm_key_loader_get_builtin_key``
2) ``tfm_key_loader_get_builtin_key_len``

The flow for these functions being used is:

1) A request is made to a PSA Crypto API that references a key by a key
   handle.
2) The PSA Crypto core layer checks that the handle is inside the builtin keys
    region, and then if the key has not yet been loaded into a transient Mbed
    TLS keyslot calls ``tfm_plat_builtin_key_get_lifetime_and_slot`` (which is a
    wrapper around ``mbedtls_psa_platform_get_builtin_key``), which is defined
    in ``crypto_keys.h``. This function maps each builtin key to a driver, which
    in most cases is the default ``tfm_builtin_key_loader`` via
    ``TFM_BUILTIN_KEY_LOADER_KEY_LOCATION``. The function also returns a slot
    number, which is a driver-specific index to specify the key.
3) This location and slot index then calls
   ``psa_driver_wrapper_get_builtin_key``, which for the key location
   ``TFM_BUILTIN_KEY_LOADER_KEY_LOCATION`` (the new location value that is bound
   to the TF-M builtin keys driver) calls the previously hooked function
   ``tfm_key_loader_get_builtin_key``.
4) This function, along with its counterpart
   ``tfm_key_loader_get_builtin_key_len``, allow Mbed TLS to copy the
   key material into an internal keyslot, which is then used whenever further
   calls to using that same builtin key ID are made.

In order to load the keys into the tfm_key_loader memory (in the crypto
partition), ``crypto_keys.h`` defines a function ``tfm_plat_load_builtin_keys``
which is responsible for loading all builtin keys into and driver that requires
loading.

*****************
Technical details
*****************

------------------------------
Builtin key IDs and overriding
------------------------------

TF-M builtin key IDs are defined in ``interface/include/tfm_crypto_defs.h`` by
the enum ``tfm_key_id_builtin_t``. They are allocated inside the range that PSA
considers to be builtin keys. A platform can specify extra builtin key IDs by
setting the ``PLATFORM_DEFAULT_CRYPTO_KEYS`` variable to ``OFF``, creating the
header ``platform_builtin_key_ids.h``, and specifying new keys and IDs.

--------------------------
Builtin key access control
--------------------------

Builtin keys by default can be used by any caller since the key handle is
public information. TF-M must mediate access to the keys, which is done in the
function ``tfm_plat_builtin_key_get_usage`` (part of ``crypto_keys.h``). This
function maps the caller ID to a particular key usage, which allows granular key
permissions. The function returns ``PSA_ERROR_NOT_PERMITTED`` if a caller does
not have permission to use the key.

------------------------------
Multi-partition key derivation
------------------------------

The HUK is used for key derivation by any secure partition or NS caller that
requires keys that are bound to a particular context. For example, Protected
Storage derives keys uniquely for each user of the service which are used to
encrypt each user's files. In order to provide HUK derivation to every secure
partition / NS caller, it must be ensured that no service that utilises HUK
derivation can derive the same key as another service (simply by inputting the
same KDF inputs).

This is accomplished by deriving a further "platform key" for each builtin key
that can be used for key derivation. These platform keys are derived from the
builtin key, using the partition ID as a KDF input, and can then be used for
further derivation by the partition (or NS caller) with the further derived keys
being unique for each partition even if the KDF inputs are the same.

.. Note::
    If the NS client ID feature is disabled, all NS callers share a partition ID
    of ``-1``, and therefore will share a platform key and be therefore be able
    to derive the same keys as other NS callers.

For keys that are not exposed outside the device, this is transparent to the
service that is using the key derivation, as they have no access to the builtin
key material and cannot distinguish between keys derived directly from it and
keys derived from the platform key. For some builtin keys, deriving platform
keys is not acceptable, as the key is used outside the device (i.e. the IAK
public key is used to verify attestation tokens) so the actual builtin key is
used.

The decision has been taken to derive platform keys for any key that can be used
for key derivation (``PSA_KEY_USAGE_DERIVE``), and not derive platform keys
otherwise. For builtin keys that do not derive platform keys but are directly
used, care must be taken with access control where multiple partitions have
access.

---------------------------------
Mbed TLS transparent builtin keys
---------------------------------

Mbed TLS does not natively support transparent builtin keys (transparent keys
are keys where the key material is directly accessible to the PSA Crypto core),
so some modifications had to be made. Opaque keyslots have the same basic
structure as standard transparent keyslots, and can be passed to the functions
usually reserved for transparent keys, though this behaviour is not defined and
may not continue to work in future versions. Therefore, the only modification
required currently is to force keys that have the location
``TFM_BUILTIN_KEY_LOADER_KEY_LOCATION`` to be passed to the functions that only
usually accept keys with the location ``PSA_KEY_LOCATION_LOCAL_STORAGE``.

--------------

*Copyright (c) 2022, Arm Limited. All rights reserved.*
