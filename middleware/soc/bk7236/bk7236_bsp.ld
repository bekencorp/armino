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

__MSP_STACK_SIZE = (10 << 10);
__MIN_HEAP_SIZE  = (64 << 10);

MEMORY
{
#if CONFIG_SPE
  FLASH (rx)            : ORIGIN = (SOC_FLASH_DATA_BASE + CONFIG_PRIMARY_APP_VIRTUAL_CODE_START), LENGTH = CONFIG_PRIMARY_APP_VIRTUAL_CODE_SIZE
  RAM (rwx)             : ORIGIN = SOC_SRAM0_DATA_BASE, LENGTH = CONFIG_SPE_TOTAL_RAM_SIZE
#else
  FLASH (rx)            : ORIGIN = (SOC_FLASH_DATA_BASE + CONFIG_PRIMARY_NS_VIRTUAL_CODE_START), LENGTH = CONFIG_PRIMARY_NS_VIRTUAL_CODE_SIZE
  RAM (rwx)             : ORIGIN = SOC_SRAM3_DATA_BASE, LENGTH = CONFIG_NSPE_TOTAL_RAM_SIZE
#endif
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
	_stext = .;
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
	_etext = .;
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
        LONG ((__data_end__ - __data_start__) / 4)
        /* Add each additional data section here */
        __copy_table_end__ = .;
    } > FLASH

    .zero.table :
    {
        . = ALIGN(4);
        __zero_table_start__ = .;
        LONG (_bss_start)
        LONG ((_bss_end - _bss_start) / 4)
        /* Add each additional bss section here */
        __zero_table_end__ = .;
    } > FLASH

    .data :
    {
        . = ALIGN(4);
        PROVIDE(__etext = LOADADDR(.data));
        __data_start__ = .;

        *(.data)
        *(".data.*")
        *(.sdata)
        *(.dtcm_sec_data*)
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

    /* Set stack top to end of RAM, and stack limit move down by
     * size of stack_dummy section */
    __MSPTop = ORIGIN(RAM) + LENGTH(RAM);
    __MSPLimit = __MSPTop - __MSP_STACK_SIZE;

    ASSERT((s_rom_end < ORIGIN(FLASH) + LENGTH(FLASH)), "ROM overflow!!!")
    ASSERT(((ORIGIN(RAM) + LENGTH(RAM)) > (_heap_start + __MSP_STACK_SIZE + 8)), "Stack overflowed with bss")
    ASSERT(((_heap_end - _heap_start) >= __MIN_HEAP_SIZE), "Heap smaller than minimize size 64K!!!")
}
