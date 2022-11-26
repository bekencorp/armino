I2C User Guide
==============================================================================

:link_to_translation:`zh_CN:[中文]`

Summary
--------------------------------------------------------------

I2C is a serial synchronous half duplex communication protocol. Multiple hosts and slaves can be attached to the bus at the same time. 
I2C bus is composed of serial data line (SDA) and serial clock line (SCL). These wires need pull-up resistance.

I2C has the advantages of simplicity and low manufacturing cost, and is mainly used for short distance communication of low-speed peripheral devices (within one foot)

.. figure:: ../../../_static/i2c_connection.png
    :align: center
    :alt: I2C Connection
    :figclass: align-center

    I2C Connection

The above figure shows I2C hardware connection, in which：
 
 - SCL：Clock signal, which provides the clock for data transmission to synchronize data transmission
 - SDA：Used to transfer data.

Beken The chip usually has two I2C controllers, and BK7271 supports three I2Cs (I2C0, I2C1, and I2C2).


Use of I2C
---------------------------------------------------

Beken I2C It can work in both master mode and slave mode. Whether the host or slave, you need to call 'bk_i2c_init()' to configure I2C before reading and writing.
bk_i2c_init() It mainly configures the clock frequency of I2C SCL. If it works in slave mode, it also needs to configure the device address of Beken I2C.

.. note::

  For bk7256/bk7231n/bk7251, I2C_ID_1 (corresponding to I2C2 on address_mapping) shares GPIO with UART2, so when I2C_ID_1 is enabled, UART2 cannot be used.


Communication in I2C host mode
*********************************************************************

Call 'bk_i2c_master_write()' to write data to the slave device and 'bk_i2c_master_read()' to read data.
For memory devices such as eeprom, Beken provides an A/B interface that can be assigned to the internal 
address of the slave device to perform read and write operations.For memory devices such as eeprom, 
Beken provides a 'bk_i2c_memory_write()/bk_i2c_memory_read()' interface that can be assigned to the 
internal address of the slave device to perform read and write operations.

.. figure:: ../../../_static/i2c_memory_write.png
    :align: center
    :alt: I2C memory write
    :figclass: align-center

    I2C memory write
	
.. figure:: ../../../_static/i2c_memory_read.png
    :align: center
    :alt: I2C memory read
    :figclass: align-center

    I2C memory read


I2C communication in slave mode
*********************************************************

For slave mode, Beken provides 'bk_i2c_slave_write()/bk_i2c_slave_read()' for sending/receiving data.