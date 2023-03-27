DM BLE APIs
================

:link_to_translation:`en:[English]`

.. important::

   The DM BLE API v1.0 is the lastest stable DM BLE APIs. All new applications should use DM BLE API v1.0.


DM BLE API Categories
----------------------------

Most of DM BLE APIs can be categoried as:

Interface specific DM BLE APIs:
 - BLE common interface
 - BLE scan interface
 - BLE ADV interface
 - BLE connect interface

Compitability and Extension
----------------------------------------

The DM BLE APIs are flexible, easy to be extended and backward compatible. For most of the BLE configurations, we put some reserved fields in the config struct for future extendence. The API users need to make sure the reserved fields are initialized to 0, otherwise the compatibility may be broken as more fields are added.

Programing Principle
----------------------------------------

.. important::
  Here is some general principle for DM BLE API users:
   - Always init the reserved fields of config stuct to 0
   - Use BK_ERR_CHECK to check the return value of the DM BLE API
   - If you are not sure how to use DM BLE APIs, study the BLE example code first
   - If you are not sure how to initialize some fields of config struct, use the default configuration macro to use the config first and then set application specific fields.
   - Don't do too much work in BLE event callback, relay the event to your own application task.

User Development Model
----------------------------------------

Similar as most popular BLE driver, the Beken BLE driver is implemented as event driver. The application call DM BLE APIs to operate the BLE driver and get notified by BLE event.


User Guide
----------------------------------------
- create att database

::

    typedef struct
    {
        uint16_t pri_service_handle;
        uint16_t chara_notify_handle;
        uint16_t chara_ssid_handle;
        uint16_t chara_pass_handle;
    }boarding_env_s;

    static boarding_env_s boarding_env;

    void ble_do_reg_db(void)
    {
        int retval = kNoErr;

        GATT_DB_SERVICE_INFO service_info;
        uint16_t num_attr_handles;
        uint16_t service_handle;
        GATT_DB_UUID_TYPE char_uuid;
        uint16_t perm;
        uint16_t property;
        ATT_VALUE char_value;
        UINT16 char_handle;

        bk_ble_set_notice_cb(ble_at_notice_cb);

        service_info.is_primary = 1;
        service_info.uuid.uuid_format = ATT_16_BIT_UUID_FORMAT;
        service_info.uuid.uuid.uuid_16 = GATT_BOARDING_SERVICE_UUID;
        service_info.link_req = GATT_DB_SER_SUPPORT_ANY_LINK_TYPE;
        service_info.sec_req = GATT_DB_SER_NO_SECURITY_PROPERTY;
        num_attr_handles = 30U;
        retval = bk_ble_gatt_db_add_service
                   (
                       &service_info,
                       num_attr_handles,
                       &service_handle
                   );

        if (0 != retval)
        {
            os_printf("%s: BT_gatt_db_add_service() failed. Result: 0x%04X\n", __func__, retval);
            goto error;
        }
        else
        {
            boarding_env.pri_service_handle = service_handle;
        }

        char_uuid.uuid_format = ATT_16_BIT_UUID_FORMAT;
        char_uuid.uuid.uuid_16 = GATT_BOARDING_NOTIFY_CHARACTERISTIC;
        perm = GATT_DB_PERM_READ;
        property = (GATT_DB_CHAR_READ_PROPERTY | GATT_DB_CHAR_NOTIFY_PROPERTY);
        char_value.val = s_boarding_notify;
        char_value.len = sizeof(s_boarding_notify);
        char_value.actual_len = char_value.len;
        retval = bk_ble_gatt_db_add_characteristic
                       (
                           service_handle,
                           &char_uuid,
                           perm,
                           property,
                           &char_value,
                           &char_handle
                       );
        if (0 != retval)
        {
            os_printf("%s: bk_ble_gatt_db_add_characteristic() failed. Result: 0x%04X\n", __func__, retval);
            goto error;
        }
        else
        {
            boarding_env.chara_notify_handle = char_handle;
        }

        GATT_DB_UUID_TYPE    desc_uuid;
        ATT_VALUE            desc_value;

        uint8_t cccd_value[2U]    = { 0x00U, 0x00U };

        desc_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
        desc_uuid.uuid.uuid_16 = GATT_CLIENT_CONFIG;

        perm                   = (GATT_DB_PERM_READ | GATT_DB_PERM_WRITE);

        desc_value.val         = cccd_value;
        desc_value.len         = 2U;
        desc_value.actual_len  = desc_value.len;

        /* Add descriptor CCCD */
        retval = bk_ble_gatt_db_add_characteristic_descriptor
                 (
                     service_handle,
                     boarding_env.chara_notify_handle,
                     &desc_uuid,
                     perm,
                     &desc_value
                 );
        if (0 != retval)
        {
            os_printf("%s: bk_ble_gatt_db_add_characteristic_descriptor() failed. Result: 0x%04X\n", __func__, retval);
            goto error;
        }

        char_uuid.uuid_format = ATT_16_BIT_UUID_FORMAT;
        char_uuid.uuid.uuid_16 = GATT_BOARDING_SSID_CHARACTERISTIC;
        perm = GATT_DB_PERM_READ | GATT_DB_PERM_WRITE;
        property = (GATT_DB_CHAR_READ_PROPERTY | GATT_DB_CHAR_WRITE_PROPERTY | GATT_DB_CHAR_WRITE_WITHOUT_RSP_PROPERTY);
        char_value.val = s_boarding_ssid;
        char_value.len = sizeof(s_boarding_ssid);
        char_value.actual_len = char_value.len;
        retval = bk_ble_gatt_db_add_characteristic
                       (
                           service_handle,
                           &char_uuid,
                           perm,
                           property,
                           &char_value,
                           &char_handle
                       );
        if (0 != retval)
        {
            os_printf("%s: bk_ble_gatt_db_add_characteristic() failed. Result: 0x%04X\n", __func__, retval);
            goto error;
        }
        else
        {
            boarding_env.chara_ssid_handle = char_handle;
        }

        char_uuid.uuid_format = ATT_16_BIT_UUID_FORMAT;
        char_uuid.uuid.uuid_16 = GATT_BOARDING_PASSWORD_CHARACTERISTIC;
        perm = GATT_DB_PERM_READ | GATT_DB_PERM_WRITE;
        property = (GATT_DB_CHAR_READ_PROPERTY | GATT_DB_CHAR_WRITE_PROPERTY | GATT_DB_CHAR_WRITE_WITHOUT_RSP_PROPERTY);
        char_value.val = s_boarding_password;
        char_value.len = sizeof(s_boarding_password);
        char_value.actual_len = char_value.len;
        retval = bk_ble_gatt_db_add_characteristic
                       (
                           service_handle,
                           &char_uuid,
                           perm,
                           property,
                           &char_value,
                           &char_handle
                       );
        if (0 != retval)
        {
            os_printf("%s: bk_ble_gatt_db_add_characteristic() failed. Result: 0x%04X\n", __func__, retval);
            goto error;
        }
        else
        {
            boarding_env.chara_pass_handle = char_handle;
        }

        retval = bk_ble_gatt_db_add_completed();

        if (retval != 0)
        {
            os_printf("%s GATT Database Registration err: 0x%04X\n", __func__, retval);
            goto error;
        }

        retval = bk_ble_gatt_db_set_callback(gatt_db_boarding_gatt_char_handler);

        if (retval != 0)
        {
            os_printf("%s bk_ble_gatt_db_set_callback err: 0x%04X\n", __func__, retval);
            goto error;
        }
    }

    static API_RESULT gatt_db_boarding_gatt_char_handler(GATT_DB_HANDLE    * handle, GATT_DB_PARAMS    * params)
    {
        uint16_t config;

        if(handle->service_id == boarding_env.pri_service_handle)
        {
            switch (params->db_op)
            {
                case GATT_DB_CHAR_PEER_CLI_CNFG_WRITE_REQ:
                {
                    if(handle->char_id == boarding_env.chara_notify_handle)
                    {
                        config = (((uint16_t)(params->value.val[1]))<<8) | params->value.val[0];
                        if (GATT_CLI_CNFG_NOTIFICATION == config)
                        {
                            os_printf("client notify config open\r\n");
                        }
                        else if(GATT_CLI_CNFG_DEFAULT == config)
                        {
                            os_printf("client notify config close\r\n");
                        }
                        else
                        {
                            //nothing to do
                        }
                    }
                }
                break;

                case GATT_DB_CHAR_PEER_READ_REQ:
                {
                    if(handle->char_id == boarding_env.chara_pass_handle)
                    {
                        ATT_VALUE param;
                        bk_ble_gatt_get_char_val(handle, &param);
                        os_printf("Borading read PASS:%s, %d \r\n",param.val, param.actual_len);
                    }
                    else if(handle->char_id == boarding_env.chara_ssid_handle)
                    {
                        ATT_VALUE param;
                        bk_ble_gatt_get_char_val(handle, &param);
                        os_printf("Borading read SSID:%s, %d \r\n",param.val, param.actual_len);
                    }
                    else
                    {
                        //nothing to do
                    }
                }
                break;

                case GATT_DB_CHAR_PEER_WRITE_REQ:
                {
                    if(handle->char_id == boarding_env.chara_pass_handle)
                    {
                        s_boarding_password_len = 0;
                        os_memset((uint8_t *)s_boarding_password, 0, sizeof(s_boarding_password)/sizeof(s_boarding_password[0]));
                        os_memcpy((uint8_t *)s_boarding_password, params->value.val, params->value.len);
                        s_boarding_password_len = params->value.len;
                        os_printf("Boarding write PASS:%s, %d, %d\r\n",s_boarding_password, s_boarding_password_len, params->value.actual_len);
                        demo_sta_app_init((char *)s_boarding_ssid, (char *)s_boarding_password);
                    }
                    else if(handle->char_id == boarding_env.chara_ssid_handle)
                    {
                        s_boarding_ssid_len = 0;
                        os_memset((uint8_t *)s_boarding_ssid, 0, sizeof(s_boarding_ssid)/sizeof(s_boarding_ssid[0]));
                        os_memcpy((uint8_t *)s_boarding_ssid, params->value.val, params->value.len);
                        s_boarding_ssid_len = params->value.len;
                        os_printf("Boarding write SSID:%s, %d, %d\r\n",s_boarding_ssid, s_boarding_ssid_len, params->value.actual_len);
                    }
                    else
                    {
                        //nothing to do
                    }
                }
                break;
                default:
                    os_printf("No Specific Application Handling Required for Operation 0x%02X\n", params->db_op);
                    break;
            }
        }

        return 0;
    }



- start adv

::
    int retval;
    ble_adv_parameter_t tmp_param;

    memset(&tmp_param, 0, sizeof(tmp_param));

    tmp_param.adv_intv_max = 160;
    tmp_param.adv_intv_min = 120;
    tmp_param.adv_type = ADV_LEGACY_TYPE_ADV_IND;
    tmp_param.chnl_map = ADV_ALL_CHNLS;
    tmp_param.filter_policy = ADV_FILTER_POLICY_ALLOW_SCAN_ANY_CONNECT_ANY;
    tmp_param.own_addr_type = 0;
    tmp_param.peer_addr_type = 0;

    const uint8_t adv_data[] = {0x02, 0x01, 0x06, 0x0f, 0x08, 0x53, 0x6d, 0x61, 0x72, 0x74, 0x2d, 0x44, 0x6f, 0x6f, 0x72, 0x62, 0x65, 0x6c, 0x6c};

    //wait for ble_at_cmd_cb
    retval = bk_ble_set_advertising_data(adv_len, adv_data, ble_at_cmd_cb);
    if (retval != BK_ERR_BLE_SUCCESS)
    {

    }

    retval = bk_ble_set_advertising_enable(1, ble_at_cmd_cb);
    //wait for ble_at_cmd_cb
    if (retval != BK_ERR_BLE_SUCCESS)
    {

    }








API Reference
----------------------------------------

.. include:: ../../_build/inc/dm_ble.inc

API Typedefs
----------------------------------------
.. include:: ../../_build/inc/dm_ble_types.inc
