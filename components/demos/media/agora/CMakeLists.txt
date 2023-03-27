if (CONFIG_AGORA_IOT_SDK_TEST)
set(incs)
set(srcs)

  list(APPEND incs
    ./
    include
    dvp
  )

	list(APPEND srcs
		"agora_demo.c"
		"amf_ringbuf.c"
		"audio/audio_control.c"
	)
endif()

armino_component_register(SRCS  "${srcs}"  INCLUDE_DIRS  "${incs}" PRIV_REQUIRES media aud_intf bk_common agora-iot-sdk)
