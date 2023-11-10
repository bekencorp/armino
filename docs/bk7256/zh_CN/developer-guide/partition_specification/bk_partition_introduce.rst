物理地址与逻辑地址的关系介绍
================================================

:link_to_translation:`en:[English]`

.. important::
  - 物理地址：flash视角，每32字节加了2字节crc；
  - 逻辑地址：cpu视角地址，已去掉crc;

为确保存储在flash中数据的准确性，每32个字节会增加2个字节crc校验位（crc检验由硬件自动完成），从而构成的物理地址（flash视角）会比逻辑地址（cpu视角）多2个字节，对比关系见下图1：（左侧为cpu视角，右侧为flash视角，红色字体为crc校验）

.. figure:: ../../../_static/logic.png
    :align: center
    :alt: logic
    :figclass: align-center

    图1物理地址与逻辑地址的对比

