;/*
; * Copyright (c) 2009-2019 Arm Limited
; *
; * Licensed under the Apache License, Version 2.0 (the "License");
; * you may not use this file except in compliance with the License.
; * You may obtain a copy of the License at
; *
; *     http://www.apache.org/licenses/LICENSE-2.0
; *
; * Unless required by applicable law or agreed to in writing, software
; * distributed under the License is distributed on an "AS IS" BASIS,
; * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; * See the License for the specific language governing permissions and
; * limitations under the License.
; */

;/*
; * This file is derivative of CMSIS V5.00 gcc_arm.ld
; */

/* Linker script to configure memory regions. */
/* This file will be run trough the pre-processor. */

#include "region_defs_bl0.h"

MEMORY
{
  FLASH (rx)  : ORIGIN = BL0_FLASH_BASE, LENGTH = BL0_FLASH_SIZE
  CODE_RAM (rwx) : ORIGIN = BL0_CODE_SRAM_BASE, LENGTH = BL0_CODE_SRAM_SIZE
  RAM   (rwx) : ORIGIN = BL0_DATA_BASE, LENGTH = BL0_DATA_SIZE
}

__heap_size__  = BL0_HEAP_SIZE;
__msp_stack_size__ = BL0_MSP_STACK_SIZE;

/* Library configurations */
GROUP(libgcc.a libc.a libm.a libnosys.a)

/* Linker script to place sections and symbol values. Should be used together
 * with other linker script that defines memory regions FLASH and RAM.
 * It references following symbols, which must be defined in code:
 *   Reset_Handler : Entry of reset handler
 *
 * It defines following symbols, which code can use without definition:
 *   __exidx_start
 *   __exidx_end
 *   __copy_table_start__
 *   __copy_table_end__
 *   __zero_table_start__
 *   __zero_table_end__
 *   __etext
 *   __data_start__
 *   __preinit_array_start
 *   __preinit_array_end
 *   __init_array_start
 *   __init_array_end
 *   __fini_array_start
 *   __fini_array_end
 *   __data_end__
 *   __bss_start__
 *   __bss_end__
 *   __end__
 *   end
 *   __HeapBase
 *   __HeapLimit
 *   __StackLimit
 *   __StackTop
 *   __stack
 *   __Vectors_End
 *   __Vectors_Size
 */
ENTRY(Reset_Handler)

SECTIONS
{
    /* Startup section is loaded to Flash and runs from Flash */
    .startup :
    {
        KEEP(*(.vectors))
        __Vectors_End = .;
        __Vectors_Size = __Vectors_End - __Vectors;
        __end__ = .;


        KEEP(*(.init))
        KEEP(*(.fini))

        *startup_cmsdk_musca_bl2.*
    } > FLASH

    .ER_CODE_SRAM :
    {
        *bl0_main.o(.text*)
        *bl0_main.o(.rodata*)
        . = ALIGN(4); /* This alignment is needed to make the section size 4 bytes aligned */
    } > CODE_RAM AT > FLASH
    Image$$ER_CODE_SRAM$$Base = ADDR(.ER_CODE_SRAM);
    Image$$ER_CODE_SRAM$$Limit = ADDR(.ER_CODE_SRAM) + SIZEOF(.ER_CODE_SRAM);

    .text :
    {
        *(.text*)

        /* .ctors */
        *crtbegin.o(.ctors)
        *crtbegin?.o(.ctors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
        *(SORT(.ctors.*))
        *(.ctors)

        /* .dtors */
         *crtbegin.o(.dtors)
         *crtbegin?.o(.dtors)
         *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
         *(SORT(.dtors.*))
         *(.dtors)

        *(.rodata*)

        KEEP(*(.eh_frame*))
    } > FLASH

    .ARM.extab : ALIGN(32)
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > FLASH

    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > FLASH
    __exidx_end = .;

    /* To copy multiple ROM to RAM sections,
     * define etext2/data2_start/data2_end and
     * define __STARTUP_COPY_MULTIPLE in startup_cmsdk_mps2_sse_200.S */
    .copy.table : ALIGN(4)
    {
        __copy_table_start__ = .;
        LONG (__etext)
        LONG (__data_start__)
        LONG (__data_end__ - __data_start__)
        LONG (LOADADDR(.ER_CODE_SRAM))
        LONG (ADDR(.ER_CODE_SRAM))
        LONG (SIZEOF(.ER_CODE_SRAM))
        LONG (DEFINED(__etext2) ? __etext2 : 0)
        LONG (DEFINED(__data2_start__) ? __data2_start__ : 0)
        LONG (DEFINED(__data2_start__) ? __data2_end__ - __data2_start__ : 0)
        __copy_table_end__ = .;
    } > FLASH

    /* To clear multiple BSS sections,
     * uncomment .zero.table section and,
     * define __STARTUP_CLEAR_BSS_MULTIPLE in startup_ARMCMx.S */
    .zero.table : ALIGN(4)
    {
        __zero_table_start__ = .;
        LONG (__bss_start__)
        LONG (__bss_end__ - __bss_start__)
        LONG (DEFINED(__bss2_start__) ? __bss2_start__ : 0)
        LONG (DEFINED(__bss2_start__) ? __bss2_end__ - __bss2_start__ : 0)
        __zero_table_end__ = .;
    } > FLASH

    __etext = ALIGN(4);

    .data : ALIGN(4)
    {
        __data_start__ = .;
        *(vtable)
        *(.data*)

        . = ALIGN(4);
        /* preinit data */
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP(*(.preinit_array))
        PROVIDE_HIDDEN (__preinit_array_end = .);

        . = ALIGN(4);
        /* init data */
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE_HIDDEN (__init_array_end = .);


        . = ALIGN(4);
        /* finit data */
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        PROVIDE_HIDDEN (__fini_array_end = .);

        KEEP(*(.jcr*))
        . = ALIGN(4);
        /* All data end */
        __data_end__ = .;

    } > RAM AT> FLASH

    .bss :
    {
        . = ALIGN(4);
        __bss_start__ = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
    } > RAM

    bss_size = __bss_end__ - __bss_start__;

    .msp_stack : ALIGN(32)
    {
        . += __msp_stack_size__;
    } > RAM
    Image$$ARM_LIB_STACK$$ZI$$Base = ADDR(.msp_stack);
    Image$$ARM_LIB_STACK$$ZI$$Limit = ADDR(.msp_stack) + SIZEOF(.msp_stack);

    .heap : ALIGN(8)
    {
        . = ALIGN(8);
        __end__ = .;
        PROVIDE(end = .);
        __HeapBase = .;
        . += __heap_size__;
        __HeapLimit = .;
        __heap_limit = .; /* Add for _sbrk */
    } > RAM
    Image$$ARM_LIB_HEAP$$ZI$$Limit = ADDR(.heap) + SIZEOF(.heap);

    PROVIDE(__stack = Image$$ARM_LIB_STACK$$ZI$$Limit);
}
