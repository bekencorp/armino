## Project：softap

## Life Cycle：2023-06-06 ~~ 2023-12-06

## Application：SAP supports 10 stations.

## Special Macro Configuration Description：
1、CONFIG_MAX_STA_IN_SAP	// Config SAP supports 10 stations.

## Complie Command:	
1、make bk7256 PROJECT=softap

## Base Config：CPU0+CPU1

## CPU: riscv

## RAM:
mem_type start      end        size    
-------- ---------- ---------- --------
itcm     0x10000000 0x10006aa8 27304   
dtcm     0x20000400 0x20006b38 26424   
ram      0x30020000 0x300787b8 362424  
data     0x30020000 0x30020f88 3976    
bss      0x30020f88 0x300787b8 358448  
heap     0x30021d24 0x30056d24 217088  
psram    0x60700000 0x60800000 1048576 

## WIFI: STA/AP

## BT: BLE

## MM: Doorbell

