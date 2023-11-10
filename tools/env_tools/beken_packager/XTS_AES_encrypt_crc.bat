XTS_AES_encrypt.exe genkey  -version 123 -outfile bk7256.txt
XTS_AES_encrypt.exe encrypt -infile xaes.bin -startaddress 0x0 -keyfile 123_bk7256.txt -outfile xaes_xts.bin
encrypt_crc_enc.exe xaes_xts_666.bin 0 0 0 0 0
pause