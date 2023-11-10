DUT
========================

:link_to_translation:`zh_CN:[中文]`

Overview
--------------------------
	It is introduced how to start a Bluetooth DUT test, including BLE non-signaling test and BT signaling and non-signaling test.

Introduction
--------------------------
1. BLE non-signaling test

 - test environment
	Connect the rf port of the test board to the rf port of the test equipment (such as CMW270), and connect the serial cable on the test board to the test equipment.

 - Enter ATE mode
	First connect the ATE jumper cap of the serial port cable, and then power on to enter the ATE mode

 - open test path
	Send ``ble dut`` command to serial port

 - start test
	Perform BLE non-signaling test according to the test equipment instructions

2. BT signaling test

 - test environment
	Connect the rf port of the test board to the rf port of the test equipment (such as CMW270), and connect the serial cable on the test board to the test equipment.

 - Enter ATE mode
	First connect the ATE jumper cap of the serial port cable, and then power on to enter the ATE mode

 - open test path
	Send ``ble dut`` command to serial port

 - start test
	Perform BT signaling test according to the test equipment instructions

3. BT non-signaling test

 - test environment
	Connect the rf port of the test board to the rf port of the test equipment (such as CMW270), and connect the serial cable on the test board to the test equipment.

 - Enter ATE mode
	First connect the ATE jumper cap of the serial port cable, and then power on to enter the ATE mode

 - open test path
	Send ``ble dut`` command to serial port

 - start test
	| The test equipment is in RX mode, send TX command through the serial port to perform the TX test, and send the end command to stop the TX test
	| The test equipment is in TX mode, send RX command through the serial port to perform the RX test, and send the end command to stop the RX test

	[TX/RX command format]
	  ``01 E0 FC 0C FD UAP LAP2 LAP1 LAP0 Test_Scenarios Hop_mode TX_chnl RX_chnl Interval Packet_Type Tx_Power``

	+------------------+---------------------------------------------------------------------------------+
	|01 E0 FC 0C FD    | fixed command header                                                            |
	+------------------+---------------------------------------------------------------------------------+
	|UAP LAP2 LAP1 LAP | Test equipment address, valid is in rx mode                                     |
	+------------------+---------------------------------------------------------------------------------+
	|Test_Scenarios    | test mode                                                                       |
	|                  |  | 01 : Repeated '00000000' (in transmission order) sequence                    |
	|                  |  | 02 : Repeated '11111111' (in transmission order) sequence                    |
	|                  |  | 03 : Repeated '10101010' (in transmission order) sequence                    |
	|                  |  | 04 : PN9 sequence (in transmission order)                                    |
	|                  |  | 07 : rx test                                                                 |
	|                  |  | 09 : Repeated '11110000' (in transmission order) sequence                    |
	+------------------+---------------------------------------------------------------------------------+
	|Hop_mode          | hop                                                                             |
	|                  |  | 00 : off                                                                     |
	|                  |  | 01 : on                                                                      |
	+------------------+---------------------------------------------------------------------------------+
	|TX_chnl           | tx channel                                                                      |
	|                  |  | 0-78,corresponding frequency 2402-2480                                       |
	+------------------+---------------------------------------------------------------------------------+
	|RX_chnl           | rx channel                                                                      |
	|                  |  | 0-78,corresponding frequency 2402-2480                                       |
	+------------------+---------------------------------------------------------------------------------+
	|Interval          | currently unused                                                                |
	+------------------+---------------------------------------------------------------------------------+
	|Packet_Type       | package type                                                                    |
	|                  |  | 0 : NULLpkt                                                                  |
	|                  |  | 1 : POLLpkt                                                                  |
	|                  |  | 2 : FHSpkt                                                                   |
	|                  |  | 3 : DM1                                                                      |
	|                  |  | 4 : DH1                                                                      |
	|                  |  | 5 : HV1                                                                      |
	|                  |  | 6 : HV2                                                                      |
	|                  |  | 7 : HV3                                                                      |
	|                  |  | 8 : DV                                                                       |
	|                  |  | 9 : AUX1                                                                     |
	|                  |  | 10 : DM3                                                                     |
	|                  |  | 11 : DH3                                                                     |
	|                  |  | 12 : EV4                                                                     |
	|                  |  | 13 : EV5                                                                     |
	|                  |  | 14 : DM5                                                                     |
	|                  |  | 15 : DH5                                                                     |
	|                  |  | 16 : IDpkt                                                                   |
	|                  |  | 20 : EDR_2DH1                                                                |
	|                  |  | 21 : EV3                                                                     |
	|                  |  | 22 : EDR_2EV3                                                                |
	|                  |  | 23 : EDR_3EV3                                                                |
	|                  |  | 24 : EDR_3DH1                                                                |
	|                  |  | 25 : EDR_AUX1                                                                |
	|                  |  | 26 : EDR_2DH3                                                                |
	|                  |  | 27 : EDR_3DH3                                                                |
	|                  |  | 28 : EDR_2EV5                                                                |
	|                  |  | 29 : EDR_3EV5                                                                |
	|                  |  | 30 : EDR_2DH5                                                                |
	|                  |  | 31 : EDR_3DH5                                                                |
	+------------------+---------------------------------------------------------------------------------+
	|Tx_Power          | currently inactive                                                              |
	+------------------+---------------------------------------------------------------------------------+

	[end command format]
	  ``01 E0 FC 01 90`` After sending this command, the serial port will receive an end resp

	[end resp format]
	  ``04 0E 18 01 E0 FC 90 TxTotolPackets RxTotolPackets RxValidPackets RxHECErrorCntr RxCRCErrorCntr``

	+----------------------+------------------------------------------------------------------------+
	|04 0E 18 01 E0 FC 90  | response fixed header                                                  |
	+----------------------+------------------------------------------------------------------------+
	|TxTotolPackets        | The total number of packets sent, valid in tx mode                     |
	|                      |  | This parameter is 4 bytes, displayed in little endian               |
	+----------------------+------------------------------------------------------------------------+
	|RxTotolPackets        | The total number of packets received, valid in rx mode                 |
	|                      |  | This parameter is 4 bytes, displayed in little endian               |
	+----------------------+------------------------------------------------------------------------+
	|RxValidPackets        | The total number of packets correctly received, valid in rx mode       |
	|                      |  | This parameter is 4 bytes, displayed in little endian               |
	+----------------------+------------------------------------------------------------------------+
	|RxHECErrorCntr        | The number of HEC error packets received, valid in rx mode             |
	|                      |  | This parameter is 4 bytes, displayed in little endian               |
	+----------------------+------------------------------------------------------------------------+
	|RxCRCErrorCntr        | The number of CRC error packets received, valid in rx mode             |
	|                      |  | This parameter is 4 bytes, displayed in little endian               |
	+----------------------+------------------------------------------------------------------------+

	Note: All command parameters are in hexadecimal format

	[Test Case]
		| tx  ：``01 e0 fc 0c fd 12 34 56 12 09 00 00 00 01 04 7f``
		| rx ：``01 e0 fc 0c fd 9c bd 35 9c 07 00 00 00 01 04 7f``
		| end tx/rx ：``01 E0 FC 01 90``
		| Received end resp ：``04 0E 18 01 E0 FC 90 DD 13 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00``


Reference link
----------------

    `API Reference : <../../api-reference/bluetooth/index.html>`_ Introduced the Bluetooth API interface

    `User and Developer Guide : <../../developer-guide/bluetooth/index.html>`_ Introduced common usage scenarios of Bluetooth

    `Samples and Demos: <../../examples/bluetooth/index.html>`_ Introduced the use and operation of Bluetooth samples

    `Bluetooth Project: <../../projects_work/bluetooth/index.html>`_ Introduced Bluetooth related Project
