## Project：agora

## Life Cycle：2023-06-16 ~~ 2023-12-06

## Application：
	This demo implements image transmission, voice intercom and local display functions based on the Agora-iot-SDK.

## Special Macro Configuration Description：
CONFIG_AGORA_IOT_SDK=y        // support agora-iot-sdk
CONFIG_MEDIA=y                // support media service
CONFIG_WIFI_TRANSFER=y        // support wifi transfer encode frame
CONFIG_IMAGE_STORAGE=y        // support capture frame and save to sdcard
CONFIG_JPEG_YUV_MIX=y         // support dvp jpeg yuv by order function
CONFIG_LCD=y                  // support LCD Disply
CONFIG_AUD_INTF=y             // support audio

## Complie Command:
1、make bk7256 PROJECT=thirdparty/agora

## CPU:
1、bk7256: cpu0+cpu1

## Media: video audio lcd
1、video only support MJPEG encoder type
2、audio only support 8K sample rate
3、lcd support rotate

## Bluetooth: BLE

## WIFI: STA/AP
