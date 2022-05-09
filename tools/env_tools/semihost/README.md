# Semihost

Semihost is used when host implements lwip at its own, and firmware takes response of 802.11 SME/MLME, all its network frames except EAPoL frames are passed up to host.

## Usage

### Firmware

* modify `CFG_HOSTED=CONFIG_SEMI_HOSTED` in `sys_config_bk7231u.h`.

```c
/* Host based SDIO/USB/PCI/SPI Wifi Support */
/* fully hosted: no lwip, all mgmt located at host */
#define CONFIG_FULLY_HOSTED                               1
/* semi hosted: no lwip, all mgmt located at host */
#define CONFIG_SEMI_HOSTED                                2
/* CFG_HOSTED: 0 disabled, CONFIG_FULLY_HOSTED, CONFIG_SEMI_HOSTED */
#define CFG_HOSTED                                 CONFIG_SEMI_HOSTED
```

* compile

```sh
$ make bk7231u -j8   # add V=1 to show verbose log
```

* burn firmware via GUI or CLI
* run firmware

It's high recommended to change the default mac address, now login into the demo board.

```shell
# mac c8478c723101                     # change mac address to c8:47:8c:72:31:01
```



### Host

This demo only runs on Linux. MacOS may also works.

* compile serial-tap

```shell
$ cd tools/semihost/serial-tap
$ cmake .
$ make
```

This will generate an excutable binary named `serial_tun`.

* setup a tap device

```shell
$ sudo ./setup-tapif   # add a tap0 network adapter
$ sudo ip link set address c8:47:8c:72:31:01 dev tap0     # set tap0's mac address equals to firmware
$ sudo ./serial_tun -i tap0 -p /dev/ttyUSB0
```

If acts as a softap, you must change tap0's mac address the same as softap's mac address, and setup a dhcp server. For example, udhcpd can be used.

```
$ sudo udhcpd udhcpd.conf    # setup an dhcp server
```



### Limitation

Now `UART` is used as the physical layer, and the throughput is very low due to UART's low rate.



## Debug

* Tools such as wireshark, tcpdump can be used to analysis packet at link layer.
* TAP devices' mac address must equal to firmware's mac address

