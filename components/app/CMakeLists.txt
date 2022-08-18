
#TODO should move all public include dir to modules

set(incs
	video_work
	net_work
	)

set(srcs
	video_work/video_transfer_tcp.c
	video_work/video_transfer_udp.c
	video_work/video_buffer.c
	net_work/video_demo_main.c
	net_work/video_demo_station.c
	net_work/video_demo_softap.c
	)
if (CONFIG_APP_DEMO_VIDEO_TRANSFER)
	list(APPEND srcs
	video_work/video_upd_spd.c
	video_work/video_upd_spd_pub.c
	)
endif()
if (CONFIG_COMPONENTS_P2P)
	list(APPEND srcs
	net_work/video_demo_p2p.c
        )
endif()

if (CONFIG_HOST_NONE)
	list(APPEND incs)
endif()

#TODO fix main
armino_component_register(SRCS "${srcs}"
	INCLUDE_DIRS "${incs}"
	REQUIRES bk_wifi bk_cli lwip_intf_v2_0 at_server bk_rtos
	PRIV_REQUIRES bk_common audio_transfer aud_intf
	)
