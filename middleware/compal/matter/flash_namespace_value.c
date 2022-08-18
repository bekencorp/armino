#include<string.h>
#include<stdlib.h>

#include "common/bk_include.h"
#include "common/bk_err.h"
#include <os/mem.h>
#include <os/os.h>
#if CONFIG_FLASH_ORIGIN_API
#include "bk_flash.h"
#else
#include "driver/flash.h"
#include <driver/flash_partition.h>
#endif
#include "bk_drv_model.h"

//#include "wpa_supplicant_i.h"

#include "flash_namespace_value.h"


/*************************************************************************
*************************************************************************
                             Matter flash map
 ---|-----------------------------START----------------------------------------| 
 |  |                                                                          |
 4KB  |
-------------flash_option_arr header[FLASH_NAME_SPACE_MAX_ARRAY]----------| //sizeof(namesapce_data_arr)
 |  |                                                                          |
--
 |  
 |  |                                                                          |
80KB|
-------------flash_option_arr data[FLASH_NAME_SPACE_MAX_ARRAY]------------| //Depends on the actual amount of data
--- |                           4KB * FLASH_NAME_SPACE_MAX_ARRAY               |
--- |--- ----------------------------------------------------------------------| 

--- |----------------------------DATA START ------    -------------------------| 
80KB|--------------------4KB * FLASH_NAME_SPACE_MAX_ARRAY----------------------| 
--- |------------------------------DATA END------------------------------------| 

--- |------------------------------END-----------------------------------------| 
**************************************************************************
*************************************************************************/


/*********************************************************************
 * Funtion Name:get_matter_flash_base_addr
 *
 * Funtion Discription:Get Flash data into global variables
 *
 * 
 *
 * 
 * Date:2021-12-22
 *******************************************************************/
uint32_t bk_strnlen (const char *ucdata,uint32_t dwLenMax) 
{
    uint32_t dwDataLen = 0;

    dwDataLen = strlen(ucdata);

    return (dwDataLen >= dwLenMax ? dwLenMax : dwDataLen );
}

/*************************************************************************
*************************************************************************/
/*********************************************************************
 * Funtion Name:get_matter_flash_base_addr
 *
 * Funtion Discription:Get Flash data into global variables
 *
 * 
 *
 * 
 * Date:2021-12-22
 *******************************************************************/
uint32_t get_matter_flash_base_addr (uint32_t *base_addr) 
{
    bk_logic_partition_t *partition_info = NULL;

    BK_CHECK_POINTER_NULL(base_addr);
#if CONFIG_FLASH_ORIGIN_API    
    partition_info = bk_flash_get_info(BK_PARTITION_MATTER_FLASH);
#else
    partition_info = bk_flash_partition_get_info(BK_PARTITION_MATTER_FLASH);
#endif
    BK_CHECK_POINTER_NULL(partition_info);

    *base_addr = partition_info->partition_start_addr;

    return kNoErr;
}

/*********************************************************************
 * Funtion Name:get_namespace_addr
 *
 * Funtion Discription:Get Flash data into global variables
 *
 * 
 *
 * 
 * Date:2021-12-22
 *******************************************************************/
uint32_t get_namespace_addr ( const char * ucnamespace,uint32_t np_base_addr,uint32_t *addr,uint32_t *block) //base_addr 要用上
{
#if CONFIG_FLASH_ORIGIN_API    
    DD_HANDLE flash_hdl = 0;
    uint32_t status = 0,protect_param = 0,protect_flag = 0;
#endif
    uint32_t dw_start_addr = 0 ;
    namespace_flash_t *namespace_data_arr = NULL;
    uint32_t dw_loop = 0 ;
    uint32_t dw_rtn = 0 ;

    BK_CHECK_POINTER_NULL(ucnamespace);
    BK_CHECK_POINTER_NULL(addr);
    BK_CHECK_POINTER_NULL(block);

    namespace_data_arr = (namespace_flash_t *)os_malloc(NAME_SPACE_FLASH_ACTUAL_USE_SIZE);
    if(NULL == namespace_data_arr)
    {
        bk_printf("[%s] [%d] os_malloc failed \r\n", __FUNCTION__,__LINE__);
        return kGeneralErr;
    }
    bk_flash_debug_printf("ptr = 0x%x size=%d  \r\n",namespace_data_arr,NAME_SPACE_FLASH_ACTUAL_USE_SIZE);
    memset((char *)namespace_data_arr,0,NAME_SPACE_FLASH_ACTUAL_USE_SIZE);
    
    dw_start_addr = np_base_addr;
    
#if CONFIG_FLASH_ORIGIN_API    
    flash_hdl = ddev_open(DD_DEV_TYPE_FLASH, (UINT32 *)&status, 0);
    ddev_control(flash_hdl, CMD_FLASH_GET_PROTECT, &protect_flag);
    protect_param = FLASH_PROTECT_NONE;
    ddev_control(flash_hdl, CMD_FLASH_SET_PROTECT, &protect_param);
    ddev_read(flash_hdl,(char *) namespace_data_arr, NAME_SPACE_FLASH_ACTUAL_USE_SIZE, dw_start_addr);
    ddev_control(flash_hdl, CMD_FLASH_SET_PROTECT, &protect_flag);
#else
    flash_protect_type_t protect_type;
    protect_type = bk_flash_get_protect_type();
    bk_flash_set_protect_type(FLASH_PROTECT_NONE);
    bk_flash_read_bytes(dw_start_addr, (uint8_t *) namespace_data_arr, NAME_SPACE_FLASH_ACTUAL_USE_SIZE);
    bk_flash_set_protect_type(protect_type);
#endif

    for(dw_loop = 0;dw_loop < FLASH_NAME_SPACE_MAX_ARRAY;dw_loop++)
    {
        if(namespace_data_arr[dw_loop].dwUsingFlag != FLASH_SPACE_USING)
        {
            continue;
        }
        
        if(0 == memcmp(namespace_data_arr[dw_loop].ucNamespace,ucnamespace,bk_strnlen(ucnamespace,sizeof(namespace_data_arr[dw_loop].ucNamespace) - 1)))
        {
            if(strlen(namespace_data_arr[dw_loop].ucNamespace) != strlen(ucnamespace))//针对前面的几个字符相同，后面不相同的情况
            {
                continue;
            }
            //*addr  = dw_loop * NAME_FLASH_TOTAL_SIZE + NAME_SPACE_FLASH_TOTAL_SIZE;
            *addr  = namespace_data_arr[dw_loop].dwOffsetByByte;
            *block = dw_loop;//namespace 所在的空间偏移
            dw_rtn =  kNoErr;
            break;
        }
    }

    if(dw_loop == FLASH_NAME_SPACE_MAX_ARRAY)
    {
        dw_rtn = kGeneralErr;
    }

    os_free(namespace_data_arr);
    namespace_data_arr = NULL;
    
    return dw_rtn;
}

/*********************************************************************
 * Funtion Name:get_name_addr
 *
 * Funtion Discription:Get Flash data into global variables
 *
 * 
 *
 * 
 * Date:2021-12-22
 *******************************************************************/
uint32_t get_name_addr ( const char * ucname,uint32_t base_addr,uint32_t *offset_addr ,uint32_t *data_offset_addr)
{
#if CONFIG_FLASH_ORIGIN_API
    DD_HANDLE flash_hdl = 0;
    uint32_t status = 0,protect_param = 0,protect_flag = 0;
#endif
    uint32_t dw_loop = 0 ;
    name_flash_t *name_data_arr = NULL;
    uint32_t dw_rtn = 0 ;

    BK_CHECK_POINTER_NULL(ucname);
    BK_CHECK_POINTER_NULL(offset_addr);
    BK_CHECK_POINTER_NULL(data_offset_addr);

    name_data_arr = (name_flash_t *)os_malloc(NAME_FLASH_TOTAL_SIZE);
    if(NULL == name_data_arr)
    {
        bk_printf("[%s] [%d] os_malloc failed \r\n", __FUNCTION__,__LINE__);
        return kGeneralErr;
    }
    bk_flash_debug_printf("ptr = 0x%x size=%d addr =0x%x \r\n",name_data_arr,NAME_FLASH_TOTAL_SIZE,base_addr);
    memset((char *)name_data_arr,0,NAME_FLASH_TOTAL_SIZE);

#if CONFIG_FLASH_ORIGIN_API    
    flash_hdl = ddev_open(DD_DEV_TYPE_FLASH, (UINT32 *)&status, 0);
    ddev_control(flash_hdl, CMD_FLASH_GET_PROTECT, &protect_flag);
    protect_param = FLASH_PROTECT_NONE;
    ddev_control(flash_hdl, CMD_FLASH_SET_PROTECT, &protect_param);
    ddev_read(flash_hdl,(char *) name_data_arr, NAME_FLASH_TOTAL_SIZE, base_addr);
    ddev_control(flash_hdl, CMD_FLASH_SET_PROTECT, &protect_flag);
#else
    flash_protect_type_t protect_type;
    protect_type = bk_flash_get_protect_type();
    bk_flash_set_protect_type(FLASH_PROTECT_NONE);
    bk_flash_read_bytes(base_addr, (uint8_t *) name_data_arr, NAME_FLASH_TOTAL_SIZE);
    bk_flash_set_protect_type(protect_type);
#endif

    bk_flash_debug_printf("[%s] [%d] data_offset_addr %u %u  \r\n", __FUNCTION__,__LINE__,base_addr,NAME_FLASH_TOTAL_SIZE);

    for(dw_loop = 0;dw_loop < FLASH_NAME_MAX_ARRAY;dw_loop++)
    {
        if(name_data_arr[dw_loop].dwUsingFlag != FLASH_SPACE_USING)//如果不是正在使用的，直接跳到下一次循环
        {
            continue;
        }
        else //name_flash_arr[dw_loop].dwUsingFlag == FLASH_SPACE_USING  //如果此时是正在用的
        {
            // *data_offset_addr = name_flash_arr[dw_loop].dwOffsetByByte + name_flash_arr[dw_loop].dwlength;
            *data_offset_addr = name_data_arr[dw_loop].dwOffsetByByte;
             bk_flash_debug_printf("[%s] [%d] data_offset_addr %u %u  \r\n", __FUNCTION__,__LINE__,dw_loop,*data_offset_addr);
        }

        bk_flash_debug_printf("[%s] [%d] data_offset_addr  %u %x %u \r\n", __FUNCTION__,__LINE__,dw_loop,name_data_arr[dw_loop].dwUsingFlag,name_data_arr[dw_loop].dwOffsetByByte);
        bk_flash_debug_printf("[%s] [%d] data_offset_addr  %s %u \r\n", __FUNCTION__,__LINE__,name_data_arr[dw_loop].ucName,name_data_arr[dw_loop].dwlength);

        if(0 == memcmp(name_data_arr[dw_loop].ucName,ucname,bk_strnlen(ucname,sizeof(name_data_arr[dw_loop].ucName) - 1)))
        {
            if(strlen(name_data_arr[dw_loop].ucName) != strlen(ucname))//针对前面的几个字符相同，后面不相同的情况
            {
                continue;
            }
            *offset_addr  = dw_loop * PER_NAME_FLASH_SIZE;
            //*addr  = name_flash_arr[dw_loop].dwOffsetByByte;
            dw_rtn =  kNoErr;
            break;
        }
    }

    if(dw_loop == FLASH_NAME_MAX_ARRAY)
    {
        dw_rtn = kGeneralErr;
    }

    os_free(name_data_arr);
    name_data_arr = NULL;
    
    return dw_rtn;
}

/*********************************************************************
 * Funtion Name:bk_erase_name_data
 *
 * Funtion Discription:Get Flash data into global variables
 * base_addr --二级目录的地址
 * 
 *
 * 
 * Date:2021-12-22
 *******************************************************************/
uint32_t bk_erase_name_data( uint32_t n_base_addr,uint32_t data_sector_addr)//清除二级目录的数据
{
#if CONFIG_FLASH_ORIGIN_API
    DD_HANDLE flash_hdl = 0;
    uint32_t status = 0,protect_param = 0,protect_flag = 0;

    flash_hdl = ddev_open(DD_DEV_TYPE_FLASH, (UINT32 *)&status, 0);
    ddev_control(flash_hdl, CMD_FLASH_GET_PROTECT, &protect_flag);
    protect_param = FLASH_PROTECT_NONE;
    ddev_control(flash_hdl, CMD_FLASH_SET_PROTECT, &protect_param);
    ddev_control(flash_hdl, CMD_FLASH_ERASE_SECTOR, &n_base_addr);//将这块二级目录的数据erase掉
    ddev_control(flash_hdl, CMD_FLASH_ERASE_SECTOR, &data_sector_addr);//将二级目录对应的数据区erase掉

    ddev_control(flash_hdl, CMD_FLASH_SET_PROTECT, &protect_flag);
#else
    flash_protect_type_t protect_type;
    protect_type = bk_flash_get_protect_type();
    bk_flash_set_protect_type(FLASH_PROTECT_NONE);
    bk_flash_erase_sector(n_base_addr);
    bk_flash_erase_sector(data_sector_addr);
    bk_flash_set_protect_type(protect_type);
#endif

    return kNoErr;
}

/*********************************************************************
 * Funtion Name:bk_reorganize_name_data
 *
 * Funtion Discription:Get Flash data into global variables
 * base_addr --二级目录的地址
 * 
 *
 * 
 * Date:2021-12-22
 *******************************************************************/
uint32_t bk_reorganize_name_data( uint32_t n_base_addr,name_flash_t *name_temp ,char *databuf,uint32_t data_sector_addr,
                                            char *indata,uint32_t dw_data_len,name_flash_t name_data)//清除二级目录的数据
{
#if CONFIG_FLASH_ORIGIN_API
    DD_HANDLE flash_hdl = 0;
    uint32_t status = 0,protect_param = 0,protect_flag = 0;
#endif
    uint32_t dw_loop = 0 ;
    uint32_t dw_data_addr = data_sector_addr;
    uint32_t dw_name_addr = n_base_addr;
    uint32_t dw_offset = 0;//计数
    uint32_t dw_rtn = kNoErr;
    //name_flash_t name_temp_2 = {0};

    BK_CHECK_POINTER_NULL(name_temp);
    BK_CHECK_POINTER_NULL(databuf);
    BK_CHECK_POINTER_NULL(indata);
    
#if CONFIG_FLASH_ORIGIN_API
    flash_hdl = ddev_open(DD_DEV_TYPE_FLASH, (UINT32 *)&status, 0);
    ddev_control(flash_hdl, CMD_FLASH_GET_PROTECT, &protect_flag);
    protect_param = FLASH_PROTECT_NONE;
    ddev_control(flash_hdl, CMD_FLASH_SET_PROTECT, &protect_param);
    ddev_control(flash_hdl, CMD_FLASH_ERASE_SECTOR, &n_base_addr);//将这块二级目录的数据erase掉
    ddev_control(flash_hdl, CMD_FLASH_ERASE_SECTOR, &data_sector_addr);//将二级目录对应的数据区erase掉
 #else
    flash_protect_type_t protect_type;
    protect_type = bk_flash_get_protect_type();
    bk_flash_set_protect_type(FLASH_PROTECT_NONE);
    bk_flash_erase_sector(n_base_addr);
    bk_flash_erase_sector(data_sector_addr);
#endif
   
    for(dw_loop = 0;dw_loop < FLASH_NAME_MAX_ARRAY;dw_loop++)
    {
        if(name_temp[dw_loop].dwUsingFlag == FLASH_SPACE_USING)//表示该段数据有效
        {
#if CONFIG_FLASH_ORIGIN_API
            ddev_write(flash_hdl, (char *)(&databuf[name_temp[dw_loop].dwOffsetByByte]), name_temp[dw_loop].dwlength, dw_data_addr);//先写数据
#else
            bk_flash_write_bytes(dw_data_addr, (uint8_t *)(&databuf[name_temp[dw_loop].dwOffsetByByte]), name_temp[dw_loop].dwlength);
#endif
            dw_data_addr += name_temp[dw_loop].dwlength;
            
            name_temp[dw_loop].dwOffsetByByte = dw_offset;
            dw_offset += name_temp[dw_loop].dwlength;
#if CONFIG_FLASH_ORIGIN_API
            ddev_write(flash_hdl, (char *)(&name_temp[dw_loop]), PER_NAME_FLASH_SIZE, dw_name_addr);//写二级目录的数据
#else
            bk_flash_write_bytes(dw_name_addr, (uint8_t *)(&name_temp[dw_loop]), PER_NAME_FLASH_SIZE);
#endif
            dw_name_addr += PER_NAME_FLASH_SIZE;
        }
    }

    if((dw_offset + dw_data_len) >= FLASH_SECTOR_SIZE)//如果整理之后,还是不够存下，则表明已经满了
    {
        bk_printf("[%s] [%d] mem is full, dw_data_len:%d \r\n", __FUNCTION__,__LINE__, dw_data_len);
        dw_rtn = kGeneralErr;
    }
    else//将数据直接写到更新之后的空间中
    {
#if CONFIG_FLASH_ORIGIN_API
        ddev_write(flash_hdl, indata, dw_data_len, dw_data_addr);//先写三级目录的数据dw_thir_addr
#else
        bk_flash_write_bytes(dw_data_addr, (uint8_t *)indata, dw_data_len);
#endif
        name_data.dwlength = dw_data_len;
        name_data.dwOffsetByByte = dw_offset;
        name_data.dwUsingFlag = FLASH_SPACE_USING;
#if CONFIG_FLASH_ORIGIN_API
        ddev_write(flash_hdl, (char *)(&name_data), PER_NAME_FLASH_SIZE, dw_name_addr);//先写二级目录的数据
#else
        bk_flash_write_bytes(dw_name_addr, (uint8_t *)(&name_data), PER_NAME_FLASH_SIZE);
#endif
        //ddev_read(flash_hdl,(char *)(&name_temp_2), PER_NAME_FLASH_SIZE, dw_name_addr);//将缓存区中的数据读出来
        //bk_flash_debug_printf("[%s] [%d] %s [%u] {%u} [%x] \r\n", __FUNCTION__,__LINE__,name_temp_2.ucName,name_temp_2.dwlength,name_temp_2.dwOffsetByByte,name_temp_2.dwUsingFlag);
    }

#if CONFIG_FLASH_ORIGIN_API
    ddev_control(flash_hdl, CMD_FLASH_SET_PROTECT, &protect_flag);
#else
    bk_flash_set_protect_type(protect_type);
#endif
    return dw_rtn;
}

/*********************************************************************
 * Funtion Name:get_name_new_addr
 *
 * Funtion Discription:Get Flash data into global variables
 *
 * 
 *
 * 
 * Date:2021-12-22
 *******************************************************************/
uint32_t bk_set_name_data_new_addr( uint32_t data_sector_addr,uint32_t base_addr,char * indata,uint32_t dw_data_len,name_flash_t name_data)
{
#if CONFIG_FLASH_ORIGIN_API
    DD_HANDLE flash_hdl = 0;
    uint32_t status = 0,protect_param = 0,protect_flag = 0;
#endif
    uint32_t dw_loop = 0;
    name_flash_t *name_temp = NULL;
    char *data_buff = NULL;//data buf
    uint32_t data_offset_addr = 0;
    uint32_t dw_rtn = 0;
    uint32_t dw_mask_count = 0;
    //name_flash_t name_temp_2 = {0};
        
    name_temp = (name_flash_t *) os_malloc(NAME_FLASH_TOTAL_SIZE);
    data_buff = (char *) os_malloc(FLASH_SECTOR_SIZE);
    if ((NULL == name_temp) || (NULL == data_buff))
    {
        bk_printf("[%s] [%d] os_malloc fail \r\n", __FUNCTION__,__LINE__);
        return kGeneralErr;
    }
    memset((char *)name_temp,0,NAME_FLASH_TOTAL_SIZE);
    memset((char *)data_buff,0,FLASH_SECTOR_SIZE);

#if CONFIG_FLASH_ORIGIN_API
    flash_hdl = ddev_open(DD_DEV_TYPE_FLASH, (UINT32 *)&status, 0);
    ddev_control(flash_hdl, CMD_FLASH_GET_PROTECT, &protect_flag);
    protect_param = FLASH_PROTECT_NONE;
    ddev_control(flash_hdl, CMD_FLASH_SET_PROTECT, &protect_param);
    ddev_read(flash_hdl,(char *)name_temp, NAME_FLASH_TOTAL_SIZE, base_addr);
#else
    flash_protect_type_t protect_type;
    protect_type = bk_flash_get_protect_type();
    bk_flash_set_protect_type(FLASH_PROTECT_NONE);
    bk_flash_read_bytes(base_addr, (uint8_t *) name_temp, NAME_FLASH_TOTAL_SIZE);
#endif

    bk_flash_debug_printf("[%s] [%d]   [%u] {%u} \r\n", __FUNCTION__,__LINE__,base_addr,NAME_FLASH_TOTAL_SIZE);
    
    for(dw_loop = 0;dw_loop < FLASH_NAME_MAX_ARRAY;dw_loop++)
    {
        bk_flash_debug_printf("[%s] [%d]   [%u] {%x} \r\n", __FUNCTION__,__LINE__,dw_loop,name_temp[dw_loop].dwUsingFlag);
        //如果此时即不是被MASK或者被正在用的,表明这个空间可能是第一次使用时的数据，或者是为FLASH_SPACE_UNUSED的数据
        if((name_temp[dw_loop].dwUsingFlag != FLASH_SPACE_USING ) && (name_temp[dw_loop].dwUsingFlag != FLASH_SPACE_MASK))
        {
            bk_flash_debug_printf("[%s] [%d]   [%u] {%u} \r\n", __FUNCTION__,__LINE__,data_offset_addr,dw_data_len);
            if((data_offset_addr + dw_data_len) >= FLASH_SECTOR_SIZE) //如果当前的地址可用，判断当前的空间是否可以存贮下，如果存储不下，则进一步处理
            {
#if CONFIG_FLASH_ORIGIN_API
                ddev_read(flash_hdl,(char *)data_buff, FLASH_SECTOR_SIZE, data_sector_addr);//将缓存区中的数据读出来
#else
                bk_flash_read_bytes(data_sector_addr, (uint8_t *)data_buff, FLASH_SECTOR_SIZE);
#endif
                dw_rtn = bk_reorganize_name_data( base_addr,name_temp ,data_buff, data_sector_addr,indata,dw_data_len,name_data);//clean 二级目录的数据
                break;//跳出循环
            }
            else//如果能够存储下，则直接将数据写进去
            {
#if CONFIG_FLASH_ORIGIN_API
                ddev_write(flash_hdl, indata, dw_data_len, data_sector_addr + data_offset_addr);//先写三级目录的数据
#else
                bk_flash_write_bytes(data_sector_addr + data_offset_addr, (uint8_t *)(indata), dw_data_len);
#endif
                name_data.dwlength = dw_data_len;
                name_data.dwOffsetByByte = data_offset_addr;
                name_data.dwUsingFlag = FLASH_SPACE_USING;

#if CONFIG_FLASH_ORIGIN_API
                ddev_write(flash_hdl, (char *)&name_data, PER_NAME_FLASH_SIZE, base_addr + dw_loop * PER_NAME_FLASH_SIZE);//先写二级目录的数据
#else
                bk_flash_write_bytes(base_addr + dw_loop * PER_NAME_FLASH_SIZE, (uint8_t *)(&name_data), PER_NAME_FLASH_SIZE);
#endif
                
                //ddev_read(flash_hdl,(char *)(&name_temp_2), PER_NAME_FLASH_SIZE, base_addr + dw_loop * PER_NAME_FLASH_SIZE);//将缓存区中的数据读出来
                //bk_flash_debug_printf("[%s] [%d] %s %u %u %x \r\n", __FUNCTION__,__LINE__,name_temp_2.ucName,name_temp_2.dwlength,name_temp_2.dwOffsetByByte,name_temp_2.dwUsingFlag);
                break;//跳出循环
            }
        }
        else//如果是是被MASK或者被正在用的,则计算他们的偏移地址
        {
            data_offset_addr = name_temp[dw_loop].dwOffsetByByte + name_temp[dw_loop].dwlength;//给下一次的不是unused或者没有写过的数据使用偏移
            bk_flash_debug_printf("[%s] [%d] %u %u\r\n", __FUNCTION__,__LINE__,name_temp[dw_loop].dwOffsetByByte,name_temp[dw_loop].dwlength);
            bk_flash_debug_printf("[%s] [%d] %u %x\r\n", __FUNCTION__,__LINE__,data_offset_addr,name_temp[dw_loop].dwUsingFlag);
            
            if(name_temp[dw_loop].dwUsingFlag == FLASH_SPACE_MASK)//如果前面有MASK的则计数加一
            {
                dw_mask_count++;
            }
        }
    }

    if(dw_loop == FLASH_NAME_MAX_ARRAY)
    {   
        if(dw_mask_count > 0)//如果names的空间全部都满了,且前面还有被MASK掉的空间，则继续整理
        {
            bk_flash_debug_printf("[%s] [%d] %u %u %s \r\n", __FUNCTION__,__LINE__,data_sector_addr,dw_data_len,indata);

#if CONFIG_FLASH_ORIGIN_API
            ddev_read(flash_hdl,(char *)data_buff, FLASH_SECTOR_SIZE, data_sector_addr);//将缓存区中的数据读出来
#else
            bk_flash_read_bytes(data_sector_addr, (uint8_t *)data_buff, FLASH_SECTOR_SIZE);
#endif
            dw_rtn = bk_reorganize_name_data( base_addr,name_temp ,data_buff, data_sector_addr,indata,dw_data_len,name_data);//clean 二级目录的数据
        }
        else
        {
            bk_printf("[%s] [%d] set name fail \r\n", __FUNCTION__,__LINE__);
            dw_rtn = kGeneralErr;
        }
    }

    os_free(name_temp);
    os_free(data_buff);
    name_temp = NULL;
    data_buff = NULL;
#if CONFIG_FLASH_ORIGIN_API
    ddev_control(flash_hdl, CMD_FLASH_SET_PROTECT, &protect_flag);
#else
    bk_flash_set_protect_type(protect_type);
#endif

    return dw_rtn;
}

/*********************************************************************
 * Funtion Name:set_namespace_addr
 *
 * Funtion Discription:Get Flash data into global variables
 *
 * 
 *
 * 
 * Date:2021-12-22
 *******************************************************************/
uint32_t bk_set_namespace ( const char * ucnamespace,uint32_t np_base_addr,uint32_t *addr,uint32_t *block) //base_addr 要用上
{
    uint32_t dw_rtn = kNoErr ;
#if CONFIG_FLASH_ORIGIN_API
    DD_HANDLE flash_hdl = 0;
    uint32_t status = 0,protect_param = 0,protect_flag = 0;
#endif
    uint32_t dw_offset_addr = 0;
    uint32_t dw_loop = 0 ;
    //namespace_flash_t namespace_temp = {0};
    uint32_t dw_set_flag = NAME_SPACE_ADDR_SET_FAIL;
    namespace_flash_t *namespace_data_arr = NULL;

    BK_CHECK_POINTER_NULL(ucnamespace);
    namespace_data_arr = (namespace_flash_t *)os_malloc(NAME_SPACE_FLASH_ACTUAL_USE_SIZE);
    if(NULL == namespace_data_arr)
    {
        bk_printf("[%s] [%d] os_malloc failed \r\n", __FUNCTION__,__LINE__);
        return kGeneralErr;
    }
    memset((char *)namespace_data_arr,0,NAME_SPACE_FLASH_ACTUAL_USE_SIZE);
    
#if CONFIG_FLASH_ORIGIN_API
    flash_hdl = ddev_open(DD_DEV_TYPE_FLASH, (UINT32 *)&status, 0);
    ddev_control(flash_hdl, CMD_FLASH_GET_PROTECT, &protect_flag);
    protect_param = FLASH_PROTECT_NONE;
    ddev_control(flash_hdl, CMD_FLASH_SET_PROTECT, &protect_param);
    ddev_read(flash_hdl,(char *) namespace_data_arr, NAME_SPACE_FLASH_ACTUAL_USE_SIZE, np_base_addr);
#else
    flash_protect_type_t protect_type;
    protect_type = bk_flash_get_protect_type();
    bk_flash_set_protect_type(FLASH_PROTECT_NONE);
    bk_flash_read_bytes(np_base_addr, (uint8_t *) namespace_data_arr, NAME_SPACE_FLASH_ACTUAL_USE_SIZE);
#endif

    //not found
    for(dw_loop = 0;dw_loop < FLASH_NAME_SPACE_MAX_ARRAY;dw_loop++)
    {
        if(namespace_data_arr[dw_loop].dwUsingFlag != FLASH_SPACE_USING)
        {
            namespace_data_arr[dw_loop].dwUsingFlag = FLASH_SPACE_USING;
            namespace_data_arr[dw_loop].dwOffsetByByte = dw_loop * NAME_FLASH_TOTAL_SIZE + NAME_SPACE_FLASH_TOTAL_SIZE;
            memset(namespace_data_arr[dw_loop].ucNamespace, 0, sizeof(namespace_data_arr[dw_loop].ucNamespace));
            memcpy(namespace_data_arr[dw_loop].ucNamespace, ucnamespace, bk_strnlen(ucnamespace,sizeof(namespace_data_arr[dw_loop].ucNamespace) - 1));
            dw_set_flag = NAME_SPACE_ADDR_SET_SUCCESS;
            break;
        }
        bk_flash_debug_printf("[%s] [%d] namespace [%s ] \r\n", __FUNCTION__,__LINE__,namespace_data_arr[dw_loop].ucNamespace);
    }

    if(NAME_SPACE_ADDR_SET_SUCCESS == dw_set_flag)
    {
        dw_offset_addr = np_base_addr + dw_loop * PER_NAME_SPACE_FLASH_SIZE;
        
        *addr = namespace_data_arr[dw_loop].dwOffsetByByte;
        *block = dw_loop;

        bk_flash_debug_printf("[%s] [%d] np_base_addr is [%u] [%u]  \r\n", __FUNCTION__,__LINE__,dw_offset_addr,PER_NAME_SPACE_FLASH_SIZE);
#if CONFIG_FLASH_ORIGIN_API
        ddev_write(flash_hdl,(char *) &(namespace_data_arr[dw_loop]), PER_NAME_SPACE_FLASH_SIZE, dw_offset_addr);
#else
        bk_flash_write_bytes(dw_offset_addr, (uint8_t *)(&(namespace_data_arr[dw_loop])), PER_NAME_SPACE_FLASH_SIZE);
#endif
        //ddev_read(flash_hdl,(char *) (&namespace_temp), PER_NAME_SPACE_FLASH_SIZE, dw_offset_addr);
        //bk_flash_debug_printf("[%s] [%d] namespace  %u  %x [%s] \r\n", __FUNCTION__,__LINE__,namespace_temp.dwOffsetByByte,namespace_temp.dwUsingFlag,namespace_temp.ucNamespace);
    }
    else
    {
        bk_printf("[%s] [%d] namespace [%s ] set addr fail\r\n", __FUNCTION__,__LINE__,ucnamespace);
        dw_rtn = kGeneralErr;
    }

#if CONFIG_FLASH_ORIGIN_API
    ddev_control(flash_hdl, CMD_FLASH_SET_PROTECT, &protect_flag);
#else
    bk_flash_set_protect_type(protect_type);
#endif

    os_free(namespace_data_arr);
    namespace_data_arr = NULL;

    return dw_rtn;
}

/*********************************************************************
 * Funtion Name:set_name_addr
 *
 * Funtion Discription:Get Flash data into global variables
 *
 * 
 *
 * 
 * Date:2021-12-22
 *******************************************************************/
uint32_t bk_set_name_data( const char * ucnamespace,const char * ucname,uint32_t np_base_addr, char * indata,uint32_t dw_data_len,uint32_t dw_n_offset_addr,
                                   uint32_t dw_offset_block)
{
#if CONFIG_FLASH_ORIGIN_API
    DD_HANDLE flash_hdl = 0;
    uint32_t status = 0,protect_param = 0,protect_flag = 0;
#endif
    uint32_t dw_offset_addr = 0,dw_actual_addr = 0,dw_data_offset_addr = 0;
    uint32_t dw_sec_addr = 0;
    name_flash_t name_temp = {0};
    char data_buf[2048] = {0};//数据最大2048个字节, 以后会优化最快
    uint32_t dw_rtn = kNoErr;
    //uint32_t dw_sec_flag = FLASH_SPACE_MASK;

    BK_CHECK_POINTER_NULL(ucnamespace);
    BK_CHECK_POINTER_NULL(ucname);
    BK_CHECK_POINTER_NULL(indata);
    
    dw_actual_addr = np_base_addr + dw_n_offset_addr;//如果找到，继续找二级目录
    dw_sec_addr = dw_actual_addr;//二级目录的地址

    bk_flash_debug_printf("[%s] [%d] NO NAME SPACE[%u] ucnamespace:%s , ucname:%s \r\n", __FUNCTION__,__LINE__,dw_data_len, ucnamespace, ucname);
    bk_flash_debug_printf("[%s] [%d] NO NAME SPACE [%u] \r\n", __FUNCTION__,__LINE__,dw_actual_addr);

    if(kNoErr == get_name_addr(ucname, dw_actual_addr,&dw_offset_addr,&dw_data_offset_addr))//如果二级目录找到，将这个地址中的数据取出来
    {
        dw_actual_addr += dw_offset_addr;//找到对应二级目录对应的数据
        
#if CONFIG_FLASH_ORIGIN_API
        flash_hdl = ddev_open(DD_DEV_TYPE_FLASH, (UINT32 *)&status, 0);
        ddev_control(flash_hdl, CMD_FLASH_GET_PROTECT, &protect_flag);
        protect_param = FLASH_PROTECT_NONE;
        ddev_control(flash_hdl, CMD_FLASH_SET_PROTECT, &protect_param);
        ddev_read(flash_hdl,(char *) &name_temp, sizeof(name_temp), dw_actual_addr);
#else
        flash_protect_type_t protect_type;
        protect_type = bk_flash_get_protect_type();
        bk_flash_set_protect_type(FLASH_PROTECT_NONE);
        bk_flash_read_bytes(dw_actual_addr, (uint8_t *) &name_temp, sizeof(name_temp));
#endif
       
        dw_actual_addr = np_base_addr + dw_offset_block * FLASH_SECTOR_SIZE + NAME_FLASH_DATA_BASE_ADDR + dw_data_offset_addr ;
        
#if CONFIG_FLASH_ORIGIN_API
        ddev_read(flash_hdl,(char *) data_buf,name_temp.dwlength, dw_actual_addr);
#else
        bk_flash_read_bytes(dw_actual_addr, (uint8_t *) data_buf, name_temp.dwlength);
#endif
        bk_flash_debug_printf("[%s] [%d]  NAME  [%u] [%u] \r\n", __FUNCTION__,__LINE__,dw_data_len,name_temp.dwlength);

        if((name_temp.dwlength != dw_data_len) || (0 != memcmp(data_buf,indata,dw_data_len))) //长度不相等，切数据不一样，则重新找新的地址进行存储
        {
            name_temp.dwUsingFlag  =  FLASH_SPACE_MASK;
#if CONFIG_FLASH_ORIGIN_API
            ddev_write(flash_hdl,(char *) &name_temp, sizeof(name_temp), (dw_sec_addr + dw_offset_addr));//将这二级目录空间对应的Flag置位无效
            ddev_read(flash_hdl,(char *) &name_temp, sizeof(name_temp), (dw_sec_addr + dw_offset_addr));
#else
            bk_flash_write_bytes((dw_sec_addr + dw_offset_addr), (uint8_t *) &name_temp, sizeof(name_temp));
            bk_flash_read_bytes((dw_sec_addr + dw_offset_addr), (uint8_t *) &name_temp, sizeof(name_temp));
#endif

            bk_flash_debug_printf("[%s] [%d]  NAME   [%u] [%u]  \r\n", __FUNCTION__,__LINE__,dw_sec_addr,dw_offset_addr);
            bk_flash_debug_printf("[%s] [%d]  NAME   [%x] [%u] [%u] \r\n", __FUNCTION__,__LINE__,name_temp.dwUsingFlag,name_temp.dwOffsetByByte,name_temp.dwlength);
            dw_rtn = bk_set_name_data_new_addr( dw_actual_addr - dw_data_offset_addr, dw_sec_addr,indata,dw_data_len,name_temp);
        }
#if CONFIG_FLASH_ORIGIN_API
        ddev_control(flash_hdl, CMD_FLASH_SET_PROTECT, &protect_flag);
#else
        bk_flash_set_protect_type(protect_type);
#endif

    }
    else//如果没有找到,新建一个二级和三级目录
    {
        name_temp.dwlength = dw_data_len;
        name_temp.dwOffsetByByte = 0;
        name_temp.dwUsingFlag = FLASH_SPACE_USING;
        memset(name_temp.ucName,0,sizeof(name_temp.ucName));
        memcpy(name_temp.ucName,ucname,bk_strnlen(ucname,sizeof(name_temp.ucName) - 1));

        dw_actual_addr = np_base_addr + dw_offset_block * FLASH_SECTOR_SIZE + NAME_FLASH_DATA_BASE_ADDR; //数据所在的位置
        
        bk_flash_debug_printf("[%s] [%d]  NAME  [%u] [%u] \r\n", __FUNCTION__,__LINE__,dw_sec_addr,dw_offset_block);

        bk_flash_debug_printf("[%s] [%d]  NAME  [%u] [%u] \r\n", __FUNCTION__,__LINE__,dw_actual_addr,dw_data_len);
        
        dw_rtn = bk_set_name_data_new_addr( dw_actual_addr,dw_sec_addr,indata,dw_data_len,name_temp);
    }

    return dw_rtn;
}

/*********************************************************************
 * Funtion Name:bk_clean_data
 *
 * Funtion Discription:clean data in the flash 
 *
 * 
 *
 * 
 * Date:2021-12-22
 *******************************************************************/
uint32_t bk_clean_data ( const char * ucnamespace,const char * ucname)
{
#if CONFIG_FLASH_ORIGIN_API
    DD_HANDLE flash_hdl = 0;
    uint32_t status = 0,protect_param = 0,protect_flag = 0;
#endif
    uint32_t dw_offset_addr = 0,dw_actual_addr = 0,dw_offset_block = 0,dw_data_offset_addr = 0;
    //uint32_t dw_sec_addr = 0;
    uint32_t dw_rtn = kNoErr;
    uint32_t dw_sec_flag = FLASH_SPACE_MASK;
    uint32_t np_base_addr = 0;
    uint32_t dw_name_offset = 0;
    //name_flash_t name_temp = {0};

    BK_CHECK_POINTER_NULL(ucnamespace);
    BK_CHECK_POINTER_NULL(ucname);

    dw_rtn = get_matter_flash_base_addr(&np_base_addr);
    BK_CHECK_RETURN_VAULEL(dw_rtn);
    
#if CONFIG_FLASH_ORIGIN_API
    flash_hdl = ddev_open(DD_DEV_TYPE_FLASH, (UINT32 *)&status, 0);
#else
    flash_protect_type_t protect_type;
    protect_type = bk_flash_get_protect_type();
#endif
    if(kNoErr == get_namespace_addr(ucnamespace, np_base_addr,(uint32_t *)&dw_offset_addr,(uint32_t *)&dw_offset_block))//看是否在一级目录中找到对应namespace
    {
        dw_actual_addr = np_base_addr + dw_offset_addr;//如果找到，继续找二级目录
        //dw_sec_addr = dw_actual_addr;//二级目录的地址

        if(kNoErr == get_name_addr(ucname, dw_actual_addr,&dw_name_offset,&dw_data_offset_addr))//如果二级目录找到，将这个地址中的数据取出来
        {
            dw_actual_addr += dw_name_offset;//找到对应目录对应的数据
            bk_flash_debug_printf("[%s] [%d] [%u] [%u] [%u] \r\n", __FUNCTION__,__LINE__,dw_name_offset,dw_actual_addr);
#if CONFIG_FLASH_ORIGIN_API
            ddev_control(flash_hdl, CMD_FLASH_GET_PROTECT, &protect_flag);
            protect_param = FLASH_PROTECT_NONE;
            ddev_control(flash_hdl, CMD_FLASH_SET_PROTECT, &protect_param);
            //ddev_read(flash_hdl,(char *) &name_temp, sizeof(name_temp), dw_actual_addr);
            //bk_flash_debug_printf("[%s] [%d] [%s] [%u] [%u] [%u] [%u] \r\n", __FUNCTION__,__LINE__,name_temp.ucName,name_temp.dwUsingFlag,name_temp.dwOffsetByByte,name_temp.dwlength);
            ddev_write(flash_hdl,(char *)&dw_sec_flag, sizeof(dw_sec_flag), dw_actual_addr);//将这二级目录空间对应的Flag置位无效
            //ddev_read(flash_hdl,(char *) &name_temp, sizeof(name_temp), dw_actual_addr);
            //bk_flash_debug_printf("[%s] [%d] [%s] [%u] [%u] [%u] [%u] \r\n", __FUNCTION__,__LINE__,name_temp.ucName,name_temp.dwUsingFlag,name_temp.dwOffsetByByte,name_temp.dwlength);
#else
            bk_flash_set_protect_type(FLASH_PROTECT_NONE);
            bk_flash_write_bytes(dw_actual_addr, (uint8_t *) &dw_sec_flag, sizeof(dw_sec_flag));
#endif
        }
        else
        {
            bk_printf("[%s] [%d] No Such Name \r\n", __FUNCTION__,__LINE__);
            dw_rtn = kGeneralErr;
        }
    }
    else
    {
        bk_printf("[%s] [%d] No Such Namespace \r\n", __FUNCTION__,__LINE__);
        dw_rtn = kGeneralErr;
    }
    
#if CONFIG_FLASH_ORIGIN_API
    ddev_control(flash_hdl, CMD_FLASH_SET_PROTECT, &protect_flag);
#else
    bk_flash_set_protect_type(protect_type);
#endif

    return dw_rtn;
}

/*********************************************************************
 * Funtion Name:bK_clear_namespace
 *
 * Funtion Discription:Clear namespace
 *
 * 
 * 
 * Date:2021-12-22
 *******************************************************************/
uint32_t bK_clear_namespace ( const char * ucnamespace) //base_addr 要用上
{
    uint32_t dw_rtn = kNoErr ;

#if CONFIG_FLASH_ORIGIN_API
    DD_HANDLE flash_hdl = 0;
    uint32_t status = 0,protect_param = 0,protect_flag = 0;
#endif
    uint32_t dw_offset_addr = 0 ,dw_off_block = 0;
    uint32_t dw_start_addr = 0 ;
    uint32_t np_base_addr = 0;
    uint32_t data_sector_addr = 0;
    uint32_t dwLoop = 0;
    namespace_flash_t *namespace_temp = NULL;

    BK_CHECK_POINTER_NULL(ucnamespace);
    dw_rtn = get_matter_flash_base_addr(&np_base_addr);
    BK_CHECK_RETURN_VAULEL(dw_rtn);

    namespace_temp =(namespace_flash_t *) os_malloc(NAME_SPACE_FLASH_ACTUAL_USE_SIZE);
    if(NULL == namespace_temp)
    {
        bk_printf("[%s] [%d] os_malloc failed \r\n", __FUNCTION__,__LINE__);
        return kGeneralErr;
    }
    memset((char *)namespace_temp,0,NAME_SPACE_FLASH_ACTUAL_USE_SIZE);
    
    if(kNoErr == get_namespace_addr(ucnamespace, np_base_addr,&dw_offset_addr,&dw_off_block))//found namespace
    {
#if CONFIG_FLASH_ORIGIN_API
        flash_hdl = ddev_open(DD_DEV_TYPE_FLASH, (UINT32 *)&status, 0);
        ddev_control(flash_hdl, CMD_FLASH_GET_PROTECT, &protect_flag);
        protect_param = FLASH_PROTECT_NONE;
        ddev_control(flash_hdl, CMD_FLASH_SET_PROTECT, &protect_param);
        ddev_read(flash_hdl,(char *) namespace_temp, NAME_SPACE_FLASH_ACTUAL_USE_SIZE, np_base_addr);
#else
        flash_protect_type_t protect_type;
        protect_type = bk_flash_get_protect_type();
        bk_flash_set_protect_type(FLASH_PROTECT_NONE);
        bk_flash_write_bytes(np_base_addr, (uint8_t *) namespace_temp, NAME_SPACE_FLASH_ACTUAL_USE_SIZE);
#endif

        dw_start_addr = np_base_addr + NAME_SPACE_FLASH_TOTAL_SIZE + dw_off_block * NAME_FLASH_TOTAL_SIZE;
        data_sector_addr = np_base_addr + dw_off_block * FLASH_SECTOR_SIZE + NAME_FLASH_DATA_BASE_ADDR;//数据区域的地址
        bk_erase_name_data(dw_start_addr,data_sector_addr);//清除二级目录的数据,把二级目录和其对应的数据全部擦掉

#if CONFIG_FLASH_ORIGIN_API
        ddev_control(flash_hdl, CMD_FLASH_ERASE_SECTOR, &np_base_addr);//将一级目录对应的数据区erase掉
#else
        bk_flash_erase_sector(np_base_addr);
#endif
        namespace_temp[dw_off_block].dwUsingFlag = FLASH_SPACE_MASK;

        for(dwLoop = 0;dwLoop < FLASH_NAME_SPACE_MAX_ARRAY;dwLoop++)
        {
            if(FLASH_SPACE_USING == namespace_temp[dwLoop].dwUsingFlag)//如果该命名空间有效，则在写回去,改命名空间的位置不做改变
            {
#if CONFIG_FLASH_ORIGIN_API
                ddev_write(flash_hdl, (char *)(&namespace_temp[dwLoop]), PER_NAME_SPACE_FLASH_SIZE,np_base_addr + dwLoop * PER_NAME_SPACE_FLASH_SIZE);//先写一级目录的数据
#else
                bk_flash_write_bytes(np_base_addr + dwLoop * PER_NAME_SPACE_FLASH_SIZE, (uint8_t *)(&namespace_temp[dwLoop]), PER_NAME_SPACE_FLASH_SIZE);
#endif
            }
        }
#if CONFIG_FLASH_ORIGIN_API
        ddev_control(flash_hdl, CMD_FLASH_SET_PROTECT, &protect_flag);
#else
        bk_flash_set_protect_type(protect_type);
#endif
    }
    else
    {
        bk_printf("[%s] [%d] No Such Namespace \r\n", __FUNCTION__,__LINE__);
        dw_rtn = kGeneralErr;
    }
    os_free(namespace_temp);
    namespace_temp = NULL;

    return dw_rtn;
}

/*********************************************************************
 * Funtion Name:bk_read_data
 *
 * Funtion Discription:read data from flash 
 *
 * 
 *
 * 
 * Date:2021-12-22
 *******************************************************************/
uint32_t bk_read_data( const char * ucnamespace,const char * ucname,char * out_data,uint32_t dw_read_len,uint32_t *out_length)
{
    uint32_t dw_rtn = kNoErr;
#if CONFIG_FLASH_ORIGIN_API
    DD_HANDLE flash_hdl = 0;
    uint32_t status = 0,protect_param = 0,protect_flag = 0;
#endif
    uint32_t dw_offset_addr = 0,dw_off_block = 0;
    uint32_t dw_sec_base_addr = 0,dw_data_offset_addr = 0,dw_data_addr = 0;
    name_flash_t name_temp = {0};
    uint32_t np_base_addr = 0;
    uint32_t dw_actual_len = 0;

    BK_CHECK_POINTER_NULL(ucnamespace);
    BK_CHECK_POINTER_NULL(ucname);
    BK_CHECK_POINTER_NULL(out_data);
    BK_CHECK_POINTER_NULL(out_length);
    bk_flash_debug_printf("read %s.%s \r\n",ucnamespace,ucname);
    dw_rtn = get_matter_flash_base_addr(&np_base_addr);
    BK_CHECK_RETURN_VAULEL(dw_rtn);

    if(kNoErr == get_namespace_addr(ucnamespace, np_base_addr,&dw_offset_addr,&dw_off_block))//如果找到对应的namespace,继续找二级目录
    {
        dw_sec_base_addr = np_base_addr + dw_offset_addr;//获取二级目录的地址 
        if(kNoErr == get_name_addr(ucname, dw_sec_base_addr,&dw_offset_addr,&dw_data_offset_addr))//如果二级目录找到，将这个地址中的数据取出来
        {
            bk_flash_debug_printf("[%s] [%d] read  [%u] [%u] [%u] \r\n", __FUNCTION__,__LINE__,dw_sec_base_addr,dw_offset_addr,dw_data_offset_addr);
            dw_data_addr = dw_sec_base_addr + dw_offset_addr;///找到对应二级目录块存在的位置
#if CONFIG_FLASH_ORIGIN_API
            flash_hdl = ddev_open(DD_DEV_TYPE_FLASH, (UINT32 *)&status, 0);
            ddev_control(flash_hdl, CMD_FLASH_GET_PROTECT, &protect_flag);
            protect_param = FLASH_PROTECT_NONE;
            ddev_control(flash_hdl, CMD_FLASH_SET_PROTECT, &protect_param);
            ddev_read(flash_hdl,(char *) &name_temp, sizeof(name_temp), dw_data_addr);
#else
            flash_protect_type_t protect_type;
            protect_type = bk_flash_get_protect_type();
            bk_flash_set_protect_type(FLASH_PROTECT_NONE);
            bk_flash_read_bytes(dw_data_addr, (uint8_t *) &name_temp, sizeof(name_temp));
#endif
            dw_actual_len = ((dw_read_len > name_temp.dwlength) ? name_temp.dwlength : dw_read_len);//取最小的长度读数据
            *out_length = dw_actual_len;
            
            dw_data_addr = np_base_addr + dw_off_block * FLASH_SECTOR_SIZE + NAME_FLASH_DATA_BASE_ADDR + dw_data_offset_addr ;;//找到对应二级目录中对应数据的位置
#if CONFIG_FLASH_ORIGIN_API
            ddev_read(flash_hdl,(char *) out_data, dw_actual_len, dw_data_addr);//读取的长度有外面的变量传进来
#else
            bk_flash_read_bytes(dw_data_addr, (uint8_t *) out_data, dw_actual_len);
#endif
            bk_flash_debug_printf("[%s] [%d] read [%s] [%u] [%u] \r\n", __FUNCTION__,__LINE__,out_data,dw_data_addr,dw_actual_len);

#if CONFIG_FLASH_ORIGIN_API
            ddev_control(flash_hdl, CMD_FLASH_SET_PROTECT, &protect_flag);
#else
            bk_flash_set_protect_type(protect_type);
#endif
        }
		else//表示没有找到name
		{
			dw_rtn = kGeneralErr;
            bk_flash_debug_printf("[%s] [%d] NO NAME [%u] [%u] [%u] \r\n", __FUNCTION__,__LINE__,dw_sec_base_addr,dw_offset_addr,dw_data_offset_addr);
		}
    }
    else//表示没有找到namespace
    {
        dw_rtn = kGeneralErr;
        bk_flash_debug_printf("[%s] [%d] NO NAME SPACE [%u] [%u] [%u] \r\n", __FUNCTION__,__LINE__,np_base_addr,dw_offset_addr,dw_off_block);
    }

    return dw_rtn;
}

/*********************************************************************
 * Funtion Name:bk_write_data
 *
 * Funtion Discription:read data from flash 
 *
 * 
 *
 * 
 * Date:2021-12-22
 *******************************************************************/
uint32_t bk_write_data( const char * ucnamespace,const char * ucname, char * indata,uint32_t data_size)
{
    uint32_t dw_rtn = kNoErr;
    uint32_t dw_offset_addr = 0,dw_off_block = 0;
    uint32_t np_base_addr = 0;

    BK_CHECK_POINTER_NULL(ucnamespace);
    BK_CHECK_POINTER_NULL(ucname);
    BK_CHECK_POINTER_NULL(indata);
    bk_flash_debug_printf("write %s.%s \r\n",ucnamespace,ucname);

    dw_rtn = get_matter_flash_base_addr(&np_base_addr);
    BK_CHECK_RETURN_VAULEL(dw_rtn);
    bk_flash_debug_printf("[%s] [%d] np_base_addr is [%u] \r\n", __FUNCTION__,__LINE__,np_base_addr);
    if(kNoErr != get_namespace_addr(ucnamespace, np_base_addr,&dw_offset_addr,&dw_off_block))
    {
        bk_flash_debug_printf("[%s] [%d] np_base_addr is [%u] [%u] [%u] \r\n", __FUNCTION__,__LINE__,np_base_addr,dw_offset_addr,dw_off_block);
        dw_rtn = bk_set_namespace(ucnamespace, np_base_addr,&dw_offset_addr,&dw_off_block);
        BK_CHECK_RETURN_VAULEL(dw_rtn);
    }

    bk_flash_debug_printf("[%s] [%d] np_base_addr is [%u] [%u] [%u] data_size:%d \r\n", __FUNCTION__,__LINE__,np_base_addr,dw_offset_addr,dw_off_block, data_size);
    dw_rtn = bk_set_name_data(ucnamespace, ucname, np_base_addr, indata,data_size,dw_offset_addr,dw_off_block);
    BK_CHECK_RETURN_VAULEL(dw_rtn);


    return dw_rtn;
}

/*********************************************************************
 * Funtion Name:bk_ensure_namespace
 * Funtion Discription:check namespace exit or not
 *
 * 
 *
 * 
 * Date:2021-12-22
 *******************************************************************/
uint32_t bk_ensure_namespace( const char * ucnamespace)
{
    uint32_t dw_rtn = kGeneralErr;
    uint32_t dw_offset_addr = 0,dw_off_block = 0;
    uint32_t np_base_addr = 0;

    BK_CHECK_POINTER_NULL(ucnamespace);
    dw_rtn = get_matter_flash_base_addr(&np_base_addr);
    BK_CHECK_RETURN_VAULEL(dw_rtn);
    bk_flash_debug_printf("check for %s \r\n",ucnamespace);
    if(kNoErr == get_namespace_addr(ucnamespace, np_base_addr,&dw_offset_addr,&dw_off_block))//如果找到对应的namespace,继续找二级目录
    {
        bk_flash_debug_printf("check %s find addr = %d \r\n",ucnamespace,dw_offset_addr);
        dw_rtn = kNoErr;
    }
    else
    {
        if(kNoErr == bk_set_namespace ( ucnamespace, np_base_addr,&dw_offset_addr,&dw_off_block))
        {
            dw_rtn = kNoErr;
        }
        else
        {
            dw_rtn = kGeneralErr;
        }
        bk_flash_debug_printf("check %s set addr = %d \r\n",ucnamespace,dw_offset_addr);
    }

    return dw_rtn;
}
/*********************************************************************
 * Funtion Name:bk_ensure_name_data
 *
 * Funtion Discription:check data exit or not
 *
 * 
 *
 * 
 * Date:2021-12-22
 *******************************************************************/
uint32_t bk_ensure_name_data( const char * ucnamespace,const char * ucname)
{
    uint32_t dw_rtn = kGeneralErr;
    uint32_t dw_offset_addr = 0,dw_off_block = 0;
    uint32_t np_base_addr = 0;
    uint32_t dw_actual_addr = 0;
    uint32_t dw_data_offset_addr = 0;

    BK_CHECK_POINTER_NULL(ucnamespace);
    dw_rtn = get_matter_flash_base_addr(&np_base_addr);
    BK_CHECK_RETURN_VAULEL(dw_rtn);
    bk_flash_debug_printf("check for %s.%s \r\n",ucnamespace,ucname);

    if(kNoErr == get_namespace_addr(ucnamespace, np_base_addr,&dw_offset_addr,&dw_off_block))//如果找到对应的namespace,继续找二级目录
    {
        dw_actual_addr = np_base_addr + dw_offset_addr;//如果找到，继续找二级目录
        bk_flash_debug_printf("check %s.%s.b_addr=0x%x \r\n",ucnamespace,ucname,dw_actual_addr);
        if(kNoErr == get_name_addr(ucname, dw_actual_addr,&dw_offset_addr,&dw_data_offset_addr))
        {
            os_printf("find addr = dw_offset_addr \r\n ");
            dw_rtn = kNoErr;
        }
        else
        {
            os_printf("find fail\r\n ");
            dw_rtn = kGeneralErr;
        }
    }
    else
    {
        bk_flash_debug_printf("check fail\r\n ");
        dw_rtn = kGeneralErr;
    }

    return dw_rtn;
}

/*********************************************************************
 * Funtion Name:bk_erase_all
 *
 * Funtion Discription:Erase all Matter Flash data
 * 
 *
 * 
 * Date:2022-02-07
 *******************************************************************/
uint32_t bk_erase_all( bk_partition_t type )
{
#if CONFIG_FLASH_ORIGIN_API
    DD_HANDLE flash_hdl = 0;
    uint32_t status = 0,protect_param = 0,protect_flag = 0;
#endif
    uint32_t flash_occu_length = 0;
    uint32_t operation_addr = 0;
    uint32_t dw_loop = 0;
    bk_logic_partition_t *partition_info = NULL;
    
#if CONFIG_FLASH_ORIGIN_API
    partition_info = bk_flash_get_info(type);
#else
    partition_info = bk_flash_partition_get_info(type);
#endif
    BK_CHECK_POINTER_NULL(partition_info);

    operation_addr = partition_info->partition_start_addr;
    flash_occu_length = partition_info->partition_length;

#if CONFIG_FLASH_ORIGIN_API
    flash_hdl = ddev_open(DD_DEV_TYPE_FLASH, (UINT32 *)&status, 0);
    ddev_control(flash_hdl, CMD_FLASH_GET_PROTECT, &protect_flag);
    protect_param = FLASH_PROTECT_NONE;
    ddev_control(flash_hdl, CMD_FLASH_SET_PROTECT, &protect_param);
#else
    flash_protect_type_t protect_type;
    protect_type = bk_flash_get_protect_type();
    bk_flash_set_protect_type(FLASH_PROTECT_NONE);
#endif

    for(dw_loop = 0;dw_loop < flash_occu_length / NAME_SPACE_FLASH_TOTAL_SIZE;dw_loop++)
    {
#if CONFIG_FLASH_ORIGIN_API
        ddev_control(flash_hdl, CMD_FLASH_ERASE_SECTOR, &operation_addr);
#else
        bk_flash_erase_sector(operation_addr);
#endif
        operation_addr += NAME_SPACE_FLASH_TOTAL_SIZE;
    }

#if CONFIG_FLASH_ORIGIN_API
    ddev_control(flash_hdl, CMD_FLASH_SET_PROTECT, &protect_flag);
#else
    bk_flash_set_protect_type(protect_type);
#endif

    return kNoErr;
}

/*********************************************************************
 * Funtion Name:bk_write_ota_data_to_flash
 *
 * Funtion Discription:write ota data to flash 
 *
 * 
 *
 * 
 * Date:2022-02-10
 *******************************************************************/
uint32_t bk_write_ota_data_to_flash( char * indata,uint32_t data_offset_addr,uint32_t data_length)
{
    uint32_t dw_rtn = kNoErr;

    BK_CHECK_POINTER_NULL(indata);
    
    bk_flash_debug_printf("[%s] [%d]  NAME  [%u] [%u] \r\n", __FUNCTION__,__LINE__,data_offset_addr,data_length);
#if (CONFIG_FLASH_ORIGIN_API)
    dw_rtn = bk_flash_write( BK_PARTITION_OTA, data_offset_addr, (uint8_t *)indata , data_length);
#else
    bk_logic_partition_t *pt = bk_flash_partition_get_info(BK_PARTITION_OTA);
    dw_rtn = bk_flash_write_bytes((pt->partition_start_addr + data_offset_addr), (uint8_t *)indata, data_length);
#endif

    return dw_rtn;
}

/*********************************************************************
 * Funtion Name:bk_read_ota_data_in_flash
 *
 * Funtion Discription:read ota data in flash 
 *
 * 
 *
 * 
 * Date:2022-02-10
 *******************************************************************/
uint32_t bk_read_ota_data_in_flash( char * outdata,uint32_t data_offset_addr,uint32_t data_length)
{
    uint32_t dw_rtn = kNoErr;

    BK_CHECK_POINTER_NULL(outdata);
#if (CONFIG_FLASH_ORIGIN_API)
    dw_rtn = bk_flash_read( BK_PARTITION_OTA, data_offset_addr, (uint8_t *)outdata, data_length);
 #else
    bk_logic_partition_t *pt = bk_flash_partition_get_info(BK_PARTITION_OTA);
    dw_rtn = bk_flash_read_bytes((pt->partition_start_addr + data_offset_addr), (uint8_t *)outdata, data_length);
#endif
   
    return dw_rtn;
}

/*********************************************************************
 * Funtion Name:bk_erase_ota_data_in_flash
 *
 * Funtion Discription:erase ota data in flash 
 *
 * 
 *
 * 
 * Date:2022-02-10
 *******************************************************************/
uint32_t bk_erase_ota_data_in_flash( )
{
    uint32_t dw_rtn = kNoErr;
    
    dw_rtn = bk_erase_all( BK_PARTITION_OTA);
    
    return dw_rtn;
}

#if 1
/*********************************************************************
 * Funtion Name:test bk_flash write 
 *
 * Funtion Discription:check data exit or not
 *
 * 
 *
 * 
 * Date:2021-12-22
 *******************************************************************/
void bk_write_data_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv )
{
    uint32_t dwRtn = 0;
    uint32_t dwLoop = 0;

    name_flash_test_t temp_value[] = { \
        {"bekenspace1" ,"beken_name1" ,"1188888888888811",0}, \
        {"bekenspace2" ,"beken_name2" ,"2288888888888822",0}, \
        {"bekenspace3" ,"beken_name3" ,"3388888888888833",0},};

    name_flash_test_t temp_value1[] = { \
        {"bekenspace1" ,"beken_name1" ,"1188888888888811909090",0}, };


    uint32_t dwDataLen = sizeof(temp_value) / sizeof(temp_value[0]);
    uint32_t dwDataLen1 = sizeof(temp_value1) / sizeof(temp_value1[0]);

    uint32_t dwData = 0;
    char cmd0 = 0;
    char cmd1 = 0;

    for(dwLoop = 0; dwLoop < argc; dwLoop++)
    {
        bk_printf("Argument %d = %s\r\n", dwLoop + 1, argv[dwLoop]);
    }

    if(argc == 2)
    {
        cmd0 = argv[1][0] - 0x30;
        cmd1 = argv[1][1] - 0x30;

        dwData = (uint32_t)(cmd0 * 10 + cmd1);
        bk_printf("---%u \r\n", dwData);
    }
    else
    {
        bk_printf("cmd param error \r\n");
        return ;
    }

    if(dwData == 0)
    {
        for(dwLoop = 0;dwLoop < dwDataLen;dwLoop++)
        {
            dwRtn = bk_write_data( temp_value[dwLoop].ucNamespace,temp_value[dwLoop].ucName, temp_value[dwLoop].ucData,strlen(temp_value[dwLoop].ucData));
            BK_CHECK_NO_RETURN(dwRtn);
        }
    }
    else
    {
        for(dwLoop = 0;dwLoop < dwDataLen1;dwLoop++)
        {
            dwRtn = bk_write_data( temp_value1[dwLoop].ucNamespace,temp_value1[dwLoop].ucName, temp_value1[dwLoop].ucData,strlen(temp_value1[dwLoop].ucData));
            BK_CHECK_NO_RETURN(dwRtn);
        }
    }

    return ;
}

/*********************************************************************
 * Funtion Name:test bk_flash read
 *
 * Funtion Discription:check data exit or not
 *
 * 
 *
 * 
 * Date:2021-12-22
 *******************************************************************/
void bk_read_data_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    uint32_t dwRtn = 0;
    uint32_t dwLoop = 0;
    uint32_t dwOut_length = 0;
    name_flash_test_t temp_value[10] = {        \
        {"bekenspace1" ,"beken_name1" ,"",16},\
        {"bekenspace2" ,"beken_name2" ,"",16},\
        {"bekenspace3" ,"beken_name3" ,"",16},};

    uint32_t dwDataLen = sizeof(temp_value) / sizeof(temp_value[0]);
    uint32_t dwData = 0;
    char cmd0 = 0;
    char cmd1 = 0;

    for(dwLoop = 0; dwLoop < argc; dwLoop++)
    {
        bk_printf("Argument %d = %s\r\n", dwLoop + 1, argv[dwLoop]);
    }

    if(argc == 2)
    {
        cmd0 = argv[1][0] - 0x30;
        cmd1 = argv[1][1] - 0x30;

        dwData = (uint32_t)(cmd0 * 10 + cmd1);
        bk_printf("---%u \r\n", dwData);
    }
    else
    {
        bk_printf("cmd param error \r\n");
        return ;
    }

    if(dwData >= dwDataLen)
    {
        bk_printf("dwData [%u] is overflow,the max is %u \r\n",dwData,dwDataLen);
        return ;
    }

    dwRtn = bk_read_data(temp_value[dwData].ucNamespace,temp_value[dwData].ucName, temp_value[dwData].ucData, 50 ,&dwOut_length);
    BK_CHECK_NO_RETURN(dwRtn);
    
    bk_printf("Namespace is [%s] \r\n",temp_value[dwData].ucNamespace);

    bk_printf("Name is [%s] \r\n",temp_value[dwData].ucName);

    bk_printf("Data is [%s] \r\n",temp_value[dwData].ucData);

    bk_printf("Length is [%u] \r\n",dwOut_length);

    return ;
}

/*********************************************************************
 * Funtion Name:test bk_erase_namespace_test
 *
 * Funtion Discription:check data exit or not
 *
 * 
 *
 * 
 * Date:2021-12-22
 *******************************************************************/
void bk_erase_namespace_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    uint32_t dwRtn = 0;
    uint32_t dwLoop = 0;
    name_flash_test_t temp_value[10] = {            \
        {"bekenspace1" ,"beken_name1" ,"1188888888888811",16}, \
        {"bekenspace2" ,"beken_name2" ,"2288888888888822",16}, \
        {"bekenspace3" ,"beken_name3" ,"3388888888888833",16},};

    uint32_t dwDataLen = sizeof(temp_value) / sizeof(temp_value[0]);
    uint32_t dwData = 0;
    char cmd0 = 0;
    char cmd1 = 0;

    for(dwLoop = 0; dwLoop < argc; dwLoop++)
    {
        bk_printf("Argument %d = %s\r\n", dwLoop + 1, argv[dwLoop]);
    }

    if(argc == 2)
    {
        cmd0 = argv[1][0] - 0x30;
        cmd1 = argv[1][1] - 0x30;

        dwData = (uint32_t)(cmd0 * 10 + cmd1);
        bk_printf("---%u \r\n", dwData);
    }
    else
    {
        bk_printf("cmd param error \r\n");
        return ;
    }

    if(dwData >= dwDataLen)
    {
        bk_printf("dwData [%u] is overflow,the max is %u \r\n",dwData,dwDataLen);
        return ;
    }
    
    dwRtn =  bK_clear_namespace(temp_value[dwData].ucNamespace);
    BK_CHECK_NO_RETURN(dwRtn);

    return  ;
}

/*********************************************************************
 * Funtion Name:bk_erase_name_test
 *
 * Funtion Discription:check data exit or not
 *
 * 
 *
 * 
 * Date:2021-12-22
 *******************************************************************/
void bk_erase_name_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    name_flash_test_t temp_value[10] = {            \
        {"bekenspace1" ,"beken_name1" ,"1188888888888811",16}, \
        {"bekenspace2" ,"beken_name2" ,"2288888888888822",16}, \
        {"bekenspace3" ,"beken_name3" ,"3388888888888833",16},};

    uint32_t dwDataLen = sizeof(temp_value) / sizeof(temp_value[0]);
    uint32_t dwRtn = 0;
    uint32_t dwData = 0;
    uint32_t dwLoop = 0;
    char cmd0 = 0;
    char cmd1 = 0;

    for(dwLoop = 0; dwLoop < argc; dwLoop++)
    {
        bk_printf("Argument %d = %s\r\n", dwLoop + 1, argv[dwLoop]);
    }

    if(argc == 2)
    {
        cmd0 = argv[1][0] - 0x30;
        cmd1 = argv[1][1] - 0x30;

        dwData = (uint32_t)(cmd0 * 10 + cmd1);
        bk_printf("---%u \r\n", dwData);
    }
    else
    {
        bk_printf("cmd param error \r\n");
        return ;
    }
    
    if(dwData >= dwDataLen)
    {
        bk_printf("dwData [%u] is overflow,the max is %u \r\n",dwData,dwDataLen);
        return ;
    }

    dwRtn = bk_clean_data ( temp_value[dwData].ucNamespace,temp_value[dwData].ucName);
    BK_CHECK_NO_RETURN(dwRtn);

    return;
}

/*********************************************************************
 * Funtion Name:bk_erase_name_test
 *
 * Funtion Discription:check data exit or not
 *
 * 
 *
 * 
 * Date:2021-12-22
 *******************************************************************/
void  bk_name_data_exits(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    name_flash_test_t temp_value[10] = {            \
        {"bekenspace1" ,"beken_name1" ,"1188888888888811",16},\
        {"bekenspace2" ,"beken_name2" ,"2288888888888822",16},\
        {"bekenspace3" ,"beken_name3" ,"3388888888888833",16},};

    uint32_t dwDataLen = sizeof(temp_value) / sizeof(temp_value[0]);
    uint32_t dwRtn = 0;
    uint32_t dwData = 0;
    uint32_t dwLoop = 0;
    char cmd0 = 0;
    char cmd1 = 0;

    for(dwLoop = 0; dwLoop < argc; dwLoop++)
    {
        bk_printf("Argument %d = %s\r\n", dwLoop + 1, argv[dwLoop]);
    }

    if(argc == 2)
    {
        cmd0 = argv[1][0] - 0x30;
        cmd1 = argv[1][1] - 0x30;

        dwData = (uint32_t)(cmd0 * 10 + cmd1);
        bk_printf("---%u \r\n", dwData);
    }
    else
    {
        bk_printf("cmd param error \r\n");
        return ;
    }

    if(dwData >= dwDataLen)
    {
        bk_printf("dwData [%u] is overflow,the max is %u \r\n",dwData,dwDataLen);
        return ;
    }

    dwRtn = bk_ensure_name_data( temp_value[dwData].ucNamespace,temp_value[dwData].ucName);

    if (kNoErr ==dwRtn)
    {
        bk_printf("Namespace [%s] Name [%s] exits \r\n",temp_value[dwData].ucNamespace,temp_value[dwData].ucName);
    }
    else
    {
        bk_printf("Namespace [%s] Name [%s] is not exits \r\n",temp_value[dwData].ucNamespace,temp_value[dwData].ucName);
    }

    return ;
}
/*********************************************************************
 * Funtion Name:bk_erase_all_test
 *
 * Funtion Discription:Erase all Matter Flash data
 *
 * 
 *
 * 
 * Date:2021-12-22
 *******************************************************************/
void  bk_erase_all_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    uint32_t dwRtn = 0;

    dwRtn = bk_erase_all(BK_PARTITION_MATTER_FLASH);

    if (kNoErr ==dwRtn)
    {
        bk_printf("bk_erase_all_test succeed \r\n");
    }
    else
    {
        bk_printf("bk_erase_all_test fialed \r\n");
    }

    return ;
}

#endif
#if 0
extern struct wpa_supplicant *wpa_suppliant_ctrl_get_wpas();
/*********************************************************************
 * Funtion Name:bk_wifi_info_read
 *
 * Funtion Discription:read wifi data from flash 
 *
 * 
 *
 * 
 * Date:2021-12-22
 *******************************************************************/
uint32_t wpa_ssid_key_get(ssid_key_save_t *wpas_get)
{    
    struct wpa_supplicant *wpa_s = NULL;

    if(wpas_get == NULL)
    {
        bk_printf("%s %d the pointer is null \r\n",__FUNCTION__,__LINE__);
        return 1;
    }

    wpa_s = wpa_suppliant_ctrl_get_wpas();
    memcpy((char*)wpas_get->ucssid, wpa_s->current_ssid->ssid, wpa_s->current_ssid->ssid_len);
    strcpy((char*)wpas_get->ucKey, wpa_s->current_ssid->passphrase);
    return 0;
}
#endif
