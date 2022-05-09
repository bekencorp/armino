#ifndef __SPIDMA_INTF_H__
#define __SPIDMA_INTF_H__

#if CONFIG_HSLAVE_SPI

#if (CONFIG_SOC_BK7231)
typedef struct spidma_desc {
	UINT8 *rxbuf;
	UINT8 *txbuf;
	UINT16 rxbuf_len;
	UINT16 txbuf_len;
	void (*rx_handler)(void *curptr, UINT32 newlen);
	void (*rx_timeout)(void);
	void (*tx_handler)(void);
	/* mode:     SPIDMA mode
	 * bit[0]: SPIDMA sck clock invert
	 *          0:  posedge sample data
	 *          1:  negedge sample data
	 * bit[1]: SPIDMA LSB transfer first
	 *          0:  MSB transfer first
	 *          1:  LSB transfer first
	 * bit[2]: SPIDMA 3-wire mode
	 *          0:  4-wire mode
	 *          1:  3-wire mode
	 * bit[3:19]: 16bit SPIDMA rxbuf threshold val
	 */
	UINT32 mode;
	UINT32 timeout_val;
	UINT32 node_len;
} SPIDMA_DESC_ST, *SPIDMA_DESC_PTR;
#else
typedef struct spidma_desc {
	UINT8 *rxbuf;

	void (*node_full_handler)(void *curptr, UINT32 newlen, UINT32 is_eof, UINT32 frame_len);
	void (*data_end_handler)(void);

	UINT32 rxbuf_len;
	UINT32 rx_read_len;
	UINT32 node_len;


	UINT8 *txbuf;
	void (*tx_handler)(void);

	/* mode:     SPIDMA mode
	 * bit[0]: SPIDMA sck clock invert
	 *          0:  posedge sample data
	 *          1:  negedge sample data
	 * bit[1]: SPIDMA LSB transfer first
	 *          0:  MSB transfer first
	 *          1:  LSB transfer first
	 * bit[2]: SPIDMA 3-wire mode
	 *          0:  4-wire mode
	 *          1:  3-wire mode
	 * bit[3:19]: 16bit SPIDMA rxbuf threshold val
	 */
	UINT32 mode;
	UINT32 timeout_val;
	UINT32 txbuf_len;
	void (*end_frame_handler)(void);
#if CONFIG_GENERAL_DMA
	void (*dma_rx_handler)(UINT32);
	UINT32 dma_rx_channel;
	void (*dma_tx_handler)(UINT32);
	UINT32 dma_tx_channel;
#endif
} SPIDMA_DESC_ST, *SPIDMA_DESC_PTR;
#endif

#endif

#endif

