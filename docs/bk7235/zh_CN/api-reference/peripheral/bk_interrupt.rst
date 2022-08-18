Interrupt Control Unit
===========================

:link_to_translation:`en:[English]`

Beken chip supports interrupt service function registe for user.


Interrupt API Status
----------------------

+--------------------------------------------+---------+---------+--------+
| API                                        | BK7235A | BK7235B | BK7235 |
+============================================+=========+=========+========+
| :cpp:func:`bk_int_isr_register`            | Y       | Y       | Y      |
+--------------------------------------------+---------+---------+--------+
| :cpp:func:`bk_int_isr_unregister`          | Y       | Y       | Y      |
+--------------------------------------------+---------+---------+--------+
| :cpp:func:`bk_int_set_priority`            | Y       | Y       | Y      |
+--------------------------------------------+---------+---------+--------+
| :cpp:func:`bk_int_set_group`               | Y       | Y       | Y      |
+--------------------------------------------+---------+---------+--------+
| :cpp:func:`bk_get_int_statis`              | Y       | Y       | Y      |
+--------------------------------------------+---------+---------+--------+
| :cpp:func:`bk_dump_int_statis`             | Y       | Y       | Y      |
+--------------------------------------------+---------+---------+--------+

Interrupt API Reference
--------------------------

.. include:: ../../_build/inc/int.inc

Interrupt API Typedefs
------------------------

.. include:: ../../_build/inc/int_types.inc


