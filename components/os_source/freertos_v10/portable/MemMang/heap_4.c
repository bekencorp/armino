/*
    FreeRTOS V9.0.0 - Copyright (C) 2016 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

/*
 * A sample implementation of pvPortMalloc() and vPortFree() that combines
 * (coalescences) adjacent memory blocks as they are freed, and in so doing
 * limits memory fragmentation.
 *
 * See heap_1.c, heap_2.c and heap_3.c for alternative implementations, and the
 * memory management pages of http://www.FreeRTOS.org for more information.
 */
#include <common/bk_include.h>
#include <os/mem.h>

#include <stdlib.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
all the API functions to use the MPU wrappers.  That should only be done when
task.h is included from an application file. */
#define MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#include "FreeRTOS.h"
#include "task.h"
#include <components/log.h>
#if CONFIG_MEM_DEBUG
#include "bk_list.h"
#include <os/str.h>
#endif

#undef MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#define TAG "os"

#if( configSUPPORT_DYNAMIC_ALLOCATION == 0 )
	#error This file must not be used if configSUPPORT_DYNAMIC_ALLOCATION is 0
#endif

/* Block sizes must not get too small. */
#define heapMINIMUM_BLOCK_SIZE	( ( size_t ) ( xHeapStructSize + 8 ) )

/* Assumes 8bit bytes! */
#define heapBITS_PER_BYTE		( ( size_t ) 8 )

#define MEM_OVERFLOW_TAG      0xcd
#if CONFIG_MEM_DEBUG
#define MEM_CHECK_TAG_LEN      0x4
#else
#define MEM_CHECK_TAG_LEN      0x0
#endif

/* Allocate the memory for the heap. */
#if configDYNAMIC_HEAP_SIZE
uint8_t *ucHeap;
#elif( configAPPLICATION_ALLOCATED_HEAP == 1 )
	/* The application writer has already defined the array used for the RTOS
	heap - probably so it can be placed in a special segment or address. */
	extern uint8_t ucHeap[ configTOTAL_HEAP_SIZE ];
#else
	static uint8_t ucHeap[ configTOTAL_HEAP_SIZE ];
#endif /* configAPPLICATION_ALLOCATED_HEAP */

#ifndef CONFIG_MEM_DEBUG_FUNC_NAME_LEN
#define CONFIG_MEM_DEBUG_FUNC_NAME_LEN 16
#endif

#ifndef CONFIG_MEM_DEBUG_TASK_NAME_LEN
#define CONFIG_MEM_DEBUG_TASK_NAME_LEN 8
#endif

/* Define the linked list structure.  This is used to link free blocks in order
of their memory address. */
typedef struct A_BLOCK_LINK
{
	struct A_BLOCK_LINK *pxNextFreeBlock;	/*<< The next free block in the list. */
	size_t xBlockSize;						/*<< The size of the free block. */
#if CONFIG_MEM_DEBUG
	struct list_head node;					/*<< linked to xUsed */
	unsigned int allocTime;					/*<< the leak time (*1sec)*/
#if CONFIG_MEM_DEBUG_FUNC_NAME
	char funcName[CONFIG_MEM_DEBUG_FUNC_NAME_LEN];		/*<< the function name */
#endif

#if CONFIG_MEM_DEBUG_TASK_NAME
	char taskName[CONFIG_MEM_DEBUG_TASK_NAME_LEN];		/*<< the task name */
#endif

	unsigned int line;					/*<< the function line */
	int wantedSize;						/*<< malloc size */
#endif
} BlockLink_t;

/*-----------------------------------------------------------*/

/*
 * Inserts a block of memory that is being freed into the correct position in
 * the list of free memory blocks.  The block being freed will be merged with
 * the block in front it and/or the block behind it if the memory blocks are
 * adjacent to each other.
 */
static void prvInsertBlockIntoFreeList( BlockLink_t *pxBlockToInsert );

/*
 * Called automatically to setup the required heap structures the first time
 * pvPortMalloc() is called.
 */
static void prvHeapInit( void );

/*-----------------------------------------------------------*/

/* The size of the structure placed at the beginning of each allocated memory
block must by correctly byte aligned. */
static const size_t xHeapStructSize	= ( sizeof( BlockLink_t ) + ( ( size_t ) ( portBYTE_ALIGNMENT - 1 ) ) ) & ~( ( size_t ) portBYTE_ALIGNMENT_MASK );

/* Create a couple of list links to mark the start and end of the list. */
static BlockLink_t xStart, *pxEnd = NULL;
#if CONFIG_MEM_DEBUG
static struct list_head xUsed;
#endif

/* Keeps track of the number of free bytes remaining, but says nothing about
fragmentation. */
static size_t xFreeBytesRemaining = 0U;
static size_t xMinimumEverFreeBytesRemaining = 0U;

/* Gets set to the top bit of an size_t type.  When this bit in the xBlockSize
member of an BlockLink_t structure is set then the block belongs to the
application.  When the bit is free the block is still part of the free heap
space. */
static size_t xBlockAllocatedBit = 0;

#if (CONFIG_SOC_BK7251)
uint8_t *psram_ucHeap;
/* Create a couple of list links to mark the start and end of the list. */
static BlockLink_t psram_xStart, *psram_pxEnd = NULL;

/* Keeps track of the number of free bytes remaining, but says nothing about
fragmentation. */
static size_t psram_xFreeBytesRemaining = 0U;
static size_t psram_xMinimumEverFreeBytesRemaining = 0U;

#define PSRAM_START_ADDRESS    (void*)(0x00900000)
#define PSRAM_END_ADDRESS      (void*)(0x00900000 + 256 * 1024)

/*-----------------------------------------------------------*/

static void psram_prvInsertBlockIntoFreeList( BlockLink_t *pxBlockToInsert )
{
BlockLink_t *pxIterator;
uint8_t *puc;

	/* Iterate through the list until a block is found that has a higher address
	than the block being inserted. */
	for( pxIterator = &psram_xStart; pxIterator->pxNextFreeBlock < pxBlockToInsert; pxIterator = pxIterator->pxNextFreeBlock )
	{
		/* Nothing to do here, just iterate to the right position. */
	}

	/* Do the block being inserted, and the block it is being inserted after
	make a contiguous block of memory? */
	puc = ( uint8_t * ) pxIterator;
	if( ( puc + pxIterator->xBlockSize ) == ( uint8_t * ) pxBlockToInsert )
	{
		pxIterator->xBlockSize += pxBlockToInsert->xBlockSize;
		pxBlockToInsert = pxIterator;
	}
	else
	{
		mtCOVERAGE_TEST_MARKER();
	}

	/* Do the block being inserted, and the block it is being inserted before
	make a contiguous block of memory? */
	puc = ( uint8_t * ) pxBlockToInsert;
	if( ( puc + pxBlockToInsert->xBlockSize ) == ( uint8_t * ) pxIterator->pxNextFreeBlock )
	{
		if( pxIterator->pxNextFreeBlock != psram_pxEnd )
		{
			/* Form one big block from the two blocks. */
			pxBlockToInsert->xBlockSize += pxIterator->pxNextFreeBlock->xBlockSize;
			pxBlockToInsert->pxNextFreeBlock = pxIterator->pxNextFreeBlock->pxNextFreeBlock;
		}
		else
		{
			pxBlockToInsert->pxNextFreeBlock = psram_pxEnd;
		}
	}
	else
	{
		pxBlockToInsert->pxNextFreeBlock = pxIterator->pxNextFreeBlock;
	}

	/* If the block being inserted plugged a gab, so was merged with the block
	before and the block after, then it's pxNextFreeBlock pointer will have
	already been set, and should not be set here as that would make it point
	to itself. */
	if( pxIterator != pxBlockToInsert )
	{
		pxIterator->pxNextFreeBlock = pxBlockToInsert;
	}
	else
	{
		mtCOVERAGE_TEST_MARKER();
	}
}

/*-----------------------------------------------------------*/

static void *psram_malloc_without_lock( size_t xWantedSize )
{
BlockLink_t *pxBlock, *pxPreviousBlock, *pxNewBlockLink;
void *pvReturn = NULL;

	{
		/* If this is the first call to malloc then the heap will require
		initialisation to setup the list of free blocks. */
		if( psram_pxEnd == NULL )
		{
			prvHeapInit();
		}
		else
		{
			mtCOVERAGE_TEST_MARKER();
		}

		/* Check the requested block size is not so large that the top bit is
		set.  The top bit of the block size member of the BlockLink_t structure
		is used to determine who owns the block - the application or the
		kernel, so it must be free. */
		if( ( xWantedSize & xBlockAllocatedBit ) == 0 )
		{
			/* The wanted size is increased so it can contain a BlockLink_t
			structure in addition to the requested amount of bytes. */
			if( xWantedSize > 0 )
			{
				xWantedSize += xHeapStructSize;

				/* Ensure that blocks are always aligned to the required number
				of bytes. */
				if( ( xWantedSize & portBYTE_ALIGNMENT_MASK ) != 0x00 )
				{
					/* Byte alignment required. */
					xWantedSize += ( portBYTE_ALIGNMENT - ( xWantedSize & portBYTE_ALIGNMENT_MASK ) );
					configASSERT( ( xWantedSize & portBYTE_ALIGNMENT_MASK ) == 0 );
				}
				else
				{
					mtCOVERAGE_TEST_MARKER();
				}
			}
			else
			{
				mtCOVERAGE_TEST_MARKER();
			}

			if( ( xWantedSize > 0 ) && ( xWantedSize <= psram_xFreeBytesRemaining ) )
			{
				/* Traverse the list from the start	(lowest address) block until
				one	of adequate size is found. */
				pxPreviousBlock = &psram_xStart;
				pxBlock = psram_xStart.pxNextFreeBlock;
				while( ( pxBlock->xBlockSize < xWantedSize ) && ( pxBlock->pxNextFreeBlock != NULL ) )
				{
					pxPreviousBlock = pxBlock;
					pxBlock = pxBlock->pxNextFreeBlock;
				}

				/* If the end marker was reached then a block of adequate size
				was	not found. */
				if( pxBlock != psram_pxEnd )
				{
					/* Return the memory space pointed to - jumping over the
					BlockLink_t structure at its start. */
					pvReturn = ( void * ) ( ( ( uint8_t * ) pxPreviousBlock->pxNextFreeBlock ) + xHeapStructSize );

					/* This block is being returned for use so must be taken out
					of the list of free blocks. */
					pxPreviousBlock->pxNextFreeBlock = pxBlock->pxNextFreeBlock;

					/* If the block is larger than required it can be split into
					two. */
					if( ( pxBlock->xBlockSize - xWantedSize ) > heapMINIMUM_BLOCK_SIZE )
					{
						/* This block is to be split into two.  Create a new
						block following the number of bytes requested. The void
						cast is used to prevent byte alignment warnings from the
						compiler. */
						pxNewBlockLink = ( void * ) ( ( ( uint8_t * ) pxBlock ) + xWantedSize );
						configASSERT( ( ( ( size_t ) pxNewBlockLink ) & portBYTE_ALIGNMENT_MASK ) == 0 );

						/* Calculate the sizes of two blocks split from the
						single block. */
						pxNewBlockLink->xBlockSize = pxBlock->xBlockSize - xWantedSize;
						pxBlock->xBlockSize = xWantedSize;

						/* Insert the new block into the list of free blocks. */
						psram_prvInsertBlockIntoFreeList( pxNewBlockLink );
					}
					else
					{
						mtCOVERAGE_TEST_MARKER();
					}

					psram_xFreeBytesRemaining -= pxBlock->xBlockSize;

					if( psram_xFreeBytesRemaining < psram_xMinimumEverFreeBytesRemaining )
					{
						psram_xMinimumEverFreeBytesRemaining = psram_xFreeBytesRemaining;
					}
					else
					{
						mtCOVERAGE_TEST_MARKER();
					}

					/* The block is being returned - it is allocated and owned
					by the application and has no "next" block. */
					pxBlock->xBlockSize |= xBlockAllocatedBit;
					pxBlock->pxNextFreeBlock = NULL;
				}
				else
				{
					mtCOVERAGE_TEST_MARKER();
				}
			}
			else
			{
				mtCOVERAGE_TEST_MARKER();
			}
		}
		else
		{
			mtCOVERAGE_TEST_MARKER();
		}

		traceMALLOC( pvReturn, xWantedSize );
	}

	#if( configUSE_MALLOC_FAILED_HOOK == 1 )
	{
		if( pvReturn == NULL )
		{
			extern void vApplicationMallocFailedHook( void );
			vApplicationMallocFailedHook();
		}
		else
		{
			mtCOVERAGE_TEST_MARKER();
		}
	}
	#endif

	configASSERT( ( ( ( size_t ) pvReturn ) & ( size_t ) portBYTE_ALIGNMENT_MASK ) == 0 );
	return pvReturn;
}

#if CONFIG_MALLOC_STATIS || CONFIG_MEM_DEBUG
void *psram_malloc_cm(const char *call_func_name, int line, size_t xWantedSize, int need_zero )
#else
void *psram_malloc( size_t xWantedSize )
#endif
{
	void *pvReturn = NULL;

	if (xWantedSize == 0)
		xWantedSize = 4;

	vTaskSuspendAll();
	pvReturn = psram_malloc_without_lock(xWantedSize);
	#if CONFIG_MALLOC_STATIS || CONFIG_MEM_DEBUG
	{
	if(pvReturn && call_func_name) {
	BlockLink_t *pxLink = (BlockLink_t *)((u8*)pvReturn - xHeapStructSize);
	BK_LOGI(TAG, "m:%p,%d|%s,%d\r\n", pxLink, (pxLink->xBlockSize & ~xBlockAllocatedBit), call_func_name, line);
	}
	}
	#endif
	( void ) xTaskResumeAll();

	#if CONFIG_MALLOC_STATIS || CONFIG_MEM_DEBUG
	if(pvReturn && need_zero)
		os_memset(pvReturn, 0, xWantedSize);
	#endif
	return pvReturn;
}

/*yhb added 2015-08-13. wzl added:this function has an issue*/
void *psram_realloc( void *pv, size_t xWantedSize )
{
	uint8_t *puc = ( uint8_t * ) pv;
	BlockLink_t *pxLink;
	int presize, datasize;
	void *pvReturn = NULL;
	BlockLink_t *pxIterator, *pxPreviousBlock, *tmp;

	if (pv == NULL)
		return psram_malloc(xWantedSize);

	/* The memory being freed will have an BlockLink_t structure immediately
	before it. */
	puc -= xHeapStructSize;
	/* This casting is to keep the compiler from issuing warnings. */
	pxLink = ( void * ) puc;

	presize = (pxLink->xBlockSize & ~xBlockAllocatedBit);
	datasize = presize - xHeapStructSize;
	if (datasize >= xWantedSize) // have enough memory don't need realloc
		return pv;

	pxLink->xBlockSize &= ~xBlockAllocatedBit;
	vTaskSuspendAll();
	/* Add this block to the list of free blocks. */
	psram_xFreeBytesRemaining += pxLink->xBlockSize;
	psram_prvInsertBlockIntoFreeList( ( ( BlockLink_t * ) pxLink ) );
	pvReturn = psram_malloc_without_lock(xWantedSize);
	if (pvReturn != NULL) {
		if (pvReturn != pv)
			os_memcpy(pvReturn, pv, datasize);
	} else { // if can't realloc such big memory, we should NOT put pv in free list.
		pxPreviousBlock = &psram_xStart;
		pxIterator = psram_xStart.pxNextFreeBlock;
		while( pxIterator != NULL )
		{
			if (pxIterator > pxLink) {
				break;
			}
			if (pxIterator == pxLink) {// find pxLink at the begin
				if (pxIterator->xBlockSize > presize) {
					tmp = (BlockLink_t *)((uint8_t*)pxLink + presize);
					tmp->xBlockSize = (pxIterator->xBlockSize - presize);
					tmp->pxNextFreeBlock = pxIterator->pxNextFreeBlock;
					pxPreviousBlock->pxNextFreeBlock = tmp;
				} else {
					pxPreviousBlock->pxNextFreeBlock = pxIterator->pxNextFreeBlock;
				}
				goto INSERTED;
			}
			if ((uint8_t*)pxIterator+pxIterator->xBlockSize == (uint8_t*)pxLink + presize) { // find pxLink at the end
				pxIterator->xBlockSize -= presize;
				goto INSERTED;
			}
			if ((uint8_t*)pxIterator+pxIterator->xBlockSize > (uint8_t*)pxLink + presize) { // find pxLink in the middle
				pxPreviousBlock = pxIterator;
				pxIterator = (BlockLink_t *)((uint8_t*)pxLink + presize);
				pxIterator->xBlockSize = ((uint8_t*)pxPreviousBlock+pxPreviousBlock->xBlockSize)-
					((uint8_t*)pxLink + presize);
				tmp = pxPreviousBlock->pxNextFreeBlock;
				pxPreviousBlock->pxNextFreeBlock = pxIterator;
				pxPreviousBlock->xBlockSize = (uint8_t*)pxLink - (uint8_t*)pxPreviousBlock;
				pxIterator->pxNextFreeBlock = tmp;
				goto INSERTED;
			}
			pxPreviousBlock = pxIterator;
			pxIterator = pxIterator->pxNextFreeBlock;
		}

INSERTED:
		pxLink->xBlockSize = presize|xBlockAllocatedBit;;
		pxLink->pxNextFreeBlock = NULL;
		psram_xFreeBytesRemaining -= presize;
	}
	( void ) xTaskResumeAll();

	return pvReturn;
}
#endif

#if CONFIG_MEM_DEBUG
static inline void show_mem_info(BlockLink_t *pxLink)
{
#if CONFIG_MEM_DEBUG_FUNC_NAME && CONFIG_MEM_DEBUG_TASK_NAME
	BK_LOG_RAW("%-8d   0x%-8x   %-4d   %-5d   %-32s   %-16s\r\n",
		pxLink->allocTime, (u8*)pxLink + xHeapStructSize, pxLink->wantedSize,
		pxLink->line, pxLink->funcName, pxLink->taskName);
#elif  CONFIG_MEM_DEBUG_FUNC_NAME
	BK_LOG_RAW("%-8d   0x%-8x   %-4d   %-5d   %-32s\r\n",
		pxLink->allocTime, (u8*)pxLink + xHeapStructSize, pxLink->wantedSize,
		pxLink->line, pxLink->funcName);
#elif CONFIG_MEM_DEBUG_TASK_NAME
	BK_LOG_RAW("%-8d   0x%-8x   %-4d   %-5d   %-16s\r\n",
		pxLink->allocTime, (u8*)pxLink + xHeapStructSize, pxLink->wantedSize,
		pxLink->line, pxLink->taskName);
#else
	BK_LOG_RAW("%-8d   0x%-8x   %-4d   %-5d\r\n",
		pxLink->allocTime, (u8*)pxLink + xHeapStructSize, pxLink->wantedSize,
		pxLink->line);
#endif
}

static inline void mem_overflow_check(BlockLink_t *pxLink)
{
	uint8_t *mem_end;

	mem_end = (uint8_t *)pxLink + (pxLink->xBlockSize & ~xBlockAllocatedBit) - MEM_CHECK_TAG_LEN;
	
	if (MEM_OVERFLOW_TAG != mem_end[0]
		|| MEM_OVERFLOW_TAG != mem_end[1]
		|| MEM_OVERFLOW_TAG != mem_end[2] 
		|| MEM_OVERFLOW_TAG != mem_end[3])
	{
		BK_LOG_RAW("Mem Overflow ............\r\n");
		show_mem_info(pxLink);
		configASSERT( false );
	}
}
#endif

/*-----------------------------------------------------------*/

static void *malloc_without_lock( size_t xWantedSize )
{
	BlockLink_t *pxBlock, *pxPreviousBlock, *pxNewBlockLink;
	void *pvReturn = NULL;

	{
		/* If this is the first call to malloc then the heap will require
		initialisation to setup the list of free blocks. */
		if( pxEnd == NULL )
		{
			prvHeapInit();
		}
		else
		{
			mtCOVERAGE_TEST_MARKER();
		}

		/* Check the requested block size is not so large that the top bit is
		set.  The top bit of the block size member of the BlockLink_t structure
		is used to determine who owns the block - the application or the
		kernel, so it must be free. */
		if( ( xWantedSize & xBlockAllocatedBit ) == 0 )
		{
			/* The wanted size is increased so it can contain a BlockLink_t
			structure in addition to the requested amount of bytes. */
			if( xWantedSize > 0 )
			{
				xWantedSize += xHeapStructSize;

				/* Ensure that blocks are always aligned to the required number
				of bytes. */
				if( ( xWantedSize & portBYTE_ALIGNMENT_MASK ) != 0x00 )
				{
					/* Byte alignment required. */
					xWantedSize += ( portBYTE_ALIGNMENT - ( xWantedSize & portBYTE_ALIGNMENT_MASK ) );
					configASSERT( ( xWantedSize & portBYTE_ALIGNMENT_MASK ) == 0 );
				}
				else
				{
					mtCOVERAGE_TEST_MARKER();
				}
			}
			else
			{
				mtCOVERAGE_TEST_MARKER();
			}

			if( ( xWantedSize > 0 ) && ( xWantedSize <= xFreeBytesRemaining ) )
			{
				/* Traverse the list from the start	(lowest address) block until
				one	of adequate size is found. */
				pxPreviousBlock = &xStart;
				pxBlock = xStart.pxNextFreeBlock;
				while( ( pxBlock->xBlockSize < xWantedSize ) && ( pxBlock->pxNextFreeBlock != NULL ) )
				{
					pxPreviousBlock = pxBlock;
					pxBlock = pxBlock->pxNextFreeBlock;
				}

				/* If the end marker was reached then a block of adequate size
				was	not found. */
				if( pxBlock != pxEnd )
				{
					/* Return the memory space pointed to - jumping over the
					BlockLink_t structure at its start. */
					pvReturn = ( void * ) ( ( ( uint8_t * ) pxPreviousBlock->pxNextFreeBlock ) + xHeapStructSize );

					/* This block is being returned for use so must be taken out
					of the list of free blocks. */
					pxPreviousBlock->pxNextFreeBlock = pxBlock->pxNextFreeBlock;

					/* If the block is larger than required it can be split into
					two. */
					if( ( pxBlock->xBlockSize - xWantedSize ) > heapMINIMUM_BLOCK_SIZE )
					{
						/* This block is to be split into two.  Create a new
						block following the number of bytes requested. The void
						cast is used to prevent byte alignment warnings from the
						compiler. */
						pxNewBlockLink = ( void * ) ( ( ( uint8_t * ) pxBlock ) + xWantedSize );
						configASSERT( ( ( ( size_t ) pxNewBlockLink ) & portBYTE_ALIGNMENT_MASK ) == 0 );

						/* Calculate the sizes of two blocks split from the
						single block. */
						pxNewBlockLink->xBlockSize = pxBlock->xBlockSize - xWantedSize;
						pxBlock->xBlockSize = xWantedSize;

						/* Insert the new block into the list of free blocks. */
						prvInsertBlockIntoFreeList( pxNewBlockLink );
					}
					else
					{
						mtCOVERAGE_TEST_MARKER();
					}

					xFreeBytesRemaining -= pxBlock->xBlockSize;

					if( xFreeBytesRemaining < xMinimumEverFreeBytesRemaining )
					{
						xMinimumEverFreeBytesRemaining = xFreeBytesRemaining;
					}
					else
					{
						mtCOVERAGE_TEST_MARKER();
					}

					/* The block is being returned - it is allocated and owned
					by the application and has no "next" block. */
					pxBlock->xBlockSize |= xBlockAllocatedBit;
					pxBlock->pxNextFreeBlock = NULL;

#if CONFIG_MEM_DEBUG
					list_add_tail(&pxBlock->node, &xUsed);
#endif

				}
				else
				{
					mtCOVERAGE_TEST_MARKER();
				}
			}
			else
			{
				mtCOVERAGE_TEST_MARKER();
			}
		}
		else
		{
			mtCOVERAGE_TEST_MARKER();
		}

		traceMALLOC( pvReturn, xWantedSize );
	}

	#if( configUSE_MALLOC_FAILED_HOOK == 1 )
	{
		if( pvReturn == NULL )
		{
			extern void vApplicationMallocFailedHook( void );
			vApplicationMallocFailedHook();
		}
		else
		{
			mtCOVERAGE_TEST_MARKER();
		}
	}
	#endif

	configASSERT( ( ( ( size_t ) pvReturn ) & ( size_t ) portBYTE_ALIGNMENT_MASK ) == 0 );
	return pvReturn;
}

#if CONFIG_MALLOC_STATIS || CONFIG_MEM_DEBUG
void *pvPortMalloc_cm(const char *call_func_name, int line, size_t xWantedSize, int need_zero )
#else
void *pvPortMalloc( size_t xWantedSize )
#endif
{
	void *pvReturn = NULL;
#if CONFIG_MEM_DEBUG
	uint8_t *mem_end;
#endif

	if (xWantedSize == 0)
		xWantedSize = 4;

	vTaskSuspendAll();
	pvReturn = malloc_without_lock(xWantedSize + MEM_CHECK_TAG_LEN);
	#if CONFIG_MALLOC_STATIS || CONFIG_MEM_DEBUG
	{
		BlockLink_t *pxLink = (BlockLink_t *)((u8*)pvReturn - xHeapStructSize);
		if(pvReturn && call_func_name) {
#if CONFIG_MALLOC_STATIS
			BK_LOGI(TAG, "m:%p,%d|%s,%d\r\n", pxLink, (pxLink->xBlockSize & ~xBlockAllocatedBit), call_func_name, line);
#endif
		}
#if CONFIG_MEM_DEBUG
		pxLink->allocTime = xTaskGetTickCount();
#if CONFIG_MEM_DEBUG_FUNC_NAME
		os_strlcpy(pxLink->funcName, call_func_name, sizeof(pxLink->funcName) - 1);
#endif
#if CONFIG_MEM_DEBUG_TASK_NAME
		//malloc can only be called in Task context!
		if (rtos_is_scheduler_started())
			os_strlcpy(pxLink->taskName, pcTaskGetName(NULL), sizeof(pxLink->taskName) - 1);
		else
			os_strlcpy(pxLink->taskName, "NA", sizeof(pxLink->taskName) - 1);
#endif
		pxLink->line = line;
		pxLink->wantedSize = xWantedSize;

		mem_end = (uint8_t *)pxLink + (pxLink->xBlockSize & ~xBlockAllocatedBit) - MEM_CHECK_TAG_LEN;
 		os_memset(mem_end, MEM_OVERFLOW_TAG, MEM_CHECK_TAG_LEN);
#endif
	}
	#endif
	( void ) xTaskResumeAll();

	#if CONFIG_MALLOC_STATIS || CONFIG_MEM_DEBUG
	if(pvReturn && need_zero)
		os_memset(pvReturn, 0, xWantedSize);
	
	#endif
	return pvReturn;
}

/*-----------------------------------------------------------*/
#if CONFIG_MALLOC_STATIS || CONFIG_MEM_DEBUG
void *vPortFree_cm(const char *call_func_name, int line, void *pv )
#else
void vPortFree( void *pv )
#endif
{
	uint8_t *puc = ( uint8_t * ) pv;
	BlockLink_t *pxLink;

	if( pv != NULL )
	{
		/* The memory being freed will have an BlockLink_t structure immediately
		before it. */
		puc -= xHeapStructSize;

		/* This casting is to keep the compiler from issuing warnings. */
		pxLink = ( void * ) puc;

		/* Check the block is actually allocated. */
		configASSERT( ( pxLink->xBlockSize & xBlockAllocatedBit ) != 0 );
		configASSERT( pxLink->pxNextFreeBlock == NULL );
#if CONFIG_MEM_DEBUG
		mem_overflow_check(pxLink);
#endif

		if( ( pxLink->xBlockSize & xBlockAllocatedBit ) != 0 )
		{
			if( pxLink->pxNextFreeBlock == NULL )
			{
				/* The block is being returned to the heap - it is no longer
				allocated. */
				pxLink->xBlockSize &= ~xBlockAllocatedBit;

				vTaskSuspendAll();
#if CONFIG_MALLOC_STATIS
                if (call_func_name)
                {
                    BK_LOGI(TAG, "f:%p,%d|%s,%d\r\n", pxLink, pxLink->xBlockSize, call_func_name, line);
                }
#endif
#if CONFIG_MEM_DEBUG
				list_del(&pxLink->node);
				pxLink->allocTime = 0;
#if CONFIG_MEM_DEBUG_FUNC_NAME
				pxLink->funcName[0] = 0;
#endif
#if CONFIG_MEM_DEBUG_TASK_NAME
				pxLink->taskName[0] = 0;
#endif
				pxLink->line = 0;
#endif
#if (CONFIG_SOC_BK7251)
				if (puc > psram_ucHeap)
                {
                    /* Add this block to the list of psram free blocks. */
                    psram_xFreeBytesRemaining += pxLink->xBlockSize;
                    traceFREE( pv, pxLink->xBlockSize );
                    psram_prvInsertBlockIntoFreeList( ( ( BlockLink_t * ) pxLink ) );
                }
                else
#endif
				{
					/* Add this block to the list of ram free blocks. */
					xFreeBytesRemaining += pxLink->xBlockSize;
					traceFREE( pv, pxLink->xBlockSize );
					prvInsertBlockIntoFreeList( ( ( BlockLink_t * ) pxLink ) );
				}
				( void ) xTaskResumeAll();
			}
			else
			{
				mtCOVERAGE_TEST_MARKER();
			}
		}
		else
		{
			mtCOVERAGE_TEST_MARKER();
		}
	}

#if CONFIG_MALLOC_STATIS || CONFIG_MEM_DEBUG
	return NULL;
#endif
}

#if CONFIG_MALLOC_STATIS || CONFIG_MEM_DEBUG
void xPortDumpMemStats(uint32_t start_tick, uint32_t ticks_since_malloc, const char* task)
{
	BlockLink_t *pxLink;

	BK_LOG_RAW("%-8s   %-10s   %-4s   %-5s", "tick", "addr", "size", "line");

#if CONFIG_MEM_DEBUG_FUNC_NAME
	BK_LOG_RAW("   %-32s", "func");
#endif

#if CONFIG_MEM_DEBUG_TASK_NAME
	BK_LOG_RAW("   %-16s", "task");
#endif
	BK_LOG_RAW("\n");

	BK_LOG_RAW("%-8s   %-10s   %-4s   %-5s", "--------", "----------", "----", "-----");

#if CONFIG_MEM_DEBUG_FUNC_NAME
	BK_LOG_RAW("   %-32s", "--------------------------------");
#endif

#if CONFIG_MEM_DEBUG_TASK_NAME
	BK_LOG_RAW("   %-16s", "----------------");
#endif
	BK_LOG_RAW("\n");

	vTaskSuspendAll();

	list_for_each_entry(pxLink, &xUsed, node) {
		
		if (pxLink->allocTime < start_tick)
			continue;

		if ((pxLink->allocTime - start_tick) < ticks_since_malloc)
			continue;

#if CONFIG_MEM_DEBUG_TASK_NAME
		if (task && os_strncmp(task, pxLink->taskName, sizeof(pxLink->taskName)))
			continue;
#endif

		show_mem_info(pxLink);

		mem_overflow_check(pxLink);
	}

	xTaskResumeAll();
}
#endif

/*-----------------------------------------------------------*/

size_t xPortGetFreeHeapSize( void )
{
#if (CONFIG_SOC_BK7251)
    return xFreeBytesRemaining + psram_xFreeBytesRemaining;
#else
	return xFreeBytesRemaining;
#endif
}
/*-----------------------------------------------------------*/

size_t xPortGetMinimumEverFreeHeapSize( void )
{
#if (CONFIG_SOC_BK7251)
    return xMinimumEverFreeBytesRemaining + psram_xMinimumEverFreeBytesRemaining;
#else
	return xMinimumEverFreeBytesRemaining;
#endif
}
/*-----------------------------------------------------------*/

void vPortInitialiseBlocks( void )
{
	/* This just exists to keep the linker quiet. */
}
/*-----------------------------------------------------------*/

#if configDYNAMIC_HEAP_SIZE
#if (CONFIG_SOC_BK7256XX) || (CONFIG_SOC_BK7256_CP1)
#define HEAP_START_ADDRESS    (void*)(&ucHeap)
#else
extern unsigned char _empty_ram;
#define HEAP_START_ADDRESS    (void*)&_empty_ram
#endif

#if (CONFIG_SOC_BK7231N)
#define HEAP_END_ADDRESS      (void*)(0x00400000 + 192 * 1024)
#elif (CONFIG_SOC_BK7236)
// TBD: mem size for bk7236
#define HEAP_END_ADDRESS      (void*)(0x00400000 + 192 * 1024)
#elif (CONFIG_SOC_BK7271)
#define HEAP_END_ADDRESS      (void*)(0x00400000 + 512 * 1024)
#elif (CONFIG_SOC_BK7256XX)
#define HEAP_END_ADDRESS      (void*)(HEAP_START_ADDRESS  + configTOTAL_HEAP_SIZE)
#elif (CONFIG_SOC_BK7256_CP1)
#define HEAP_END_ADDRESS      (void*)(HEAP_START_ADDRESS  + configTOTAL_HEAP_SIZE)
#else
#define HEAP_END_ADDRESS      (void*)(0x00400000 + 256 * 1024)
#endif

static void *prvHeapGetHeaderPointer(void)
{
	return (void *)HEAP_START_ADDRESS;
}

uint32_t prvHeapGetTotalSize(void)
{
	configASSERT(HEAP_END_ADDRESS > HEAP_START_ADDRESS);
	return (HEAP_END_ADDRESS - HEAP_START_ADDRESS);
}
#else
uint32_t prvHeapGetTotalSize(void)
{
	return configTOTAL_HEAP_SIZE;
}
#endif

static void prvHeapInit( void )
{
	BlockLink_t *pxFirstFreeBlock;
	uint8_t *pucAlignedHeap;
	size_t uxAddress;
	size_t xTotalHeapSize;

	#if configDYNAMIC_HEAP_SIZE
	xTotalHeapSize = prvHeapGetTotalSize();
	ucHeap = prvHeapGetHeaderPointer();
	#else
	xTotalHeapSize = configTOTAL_HEAP_SIZE;
	#endif


	/* Ensure the heap starts on a correctly aligned boundary. */
	uxAddress = ( size_t ) ucHeap;

	if( ( uxAddress & portBYTE_ALIGNMENT_MASK ) != 0 )
	{
		uxAddress += ( portBYTE_ALIGNMENT - 1 );
		uxAddress &= ~( ( size_t ) portBYTE_ALIGNMENT_MASK );
		xTotalHeapSize -= uxAddress - ( size_t ) ucHeap;
	}

	pucAlignedHeap = ( uint8_t * ) uxAddress;

	/* xStart is used to hold a pointer to the first item in the list of free
	blocks.  The void cast is used to prevent compiler warnings. */
	xStart.pxNextFreeBlock = ( void * ) pucAlignedHeap;
	xStart.xBlockSize = ( size_t ) 0;

	/* pxEnd is used to mark the end of the list of free blocks and is inserted
	at the end of the heap space. */
	uxAddress = ( ( size_t ) pucAlignedHeap ) + xTotalHeapSize;
	uxAddress -= xHeapStructSize;
	uxAddress &= ~( ( size_t ) portBYTE_ALIGNMENT_MASK );
	pxEnd = ( void * ) uxAddress;
	pxEnd->xBlockSize = 0;
	pxEnd->pxNextFreeBlock = NULL;

	/* To start with there is a single free block that is sized to take up the
	entire heap space, minus the space taken by pxEnd. */
	pxFirstFreeBlock = ( void * ) pucAlignedHeap;
	pxFirstFreeBlock->xBlockSize = uxAddress - ( size_t ) pxFirstFreeBlock;
	pxFirstFreeBlock->pxNextFreeBlock = pxEnd;

	/* Only one block exists - and it covers the entire usable heap space. */
	xMinimumEverFreeBytesRemaining = pxFirstFreeBlock->xBlockSize;
	xFreeBytesRemaining = pxFirstFreeBlock->xBlockSize;

#if (CONFIG_SOC_BK7251)
    xTotalHeapSize = PSRAM_END_ADDRESS - PSRAM_START_ADDRESS;
    psram_ucHeap = PSRAM_START_ADDRESS;

    BK_LOGI(TAG, "prvHeapInit-start addr:0x%x, size:%d\r\n", psram_ucHeap, xTotalHeapSize);

    /* Ensure the heap starts on a correctly aligned boundary. */
    uxAddress = ( size_t ) psram_ucHeap;

    if( ( uxAddress & portBYTE_ALIGNMENT_MASK ) != 0 )
    {
        uxAddress += ( portBYTE_ALIGNMENT - 1 );
        uxAddress &= ~( ( size_t ) portBYTE_ALIGNMENT_MASK );
        xTotalHeapSize -= uxAddress - ( size_t ) psram_ucHeap;
    }

    pucAlignedHeap = ( uint8_t * ) uxAddress;

    /* psram_xStart is used to hold a pointer to the first item in the list of free
    blocks.  The void cast is used to prevent compiler warnings. */
    psram_xStart.pxNextFreeBlock = ( void * ) pucAlignedHeap;
    psram_xStart.xBlockSize = ( size_t ) 0;

    /* psram_pxEnd is used to mark the end of the list of free blocks and is inserted
    at the end of the heap space. */
    uxAddress = ( ( size_t ) pucAlignedHeap ) + xTotalHeapSize;
    uxAddress -= xHeapStructSize;
    uxAddress &= ~( ( size_t ) portBYTE_ALIGNMENT_MASK );
    psram_pxEnd = ( void * ) uxAddress;
    psram_pxEnd->xBlockSize = 0;
    psram_pxEnd->pxNextFreeBlock = NULL;

    /* To start with there is a single free block that is sized to take up the
    entire heap space, minus the space taken by psram_pxEnd. */
    pxFirstFreeBlock = ( void * ) pucAlignedHeap;
    pxFirstFreeBlock->xBlockSize = uxAddress - ( size_t ) pxFirstFreeBlock;
    pxFirstFreeBlock->pxNextFreeBlock = psram_pxEnd;

    /* Only one block exists - and it covers the entire usable heap space. */
    psram_xMinimumEverFreeBytesRemaining = pxFirstFreeBlock->xBlockSize;
    psram_xFreeBytesRemaining = pxFirstFreeBlock->xBlockSize;
#endif

#if CONFIG_MEM_DEBUG
	INIT_LIST_HEAD(&xUsed);
#endif

	/* Work out the position of the top bit in a size_t variable. */
	xBlockAllocatedBit = ( ( size_t ) 1 ) << ( ( sizeof( size_t ) * heapBITS_PER_BYTE ) - 1 );
}
/*-----------------------------------------------------------*/

static void prvInsertBlockIntoFreeList( BlockLink_t *pxBlockToInsert )
{
BlockLink_t *pxIterator;
uint8_t *puc;

	/* Iterate through the list until a block is found that has a higher address
	than the block being inserted. */
	for( pxIterator = &xStart; pxIterator->pxNextFreeBlock < pxBlockToInsert; pxIterator = pxIterator->pxNextFreeBlock )
	{
		/* Nothing to do here, just iterate to the right position. */
	}

	/* Do the block being inserted, and the block it is being inserted after
	make a contiguous block of memory? */
	puc = ( uint8_t * ) pxIterator;
	if( ( puc + pxIterator->xBlockSize ) == ( uint8_t * ) pxBlockToInsert )
	{
		pxIterator->xBlockSize += pxBlockToInsert->xBlockSize;
		pxBlockToInsert = pxIterator;
	}
	else
	{
		mtCOVERAGE_TEST_MARKER();
	}

	/* Do the block being inserted, and the block it is being inserted before
	make a contiguous block of memory? */
	puc = ( uint8_t * ) pxBlockToInsert;
	if( ( puc + pxBlockToInsert->xBlockSize ) == ( uint8_t * ) pxIterator->pxNextFreeBlock )
	{
		if( pxIterator->pxNextFreeBlock != pxEnd )
		{
			/* Form one big block from the two blocks. */
			pxBlockToInsert->xBlockSize += pxIterator->pxNextFreeBlock->xBlockSize;
			pxBlockToInsert->pxNextFreeBlock = pxIterator->pxNextFreeBlock->pxNextFreeBlock;
		}
		else
		{
			pxBlockToInsert->pxNextFreeBlock = pxEnd;
		}
	}
	else
	{
		pxBlockToInsert->pxNextFreeBlock = pxIterator->pxNextFreeBlock;
	}

	/* If the block being inserted plugged a gab, so was merged with the block
	before and the block after, then it's pxNextFreeBlock pointer will have
	already been set, and should not be set here as that would make it point
	to itself. */
	if( pxIterator != pxBlockToInsert )
	{
		pxIterator->pxNextFreeBlock = pxBlockToInsert;
	}
	else
	{
		mtCOVERAGE_TEST_MARKER();
	}
}

/*yhb added 2015-08-13. wzl added:this function has an issue*/
void *pvPortRealloc( void *pv, size_t xWantedSize )
{
	uint8_t *puc = ( uint8_t * ) pv;
	BlockLink_t *pxLink;
	int presize, datasize;
	void *pvReturn = NULL;
	BlockLink_t *pxIterator, *pxPreviousBlock, *tmp;

#if (CONFIG_SOC_BK7251)
    if (puc > psram_ucHeap)
    {
        return psram_realloc(pv, xWantedSize);
    }
	if (pv == NULL)
	{
		return psram_malloc(xWantedSize);
	}
#else
	if (pv == NULL)
		return pvPortMalloc(xWantedSize);
#endif

	/* The memory being freed will have an BlockLink_t structure immediately
	before it. */
	puc -= xHeapStructSize;
	/* This casting is to keep the compiler from issuing warnings. */
	pxLink = ( void * ) puc;

	presize = (pxLink->xBlockSize & ~xBlockAllocatedBit);
	datasize = presize - xHeapStructSize;
	if (datasize >= xWantedSize) // have enough memory don't need realloc
		return pv;

	pxLink->xBlockSize &= ~xBlockAllocatedBit;
	vTaskSuspendAll();
	/* Add this block to the list of free blocks. */
	xFreeBytesRemaining += pxLink->xBlockSize;
	prvInsertBlockIntoFreeList( ( ( BlockLink_t * ) pxLink ) );
	pvReturn = malloc_without_lock(xWantedSize);
	if (pvReturn != NULL) {
		if (pvReturn != pv)
			os_memcpy(pvReturn, pv, datasize);
	} else { // if can't realloc such big memory, we should NOT put pv in free list.
		pxPreviousBlock = &xStart;
		pxIterator = xStart.pxNextFreeBlock;
		while( pxIterator != NULL )
		{
			if (pxIterator > pxLink) {
				break;
			}
			if (pxIterator == pxLink) {// find pxLink at the begin
				if (pxIterator->xBlockSize > presize) {
					tmp = (BlockLink_t *)((uint8_t*)pxLink + presize);
					tmp->xBlockSize = (pxIterator->xBlockSize - presize);
					tmp->pxNextFreeBlock = pxIterator->pxNextFreeBlock;
					pxPreviousBlock->pxNextFreeBlock = tmp;
				} else {
					pxPreviousBlock->pxNextFreeBlock = pxIterator->pxNextFreeBlock;
				}
				goto INSERTED;
			}
			if ((uint8_t*)pxIterator+pxIterator->xBlockSize == (uint8_t*)pxLink + presize) { // find pxLink at the end
				pxIterator->xBlockSize -= presize;
				goto INSERTED;
			}
			if ((uint8_t*)pxIterator+pxIterator->xBlockSize > (uint8_t*)pxLink + presize) { // find pxLink in the middle
				pxPreviousBlock = pxIterator;
				pxIterator = (BlockLink_t *)((uint8_t*)pxLink + presize);
				pxIterator->xBlockSize = ((uint8_t*)pxPreviousBlock+pxPreviousBlock->xBlockSize)-
					((uint8_t*)pxLink + presize);
				tmp = pxPreviousBlock->pxNextFreeBlock;
				pxPreviousBlock->pxNextFreeBlock = pxIterator;
				pxPreviousBlock->xBlockSize = (uint8_t*)pxLink - (uint8_t*)pxPreviousBlock;
				pxIterator->pxNextFreeBlock = tmp;
				goto INSERTED;
			}
			pxPreviousBlock = pxIterator;
			pxIterator = pxIterator->pxNextFreeBlock;
		}

INSERTED:
		pxLink->xBlockSize = presize|xBlockAllocatedBit;;
		pxLink->pxNextFreeBlock = NULL;
		xFreeBytesRemaining -= presize;
	}
	( void ) xTaskResumeAll();

	return pvReturn;
}

// When bk_printf depends on heap initialization since bk_printf needs to init the mutex
// and mutex initialization needs to allocate memory from heap. So when heap initialization
// is called, the bk_printf is not ready, so define a special API for displaying heap
// configuration info!
//
// TODO - after we support bk_eary_printf() API, we can remove this API.
//
#define RAM_START_ADDRESS 0x400000

#define DATA_START_ADDRESS &_data_ram_begin
#define DATA_END_ADDRESS &_data_ram_end
#define BSS_START_ADDRESS &_bss_start
#define BSS_END_ADDRESS &_bss_end
extern unsigned char _bss_start, _bss_end, _data_ram_begin, _data_ram_end;
void pvShowMemoryConfigInfo(void)
{
#if CONFIG_SOC_BK7256XX || CONFIG_SOC_BK7256_CP1
#else
#if configDYNAMIC_HEAP_SIZE
	BK_LOGI(TAG, "\n");
	BK_LOGI(TAG, "%-8s %-8s %-8s %-8s\n", "mem_type", "start", "end", "size");
	BK_LOGI(TAG, "%-8s %-8s %-8s %-8s\n", "--------", "--------", "--------", "--------");
	BK_LOGI(TAG, "%-8s 0x%-6x 0x%-6x %-8d\r\n", "ram", RAM_START_ADDRESS, HEAP_END_ADDRESS, (HEAP_END_ADDRESS - RAM_START_ADDRESS));
	BK_LOGI(TAG, "%-8s 0x%-6x 0x%-6x %-8d\r\n", "non_heap", RAM_START_ADDRESS, HEAP_START_ADDRESS, (HEAP_START_ADDRESS - RAM_START_ADDRESS));
	BK_LOGI(TAG, "%-8s 0x%-6x 0x%-6x %-8d\r\n", "data", DATA_START_ADDRESS, DATA_END_ADDRESS, (DATA_END_ADDRESS - DATA_START_ADDRESS));
	BK_LOGI(TAG, "%-8s 0x%-6x 0x%-6x %-8d\r\n", "bss", BSS_START_ADDRESS, BSS_END_ADDRESS, (BSS_END_ADDRESS - BSS_START_ADDRESS));
	BK_LOGI(TAG, "%-8s 0x%-6x 0x%-6x %-8d\r\n", "heap", HEAP_START_ADDRESS, HEAP_END_ADDRESS, (HEAP_END_ADDRESS - HEAP_START_ADDRESS));
#if (CONFIG_SOC_BK7251)
	BK_LOGI(TAG, "%-8s 0x%-6x 0x%-6x %-8d\r\n", "psram", PSRAM_START_ADDRESS, PSRAM_END_ADDRESS, (PSRAM_END_ADDRESS - PSRAM_END_ADDRESS));
#endif
#endif
#endif
}
