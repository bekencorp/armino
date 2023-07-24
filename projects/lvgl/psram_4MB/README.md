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

## CPU: riscv

## RAM:
mem_type       start          end              size    
--------       ----------     ----------       --------
itcm           0x10000000     0x10006898       26776   
dtcm           0x20000400     0x20006570       24944   
ram            0x3002f480     0x30037318       32408  
data           0x3002f480     0x30030230       3504    
bss            0x30030230     0x30037318       28904   
heap           0x38000000     0x38040000       262144  
psram          0x60000000     0x602FFFFF       3145728‬ 
psram_heap     0x60300000     0x60400000       1048576‬ 


## WIFI: STA/AP

## BT: BLE

## MM: LVGL/LCD