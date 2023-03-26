#include "sdkconfig.h"
#include <string.h>
#include <components/system.h>
#include "modules/ota.h"
#include <os/os.h>
#include <common/bk_kernel_err.h>
#include "uvc_ota_demo.h"
#include "bk_usb.h"
#include <os/mem.h>
#if CONFIG_OTA_HTTP
#include "utils_httpc.h"
#include "modules/wifi.h"
#endif

/* Table of CRC values for high-order byte */
static const UINT8 table_crc_hi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};

/* Table of CRC values for low-order byte */
static const UINT8 table_crc_lo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
    0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
    0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
    0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
    0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
    0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
    0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
    0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
    0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
    0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
    0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
    0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
    0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
    0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
    0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
    0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
    0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
    0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};

static uvc_ota_demo_list *s_uvc_ota_data_list;
static char *s_uvc_ota_http_uri;
static uvc_ota_demo_http_recived_data_t *s_uvc_ota_http_recived;
static uint8_t s_transfer_wait_ms = 2;
static beken_thread_t  uvc_ota_demo_thread_hdl = NULL;
static beken_queue_t uvc_ota_demo_int_msg_que = NULL;
static beken_mutex_t uvc_ota_demo_transfer_mutex = NULL;
static beken_semaphore_t uvc_ota_demo_transfer_semaphore = NULL;
static beken_thread_t  uvc_ota_demo_http_thread_hdl = NULL;
static beken_semaphore_t uvc_ota_demo_http_semaphore = NULL;
static bool s_uvc_ota_demo_init_flag = 0;
static bool s_uvc_ota_demo_check_version_flag = 0;

static UINT16 uvc_ota_demo_crc_calculation(UINT8 *buffer, UINT16 buffer_length)
{
    uint8_t crc_hi = 0xFF; /* high CRC byte initialized */
    uint8_t crc_lo = 0xFF; /* low CRC byte initialized */
    unsigned int i; /* will index into CRC lookup */

    /* pass through message buffer */
    while (buffer_length--) {
        i = crc_hi ^ *buffer++; /* calculate the CRC  */
        if(i >= PKT_CRC_TABLE_MAX_LEN){
            UVC_OTA_LOGE("CRC calculation is out of table !!!.\r\n");
            crc_hi = 0xFF;
            crc_lo = 0xFF;
            break;
        }
        crc_hi = crc_lo ^ table_crc_hi[i];
        crc_lo = table_crc_lo[i];
    }

    return (crc_hi << 8 | crc_lo);
}
static int uvc_ota_demo_increase_crc_check_value(UINT8 *req, int *req_length)
{
    int after_add_crc_packet_length = *req_length;
    uint16_t crc = uvc_ota_demo_crc_calculation(req, after_add_crc_packet_length);

    req[after_add_crc_packet_length++] = crc >> 8;
    req[after_add_crc_packet_length++] = crc & 0x00FF;

    return after_add_crc_packet_length;
}

static uint32_t uvc_ota_demo_ctrl_sync_tx_callback(void *pContext, void *pControlIrp)
{
    rtos_set_semaphore(&uvc_ota_demo_transfer_semaphore);
    UVC_OTA_LOGD("Control_Transfer Done.\r\n");
    return 0;
}

static uint32_t uvc_ota_demo_ctrl_sync_rx_callback(void *pContext, void *pControlIrp)
{
    rtos_set_semaphore(&uvc_ota_demo_transfer_semaphore);
    UVC_OTA_LOGD("Control_Transfer Done.\r\n");
    return 0;
}

static int uvc_ota_demo_set_data(int wValue, int wIndex, UINT8 *aucData, int wLength)
{
    //按usb报文格式发送数据
    //RequestType = 0x43;
    //request = 0xA5;
	if(!aucData) {
		UVC_OTA_LOGI("%s aucData is NULL\r\n", __func__);
		return IPC_STATUS_NOT_FIND;
	}

	rtos_delay_milliseconds(s_transfer_wait_ms);
	rtos_lock_mutex(&uvc_ota_demo_transfer_mutex);

	s_usb_device_request *pSetup = (s_usb_device_request *)os_malloc(sizeof(s_usb_device_request));
	if(!pSetup) {
		UVC_OTA_LOGI("%s pSetup malloc fail\r\n", __func__);
		rtos_unlock_mutex(&uvc_ota_demo_transfer_mutex);
		return IPC_STATUS_BUSY;
	}


	s_usb_transfer_buffer_info *buffer_info = (s_usb_transfer_buffer_info *)os_malloc(sizeof(s_usb_transfer_buffer_info));
	if(!buffer_info) {
		os_free(pSetup);
		pSetup = NULL;
		UVC_OTA_LOGI("%s buffer_info malloc fail\r\n", __func__);
		rtos_unlock_mutex(&uvc_ota_demo_transfer_mutex);
		return IPC_STATUS_BUSY;
	}

	void *parameter;
	uint32_t ret = 0;

	pSetup->bmRequestType = 0x43;
	pSetup->bRequest = 0xA5;
	pSetup->wValue = wValue;
	pSetup->wIndex = wIndex;
	pSetup->wLength = wLength;

	buffer_info->pOutBuffer = (uint8_t *)aucData;
	buffer_info->dwOutLength = pSetup->wLength;
	buffer_info->pInBuffer = aucData;
	buffer_info->dwInLength = 0;
	parameter = (void *)uvc_ota_demo_ctrl_sync_tx_callback;
	buffer_info->pfIrpComplete = parameter;
	buffer_info->pCompleteParam = pSetup;

	bk_usb_control_transfer(pSetup, buffer_info);

	ret = rtos_get_semaphore(&uvc_ota_demo_transfer_semaphore, BEKEN_WAIT_FOREVER);
	if(ret != IPC_STATUS_OK){
		UVC_OTA_LOGD("%s Outbuffer: %x %x %x %x %x %x %x %x \r\n", __FUNCTION__, aucData[0], aucData[1], aucData[2],aucData[3],aucData[4],aucData[5],aucData[6],aucData[7]);
	}

	if(pSetup) {
		os_free(pSetup);
		pSetup = NULL;
	}

	if(buffer_info) {
		os_free(buffer_info);
		buffer_info = NULL;
	}

	rtos_unlock_mutex(&uvc_ota_demo_transfer_mutex);
	return ret;

}

static int uvc_ota_demo_get_data(int wValue, int wIndex, UINT8 *aucData, int iMaxDataLen)
{
    //按usb报文格式接收数据
    //RequestType = 0xC3;
    //request = 0xA5;

	if(!aucData) {
		UVC_OTA_LOGI("%s aucData is NULL\r\n", __func__);
		return IPC_STATUS_NOT_FIND;
	}

	rtos_delay_milliseconds(s_transfer_wait_ms);
	rtos_lock_mutex(&uvc_ota_demo_transfer_mutex);

	s_usb_device_request *pSetup = (s_usb_device_request *)os_malloc(sizeof(s_usb_device_request));
	if(!pSetup) {
		pSetup = NULL;
		UVC_OTA_LOGI("%s pSetup malloc fail\r\n", __func__);
		rtos_unlock_mutex(&uvc_ota_demo_transfer_mutex);
		return IPC_STATUS_BUSY;
	}

	s_usb_transfer_buffer_info *buffer_info = (s_usb_transfer_buffer_info *)os_malloc(sizeof(s_usb_transfer_buffer_info));
	if(!buffer_info) {
		os_free(pSetup);
		pSetup = NULL;
		UVC_OTA_LOGI("%s buffer_info malloc fail\r\n", __func__);
		rtos_unlock_mutex(&uvc_ota_demo_transfer_mutex);
		return IPC_STATUS_BUSY;
	}

	void *parameter;
	uint32_t ret = 0;

	pSetup->bmRequestType = 0xC3;
	pSetup->bRequest = 0xA5;
	pSetup->wValue = wValue;
	pSetup->wIndex = wIndex;
	pSetup->wLength = iMaxDataLen;

	buffer_info->pOutBuffer = aucData;
	buffer_info->dwOutLength = 0;
	buffer_info->pInBuffer = (uint8_t *)aucData;
	buffer_info->dwInLength = pSetup->wLength;
	parameter = (void *)uvc_ota_demo_ctrl_sync_rx_callback;
	buffer_info->pfIrpComplete = parameter;
	buffer_info->pCompleteParam = pSetup;

	bk_usb_control_transfer(pSetup, buffer_info);

	ret = rtos_get_semaphore(&uvc_ota_demo_transfer_semaphore, BEKEN_WAIT_FOREVER);
	if(ret != IPC_STATUS_OK){
		UVC_OTA_LOGD("%s Inbuffer: %x %x %x %x %x %x %x %x \r\n", __FUNCTION__, aucData[0], aucData[1], aucData[2],aucData[3],aucData[4],aucData[5],aucData[6],aucData[7]);
	}

	if(pSetup) {
		os_free(pSetup);
		pSetup = NULL;
	}

	if(buffer_info) {
		os_free(buffer_info);
		buffer_info = NULL;
	}

	rtos_unlock_mutex(&uvc_ota_demo_transfer_mutex);
	return ret;
}

static int uvc_ota_demo_get_version(UINT8 *pszData, int iMaxDataLen)
{
    return uvc_ota_demo_get_data(0, UVC_CUSTOM_GET_VERSION, pszData, iMaxDataLen);
}

static int uvc_ota_demo_upgrade_filelen(int iFileLen)
{
    UINT8 aucFileLen[4] = {0};
    aucFileLen[0] = iFileLen&0xff;
    aucFileLen[1] = (iFileLen>>8)&0xff;
    aucFileLen[2] = (iFileLen>>16)&0xff;
    aucFileLen[3] = (iFileLen>>24)&0xff;
    return uvc_ota_demo_set_data(0, UVC_CUSTOM_SET_UPGRADE_FILE_LEN, aucFileLen, sizeof(aucFileLen));
}

static int uvc_ota_demo_get_upgrade_filelen(uint32_t *piFileLen)
{
    int iRet = 0;
    UINT8 aucFileLen[4] = {0};
    iRet = uvc_ota_demo_get_data(0, UVC_CUSTOM_GET_UPGRADE_FILE_LEN, aucFileLen, sizeof(aucFileLen));
    if(iRet != IPC_STATUS_OK)
    {
        return iRet;
    }
    *piFileLen = (aucFileLen[0]&0xff) +
                 ((aucFileLen[1]&0xff) << 8) +
                 ((aucFileLen[2]&0xff) << 16) +
                 ((aucFileLen[3]&0xff) << 24);
    return IPC_STATUS_OK;
}

static int uvc_ota_demo_transmit_filedata(UINT8 *aucData, int iLen)
{
    int iRet = 0;
    int iOffset = 0;
    int wValue = 0;
    int iSendLen = 0;

    while(iOffset < iLen)
    {
        wValue = 0;
        /* 0: data packet
         * 1: head packet
         * 2: end packet
         * 3: one frame
         */
        if(iOffset == 0)
        {
            wValue = wValue | 0x1;
        }

        if((iOffset+UVC_TRAMSMIT_DATA_MAX_LEN) < iLen)
        {
            iSendLen = UVC_TRAMSMIT_DATA_MAX_LEN;
        }
        else
        {
            /* 2: end packet*/
            wValue = wValue | 0x2;
            iSendLen = iLen - iOffset;
        }
        UVC_OTA_LOGD("%s aucData+iOffset:%p  iSendLen:%d\r\n", __func__, aucData+iOffset, iSendLen);
        iRet = uvc_ota_demo_set_data(wValue, UVC_CUSTOM_SET_UPGRADE_DATA, aucData+iOffset, iSendLen);
        if(iRet != IPC_STATUS_OK)
        {
            UVC_OTA_LOGD("%s iRet:%d\r\n", __func__, iRet);
            return iRet;
        }
        iOffset += iSendLen;
    }

    return IPC_STATUS_OK;
}

static int uvc_ota_demo_get_transmit_num(int *piData)
{
    int iRet = 0;
    UINT8 aucFileLen[4] = {0};
    iRet = uvc_ota_demo_get_data(0, UVC_CUSTOM_GET_UPGRADE_DATA_NUM, aucFileLen, sizeof(aucFileLen));
    if(iRet != IPC_STATUS_OK)
    {
        return iRet;
    }
    *piData = (aucFileLen[0]&0xff) +
                 ((aucFileLen[1]&0xff) << 8) +
                 ((aucFileLen[2]&0xff) << 16) +
                 ((aucFileLen[3]&0xff) << 24);
    return IPC_STATUS_OK;
}

static int uvc_ota_demo_get_transmit_result(uint32_t *piData)
{
    int iRet = 0;
    UINT8 aucFileLen[4] = {0};
    iRet = uvc_ota_demo_get_data(0, UVC_CUSTOM_GET_UPGRADE_FILE_RESULT, aucFileLen, sizeof(aucFileLen));
    if(iRet != IPC_STATUS_OK)
    {
        return iRet;
    }
    *piData = (aucFileLen[0]&0xff);
    return IPC_STATUS_OK;
}

bk_err_t bk_uvc_ota_demo_get_upgrade_persent(int *piData)
{
	if(!s_uvc_ota_demo_init_flag) {
		UVC_OTA_LOGI("%s Please INIT\r\n", __func__);
		return IPC_STATUS_FAIL;
	}

    int iRet = 0;
    UINT8 aucFileLen[4] = {0};
    iRet = uvc_ota_demo_get_data(0, UVC_CUSTOM_GET_UPGRADE_STATUS, aucFileLen, sizeof(aucFileLen));
    if(iRet != IPC_STATUS_OK)
    {
        return iRet;
    }
    *piData = (aucFileLen[0]&0xff);
    return IPC_STATUS_OK;
}

static int uvc_ota_demo_upgrade_sending(UINT uiPktNum,UINT8 * ucBuf, UINT uiBufLen)
{
    UINT8 uiPkt[PKT_MAX_LEN+PKT_INDENTIFICATION_DETECTION_LEN]={0};
    //int fd = -1;
    int len = 0;
    int ret = 0;
    int iData = 0;
    int crc_len = 0;
    if(uiBufLen > PKT_MAX_LEN)
    {
        UVC_OTA_LOGI("pkt too long!!len=%d\r\n",uiBufLen);
        return IPC_STATUS_ARGV;
    }

    uiPkt[len] = PKT_HEAD;
    len++;

    uiPkt[len] = (uiPktNum >> 24) & 0xff;
    len++;
    uiPkt[len] = (uiPktNum >> 16) & 0xff;
    len++;
    uiPkt[len] = (uiPktNum >> 8) & 0xff;
    len++;
    uiPkt[len] = (uiPktNum) & 0xff;
    len++;

    memcpy(&uiPkt[len], ucBuf, uiBufLen);

    len = len + uiBufLen;
    UVC_OTA_LOGD("%s ucBuf:%p  uiBufLen:%d len:%d\r\n", __func__, ucBuf, uiBufLen, len);
    crc_len = len;
    len = uvc_ota_demo_increase_crc_check_value(uiPkt, &crc_len);
    UVC_OTA_LOGD("%s len:%d crc_len:%d\r\n", __func__, len, crc_len);
    ret = uvc_ota_demo_transmit_filedata(uiPkt, len);
    if(ret != IPC_STATUS_OK)
    {
        UVC_OTA_LOGI("uvc_ota_demo_transmit_filedata error!!\r\n");
        return ret;
    }

    ret = uvc_ota_demo_get_transmit_num(&iData);
    if(ret != IPC_STATUS_OK)
    {
        return ret;
    }
    if(uiPktNum != iData)
    {
        UVC_OTA_LOGI("pkt send error!!pktnum=%d:%d\r\n",uiPktNum,iData);
        return IPC_STATUS_ARGV;
    }
    return ret;
}

static bk_err_t uvc_ota_demo_upgrade_transmit(uint8_t *szBuf, int uiLen)
{
    int ret = 0;
    UINT uiOffset = 0;
    UINT uiPktNum = 0;
    UINT uiSendLen = 0;
    UINT uiRetry = PKT_RESEND_NUMBER;

    while(uiOffset < uiLen)
    {
        uiSendLen = ((uiLen - uiOffset) > PKT_MAX_LEN) ? PKT_MAX_LEN : (uiLen - uiOffset);

        ret = uvc_ota_demo_upgrade_sending(uiPktNum,szBuf+uiOffset, uiSendLen);
        if(IPC_STATUS_OK == ret)
        {
            UVC_OTA_LOGD("Send pkt=%d, len=%d \r\n",uiPktNum,uiSendLen);
            uiPktNum++;
            uiOffset += uiSendLen;
            uiRetry = PKT_RESEND_NUMBER;
        }
        else
        {
            while((IPC_STATUS_OK != ret) && (uiRetry > 0))
            {
                 ret = uvc_ota_demo_upgrade_sending(uiPktNum,szBuf+uiOffset, uiSendLen);
                 uiRetry--;
            }
            if(0 > uiRetry)
            {
                UVC_OTA_LOGI("Upgrade Sending err! ret=%d, num=%d, offset=%d, len=%d\r\n",ret, uiPktNum,uiOffset,uiLen);
                return IPC_STATUS_FAIL;
            }
        }
    }

    return IPC_STATUS_OK;
}

bk_err_t bk_uvc_ota_demo_transmit_result(uint32_t *result)
{
	if(!s_uvc_ota_demo_init_flag) {
		UVC_OTA_LOGI("%s Please INIT\r\n", __func__);
		return IPC_STATUS_FAIL;
	}
	if(s_uvc_ota_demo_check_version_flag)
		s_uvc_ota_demo_check_version_flag = 0;
	uint32_t iResult = 0;
	bk_err_t ret = 0;

	ret = uvc_ota_demo_get_transmit_result(&iResult);
	if(IPC_STATUS_OK != ret)
	{
		UVC_OTA_LOGI("Upgrade Dev err! ret=%d\r\n",ret);
		*result = iResult;
		return IPC_STATUS_FAIL;
	}
	if(0 != iResult)
	{
		UVC_OTA_LOGI("Upgrade Dev err! result=%d\r\n",iResult);
		*result = iResult;
		return IPC_STATUS_FAIL;
	}

	*result = iResult;
	return IPC_STATUS_OK;
}

bk_err_t bk_uvc_ota_demo_upgrade_check(char *szFirmwareVersion, char *aucFirmwareData, int iFirmwareLen)
{
	if(!s_uvc_ota_demo_init_flag) {
		UVC_OTA_LOGI("%s Please INIT\r\n", __func__);
		return IPC_STATUS_FAIL;
	}
	if(!s_uvc_ota_demo_check_version_flag)
		s_uvc_ota_demo_check_version_flag = 1;
    char szOldVersion[6] = {0};
    UINT8 get_version[6] = {0};
    int ret = 0;
    uint32_t iTmp = 0;
    ret = uvc_ota_demo_get_version(get_version,sizeof(get_version));
    if(IPC_STATUS_OK != ret)
    {
        s_uvc_ota_demo_check_version_flag = 0;
        UVC_OTA_LOGI("Get version err! ret=%d Please Check Again\r\n",ret);
        return IPC_STATUS_FAIL;
    }
	os_memcpy((void *)szOldVersion, get_version, sizeof(get_version));
    if(0 == strcmp((char *)szOldVersion, szFirmwareVersion))
    {
        UVC_OTA_LOGI("This version is already upgraded old=%s, now=%s\r\n",szOldVersion,szFirmwareVersion);
        return IPC_STATUS_OK;
    }

    UVC_OTA_LOGI("Version Check!  old=%s, now=%s\r\n",szOldVersion,szFirmwareVersion);

    UVC_OTA_LOGI("Upgrade Transmit Start!  Ver=%s\r\n", szFirmwareVersion);
    ret = uvc_ota_demo_upgrade_filelen(iFirmwareLen);
    if(IPC_STATUS_OK != ret)
    {
        UVC_OTA_LOGI("set file len err! ret=%d\r\n",ret);
        return IPC_STATUS_FAIL;
    }
    ret = uvc_ota_demo_get_upgrade_filelen(&iTmp);
    if(IPC_STATUS_OK != ret)
    {
        UVC_OTA_LOGI("get file len err! ret=%d\r\n",ret);
        return IPC_STATUS_FAIL;
    }
    if(iFirmwareLen != iTmp)
    {
        UVC_OTA_LOGI("get file len err! len=%d:%d\r\n",iFirmwareLen,iTmp);
        return IPC_STATUS_FAIL;
    }

    return IPC_STATUS_OK;
}

static void uvc_ota_demo_list_init(uvc_ota_demo_list *pList)
{
    UVC_OTA_LOGD("%s\r\n", __FUNCTION__);
    if (pList)
    {
        pList->pNext = NULL;
        pList->length = 0L;
        pList->pItem = NULL;
    }
}

/* Get the number of items in the given list */
static uint16_t uvc_ota_demo_list_length(uvc_ota_demo_list *pList)
{
    uvc_ota_demo_list *pNext;
    uint16_t wResult = 0;

    UVC_OTA_LOGD("%s\r\n", __FUNCTION__);
    if (pList)
    {
    	if (pList->pItem)
    	{
			pNext = pList->pNext;
			wResult++;
			while (pNext)
			{
				wResult++;
				pNext = pNext->pNext;
			}
    	}
    }
    return wResult;
}

static uint8_t uvc_ota_demo_list_insert(uvc_ota_demo_list *pPos, void *pItem, uint32_t length)
{
    uint8_t bOK = IPC_STATUS_OK;
    uvc_ota_demo_list *pNext = (uvc_ota_demo_list *)os_malloc(sizeof(uvc_ota_demo_list));
    if (!pNext) {
       pNext = NULL;
       UVC_OTA_LOGE("MGC_ListInsertItem: MUSB_MemAlloc failed\r\n");
       return IPC_STATUS_FAIL;
    }
    if (pPos && pNext)
    {
        memset(pNext, 0, sizeof(uvc_ota_demo_list));
        bOK = 1;
        pNext->pNext = pPos->pNext;
        pPos->pNext = pNext;
        pNext->pItem = pItem;
        pNext->length = length;
    }
    return bOK;
}

static uint8_t uvc_ota_demo_list_append(uvc_ota_demo_list *pList, void *pItem, unsigned long length)
{
    uint8_t bOK = 0;
    uvc_ota_demo_list *pNext = pList;

    UVC_OTA_LOGD("MUSB_ListAppendItem: pList = 0x%p, pList->pItem = 0x%p, pList->pNext = 0x%p, pList->length = 0x%lx\r\n",
            pList, pList->pItem, pList->pNext, pList->length);
    /* sanity */
    if (!pList)
    {
        return 0;
    }

    /* check if empty */
    if (!pList->pItem) {
        bOK = 1;
        pList->pItem = pItem;
        pList->length = length;
        pList->pNext = NULL;
    } else {
        /* general case */
        while (pNext->pNext)
        {
            pNext = pNext->pNext;
        }
        bOK = uvc_ota_demo_list_insert(pNext, pItem, length);
    }
    return bOK;
}

/* Remove an item from the given list */
static void uvc_ota_demo_list_remove(uvc_ota_demo_list *pList, void *pItem)
{
    uvc_ota_demo_list *pNext = pList;
    uvc_ota_demo_list *pPos;

    /* sanity */
    if ((!pList) || (!pList->pItem))
    {
        return;
    }

    UVC_OTA_LOGD("%s \r\n", __FUNCTION__);

    /* special case for head */
    if (pList->pItem == pItem)
    {
        /* check if this will result in an empty list (since we never free the head) */
        pNext = pList->pNext;
        if (pNext)
        {
            /* non-empty; copy next record and free its storage */
            pList->pItem = pNext->pItem;
            pList->length = pNext->length;
            pList->pNext = pNext->pNext;
			UVC_OTA_LOGD("%s pNext:%p\r\n", __FUNCTION__, pNext);
            os_free(pNext);
        }
        else
        {
            /* now the list is empty (head item pointer is NULL) */
            pList->pItem = NULL;
            pList->length = 0L;
            pList->pNext = NULL;
        }
        return;
    }

    /* general case */
    while (pNext->pNext && (pNext->pNext->pItem != pItem))
    {
        pNext = pNext->pNext;
    }
    if (pNext->pNext->pItem == pItem)
    {
        /* found it; unlink and free storage */
        pPos = pNext->pNext;
        /* check if we are removing last element */
        if (pPos->pNext)
        {
            /* not last element; link */
            pNext->pNext->pItem = pPos->pNext->pItem;
            pNext->pNext->length = pPos->pNext->length;
            pNext->pNext = pPos->pNext;
        }
        else
        {
            /* last element; clear link */
            pNext->pNext = NULL;
        }
		UVC_OTA_LOGI("%s os_free pPos:%p\r\n", __func__, pPos);
        os_free(pPos);
    }
}

static void uvc_ota_demo_get_data_from_net(char *buf, uint32_t len, uint32_t recived, uint32_t total)
{
	while((uvc_ota_demo_list_length(s_uvc_ota_data_list) > 5))
	{
		rtos_delay_milliseconds(s_transfer_wait_ms);
	}

	uint32_t ret = 0;
	uint8_t *list_buffer = (uint8_t *)os_malloc(len);
	uvc_ota_demo_drv_msg_t msg;

	if(list_buffer == NULL)
	{
		UVC_OTA_LOGI("%s:malloc fail\r\n", __func__);
		return;	
	}

	os_memcpy((void *)list_buffer, buf, len);
	if(!uvc_ota_demo_list_append(s_uvc_ota_data_list, list_buffer, len)) {
		os_free(list_buffer);
		UVC_OTA_LOGI("%s append list fail\r\n", __func__);
		return;
	}

	msg.len = len;
	msg.buffer = list_buffer;
	msg.cmd = UVC_CUSTOM_SET_UPGRADE_DATA;
	UVC_OTA_LOGD("%s list_buffer:%p msg.buffer:%p\r\n", __func__, list_buffer, msg.buffer);
	s_uvc_ota_http_recived->recived = recived;
	s_uvc_ota_http_recived->total = total;

	if (uvc_ota_demo_int_msg_que) {
		ret = rtos_push_to_queue(&uvc_ota_demo_int_msg_que, &msg, BEKEN_NO_WAIT);
		if (kNoErr != ret) {
			UVC_OTA_LOGE("uvc_ota_demo_int_msg_que fail \r\n");
			return;
		}
	}

	return;
}

bk_err_t bk_uvc_ota_http_download(const char *uri, uint32_t len, uint8_t transfer_wait_ms)
{
	if(!s_uvc_ota_demo_init_flag) {
		UVC_OTA_LOGI("%s Please INIT\r\n", __func__);
		return IPC_STATUS_FAIL;
	}
	if(!s_uvc_ota_demo_check_version_flag) {
		UVC_OTA_LOGI("%s Please Check version\r\n", __func__);
		return IPC_STATUS_FAIL;
	}

	s_uvc_ota_http_uri = (char *)os_malloc(len);
	if(!s_uvc_ota_http_uri){
		return IPC_STATUS_FAIL;
	}
	
	UVC_OTA_LOGI("uri: %s len:%d \r\n", uri, len);
	os_memcpy((void *)(s_uvc_ota_http_uri), uri, len);
	s_transfer_wait_ms = transfer_wait_ms;
	rtos_set_semaphore(&uvc_ota_demo_http_semaphore);
	return IPC_STATUS_OK;
}

bk_err_t bk_uvc_ota_http_to_device_status(float *http_percentage, bool *usb_transfer_sta)
{
	if(!s_uvc_ota_demo_check_version_flag) {
		UVC_OTA_LOGI("%s Please Check version\r\n", __func__);
		return IPC_STATUS_FAIL;
	}

	*http_percentage = (((float)(s_uvc_ota_http_recived->recived))/((float)(s_uvc_ota_http_recived->total)))*100;

	if(uvc_ota_demo_list_length(s_uvc_ota_data_list) <= 1)
		*usb_transfer_sta = 1;
	else
		*usb_transfer_sta = 0;
	return IPC_STATUS_OK;
}

static bk_err_t uvc_ota_http_download(const char *uri)
{
	int ret;
	httpclient_t httpclient;
	httpclient_data_t httpclient_data;
	char http_content[HTTP_RESP_CONTENT_LEN];

#if CONFIG_SYSTEM_CTRL
	bk_wifi_ota_dtim(1);
#endif

	os_memset(&httpclient, 0, sizeof(httpclient_t));
	os_memset(&httpclient_data, 0, sizeof(httpclient_data));
	os_memset(&http_content, 0, sizeof(HTTP_RESP_CONTENT_LEN));
	httpclient.header = "Accept: text/xml,text/html,\r\n";
	httpclient_data.response_buf = http_content;
	httpclient_data.response_content_len = HTTP_RESP_CONTENT_LEN;
	ret = httpclient_common(&httpclient,
							uri,
							80,/*port*/
							NULL,
							HTTPCLIENT_GET,
							999999,
							&httpclient_data);

	if (0 != ret)
		UVC_OTA_LOGI("request epoch time from remote server failed.ret:%d\r\n",ret);
	else
		UVC_OTA_LOGI("sucess.\r\n");

	if(s_uvc_ota_http_uri)
	{
		os_free(s_uvc_ota_http_uri);
		s_uvc_ota_http_uri = NULL;
	}

	return ret;
}

static void uvc_ota_http_download_main(void)
{
	bk_err_t ret = kNoErr;

	while(1)
	{
		ret = rtos_get_semaphore(&uvc_ota_demo_http_semaphore, BEKEN_WAIT_FOREVER);
		if (kNoErr == ret) {
			UVC_OTA_LOGI("s_uvc_ota_http_uri: %s.\r\n", s_uvc_ota_http_uri);
			uvc_ota_http_download(s_uvc_ota_http_uri);
		}
	}
}

static void uvc_ota_demo_main(beken_thread_arg_t param_data)
{
	bk_err_t ret = kNoErr;
	uvc_ota_demo_drv_msg_t msg;

	while(1)
	{
		ret = rtos_pop_from_queue(&uvc_ota_demo_int_msg_que, &msg, BEKEN_WAIT_FOREVER);
		if (kNoErr == ret) {
			switch(msg.cmd) {
				case UVC_CUSTOM_SET_UPGRADE_DATA:
				{
					if(msg.buffer) {
						uvc_ota_demo_upgrade_transmit((uint8_t *)msg.buffer, msg.len);
						UVC_OTA_LOGD("%s msg.buffer:%p\r\n", __func__, msg.buffer);
						uvc_ota_demo_list_remove(s_uvc_ota_data_list, (void *)msg.buffer);
						os_free(msg.buffer);
						msg.buffer = NULL;
					}
					break;
				}
			}
		}

	}
}

bk_err_t bk_uvc_ota_demo_init(void)
{
	if(s_uvc_ota_demo_init_flag) {
		UVC_OTA_LOGI("%s Please DEINIT\r\n", __func__);
		return IPC_STATUS_FAIL;
	}

	void *parameter;
	uint32_t ret = 0;
	uint32_t wait_count = 0;
	bool malloc_mem_status = 0;

	bk_usb_close();
	if(bk_usb_open(USB_HOST_MODE)) {
		UVC_OTA_LOGI("%s Please Check whether any module is working\r\n", __func__);
		return IPC_STATUS_FAIL;
	}

	ret = bk_usb_control_transfer_init();
	while(ret){
		if(wait_count > 10) {
			UVC_OTA_LOGI("%s Please Connect device\r\n", __func__);
			return IPC_STATUS_FAIL;
		}
		rtos_delay_milliseconds(100);
		ret = bk_usb_control_transfer_init();
		wait_count++;
	}

	parameter = (void *)uvc_ota_demo_get_data_from_net;
	http_data_process_register_callback(parameter);

	do{
		s_uvc_ota_data_list = (uvc_ota_demo_list *)os_malloc(sizeof(uvc_ota_demo_list));
		if(!s_uvc_ota_data_list) {
			UVC_OTA_LOGI("%s s_uvc_ota_data_list malloc fail\r\n", __func__);
			malloc_mem_status = 1;
			break;
		}
		uvc_ota_demo_list_init(s_uvc_ota_data_list);

		s_uvc_ota_http_recived = (uvc_ota_demo_http_recived_data_t *)os_malloc(sizeof(uvc_ota_demo_http_recived_data_t));
		if(!s_uvc_ota_http_recived) {
			UVC_OTA_LOGI("%s s_uvc_ota_http_recived malloc fail\r\n", __func__);
			malloc_mem_status = 1;
			break;
		}

		if(!uvc_ota_demo_int_msg_que) {
			ret = rtos_init_queue(&uvc_ota_demo_int_msg_que,
								  "uvc_ota_demo_msg_que",
								  sizeof(uvc_ota_demo_drv_msg_t),
								  60);
			if (ret != kNoErr) {
				UVC_OTA_LOGI("%s uvc_ota_demo_int_msg_que fail\r\n", __func__);
				malloc_mem_status = 1;
				break;
			}
		}

		if(!uvc_ota_demo_transfer_mutex) {
			ret = rtos_init_mutex(&uvc_ota_demo_transfer_mutex);
			if (ret != kNoErr) {
				UVC_OTA_LOGI("%s rtos_init_mutex fail\r\n", __func__);
				malloc_mem_status = 1;
				break;
			}
		}

		if (!uvc_ota_demo_transfer_semaphore) {
			ret = rtos_init_semaphore(&uvc_ota_demo_transfer_semaphore, 1);
			if (IPC_STATUS_OK != ret) {
				UVC_OTA_LOGI("%s uvc_ota_demo_transfer_semaphore fail\r\n", __func__);
				malloc_mem_status = 1;
				break;
			}
		}

		if (!uvc_ota_demo_http_semaphore) {
			ret = rtos_init_semaphore(&uvc_ota_demo_http_semaphore, 1);
			if (IPC_STATUS_OK != ret) {
				UVC_OTA_LOGI("%s uvc_ota_demo_http_semaphore fail\r\n", __func__);
				malloc_mem_status = 1;
				break;
			}
		}

		if(!uvc_ota_demo_thread_hdl) {
			ret = rtos_create_thread(&uvc_ota_demo_thread_hdl,
								 5,
								 "uvc_ota_demo",
								 (beken_thread_function_t)uvc_ota_demo_main,
								 4096,
								 (beken_thread_arg_t)&uvc_ota_demo_int_msg_que);
			if (ret != kNoErr) {
				UVC_OTA_LOGI("%s uvc_ota_demo creat thread fail\r\n", __func__);
				malloc_mem_status = 1;
				break;
			}
		}

		if(!uvc_ota_demo_http_thread_hdl) {
			ret = rtos_create_thread(&uvc_ota_demo_http_thread_hdl,
								 5,
								 "uvc_ota_http",
								 (beken_thread_function_t)uvc_ota_http_download_main,
								 4096,
								 NULL);
			if (ret != kNoErr) {
				UVC_OTA_LOGI("%s uvc_ota_demo_http creat thread fail\r\n", __func__);
				malloc_mem_status = 1;
				break;
			}
		}
	}while(0);

	if(!malloc_mem_status){
		UVC_OTA_LOGI("%s creat source OK\r\n", __func__);
		s_uvc_ota_demo_init_flag = 1;
		return IPC_STATUS_OK;
	} else {
		UVC_OTA_LOGI("%s creat source fail\r\n", __func__);
		if(s_uvc_ota_data_list)
			s_uvc_ota_data_list = NULL;
		if(s_uvc_ota_http_recived)
			s_uvc_ota_http_recived = NULL;
		if(uvc_ota_demo_int_msg_que) {
			rtos_deinit_queue(&uvc_ota_demo_int_msg_que);
			uvc_ota_demo_int_msg_que = NULL;
		}
		if(uvc_ota_demo_transfer_mutex) {
			rtos_deinit_mutex(&uvc_ota_demo_transfer_mutex);
			uvc_ota_demo_transfer_mutex= NULL;
		}
		if(uvc_ota_demo_transfer_semaphore) {
			rtos_deinit_semaphore(&uvc_ota_demo_transfer_semaphore);
			uvc_ota_demo_transfer_semaphore= NULL;
		}
		if(uvc_ota_demo_http_semaphore) {
			rtos_deinit_semaphore(&uvc_ota_demo_http_semaphore);
			uvc_ota_demo_http_semaphore= NULL;
		}
		if(uvc_ota_demo_thread_hdl) {
			rtos_delete_thread(&uvc_ota_demo_thread_hdl);
			uvc_ota_demo_thread_hdl = NULL;
		}
		if(uvc_ota_demo_http_thread_hdl) {
			rtos_delete_thread(&uvc_ota_demo_http_thread_hdl);
			uvc_ota_demo_http_thread_hdl = NULL;
		}
		s_uvc_ota_demo_init_flag = 0;
		return IPC_STATUS_FAIL;
	}

	return IPC_STATUS_OK;
}

bk_err_t bk_uvc_ota_demo_deinit(void)
{
	if(!s_uvc_ota_demo_init_flag) {
		UVC_OTA_LOGI("%s Please INIT\r\n", __func__);
		return IPC_STATUS_FAIL;
	}

	void *parameter;

	if(uvc_ota_demo_thread_hdl) {

		bk_usb_close();
		parameter = NULL;
		http_data_process_register_callback(parameter);
		bk_usb_control_transfer_deinit();

		rtos_deinit_queue(&uvc_ota_demo_int_msg_que);
		uvc_ota_demo_int_msg_que = NULL;
		rtos_deinit_mutex(&uvc_ota_demo_transfer_mutex);
		uvc_ota_demo_transfer_mutex = NULL;
		rtos_deinit_semaphore(&uvc_ota_demo_transfer_semaphore);
		uvc_ota_demo_transfer_semaphore= NULL;
		rtos_deinit_semaphore(&uvc_ota_demo_http_semaphore);
		uvc_ota_demo_http_semaphore= NULL;
		rtos_delete_thread(&uvc_ota_demo_thread_hdl);
		uvc_ota_demo_thread_hdl = NULL;
		rtos_delete_thread(&uvc_ota_demo_http_thread_hdl);
		uvc_ota_demo_http_thread_hdl = NULL;

		if(s_uvc_ota_data_list)
		{
			os_free(s_uvc_ota_data_list);
			s_uvc_ota_data_list = NULL;
		}
		if(s_uvc_ota_http_recived)
		{
			os_free(s_uvc_ota_http_recived);
			s_uvc_ota_http_recived = NULL;
		}
	}
	s_uvc_ota_demo_init_flag = 0;
	s_uvc_ota_demo_check_version_flag = 0;
	return IPC_STATUS_OK;
}

