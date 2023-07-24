
#TODO should move all public include dir to modules

set(incs
	video_work
	net_work
	avi_work
	./
	)

set(srcs "")

if (CONFIG_LVGL_DEMO)
	list(APPEND srcs
		lvgl_demo/lv_example_image_show.c
		lvgl_demo/img_utility.c
	)
endif()

if (CONFIG_VIDEO_AVI)
	list(APPEND srcs
	#avi_work/app_video_store.c
	avi_work/app_jpeg2avi.c
	)
endif()

if (CONFIG_NTP_SYNC_RTC)
	list(APPEND srcs
	time/time.c
	time/ntp.c
	time/app_time_intf.c
	)
endif()

if (CONFIG_LITTLEFS)
	list(APPEND srcs
	littlefs_demo/lfs_example_onchip_flash.c
	)
endif()

if (CONFIG_APP_DEMO_VIDEO_TRANSFER)
	list(APPEND srcs
		video_work/video_transfer_tcp.c
		video_work/video_transfer_udp.c
		video_work/video_upd_spd.c
		video_work/video_upd_spd_pub.c
	)
endif()

if (CONFIG_NET_WORK_VIDEO_TRANSFER)
	list(APPEND srcs
		net_work/video_demo_main.c
		net_work/video_demo_station.c
		net_work/video_demo_softap.c
	)
endif()

if (CONFIG_P2P)
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
	PRIV_REQUIRES bk_common aud_intf include  littlefs
	)
