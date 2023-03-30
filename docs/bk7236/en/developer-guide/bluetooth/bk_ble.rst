Bluetooth Low Energy(BLE)
===========================

:link_to_translation:`zh_CN:[中文]`



Overview
""""""""""""""""""""""""""

This module provides users with interface functions such as scanning, connection, advertising and so on.
BLE consists of one or more tasks that rely on BLE interrupt-driven operation. The current stack size is 3K.
BLE has multiple events and callbacks, which constitute the feedback called by the user.


Role
""""""""""""""""""""""""""
Generally speaking, an actively connecting device is called central/master/client，The connected device is called peripheral/slaver/server.
Once the connection relationship between the two ends is determined, it will basically not change.




Note
""""""""""""""""""""""""""

Most APIs have a callback parameter, and you should wait for the callback to complete before proceeding to the next step.
The processing of callback and event callback should not have blocking operations.
The call stack of callback cannot be too deep.

.. important::
    Try to avoid blocking the BLE task, otherwise there will be abnormal phenomena such as disconnection, failure to scan, and failure to connect.
	
	
Common usage scenarios
""""""""""""""""""""""""""

As a slave, create an ATT database for peer browsing
*******************************************************
BLE uses the ATT database as a double-ended operation entity, and all operations such as reading, writing and notification are performed on the ATT database.
In order to build a standards-compliant database, you need to understand the concepts of services, characteristics, and UUIDs.

- Attribute: A piece of data in the database is called a attribute, which consists of handle, type, and value.
- Services: Each ATT database has one or more services such as HID, HeartRate.
- Characteristic：Each service contains one or more characteristics. For example, HID includes HID map and HID report. The former is a key mapping table, and the latter is a key report. The specific operation is to read the HID map first, and then analyze the HID report according to the map to know the key corresponding to a specific value.
- UUID: The above all exist in the ATT database in the form of attributes. In order to know these special attributes, the UUID value specified by the Bluetooth standard is used to assign the type of the attribute. For example, DECL_PRIMARY_SERVICE_128 (0x2800) indicates that this attribute is a service declaration.


The following are specific examples
::

	//service definition
	#define DECL_PRIMARY_SERVICE_128     {0x00,0x28,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
	//characteristic definition
	#define DECL_CHARACTERISTIC_128      {0x03,0x28,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
	//Client Characteristic Configuration declaration。This is a special UUID, indicating that this attribute is used to configure the described characteristics having notification or indication function.
	#define DESC_CLIENT_CHAR_CFG_128     {0x02,0x29,0,0,0,0,0,0,0,0,0,0,0,0,0,0}

	//database subscript:
	enum {
		TEST_IDX_SVC,
		TEST_IDX_CHAR_DECL,
		TEST_IDX_CHAR_VALUE,
		TEST_IDX_CHAR_DESC,

		TEST_IDX_NB,
	};
	//database.
	//BK_BLE_PERM_SET is used to set the permission of the attribute. For example, BK_BLE_PERM_SET(RD, ENABLE) means that this attribute can be read
	ble_attm_desc_t test_service_db[TEST_IDX_NB] = {
	   //  Service Declaration
	   [TEST_IDX_SVC]              = {DECL_PRIMARY_SERVICE_128, BK_BLE_PERM_SET(RD, ENABLE), 0, 0},
	   // Characteristic declaration
	   [TEST_IDX_CHAR_DECL]    = {DECL_CHARACTERISTIC_128,  BK_BLE_PERM_SET(RD, ENABLE), 0, 0},
	   // Characteristic Value declaration,here indicates that this is a characteristic of type 0x1234, BK_BLE_PERM_SET(NTF, ENABLE) indicates that it has the permission to notify
	   // BK_BLE_PERM_SET(RI, ENABLE)Indicates that if this attribute is readable
	   [TEST_IDX_CHAR_VALUE]   = {{0x34, 0x12, 0},     BK_BLE_PERM_SET(NTF, ENABLE), BK_BLE_PERM_SET(RI, ENABLE) | BK_BLE_PERM_SET(UUID_LEN, UUID_16), 128},
	   //Client Characteristic Configuration Descriptor
	   //once the position of notification is set to 1 through this attribute, TEST_IDX_CHAR_VALUE will turn on notification.
	   [TEST_IDX_CHAR_DESC] = {DESC_CLIENT_CHAR_CFG_128, BK_BLE_PERM_SET(RD, ENABLE) | BK_BLE_PERM_SET(WRITE_REQ, ENABLE), 0, 0},
	};


	struct bk_ble_db_cfg ble_db_cfg;
	const uint16_t service_uuid = 0xffff;
	
	ble_db_cfg.att_db = (ble_attm_desc_t *)test_service_db;
	ble_db_cfg.att_db_nb = TEST_IDX_NB;
	//app handle，every time you create a database, it should be different.
	ble_db_cfg.prf_task_id = g_test_prf_task_id;
	ble_db_cfg.start_hdl = 0;
	//The type of UUID，here is 16bit
	ble_db_cfg.svc_perm = BK_BLE_PERM_SET(SVC_UUID_LEN, UUID_16);
	//Copy for service specific values
    os_memcpy(&(ble_db_cfg.uuid[0]), &service_uuid, 2);

	//set callback
	bk_ble_set_notice_cb(ble_at_notice_cb);
	//create database
	bk_ble_create_db(&ble_db_cfg);

At this point, we get a 0xffff service, which contains a 0x1234 characteristic, which can enable notification by setting TEST_IDX_CHAR_DESC to 1.

We also need to handle some events in ble_at_notice_cb.

::

	void ble_at_notice_cb(ble_notice_t notice, void *param)
	{
		switch (notice) {
		//write event from the peer，w_req->att_idx corresponds to the database subscript
		case BLE_5_WRITE_EVENT: {

			if (w_req->prf_id == g_test_prf_task_id)
			{
			    //
				switch(w_req->att_idx)
				{
				case TEST_IDX_CHAR_DECL:
					break;
				case TEST_IDX_CHAR_VALUE:
					break;
				case TEST_IDX_CHAR_DESC:
					//The writing method of TEST_IDX_CHAR_DESC has a standard definition. Here, it is simply considered that as long as it is written, the notification of TEST_IDX_CHAR_VALUE will be turned on.
					//Notify the peer through bk_ble_send_noti_value.
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
		//bk_ble_create_db was created successfully.
		break;
		}
	}


Enable Advertising
****************************************

After setting the database, you need to enable Advertising to allow the peer to discover us

::

	ble_adv_param_t adv_param;

	adv_param.own_addr_type = 0;//BLE_STATIC_ADDR
	adv_param.adv_type = 0; //ADV_IND
	//7(default)
	adv_param.chnl_map = 7;
	adv_param.adv_prop = 3;
	//Minimum advertising interval
	adv_param.adv_intv_min = 0x120; //min
	//Maximum advertising interval.Generally, the smaller the value, the higher the probability of being scanned.
	adv_param.adv_intv_max = 0x160; //max
	adv_param.prim_phy = 1;// 1M
	adv_param.second_phy = 1;// 1M
	
	//Get the currently idle active index to start advertising
	actv_idx = bk_ble_get_idle_actv_idx_handle();
	if (actv_idx != UNKNOW_ACT_IDX) {
		bk_ble_create_advertising(actv_idx, &adv_param, ble_at_cmd_cb);
	}

	//Wait for BLE_CREATE_ADV event in ble_at_cmd_cb
	...
	//

	//BLE advertising data, please refer to BLE standard format
	const uint8_t adv_data[] = {0x02, 0x01, 0x06, 0x0A, 0x09, 0x37 0x32, 0x33, 0x31, 0x4e, 0x5f, 0x42, 0x4c, 0x45};
	bk_ble_set_adv_data(actv_idx, adv_data, sizeof(adv_data), ble_at_cmd_cb);

	//Wait for BLE_SET_ADV_DATA event in ble_at_cmd_cb
	...
	//

	//Scan Response Data，please refer to BLE standard format
	const uint8_t scan_data[] = {0x02, 0x01, 0x06, 0x0A, 0x09, 0x37 0x32, 0x33, 0x31, 0x4e, 0x5f, 0x42, 0x4c, 0x45};
	bk_ble_set_scan_rsp_data(actv_idx, scan_data, sizeof(scan_data), ble_at_cmd_cb);


	//Wait for BLE_SET_RSP_DATA event in ble_at_cmd_cb
	...
	//

	//Enable Advertising
	bk_ble_start_advertising(actv_idx, 0, ble_at_cmd_cb);
	
	//Wait for BLE_START_ADV event in ble_at_cmd_cb
	...
	//



Enable Scan
****************************************

::

	ble_scan_param_t scan_param;

	scan_param.own_addr_type = 0;//BLE_STATIC_ADDR
	scan_param.scan_phy = 5;
	
	//Generally, the smaller the interval, the larger the windows, and the faster to scan data.
	scan_param.scan_intv = 0x64; //scan interval
	scan_param.scan_wd = 0x1e; //scan windows
	//Get the currently idle active index to enable scan
	actv_idx = bk_ble_get_idle_actv_idx_handle();
	bk_ble_create_scaning(actv_idx, &scan_param, ble_at_cmd);

	//Wait for BLE_CREATE_SCAN event in ble_at_cmd_cb
	...
	//
	
	bk_ble_start_scaning(actv_idx, ble_at_cmd);
	
	//Wait for BLE_START_SCAN event in ble_at_cmd_cb
	...
	//
	
	//Process BLE_5_REPORT_ADV in ble_notice_cb_t for advertising data


	
Setup Connection
****************************************

::

	ble_conn_param_t conn_param;
	//Generally, the smaller the interval, the better performance of the link, but the performance of other links, scanning and advertising will be poor.
	conn_param.intv_min = 0x40; //interval
	conn_param.intv_max = 0x40; //interval
	conn_param.con_latency = 0;
	conn_param.sup_to = 0x200;//supervision timeout
	conn_param.init_phys = 1;// 1M
	//Get the currently idle active index to setup connection
	con_idx = bk_ble_get_idle_conn_idx_handle();
	
	
	bk_ble_create_init(con_idx, &conn_param, ble_at_cmd);
	
	//Wait for BLE_INIT_CREATE event in ble_at_cmd_cb
	...
	//
	
	//set the peer address type, mismatch will result in failure to connect
	bk_ble_init_set_connect_dev_addr(con_idx, bt_mac, 1);
	
	
	bk_ble_init_start_conn(con_idx, ble_at_cmd)

	//Wait for BLE_INIT_START_CONN event in ble_at_cmd_cb
	...
	//