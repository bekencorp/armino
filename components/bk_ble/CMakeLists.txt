set(incs)

if (CONFIG_BLE_4_2)
	list(APPEND incs
		#ble_v4/include
		#ble_v4/include/bk_private
		../../properties/modules/ble/include/bk_private
		)
elseif (CONFIG_BLE_5_X)
	list(APPEND incs
		#ble_v5/include
		#ble_v5/include/bk_private
		../../properties/modules/ble_5_x_rw/ble_pub/include/bk_private/
		)
elseif (CONFIG_BTDM_5_2)
	list(APPEND incs
		#ble_v5_2/include
		#ble_v5_2/include/bk_private
		../../properties/modules/btdm_5_2_rw/ble_pub/include/bk_private/
		)
endif()


    list(APPEND incs
                .
        )


armino_component_register(INCLUDE_DIRS "${incs}")
