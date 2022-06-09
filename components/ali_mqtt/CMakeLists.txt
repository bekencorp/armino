
set(incs mqtt )

unset(c_compile_options)
# always set BK_SUPPLICANT macro to indicate changes that modified by BK.
list(APPEND c_compile_options "-DMQTT_COMM_ENABLED")
list(APPEND c_compile_options "-DMQTT_DIRECT")
list(APPEND c_compile_options "-DIOTX_WITHOUT_TLS")
list(APPEND c_compile_options "-DOTA_SIGNAL_CHANNEL=1")


if (CONFIG_ALI_MQTT)
	list(APPEND incs
				mqtt/utils
	)
	list(APPEND srcs
				mqtt/mqtt_client.c
				mqtt/mqtt_instance.c
				mqtt/mqtt_adapter.c
				mqtt/utils/utils_list.c
				mqtt/utils/device.c
				mqtt/utils/report.c
				mqtt/utils/ca.c
				mqtt/utils/guider.c
	)
	list(APPEND srcs
				mqtt/mqtt_test.c
	)
	list(APPEND incs
				mqtt/MQTTPacket
	)
	list(APPEND srcs
				mqtt/MQTTPacket/MQTTConnectClient.c
				mqtt/MQTTPacket/MQTTDeserializePublish.c
				mqtt/MQTTPacket/MQTTPacket.c
				mqtt/MQTTPacket/MQTTSerializePublish.c
				mqtt/MQTTPacket/MQTTSubscribeClient.c
				mqtt/MQTTPacket/MQTTUnsubscribeClient.c
	)
endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES bk_common http)

# set CFLAGS
armino_build_set_property(C_COMPILE_OPTIONS "${c_compile_options}" APPEND)