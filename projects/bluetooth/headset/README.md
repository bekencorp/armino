## Project：headset

## Life Cycle：2023-06-16 ~~ 2023-12-06

## Application：headset

## Special Macro Configuration Description：
CONFIG_AVDTP=y  //support a2dp sink
CONFIG_A2DP=y
CONFIG_A2DP_SINK=y

## Complie Command:	
1、make bk7236 PROJECT=bluetooth/headset

## CPU: riscv

## RAM:
mem_type start      end        size    
-------- ---------- ---------- --------
itcm     0x10000000 0x100007c6 1990    
dtcm     0x20000400 0x20001de8 6632    
ram      0x3000c800 0x3001d3c0 68544   
data     0x3000c800 0x3000df00 5888    
bss      0x3000df00 0x3001d3c0 62656   
heap     0x38000000 0x38040000 262144  
psram    0x60700000 0x60800000 1048576 

## Bluetooth: BLE and BT
