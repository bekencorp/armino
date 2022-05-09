#include <common/bk_include.h>
#include "ring_buffer_dma_read.h"
#include "bk_arch.h"
#include <os/mem.h>
#include "bk_drv_model.h"
#include "bk_uart.h"

#if CONFIG_GENERAL_DMA
#include <driver/dma.h>
#include "bk_general_dma.h"

#define RWP_SAFE_INTERVAL           (4)

#define RB_DMA_RD_MEMCPY                   os_memcpy
#define RB_DMA_RD_INT_DECLARATION()
#define RB_DMA_RD_INT_DISABLE()
#define RB_DMA_RD_INT_RESTORE()
#define RB_DMA_RD_PRT(...)

void rb_init_dma_read(RB_DMA_RD_PTR rb, UINT8 *addr, UINT32 capacity, UINT32 ch)
{
	RB_DMA_RD_INT_DECLARATION();

	RB_DMA_RD_INT_DISABLE();
	rb->address  = addr;
	rb->capacity = capacity;
	rb->wp       = 0;
	rb->rp       = 0;
	rb->dma_ch   = ch;
	RB_DMA_RD_INT_RESTORE();

	// set pause in order to stop dma when enable dma bit
	RB_DMA_RD_PRT("init set src:%x\r\n", (uint32_t)rb->address);
	dma_set_src_pause_addr(rb->dma_ch, (uint32_t)rb->address);

	bk_dma_start(rb->dma_ch);
	// trigger dma bit in order to set dma read ptr to start of buffer
	bk_dma_stop(rb->dma_ch);
}

void rb_clear_dma_read(RB_DMA_RD_PTR rb)
{
	RB_DMA_RD_INT_DECLARATION();

	RB_DMA_RD_INT_DISABLE();
	rb->wp    = 0;
	rb->rp    = 0;
	RB_DMA_RD_INT_RESTORE();
}


UINT32 rb_write_dma_read(RB_DMA_RD_PTR rb, UINT8 *buffer, UINT32 size, UINT32 count)
{
	UINT32 remain_bytes;
	UINT32 write_bytes = size * count;
	UINT32 rp;

	RB_DMA_RD_INT_DECLARATION();

	if (write_bytes == 0)
		return 0;

	rp = dma_get_src_read_addr(rb->dma_ch);
	RB_DMA_RD_PRT("write get src_rd:%x\r\n", rp);
	rp -= (UINT32)rb->address;
	rb->rp = rp;

	if (rb->wp >= rp) {
		remain_bytes = rb->capacity - rb->wp + rp;

		if (remain_bytes >= write_bytes + RWP_SAFE_INTERVAL) {
			remain_bytes = rb->capacity - rb->wp;

			if (remain_bytes >= write_bytes) {
				RB_DMA_RD_MEMCPY(&rb->address[rb->wp], buffer, write_bytes);
				RB_DMA_RD_INT_DISABLE();
				rb->wp += write_bytes;
				RB_DMA_RD_INT_RESTORE();
			} else {
				RB_DMA_RD_MEMCPY(&rb->address[rb->wp], buffer, remain_bytes);
				RB_DMA_RD_INT_DISABLE();
				rb->wp = write_bytes - remain_bytes;
				RB_DMA_RD_INT_RESTORE();
				RB_DMA_RD_MEMCPY(&rb->address[0], &buffer[remain_bytes], rb->wp);
			}
		} else
			return 0;
	} else {
		remain_bytes = rp - rb->wp;

		if (remain_bytes >= write_bytes + RWP_SAFE_INTERVAL) {
			RB_DMA_RD_MEMCPY(&rb->address[rb->wp], buffer, write_bytes);
			RB_DMA_RD_INT_DISABLE();
			rb->wp += write_bytes;
			RB_DMA_RD_INT_RESTORE();
		} else
			return 0;
	}

	if (rb->wp >= rb->capacity && rb->rp) {
		RB_DMA_RD_INT_DISABLE();
		rb->wp = 0;
		RB_DMA_RD_INT_RESTORE();
	}

	RB_DMA_RD_PRT("write set src:%x\r\n", (uint32_t)(rb->address + rb->wp));
	dma_set_src_pause_addr(rb->dma_ch, (uint32_t)(rb->address + rb->wp));

	return write_bytes;
}

UINT32 rb_get_fill_size_dma_read(RB_DMA_RD_PTR rb)
{
	UINT32 fill_size, rp;

	rp = dma_get_src_read_addr(rb->dma_ch);
	RB_DMA_RD_PRT("fillsize get src_rd:%x\r\n", rp);
	rp -= (UINT32)rb->address;
	rb->rp = rp;

	fill_size = rb->wp >= rb->rp ? rb->wp - rb->rp : rb->capacity - rb->rp + rb->wp;

	return fill_size;
}

UINT32 rb_get_free_size_dma_read(RB_DMA_RD_PTR rb)
{
	UINT32 free_size, rp;

	rp = dma_get_src_read_addr(rb->dma_ch);
	RB_DMA_RD_PRT("freesize get src_rd:%x\r\n", rp);
	rp -= (UINT32)rb->address;
	rb->rp = rp;

	free_size = rb->wp >= rb->rp ? rb->capacity - rb->wp + rb->rp : rb->rp - rb->wp;

	return free_size > RWP_SAFE_INTERVAL ? free_size - RWP_SAFE_INTERVAL : 0;
}

#endif // CONFIG_GENERAL_DMA
