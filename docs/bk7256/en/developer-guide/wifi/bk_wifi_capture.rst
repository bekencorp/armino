Wi-Fi Capture Introduction
==================================================

:link_to_translation:`zh_CN:[中文]`

Powerful Network Protocol Analyzer-OmniPeek
-----------------------------------------------------
Meet Omnipeek
+++++++++++++++++++++++++++++++++++++++++++++++++++
Known as the world's most powerful network protocol analyzer, omnipeek is a product of Savvius(formerly known as WildPackets), a worl leader in packet capture and analysis, with 25 years of leadership in packet capture, deep packet detection and network diagnosis solutions. It has been acquired by LiveAction. LiveAction previously focused on network data flow level analysis. With the acquisition of savvius, LiveAction and Savvius are integrated and complementary to achieve a comprehensive analysis platform from streams to packets. Omnipeek can analyze data packets in depth, analyze thousands of protocols, analyze network traffic from multiple dimensions, provide dozens of intuitive chart reports, and provide expert level network analysis and diagnosis.

OmniPeek Panel
+++++++++++++++++++++++++++++++++++++++++++++++++++
.. image:: ../../../_static/omni_panel.png

Omnipeek provides multiple dimensions of analysis methods and chart to use. The figure above shows the Compass panel, an interactive forensic dashboard that displays network utilization over time, including events、protocols、flows、nodes、channels、VLAN、data rates、applications and national statistics. These statistics are displayed in the optional data source widget and can be viewed from live capture or a single capture file.

OmniPeek Application Analysis
+++++++++++++++++++++++++++++++++++++++++++++++++++
"Application" dashboad displays key statistics for the application in the capture window. The application visibility provides insight into user behavior and traffic patterns on the network at specific times of the day, week, month or year. It can help analysts better understand who will visit which sites and which applications when.

OmniPeek Statistical Analysis
+++++++++++++++++++++++++++++++++++++++++++++++++++
.. image:: ../../../_static/omni_graphs.png

Omnipeek and the capture engine compute various key statistics in real time and present these in an intuitive graphical display. You can save、copy、print or automatically genertate periodic reports on these statistics in variety of formats.

Summary
+++++++++++++++++++++++++++++++++++++++++++++++++++
Omnipeek is a powerful network analysis tool that can be easily used by beginners. However, due to its rich features, the above brief introduction can't be fully understood. For more details, please visit the official website link.
`Onmipeek <https://www.liveaction.com/>`_

OmniPeek Drivers
---------------------------------------------------
According to the driver information in the software installation directory ``Omnipeek\Drivers``, Omnipeek can support two types of capture sniffer at present, one based on Atheros chip ``Atheros USB 802.11n Wireless LAN card``, it can only support 11N mode; another one based on ``Ralink rt2870`` , it can support 802.11bgn/ac. It is recommended to use  ``ASUS USB-AC55`` or ``Netgear A6210`` .


Capture Scenario
---------------------------------------------------
- Wi-Fi Connection, analyze key frames with packet capture
- Wi-Fi Performance, analyze the retransmission, aggregation degree, phy rate, etc
- Wi-Fi Power consumption, analyze the value of power save mode

FAQ
---------------------------------------------------
OmniPeek Capture Example
+++++++++++++++++++++++++++++++++++++++++++++++++++
- Scan

.. image:: ../../../_static/scan.png

- Authentication

.. image:: ../../../_static/auth.png

- Four handshake

.. image:: ../../../_static/eapol.png

- Decryption

If user can catch all above complete authentication process, you can enter SSID and password for decryption through ``Tools-Decrypt WLAN Packets`` .

.. image:: ../../../_static/decrypt.png



