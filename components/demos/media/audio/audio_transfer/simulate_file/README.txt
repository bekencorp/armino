g711_decoder_data.pcm is the encoded mic data by G711A.

test step:
(1)Copy the g711_decoder_data.pcm to sdcard.
(2)Send AT command "cpu1 aud_cp1_audio_transfer_test start" to excute test case in cpu1.
(3)Send AT command "aud_cp0_audio_transfer_test start" to excute test case in cpu0.
(4)BK7256 board will play the audio in g711_decoder_data.pcm, and output mic data to g711_encoder_data.pcm.
(5)Wait 10s
(5)Send AT command "aud_cp0_audio_transfer_test stop" to stop test case.
