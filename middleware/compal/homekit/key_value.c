

/**
 ****************************************************************************************
 *
 * @file key_value.c
 *
 * @brief This file contains key_value.
 * @author zhengyaohan
 * @date 2021-10-29
 *
 ****************************************************************************************
 */
#include "common/bk_include.h"
#if CONFIG_FLASH_ORIGIN_API
#include "bk_flash.h"
#else
#include "driver/flash.h"
#include <driver/flash_partition.h>
#endif

#include "bk_drv_model.h"
#include <os/mem.h>
#include <os/os.h>




#define SECTOR_SIZE             0x1000
#define DOMAIN_SECTOR_SIZE      0x2000
#define DOMAIN_OFFSET           0x0A

#define KEY_OFFSET              0x400
#define KEY_MIN_SIZE            0x20

#define AUTH_USED_KEY           0xAA
#define AUTH_KEY                0xAF 




enum KEY_CMD
{
    KEY_CMD_NONE = 0, 
    KEY_CMD_RM, 
    KEY_CMD_RENEW, 
};


typedef struct 
{
    UINT8 auth;
    UINT8 domain;
    UINT8 offset;
} DOMAIN_HEAD_T;


typedef struct 
{
    UINT8 auth;
    UINT8 key;
    UINT8 in_offset;
    UINT8 in_count;
} KEY_HEAD_T;


/**
 ************************************************************
 * @brief erase_flash.
 *
 * @param[in] 
 * @param[out] 
 * @return 
 *************************************************************
 */
UINT8 erase_flash(UINT32 addr)
{
#if CONFIG_FLASH_ORIGIN_API
    UINT32 status, protect_flag, protect_param;
    DD_HANDLE flash_hdl;

    addr = (addr & (~0x0000fff));
    flash_hdl = ddev_open(DD_DEV_TYPE_FLASH, &status, 0);
    ddev_control(flash_hdl, CMD_FLASH_GET_PROTECT, &protect_flag);
    protect_param = FLASH_PROTECT_NONE;
    ddev_control(flash_hdl, CMD_FLASH_SET_PROTECT, &protect_param);
    ddev_control(flash_hdl, CMD_FLASH_ERASE_SECTOR, &addr);
    ddev_control(flash_hdl, CMD_FLASH_SET_PROTECT, &protect_flag);
#else
    addr = (addr & (~0x0000fff));
    flash_protect_type_t protect_type;
    protect_type = bk_flash_get_protect_type();
    bk_flash_set_protect_type(FLASH_PROTECT_NONE);
    bk_flash_erase_sector(addr);
    bk_flash_set_protect_type(protect_type);
#endif

    return 1;
}

/**
 ************************************************************
 * @brief wirte_flash.
 *
 * @param[in] 
 * @param[out] 
 * @return 
 *************************************************************
 */
UINT8 write_flash(UINT32 addr ,UINT8 *data,UINT32 len)
{
#if CONFIG_FLASH_ORIGIN_API
    UINT32 status, protect_flag, protect_param,len_max,addr_t;
    DD_HANDLE flash_hdl;
    
    flash_hdl = ddev_open(DD_DEV_TYPE_FLASH, &status, 0);
    ddev_control(flash_hdl, CMD_FLASH_GET_PROTECT, &protect_flag);
    protect_param = FLASH_PROTECT_NONE;
    ddev_control(flash_hdl, CMD_FLASH_SET_PROTECT, &protect_param);
#else
    UINT32 len_max,addr_t;
    flash_protect_type_t protect_type;
    protect_type = bk_flash_get_protect_type();
    bk_flash_set_protect_type(FLASH_PROTECT_NONE);
#endif

    while(len>0)
    {
        if(len>0x1000)
        {
            len_max = 0x1000;
        }
        else
        {
            len_max=len;
        }
        if( (addr+len_max)>=((addr & (~0x0000fff))+0x1000) )
        {
            addr_t = (addr & (~0x0000fff))+0x1000;
#if CONFIG_FLASH_ORIGIN_API
            ddev_control(flash_hdl, CMD_FLASH_ERASE_SECTOR, &addr_t);
#else
            bk_flash_erase_sector(addr_t);
#endif
        }
#if CONFIG_FLASH_ORIGIN_API
        ddev_write(flash_hdl, (char *) data, len_max, addr);
#else
	bk_flash_write_bytes(addr, (const uint8_t *)data, len_max);
#endif
        addr+=len_max;
        len-=len_max;
    }
    
#if CONFIG_FLASH_ORIGIN_API
    ddev_control(flash_hdl, CMD_FLASH_SET_PROTECT, &protect_flag);    
#else
    bk_flash_set_protect_type(protect_type);
#endif

    return 1;
}

/**
 ************************************************************
 * @brief read_flash.
 *
 * @param[in] 
 * @param[out] 
 * @return 
 *************************************************************
 */
UINT8 read_flash(UINT32 addr ,UINT8 *data,UINT32 len)
{
#if CONFIG_FLASH_ORIGIN_API
    UINT32 status;
    DD_HANDLE flash_hdl;
    
    flash_hdl = ddev_open(DD_DEV_TYPE_FLASH, &status, 0);
    ddev_read(flash_hdl, (char *) data, len, addr);  
#else
    bk_flash_read_bytes(addr, (uint8_t *)data, len);
#endif
    return 1;
}


/**
 ************************************************************
 * @brief domain_collect.
 *
 * @param[in] 
 * @param[out] 
 * @return 
 *************************************************************
 */
UINT8 domain_collect(UINT8 domain, UINT32 addr_domain)
{
    UINT8 * ptr, *data;
#if CONFIG_FLASH_ORIGIN_API
    DD_HANDLE flash_hdl;
    UINT32 status, idx, key_addr, key_count = 0, all_len = 0, len = 0, offset = 0, first = 1;
    UINT32 res_len, protect_flag, protect_param;
#else
    UINT32 idx, key_addr, key_count = 0, all_len = 0, len = 0, offset = 0, first = 1;
    UINT32 res_len;
#endif

    ptr = ((UINT8 *) os_malloc(0x400));
    data = ((UINT8 *) os_malloc(0x1000));
    if ((ptr == NULL) || (data == NULL))
    {
        os_printf("domain_collect os_malloc fail.\r\n");
        os_free(ptr);
        os_free(data);
        return 0;
    }
    os_memset(ptr, 0xff, 0x400);
    os_memset(data, 0xff, 0x1000);


#if CONFIG_FLASH_ORIGIN_API
    flash_hdl = ddev_open(DD_DEV_TYPE_FLASH, &status, 0);
    ddev_read(flash_hdl, (char *) ptr, 0x400, addr_domain);
#else
    bk_flash_read_bytes(addr_domain, (uint8_t *)ptr, 0x400);
#endif

    os_printf("domain_collect= %02X: \r\n", domain);

    for (idx = 0; idx < 256; idx++)
    {
        if ((ptr[idx * 4] &0XF0) == 0XA0)
        {
            if (ptr[idx * 4] == AUTH_KEY)
            {
                os_printf("key =  %02X, len = %d; ", ptr[1 + idx * 4],ptr[3 + idx * 4]*KEY_MIN_SIZE);
                if (idx > key_count)
                {
                    os_memcpy(ptr + key_count * 4, ptr + idx * 4, 4);
                }
                key_count++;
            }
        }
        else break;
    }
    os_printf("end.\r\n");

    for (idx = 0; idx < key_count; idx++)
    {
        len = ptr[3 + idx * 4] *KEY_MIN_SIZE;
        all_len += len;
        if (all_len > (DOMAIN_SECTOR_SIZE - KEY_OFFSET - 32))
        {
            os_printf("domain_collect fail.all_len = %d.\r\n", all_len);
            os_free(ptr);
            os_free(data);
            return 0;
        }
    }
    os_printf("domain = %u ,all_len = %d, in collect.\r\n", domain, all_len);

#if CONFIG_FLASH_ORIGIN_API
    ddev_control(flash_hdl, CMD_FLASH_GET_PROTECT, &protect_flag);
    protect_param = FLASH_PROTECT_NONE;
    ddev_control(flash_hdl, CMD_FLASH_SET_PROTECT, &protect_param);
#else
    flash_protect_type_t protect_type;
    protect_type = bk_flash_get_protect_type();
    bk_flash_set_protect_type(FLASH_PROTECT_NONE);
#endif


    for (idx = 0; idx < key_count; idx++)
    {
        key_addr = addr_domain + KEY_OFFSET + ptr[2 + idx * 4] *KEY_MIN_SIZE;
        len = ptr[3 + idx * 4] *KEY_MIN_SIZE;
        if (((offset + len) >= (SECTOR_SIZE - KEY_OFFSET)) && first)
        {
            first = 0;

            res_len = offset + len - ((SECTOR_SIZE - KEY_OFFSET));
#if CONFIG_FLASH_ORIGIN_API
            ddev_read(flash_hdl, (char *) data + offset, (len - res_len), key_addr);
            ddev_control(flash_hdl, CMD_FLASH_ERASE_SECTOR, &addr_domain);
            ddev_write(flash_hdl, (char *) data, (SECTOR_SIZE - KEY_OFFSET), addr_domain + KEY_OFFSET);
#else
            bk_flash_read_bytes(key_addr, (uint8_t *)data + offset, (len - res_len));
            bk_flash_erase_sector(addr_domain);
            bk_flash_write_bytes(addr_domain + KEY_OFFSET, (const uint8_t *)data, (SECTOR_SIZE - KEY_OFFSET));
#endif

            offset = 0;
#if CONFIG_FLASH_ORIGIN_API
            ddev_read(flash_hdl, (char *) data + offset, (res_len), key_addr + res_len);
#else
            bk_flash_read_bytes(key_addr + res_len, (uint8_t *)data + offset, res_len);
#endif
            offset += res_len;
            continue;
        }

#if CONFIG_FLASH_ORIGIN_API
        ddev_read(flash_hdl, (char *) data + offset, len, key_addr);
#else
        bk_flash_read_bytes(key_addr, (uint8_t *)data + offset, len);
#endif
        offset += len;
        if (idx == 0)
        {
            ptr[2 + idx * 4] = 0;
        }
        else 
        {
            ptr[2 + idx * 4] = (ptr[(idx - 1) * 4 + 2] +ptr[(idx - 1) * 4 + 3]);
        }

    }

    if (all_len < (SECTOR_SIZE - KEY_OFFSET))
    {
#if CONFIG_FLASH_ORIGIN_API
        ddev_control(flash_hdl, CMD_FLASH_ERASE_SECTOR, &addr_domain);
        ddev_write(flash_hdl, (char *) data, offset, addr_domain + KEY_OFFSET);
#else
        bk_flash_erase_sector(addr_domain);
        bk_flash_write_bytes(addr_domain + KEY_OFFSET, (const uint8_t *)data, offset);
#endif
    }
#if CONFIG_FLASH_ORIGIN_API
    ddev_write(flash_hdl, (char *) ptr, key_count * 4, addr_domain);
#else
        bk_flash_write_bytes(addr_domain, (const uint8_t *)ptr, key_count * 4);
#endif

    addr_domain += SECTOR_SIZE;
#if CONFIG_FLASH_ORIGIN_API
    ddev_control(flash_hdl, CMD_FLASH_ERASE_SECTOR, &addr_domain);
#else
    bk_flash_erase_sector(addr_domain);
#endif
    if (all_len > (DOMAIN_SECTOR_SIZE - (SECTOR_SIZE - KEY_OFFSET)))
    {
#if CONFIG_FLASH_ORIGIN_API
        ddev_write(flash_hdl, (char *) data, offset, addr_domain);
#else
        bk_flash_write_bytes(addr_domain, (const uint8_t *)data, offset);
#endif
    }

#if CONFIG_FLASH_ORIGIN_API
    ddev_control(flash_hdl, CMD_FLASH_SET_PROTECT, &protect_flag);
#else
    bk_flash_set_protect_type(protect_type);
#endif
    os_free(ptr);
    os_free(data);
    return 1;

}




/**
 ************************************************************
 * @brief get_domain_addr.
 *
 * @param[in] 
 * @param[out] 
 * @return 
 *************************************************************
 */
UINT8 get_domain_addr(UINT32 addr_base, UINT8 domain, UINT8 need, UINT32 * addr)
{
    UINT8 * ptr, find = 0;
#if CONFIG_FLASH_ORIGIN_API
    DD_HANDLE flash_hdl;
    UINT32 status, idx, protect_flag, protect_param,tmp;
#else
    UINT32 idx, tmp;
#endif
    DOMAIN_HEAD_T domain_item;

    ptr = ((UINT8 *) os_malloc(0x400));
    addr_base = addr_base & (~0x0fff);

#if CONFIG_FLASH_ORIGIN_API
    flash_hdl = ddev_open(DD_DEV_TYPE_FLASH, &status, 0);
    ddev_control(flash_hdl, CMD_FLASH_GET_PROTECT, &protect_flag);
    protect_param = FLASH_PROTECT_NONE;
    ddev_control(flash_hdl, CMD_FLASH_SET_PROTECT, &protect_param);
    ddev_read(flash_hdl, (char *) ptr, 0x400, addr_base);
#else
    flash_protect_type_t protect_type;
    protect_type = bk_flash_get_protect_type();
    bk_flash_set_protect_type(FLASH_PROTECT_NONE);
    bk_flash_read_bytes(addr_base, (uint8_t *)ptr, 0x400);
#endif

    if (os_memcmp(ptr, "beken", 5) != 0)
    {
#if CONFIG_FLASH_ORIGIN_API
        ddev_control(flash_hdl, CMD_FLASH_ERASE_SECTOR, (void *) &addr_base);
        ddev_write(flash_hdl, "beken", 5, addr_base);
#else
        bk_flash_erase_sector(addr_base);
        bk_flash_write_bytes(addr_base, (const uint8_t *)"beken", 5);
#endif
        os_memset(ptr + 10, 0xff, 10);
    }

    for (idx = 0; idx < 256; idx++)
    {
        if (ptr[10 + idx * 3] == AUTH_KEY)
        {
            if (ptr[10 + 1 + idx * 3] == domain)
            {
                find = 1;
                *addr = addr_base + SECTOR_SIZE + idx * DOMAIN_SECTOR_SIZE;
                break;
            }
        }
        else break;
    }

    if (need && (find == 0))
    {
        domain_item.auth = AUTH_KEY;
        domain_item.domain = domain;
        domain_item.offset = 0xff;
        *addr = addr_base + SECTOR_SIZE + idx * DOMAIN_SECTOR_SIZE;
#if CONFIG_FLASH_ORIGIN_API
        ddev_control(flash_hdl, CMD_FLASH_ERASE_SECTOR, addr);
        tmp = *addr + SECTOR_SIZE;
        ddev_control(flash_hdl, CMD_FLASH_ERASE_SECTOR, &tmp);
        ddev_write(flash_hdl, (char *) &domain_item, 3, addr_base + 10 + idx * 3);
#else
        bk_flash_erase_sector(*addr);
        tmp = *addr + SECTOR_SIZE;
        bk_flash_erase_sector(tmp);
        bk_flash_write_bytes(addr_base + 10 + idx * 3, (const uint8_t *)&domain_item, 3);
#endif
        find = 1;
    }
#if CONFIG_FLASH_ORIGIN_API
    ddev_control(flash_hdl, CMD_FLASH_SET_PROTECT, &protect_flag);
#else
    bk_flash_set_protect_type(protect_type);
#endif
    os_free(ptr);
    return find;
}


/**
 ************************************************************
 * @brief get_key_addr.
 *
 * @param[in] 
 * @param[out] 
 * @return 
 *************************************************************
 */
UINT8 get_key_addr(UINT32 addr_base, UINT8 key, UINT8 erase_need, UINT32 size, UINT32 * addr)
{
    UINT8 * ptr, find = 0;
#if CONFIG_FLASH_ORIGIN_API
    DD_HANDLE flash_hdl;
    UINT32 status, idx, protect_flag, protect_param;
#else
    UINT32 idx;
#endif
    KEY_HEAD_T key_item;

    ptr = ((UINT8 *) os_malloc(0x400));
    addr_base = addr_base & (~0x0fff);
#if CONFIG_FLASH_ORIGIN_API
    flash_hdl = ddev_open(DD_DEV_TYPE_FLASH, &status, 0);
    ddev_control(flash_hdl, CMD_FLASH_GET_PROTECT, &protect_flag);
    protect_param = FLASH_PROTECT_NONE;
    ddev_control(flash_hdl, CMD_FLASH_SET_PROTECT, &protect_param);
    ddev_read(flash_hdl, (char *) ptr, 0x400, addr_base);
#else
    flash_protect_type_t protect_type;
    protect_type = bk_flash_get_protect_type();
    bk_flash_set_protect_type(FLASH_PROTECT_NONE);
    bk_flash_read_bytes(addr_base, (uint8_t *)ptr, 0x400);
#endif

    for (idx = 0; idx < 256; idx++)
    {
        if ((ptr[idx * 4] &0XF0) == 0XA0)
        {
            if ((ptr[idx * 4] == AUTH_KEY) && (ptr[1 + idx * 4] == key))
            {
                find = 1;
                *addr = addr_base + KEY_OFFSET + ptr[2 + idx * 4] *KEY_MIN_SIZE;

                if (erase_need > 0)
                {
                    key_item.auth = AUTH_USED_KEY;
#if CONFIG_FLASH_ORIGIN_API
                    ddev_write(flash_hdl, (char *) &key_item.auth, 1, addr_base + idx * 4);
#else
                    bk_flash_write_bytes(addr_base + idx * 4, (const uint8_t *)&key_item.auth, 1);
#endif
                }
                if (erase_need <= KEY_CMD_RM) break;
            }
        }
        else break;
    }

    if ((erase_need > KEY_CMD_RM))
    {
        key_item.auth = AUTH_KEY;
        key_item.key = key;
        if (idx > 0)
        {
            key_item.in_offset = (ptr[(idx - 1) * 4 + 2] +ptr[(idx - 1) * 4 + 3]);
        }
        else 
        {
            key_item.in_offset = 0;
        }
        *addr = addr_base + KEY_OFFSET + key_item.in_offset * KEY_MIN_SIZE;
        if ((KEY_OFFSET + key_item.in_offset * KEY_MIN_SIZE) > DOMAIN_SECTOR_SIZE)
        {
            os_printf("!!! key = %02X is full,please erase \r\n", key);
            os_free(ptr);
            return 0;
        }

        key_item.in_count = (size + KEY_MIN_SIZE - 1) / KEY_MIN_SIZE;
#if CONFIG_FLASH_ORIGIN_API
        ddev_write(flash_hdl, (char *) &key_item, 4, addr_base + idx * 4);
#else
        bk_flash_write_bytes(addr_base + idx * 4, (const uint8_t *)&key_item, 4);
#endif
        find = 1;
    }
#if CONFIG_FLASH_ORIGIN_API
    ddev_control(flash_hdl, CMD_FLASH_SET_PROTECT, &protect_flag);
#else
    bk_flash_set_protect_type(protect_type);
#endif
    os_free(ptr);
    return find;
}



/**
 ************************************************************
 * @brief rm_domain_key.
 *
 * @param[in] 
 * @param[out] 
 * @return 
 *************************************************************
 */
UINT8 rm_domain_key(UINT32 addr_base, UINT8 domain, UINT8 key)
{
    UINT32 addr_domain, addr_key;

    os_printf("rm key %02X.%02X . \r\n", domain, key);

    if (get_domain_addr(addr_base, domain, 0, &addr_domain))
    {
        get_key_addr(addr_domain, key, KEY_CMD_RM, 0, &addr_key);
    }
    return 1;
}


/**
 ************************************************************
 * @brief rm_domain.
 *
 * @param[in] 
 * @param[out] 
 * @return 
 *************************************************************
 */
UINT8 rm_domain(UINT32 addr_base, UINT8 domain)
{
#if CONFIG_FLASH_ORIGIN_API
    DD_HANDLE flash_hdl;
    UINT32 status, protect_flag, addr_domain, protect_param, idx;
#else
    UINT32 addr_domain, idx;
#endif

    os_printf("rm domain %02X. \r\n", domain);

#if CONFIG_FLASH_ORIGIN_API
    flash_hdl = ddev_open(DD_DEV_TYPE_FLASH, &status, 0);
#endif

    if (get_domain_addr(addr_base, domain, 0, &addr_domain))
    {
        os_printf("addr_domain= %08X: \r\n", addr_domain);
#if CONFIG_FLASH_ORIGIN_API
        ddev_control(flash_hdl, CMD_FLASH_GET_PROTECT, &protect_flag);
        protect_param = FLASH_PROTECT_NONE;
        ddev_control(flash_hdl, CMD_FLASH_SET_PROTECT, &protect_param);
#else
        flash_protect_type_t protect_type;
        protect_type = bk_flash_get_protect_type();
        bk_flash_set_protect_type(FLASH_PROTECT_NONE);
#endif
        for (idx = 0; idx < (DOMAIN_SECTOR_SIZE / SECTOR_SIZE); idx++)
        {
#if CONFIG_FLASH_ORIGIN_API
            ddev_control(flash_hdl, CMD_FLASH_ERASE_SECTOR, &addr_domain);
            addr_domain += SECTOR_SIZE;
            ddev_control(flash_hdl, CMD_FLASH_ERASE_SECTOR, &addr_domain);
#else
            bk_flash_erase_sector(addr_domain);
            addr_domain += SECTOR_SIZE;
            bk_flash_erase_sector(addr_domain);
#endif
        }
#if CONFIG_FLASH_ORIGIN_API
        ddev_control(flash_hdl, CMD_FLASH_SET_PROTECT, &protect_flag);
#else
        bk_flash_set_protect_type(protect_type);
#endif
    }
    return 1;
}



/**
 ************************************************************
 * @brief get_domain_key_value.
 *
 * @param[in] 
 * @param[out] 
 * @return 
 *************************************************************
 */
UINT8 get_domain_key_value(UINT32 addr_base, UINT8 domain, UINT8 key, UINT8 * buf, UINT32 size, UINT32 * ret_size)
{
#if CONFIG_FLASH_ORIGIN_API
    DD_HANDLE flash_hdl;
    UINT32 addr_domain, status, addr_key, len = 0;
#else
    UINT32 addr_domain, addr_key, len = 0;
#endif



    os_printf("get key %02X.%02X . \r\n", domain, key);

#if CONFIG_FLASH_ORIGIN_API
    flash_hdl = ddev_open(DD_DEV_TYPE_FLASH, &status, 0);
#endif

    if (get_domain_addr(addr_base, domain, 0, &addr_domain))
    {
        if (get_key_addr(addr_domain, key, KEY_CMD_NONE, 0, &addr_key))
        {
#if CONFIG_FLASH_ORIGIN_API
            ddev_read(flash_hdl, (char *) &len, 2, addr_key);
#else
            bk_flash_read_bytes(addr_key, (uint8_t *)&len, 2);
#endif
            os_printf("key %02X.%02X addr = %08X,len=%d. \r\n", domain, key, addr_key, len);
            if (len > size) len = size;
            if (ret_size)
            {
                *ret_size = len;
            }
#if CONFIG_FLASH_ORIGIN_API
            if (buf) ddev_read(flash_hdl, (char *) buf, len, addr_key + 2);
#else
            if (buf) bk_flash_read_bytes(addr_key + 2,  (uint8_t *)buf, len);
#endif
            return 1;
        }
    }
    return 0;
}


/**
 ************************************************************
 * @brief set_domain_key_value.
 *
 * @param[in] 
 * @param[out] 
 * @return 
 *************************************************************
 */
UINT8 set_domain_key_value(UINT32 addr_base, UINT8 domain, UINT8 key, UINT8 * buf, UINT32 size)
{

#if CONFIG_FLASH_ORIGIN_API
    DD_HANDLE flash_hdl;
    UINT32 status, protect_flag, protect_param, addr_domain, addr_key;
#else
    UINT32 addr_domain, addr_key;
#endif

#if CONFIG_FLASH_ORIGIN_API
    flash_hdl = ddev_open(DD_DEV_TYPE_FLASH, &status, 0);
#endif

    os_printf("set key %02X.%02X . \r\n", domain, key);

    if (get_domain_addr(addr_base, domain, 1, &addr_domain))
    {
        if (get_key_addr(addr_domain, key, KEY_CMD_RENEW, size + 2, &addr_key))
        {
            os_printf("key %02X.%02X addr = %08X \r\n", domain, key, addr_key);
#if CONFIG_FLASH_ORIGIN_API
            ddev_control(flash_hdl, CMD_FLASH_GET_PROTECT, &protect_flag);
            protect_param = FLASH_PROTECT_NONE;
            ddev_control(flash_hdl, CMD_FLASH_SET_PROTECT, &protect_param);
            ddev_write(flash_hdl, (char *) &size, 2, addr_key);
            ddev_write(flash_hdl, (char *) buf, size, addr_key + 2);
            ddev_control(flash_hdl, CMD_FLASH_SET_PROTECT, &protect_flag);
#else
            flash_protect_type_t protect_type;
            protect_type = bk_flash_get_protect_type();
            bk_flash_set_protect_type(FLASH_PROTECT_NONE);
            bk_flash_write_bytes(addr_key, (uint8_t *)&size, 2);
            bk_flash_write_bytes(addr_key + 2, (uint8_t *)buf, size);
            bk_flash_set_protect_type(protect_type);
#endif
            return 1;
        }
        else 
        {
            os_printf("!!! domain = %02X is full,domain_collect\r\n", domain);
            if (domain_collect(domain, addr_domain))
            {
                if (get_key_addr(addr_domain, key, KEY_CMD_RENEW, size + 2, &addr_key))
                {
                    os_printf("key %02X.%02X addr = %08X \r\n", domain, key, addr_key);
#if CONFIG_FLASH_ORIGIN_API
                    ddev_control(flash_hdl, CMD_FLASH_GET_PROTECT, &protect_flag);
                    protect_param = FLASH_PROTECT_NONE;
                    ddev_control(flash_hdl, CMD_FLASH_SET_PROTECT, &protect_param);
                    ddev_write(flash_hdl, (char *) &size, 2, addr_key);
                    ddev_write(flash_hdl, (char *) buf, size, addr_key + 2);
                    ddev_control(flash_hdl, CMD_FLASH_SET_PROTECT, &protect_flag);
#else
                    flash_protect_type_t protect_type;
                    protect_type = bk_flash_get_protect_type();
                    bk_flash_set_protect_type(FLASH_PROTECT_NONE);
                    bk_flash_write_bytes(addr_key, (uint8_t *)&size, 2);
                    bk_flash_write_bytes(addr_key + 2, (uint8_t *)buf, size);
                    bk_flash_set_protect_type(protect_type);
#endif
                    return 1;
                }
                else 
                {
                    os_printf("!!! domain = %02X is full,please erase \r\n", domain);
                    return 0;
                }
            }

        }
    }
    return 0;
}


/**
 ************************************************************
 * @brief search_domain_key.
 *
 * @param[in] 
 * @param[out] 
 * @return 
 *************************************************************
 */
UINT32 search_domain_key(UINT32 addr_base, UINT8 domain, UINT8 * key)
{
    UINT8 * ptr;
#if CONFIG_FLASH_ORIGIN_API
    DD_HANDLE flash_hdl;
    UINT32 status, idx, addr_domain, key_count = 0;
#else
    UINT32 idx, addr_domain, key_count = 0;
#endif

    ptr = ((UINT8 *) os_malloc(0x400));

    if (get_domain_addr(addr_base, domain, 1, &addr_domain))
    {
        os_printf("addr_domain= %08X: \r\n", addr_domain);
#if CONFIG_FLASH_ORIGIN_API
        flash_hdl = ddev_open(DD_DEV_TYPE_FLASH, &status, 0);
        ddev_read(flash_hdl, (char *) ptr, 0x400, addr_domain);
#else
        bk_flash_read_bytes(addr_domain, (uint8_t *)ptr, 0x400);
#endif
        os_printf("search_domain %02X: \r\n", domain);

        for (idx = 0; idx < 256; idx++)
        {
            if ((ptr[idx * 4] &0XF0) == 0XA0)
            {
                if (ptr[idx * 4] == AUTH_KEY)
                {
                    os_printf(" %02X, ", ptr[1 + idx * 4]);
                    key[key_count++] = ptr[1 + idx * 4];
                    if (key_count >= 255) break;
                }
            }
            else break;
        }
        os_printf(" end.\r\n");
    }
    os_free(ptr);
    return key_count;
}




