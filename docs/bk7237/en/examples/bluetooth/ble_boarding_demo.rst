ble_boarding
========================

:link_to_translation:`zh_CN:[Chinese]`

Overview
--------------------------
Ble boarding is the process of adding unconfigured devices to the network through the Bluetooth function to make them become nodes in the network. This function mainly defines the use of the Bluetooth GATT Service to realize the SSID and Password required for the WIFI connection during the network configuration process, so that the unconfigured device can obtain the information of the connected network, and then can connect to WIFI.

This section will introduce the use of Bluetooth distribution network through the ble boarding demo.

Demo brief introduction
--------------------------
1. The main design ideas of ble boarding demo are as follows:

 1) Bluetooth device sends out ble_boarding broadcase.
 2) The mobile phone APK searches for bluetooth broadcase to find device and makes bluetooth connection.
 3) The moblie phone APK sends the WIFI SSID and PASSWORD to the device to be connected through the GATT Service.
 4) After the Bluetooth device obtains the SSID and PASSWORD required for the WIFI connection, the WIFI connection is made, and the ble boarding is successful.
 
The design flow chart of the demo project is shown in the following figure:

.. figure:: ../../../_static/ble_boarding_demo_work_flow.png
    :align: center
    :alt: ble boarding demo work flow chart
    :figclass: align-center

    Figure 1. ble boarding demo work flow chart

2. Software tools required for the bluetooth demo are nRF Connect APK, SSCOM and char to hex tool.
	
3. GATT Service:

 1) Service UUID: 0xFFFF 
 2) SSID characteristic UUID: 0x9ABC 
 3) PASSWORD characteristic UUID: 0xDEF0 

Demo example
--------------------------
 - For details of the API interface used by the ble boarding demo, please refer to ``/api-reference/bluetooth/ble.html``

 - For details of the example code of the ble boarding demo, please refer to ``demo/bluetooth/ble_boarding/ble_boarding_demo.c``

Demo workflow
--------------------------
	
.. figure:: ../../../_static/ble_boarding_demo_software_design_flow.png
    :align: center
    :alt: ble boarding demo software design flow chart
    :figclass: align-center

    Figure 2. ble boarding demo software design flow chart


Demo instruction
--------------------------
 1) Send ``AT+BLE=BOARDING,0201060908373235365f424c45,D`` command through SSCOM software.
 2) Open the nRF Connect APP on the mobile phone to scan, find the device named 7256_BLE and click the connect button.
 3) After the device is connected, find the service whose service UUID is 0xFFFF and click it.
 4) Find the characteristic whose UUID is 0x9ABC under the service, write the hex data of the WIFI SSID required for the ble boarding and click send.
 5) Find the characteristic whose UUID is 0xDEF0 under the service, write the hex data of the WIFI PASSWORD required for the ble boarding and click send.
 6) In the SSCOM window, a log showing that the wifi connection is successful and the IP is obtained will be output.
 7) Input the ``PING 192.xxx.xxx.xxx`` command in SSCOM to ping the IP address of the connected router to check whether the wifi connection is successful.

Attention
--------------------------
In the demo, the WIFI SSID and PASSWORD required to send through the nRF Connect software need to be converted int hex data through the char to hex tool for input.
	
