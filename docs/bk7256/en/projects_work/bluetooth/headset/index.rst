Headset
======================================

:link_to_translation:`zh_CN:[中文]`

1 Function Overview
-------------------------------------
    This project show how pad work with main function: bluetooth a2dp sink/avrcp tg ct/ble

2 Code Path
-------------------------------------
	demo：``./projects/bluetooth/headset/main/armino_main.c``

	build cmd：``make bk7256 PROJECT=bluetooth/headset``


3 avrcp cli
-------------------------------------
    You can use cli to test avrcp after connection completed.(Attention: cli's effective depends on the phone and apk.)

    +------------------------------------+-----------------------+
    | AT+BT=AVRCP_CTRL,play              | play                  |
    +------------------------------------+-----------------------+
    | AT+BT=AVRCP_CTRL,pause             | pause                 |
    +------------------------------------+-----------------------+
    | AT+BT=AVRCP_CTRL,next              | next music            |
    +------------------------------------+-----------------------+
    | AT+BT=AVRCP_CTRL,prev              | prev music            |
    +------------------------------------+-----------------------+
    | AT+BT=AVRCP_CTRL,rewind            | rewind                |
    +------------------------------------+-----------------------+
    | AT+BT=AVRCP_CTRL,fast_forward      | fast forward          |
    +------------------------------------+-----------------------+
    | AT+BT=AVRCP_CTRL,vol_up            | volume up in local    |
    +------------------------------------+-----------------------+
    | AT+BT=AVRCP_CTRL,vol_down          | volume down in local  |
    +------------------------------------+-----------------------+
