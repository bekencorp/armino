BT Cli
===============

:link_to_translation:`zh_CN:[中文]`


1. Function overview
--------------------------
	In order to verify BT-related functions, a series of AT CMDs are designed. All BT AT CMDs start with ``AT+BT=``, followed by specific command and parameters. If the CMD is executed successfully, ``CMDRSP:OK`` is returned. If the CMD execution fails, ``CMDRSP:ERROR`` is returned.

2. Code path
--------------------------
	path: ``\components\at\src\bt_at_command.c``

3. Cli command introduction
---------------------------------
The supported CMDs are as follows:

    1.AT+BT=INQUIRY
     - Function Description
        Scan command, used to scan classic devices visible around
     - CMD Format
        AT+BT=INQUIRY
     - Parameter Description
        no parameters
     - Return Value Description
        Scanned device address

    2.AT+BT=READ_SCAN_ENABLE
     - Function Description
        Get current device visibility
     - CMD Format
        AT+BT=READ_SCAN_ENABLE
     - Parameter Description
        no parameters
     - Return Value Description
        .. csv-table::
            :header: "Value", "Parameter Description"
            :widths: 10, 50

            0x00,"No Scans enabled."
            0x01,"Inquiry Scan enabled. Page Scan disabled."
            0x02,"Inquiry Scan disabled. Page Scan enabled."
            0x03,"Inquiry Scan enabled. Page Scan enabled."

    3.AT+BT=WRITE_SCAN_ENABLE
     - Function Description
        Set current device visibility
     - CMD Format
        AT+BT=WRITE_SCAN_ENABLE,param1
     - Parameter Description
        .. csv-table::
            :header: "Param1", "Parameter Description"
            :widths: 10, 40

            0x00,"No Scans enabled."
            0x01,"Inquiry Scan enabled. Page Scan disabled."
            0x02,"Inquiry Scan disabled. Page Scan enabled."
            0x03,"Inquiry Scan enabled. Page Scan enabled."

    4.AT+BT=CONNECT
     - Function Description
        Connect to specified device
     - CMD Format
        AT+BT=CONNECT,param1,param2
     - Parameter Description
        .. csv-table::
            :header: "Param", "Parameter Description"
            :widths: 10, 40

            param1, "Bluetooth address"
            param2, "Allow_Role_Switch
             | 0x00 not accept role switch
             | 0x01 accept role switch"

    5.AT+BT=DISCONNECT
     - Function Description
        Disconnect the designated device
     - CMD Format
        AT+BT=DISCONNECT,param1
     - Parameter Description
        .. csv-table::
            :header: "Param", "Parameter Description"
            :widths: 10, 40

            param1, "Bluetooth address"

    6.AT+BT=SPP_INIT
     - Function Description
        SPP initialization, register to SDP
     - CMD Format
        AT+BT=SPP_INIT
     - Parameter Description
        no parameters

    7.AT+BT=SPP_CONNECT
     - Function Description
        SPP connect to specified device
     - CMD Format
        AT+BT=SPP_CONNECT,param1
     - Parameter Description
        .. csv-table::
            :header: "Param", "Parameter Description"
            :widths: 10, 40

            param1, "Bluetooth address"

    8.AT+BT=SPP_TX
     - Function Description
        SPP sends specified data
     - CMD Format
        AT+BT=SPP_TX,param1
     - Parameter Description
        .. csv-table::
            :header: "Param", "Parameter Description"
            :widths: 10, 40

            param1, "send data"
     - Test Case
        AT+BT=SPP_TX,beken-spp-tx_test!!!!

    9.AT+BT=SPP_THROUGH_TEST
     - Function Description
        SPP throughput test, the data is randomly generated
     - CMD Format
        AT+BT=SPP_THROUGH_TEST,param1
     - Parameter Description
        .. csv-table::
            :header: "Param", "Parameter Description"
            :widths: 10, 40

            param1, "send data length"
     - Return Value Description
        After the data of the specified length is sent, the client and server will output the CRC value calculated by sending the data and the transmission rate from the log.
     - Test Case
        AT+BT=SPP_THROUGH_TEST,0xfffff
