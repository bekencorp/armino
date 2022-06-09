mic_data.pcm is the original mic data.
encoder_mic_data.pcm is the encoded mic data by G711A.

test step:
(1)Copy the mic_data.pcm and encoder_mic_data.pcm to sdcard.
(2)Send AT command "g711_encoder_test" to excute encoder test case, The case will output result to out_encoder_data.pcm.
(3)Send AT command "g711_decoder_test" to excute decoder test case, The case will output result to out_decoder_data.pcm.
(4)If encoder_mic_data.pcm is the same as out_encoder_data.pcm, the encoder test case pass.
(5)If mic_data.pcm is the same as out_decoder_data.pcm, the decoder test case pass.
