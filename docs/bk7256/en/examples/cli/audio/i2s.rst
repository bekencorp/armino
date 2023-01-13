I2S Cli
================

:link_to_translation:`zh_CN:[Chinese]`

1、Function overview
--------------------------
	In order to facilitate the verification of I2S codec-related functions, a series of cli CMDs are designed, which are also I2S function scene demos for customers' reference.


2、Code path
--------------------------
	path: ``components\demos\media\audio\aud_intf\demo\i2s_demo.c``

3、Cli command introduction
----------------------------------
The supported CMDs are as follows:

	1.i2s_master_test
	 - Function Description
		Used to test sending data in standard ``Philips`` mode as ``master``.
	 - CMD Format
		i2s_master_test param1

		+-----------+------------------------------------------------------------------------+
		|Param      | value                                                                  |
		+===========+========================================================================+
		|param1     | opcode                                                                 |
		|           |  | start : Start send                                                  |
		|           |  | stop : Stop send                                                    |
		+-----------+------------------------------------------------------------------------+

	 - Test Case
		| Start send：``i2s_master_test start``
		| Stop send：``i2s_master_test stop``

	2.i2s_slave_test
	 - Function Description
		Used to test receiving data as a ``slave`` in standard ``Philips`` mode.
	 - CMD Format
		i2s_slave_test param1

		+-----------+------------------------------------------------------------------------+
		|Param      | value                                                                  |
		+===========+========================================================================+
		|param1     | opcode                                                                 |
		|           |  | start : Start receive                                               |
		|           |  | stop : Stop receive                                                 |
		+-----------+------------------------------------------------------------------------+

	 - Test Case
		| Start receive：``i2s_slave_test start``
		| Stop receive：``i2s_slave_test stop``

	3.pcm_master_test
	 - Function Description
		Used to test sending data in ``PCM`` mode as a ``master``.
	 - CMD Format
		pcm_master_test param1

		+-----------+------------------------------------------------------------------------+
		|Param      | value                                                                  |
		+===========+========================================================================+
		|param1     | opcode                                                                 |
		|           |  | start : Start send                                                  |
		|           |  | stop : Stop send                                                    |
		+-----------+------------------------------------------------------------------------+

	 - Test Case
		| Start send：``pcm_master_test start``
		| Stop send：``pcm_master_test stop``

	4.pcm_slave_test
	 - Function Description
		Used to test receiving data as a ``slave`` in standard ``PCM`` mode.
	 - CMD Format
		pcm_slave_test param1

		+-----------+------------------------------------------------------------------------+
		|Param      | value                                                                  |
		+===========+========================================================================+
		|param1     | opcode                                                                 |
		|           |  | start : Start receive                                               |
		|           |  | stop : Stop receive                                                 |
		+-----------+------------------------------------------------------------------------+

	 - Test Case
		| Start receive：``pcm_slave_test start``
		| Stop receive：``pcm_slave_test stop``

	5.dtm_master_test
	 - Function Description
		Used to test sending data in ``PCM+DTM`` mode as ``master``.
	 - CMD Format
		dtm_master_test param1

		+-----------+------------------------------------------------------------------------+
		|Param      | value                                                                  |
		+===========+========================================================================+
		|param1     | opcode                                                                 |
		|           |  | start : Start send                                                  |
		|           |  | stop : Stop send                                                    |
		+-----------+------------------------------------------------------------------------+

	 - Test Case
		| Start send：``dtm_master_test start``
		| Stop send：``dtm_master_test stop``

	6.dtm_slave_test
	 - Function Description
		Used to test receiving data as a ``slave`` in the standard ``PCM+DTM`` mode.
	 - CMD Format
		dtm_slave_test param1

		+-----------+------------------------------------------------------------------------+
		|Param      | value                                                                  |
		+===========+========================================================================+
		|param1     | opcode                                                                 |
		|           |  | start : Start receive                                               |
		|           |  | stop : Stop receive                                                 |
		+-----------+------------------------------------------------------------------------+

	 - Test Case
		| Start receive：``dtm_slave_test start``
		| Stop receive：``dtm_slave_test stop``

	7.2bd_master_test
	 - Function Description
		Used to test sending data in ``2B+D`` mode as ``master``.
	 - CMD Format
		2bd_master_test param1

		+-----------+------------------------------------------------------------------------+
		|Param      | value                                                                  |
		+===========+========================================================================+
		|param1     | opcode                                                                 |
		|           |  | start : Start send                                                  |
		|           |  | stop : Stop send                                                    |
		+-----------+------------------------------------------------------------------------+

	 - Test Case
		| Start send：``2bd_master_test start``
		| Stop send：``2bd_master_test stop``

	8.2bd_slave_test
	 - Function Description
		Used to test receiving data as ``slave`` in standard ``2B+D`` mode.
	 - CMD Format
		2bd_slave_test param1

		+-----------+------------------------------------------------------------------------+
		|Param      | value                                                                  |
		+===========+========================================================================+
		|param1     | opcode                                                                 |
		|           |  | start : Start receive                                               |
		|           |  | stop : Stop receive                                                 |
		+-----------+------------------------------------------------------------------------+

	 - Test Case
		| Start receive：``2bd_slave_test start``
		| Stop receive：``2bd_slave_test stop``
