Software Architecture
-------------------------------------

:link_to_translation:`zh_CN:[中文]`


Summary
******************

	Armino software is divided into middleware layer, component layer and application layer.
	Armino Armino abstracts the coupling dependence of software on hardware platform and realizes 
	the shelving of software platform. Through this shelf design and rich SDK APIs, SDK developers
	can design and develop more efficiently.



.. figure:: ../../../_static/armino_architecture.png
    :align: center
    :alt: Architecture Overview
    :figclass: align-center

    Architecture Overview

Software Architecture
************************************

	Armino SDK can flexibly support different product software architectures.
	
	1、 Based on Armino SDK's own software architecture
	See the legend of Architecture Overview for the overall framework of the solution.
	In this solution, the operating system has been decoupled and adapted.
	The current Armino solution supports FreeRTOS and LiteOS.


	2、 Based on the developer's own software architecture
	Armino SDKThe Armino SDK is compatible with a variety of software ecosystems,
	facilitating developers to integrate the Armino chip solution into their own 
	mature product software architecture.


.. figure:: ../../../_static/armino_os_solution.png
    :align: center
    :alt: Armino OS solution
    :figclass: align-center

    Armino OS solution

.. figure:: ../../../_static/armino_custiomer_solution.png
    :align: center
    :alt: Armino customer solution
    :figclass: align-center

    Armino customer solution

Hardware Layer
************************************

    Hardware layer, namely hardware layer, refers specifically to the hardware of BEKEN solution.。


Middleware Layer
************************************

    Middleware layer, namely the middle layer, is an abstract layer for the underlying hardware and 
	upper components. Through the M-server, we can seamlessly switch between different solutions.

.. figure:: ../../../_static/armino_middleware.png
    :align: center
    :alt: Armino middleware
    :figclass: align-center

    Armino middleware

As shown in the figure:：
 - arch, It includes related functions of chip architecture, such as arm, riscv, general interrupt processing, boot, pmp, etc.
 - soc, Chip related special processing and related configuration.
 - dirver, Driver layer, including various peripheral drivers, such as I2C, GPIP, UART, etc.
 - compal, Components abstraction layer. In order to enable third-party open source components to function on the armono platform while reducing code coupling.
 - boards, Board level configuration is related, covering some engineering configurations of specific projects.
 
Component Layer
************************************

    The Components layer, or component layer, is a service component that contains many third-party open source or self developed by armono. Developers can configure defconfig in the middleware layer to switch related component services.
	
Application Layer
************************************

    The application layer is organized under the projects directory, and the last level of the CmakeList directory is the building entry for a complete solution. It carries the differentiation of different projects and the configuration set of different product solutions.