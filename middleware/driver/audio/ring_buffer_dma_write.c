#include <common/bk_include.h>
#include "ring_buffer_dma_write.h"
#include "bk_arch.h"
#include <os/mem.h>
#include "bk_drv_model.h"
#include "bk_uart.h"

#if CONFIG_GENERAL_DMA
#include "bk_general_dma.h"
#include <driver/dma.h>

#define RWP_SAFE_INTERVAL           (4)
#define RB_DMA_WR_MEMCPY                   os_memcpy
#define RB_DMA_WR_INT_DECLARATION()
#define RB_DMA_WR_INT_DISABLE()
#define RB_DMA_WR_INT_RESTORE()
#define RB_DMA_WR_PRT(...)                 //AUDIO_LOGI

void rb_init_dma_write(RB_DMA_WR_PTR rb, UINT8 *addr, UINT32 capacity, UINT32 ch)
{
	RB_DMA_WR_INT_DECLARATION();

	RB_DMA_WR_INT_DISABLE();
	rb->address  = addr;
	rb->capacity = capacity;
	rb->wp       = 0;
	rb->rp       = 0;
	rb->dma_ch   = ch;
	RB_DMA_WR_INT_RESTORE();

	uint32_t addr_param = (uint32_t)(rb->address + rb->capacity - RWP_SAFE_INTERVAL);
	RB_DMA_WR_PRT("init set dst:%x\r\n", addr_param);
	dma_set_dst_pause_addr(rb->dma_ch, addr_param);
}

void rb_clear_dma_write(RB_DMA_WR_PTR rb)
{
	RB_DMA_WR_INT_DECLARATION();

	RB_DMA_WR_INT_DISABLE();
	rb->wp    = 0;
	rb->rp    = 0;
	RB_DMA_WR_INT_RESTORE();

	uint32_t addr_param = (uint32_t)(rb->address + rb->capacity - RWP_SAFE_INTERVAL);
	RB_DMA_WR_PRT("clear set dst:%x\r\n", addr_param);
	dma_set_dst_pause_addr(rb->dma_ch, addr_param);
}

UINT32 rb_read_dma_write(RB_DMA_WR_PTR rb, UINT8 *buffer, UINT32 size, UINT32 count)
{
	UINT32 required_bytes = size * count;
	UINT32 read_bytes;
	UINT32 remain_bytes;
	UINT32 wp;
	UINT32 addr_param;

	RB_DMA_WR_INT_DECLARATION();

	if (required_bytes == 0)
		return 0;

	wp = dma_get_dest_write_addr(rb->dma_ch);
	RB_DMA_WR_PRT("rb read get dst_wr:%x\r\n", wp);
	wp -= (UINT32)rb->address;
	rb->wp = wp;

	if (wp >= rb->rp) {
		remain_bytes = wp - rb->rp;

		if (required_bytes > remain_bytes)
			read_bytes = remain_bytes;
		else
			read_bytes = required_bytes;

		RB_DMA_WR_MEMCPY(buffer, &rb->address[rb->rp], read_bytes);
		RB_DMA_WR_INT_DISABLE();
		rb->rp += read_bytes;
		RB_DMA_WR_INT_RESTORE();
	} else {
		remain_bytes = rb->capacity - rb->rp;

		if (required_bytes > remain_bytes) {
			read_bytes = remain_bytes;
			RB_DMA_WR_MEMCPY(buffer, &rb->address[rb->rp], read_bytes);

			if (required_bytes - read_bytes > wp) {
				RB_DMA_WR_MEMCPY(buffer + read_bytes, &rb->address[0], wp);
				RB_DMA_WR_INT_DISABLE();
				rb->rp = wp;
				RB_DMA_WR_INT_RESTORE();
				read_bytes += wp;
			} else {
				RB_DMA_WR_MEMCPY(buffer + read_bytes, &rb->address[0], required_bytes - read_bytes);
				RB_DMA_WR_INT_DISABLE();
				rb->rp = required_bytes - read_bytes;
				RB_DMA_WR_INT_RESTORE();
				read_bytes = required_bytes;
			}
		} else {
			read_bytes = required_bytes;
			RB_DMA_WR_MEMCPY(buffer, &rb->address[rb->rp], read_bytes);
			RB_DMA_WR_INT_DISABLE();
			rb->rp += read_bytes;
			RB_DMA_WR_INT_RESTORE();
		}
	}

	if (rb->rp >= RWP_SAFE_INTERVAL)
		addr_param = (UINT32)(rb->address + rb->rp - RWP_SAFE_INTERVAL);
	else
		addr_param = (UINT32)(rb->address + rb->capacity + rb->rp - RWP_SAFE_INTERVAL);

	RB_DMA_WR_PRT("read set dst:%x\r\n", addr_param);
	dma_set_dst_pause_addr(rb->dma_ch, (uint32_t)addr_param);

	return read_bytes;
}

UINT32 rb_get_fill_size_dma_write(RB_DMA_WR_PTR rb)
{
	UINT32 fill_size, wp;

	wp = dma_get_dest_write_addr(rb->dma_ch);
	RB_DMA_WR_PRT("fillsize get dst_wr:%x\r\n", wp);
	wp -= (UINT32)rb->address;
	rb->wp = wp;

	fill_size = rb->wp >= rb->rp ? rb->wp - rb->rp : rb->capacity - rb->rp + rb->wp;

	return fill_size;
}

UINT32 rb_get_free_size_dma_write(RB_DMA_WR_PTR rb)
{
	UINT32 free_size, wp;

	wp = dma_get_dest_write_addr(rb->dma_ch);
	RB_DMA_WR_PRT("free size get dst_wr:%x\r\n", wp);
	wp -= (UINT32)rb->address;
	rb->wp = wp;

	free_size = rb->wp >= rb->rp ? rb->capacity - rb->wp + rb->rp : rb->rp - rb->wp;

	return free_size > RWP_SAFE_INTERVAL ? free_size - RWP_SAFE_INTERVAL : 0;
}

#endif // CONFIG_GENERAL_DMA
