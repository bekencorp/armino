Interruption Management
=====================================================

:link_to_translation:`zh_CN:[中文]`


Summary
-------------------------------------------------------

.. figure:: ../../../_static/interrupt.png
    :align: center
    :alt: Interrupt Overview
    :figclass: align-center

    Interrupt Overview


As shown in the figure above, the core module of interrupt management is the ICU (Interrupt Control Unit). Peripherals report interrupts to CPU through ICU.

The whole interrupt processing framework can be simply divided into three levels:

 - Equipment channel level

   Different channels of the device can independently generate interrupts, and a set of APIs are used to control device channel interrupts：

     - bk_dev_enable_interrupt() Turn on device specific channel interrupt
     - bk_dev_disable_interrupt() Turn off device specific channel interrupt
     - bk_dev_register_isr() Register device specific channel interrupt handler

   Generally, the device channel API is implemented by the specific device. Please refer to the API documentation of the specific device.

 - Equipment Level

   API for configuring interrupt sources：

    - bk_interrupt_register_isr() Register interrupt source handler
    - bk_interrupt_set_group() Set interrupt source group priority
    - bk_interrupt_set_priority() Set interrupt source priority

 - CPU

   'arch_register_group_isr()' is used to register interrupt group handlers. Usually, this level of interrupt processing is ignored.

Interrupt processing flow can be summarized as follows：

 - Interrupt processing flow can be summarized as follows
 - CPU/ICU calls interrupt group handler according to interrupt group priority
 - The interrupt group handler handles interrupts and calls the interrupt source handler
 - The interrupt source handler handles the interrupt and calls the device channel handler


Interrupt source
----------------------------------------------------

In ARMINO, peripherals that can generate interrupts are called interrupt sources. Generally, a peripheral is an interrupt source, corresponding to an interrupt source macro 'INT_SRC_DEV'. DEV in 'INT_SRC_DEV' represents the name of the peripheral, for example, GPIO interrupt source macro is 'INT_SRC_GPIO', SDIO interrupt source is 'INT_SRC_SDIO'.
For all peripheral interrupt sources supported by SoC, refer to definition 'icu_int_src_t' in 'int_types.h'.


.. note::

  The interrupt source corresponds to a peripheral, not a specific channel of the peripheral.


Interrupt Priority
--------------------------------------------------------

When two different devices are interrupted at the same time, the hardware decides which interrupt to report first according to the priority of the interrupt source, and the software decides which interrupt to handle first according to the priority of the interrupt source.


ARMINO Provide two-level interrupt priority configuration：

 - Group Priority

   Software/hardware prioritizes interrupts with higher group priority. In a specific implementation, the group priority is usually mapped to the CPU hardware priority. For example, the FIQ/IRQ interrupt of ARM9 is mapped to two different group priorities. The group priority can be configured through 'bk_interrupt_set_priority()'.

 - Same Group Priority

   The same group priority is also referred to as priority. When the group priorities are the same, the software/hardware will give priority to handling interrupts with higher priority in the group. In ARMINO ARM9 SoC, intra group priority is realized through software. It should be noted that when two peripherals with the same priority in the group generate interrupts at the same time, ARMINO does not guarantee the interrupt processing sequence of the two devices. The processing sequence is determined by the sequence of the two interrupt registration.



.. note::

  In ARMINO ARM9 SoC, only intra group priority configuration is currently supported, and group priority configuration is not supported temporarily. The group priority of each interrupt source is fixed.

.. note::

  Different channels of the same peripheral have the same priority. ARMINO cannot set different interrupt priorities for different channels of the same peripheral. Because the priority is configured on the interrupt source, and the interrupt source corresponds to a peripheral rather than a peripheral channel.


Interrupt Handler
--------------------------------------------------------

ARMINO Three levels of interrupt handling routine registration are provided：

 - Peripheral channel interrupt handler registration


   'bk_dev_register_isr()' is used to register the interrupt handler for a specific channel of peripherals. In 'bk_dev_register_isr()', dev represents the name of the peripherals. For example, 'bk_gpio_register_isr(GPIO_0, ...)' represents the interrupt handler for registering 'GPIO_0'. When 'GPIO_0' generates an interrupt, the registered interrupt handler will be called.
   

 - Peripheral interrupt handler registration

   'bk_dev_register_isr()' is used to register the interrupt handler for a specific channel of peripherals. In 'bk_dev_register_isr()', dev represents the name of the peripherals. For example, 'bk_gpio_register_isr(GPIO_0, ...)' represents the interrupt handler for registering 'GPIO_0'. When 'GPIO_0' generates an interrupt, the registered interrupt handler will be called.


   Generally, the ARMINO peripheral initialization function calls 'bk_interrupt_register_isr()' to register a default interrupt source handler, which generally performs the following processing:

     - Get peripheral interrupt status register
     - Clear peripheral interrupt status register
     - Processing peripheral interrupt
     - XXX calls the peripheral specific channel handler XXX registered in 'bk_dev_register_isr()'

   Generally speaking, the application does not need to register its own interrupt source handler, and only needs to register when the default interrupt source handler cannot meet the requirements. It should be noted that when XXX's default interrupt source handler is replaced, 'bk_dev_register_isr()''s registered channel interrupt handler will not be available. At this time, the interrupt processing of the device is completely implemented by the application XXX.

   

 - Interrupt Group Handler Registration

   'arch_register_group_isr()' is used to register the handler of a specific interrupt priority group. When any device in the group generates an interrupt, this handler will call. Generally, this handler will call the interrupt source handler registered by 'bk_interrupt_register_isr()' according to the priority within the group. Generally, the application does not need to pay attention to this level of interrupt registration.


Write interrupt application
-------------------------------------------------------------

The application of peripheral interrupt can be divided into general application and special application.

General Applications
************************************************************

Most applications use peripheral interrupt in the following ways：

 - bk_dev_register_isr() Register Device Channel Interrupt Handler
 - bk_dev_enable_interrupt() Turn on channel interrupt
 - bk_dev_disable_interrupt() Turn off channel interrupt


Special Applications
************************************************************

For applications that need to completely control the device interrupt, the interrupt mode is as follows：

 - bk_interrupt_register_isr() Register Device Channel Interrupt Handler
 - bk_dev_enable_interrupt() Turn on channel interrupt
 - bk_dev_disable_interrupt() Turn off channel interrupt

.. note::


   When 'bk_interrupt_register_isr()' replaces the default interrupt source handler, 'bk_dev_register_isr()' is unavailable, but 'bk_dev_enable_interrupt()' and 'bk_dev_disable_interrupt()' are still available.





