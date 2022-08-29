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

OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")

__STACK_SIZE = 0x00000400;
__HEAP_SIZE  = 0x00000C00;

MEMORY
{
  FLASH (rx)            : ORIGIN = SOC_FLASH_DATA_BASE, LENGTH = 0x2000000
  RAM (rwx)             : ORIGIN = SOC_SRAM0_DATA_BASE, LENGTH = 0x80000
}

ENTRY(Reset_Handler)

SECTIONS
{
    ASSERT((. == ALIGN(128)), "vector table address align fault.")
    .vectors :
    {
        __vector_table = .;
        KEEP(*(.vectors))
        . = ALIGN(32);
    } > FLASH

    .gnu.sgstubs ALIGN(32) : ALIGN(32)
    {
        *(.gnu.sgstubs*)
        . = ALIGN(32);
    } > FLASH

    .text :
    {
        . = ALIGN(4);
        *(.text*)
        *(".text.*")

        . = ALIGN(4);
        *(.rodata*)
        *(".rodata.*")

        . = ALIGN(4);
        __devconfig_start = .;
        *(".devconfig.*")
        KEEP(*(SORT_BY_NAME(".devconfig*")))
        __devconfig_end = .;

        . = ALIGN(4);
        __apps_start = .;
        KEEP (*(.apps_data))
        __apps_end = .;
        . = ALIGN(4);
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
        LONG (__data_end__ - __data_start__)
        /* Add each additional data section here */
        /*
        LONG (__etext2)
        LONG (__data2_start__)
        LONG (__data2_end__ - __data2_start__)
        */
        __copy_table_end__ = .;
    } > FLASH

    .zero.table :
    {
        . = ALIGN(4);
        __zero_table_start__ = .;
        LONG (_bss_start)
        LONG (_bss_end - _bss_start)
        /* Add each additional bss section here */
        /*
            LONG (__bss2_start__)
            LONG (__bss2_end__ - __bss2_start__)
        */
        __zero_table_end__ = .;
    } > FLASH

    .data :
    {
        . = ALIGN(4);
        PROVIDE(__etext = LOADADDR(.data));
        __data_start__ = .;
        *(.data)
        *(".data.*")

        . = ALIGN(4);
        __device_init_start = .;
        KEEP(*(SORT_BY_NAME(.init_level_[0-9]_[0-9])));
        KEEP(*(SORT_BY_NAME(.init_level_[0-9]_[1-9][0-9])));
        . = ALIGN(4);
        __device_init_end = .;

        __commands_table_start = .;
        KEEP(*(SORT_BY_NAME(.commands)));
        KEEP(*(SORT_BY_NAME(.commands.*)));
        . = ALIGN(4);
        __commands_table_end = .;

        /* All data end */
        __data_end__ = .;

    } > RAM AT > FLASH

    _data_flash_begin = LOADADDR(.data);
    _data_ram_begin = ADDR(.data);
    _data_ram_end = .;

    /* set the global rom end */
    g_rom_end = LOADADDR(.data) + SIZEOF(.data);

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
        __START_BSS = .;
        _bss_start = .;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _bss_end = .;
        __END_BSS = .;

    } > RAM

    . = ALIGN (8);
    _empty_ram = .;

    bss_size = _bss_end - _bss_start;

    .heap (COPY) :
    {
        . = ALIGN(8);
        __end__ = .;
        PROVIDE(end = .);
        . = . + __HEAP_SIZE;
        . = ALIGN(8);
        __HeapLimit = .;
    } > RAM

    .stack (ORIGIN(RAM) + LENGTH(RAM) - __STACK_SIZE) (COPY) :
    {
        . = ALIGN(8);
        __StackLimit = .;
        . = . + __STACK_SIZE;
        . = ALIGN(8);
        __StackTop = .;
    } > RAM

    /* Set stack top to end of RAM, and stack limit move down by
     * size of stack_dummy section */
    __MSPTop = ORIGIN(RAM) + LENGTH(RAM);
    __MSPLimit = __MSPTop - 10240;
    PROVIDE(__stack = __MSPTop);

    /* Check if data + heap + stack exceeds RAM limit */
    ASSERT(__MSPLimit >= _bss_end, "region RAM overflowed with stack")
    ASSERT((g_rom_end < ORIGIN(FLASH) + LENGTH(FLASH)), "ROM overflow!!!")
    ASSERT((_bss_end < ORIGIN(RAM)  + LENGTH(RAM)), "RAM overflow!!!")

}   /* End of sections */
