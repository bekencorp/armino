## Project：lvgl/psram_4MB

## Life Cycle：2023-06-12 ~~ 2023-12-12

## Application：：LVGL psram_4MB demo.

## Special Macro Configuration Description：
1、CONFIG_PSRAM_WBD955=y	// use psram type is wbd955.
2、CONFIG_EXTENTED_PSRAM_LAYOUT=y	// user should reconfig psram mem for video

## you should reconfig psram for video[jpeg/lcd_display]
1、fb_layout_t fb_layout[]	// must understand fb_layout_t config, please reference frame_buffer.c

## Complie Command:
1、make bk7256 PROJECT=lvgl/psram_4MB

## Base Config：CPU0

## CPU:
1、BK7256: CPU0

## WIFI: STA/AP

## BT: BLE

## MM: LVGL/LCD