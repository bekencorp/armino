/*
 * Copyright (C), 2018-2019, Arm Technology (China) Co., Ltd.
 * All rights reserved
 *
 * The content of this file or document is CONFIDENTIAL and PROPRIETARY
 * to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 * License Agreement between Licensee and Arm Technology (China) Co., Ltd
 * restricting among other things, the use, reproduction, distribution
 * and transfer.  Each of the embodiments, including this information and,,
 * any derivative work shall retain this copyright notice.
 */

#include "soc/bk7236/reg_base.h"
#include "partitions.h"
#include "sdkconfig.h"

OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")

__MSP_STACK_SIZE = (4 << 10);
__MIN_HEAP_SIZE  = (32 << 10);

__SWAP_SIZE = (1024);

MEMORY
{
#if CONFIG_SPE
  FLASH (rx)            : ORIGIN = (SOC_FLASH_DATA_BASE + CONFIG_PRIMARY_APP_VIRTUAL_CODE_START), LENGTH = CONFIG_PRIMARY_APP_VIRTUAL_CODE_SIZE
  IRAM (rwx)            : ORIGIN = SOC_SRAM1_DATA_BASE, LENGTH = CONFIG_PRIMARY_APP_IRAM_SIZE
  RAM (rwx)             : ORIGIN = SOC_SRAM1_DATA_BASE + CONFIG_PRIMARY_APP_IRAM_SIZE, LENGTH = CONFIG_SPE_TOTAL_RAM_SIZE - CONFIG_PRIMARY_APP_IRAM_SIZE
#else
  FLASH (rx)            : ORIGIN = (SOC_FLASH_DATA_BASE + CONFIG_PRIMARY_NS_VIRTUAL_CODE_START), LENGTH = CONFIG_PRIMARY_NS_VIRTUAL_CODE_SIZE
  RAM (rwx)             : ORIGIN = SOC_SRAM1_DATA_BASE, LENGTH = CONFIG_NSPE_TOTAL_RAM_SIZE
#endif


  ITCM (rwx)            : ORIGIN = SOC_ITCM_DATA_BASE + 4, LENGTH = CONFIG_ITCM_SIZE - 4
  DTCM (rwx)            : ORIGIN = SOC_DTCM_DATA_BASE + 0, LENGTH = CONFIG_DTCM_SIZE
  SWAP (rwx)            : ORIGIN = SOC_SRAM1_DATA_BASE + CONFIG_SPE_TOTAL_RAM_SIZE + CONFIG_SECOND_APP_RAM_SIZE - __SWAP_SIZE, LENGTH = __SWAP_SIZE
  PSRAM (rwx)           : ORIGIN = SOC_PSRAM_DATA_BASE, LENGTH = 0x4000000
}

ENTRY(Reset_Handler)

SECTIONS
{

    ASSERT((. == ALIGN(128)), "vector table address align fault.")
    .vectors :
    {
        __vector_table = .;
        KEEP(*(.vectors))
        . = ALIGN(512);
    } > FLASH

#if CONFIG_CPU_WITHOUT_BOOTLOADER
    ASSERT((. == ALIGN(128)), "vector table address align fault.")
    .vectors_cpu0 :
    {
        __vector_cpu0_table = .;
        KEEP(*(.vectors_cpu0))
        . = ALIGN(512);
    } > FLASH
#endif

    .gnu.sgstubs ALIGN(32) : ALIGN(32)
    {
        *(.gnu.sgstubs*)
        . = ALIGN(32);
    } > FLASH

    .text :
    {
        . = ALIGN(4);
        _stext = .;
        EXCLUDE_FILE (*txl_he.c.obj *txl_agg.c.obj *txl_cntrl.c.obj *rxl_hwdesc.c.obj *libip_ax_next.a:rc.c.obj) *(.text*)
        EXCLUDE_FILE (*txl_he.c.obj *txl_agg.c.obj *txl_cntrl.c.obj *rxl_hwdesc.c.obj *libip_ax_next.a:rc.c.obj) *(".text.*")

        . = ALIGN(4);
        EXCLUDE_FILE (*txl_he.c.obj *txl_agg.c.obj *txl_cntrl.c.obj *rxl_hwdesc.c.obj *libip_ax_next.a:rc.c.obj) *(.rodata*)
        EXCLUDE_FILE (*txl_he.c.obj *txl_agg.c.obj *txl_cntrl.c.obj *rxl_hwdesc.c.obj *libip_ax_next.a:rc.c.obj) *(".rodata.*")

        . = ALIGN(4);
        __devconfig_start = .;
        *(".devconfig.*")
        KEEP(*(SORT_BY_NAME(".devconfig*")))
        __devconfig_end = .;

        . = ALIGN(4);
        __apps_start = .;
        KEEP (*(.apps_data))
        __apps_end = .;
        _etext = .;
        . = ALIGN(4);
    } > FLASH

    .a_device_null :
    {
      KEEP(*(.a_deviceobj_null))
    } > FLASH

    .a_devices :
    {
      __device_start = .;
      KEEP(*(.a_deviceobj_*))
      __device_end = .;
    } > FLASH

    .a_init_entries :
    {
      __a_init_start = .;
      KEEP(*(.a_init_entry_*))
      __a_init_end = .;
    } > FLASH

    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
        . = ALIGN(4);
    } > FLASH

    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > FLASH
    __exidx_end = .;

    .copy.table :
    {
        . = ALIGN(4);
        __copy_table_start__ = .;
        LONG (__etext)
        LONG (__data_start__)
        LONG ((__data_end__ - __data_start__) / 4)

        /* Add each additional data section here */
        LONG (__etext2)
        LONG (__data2_start__)
        LONG ((__data2_end__ - __data2_start__) / 4)

        LONG (__iram_flash_begin)
        LONG (__iram_start__)
        LONG ((__iram_end__ - __iram_start__) / 4)

        LONG (__itcm_text)
        LONG (__itcm_start__)
        LONG ((__itcm_end__ - __itcm_start__) / 4)

        LONG (__dtcm_content)
        LONG (__dtcm_start__)
        LONG ((__dtcm_end__ - __dtcm_start__) / 4)
	
        __copy_table_end__ = .;
    } > FLASH

    .zero.table :
    {
        . = ALIGN(4);
        __zero_table_start__ = .;
        LONG (_bss_start)
        LONG ((_bss_end - _bss_start) / 4)
        /* Add each additional bss section here */
        LONG (__bss2_start__)
        LONG ((__bss2_end__ - __bss2_start__) / 4)
        __zero_table_end__ = .;
    } > FLASH

    .itcm :
    {
        . = ALIGN(4);
        PROVIDE(__itcm_text = LOADADDR(.itcm));
        __itcm_start__ = .;
        *(.itcm)
        *(.itcm_section*)
        *(.itcm_sec_code*)
        . = ALIGN(4);

        *txl_he.c.obj(.text.* )
        *txl_cntrl.c.obj(.text.*)
        *rxl_hwdesc.c.obj(.text.* *.rodata *.rodata.*)
        __itcm_end__ = .;
    } > ITCM AT > FLASH

    .dtcm :
    {
        . = ALIGN(4);

        PROVIDE(__dtcm_content = LOADADDR(.dtcm));
        __dtcm_start__ = .;
        *(.dtcm)
        *(.dtcm_section*)
	*(.dtcm_sec_data*)

        __dtcm_end__ = .;
    } > DTCM AT > FLASH

    .iram :
    {
        . = ALIGN(512);
        PROVIDE(__iram_flash_begin = LOADADDR(.iram));
        __iram_start__ = .;

        /* TBD: Put macsw time ciritical code here, may move to itcm section */
        *libip_ax_next.a:rc.c.obj(.text.* *.rodata *.rodata.*)
        *txl_he.c.obj(.text.* *.rodata *.rodata.*)
        *txl_agg.c.obj(.text.* *.rodata *.rodata.*)
        *txl_cntrl.c.obj(.text.* *.rodata *.rodata.*)

        *(.iram)
        __iram_end__ = .;
    } > IRAM AT > FLASH

    .data :
    {
        . = ALIGN(4);
        PROVIDE(__etext = LOADADDR(.data));
        __data_start__ = .;

        *(.data)
        *(".data.*")
        *(.sdata)

        *(.video_spec_data*)
        *(.gnu.linkonce.d*)
        __data_end__ = .;
    } > RAM AT > FLASH

    _data_flash_begin = LOADADDR(.data);
    _data_ram_begin = ADDR(.data);
    _data_ram_end = .;

    s_rom_end = LOADADDR(.data) + SIZEOF(.data);

    .uninitialized (NOLOAD):
    {
        . = ALIGN(32);
        __uninitialized_start = .;
        *(.uninitialized)
        *(".uninitialized.*")
        KEEP(*(.keep.uninitialized))
        . = ALIGN(32);
        __uninitialized_end = .;
    } > RAM

    .bss (NOLOAD):
    {
        . = ALIGN(4);
        _bss_start = .;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _bss_end = .;
    } > RAM

    bss_size = _bss_end - _bss_start;

    .heap (COPY) :
    {
        . = ALIGN(8);
        _heap_start = .;
        . = . + (ORIGIN(RAM) + LENGTH(RAM) - __MSP_STACK_SIZE - _heap_start - 8); /* 16 bytes for boundary protection */
        . = ALIGN(8);
        _heap_end = .;
    } > RAM

    .stack (ORIGIN(RAM) + LENGTH(RAM) - __MSP_STACK_SIZE) (COPY) :
    {
        . = ALIGN(8);
        _sstack = .;
        __StackLimit = .;
        . = . + __MSP_STACK_SIZE;
        . = ALIGN(8);
        __StackTop = .;
        _estack = .;
    } > RAM

    .swap ORIGIN(SWAP):
    {
        . = ALIGN(8);
        _swap_start = .;

        * (.swap_data)
        * (.swap_data*)
        _swap_end = .;
    } > IRAM AT > RAM

    .video.cache.data :
    {
        . = ALIGN(4);
        PROVIDE(__etext2 = LOADADDR(.video.cache.data));
        __data2_start__ = .;

        *(.video_cache_data)
        *(.video_cache_data*)

        . = ALIGN(4);
        __data2_end__ = .;
    } > PSRAM AT > FLASH

    .video_cache_bss (NOLOAD):
    {
        . = ALIGN(4);
        __bss2_start__ = .;
        *(.video_cache_bss)
        *(.video_cache_bss*)
        . = ALIGN(4);
        __bss2_end__ = .;
    } > PSRAM

    /* Set stack top to end of RAM, and stack limit move down by
     * size of stack_dummy section */
    __MSPTop = ORIGIN(RAM) + LENGTH(RAM);
    __MSPLimit = __MSPTop - __MSP_STACK_SIZE;

    ASSERT((s_rom_end < ORIGIN(FLASH) + LENGTH(FLASH)), "ROM overflow!!!")
    ASSERT(((ORIGIN(RAM) + LENGTH(RAM)) > (_heap_start + __MSP_STACK_SIZE + 8)), "Stack overflowed with bss")
    ASSERT(((_heap_end - _heap_start) >= __MIN_HEAP_SIZE), "Heap smaller than minimize size 32K!!!")
}
