#include "at_common.h"
//#if (CONFIG_BLE_5_X || CONFIG_BTDM_5_2)
const at_command_t *lookup_ble_at_command(char *str1)
{
    uint8_t type = bk_ble_get_controller_stack_type();

    if(type != BK_BLE_CONTROLLER_STACK_TYPE_BLE_5_X &&
        type != BK_BLE_CONTROLLER_STACK_TYPE_BTDM_5_2)
    {
        os_printf("%s stack type %d not support\n", __func__, type);
        return NULL;
    }

    for (int i = 0; i < ble_at_cmd_cnt(); i++)
    {
        if (ble_at_cmd_table[i].name == NULL) {
            i++;
            continue;
        }

        if(!os_strcmp(ble_at_cmd_table[i].name, str1))
        {
            return &ble_at_cmd_table[i];
        }
    }
    return NULL;
}
#if CONFIG_LWIP
const at_command_t *lookup_wifi_at_command(char *str1)
{

    for (int i = 0; i < wifi_at_cmd_cnt(); i++)
    {
        if (wifi_at_cmd_table[i].name == NULL) {
            i++;
            continue;
        }

        if(!os_strcmp(wifi_at_cmd_table[i].name, str1))
        {
            return &wifi_at_cmd_table[i];
        }
    }
    return NULL;
}
#endif

void at_set_data_handle(uint8_t *out, char *buff, uint8_t len)
{
    char temp[3];

    int i = 0, j = 0;
    for (i = 0; i < len;)
    {
        os_memcpy(temp, buff + i, 2);
        temp[2] = '\0';
        i = i + 2;
        out[j++] = os_strtoul(temp, NULL, 16) & 0xFF;
    }
}

int get_addr_from_param(bd_addr_t *bdaddr, char *input_param)
{
    int err = kNoErr;
    uint8_t addr_len = os_strlen(input_param);
    char temp[3];
    uint8_t j = 1;
    uint8_t k = 0;

    if ( addr_len != (BK_BLE_GAP_BD_ADDR_LEN * 2 + 5))
    {
        err = kParamErr;
        return err;
    }

    for (uint8_t i = 0; i < addr_len; i++)
    {
        if (input_param[i] >= '0' && input_param[i] <= '9')
        {
            temp[k] = input_param[i];
            k += 1;
        }
        else if (input_param[i] >= 'a' && input_param[i] <= 'f')
        {
            temp[k] = input_param[i];
            k += 1;
        }
        else if (input_param[i] >= 'A' && input_param[i] <= 'F')
        {
            temp[k] = input_param[i];
            k += 1;
        }
        else if (input_param[i] == ':')
        {
            temp[k] = '\0';
            bdaddr->addr[BK_BLE_GAP_BD_ADDR_LEN - j] = os_strtoul(temp, NULL, 16) & 0xFF;
            k = 0;
            j++;
        }
        else
        {
            err = kParamErr;
            return err;
        }

        if (i == (addr_len - 1))
        {
            temp[k] = '\0';
            bdaddr->addr[BK_BLE_GAP_BD_ADDR_LEN - j] = os_strtoul(temp, NULL, 16) & 0xFF;
            k = 0;
        }
    }

    return kNoErr;
}

//#endif

const at_command_t *lookup_video_at_command(char *str1)
{
    for (int i = 0; i < video_at_cmd_cnt(); i++)
    {
        if (video_at_cmd_table[i].name == NULL) {
            i++;
            continue;
        }

        if(!os_strcmp(video_at_cmd_table[i].name, str1))
        {
            return &video_at_cmd_table[i];
        }
    }
    return NULL;
}

