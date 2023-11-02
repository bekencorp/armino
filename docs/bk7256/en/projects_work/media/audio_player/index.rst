Audio Player
========================

:link_to_translation:`zh_CN:[中文]`

1 Function Overview
-------------------------------------
	The project shows the function of network music player, which mainly realizes the function of network music download and play, pause, continue to play, set the volume, the previous song and the next song.

2 Code Path
-------------------------------------
	Demo path: ``./projects/media/doorbell/main/src/doorbell_udp_service.c``

	Compile command: ``make bk7256 PROJECT=media/audio_player``

3 Cli Overview
-------------------------------------
	The project first needs to send command ``sta {ssid} {password}`` to connect to the available network, and the commands for other functions are as follows:

	+-----------------------------+------------+
	| audio_player start          | 开始播放   |
	+-----------------------------+------------+
	| audio_player pause          | 暂停播放   |
	+-----------------------------+------------+
	| audio_player play           | 继续播放   |
	+-----------------------------+------------+
	| audio_player {volume_value} | 设置音量   |
	+-----------------------------+------------+
	| audio_player last           | 上一曲     |
	+-----------------------------+------------+
	| audio_player next           | 下一曲     |
	+-----------------------------+------------+

	The volume value ranges from 0x00 to 0x3F, and the current volume value is 0x2D by default.