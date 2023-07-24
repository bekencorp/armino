## Project：mesh

## Life Cycle：2023-06-16 ~~ 2023-12-06

## Application：ble mesh

## Special Macro Configuration Description：
CONFIG_BT_MESH_LOW_POWER=y    //support mesh low power node
CONFIG_BT_MESH_FRIEND=y       //support mesh friend node
CONFIG_BT_MESH_PROV=y         //support mesh PROVISION
CONFIG_BT_MESH_PROV_DEVICE=y  //support mesh PROVISION
CONFIG_BT_MESH_PROVISIONER=y  //support mesh PROVISION
CONFIG_BT_MESH_HEALTH_CLI=y   //support mesh health profile

## Complie Command:	
1、make bk7236 PROJECT=bluetooth/mesh

## CPU: riscv

## RAM:
mem_type start      end        size    
-------- ---------- ---------- --------
itcm     0x10000000 0x100007c4 1988    
dtcm     0x20000400 0x20001de8 6632    
ram      0x3000c800 0x3001b6c0 61120   
data     0x3000c800 0x3000e0a0 6304    
bss      0x3000e0a0 0x3001b6c0 54816   
heap     0x38000000 0x38040000 262144  
psram    0x60700000 0x60800000 1048576 

## Bluetooth: BLE
