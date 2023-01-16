OS Abstraction Layer
=================================================================

:link_to_translation:`en:[English]`



OS Abstract Layer Introduction
--------------------------------------------------------------------

 - The OS abstraction layer is mainly used to adapt to different operating systems of the Armino platform
 - For different operating systems, the OS abstraction layer provides a unified set of interfaces
 - At present, the operating systems supported by the OS abstraction layer of the Armino platform include FreeRTOS, HarmonyOS, RTThread
 - At present, the posix interface of the Armino platform only supports the FreeRTOS V10.4 operating system, which is turned off by default. 
   If it is used, you need to turn on the 'CONFIG_FREERTOS_POSIX' configuration switch


OS API Details
---------------------------------------------------------

.. toctree::
    :maxdepth: 1

        OS API <../../api-reference/system/os>
        Posix API <../../api-reference/system/posix/index>



