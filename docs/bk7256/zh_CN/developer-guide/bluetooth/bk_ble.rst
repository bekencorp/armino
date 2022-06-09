低功耗蓝牙(BLE)
========================

:link_to_translation:`en:[English]`



概述
""""""""""""""""""""""""""

蓝牙模块向用户提供扫描、连接、广播、传输数据等接口功能，用于短距通讯。
蓝牙由一个或多个task执行体组成，依靠蓝牙中断驱动运行。目前栈大小为3K。
蓝牙有多个event和callback，这些构成了用户调用的反馈。


角色
""""""""""""""""""""""""""
一般来讲，主动连接的设备称之为central/master/client，被连接的设备称之为peripheral/slaver/server。
一旦两端连接关系确定下来，则基本不会变化。




API调用注意事项
""""""""""""""""""""""""""

大部分API具有callback参数，应当等待callback执行完成后再进行下一步。
callback、event callback的处理不应有阻塞操作。
callback的调用栈不能太深。

.. important::
    应极力避免蓝牙task被阻塞，否则会出现断连、扫不到、连不上等异常现象。
	
	
常用使用场景
""""""""""""""""""""""""""

作为slaver，创建ATT数据库供对端浏览
****************************************
ble通过ATT数据库作为双端的操作实体，所有的读写通知等操作都是对ATT数据库进行的。
为了建立一个符合标准的数据库，需要了解服务、特征、UUID的概念。

- 记录：数据库的一条数据称之为记录，由handle，类型、值组成。
- 服务：每个ATT数据库具有一个或多个服务，例如HID、HeartRate。
- 特征：每个服务包含一个或多个特征，例如HID包括HID map、HID report，前者是按键映射表，后者是按键上报，具体操作是先读取HID map，再根据map解析HID report就能知道按键具体值。
- UUID：以上几个均以记录的形式存在于ATT数据库中，为了知晓这些特殊记录，要用蓝牙标准规定的UUID值赋予记录的type。例如，DECL_PRIMARY_SERVICE_128(0x2800)表示这条记录为服务声明。


以下为具体示例
::

	//服务声明
	#define DECL_PRIMARY_SERVICE_128     {0x00,0x28,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
	//特征声明
	#define DECL_CHARACTERISTIC_128      {0x03,0x28,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
	//特征client配置声明。这是一个特殊的UUID，表示这条记录用于配置被描述的特征，一般有notify、indicate
	#define DESC_CLIENT_CHAR_CFG_128     {0x02,0x29,0,0,0,0,0,0,0,0,0,0,0,0,0,0}

	//数据库下标:
	enum {
		TEST_IDX_SVC,
		TEST_IDX_CHAR_DECL,
		TEST_IDX_CHAR_VALUE,
		TEST_IDX_CHAR_DESC,

		TEST_IDX_NB,
	};
	//数据库.
	//BK_BLE_PERM_SET用于设置该记录的权限，例如BK_BLE_PERM_SET(RD, ENABLE)表示这条记录可被读
	ble_attm_desc_t test_service_db[TEST_IDX_NB] = {
	   //  Service Declaration
	   [TEST_IDX_SVC]              = {DECL_PRIMARY_SERVICE_128, BK_BLE_PERM_SET(RD, ENABLE), 0, 0},
	   // Characteristic declare
	   [TEST_IDX_CHAR_DECL]    = {DECL_CHARACTERISTIC_128,  BK_BLE_PERM_SET(RD, ENABLE), 0, 0},
	   // 特征的真实值记录，这里表示这是一个type为0x1234的特征，BK_BLE_PERM_SET(NTF, ENABLE)表示具备notify的权限
	   // BK_BLE_PERM_SET(RI, ENABLE)表示如果这条记录被读，则开启nofity
	   [TEST_IDX_CHAR_VALUE]   = {{0x34, 0x12, 0},     BK_BLE_PERM_SET(NTF, ENABLE), BK_BLE_PERM_SET(RI, ENABLE) | BK_BLE_PERM_SET(UUID_LEN, UUID_16), 128},
	   //Client Characteristic Configuration Descriptor
	   //具备被读被写的权限，一旦通过这条记录把nofify的位置1，则TEST_IDX_CHAR_VALUE会开启notiify。
	   [TEST_IDX_CHAR_DESC] = {DESC_CLIENT_CHAR_CFG_128, BK_BLE_PERM_SET(RD, ENABLE) | BK_BLE_PERM_SET(WRITE_REQ, ENABLE), 0, 0},
	};


	struct bk_ble_db_cfg ble_db_cfg;
	const uint16_t service_uuid = 0xffff;
	
	ble_db_cfg.att_db = (ble_attm_desc_t *)test_service_db;
	ble_db_cfg.att_db_nb = TEST_IDX_NB;
	//app handle，每次创建数据库，应当不同。
	ble_db_cfg.prf_task_id = g_test_prf_task_id;
	ble_db_cfg.start_hdl = 0;
	//服务记录的UUID的类型，这里为16bit
	ble_db_cfg.svc_perm = BK_BLE_PERM_SET(SVC_UUID_LEN, UUID_16);
	//给服务具体值复制
    os_memcpy(&(ble_db_cfg.uuid[0]), &service_uuid, 2);

	//设置回调
	bk_ble_set_notice_cb(ble_at_notice_cb);
	//创建数据库
	bk_ble_create_db(&ble_db_cfg);

到此，我们得到一个0xfff的服务，该服务包含一个0x1234的特征，该特征可以通过读，或写TEST_IDX_CHAR_DESC
来开启notify的行为。

我们还需要在ble_at_notice_cb处理一些事件。

::

	void ble_at_notice_cb(ble_notice_t notice, void *param)
	{
		switch (notice) {
		//对端的写事件，w_req->att_idx对应着数据库下标
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
					//TEST_IDX_CHAR_DESC的写法有标准定义，这里简单地认为只要写入，就开启TEST_IDX_CHAR_VALUE的notify
					//通过bk_ble_send_noti_value notify对端。
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
		//bk_ble_create_db 创建成功。
		break;
		}
	}


开启广播
****************************************

设定好数据库后，需要开启广播以让对端扫描到我们。

::

	ble_adv_param_t adv_param;

	adv_param.own_addr_type = 0;//BLE_STATIC_ADDR
	adv_param.adv_type = 0; //ADV_IND
	//一般为7
	adv_param.chnl_map = 7;
	adv_param.adv_prop = 3;
	//最小interval
	adv_param.adv_intv_min = 0x120; //min
	//最大interval，一般越小，被扫描到的概率越高
	adv_param.adv_intv_max = 0x160; //max
	adv_param.prim_phy = 1;// 1M
	adv_param.second_phy = 1;// 1M
	
	//获取当前空闲的active index，用于开启广播
	actv_idx = bk_ble_get_idle_actv_idx_handle();
	if (actv_idx != UNKNOW_ACT_IDX) {
		bk_ble_create_advertising(actv_idx, &adv_param, ble_at_cmd_cb);
	}

	//在ble_at_cmd_cb中，等待BLE_CREATE_ADV事件
	...
	//

	//蓝牙广播数据，请参考ble标准格式
	const uint8_t adv_data[] = {0x02, 0x01, 0x06, 0x0A, 0x09, 0x37 0x32, 0x33, 0x31, 0x4e, 0x5f, 0x42, 0x4c, 0x45};
	bk_ble_set_adv_data(actv_idx, adv_data, sizeof(adv_data), ble_at_cmd_cb);

	//在ble_at_cmd_cb中，等待BLE_SET_ADV_DATA事件
	...
	//

	//蓝牙广播数据，请参考ble标准格式
	const uint8_t scan_data[] = {0x02, 0x01, 0x06, 0x0A, 0x09, 0x37 0x32, 0x33, 0x31, 0x4e, 0x5f, 0x42, 0x4c, 0x45};
	bk_ble_set_scan_rsp_data(actv_idx, scan_data, sizeof(scan_data), ble_at_cmd_cb);


	//在ble_at_cmd_cb中，等待BLE_SET_ADV_DATA事件
	...
	//

	//开启广播
	bk_ble_start_advertising(actv_idx, 0, ble_at_cmd_cb);
	
	//在ble_at_cmd_cb中，等待BLE_START_ADV事件
	...
	//



开启扫描
****************************************

::

	ble_scan_param_t scan_param;

	scan_param.own_addr_type = 0;//BLE_STATIC_ADDR
	scan_param.scan_phy = 5;
	
	//一般interval越小，windows越大，越有可能扫描到数据
	scan_param.scan_intv = 0x64; //scan interval
	scan_param.scan_wd = 0x1e; //scan windows
	//获取当前空闲的active index，用于开启扫描
	actv_idx = bk_ble_get_idle_actv_idx_handle();
	bk_ble_create_scaning(actv_idx, &scan_param, ble_at_cmd);

	//在ble_at_cmd_cb中，等待BLE_CREATE_SCAN
	...
	//
	
	bk_ble_start_scaning(actv_idx, ble_at_cmd);
	
	//在ble_at_cmd_cb中，等待BLE_START_SCAN
	...
	//
	
	//在ble_notice_cb_t中处理BLE_5_REPORT_ADV ，为广播数据


	
建立连接
****************************************

::

	ble_conn_param_t conn_param;
	//一般interval越小，该链路性能越好，但其他链路、扫描、广播性能会差
	conn_param.intv_min = 0x40; //interval
	conn_param.intv_max = 0x40; //interval
	conn_param.con_latency = 0;
	//超时时间
	conn_param.sup_to = 0x200;//supervision timeout
	conn_param.init_phys = 1;// 1M
	//获取当前空闲的active index，用于开启扫描
	con_idx = bk_ble_get_idle_conn_idx_handle();
	
	
	bk_ble_create_init(con_idx, &conn_param, ble_at_cmd);
	
	//在ble_at_cmd_cb中，等待BLE_INIT_CREATE
	...
	//
	
	//设置对端地址类型，不匹配会导致连接不上
	bk_ble_init_set_connect_dev_addr(con_idx, bt_mac, 1);
	
	
	bk_ble_init_start_conn(con_idx, ble_at_cmd)

	//在ble_at_cmd_cb中，等待BLE_INIT_START_CONN
	...
	//