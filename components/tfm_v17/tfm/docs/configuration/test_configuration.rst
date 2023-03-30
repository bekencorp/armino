.. _test_configuration:

###################
Tests configuration
###################

Regression tests
================

Regression test configuration is controlled entirely by ``TEST_NS`` or
``TEST_S`` or single test suite configuration. The group test
configurations and single test suite configurations are listed below, all of
them are disabled by default.

+---------------------+--------------------------------------------------------------------+
| Parameter           | Description                                                        |
+=====================+====================================================================+
| TEST_NS_ATTESTATION | Build non-secure regression Attestation tests.                     |
+---------------------+--------------------------------------------------------------------+
| TEST_NS_T_COSE      | Build non-secure regression t_cose tests.                          |
+---------------------+--------------------------------------------------------------------+
| TEST_NS_QCBOR       | Build non-secure regression QCBOR tests.                           |
+---------------------+--------------------------------------------------------------------+
| TEST_NS_CRYPTO      | Build non-secure regression Crypto tests.                          |
+---------------------+--------------------------------------------------------------------+
| TEST_NS_ITS         | Build non-secure regression ITS tests.                             |
+---------------------+--------------------------------------------------------------------+
| TEST_NS_PS          | Build non-secure regression PS tests.                              |
+---------------------+--------------------------------------------------------------------+
| TEST_NS_PLATFORM    | Build non-secure regression Platform tests.                        |
+---------------------+--------------------------------------------------------------------+
| TEST_NS_FWU         | Build non-secure regression FWU tests.                             |
+---------------------+--------------------------------------------------------------------+
| TEST_NS_IPC         | Build non-secure regression IPC tests.                             |
+---------------------+--------------------------------------------------------------------+
| TEST_NS_SLIH_IRQ    | Build non-secure regression Second-Level Interrupt Handling tests. |
+---------------------+--------------------------------------------------------------------+
| TEST_NS_FLIH_IRQ    | Build non-secure regression First-Level Interrupt Handling tests.  |
+---------------------+--------------------------------------------------------------------+
| TEST_NS_MULTI_CORE  | Build non-secure regression multi-core tests.                      |
+---------------------+--------------------------------------------------------------------+
| TEST_S_ATTESTATION  | Build secure regression Attestation tests.                         |
+---------------------+--------------------------------------------------------------------+
| TEST_S_CRYPTO       | Build secure regression Crypto tests.                              |
+---------------------+--------------------------------------------------------------------+
| TEST_S_ITS          | Build secure regression ITS tests.                                 |
+---------------------+--------------------------------------------------------------------+
| TEST_S_PS           | Build secure regression PS tests.                                  |
+---------------------+--------------------------------------------------------------------+
| TEST_S_PLATFORM     | Build secure regression Platform tests.                            |
+---------------------+--------------------------------------------------------------------+
| TEST_S_FWU          | Build secure regression FWU tests.                                 |
+---------------------+--------------------------------------------------------------------+
| TEST_S_IPC          | Build secure regression IPC tests.                                 |
+---------------------+--------------------------------------------------------------------+

Individual test suites can be enabled when their dependencies like partitions or
other specific configurations are set. On the one hand, some test suites depend
on other test suites. On the other hand, some test suites conflict with
other test suites. Test configurations and dependencies will be
checked in ``${TFM_TEST_REPO_PATH}/test/config/check_config.cmake``.

If regression testing is enabled by ``TEST_NS`` or ``TEST_S``, individual
test suites will be enabled or disabled as appropriate for the TF-M
configuration (i.e. all enabled secure partitions will be tested).

Some cryptographic tests can be enabled and disabled. This is done to prevent
false failures from being reported when a smaller Mbed Crypto config is being
used which does not support all features.

+---------------------------------------+---------------------------------------+---------------+
| Parameter                             | Description                           | Default value |
+=======================================+=======================================+===============+
| TFM_CRYPTO_TEST_ALG_CBC               | Test CBC cryptography mode            | ON            |
+---------------------------------------+---------------------------------------+---------------+
| TFM_CRYPTO_TEST_ALG_CCM               | Test CCM cryptography mode            | ON            |
+---------------------------------------+---------------------------------------+---------------+
| TFM_CRYPTO_TEST_ALG_CFB               | Test CFB cryptography mode            | ON            |
+---------------------------------------+---------------------------------------+---------------+
| TFM_CRYPTO_TEST_ALG_ECB               | Test ECB cryptography mode            | ON            |
+---------------------------------------+---------------------------------------+---------------+
| TFM_CRYPTO_TEST_ALG_CTR               | Test CTR cryptography mode            | ON            |
+---------------------------------------+---------------------------------------+---------------+
| TFM_CRYPTO_TEST_ALG_OFB               | Test OFB cryptography mode            | ON            |
+---------------------------------------+---------------------------------------+---------------+
| TFM_CRYPTO_TEST_ALG_GCM               | Test GCM cryptography mode            | ON            |
+---------------------------------------+---------------------------------------+---------------+
| TFM_CRYPTO_TEST_ALG_SHA_384           | Test SHA-384 cryptography algorithm   | ON            |
+---------------------------------------+---------------------------------------+---------------+
| TFM_CRYPTO_TEST_ALG_SHA_512           | Test SHA-512 cryptography algorithm   | ON            |
+---------------------------------------+---------------------------------------+---------------+
| TFM_CRYPTO_TEST_HKDF                  | Test HKDF key derivation algorithm    | ON            |
+---------------------------------------+---------------------------------------+---------------+
| TFM_CRYPTO_TEST_ECDH                  | Test ECDH key agreement algorithm     | ON            |
+---------------------------------------+---------------------------------------+---------------+
| TFM_CRYPTO_TEST_CHACHA20              | Test ChaCha20 stream cipher           | ON            |
+---------------------------------------+---------------------------------------+---------------+
| TFM_CRYPTO_TEST_ALG_CHACHA20_POLY1305 | Test ChaCha20-Poly1305 AEAD algorithm | ON            |
+---------------------------------------+---------------------------------------+---------------+

PSA tests
=========

PSA tests are configured by using the ``TEST_PSA_API`` cmake variable. The
variable should be set to the name of the test suite that is desired. It is
_not_ supported to set both ``TEST_PSA_API`` and ``TEST_S`` or ``TEST_NS``.

The Functional API tests are:
 - ``CRYPTO``
 - ``INITIAL_ATTESTATION``
 - ``STORAGE`` (INTERNAL_TRUSTED_STORAGE and PROTECTED_STORAGE)
 - ``INTERNAL_TRUSTED_STORAGE``
 - ``PROTECTED_STORAGE``

The Firmware Framework test suites are:
 - ``IPC``
 - ``SFN``
 - ``Interrupt``

Note that these map directly to the ``SUITE`` cmake variable used in the
psa-arch-tests documentation.
