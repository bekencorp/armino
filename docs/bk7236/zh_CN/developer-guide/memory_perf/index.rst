内存与性能 (TBC)
-------------------------------------

:link_to_translation:`en:[English]`

1、内存概述
--------------------------

    BK7236芯片整体有4*128K sram，双核各有独立的32k dtcm和32k itcm，8M PSRAM


2、PSRAM概述
-------------------------

    BK7236芯片有8M PSRAM，最后1M(0x60700000~0x60800000)可以用作堆内存使用

 - 功能开关CONFIG_PSRAM_AS_SYS_MEMORY依赖CONFIG_PSRAM和CONFIG_FREERTOS_V10
 - 内存接口psram_malloc和psram_free
 - BK7236芯片PSRAM只支持4字节对齐访问，必须按照WORD操作PSRAM申请内存并保证4字节对齐 ::

    #include <os/mem.h>

    #define os_write_word(addr,val)                 *((volatile uint32_t *)(addr)) = val
    #define os_read_word(addr,val)                  val = *((volatile uint32_t *)(addr))
    #define os_get_word(addr)                       *((volatile uint32_t *)(addr))

    void os_memcpy_word(uint32_t *out, const uint32_t *in, uint32_t n);
    void os_memset_word(uint32_t *b, int32_t c, uint32_t n);

 - BK7236芯片低压功能PARAM会下电，退出低压模式时必须使用void bk_psram_heap_init(void)接口重新初始化PSRAM Heap内存，
   并且之前通过psram_malloc申请的内存，不能再使用。


