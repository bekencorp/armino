Wi-Fi iPerf示例说明
=============================================
本工程展示了使用iPerf测试Wi-Fi吞吐率的使用示例：

- Wi-Fi STA模式测试iPerf
- Wi-Fi AP模式测试iPerf
- iPerf上下行吞吐率测试及参数配置

工程示例及配置说明
-----------------------------------------------
IPERF的配置文件在"components/demos/net/iperf/iperf.c",支持四种模式：
TCP_SERVER、TCP_CLIENT、UDP_SERVER、UDP_CLIENT

- IPERF_REPORT_INTERVAL 吞吐测试上报间隔,一般设置为1
- IPERF_MAX_TX_RETRY    最大重传次数,默认值为10
- THREAD_PROIRITY       系统优先级,默认设置为4

BEKEN iPerf使用介绍
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

iPerf Example
+++++++++++++++++++++++++++++++++++++++++++++++++++++
+---------------------------------------------------+
| Client Mode(上行)                                 |
+------------------+--------------------------------+
| TCP Client Mode  | iperf -c IP                    |
+------------------+--------------------------------+
| UDP Client Mode  | iperf -c IP -u                 |
+------------------+--------------------------------+
| Server Mode(下行)                                 |
+------------------+--------------------------------+
| TCP Server Mode  | iperf -s                       |
+------------------+--------------------------------+
| UDP Server Mode  | iperf -s -u                    |
+------------------+--------------------------------+

常见问题
-------------------------------------------------
iPerf工具介绍
+++++++++++++++++++++++++++++++++++++++++++++++++
- iPerf测试工具下载: `点击下载 <https://iperf.fr/>`_ 进行下载,使用说明可以参考iPerf user docs.
.. image:: ../../../_static/iperf.png
- 推荐使用iPerf2.0版本.

影响iPerf测试吞吐率的因素
++++++++++++++++++++++++++++++++
- DUT Wi-Fi是否进行RF参数校准
- DUT天线是否进行调试,OTA测试、EVM测试结果是否正常
- 测试环境干扰,尽量选择屏蔽箱或者屏蔽房内进行验证
- 测试环境确认:DUT、AP的距离,使用外置天线还是内置天线等
- SDK运行过程中是否有足够的内存
- 特殊AP的兼容性问题

:link_to_translation:`en:[English]`

