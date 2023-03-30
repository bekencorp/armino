Driver Development Specification
================================================================

API Specification
---------------------------------------------------

Header file
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Each peripheral driver has three API related header files：

 - ``bk_api_xxx.h`` Define the API of xxx peripherals.
 - ``bk_api_xxx_types.h`` Define the public data types and macros of xxx peripherals. External components and driver layers can be directly accessed, while HAL layer and SOC layer cannot be accessed.
 - ``xxx_types.h`` Define the public data types and macros of xxx peripherals. External components, driver layer, HAL layer and SOC layer can all be accessed.

The interface, data type and macro in the API header file should be stable, abstract and independent of the specific hardware.

.. note::

  ``xxx_types.h`` The internal data type should not be included, but should be placed in the internal header file:
  
   - If it is a private data type of the driver layer, it will be placed in ``xxx_driver.h``.
   - If it is a HAL layer private data type, put it in ``xxx_Hal.h``.
   - If it is a SOC layer private data type, it will be placed in ``xxx_ll.h``.


Device ID starts from 0
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

For multi-channel peripherals such as DMA/ADC/GPIO, the channel number or identification number starts from 0.

Error code
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Each driver should define its own error code, with the error code offset at bk_err.h.


Reentrant
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



The Armino driver API can be implemented as non reentrant by default. For some 
peripherals with parallel access requirements, for example, when two different 
tasks may compete to access the same ADC channel at the same time, the driver layer 
needs to use a lock to ensure reentrant. The drive API reentrancy needs to be 
described in the API documentation.


File
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Documentation requirements are as follows：

 - API documentation is required for all driver APIs
 - For complex driver APIs, API usage guidelines need to be added

Example
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

For using complex drivers, you need to add corresponding sample programs under ``project/examples/serial``.

CLI Commands
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Each driver API needs to add a set of CLI commands to test the driver.

Driver Layer Specification
------------------------------------------------------------------------------------------------------------------------------

Chip Independence
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

The driver layer code has nothing to do with the specific chip, and should not appear like ``CONFIG_SOC_XXX`` Wrapped code.
The specific chip related implementation should be placed in the HAL layer.


Register independent
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

The driver layer code should not directly operate the register, but must operate the register through the HAL/LL interface


Low Power Requirements
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Please fully consider the low power consumption design requirements of peripherals：

 - When the peripheral operation needs to wait for the hardware status, you need to vote to prevent the CPU from entering the 'low-voltage sleep mode' before the transaction starts, and then vote for it after the transaction ends.
 - When implementing the peripheral API, turn on the clock before the operation and turn off the clock after the operation.


Transmission Mode
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

The following peripheral read/write modes are recommended：

================ =========== =================================================================================
Mode             Write/Read     Describe
================ =========== =================================================================================
Poling           Write/Read   Do not use the interrupt mechanism. 

                              Wait for the return after the transmission is completed by using
                              the 'busy waiting' method..

receive cache    Write        Driver layer defines a receive cache with a configurable length.
                              The data received in the interrupt is stored in the receive cache.
                              The API fetches data from the receive cache, supports blocking and 
                              non blocking modes, and sets the blocking timeout.

0-copy           Write        The interrupt directly uses the data pointer passed in by the user to write.
                              The API supports blocking and non blocking modes, and the timeout
                              can be set for blocking modes.
                              When using the 'non blocking' mode, there is an independent 
                              API that can wait for the peripheral transmission to complete
================ =========== =================================================================================

Interface Integrity
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

When implementing specific drivers, it is recommended to refer to 
``CMS/FreeRTOS_The requirements for IO`` and typical operating system peripheral drivers during
adaptation ensure that the driver peripheral has achieved the most common functions, facilitating
subsequent drive adaptation.


Common API of driver layer
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

The Driver layer usually requires the implementation of the following standard APIs. 
When implementing the API, the possibility of future expansion should be fully considered, 
and the complexity of driver development should be minimized.

+------------------------+-----------------------------------------------------------------------+
| Driver API             |  Function description                                                 |
+========================+=======================================================================+
| ``bk_xxx_driver_init`` |  1. Driver framework related data structure initialization            |
|                        |  2. Drive the global configuration. For multi-channel devices,        |
|                        |                                                                       |
|                        |  the general configuration can be configured here                     |
+------------------------+-----------------------------------------------------------------------+
|``bk_xxx_driver_deinit``|  Uninstall the driver framework\free all software/hardware resources  |
+------------------------+-----------------------------------------------------------------------+
| ``bk_xxx_init``        |  1. Multi channel device, use this API to initialize specific channels|
|                        |  2. Initialize software resources                                     |
|                        |  3. Initialize the hardware, such as powering on the device, etc      |
|                        |  4. Initialize the configuration related to operation of the channel  |
+------------------------+-----------------------------------------------------------------------+
| ``bk_xxx_deinit``      |  1. Reset channel hardware configuration to default                   |
|                        |  2. Turn off channel hardware                                         |
|                        |  3. Turn off the channel hardware clock, power, etc                   |
|                        |  4. Release channel software resources                                |
+------------------------+-----------------------------------------------------------------------+
| ``bk_xxx_start``       |  Start the device. The device works normally after start              |
+------------------------+-----------------------------------------------------------------------+
| ``bk_xxx_stop``        |  Turn off the device, and the existing configuration                  |
+------------------------+-----------------------------------------------------------------------+

HAL/LL/SOC layer specification
----------------------------------------------------------------------------------------------------

Software/hardware register definition consistency
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

The name of the driver software register should be consistent with the name in the chip 
register definition. It is recommended to generate relevant codes through scripts:：

 - SoC Layer ``xxx_reg.h``/``xxx_struct.h`` Generated by script according to register definition
 - LL layer xxx_ll.h Generated by script according to register definition


Register Access
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Access register through structure
**************************************************************************

To ensure the readability of the code, the structure field defined in 
``xxx_struct.h`` should be used preferentially to access the register in 
the driver code.


Access register through macro
**************************************************************************

For some multi-channel bit operations, or some performance sensitive 
operations, you can choose to use register macro operations.

The most commonly used register bit operation macros are as follows.
It is not recommended to define another set of register access macros.
Please give priority to the following macros to access registers.。

+------------------------+-----------------------------------------------+
| Macro Name             |   Describe                                    |
+========================+===============================================+
|  REG_WRITE(r, b)       |  Write 32-bit register                        |
+------------------------+-----------------------------------------------+
|  REG_READ(r)           |  Read 32-bit register                         |
+------------------------+-----------------------------------------------+
|  REG_SET_BIT(r, b)     |  Write one or more bits. The mask is 0xffffff |
+------------------------+-----------------------------------------------+
|  REG_CLR_BIT(r, b)     |  Clear one or more bits, mask is 0xffffff     |
+------------------------+-----------------------------------------------+
|  REG_SET_BITS(r, b, m) |  Read one or more bits. The mask is 0xffffff  |
+------------------------+-----------------------------------------------+
|  REG_SET_FIELD(r, f, v)|  Write to specify multiple bits,mask is _S&_v |
+------------------------+-----------------------------------------------+
|  REG_GET_FIELD(r, f, v)|  Read to specify multiple bits,mask is _S&_v  |
+------------------------+-----------------------------------------------+
