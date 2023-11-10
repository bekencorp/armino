.. _tfm_cmake_configuration:

###################
Build configuration
###################

All configuration options are provided by cmake variables, and their default
values, with docstrings, can be found in ``config/config_base.cmake``.

Configuration is provided in multiple stages. Each stage will not override any
config that has already been set at any of the prior stages.

   1. Command-line variable settings are applied.
   2. If the ``TFM_EXTRA_CONFIG_PATH`` variable has been set, that file is
      loaded.
   3. If TEST_PSA_TEST is set, then PSA API test related config is applied from
      ``config/tests/config_test_psa_api.cmake``.
   4. If it exists, CMAKE_BUILD_TYPE specific config is applied from
      ``config/build_type/<build_type>.cmake``.
   5. Target specific config from ``platform/ext/target/<target_platform>/config.cmake``
      is applied.
   6. If CRYPTO_HW_ACCELERATOR is set, then a config specific to the
      accelerator type is applied if it exists.
   7. If it exists, TFM Profile specific config is applied from
      ``config/profile/<tfm_profile>.cmake``.
   8. ``config/config_default.cmake`` is loaded.
   9. If ``TEST_S`` or ``TEST_NS`` or other single test suite config like
      ``TEST_NS_ATTESTATION`` (see :ref:`test_configuration`) is set, then
      config from ``${TFM_TEST_REPO_PATH}/test/config/set_config.cmake`` and
      ``${TFM_TEST_REPO_PATH}/test/config/default_ns_test_config.cmake`` or
      ``${TFM_TEST_REPO_PATH}/test/config/default_s_test_config.cmake`` or
      ``${TFM_TEST_REPO_PATH}/test/config/default_test_config.cmake`` is
      applied.

.. Warning::
    This means that command-line settings are not applied when they conflict
    with required platform settings. If it is required to override platform
    settings (this is not usually a good idea) then TFM_EXTRA_CONFIG_PATH should be
    used.
