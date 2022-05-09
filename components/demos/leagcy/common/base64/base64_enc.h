#ifndef _BSAE64_ENC_H_
#define _BSAE64_ENC_H_

#ifdef _USE_BASE64_
unsigned int calc_Base64_encode_length(unsigned int src_Len);
unsigned char Base64_encode(const unsigned char *src, int len,
                            int *out_len, unsigned char *out);

unsigned int calc_Base64_decode_length(const unsigned char *src, unsigned int src_Len);
unsigned char Base64_decode(const unsigned char *src, int len,
                            int *out_len, unsigned char *out);
#endif /*_USE_BASE64_*/
#endif /*_base64_Enc_H_*/

