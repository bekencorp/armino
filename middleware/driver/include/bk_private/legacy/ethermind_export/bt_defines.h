#ifndef _BT_DEFINES_H_
#define _BT_DEFINES_H_

/* -------------------------------------------- Structures/Data Types */
/* 'signed' datatype of size '1 octet' */
typedef char CHAR;
/* 'signed' datatype of size '1 octet' */
typedef signed char INT8;
/* 'unsigned' datatype of size '1 octet' */
typedef unsigned char UCHAR;
/* 'unsigned' datatype of size '1 octet' */
typedef unsigned char UINT8;
/* 'signed' datatype of size '2 octet' */
typedef short int INT16;
/* 'unsigned' datatype of size '2 octet' */
typedef unsigned short int UINT16;

/* the following three are already defined by Windows */
#ifndef FREERTOS_WIN_EMULATION
/* 'signed' datatype of size '4 octet' */
typedef int INT32;
/* 'unsigned' datatype of size '4 octet' */
typedef unsigned int  UINT32;
/* 'unsigned' datatype of size '8 octet' */
typedef unsigned long long UINT64;
#endif /* FREERTOS_WIN_EMULATION */

/* 'unsigned' datatype of size '1 octet' */
typedef unsigned char BOOLEAN;
/**
 * @brief ble APIs Version 1.0
 * @addtogroup bk_bt_api_v1 New ble API group
 * @{
 */
 
/* LMP Packet Types - ACL Connection */
#define LMP_ACL_DM1                             0x0008U
#define LMP_ACL_DH1                             0x0010U
#define LMP_ACL_DM3                             0x0400U
#define LMP_ACL_DH3                             0x0800U
#define LMP_ACL_DM5                             0x4000U
#define LMP_ACL_DH5                             0x8000U

#define LMP_ACL_2_DH1                           0x0002U
#define LMP_ACL_3_DH1                           0x0004U
#define LMP_ACL_2_DH3                           0x0100U
#define LMP_ACL_3_DH3                           0x0200U
#define LMP_ACL_2_DH5                           0x1000U
#define LMP_ACL_3_DH5                           0x2000U

/**
 *  SPP Callback Event Types
 */
typedef enum
{
    SPP_NO_EVENT,
    SPP_CONNECT_CNF,
    SPP_CONNECT_IND,
    SPP_DISCONNECT_CNF,
    SPP_DISCONNECT_IND,
    SPP_STOP_CNF,
    SPP_SEND_CNF,
    SPP_RECVD_DATA_IND
} SPP_EVENTS;

/**
 *  SPP Handle
 */
typedef UINT32 SPP_HANDLE;

typedef UINT16    API_RESULT;

/**
 *  \defgroup spp_cb Application Callback
 *  \{
 *  This Section Describes the module Notification Callback interface offered
 *  to the application
 */

/**
 * \brief Application should register a callback through the API BT_spp_init.
 * \param [in] spp_handle
 *   Handle associates a SPP connection to a remote SPP device.
 * \param [in] spp_event
 *   Event indicated to application by Serial Port profile.
 * \param [in] status
 *   The status of the event.
 * \param [in] data
 *   Pointer to the array containing the parameters associated with the event.
 * \param [in] data_length
 *    Length of the parameters associated with the event.
 * \return API_RESULT:
 *   API_RESULT:  API_SUCCESS or one of the error codes defined in BT_error.h.
 * \see   BT_error.h
 */
typedef API_RESULT (* SPP_APPL_EVENT_NOTIFY_CB)
                   (
                       /* IN */  SPP_HANDLE    spp_handle,
                       /* IN */  SPP_EVENTS    spp_event,
                       /* IN */  API_RESULT    status,
                       /* IN */  void *        data,
                       /* IN */  UINT16        data_length
                   );


#endif