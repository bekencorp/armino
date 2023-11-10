## Project：av_server

## Life Cycle：2023-06-16 ~~ 2023-12-06

## Application：
	This demo is used as the server side of the dual board pairing. The server 
provides a AP, sends the voice data collected by the mic to the client peer, 
and at the same time receives the voice data transmitted by the client peer and 
sends it to the speaker for playback. In addition, the server receives the image 
transmission data sent by the client and displays it on the local screen.

## Special Macro Configuration Description：
CONFIG_MEDIA=y                // support media service
CONFIG_WIFI_TRANSFER=y        // support wifi transfer encode frame
CONFIG_IMAGE_STORAGE=y        // support capture frame and save to sdcard
CONFIG_JPEG_YUV_MIX=y         // support dvp jpeg yuv by order function
CONFIG_LCD=y                  // support LCD Disply
CONFIG_AUD_INTF=y             // support audio

## Complie Command:
1、make bk7256 PROJECT=media/av_server

## CPU:
1、bk7256: cpu0+cpu1

## Media: video audio lcd

## WIFI: AP
