UART 使用指南
==================

:link_to_translation:`en:[English]`

概述
------------------

UART (Universal Asynchronous Receiver and Transmitter) 通用异步接收器/发送器，用来实现不同设备之间的全双工或半双工数据交换。

.. figure:: ../../../_static/uart.png
    :align: center
    :alt: UART Connection
    :figclass: align-center

    UART Connection


上图为UART串口TTL电平硬件连接，其中：
 
 - TXD：用于发送数据，应连接到接收设备的RXD 引脚上；
 - RXD：用于接收数据，应连接到发送设备的TXD 引脚上；
 - GND：为双方提供一个相同的参考电平。

Beken 芯片通常具有 2 个 UART (UART1和UART2)，BK7271支持 3 个 UART (UART1、UART2和UART3)，其中 UART2 用于打印调试信息(在menuconfig中可配置，默认UART2)。

UART配置
-------------------


为了使用 UART 进行数据传输，通常需要配置下述参数：
 
 - 波特率：每秒传输的bit数，一般有9600, 19200, 115200等选项
 - 数据位：可以有5, 6, 7或8位的数据，一般我们是按字节（8位）传输数据，发送方一位一位的改变数据线上的状态（高电平或低电平）将它们发送出去，传输数据时先传最低位，最后传送最高位
 - 校验位：用来校验数据传送的正确性，只能检错，不能纠错
 - 停止位：用来表示当前数据传输完毕，为逻辑电平"1"

UART时序
------------------

.. figure:: ../../../_static/uart_data.png
    :align: center
    :alt: UART Timing
    :figclass: align-center

    UART Timing


上图为 8位数据位+1位偶校验位+1位停止位 发送'A'(0x41) 的波形
'A'的二进制是01000001，小端传输，即低位（LSB）在前，高位（MSB）在后
 
 - 空闲时，数据线上为高电平
 - 起始位，当需要发送数据时，UART将改变TXD的状态，变为低电平，通常为1bit
 - 数据位，发送方一位一位的改变数据线上的状态（高电平或低电平）将它们发送出去，先传bit[0]，其值为1；再发送bit[1]，其值为0，以此类推
 - 校验位，偶校验：数据位 + 校验位 值为“1”的bit数为偶数，因此校验位为低电平
 - 停止位，因配置停止位为1bit，因此停止位为1bit高电平

UART 发包通路
------------------

发包通路如下：

 - 应用程序可调用 bk_uart_write_bytes() 进行发包
 - UART 硬件发包

.. note::

  当应用程序发包速率超过 UART 硬件发送速率时，bk_uart_write_bytes() 会等进行忙等待，这可能对 CPU 性能有一定影响。但在实际应用中，这通常不是问题。

UART 收包通路
------------------

.. figure:: ../../../_static/uart_recv.png
    :align: center
    :alt: UART Receive Data
    :figclass: align-center

    UART Receive Data

UART 收包通路如上图所示：

 - UART 硬件收包：
   UART 硬件将包收到硬件 FIFO 中，当包的个数达到 RX Full Threshold 时，上报 UART RX 中断；当硬件在一段时间没有收到包时，上报 UART RX Finish 中断。
   RX Full Threshold 可通过 bk_uart_set_rx_full_threshold() 来配置； 硬件判断 RX Finish 的时间长度可通过 bk_uart_set_rx_timeout() 配置。
 - UART 中断收包：
   当 UART RX 或者 UART RX Finish 中断产生时，软件进入 UART 中断处理函数。 UART 中断从硬件 FIFO 中读取数据，并放入 RX FIFO。如果此时有应用程序在等
   待数据，则唤醒应用程序进行收包。
   UART RX FIFO 的大小可通过 menuconfig 来配置。
 - UART 应用收包：
   当应用调用 bk_uart_read_bytes() 时，会从 UART RX FIFO 中读取数据，当 UART RX FIFO 中没有数据时，如果 timeout 设置为 0，则 bk_uart_read_bytes()
   立即返回，否则会等待，直到超时或者 UART RX FIFO 中收到新数据。
   
UART 收包流控
**********************

当 UART 中断从 UART HW FIFO 中取包速率小于 UART 硬件向 HW FIFO 中填包速率时，UART HW FIFO 会 Full。
如果 UART 硬件流控使能，则 UART 硬件会通知发送方停止发包，否则会出现丢包。

同理，当 UART 应用从 SW FIFO 中取包速率小于 UART 中断向 SW FIFO 中填包速率时，UART SW FIFO 会 Full。
当 SW FIFO Full 时，中断会读取硬件 FIFO 中的数据并丢弃，此时会打出 “rx kfifo is full” 的log，当这种情况出现时可能会影响
应用程序功能，通常解决方案是提高应用程序处理速率，如提升应用任务的优先级等。

.. note::

  解决 SW FIFO Full 的另一个方案是中断停止从 HW FIFO 中取包，从而触发硬件收包流控，最终实现收包降速。ARMINO 未来可能会支持这种机制。
  
UART 应用场景
------------------

目前 UART 支持三种不同的应用方式：

 - 使用默认 UART 中断处理流程：
   使用默认的 bk_uart_write_bytes()/bk_uart_read_bytes 来处理 UART 收发包，收发包通路如 UART 收包通路一节所述，通常这种方式能满足大部分应用的需求。
 - 使用默认 UART 中断处理流程，同时注册用户 callback：
   同第一方式，唯一不同的点是，在产生 UART 中断之后，会调用用户注册的 callback。
 - 完全由应用自己实现 UART 中断处理：
   当默认的 UART 中断处理流程无法满足应用需求时，如应用的 UART 由第三方代码实现时，可以通过 bk_interrupt_register(xx, isr, arg) 替换默认的 UART 中断处理程序。
   此时收发包过程完全由应用实现。


