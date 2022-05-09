#include "base64_enc.h"
#include <string.h>
#include <common/bk_include.h>

#if _USE_BASE64_

/* ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/ */
static const unsigned char base64_table[64] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
        'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',
        'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a',
        'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
        'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's',
        't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1',
        '2', '3', '4', '5', '6', '7', '8', '9', '+',
        '/'
                                              };
/**
 *calculate a string base64_encode length
 *
 */
unsigned int calc_Base64_encode_length(unsigned int src_Len)
{
    unsigned int enc_len = 0;
    enc_len = src_Len * 4 / 3 + 4; /* 3-byte blocks to 4-byte */
    enc_len += enc_len / 72; /* line feeds */
    enc_len++; /* nul termination ('\0') */
    return enc_len;
}

/**
 * Base64_encode - Base64 encode
 * @src: Data to be encoded
 * @len: Length of the data to be encoded
 * @out_len: Pointer to output length variable, or %NULL if not used
 * @out:Pointer to save encoding buf
 * Returns:1:succeed,0:failed
 */
unsigned char Base64_encode(const unsigned char *src, int len,
                            int *out_len, unsigned char *out)
{
    unsigned char  *pos;
    const unsigned char *end, *in;
    int line_len;

    if(out == ((unsigned char *)0))
        return 0;

    end = src + len;
    in = src;
    pos = out;
    line_len = 0;
    while (end - in >= 3)
    {
        *pos++ = base64_table[in[0] >> 2];
        *pos++ = base64_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
        *pos++ = base64_table[((in[1] & 0x0f) << 2) | (in[2] >> 6)];
        *pos++ = base64_table[in[2] & 0x3f];
        in += 3;
        line_len += 4;
        if (line_len >= 72)
        {
            *pos++ = '\n';
            line_len = 0;
        }
    }

    if (end - in)
    {
        *pos++ = base64_table[in[0] >> 2];
        if (end - in == 1)
        {
            *pos++ = base64_table[(in[0] & 0x03) << 4];
            *pos++ = '=';
        }
        else
        {
            *pos++ = base64_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
            *pos++ = base64_table[(in[1] & 0x0f) << 2];
        }
        *pos++ = '=';
        line_len += 4;
    }

    if (line_len)
        *pos++ = '\n';

    *pos = '\0';
    if (out_len)
        (*out_len) = pos - out;
    return 1;
}

/**
 *calculate a string base64_encode length
 *
 */
unsigned int calc_Base64_decode_length(const unsigned char *src, unsigned int src_Len)
{
    unsigned int dec_len;
    unsigned char dtable[256];
    unsigned int i;

    memset(dtable, 0x80, 256);
    for (i = 0; i < sizeof(base64_table); i++)
        dtable[base64_table[i]] = i;
    dtable['='] = 0;

    dec_len = 0;
    for (i = 0; i < src_Len; i++)
    {
        if (dtable[src[i]] != 0x80)
            dec_len++;
    }
    if (dec_len % 4)
        dec_len = 0;
    return dec_len;
}


/**
 * base64_decode - Base64 decode
 * @src: Data to be decoded
 * @len: Length of the data to be decoded
 * @out_len: Pointer to output length variable
 * @out:Pointer to save encoding buf
 * Returns:1:succeed,0:failed
 */
unsigned char Base64_decode(const unsigned char *src, int len,
                            int *out_len, unsigned char *out)
{
    unsigned char dtable[256], *pos, in[4], block[4], tmp;
    unsigned int i, count;

    memset(dtable, 0x80, 256);
    for (i = 0; i < sizeof(base64_table); i++)
        dtable[base64_table[i]] = i;
    dtable['='] = 0;

    count = 0;
    for (i = 0; i < len; i++)
    {
        if (dtable[src[i]] != 0x80)
            count++;
    }

    if (count % 4)
        return 0;
    if (out == ((unsigned char *)0))
    {
        *out_len = (count * 3) / 4;
        return 0;
    }
    pos = out;

    count = 0;
    for (i = 0; i < len; i++)
    {
        tmp = dtable[src[i]];
        if (tmp == 0x80)  //'\n'
            continue;

        in[count] = src[i];
        block[count] = tmp;
        count++;
        if (count == 4)
        {
            *pos++ = (block[0] << 2) | (block[1] >> 4);
            *pos++ = (block[1] << 4) | (block[2] >> 2);
            *pos++ = (block[2] << 6) | block[3];
            count = 0;
        }
    }

    if (pos > out)
    {
        if (in[2] == '=')
            pos -= 2;
        else if (in[3] == '=')
            pos--;
    }

    (*out_len) = pos - out;
    return 1;
}


#endif
// EOF

