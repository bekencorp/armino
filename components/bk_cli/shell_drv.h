
#ifndef _shell_drv_h_
#define _shell_drv_h_

#ifdef __cplusplus
extern "C" {
#endif

#include <common/bk_typedef.h>

typedef  enum
{
	bFALSE = 0,
	bTRUE  = !bFALSE,
} bool_t;

typedef enum
{
	SHELL_IO_CTRL_GET_STATUS = 0,
	SHELL_IO_CTRL_RX_RESET,
	SHELL_IO_CTRL_TX_RESET,
	SHELL_IO_CTRL_FLUSH,
} shell_ctrl_cmd_t;

enum
{
	SHELL_DEV_UART = 0,
	SHELL_DEV_MAILBOX,
};

struct _shell_dev_drv;

typedef struct
{
	struct _shell_dev_drv  *dev_drv;
	u8      dev_type;
	void  * dev_ext;
} shell_dev_t;

typedef void  (* tx_complete_t)(u8 *pbuf, u16 Tag);
typedef void  (* rx_indicate_t)(void);

typedef struct _shell_dev_drv
{
	bool_t   (*init)(shell_dev_t * shell_dev);
	bool_t   (*open)(shell_dev_t * shell_dev, tx_complete_t tx_callback, rx_indicate_t rx_callback);
	u16      (*write_async)(shell_dev_t * shell_dev, u8 * pBuf, u16 BufLen, u16 Tag);
	u16      (*read)(shell_dev_t * shell_dev, u8 * pBuf, u16 BufLen);
	u16      (*write_sync)(shell_dev_t * shell_dev, u8 * pBuf, u16 BufLen);
	u16      (*write_echo)(shell_dev_t * shell_dev, u8 * pBuf, u16 BufLen);
	bool_t   (*io_ctrl)(shell_dev_t * shell_dev, u8 cmd, void * param);
	bool_t   (*close)(shell_dev_t * shell_dev);
} shell_dev_drv_t;

extern shell_dev_t		shell_uart;
extern shell_dev_t		shell_dev_mb;
extern shell_dev_t		shell_uart3;


#ifdef CONFIG_DUAL_CORE

#include "mailbox_channel.h"

struct _shell_ipc_drv;

typedef struct
{
	struct _shell_ipc_drv  *dev_drv;
	u8      dev_type;
	void  * dev_ext;
} shell_dev_ipc_t;

typedef int  (* shell_ipc_rx_t)(u16 cmd, void *data_buf, u16 dataLen);

typedef struct _shell_ipc_drv
{
	bool_t   (*init)(shell_dev_ipc_t * dev_ipc);
	bool_t   (*open)(shell_dev_ipc_t * dev_ipc, shell_ipc_rx_t rx_callback);
	u16      (*read)(shell_dev_ipc_t * dev_ipc, u8 * pBuf, u16 BufLen);
	u16      (*write_sync)(shell_dev_ipc_t * dev_ipc, u8 * pBuf, u16 BufLen);
	bool_t   (*io_ctrl)(shell_dev_ipc_t * dev_ipc, u8 cmd, void * param);
	bool_t   (*close)(shell_dev_ipc_t * dev_ipc);
} shell_ipc_drv_t;

typedef union
{
	struct
	{
		mb_chnl_hdr_t	hdr;
		u8			  * buf;
		u16				len;
		u16				tag;
	};
	mb_chnl_cmd_t	cmd_buf;
} log_cmd_t;

typedef union
{
	struct
	{
		mb_chnl_hdr_t	hdr;
		u8    * 		buf;
		u16     		len;
	};
	mb_chnl_cmd_t	cmd_buf;
} user_cmd_t;

enum
{
	MB_CMD_LOG_OUT = 1,
	MB_CMD_LOG_OUT_OK,
	MB_CMD_USER_INPUT,
	MB_CMD_ASSERT_OUT,
} ;

extern shell_dev_ipc_t		shell_dev_ipc;

#endif /* CONFIG_DUAL_CORE */

#ifdef __cplusplus
}
#endif

#endif /* _shell_drv_h_ */

