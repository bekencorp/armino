Memory and performance
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

3. PSRAM Partition
---------------------------------------

    The BK7256 chip has 8M PSRAM, with the first 7M mainly used for Camera data acquisition and LCD image display;

 - 800 * 600 Resolution ::

    YUV image storage address range, 0x6000000-0x603BFFFF, a total of 4 frames,
        Display occupies 1 frame, pingping buffer occupies 1 frame, jpeg decoding occupies 1 frame, and rotation occupies 1 frame;
    The storage address range for jpeg images is 0x60330000-0x60488000, with a total of 4 frames,
        Camera acquisition occupies 1 frame, jpeg decoding occupies 1 frame, image transmission occupies 1 frame, and image storage occupies 1 frame;

 - 1280 * 720 Resolution ::

    Yuv image storage address range, 0x6000000-0x6056FFFF, a total of 3 frames,
        Display occupies 1 frame, pingping buffer occupies 1 frame, and jpeg decoding occupies 1 frame;
    Jpeg image storage address range, 0x60570000-0x60637FFF, a total of 4 frames,
        Camera acquisition occupies 1 frame, jpeg decoding occupies 1 frame, image transmission occupies 1 frame, and image storage occupies 1 frame;

 - LVGL drawing is a separate usage scenario ::

    LVGL drawing start address: 0x6050000 (configurable)
