set(incs)
set(srcs)

if (CONFIG_RTP)
list(APPEND incs
	src
	include
	)

list(APPEND srcs
	src/rtcpapppacket.cpp
	src/rtcpbyepacket.cpp
	src/rtcpcompoundpacket.cpp
	src/rtcpcompoundpacketbuilder.cpp
	src/rtcppacket.cpp
	src/rtcppacketbuilder.cpp
	src/rtcprrpacket.cpp
	src/rtcpscheduler.cpp
	src/rtcpsdesinfo.cpp
	src/rtcpsdespacket.cpp
	src/rtcpsrpacket.cpp
	src/rtpcollisionlist.cpp
	src/rtpdebug.cpp
	src/rtperrors.cpp
	src/rtpinternalsourcedata.cpp
	src/rtpipv4address.cpp
	#src/rtpipv6address.cpp
	src/rtpipv4destination.cpp
	#src/rtpipv6destination.cpp
	src/rtplibraryversion.cpp
	src/rtppacket.cpp
	src/rtppacketbuilder.cpp
	src/rtppollthread.cpp
	src/rtprandom.cpp
	src/rtprandomrand48.cpp
	src/rtprandomrands.cpp
	src/rtprandomurandom.cpp
	src/rtpsession.cpp
	src/rtpsessionparams.cpp
	src/rtpsessionsources.cpp
	src/rtpsourcedata.cpp
	src/rtpsources.cpp
	src/rtptimeutilities.cpp
	src/rtpudpv4transmitter.cpp
	#src/rtpudpv6transmitter.cpp 
	src/rtpbyteaddress.cpp
	src/rtpexternaltransmitter.cpp
	src/rtpsecuresession.cpp
	src/rtpabortdescriptors.cpp
	src/rtptcpaddress.cpp
	src/rtptcptransmitter.cpp
	#examples/example1.cpp
	RTP.cpp
	)
endif()

armino_component_register(SRCS "${srcs}"
        INCLUDE_DIRS "${incs}"
        REQUIRES lwip_intf_v2_1 bk_rtos
        PRIV_REQUIRES bk_common include
        )

