/**
 ****************************************************************************************
 *
 * @file fuc_convert.h
 *
 * @brief This file contains fuc_convert.
 * @author zhengyaohan
 * @date 2021-11-10
 *
 ****************************************************************************************
 */

#ifndef _FUNC_CONVERT_H_
#define _FUNC_CONVERT_H_


void uint16_to_array(UINT16 val, UINT8 * array);
UINT16 array_to_uint16(UINT8 * array);
void uint32_to_array(UINT32 val, UINT8 * array);
UINT32 array_to_uint32(UINT8 * array);


UINT8 char_to_int(char val);
UINT32 str_to_hexnum(char * str);
UINT32 str_to_hexarray(char * str, UINT8 * hex);

#endif // _FUNC_CONVERT_H_
// eof

