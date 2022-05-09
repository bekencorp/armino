

/**
 ****************************************************************************************
 *
 * @file fuc_convert.c
 *
 * @brief This file contains fuc_convert.
 * @author zhengyaohan
 * @date 2021-11-10
 *
 ****************************************************************************************
 */
#include <string.h>
#include "common/bk_include.h"





/**
 ************************************************************
 * @brief uint16_to_array.
 *
 * @param[in] 
 * @param[out] 
 * @return 
 *************************************************************
 */
void uint16_to_array(UINT16 val, UINT8 * array)
{

    *array = val;
    array++;
    *array = val >> 8;
}


/**
 ************************************************************
 * @brief array_to_uint16.
 *
 * @param[in] 
 * @param[out] 
 * @return 
 *************************************************************
 */
UINT16 array_to_uint16(UINT8 * array)
{
    UINT16 val;

    val = * (array + 1);
    val <<= 8;
    val += (*array);
    return val;
}


/**
 ************************************************************
 * @brief uint32_to_array.
 *
 * @param[in] 
 * @param[out] 
 * @return 
 *************************************************************
 */
void uint32_to_array(UINT32 val, UINT8 * array)
{
    UINT8 idx;

    for (idx = 0; idx < 4; idx++)
    {
        *array = val;
        array++;
        val >>= 8;
    }

}


/**
 ************************************************************
 * @brief array_to_uint32.
 *
 * @param[in] 
 * @param[out] 
 * @return 
 *************************************************************
 */
UINT32 array_to_uint32(UINT8 * array)
{
    UINT32 val = 0, idx;

    for (idx = 0; idx < 4; idx++)
    {
        val <<= 8;
        val += (* (array + 3 - idx));
    }
    return val;
}






/**
 ************************************************************
 * @brief char_to_int.
 *
 * @param[in] 
 * @param[out] 
 * @return 
 *************************************************************
 */
UINT8 char_to_int(char val)
{
    UINT8 ret = 0;

    if ((val >= '0') && (val <= '9'))
        ret = val - '0';
    else 
        if ((val >= 'a') && (val <= 'f'))
            ret = val - 'a' + 10;
    else 
        if ((val >= 'A') && (val <= 'F'))
            ret = val - 'A' + 10;
    return ret;
}


/**
 ************************************************************
 * @brief str_to_hex.
 *
 * @param[in] 
 * @param[out] 
 * @return 
 *************************************************************
 */
UINT32 str_to_hexnum(char * str)
{
    UINT32 len, idx, val;


    len = strlen(str);
    if (len > 8)
        len = 8;

    val = 0;

    for (idx = 0; idx < len; idx++)
    {
        val *= 16;
        val += (char_to_int(* (str + idx)));
    }

    return val;
}


/**
 ************************************************************
 * @brief str_to_hexarray.
 *
 * @param[in] 
 * @param[out] 
 * @return 
 *************************************************************
 */
UINT32 str_to_hexarray(char * str, UINT8 * hex)
{
    UINT32 len, idx, ret = 0;


    len = strlen(str);

    for (idx = 0; idx < len; )
    {
        *hex = char_to_int(* (str + idx));
        *hex = (*hex) << 4;
        idx++;
        if (idx >= len)
            break;

        *hex = (*hex) +char_to_int(* (str + idx));
        idx++;

        hex++;
        ret++;
    }

    return ret;
}









