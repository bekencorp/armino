系统调试
===================

:link_to_translation:`en:[English]`

Armino平台BK7256系统调试命令
--------------------------------------


 - BK7256 CPU0的log通过串口uart1输出(默认波特率为115200)
 - BK7256 CPU1的log通过mailbox转发到CPU0串口uart1输出(默认波特率为115200)
 - CPU1 log带cpu1标签（异常log除外）
 - 由于内存缓冲区的限制，每条log数据的字节数，要小于128字节。超过这个大小的log都会被shell 模块丢弃，并输出一条 !!some LOGs discarded!! 如果log数量太多，来不及输出导致log堆积，缓冲区用完，也会输出这条提示字符串。
 - 通过串口输入log命令查看当前log配置
 - 通过串口输入log 1 3 1命令设置log为同步机制
 - log 1 3 1 命令第一个参数为echo开关 (0/1)，第二个参数为log级别(0~6)，第三个参数为同步开关(0异步,1同步)
 - 通过串口输入help命令查看当前支持的调试命令：

  ::

    #
    #log 1 3 1
    log: echo 1, level 3, sync 1.

    #
    #help
    ====Build-in Commands====
    help
    log: log [echo(0,1)] [level(0~5)] [sync(0,1)]
    debug: debug cmd [param] (ex:debug help)
    cpu1: cpu1 cmd (ex:cpu1 help)
    loginfo: log statistics.
    modlog: modlog tag_name on/off

    ====User Commands====
    AT: AT
    AT+BLE: AT+TYPE_CMD=CMD_name,param1,...,paramn
    AT+MAC: mac <mac>, get/set mac. e.g. mac c89346000001
    AT+RST: AT+RST
    AT+VIDEO: video cmd(open/read/close)
    AT+WIFIAP: at ap config
    AT+WIFISTA: at sta config
    AT+WIFISTATE: state
    adc_api_test: adc_api_test []
    adc_driver: adc_driver [init/deinit]
    adc_test: adc_test  [channel] [start/stop/dump_statis]
    alpha_blend2: bk_example_alpha_blend
    alpha_fill_blend_p2: bk_example_alpha_blend
    alpha_fill_blend_p3: bk_example_alpha_blend
    alpha_patrtial_blend: bk_example_alpha_blend
    aon_rtc: aon_rtc {id} {create|destroy}
    aon_rtc_driver: aon_rtc_driver {init|deinit}
    aon_rtc_int: aon_rtc_int {id} {tick|upper}
    ap: ap ssid [password]
    assert: asset and dump system information
    aud_adc_dma_test: aud_adc_dma_test {start|stop}
    aud_adc_loop_test: aud_adc_loop_test {start|stop}
    aud_adc_mcp_test: aud_adc_mcp_test {start|stop}
    aud_adc_to_file_test: aud_adc_to_file_test {start|stop}
    aud_adc_to_sd_test: aud_adc_to_sd_test {start|stop}
    aud_cp0_adc_to_sd_test: aud_cp0_adc_to_sd_test {start|stop}
    aud_cp0_mic_to_pcm: aud_cp0_mic_to_pcm {start|stop}
    aud_cp0_psram_init: aud_cp0_psram_init {start|stop}
    aud_cp0_sdcard_play_test: aud_cp0_sdcard_play_test{start}
    aud_dtmf_loop_test: aud_dtmf_loop_test {start|stop}
    aud_dtmf_mcp_test: aud_dtmf_mcp_test {start|stop}
    aud_enable_adc_test: aud_enable_adc_test {start|stop}
    aud_eq_test: aud_eq_test {start|stop}
    aud_mic_to_pcm_test: aud_mic_to_pcm_test {start|stop}
    aud_pcm_dma_test: aud_pcm_dma_test {8000|16000|44100|48000|stop}
    aud_pcm_mcp_test: aud_pcm_mcp_test {8000|16000|44100|48000}
    backtrace: show task backtrace
    ble: ble arg1 arg2
    bootcore1: boot slave core,0:start,1:stop,others:start and stop many times
    cali: cali auto_test
    cca: cca open\close\show
    channel: channel {1~13} - set monitor channel
    cpuload: show task cpu load
    deep_sleep: deep_sleep [param]
    dhcpc: dhcpc
    dma: dma {id} {init|deinit|start|stop|set_tran_len|get_remain_len}
    dma2d_565_to_888: dma2d_pfc = RedBlueSwapConfig
    dma2d_565_to_8888: dma2d_pfc = RedBlueSwapConfig
    dma2d_8888_to_565: dma2d_pfc = RedBlueSwapConfig
    dma2d_888_to_565: dma2d_pfc = RedBlueSwapConfig
    dma2d_mem_to_mem: dma2d_mem_to_mem

    dma2d_memcpy_display: dma2d_mem_to_mem

    dma_driver: dma_driver {init|deinit}
    dma_int: dma_int {id} {reg|enable_hf_fini|disable_hf_fini|enable_fini|disable_fini}
    dvfs: dvfs [cksel_core] [ckdiv_core] [ckdiv_bus] [ckdiv_cpu0] [ckdiv_cpu1]
    efuse: efuse [-r addr] [-w addr data]
    efuse_driver: efuse_driver {init|deinit}
    efuse_test: efuse_test {write|read}
    efusemac: efusemac [-r] [-w] [mac]
    event: event {reg|unreg|post} {mod_id} {event_id}
    exception: {undefine|dabort|illegal|irq|fiq}
    fatfstest: fatfstest <cmd>
    fft_fft_test: fft_fft_test {start|stop}
    fft_ifft_test: fft_ifft_test {start|stop}
    filter: filter <bits> - bit0/d, 1/preq, 2/prsp, 3/b, 4/a
    flash: flash <cmd(R/W/E/N)>
    fmap: flash memory map
    get: get wifi status
    gpio: gpio     [set_mode/output_low/output_high/input/spi_mode]      [id]     [mode]
    gpio_driver: gpio_driver    [init/deinit]}
    gpio_int: gpio_int    [index]     [inttype/start/stop]     [low/high_level/rising/falling edge]
    gpio_map: gpio_map     [sdio_map/spi_map]
    http_ota: http_ota url
    i2c: i2c {init|write|read}
    i2c_driver: i2c_driver {init|deinit}
    ip: ip [sta|ap][{ip}{mask}{gate}{dns}]
    iperf: iperf help
    iplog: iplog [modle]
    jpeg: jpeg {init|deint}
    jpeg_driver: jpeg_driver {init|deinit}
    jtagmode: get jtag mode
    la: bk7256:la rf_adc/fe_adc/rf_dac/fe_dac;bk7236:la rx_adc/rx_dac/tx_dac
    lcd_8080_init: lcd_8080_init {start|stop}

    lcd_close: lcd_close
    lcd_cp0_psram_to_sdcard: lcd_cp0_psram_to_sdcard {start|stop}
    lcd_fill: lcd_dma2d_blending
    lcd_jpegdec: jpeg-jpegdec-lcd
    lcd_rgb_clolor: lcd_rgb_clolor {close|open}
    lcd_rgb_data_test: lcd_rgb_data_test {start|stop}
    lcd_video: lcd_video=96M,8,25
    lcd_video_jpeg_dec: lcd_video_jpeg_dec = 96M,8,25,4
    lcd_video_power: lcd_video_power = on/off
    low_power: low_power [sleep_mode] [wake_source] [vote1] [vote2] [vote3] [param1] [param2]
    low_power_debug: low_power_debug [debug_en_value]
    low_power_vote: low_power_vote [low_power_sleep_mode] [low_power_vote] [low_power_vote_value]
    mac: mac <mac>, get/set mac. e.g. mac c89346000001
    mac_ps: mac_ps {func} [param1] [param2]
    memdump: <addr> <length>
    memleak: [show memleak
    memp: print memp list
    memset: <addr> <value 1> [<value 2> ... <value n>]
    memshow: show free heap
    memstack: show stack memory usage
    micodebug: micodebug on/off
    monitor: monitor {1~13|15|99}
    net: net {sta/ap} ... - wifi net config
    osinfo: show os runtime information
    ping: ping <ip>
    printf_log: printf_log {close|open}
    ps: ps {rfdtim|mcudtim|rf_timer} {1|0}
    psram: psram enable clk_div
    pwm: pwm {chan} {config|start|stop|init|deinit|signal} [...]
    pwm_capture: pwm_capture {chan} {config|start|stop|init|deinit}
    pwm_carrier: pwm_carrier
    pwm_counter: pwm_counter
    pwm_driver: {init|deinit} [26M|DCO]}
    pwm_duty: pwm_duty {chan} {period} {d1} [d2] [d3]
    pwm_group: pwm_group {init|deinit|config|start|stop} [...]
    pwm_int: pwm_int {chan} {reg|enable|disable}
    pwm_timer: pwm_timer
    reboot: reboot system
    regdump: regdump {module}
    regshow: regshow -w/r addr [value]
    rfcali_cfg_mode: 1:manual, 0:auto
    rfcali_cfg_rate_dist: b g n40 ble (0-31)
    rfcali_cfg_tssi_b: 0-255
    rfcali_cfg_tssi_g: 0-255
    rfcali_show_data:
    rxsens: rxsens [-m] [-d] [-c] [-l]
    saradc: start close
    scan: scan [ssid]
    sdtest: sdtest <cmd>
    setjtagmode: reboot system
    spi: spi {init|write|read}
    spi_config: spi_config {id} {mode|baud_rate} [...]
    spi_driver: spi_driver {init|deinit}
    spi_int: spi_int {id} {reg} {tx|rx}
    sta: sta ssid [password][bssid][channel]
    state: state - show STA/AP state
    stop: stop {sta|ap}
    tasklist: list tasks
    testcommonio: test common io
    time: system time
    timer: timer {chan} {start|stop|read} [...]
    touch_multi_channel_scan_mode_test: touch_multi_channel_scan_mode_test {multi_channel_value} {start|stop}
    touch_single_channel_calib_mode_test: touch_single_channel_calib_mode_test {0|1|...|15}
    touch_single_channel_manul_mode_test: touch_single_channel_manul_mode_test {0|1|...|15} {calibration_value}
    trng: trng {start|stop|get}
    trng_driver: {init|deinit}
    txevm: txevm [-m] [-c] [-l] [-r] [-w]
    uart: uart {id} {init|deinit|write|read|write_string|dump_statis} [...]
    uart_config: uart_config {id} {baud_rate|data_bits} [...]
    uart_driver: {init|deinit}
    uart_int: uart_int {id} {enable|disable|reg} {tx|rx}
    vault_driver: vault_driver {init|deinit}
    vault_system: vault_system {version|state|reset}
    version
    video_buffer: open / close / read len
    video_transfer: video_transfer --help
    wdt: wdt {start|stop|feed} [...]
    wdt_driver: {init|deinit}

- 可以通过cpu1 help的命令格式，输入cpu1的调试命令：

  ::

    #cpu1 version

    #cpu1:get_version
    cpu1:firmware version : Apr 14 2022 23:49:08
    #

Armino平台BK7256系统jtag调试
--------------------------------------

 - 需要安装ANDES/AndeSight STD_V5.1.0软件
 - 需要jtag连接线
 - 默认jtag连接cpu0
 - 可以通过setjtagmode 1命令设置jtag连接cpu1
 - 可以通过setjtagmode 0命令设置jtag连接cpu0
 - 可以通过getjtagmode命令查看当前jtag状态


Armino平台BK7256系统异常log分析
--------------------------------------

异常信息中，以如下信息为例：

  ::

    ***********************************************************************************************
    ***********************************user except handler begin***********************************
    ***********************************************************************************************
    Current regs:
    1 ra x 0xcc160
    2 sp x 0x3002573c
    4 tp x 0x0
    5 t0 x 0x5
    6 t1 x 0xcc160
    7 t2 x 0x1880
    10 a0 x 0x5
    11 a1 x 0xcc164
    12 a2 x 0x0
    13 a3 x 0x30019648
    14 a4 x 0x0
    15 a5 x 0x30019638
    16 a6 x 0xff68
    17 a7 x 0x98
    28 t3 x 0x30025980
    29 t4 x 0x200017c4
    30 t5 x 0x1
    31 t6 x 0x81
    32 pc x 0xcc15c
    833 mstatus x 0x1880
    898 mepc x 0xcc160
    899 mcause x 0x5
    2053 mxstatus x 0x30021eb0

    System will dump memory in 5s, please ready to save whole log.........

    [10:04:22.401]收←◆>>>>dump task backtrace begin.
    task               stack_addr              top        size   overflow   backtrace
    IDLE               [0x30011f38 ~ 0x30012338]   0x3001225c   1024   0          2ab18 2aaf0 2a9f2 10b8c 1800 2abc8
    tcp/ip             [0x300221f0 ~ 0x300229f0]   0x3002287c   2048   0          29984 2997e 2a9f2 1880 fffc 10b82 982 104fc 21706 21706 17656 12476
    core_thread        [0x30025120 ~ 0x30025920]   0x300257bc   2048   0          cc164 cc160 cc124 ff68 1880 cf326 ae0f4 ce124 ce464
    wpas_thread        [0x30026960 ~ 0x30027960]   0x3002780c   4096   0          29984 2997e 2a9f2 50f2 1880 256 10b6c 982 dfb50 e1190 4ae df9e2 7e8 30570 e1190 e131a
    Tmr Svc            [0x300112d0 ~ 0x30011ed0]   0x30011dec   3072   0          2b26e 2b268 2a9f2 1880
    cli                [0x30029080 ~ 0x3002a080]   0x30029f2c   4096   0          29ae6 29ae0 2a9f2 1880 b196 792 1cc8
    event              [0x30021590 ~ 0x30021d90]   0x30021c3c   2048   0          29984 2997e 2a9f2 1880 e6b4 982 2b818 28314
    ble                [0x30027c30 ~ 0x30028830]   0x3002870c   3072   0          29984 2997e 2a9f2 fffe faa8 1880 3e8 1005e 982 65b70 59386 593ec
    dhcp-server        [0x3002ae10 ~ 0x3002b290]   0x3002b01c   1152   0          29ae6 29ae0 2a9f2 1880 10b20 792 105a0 21640 11eb0 19578 112 3cf2e 3ceac
    kmsgbk             [0x30023950 ~ 0x30024950]   0x3002484c   4096   0          29ae6 29ae0 2a9f2 1880 10b56 792 ce0c4
    <<<<dump task backtrace end.

    >>>>dump task list begin.
    task           state   pri     water   no
    core_thread     X   7   291 7
    ble             R   5   403 11
    IDLE            R   0   201 2
    wpas_thread     B   4   535 10
    tcp/ip          B   7   315 5
    cli             B   3   763 12
    event           B   0   381 4
    dhcp-server     B   7   43  14
    kmsgbk          B   6   955 6
    Tmr Svc         B   2   711 3
    <<<<dump task list end.


    ***********************************************************************************************
    ************************************user except handler end************************************
    ***********************************************************************************************
    Unhandled Trap : mcause = 0x5, mepc = 0xcc160


比较关键的几个点：

1.  MEPC寄存器：0xcc160
2.  RA寄存器：0xcc160
3.  SP当前栈寄存器：0x3002573c
4.  没有现场的情况下可以根据对应版本的elf文件初步分析：

 ::

    ~/Version$ /opt/risc-v/nds32le-elf-mculib-v5/bin/riscv32-elf-addr2line -piaf -e *.elf cc160
    0x000cc160: rwm_upload_data at /home/jenkins/workspace/ArminoMainCI/properties/modules/rwnx_intf/rw_msdu.c:1299

 - 根据SP指针0x3002573c，得知当前Task为core_thread：

 ::

  core_thread        [0x30025120 ~ 0x30025920]   0x300257bc   2048   0          cc164 cc160 cc124 ff68 1880 cf326 ae0f4 ce124 ce464

 - 这里面的列出来的cc164 cc160 cc124 ff68 1880 cf326 ae0f4 ce124 ce464是任务栈里可能的函数指针：

 ::

    ~/Version$ /opt/risc-v/nds32le-elf-mculib-v5/bin/riscv32-elf-addr2line -piaf -e *.elf cc164 cc160 cc124 ff68 1880 cf326 ae0f4 ce124 ce464
    0x000cc164: rwm_upload_data at /home/jenkins/workspace/ArminoMainCI/properties/modules/rwnx_intf/rw_msdu.c:1299
    0x000cc160: rwm_upload_data at /home/jenkins/workspace/ArminoMainCI/properties/modules/rwnx_intf/rw_msdu.c:1299
    0x000cc124: rwm_upload_data at /home/jenkins/workspace/ArminoMainCI/properties/modules/rwnx_intf/rw_msdu.c:1282
    0x00001880: hint_out at /home/jenkins/workspace/ArminoMainCI/components/bk_cli/shell_task.c:536
    0x000cf326: fhost_rx_desc_handler at /home/jenkins/workspace/ArminoMainCI/properties/modules/rwnx_intf/rwnx_rx.c:230
    0x000ae0f4: macif_rx_app_handler at /home/jenkins/workspace/ArminoMainCI/properties/modules/ip_ax/macsw/modules/macif/src/macif_fhost.c:306
    0x000ce124: bmsg_rx_handler at /home/jenkins/workspace/ArminoMainCI/properties/modules/rwnx_intf/rw_task.c:124
    0x000ce464: core_thread_main at /home/jenkins/workspace/ArminoMainCI/properties/modules/rwnx_intf/rw_task.c:521

