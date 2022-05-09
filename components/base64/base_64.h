#ifndef _BSAE64_ENC_H_
#define _BSAE64_ENC_H_

#define CFG_USE_BASE64        1

#ifdef CFG_USE_BASE64
unsigned int base64_calc_encode_length(unsigned int src_Len);
unsigned char base64_encode(const unsigned char *src, int len,
							int *out_len, unsigned char *out);

unsigned int base64_calc_decode_length(const unsigned char *src, unsigned int src_Len);
unsigned char base64_decode(const unsigned char *src, int len,
							int *out_len, unsigned char *out);
#endif /*CFG_USE_BASE64*/
#endif /*_base64_Enc_H_*/

