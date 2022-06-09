BLE APIs
================

:link_to_translation:`en:[English]`

.. important::

   The BLE API v1.0 is the lastest stable BLE APIs. All new applications should use BLE API v1.0.


BLE API Categories
----------------------------

Most of BLE APIs can be categoried as:

Interface specific BLE APIs:
 - BLE common interface
 - BLE scan interface
 - BLE ADV interface
 - BLE connect interface

Compitability and Extension
----------------------------------------

The BLE APIs are flexible, easy to be extended and backward compatible. For most of the BLE configurations, we put some reserved fields in the config struct for future extendence. The API users need to make sure the reserved fields are initialized to 0, otherwise the compatibility may be broken as more fields are added.

Programing Principle
----------------------------------------

.. important::
  Here is some general principle for BLE API users:
   - Always init the reserved fields of config stuct to 0
   - Use BK_ERR_CHECK to check the return value of the BLE API
   - If you are not sure how to use BLE APIs, study the BLE example code first
   - If you are not sure how to initialize some fields of config struct, use the default configuration macro to use the config first and then set application specific fields.
   - Don't do too much work in BLE event callback, relay the event to your own application task.

User Development Model
----------------------------------------

Similar as most popular BLE driver, the Beken BLE driver is implemented as event driver. The application call BLE APIs to operate the BLE driver and get notified by BLE event.


User Guide
----------------------------------------
- create att database

::

	#define DECL_PRIMARY_SERVICE_128     {0x00,0x28,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
	#define DECL_CHARACTERISTIC_128      {0x03,0x28,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
	#define DESC_CLIENT_CHAR_CFG_128     {0x02,0x29,0,0,0,0,0,0,0,0,0,0,0,0,0,0}

	//database index:
	enum {
		TEST_IDX_SVC,
		TEST_IDX_CHAR_DECL,
		TEST_IDX_CHAR_VALUE,
		TEST_IDX_CHAR_DESC,


		TEST_IDX_NB,
	};
	//database build by some att records.
	ble_attm_desc_t test_service_db[TEST_IDX_NB] = {
	   //  Service Declaration
	   [TEST_IDX_SVC]              = {DECL_PRIMARY_SERVICE_128, BK_BLE_PERM_SET(RD, ENABLE), 0, 0},
	   // Characteristic declare
	   [TEST_IDX_CHAR_DECL]    = {DECL_CHARACTERISTIC_128,  BK_BLE_PERM_SET(RD, ENABLE), 0, 0},
	   // Characteristic Value
	   [TEST_IDX_CHAR_VALUE]   = {{0x34, 0x12, 0},     BK_BLE_PERM_SET(NTF, ENABLE), BK_BLE_PERM_SET(RI, ENABLE) | BK_BLE_PERM_SET(UUID_LEN, UUID_16), 128},
	   //Client Characteristic Configuration Descriptor
	   [TEST_IDX_CHAR_DESC] = {DESC_CLIENT_CHAR_CFG_128, BK_BLE_PERM_SET(RD, ENABLE) | BK_BLE_PERM_SET(WRITE_REQ, ENABLE), 0, 0},
	};


	struct bk_ble_db_cfg ble_db_cfg;
	const uint16_t service_uuid = 0xffff;
	
	ble_db_cfg.att_db = (ble_attm_desc_t *)test_service_db;
	ble_db_cfg.att_db_nb = TEST_IDX_NB;
	ble_db_cfg.prf_task_id = g_test_prf_task_id;
	ble_db_cfg.start_hdl = 0;
	ble_db_cfg.svc_perm = BK_BLE_PERM_SET(SVC_UUID_LEN, UUID_16);
    os_memcpy(&(ble_db_cfg.uuid[0]), &service_uuid, 2);

	bk_ble_set_notice_cb(ble_at_notice_cb);
	bk_ble_create_db(&ble_db_cfg);


	void ble_at_notice_cb(ble_notice_t notice, void *param)
	{
		switch (notice) {

		case BLE_5_WRITE_EVENT: {

			if (w_req->prf_id == g_test_prf_task_id)
			{
				switch(w_req->att_idx)
				{
				case TEST_IDX_CHAR_DECL:
					break;
				case TEST_IDX_CHAR_VALUE:
					break;
				case TEST_IDX_CHAR_DESC:
					//when peer enable notification, we create time and notify peer, such as
					//write_buffer = (uint8_t *)os_malloc(s_test_data_len);
					//bk_ble_send_noti_value(s_test_data_len, write_buffer, g_test_prf_task_id, TEST_IDX_CHAR_VALUE);
					break;

				default:
					break;
				}
			}
			break;
		}
		case BLE_5_CREATE_DB:
		//bk_ble_create_db success here
		break;
		}
	}



- start adv

::

	ble_adv_param_t adv_param;

	adv_param.own_addr_type = 0;//BLE_STATIC_ADDR
	adv_param.adv_type = 0; //ADV_IND
	adv_param.chnl_map = 7;
	adv_param.adv_prop = 3;
	adv_param.adv_intv_min = 0x120; //min
	adv_param.adv_intv_max = 0x160; //max
	adv_param.prim_phy = 1;// 1M
	adv_param.second_phy = 1;// 1M
	actv_idx = bk_ble_get_idle_actv_idx_handle();
	if (actv_idx != UNKNOW_ACT_IDX) {
		bk_ble_create_advertising(actv_idx, &adv_param, ble_at_cmd_cb);
	}

	//wait for ble_at_cmd_cb

	const uint8_t adv_data[] = {0x02, 0x01, 0x06, 0x0A, 0x09, 0x37 0x32, 0x33, 0x31, 0x4e, 0x5f, 0x42, 0x4c, 0x45};
	bk_ble_set_adv_data(actv_idx, adv_data, sizeof(adv_data), ble_at_cmd_cb);

	//wait for ble_at_cmd_cb

	const uint8_t scan_data[] = {0x02, 0x01, 0x06, 0x0A, 0x09, 0x37 0x32, 0x33, 0x31, 0x4e, 0x5f, 0x42, 0x4c, 0x45};
	bk_ble_set_scan_rsp_data(actv_idx, scan_data, sizeof(scan_data), ble_at_cmd_cb);

	//wait for ble_at_cmd_cb

	bk_ble_start_advertising(actv_idx, 0, ble_at_cmd_cb);




- start scan

::

	ble_scan_param_t scan_param;

	scan_param.own_addr_type = 0;//BLE_STATIC_ADDR
	scan_param.scan_phy = 5;
	scan_param.scan_intv = 0x64; //interval
	scan_param.scan_wd = 0x1e; //windows
	actv_idx = bk_ble_get_idle_actv_idx_handle();
	bk_ble_create_scaning(actv_idx, &scan_param, ble_at_cmd);

	//wait for ble_at_cmd_cb
	bk_ble_start_scaning(actv_idx, ble_at_cmd);




- start a connect

::

	ble_conn_param_t conn_param;
	conn_param.intv_min = 0x40; //interval
	conn_param.intv_max = 0x40; //interval
	conn_param.con_latency = 0;
	conn_param.sup_to = 0x200;//supervision timeout
	conn_param.init_phys = 1;// 1M
	con_idx = bk_ble_get_idle_conn_idx_handle();
	bk_ble_create_init(con_idx, &conn_param, ble_at_cmd);
	//wait for ble_at_cmd_cb
	bk_ble_init_set_connect_dev_addr(con_idx, bt_mac, 1);
	bk_ble_init_start_conn(con_idx, ble_at_cmd)




API Reference
----------------------------------------

.. include:: ../../_build/inc/ble.inc

API Typedefs
----------------------------------------
.. include:: ../../_build/inc/ble_types.inc
