SPP
========================

:link_to_translation:`zh_CN:[Chinese]`

Overview
--------------------------
SPP aims to achieve data transparent transmission between two devices support spp profile.

Introduction
--------------------------
1. spp work flow chart:

.. figure:: ../../../_static/bt_spp_work_flow.png
    :align: center
    :alt: spp work flow chart
    :figclass: align-center

    Figure 1. spp work flow

2. Tools required for verification: two development boards or a development borad with a mobile phone, the mobile phone need to install spp related application. The two development boards can be used for throughput testing.


Instructions
--------------------------
 1) The development board A acts as a server, through the AT command to initialize the SPP SERVER: ``AT+BT=SPP_INIT``
 2) The development board B acts as a client, through the AT command to initiate a connection: ``AT+BT=SPP_CONNECT,xx:xx:xx:xx:xx:xx`` (the Bluetooth address of the development board A or the phone)
 3) After being prompted that the spp connection is successful, you can send data through the AT command: ``AT+BT=SPP_TX,beken_spp_tx_test!!!!`` (send datas)
 4) After the spp connection is successful, the throughput test can be done through the AT command: ``AT+BT=SPP_THROUGH_TEST,0xffff`` (data length, the sent data is randomly generated data, after the specified length is sent, the client and server log will output the transmission rate and crc value)
 5) When testing with a mobile phone, after the mobile phone opens the SPP application, the development board can directly operate 2.


example
--------------------------
 | SPP related at test code refer to: ``\components\at\src\bt_at_command.c``
 | For detailed introduction of AT commands, please refer to: ``examples/cli/bluetooth/bt.html``