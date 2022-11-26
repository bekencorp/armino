Memory and performance (TBC)
---------------------------------------------------------

:link_to_translation:`zh_CN:[中文]`

1. Memory overview
----------------------------------------

    BK7256 as a whole has 4*128K sram, dual cores each have independent 32k dtcm and 32k itcm, 8M PSRAM


2. PSRAM Overview
---------------------------------------

    BK7256 chip has 8M PSRAM, and the last 1M (0x60700000~0x60800000) can be used as heap memory

 - Function switch CONFIG_PSRAM_AS_SYS_MEMORY relies on CONFIG_PSRAM and CONFIG_FREERTOS_V10
 - Memory interface psram_malloc and psram_free
 - BK7256 chip PSRAM only supports 4-byte aligned access. PSRAM must apply for memory according to WORD operation and ensure 4-byte aligned access ::

    #include <os/mem.h>

    #define os_write_word(addr,val)                 *((volatile uint32_t *)(addr)) = val
    #define os_read_word(addr,val)                  val = *((volatile uint32_t *)(addr))
    #define os_get_word(addr)                       *((volatile uint32_t *)(addr))

    void os_memcpy_word(uint32_t *out, const uint32_t *in, uint32_t n);
    void os_memset_word(uint32_t *b, int32_t c, uint32_t n);

 - The low-voltage function PARAM of BK7256 chip will be powered down. 
   When exiting the low-voltage mode, you must use interface 'void bk_psram_heap_init(void)' to reinitialize memory 'PSRAM Heap',
   and the memory previously applied through 'psram_malloc' cannot be used again.




