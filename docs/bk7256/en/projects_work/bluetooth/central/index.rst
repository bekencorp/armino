Central project
======================================

:link_to_translation:`zh_CN:[中文]`

1 Function Overview
-------------------------------------
    This project show how pad work with main function: bluetooth a2dp source/avrcp tg ct/ble

2 Code Path
-------------------------------------
	demo：``./projects/bluetooth/central/main/armino_main.c``

	build cmd：``make bk7256 PROJECT=bluetooth/central``

2 project path
----------------------------
 - central: ``project/bluetooth/central``


3 a2dp source cli introduce
-------------------------------------

    +--------------------------------------------------+----------------------+
    | a2dp_player connect <xx:xx:xx:xx:xx:xx>          | connect to soundbar  |
    +--------------------------------------------------+----------------------+
    | a2dp_player disconnect <xx:xx:xx:xx:xx:xx>       | disconnect           |
    +--------------------------------------------------+----------------------+
    | a2dp_player play <xxx.mp3>                       | play mp3             |
    +--------------------------------------------------+----------------------+
    | a2dp_player stop                                 | stop play            |
    +--------------------------------------------------+----------------------+
    | a2dp_player pause                                | pause                |
    +--------------------------------------------------+----------------------+
    | a2dp_player resume                               | resume               |
    +--------------------------------------------------+----------------------+

4 a2dp source test procedure
-------------------------------------

    | 1.Find a sdcard format exfat, copy project/bluetooth/central/1_qcs.mp3 to root dir. (Must be 16bits mp3)
    | 2.Insert sdcard and power on.
    | 3.Make soundbar enter pair mode.
    | 4.Input ``a2dp_player connect xx:xx:xx:xx:xx:xx`` (xx means soundbar addr).
    | 5.Input ``a2dp_player play xxx.mp3``
    | 6.Now you can here soundbar playing music.(You should play after connection complete as soon as possible, and don't stop, ref chapter 5)
    | 7.You can stop or pause when play, play when stop.


5 compatibility describes

    | 1.In music playing scenario, some soundbars(e.g. JBL) would disconnect(JBL would power up bluetooth) link when stop(local a2dp_player stop or peer avdtp suspend) too long. log will show "bt_api_event_cb:Disconnected from xx:xx:xx:xx:xx:xx"
    | 2.Some soundbars(e.g. xiaomi) would not register avrcp playback and could express inconformitily status between local and peer.
    | 3.Some soundbars(e.g. xiaomi) would not report avrcp volume changed evt, so central could not know.

6 Attention
-------------------------------------

    | 1.Log would show "f_mount failed" or "read data crc error" when sdcard failed.

