TRNG 使用指南
=====================

:link_to_translation:`en:[English]`

概述
-----------------

TRNG 真随机数发生器，集成在 Beken 芯片内部，通过随机噪声产生真随机数，不依赖于其他模块。


TRNG 使用方法
------------------

 - TRNG使能，可调用 bk_trng_start() 使能
 - 获取随机数，调用 bk_rand() 获取

.. note::

  默认情况下系统已经初始化 TRNG 模块，应用可以直接使用 bk_rand()
