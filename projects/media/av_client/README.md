## Project：av_client

## Life Cycle：2023-06-16 ~~ 2023-12-06

## Application：
	This demo is used as the client side of the dual board pairing. The client 
connect AP provided by server, sends the voice data collected by the mic to the server peer, 
and at the same time receives the voice data transmitted by the server peer and 
sends it to the speaker for playback. In addition, the client collect the image 
transmission data and send to the server.

## Special Macro Configuration Description：
CONFIG_MEDIA=y                // support media service
CONFIG_WIFI_TRANSFER=y        // support wifi transfer encode frame
CONFIG_IMAGE_STORAGE=y        // support capture frame and save to sdcard
CONFIG_JPEG_YUV_MIX=y         // support dvp jpeg yuv by order function
CONFIG_LCD=y                  // support LCD Disply
CONFIG_AUD_INTF=y             // support audio

## Complie Command:
1、make bk7256 PROJECT=media/av_client

## CPU:
1、bk7256: cpu0+cpu1

## Media: video audio lcd

## WIFI: AP
