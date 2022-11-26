TRNG User Guide
========================================================

:link_to_translation:`zh_CN:[中文]`

Summary
--------------------------------------------------------------

TRNG The true random number generator is integrated in Beken chip and generates true random numbers through random noise, independent of other modules.。


TRNG Usage
------------------------------------------------------------------------------

 - TRNG start, call 'bk_trng_start()' to start
 - Get random number, call 'bk_rand()' to get

.. note::

  By default, the system has initialized the TRNG module, and applications can directly use 'bk_rand()'
