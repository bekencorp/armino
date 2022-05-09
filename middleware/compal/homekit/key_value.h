/**
 ****************************************************************************************
 *
 * @file key_value.h
 *
 * @brief This file contains key_value.
 * @author zhengyaohan
 * @date 2021-10-29
 *
 ****************************************************************************************
 */

#ifndef _KEY_VALUE_H_
#define _KEY_VALUE_H_


#define KV_BASE_ADDR  0x0180000

UINT8 erase_flash(UINT32 addr);
UINT8 write_flash(UINT32 addr ,UINT8 *data,UINT32 len);
UINT8 read_flash(UINT32 addr ,UINT8 *data,UINT32 len);




UINT8 domain_collect(UINT8 domain, UINT32 addr_domain);

UINT8 rm_domain(UINT32 addr_base, UINT8 domain);
UINT8 rm_domain_key(UINT32 addr_base, UINT8 domain, UINT8 key);


UINT8 get_domain_key_value(UINT32 addr_base, UINT8 domain, UINT8 key, UINT8 * buf, UINT32 size,UINT32 *ret_size);
UINT8 set_domain_key_value(UINT32 addr_base, UINT8 domain, UINT8 key, UINT8 * buf, UINT32 size);
UINT32 search_domain_key(UINT32 addr_base, UINT8 domain, UINT8 * key);


#endif // _KEY_VALUE_H_
// eof

