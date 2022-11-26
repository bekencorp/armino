use case writing
================

:link_to_translation:`en:[English]`

ARMINO use cases are stored in the $ARMINO_PATH/examples directory. The use cases are used for:

 - Show the usage of ARMINO API, such as WiFi API, etc.
 - Usage of ARMINO key modules, such as Kconfig configuration, Backtrace viewing, compilation, etc.

Each use case can be compiled and tested independently.

When to write use cases?
-------------------------

The following situations require additional use cases:

 - When a single API is more complex, such as WiFi configuration bk_wifi_sta_set_config(), etc.
 - When using a feature that involves a set of related APIs
 - Error-prone API for ARMINO users
 - Features frequently used by ARMINO users, such as Debug, Compile, CLI, etc.
 - When an ARMINO user expressly requests

.. note::

    When a use case is to illustrate the ARMINO API or functionality, it should be placed in $ARMINO_PATH/examples and not elsewhere.

use case writing specification
------------------------------

The use case writing specification is as follows:

 - A use case describes only one function, or a set of strongly related APIs.
 - Use cases as simple as possible.
 - Do not write use cases repeatedly for the same function.
 - The file that defines the main() function is named example_main.c. If the hello world case is defined, it is named hello_world_main.c. Do not name it main.c, because too many main.c are not convenient for IDE tools to search.
 - The configuration in the use case Kconfig is recommended to start with EXAMPLE to avoid other configuration life with the SDK, which is convenient for IDE tools to search.
 - Each use case requires documentation, namely the README.md file, and the use case documentation will be automatically generated through README.md in the future.



