MP3 APIs
================

:link_to_translation:`en:[English]`

.. important::

   The MP3 API v1.0 is the lastest stable MP3 APIs. It supports MPEG-1, MPEG-2 and MPEG-2.5 standard Layer3 decoding. All new applications should use MP3 API v1.0.

MP3 API Categories
----------------------------
MP3 APIs:
 - :cpp:func:`MP3InitDecoder` - allocate memory for platform-specific data and clear all the user-accessible fields
 - :cpp:func:`MP3FreeDecoder` - free platform-specific data allocated by InitMP3Decoder and zero out the contents of MP3DecInfo struct
 - :cpp:func:`MP3FindSyncWord` - locate the next byte-alinged sync word in the raw mp3 stream
 - :cpp:func:`MP3Decode` - decode one frame of MP3 data
 - :cpp:func:`MP3GetLastFrameInfo` - get info about last MP3 frame decoded (number of sampled decoded, sample rate, bitrate, etc.)
 - :cpp:func:`MP3GetNextFrameInfo` - parse MP3 frame header

API Reference
----------------------------------------

.. include:: ../../_build/inc/mp3dec.inc
