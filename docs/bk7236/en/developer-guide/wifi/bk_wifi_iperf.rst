Wi-Fi iPerf Application Example
=============================================
This section describes the usage of iPerf based on BEKEN chip. It contains three chapters:

- iPerf Test Example and Configuration
- BEKEN iPerf Usage
- FAQ

Example and Configuration
-----------------------------------------------
The configuration file of iPerf located in ``components/demos/net/iperf/iperf.c``, it supports four modes:
TCP_SERVER、TCP_CLIENT、UDP_SERVER、UDP_CLIENT

- IPERF_REPORT_INTERVAL-----------------Report interval, default 1s
- IPERF_MAX_TX_RETRY--------------------Max retry times, default 10
- THREAD_PROIRITY-----------------------Priority of system thread, default 4

BEKEN iPerf Usage
-----------------------------------------------
iPerf Usage
+++++++++++++++++++++++++++++++++++++++++++++++
::

    Usage: iperf [-s|-c host] [options]
           iperf [-h|--stop]

    Client/Server:
    -p           server port to listen on/connect to

    Server specific:
    -s           run in server mode

    Client specific:
    -c <host>    run in client mode, connecting to <host>

    Miscellaneous:
    -u           udp support, and the default mode is tcp
    -h           print this message and quit
    --stop       stop iperf program

iPerf Cmd Example
+++++++++++++++++++++++++++++++++++++++++++++++++++++
+---------------------------------------------------+
| Client Mode(Upstream)                             |
+------------------+--------------------------------+
| TCP Client Mode  | iperf -c IP                    |
+------------------+--------------------------------+
| UDP Client Mode  | iperf -c IP -u                 |
+------------------+--------------------------------+
| Server Mode(Downstream)                           |
+------------------+--------------------------------+
| TCP Server Mode  | iperf -s                       |
+------------------+--------------------------------+
| UDP Server Mode  | iperf -s -u                    |
+------------------+--------------------------------+

iPerf Running Log
+++++++++++++++++++++++++++++++++++++++++++++++++++++
::

    iperf -c 192.168.40.69              //TCP Client TX
    create iperf_tcp_c, tcb=30024cf0, stack=[30032170-30033170:4096], prio=5
    iperf: connect to iperf server successful!
    [0-1] sec bandwidth: 20021 Kbits/sec.
    [1-2] sec bandwidth: 21757 Kbits/sec.

    iperf -s -i1                        //TCP Server RX
    create iperf_tcp_s, tcb=30024d20, stack=[30031ce0-30032ce0:4096], prio=5
    iperf: new client connected from (192.168.40.69, 65070)
    [0-1] sec bandwidth: 11567 Kbits/sec.
    [1-2] sec bandwidth: 15658 Kbits/sec.

    iperf -c 192.168.40.69 -u           //UDP Client TX
    create iperf_udp_c, tcb=30024cf0, stack=[30031ce0-30032ce0:4096], prio=5
    iperf udp mode run...
    [0-1] sec bandwidth: 28141 Kbits/sec.
    [1-2] sec bandwidth: 35268 Kbits/sec.

    iperf -s -u -i1                     //UDP Server RX
    create iperf_udp_s, tcb=30024cf0, stack=[30031ce0-30032ce0:4096], prio=5
    [0-1] sec bandwidth: 17463 Kbits/sec.
    [1-2] sec bandwidth: 18910 Kbits/sec.

FAQ
-------------------------------------------------
iPerf Tool Introduction
+++++++++++++++++++++++++++++++++++++++++++++++++
- iPerf Tool Download: `Download <https://iperf.fr/>`_ . ``iPerf user docs`` could find the presentation

.. image:: ../../../_static/iperf.png

- It is recommended to use iPerf 2.0.x version

Factors about iPerf Throughput Performance
++++++++++++++++++++++++++++++++++++++++++++++++
- DUT Wi-Fi RF Performance, make sure OTA and EVM test pass
- Test Environment interference: it is best to test under the RF shield room
- Test environment: Reasonable distance between AP and DUT, an external antenna is better than embedded one
- Make sure to remain enough memory for iPerf running
- Compatibility with some particular routers


