POSIX APIs
=======================

:link_to_translation:`en:[English]`

POSIX APIs 介绍
------------------------

 -  目前Armino平台posix接口仅支持FreeRTOS V10.4操作系统，默认关闭;
 -  若打开使用，需要打开CONFIG_FREERTOS_POSIX配置开关


.. toctree::
    :maxdepth: 1

    unistd接口 <unistd>
    utils接口 <utils>
    errno接口 <errno>
    fcntl接口 <fcntl>
    signal接口 <signal>
    mqueue接口 <mqueue>
    pthread接口 <pthread>
    sched接口 <sched>
    semaphore接口 <semaphore>
    time接口 <time>
