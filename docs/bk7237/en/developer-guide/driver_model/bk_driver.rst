Drive Tiering
=================================================================================================

:link_to_translation:`zh_CN:[中文]`

.. note::

  The driver layered model is applicable to all peripherals. This section takes UART as an example to explain.

Example
------------------------------------------------------------------------------------------------

The following figure lists the files related to UART driver implementation.

::

  - armino
      - middleware
          - driver
              - include
                  - uart.h
              - uart
                  - uart_driver.c
                  - uart_driver.h
                  - uart_statis.c
                  - uart_statis.h
          - arch
              - bk7237
                  - soc
                      - uart_cap.h
                      - uart_reg.h
                      - uart_struct.h
                  - hal
                      - uart_ll.c
                      - uart_ll.h
              - common
                  - hal
                      - include
                          - uart_hal.h
                          - uart_types.h
                      - uart_hal.c
                  - soc
                      - include
                          - uart_hw.h

Tiering Overview
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

UART (Drive is divided into drive layer), HAL (Hardware abstraction layer), LL (Register interface layer) and SOC (Register definition layer):

+-----------+---------------------------------------------------------------+
| Layer     |  Function Overview                                            |
+===========+===============================================================+
| drive     | 1. Expose APIs, which are stable and backward compatible      |
|           | 2. The hardware can only be accessed through this layer's API |
|           | 3. Depends on HAL, usually a subset of HAL functions          |
|           | 4. Internal implementations are usually platform independent  |
|           | 5. Can rely on OS and other components of Beken SDK           |
+-----------+---------------------------------------------------------------+
| HAL       | 1. Abstraction of hardware, providing a set of stable and     |
|           |                                                               |
|           |  comprehensive API interfaces                                 |
|           |                                                               |
|           | 2. Generally, it has better performance and takes less        |
|           |                                                               |
|           |  resources. When transplanting Beken chip operation to other  |
|           |  software platforms, you can consider only transplanting the  |
|           |  HAL layer.                                                   |
|           |                                                               |
|           | 3. HAL interfaces on different boards are usually consistent  |
|           | 4. The HAL layer interface does not promise stability, and    |
|           |                                                               |
|           |  the code based on HAL layer migration may face compatibility |
|           |  problems after HAL upgrade                                   |
|           |                                                               |
|           | 5. Only rely on LL/SOC and print relevant codes               |
|           | 6. No direct register operation is involved                   |
|           |                                                               |
+-----------+---------------------------------------------------------------+
| LL        | 1. It is usually a simple wrapper for register access to make |
|           |                                                               |
|           |  the program more readable                                    |
|           |                                                               |
|           | 2. Usually implemented as inline function                     |
|           | 3. The implementation is as simple as possible                |
|           |                                                               |
+-----------+---------------------------------------------------------------+
| SoC       | Provide register definition                                   |
+-----------+---------------------------------------------------------------+

Document description
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

============ ====================== ============================================================================
Layer        Header file name       Function Overview                                                     
============ ====================== ============================================================================
Driver       uart_types.h           UART API Type Definition

             uart.h                 UART API statement

             uart_driver.c          UART Drive implementation

             uart_driver.h          UART Drive private data types and macro definitions

             uart_statis.c          UART Driver statistics related information

             uart_statis.h          UART Similar definitions related to driver statistics

HAL          uart_hal.h             Hardware abstraction layer API statement, shared by different boards

             uart_hal.c             UART Hardware Abstraction Layer Interface Implementation

             uart_ll.h              Packaging register access interface, making the deposit code more readable

SoC          uart_reg.h             Register macro definition, board dependent

             uart_struct.h          Define register in the form of C structure

             uart_cap.h             Hardware capability parameters 
============ ====================== ============================================================================
