mic.pcm is the mic data to be processed.
ref.pcm is the reference data.
out_cmpa.pcm is the demo output data.

Copy the mic.pcm and ref.pcm to sdcard, and send AT command "aec_test" to excute test case.
The case will output result to out.pcm in sdcard.
If out.pcm is the same as out_cmpa.pcm, the test case pass.
