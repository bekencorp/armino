#include <common/bk_include.h>
#include "bk_arm_arch.h"

#if CONFIG_SPIDMA
#include <components/video_transfer.h>
#include "bk_gpio.h"

#include <driver/spi.h>
#include <components/spidma.h>

#include "spidma_types.h"

#include "bk_drv_model.h"
#include <os/mem.h>
#if CONFIG_GENERAL_DMA
#include <driver/dma.h>
#endif

#define SPIDMA_INTF_DEBUG           1
#include "bk_uart.h"
#if SPIDMA_INTF_DEBUG
#define SPIDMA_INTF_PRT             os_printf
#define SPIDMA_INTF_WPRT            warning_prf
#define SPIDMA_INTF_FATAL           fatal_prf
#else
#define SPIDMA_INTF_PRT             null_prf
#define SPIDMA_INTF_WPRT            null_prf
#define SPIDMA_INTF_FATAL           null_prf
#endif

#define CAMERA_REV180_GPIO_INDEX    GPIO17

#define RX_TIMEOUT_30US             30
#define RX_TIMEOUT_500US            500
#define FPGA_MAIN_CLK               120
#define SPIDMA_RXDATA_TIMEOUT       (FPGA_MAIN_CLK * RX_TIMEOUT_500US)

#ifndef SPIDMA_RXDATA_TIMEOUT
#define SPIDMA_RXDATA_TIMEOUT       SPIDMA_DEF_RXDATA_TIMEOUT_VAL
#endif

#define SPIDMA_CHNAL                DMA_ID_5

DD_HANDLE spidma_hdl = DD_HANDLE_UNVALID;
SPIDMA_DESC_ST spidma_intf;

#if CONFIG_GENERAL_DMA
static void spidma_intf_node_rx_handler(UINT32 dma)
{
	UINT16 already_len = spidma_intf.rx_read_len;
	UINT16 copy_len = spidma_intf.node_len;
	GLOBAL_INT_DECLARATION();

	if (spidma_intf.node_full_handler != NULL)
		spidma_intf.node_full_handler(spidma_intf.rxbuf + already_len, copy_len, 0, 0);

	already_len += copy_len;

	if (already_len >= spidma_intf.rxbuf_len)
		already_len = 0;

	GLOBAL_INT_DISABLE();
	spidma_intf.rx_read_len = already_len;
	GLOBAL_INT_RESTORE();
}
#endif

static void spidma_intf_frame_end_handler(void)
{
#if CONFIG_GENERAL_DMA
	UINT16 already_len = spidma_intf.rx_read_len;
	UINT32 channel = SPIDMA_CHNAL;
	int left_len = bk_dma_get_remain_len(channel);
	int rec_len = spidma_intf.node_len - left_len;

	if ((spidma_intf.node_full_handler != NULL) && (rec_len > 0))
		spidma_intf.node_full_handler(spidma_intf.rxbuf + already_len, rec_len, 0, 0);

	already_len += rec_len;
	if (already_len >= spidma_intf.rxbuf_len)
		already_len -= spidma_intf.rxbuf_len;

	spidma_intf.rx_read_len = already_len;

	// turn off dma, so dma can start from first configure. for easy handler
	bk_dma_stop(channel);

	spidma_intf.rx_read_len = 0;
	bk_dma_start(channel);
#endif

	if ((spidma_intf.data_end_handler))
		spidma_intf.data_end_handler();
}

static void spidma_intfer_config_desc(void *data)
{
	os_memset(&spidma_intf, 0, sizeof(SPIDMA_DESC_ST));
	os_memcpy(&spidma_intf, data, sizeof(video_config_t));

	spidma_intf.txbuf = NULL;
	spidma_intf.txbuf_len = 0;

	spidma_intf.tx_handler = NULL;

	spidma_intf.mode = 0;
	spidma_intf.timeout_val = SPIDMA_RXDATA_TIMEOUT;
	spidma_intf.end_frame_handler = spidma_intf_frame_end_handler;

#if CONFIG_GENERAL_DMA
	spidma_intf.dma_rx_handler = spidma_intf_node_rx_handler;
	spidma_intf.dma_rx_channel = SPIDMA_CHNAL;
	spidma_intf.dma_tx_handler = NULL;
	spidma_intf.dma_tx_channel = DMA_ID_MAX;
#endif
}

/*---------------------------------------------------------------------------*/
bk_err_t spidma_intfer_init(void *data)
{
	UINT32 status;
	spidma_intfer_config_desc(data);

	spidma_hdl = ddev_open(DD_DEV_TYPE_SPIDMA, &status, (UINT32)&spidma_intf);
	SPIDMA_INTF_FATAL("spidma_intfer_init,%p\r\n", spidma_hdl);
	return kNoErr;
}

bk_err_t spidma_intfer_deinit(void)
{
	SPIDMA_INTF_FATAL("spidma_intfer_deinit,%p\r\n", spidma_hdl);
	ddev_close(spidma_hdl);
	os_memset(&spidma_intf, 0, sizeof(SPIDMA_DESC_ST));

	spidma_hdl = DD_HANDLE_UNVALID;
	return kNoErr;
}

#if 0
void spi_camera_flip(UINT8 n)
{
	UINT8 data;

	if (n) {
		data = 1;		//flip 180
	} else {
		data = 0;		//normal
	}

	bk_gpio_config_output(CAMERA_REV180_GPIO_INDEX);
	bk_gpio_output(CAMERA_REV180_GPIO_INDEX, data);
}
#endif
/*---------------------------------------------------------------------------*/

#endif  // CONFIG_SPIDMA

